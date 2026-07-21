const {
  S7Server,
  S7Client,
  ServerArea,
  ServerParameter,
  ClientParameter,
  ServerError,
  ServerStatus
} = require('../lib/node-snap7');
const { test, before, after } = require('node:test');
const assert = require('assert');
const net = require('net');

const DB_NUMBER = 1;
const SIZE = 16;

let server, client, dynamicPort;
let dbBuffer;

function getFreePort() {
  return new Promise((resolve, reject) => {
    const s = net.createServer();
    s.on('error', reject);
    s.listen(0, '127.0.0.1', () => {
      const address = s.address();
      s.close(() => resolve(address.port));
    });
  });
}

before(async () => {
  // Pick a free high port to avoid collisions when tests run concurrently in CI.
  dynamicPort = await getFreePort();

  server = new S7Server();
  server.SetResourceless(false);

  dbBuffer = Buffer.alloc(SIZE, 0xAA);

  // Register DB area (server manages the buffer directly)
  server.RegisterArea(ServerArea.DB, DB_NUMBER, dbBuffer);

  server.SetParam(ServerParameter.LocalPort, dynamicPort);
  await server.StartTo('127.0.0.1');
});

after(async () => {
  await server.Stop();
  server.UnregisterArea(ServerArea.DB, DB_NUMBER);
});

test('resource server: write and read DB', async () => {
  client = new S7Client();
  client.SetParam(ClientParameter.RemotePort, dynamicPort);
  await client.ConnectTo('127.0.0.1', 0, 0);

  const writeBuf = Buffer.from([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]);
  await client.DBWrite(DB_NUMBER, 0, SIZE, writeBuf);

  const readBuf = await client.DBRead(DB_NUMBER, 0, SIZE);
  assert.ok(readBuf.equals(writeBuf), 'Read buffer should match written buffer');

  await client.Disconnect();
});

test('server SetParam invalid throws Snap7 error', () => {
  assert.throws(
    () => server.SetParam(9999, 1),
    (err) =>
      err &&
      err.errno === ServerError.InvalidParamNumber &&
      err.code === `SNAP7_SERVER_CODE_${ServerError.InvalidParamNumber}`
  );
});

test('server StartToSync/StopSync works', async () => {
  const srv = new S7Server();
  const port = await getFreePort();

  srv.SetParam(ServerParameter.LocalPort, port);
  srv.RegisterArea(ServerArea.DB, DB_NUMBER, Buffer.alloc(SIZE));

  try {
    srv.StartToSync('127.0.0.1');
    assert.strictEqual(srv.ServerStatus(), ServerStatus.SrvRunning);
  } finally {
    try {
      srv.StopSync();
    } catch (_) {
      // ignore
    }
  }
});
