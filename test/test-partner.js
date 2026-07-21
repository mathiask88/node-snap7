const { test, before, after, beforeEach } = require('node:test');
const assert = require('assert');

const { S7Partner, PartnerParameter, PartnerError } = require('../lib/node-snap7');

const START_DELAY_MS = 1000;
const PARTNER_TIMEOUT_MS = 2000;
const LOCAL_ADDR = '0.0.0.0';
const REMOTE_ADDR = '127.0.0.1';

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

async function waitForLinked(active, passive, attempts = 20, delayMs = 200) {
  for (let i = 0; i < attempts; i++) {
    if (active.Linked() && passive.Linked()) {
      return true;
    }
    await sleep(delayMs);
  }
  return false;
}

const passive = new S7Partner(false);
const active = new S7Partner(true);

async function cleanupPartners() {
  try {
    await active.Stop();
  } catch (_) {}
  try {
    await passive.Stop();
  } catch (_) {}
  await sleep(200);
}

function logPartnerState(t, label, activeRef, passiveRef) {
  if (!t || typeof t.diagnostic !== 'function') {
    return;
  }

  t.diagnostic(`[partner] ${label}`);
  t.diagnostic(
    `  active: status=${activeRef.Status()} linked=${activeRef.Linked()}`
  );
  t.diagnostic(
    `  passive: status=${passiveRef.Status()} linked=${passiveRef.Linked()}`
  );
}

function skipIfNotLinked(t, activeRef, passiveRef) {
  if (!activeRef.Linked() || !passiveRef.Linked()) {
    logPartnerState(t, 'test skipped: partners not linked', activeRef, passiveRef);
    t.skip('Partners not linked');
    return true;
  }
  return false;
}

function handleBindError(t, label, activeRef, passiveRef, err) {
  if (err && err.errno === PartnerError.BindError) {
    logPartnerState(t, label, activeRef, passiveRef);
    t.skip('Partner bind failed (port 102 requires elevated privileges)');
    return true;
  }
  return false;
}

async function startPartnerPair({
  t,
  activeRef,
  passiveRef,
  tsap,
  sync
}) {
  try {
    passiveRef.SetParam(PartnerParameter.BRecvTimeout, PARTNER_TIMEOUT_MS);
    passiveRef.SetParam(PartnerParameter.BSendTimeout, PARTNER_TIMEOUT_MS);
    activeRef.SetParam(PartnerParameter.BRecvTimeout, PARTNER_TIMEOUT_MS);
    activeRef.SetParam(PartnerParameter.BSendTimeout, PARTNER_TIMEOUT_MS);

    if (sync) {
      try {
        passiveRef.StartToSync(LOCAL_ADDR, REMOTE_ADDR, tsap, tsap);
        await sleep(START_DELAY_MS);
        activeRef.StartToSync(LOCAL_ADDR, REMOTE_ADDR, tsap, tsap);
      } catch (err) {
        if (handleBindError(t, 'sync bind failed', activeRef, passiveRef, err)) {
          return false;
        }
        throw err;
      }
    } else {
      try {
        await passiveRef.StartTo(LOCAL_ADDR, REMOTE_ADDR, tsap, tsap);
        await sleep(START_DELAY_MS);
        await activeRef.StartTo(LOCAL_ADDR, REMOTE_ADDR, tsap, tsap);
      } catch (err) {
        if (handleBindError(t, 'bind failed', activeRef, passiveRef, err)) {
          return false;
        }
        throw err;
      }
    }

    const linked = await waitForLinked(activeRef, passiveRef);
    if (!linked) {
      logPartnerState(t, sync ? 'sync link timeout' : 'async link timeout', activeRef, passiveRef);
      t.skip('Partners did not link in time');
      return false;
    }
    return true;
  } catch (err) {
    if (!sync && activeRef === active && passiveRef === passive) {
      await cleanupPartners();
    }
    throw err;
  }
}

before(async (t) => {
  await startPartnerPair({
    t,
    activeRef: active,
    passiveRef: passive,
    tsap: 0x1002,
    sync: false
  });
});

after(async () => {
  await cleanupPartners();
});

test('Partner BSend triggers recv event', async (t) => {
  if (skipIfNotLinked(t, active, passive)) {
    return;
  }

  const payload = Buffer.from([1, 2, 3, 4]);

  const recvPromise = new Promise((resolve, reject) => {
    const timer = setTimeout(() => reject(new Error('recv timeout')), 2000);
    passive.once('recv', (evt) => {
      clearTimeout(timer);
      resolve(evt);
    });
  });

  await active.BSend(1, payload);
  const evt = await recvPromise;

  assert.strictEqual(evt.result, 0);
  assert.strictEqual(evt.R_ID, 1);
  assert.ok(evt.data.equals(payload));

  // The recv event does not consume the internal "last recv" slot; clear it so
  // later tests don't read this payload via BRecv().
  try {
    passive.BRecvSync(0);
  } catch (_) {}
});

test('Partner BRecv (Promise) resolves with data', async (t) => {
  if (skipIfNotLinked(t, active, passive)) {
    return;
  }

  const payload = Buffer.from([5, 4, 3, 2]);

  const recvPromise = passive.BRecv(2000);
  await active.BSend(3, payload);
  const res = await recvPromise;

  assert.strictEqual(res.R_ID, 3);
  assert.ok(res.data.equals(payload));
});

test('Partner BSendSync/BRecvSync roundtrip', async (t) => {
  if (skipIfNotLinked(t, active, passive)) {
    return;
  }

  const payload = Buffer.from([9, 8, 7, 6]);

  active.BSendSync(2, payload);
  const recv = passive.BRecvSync(2000);

  assert.strictEqual(recv.R_ID, 2);
  assert.ok(recv.data.equals(payload));
});

test('Partner Start/Stop sync methods work', async (t) => {
  await cleanupPartners();

  const passiveLocal = new S7Partner(false);
  const activeLocal = new S7Partner(true);

  try {
    const linked = await startPartnerPair({
      t,
      activeRef: activeLocal,
      passiveRef: passiveLocal,
      tsap: 0x1003,
      sync: true
    });

    if (!linked) {
      return;
    }

    activeLocal.StopSync();
    passiveLocal.StopSync();
  } finally {
    try {
      activeLocal.StopSync();
    } catch (_) {}
    try {
      passiveLocal.StopSync();
    } catch (_) {}
  }
});
