const snap7 = require('../lib/node-snap7');
const { test, beforeEach, afterEach } = require('node:test');
const assert = require('assert');

let server, client;
const DB_NUMBER = 1;
const SIZE = 16;
const TM_AMOUNT = 4;
const CT_AMOUNT = 4;
const TM_SIZE = TM_AMOUNT * 2;
const CT_SIZE = CT_AMOUNT * 2;

let dbBuffer, peBuffer, paBuffer, mkBuffer, tmBuffer, ctBuffer;

beforeEach(async () => {
  server = new snap7.S7Server();
  
  dbBuffer = Buffer.alloc(SIZE, 0xAA);
  peBuffer = Buffer.alloc(SIZE, 0xBB);
  paBuffer = Buffer.alloc(SIZE, 0xCC);
  mkBuffer = Buffer.alloc(SIZE, 0xDD);
  ctBuffer = Buffer.alloc(CT_SIZE, 0xEE);
  tmBuffer = Buffer.alloc(TM_SIZE, 0xFF);


  server.RegisterArea(server.srvAreaPE, peBuffer);
  server.RegisterArea(server.srvAreaPA, paBuffer);
  server.RegisterArea(server.srvAreaMK, mkBuffer);
  server.RegisterArea(server.srvAreaDB, DB_NUMBER, dbBuffer);
  server.RegisterArea(server.srvAreaCT, ctBuffer);
  server.RegisterArea(server.srvAreaTM, tmBuffer);

  await server.Start();

  client = new snap7.S7Client();
  await client.ConnectTo('127.0.0.1', 0, 0);
});

afterEach(async () => {
  await client.Disconnect();
  await server.Stop();
  server.UnregisterArea(server.srvAreaPE);
  server.UnregisterArea(server.srvAreaPA);
  server.UnregisterArea(server.srvAreaMK);
  server.UnregisterArea(server.srvAreaDB, DB_NUMBER);
  server.UnregisterArea(server.srvAreaCT);
  server.UnregisterArea(server.srvAreaTM);
});

// --- Connection Methods ---
test('ConnectToSync', () => {
  client.Disconnect();
  const result = client.ConnectToSync('127.0.0.1', 0, 0);
  assert.strictEqual(result, true);
});
test('ConnectTo (Promise)', async () => {
  await client.Disconnect();
  await client.ConnectTo('127.0.0.1', 0, 0);
  assert.strictEqual(client.Connected(), true);
});
test('ConnectTo (Callback)', (t, done) => {
  client.Disconnect();
  client.ConnectTo('127.0.0.1', 0, 0, (err) => {
    assert.ifError(err);
    assert.strictEqual(client.Connected(), true);
    done();
  });
});
test('Disconnect', () => {
  client.Disconnect();
  assert.strictEqual(client.Connected(), false);
});
test('Connected', () => {
  assert.strictEqual(typeof client.Connected(), 'boolean');
});
test('SetConnectionParams', () => {
  assert.strictEqual(client.SetConnectionParams('127.0.0.1', 0x0100, 0x0200), true);
});
test('SetConnectionType', () => {
  assert.strictEqual(client.SetConnectionType(client.CONNTYPE_BASIC), true);
});

// --- Parameter Methods ---
test('SetParam', () => {
  assert.strictEqual(client.SetParam(client.PDURequest, 480), true);
});
test('GetParam', () => {
  assert.strictEqual(client.GetParam(client.PDURequest), 480);
});

// --- Data I/O Methods (Sync/Promise/Callback) ---
test('ReadAreaSync', () => {
  const buf = client.ReadAreaSync(client.S7AreaDB, DB_NUMBER, 0, SIZE, client.S7WLByte);
  assert.ok(Buffer.isBuffer(buf));
});
test('ReadArea (Promise)', async () => {
  const buf = await client.ReadArea(client.S7AreaDB, DB_NUMBER, 0, SIZE, client.S7WLByte);
  assert.ok(Buffer.isBuffer(buf));
});
test('ReadArea (Callback)', (t, done) => {
  client.ReadArea(client.S7AreaDB, DB_NUMBER, 0, SIZE, client.S7WLByte, (err, buf) => {
    assert.ifError(err);
    assert.ok(Buffer.isBuffer(buf));
    done();
  });
});
test('WriteAreaSync', () => {
  const buf = Buffer.alloc(SIZE, 0x11);
  assert.strictEqual(client.WriteAreaSync(client.S7AreaDB, DB_NUMBER, 0, SIZE, client.S7WLByte, buf), true);
});
test('WriteArea (Promise)', async () => {
  const buf = Buffer.alloc(SIZE, 0x12);
  await client.WriteArea(client.S7AreaDB, DB_NUMBER, 0, SIZE, client.S7WLByte, buf);
});
test('WriteArea (Callback)', (t, done) => {
  const buf = Buffer.alloc(SIZE, 0x13);
  client.WriteArea(client.S7AreaDB, DB_NUMBER, 0, SIZE, client.S7WLByte, buf, (err) => {
    assert.ifError(err);
    done();
  });
});

// --- MultiVar Methods ---
test('ReadMultiVarsSync', () => {
  const items = [{ Area: client.S7AreaDB, WordLen: client.S7WLByte, DBNumber: DB_NUMBER, Start: 0, Amount: SIZE }];
  const res = client.ReadMultiVarsSync(items);
  assert.ok(Array.isArray(res));
});
test('ReadMultiVars (Promise)', async () => {
  const items = [{ Area: client.S7AreaDB, WordLen: client.S7WLByte, DBNumber: DB_NUMBER, Start: 0, Amount: SIZE }];
  const res = await client.ReadMultiVars(items);
  assert.ok(Array.isArray(res));
});
test('ReadMultiVars (Callback)', (t, done) => {
  const items = [{ Area: client.S7AreaDB, WordLen: client.S7WLByte, DBNumber: DB_NUMBER, Start: 0, Amount: SIZE }];
  client.ReadMultiVars(items, (err, res) => {
    assert.ifError(err);
    assert.ok(Array.isArray(res));
    done();
  });
});
test('WriteMultiVarsSync', () => {
  const items = [{ Area: client.S7AreaDB, WordLen: client.S7WLByte, DBNumber: DB_NUMBER, Start: 0, Amount: SIZE, Data: Buffer.alloc(SIZE, 0x21) }];
  const res = client.WriteMultiVarsSync(items);
  assert.ok(Array.isArray(res));
});
test('WriteMultiVars (Promise)', async () => {
  const items = [{ Area: client.S7AreaDB, WordLen: client.S7WLByte, DBNumber: DB_NUMBER, Start: 0, Amount: SIZE, Data: Buffer.alloc(SIZE, 0x22) }];
  const res = await client.WriteMultiVars(items);
  assert.ok(Array.isArray(res));
});
test('WriteMultiVars (Callback)', (t, done) => {
  const items = [{ Area: client.S7AreaDB, WordLen: client.S7WLByte, DBNumber: DB_NUMBER, Start: 0, Amount: SIZE, Data: Buffer.alloc(SIZE, 0x23) }];
  client.WriteMultiVars(items, (err, res) => {
    assert.ifError(err);
    assert.ok(Array.isArray(res));
    done();
  });
});

// --- DBRead/DBWrite ---
test('DBReadSync', () => {
  const buf = client.DBReadSync(DB_NUMBER, 0, SIZE);
  assert.ok(Buffer.isBuffer(buf));
});
test('DBRead (Promise)', async () => {
  const buf = await client.DBRead(DB_NUMBER, 0, SIZE);
  assert.ok(Buffer.isBuffer(buf));
});
test('DBRead (Callback)', (t, done) => {
  client.DBRead(DB_NUMBER, 0, SIZE, (err, buf) => {
    assert.ifError(err);
    assert.ok(Buffer.isBuffer(buf));
    done();
  });
});
test('DBWriteSync', () => {
  const buf = Buffer.alloc(SIZE, 0x31);
  assert.strictEqual(client.DBWriteSync(DB_NUMBER, 0, SIZE, buf), true);
});
test('DBWrite (Promise)', async () => {
  const buf = Buffer.alloc(SIZE, 0x32);
  await client.DBWrite(DB_NUMBER, 0, SIZE, buf);
});
test('DBWrite (Callback)', (t, done) => {
  const buf = Buffer.alloc(SIZE, 0x33);
  client.DBWrite(DB_NUMBER, 0, SIZE, buf, (err) => {
    assert.ifError(err);
    done();
  });
});

// --- DBWriteSync + DBReadSync ---
test('DBWriteSync + DBReadSync verify', () => {
  const buf = Buffer.alloc(SIZE, 0x41);
  assert.strictEqual(client.DBWriteSync(DB_NUMBER, 0, SIZE, buf), true);
  const readBuf = client.DBReadSync(DB_NUMBER, 0, SIZE);
  assert.ok(readBuf.equals(buf));
});

// --- DBWrite (Promise) + DBRead (Promise) ---
test('DBWrite (Promise) + DBRead (Promise) verify', async () => {
  const buf = Buffer.alloc(SIZE, 0x42);
  await client.DBWrite(DB_NUMBER, 0, SIZE, buf);
  const readBuf = await client.DBRead(DB_NUMBER, 0, SIZE);
  assert.ok(readBuf.equals(buf));
});

// --- DBWrite (Callback) + DBRead (Callback) ---
test('DBWrite (Callback) + DBRead (Callback) verify', (t, done) => {
  const buf = Buffer.alloc(SIZE, 0x43);
  client.DBWrite(DB_NUMBER, 0, SIZE, buf, (err) => {
    assert.ifError(err);
    client.DBRead(DB_NUMBER, 0, SIZE, (err, readBuf) => {
      assert.ifError(err);
      assert.ok(readBuf.equals(buf));
      done();
    });
  });
});

// --- MBRead/MBWrite ---
test('MBReadSync', () => {
  const buf = client.MBReadSync(0, SIZE);
  assert.ok(Buffer.isBuffer(buf));
});
test('MBRead (Promise)', async () => {
  const buf = await client.MBRead(0, SIZE);
  assert.ok(Buffer.isBuffer(buf));
});
test('MBRead (Callback)', (t, done) => {
  client.MBRead(0, SIZE, (err, buf) => {
    assert.ifError(err);
    assert.ok(Buffer.isBuffer(buf));
    done();
  });
});
test('MBWriteSync', () => {
  const buf = Buffer.alloc(SIZE, 0x41);
  assert.strictEqual(client.MBWriteSync(0, SIZE, buf), true);
});
test('MBWrite (Promise)', async () => {
  const buf = Buffer.alloc(SIZE, 0x42);
  await client.MBWrite(0, SIZE, buf);
});
test('MBWrite (Callback)', (t, done) => {
  const buf = Buffer.alloc(SIZE, 0x43);
  client.MBWrite(0, SIZE, buf, (err) => {
    assert.ifError(err);
    done();
  });
});

// --- EBRead/EBWrite ---
test('EBReadSync', () => {
  const buf = client.EBReadSync(0, SIZE);
  assert.ok(Buffer.isBuffer(buf));
});
test('EBRead (Promise)', async () => {
  const buf = await client.EBRead(0, SIZE);
  assert.ok(Buffer.isBuffer(buf));
});
test('EBRead (Callback)', (t, done) => {
  client.EBRead(0, SIZE, (err, buf) => {
    assert.ifError(err);
    assert.ok(Buffer.isBuffer(buf));
    done();
  });
});
test('EBWriteSync', () => {
  const buf = Buffer.alloc(SIZE, 0x51);
  assert.strictEqual(client.EBWriteSync(0, SIZE, buf), true);
});
test('EBWrite (Promise)', async () => {
  const buf = Buffer.alloc(SIZE, 0x52);
  await client.EBWrite(0, SIZE, buf);
});
test('EBWrite (Callback)', (t, done) => {
  const buf = Buffer.alloc(SIZE, 0x53);
  client.EBWrite(0, SIZE, buf, (err) => {
    assert.ifError(err);
    done();
  });
});

// --- ABRead/ABWrite ---
test('ABReadSync', () => {
  const buf = client.ABReadSync(0, SIZE);
  assert.ok(Buffer.isBuffer(buf));
});
test('ABRead (Promise)', async () => {
  const buf = await client.ABRead(0, SIZE);
  assert.ok(Buffer.isBuffer(buf));
});
test('ABRead (Callback)', (t, done) => {
  client.ABRead(0, SIZE, (err, buf) => {
    assert.ifError(err);
    assert.ok(Buffer.isBuffer(buf));
    done();
  });
});
test('ABWriteSync', () => {
  const buf = Buffer.alloc(SIZE, 0x61);
  assert.strictEqual(client.ABWriteSync(0, SIZE, buf), true);
});
test('ABWrite (Promise)', async () => {
  const buf = Buffer.alloc(SIZE, 0x62);
  await client.ABWrite(0, SIZE, buf);
});
test('ABWrite (Callback)', (t, done) => {
  const buf = Buffer.alloc(SIZE, 0x63);
  client.ABWrite(0, SIZE, buf, (err) => {
    assert.ifError(err);
    done();
  });
});

// --- TMRead/TMWrite ---
test('TMReadSync', () => {
  const buf = client.TMReadSync(0, TM_AMOUNT);
  assert.ok(Buffer.isBuffer(buf));
});
test('TMRead (Promise)', async () => {
  const buf = await client.TMRead(0, TM_AMOUNT);
  assert.ok(Buffer.isBuffer(buf));
});
test('TMRead (Callback)', (t, done) => {
  client.TMRead(0, TM_AMOUNT, (err, buf) => {
    assert.ifError(err);
    assert.ok(Buffer.isBuffer(buf));
    done();
  });
});
test('TMWriteSync', () => {
  const buf = Buffer.alloc(TM_SIZE, 0x71);
  assert.strictEqual(client.TMWriteSync(0, TM_AMOUNT, buf), true);
});
test('TMWrite (Promise)', async () => {
  const buf = Buffer.alloc(TM_SIZE, 0x72);
  await client.TMWrite(0, TM_AMOUNT, buf);
});
test('TMWrite (Callback)', (t, done) => {
  const buf = Buffer.alloc(TM_SIZE, 0x73);
  client.TMWrite(0, TM_AMOUNT, buf, (err) => {
    assert.ifError(err);
    done();
  });
});

// --- CTRead/CTWrite ---
test('CTReadSync', () => {
  const buf = client.CTReadSync(0, CT_AMOUNT);
  assert.ok(Buffer.isBuffer(buf));
});
test('CTRead (Promise)', async () => {
  const buf = await client.CTRead(0, CT_AMOUNT);
  assert.ok(Buffer.isBuffer(buf));
});
test('CTRead (Callback)', (t, done) => {
  client.CTRead(0, CT_AMOUNT, (err, buf) => {
    assert.ifError(err);
    assert.ok(Buffer.isBuffer(buf));
    done();
  });
});
test('CTWriteSync', () => {
  const buf = Buffer.alloc(CT_SIZE, 0x71);
  assert.strictEqual(client.CTWriteSync(0, CT_AMOUNT, buf), true);
});
test('CTWrite (Promise)', async () => {
  const buf = Buffer.alloc(CT_SIZE, 0x72);
  await client.CTWrite(0, CT_AMOUNT, buf);
});
test('CTWrite (Callback)', (t, done) => {
  const buf = Buffer.alloc(CT_SIZE, 0x73);
  client.CTWrite(0, CT_AMOUNT, buf, (err) => {
    assert.ifError(err);
    done();
  });
});

// --- ListBlocks/ListBlocksOfType ---
test('ListBlocksSync', () => {
  const res = client.ListBlocksSync();
  assert.ok(typeof res === 'object');
});
test('ListBlocks (Promise)', async () => {
  const res = await client.ListBlocks();
  assert.ok(typeof res === 'object');
});
test('ListBlocks (Callback)', (t, done) => {
  client.ListBlocks((err, res) => {
    assert.ifError(err);
    assert.ok(typeof res === 'object');
    done();
  });
});
test('ListBlocksOfTypeSync', () => {
  const res = client.ListBlocksOfTypeSync(client.Block_DB);
  assert.ok(Array.isArray(res));
});
test('ListBlocksOfType (Promise)', async () => {
  const res = await client.ListBlocksOfType(client.Block_DB);
  assert.ok(Array.isArray(res));
});
test('ListBlocksOfType (Callback)', (t, done) => {
  client.ListBlocksOfType(client.Block_DB, (err, res) => {
    assert.ifError(err);
    assert.ok(Array.isArray(res));
    done();
  });
});

// --- Upload/FullUpload/Download/Delete ---
test('UploadSync', () => {
  assert.throws(() => client.UploadSync(client.Block_DB, DB_NUMBER, SIZE));
});
test('Upload (Promise)', async () => {
  await assert.rejects(client.Upload(client.Block_DB, DB_NUMBER, SIZE));
});
test('Upload (Callback)', (t, done) => {
  client.Upload(client.Block_DB, DB_NUMBER, SIZE, (err, buf) => {
    assert.ok(err);
    done();
  });
});
test('FullUploadSync', () => {
  assert.throws(() => client.FullUploadSync(client.Block_DB, DB_NUMBER, SIZE));
});
test('FullUpload (Promise)', async () => {
  await assert.rejects(client.FullUpload(client.Block_DB, DB_NUMBER, SIZE));
});
test('FullUpload (Callback)', (t, done) => {
  client.FullUpload(client.Block_DB, DB_NUMBER, SIZE, (err, buf) => {
    assert.ok(err);
    done();
  });
});
test('DownloadSync', () => {
  assert.throws(() => client.DownloadSync(DB_NUMBER, Buffer.alloc(SIZE)));
});
test('Download (Promise)', async () => {
  await assert.rejects(client.Download(DB_NUMBER, Buffer.alloc(SIZE)));
});
test('Download (Callback)', (t, done) => {
  client.Download(DB_NUMBER, Buffer.alloc(SIZE), (err) => {
    assert.ok(err);
    done();
  });
});
test('DeleteSync', () => {
  assert.throws(() => client.DeleteSync(client.Block_DB, DB_NUMBER));
});
test('Delete (Promise)', async () => {
  await assert.rejects(client.Delete(client.Block_DB, DB_NUMBER));
});
test('Delete (Callback)', (t, done) => {
  client.Delete(client.Block_DB, DB_NUMBER, (err) => {
    assert.ok(err);
    done();
  });
});

// --- DBFill/DBGet ---
test('DBFillSync', () => {
  assert.strictEqual(client.DBFillSync(DB_NUMBER, 0x00), true);
});
test('DBFill (Promise)', async () => {
  await client.DBFill(DB_NUMBER, 0x00);
});
test('DBFill (Callback)', (t, done) => {
  client.DBFill(DB_NUMBER, 0x00, (err) => {
    assert.ifError(err);
    done();
  });
});
test('DBGetSync', () => {
  const buf = client.DBGetSync(DB_NUMBER);
  assert.ok(Buffer.isBuffer(buf));
});
test('DBGet (Promise)', async () => {
  const buf = await client.DBGet(DB_NUMBER);
  assert.ok(Buffer.isBuffer(buf));
});
test('DBGet (Callback)', (t, done) => {
  client.DBGet(DB_NUMBER, (err, buf) => {
    assert.ifError(err);
    assert.ok(Buffer.isBuffer(buf));
    done();
  });
});

// --- PlcHotStart/PlcColdStart/PlcStop ---
test('PlcHotStartSync', () => {
  assert.strictEqual(typeof client.PlcHotStartSync(), 'boolean');
});
test('PlcHotStart (Promise)', async () => {
  await client.PlcHotStart();
});
test('PlcHotStart (Callback)', (t, done) => {
  client.PlcHotStart((err) => {
    assert.ifError(err);
    done();
  });
});
test('PlcColdStartSync', () => {
  assert.strictEqual(typeof client.PlcColdStartSync(), 'boolean');
});
test('PlcColdStart (Promise)', async () => {
  await client.PlcColdStart();
});
test('PlcColdStart (Callback)', (t, done) => {
  client.PlcColdStart((err) => {
    assert.ifError(err);
    done();
  });
});
test('PlcStopSync', () => {
  assert.strictEqual(typeof client.PlcStopSync(), 'boolean');
});
test('PlcStop (Promise)', async () => {
  await client.PlcStop();
});
test('PlcStop (Callback)', (t, done) => {
  client.PlcStop((err) => {
    assert.ifError(err);
    done();
  });
});

// --- CopyRamToRom/Compress ---
test('CopyRamToRomSync', () => {
  assert.throws(() => client.CopyRamToRomSync(1000));
});
test('CopyRamToRom (Promise)', async () => {
  await assert.rejects(client.CopyRamToRom(1000));
});
test('CopyRamToRom (Callback)', (t, done) => {
  client.CopyRamToRom(1000, (err) => {
    assert.ok(err);
    done();
  });
});
test('CompressSync', () => {
  assert.throws(() => client.CompressSync(1000));
});
test('Compress (Promise)', async () => {
  await assert.rejects(client.Compress(1000));
});
test('Compress (Callback)', (t, done) => {
  client.Compress(1000, (err) => {
    assert.ok(err);
    done();
  });
});

// --- PlcStatus/GetProtection ---
test('PlcStatusSync', () => {
  assert.ok([client.S7CpuStatusRun, client.S7CpuStatusStop, client.S7CpuStatusUnknown].includes(client.PlcStatusSync()));
});
test('PlcStatus (Promise)', async () => {
  const status = await client.PlcStatus();
  assert.ok([client.S7CpuStatusRun, client.S7CpuStatusStop, client.S7CpuStatusUnknown].includes(status));
});
test('PlcStatus (Callback)', (t, done) => {
  client.PlcStatus((err, status) => {
    assert.ifError(err);
    assert.ok([client.S7CpuStatusRun, client.S7CpuStatusStop, client.S7CpuStatusUnknown].includes(status));
    done();
  });
});
test('GetProtectionSync', () => {
  assert.ok(typeof client.GetProtectionSync() === 'object');
});
test('GetProtection (Promise)', async () => {
  assert.ok(typeof (await client.GetProtection()) === 'object');
});
test('GetProtection (Callback)', (t, done) => {
  client.GetProtection((err, prot) => {
    assert.ifError(err);
    assert.ok(typeof prot === 'object');
    done();
  });
});

// --- SetSessionPassword/ClearSessionPassword ---
test('SetSessionPasswordSync', () => {
  assert.strictEqual(typeof client.SetSessionPasswordSync('test'), 'boolean');
});
test('SetSessionPassword (Promise)', async () => {
  await client.SetSessionPassword('test');
});
test('SetSessionPassword (Callback)', (t, done) => {
  client.SetSessionPassword('test', (err) => {
    assert.ifError(err);
    done();
  });
});
test('ClearSessionPasswordSync', () => {
  assert.strictEqual(typeof client.ClearSessionPasswordSync(), 'boolean');
});
test('ClearSessionPassword (Promise)', async () => {
  await client.ClearSessionPassword();
});
test('ClearSessionPassword (Callback)', (t, done) => {
  client.ClearSessionPassword((err) => {
    assert.ifError(err);
    done();
  });
});

// --- DateTime Functions ---
test('GetPlcDateTimeSync', () => {
  assert.ok(client.GetPlcDateTimeSync() instanceof Date);
});
test('GetPlcDateTime (Promise)', async () => {
  assert.ok((await client.GetPlcDateTime()) instanceof Date);
});
test('GetPlcDateTime (Callback)', (t, done) => {
  client.GetPlcDateTime((err, date) => {
    assert.ifError(err);
    assert.ok(date instanceof Date);
    done();
  });
});
test('SetPlcDateTimeSync', () => {
  assert.strictEqual(typeof client.SetPlcDateTimeSync(new Date()), 'boolean');
});
test('SetPlcDateTime (Promise)', async () => {
  await client.SetPlcDateTime(new Date());
});
test('SetPlcDateTime (Callback)', (t, done) => {
  client.SetPlcDateTime(new Date(), (err) => {
    assert.ifError(err);
    done();
  });
});
test('SetPlcSystemDateTimeSync', () => {
  assert.strictEqual(typeof client.SetPlcSystemDateTimeSync(), 'boolean');
});
test('SetPlcSystemDateTime (Promise)', async () => {
  await client.SetPlcSystemDateTime();
});
test('SetPlcSystemDateTime (Callback)', (t, done) => {
  client.SetPlcSystemDateTime((err) => {
    assert.ifError(err);
    done();
  });
});

// --- SZL Operations ---
test('ReadSZLSync', () => {
  const buf = client.ReadSZLSync(0x0011, 0x0000);
  assert.ok(Buffer.isBuffer(buf));
});
test('ReadSZL (Promise)', async () => {
  const buf = await client.ReadSZLSync(0x0011, 0x0000);
  assert.ok(Buffer.isBuffer(buf));
});
test('ReadSZL (Callback)', (t, done) => {
  client.ReadSZL(0x0011, 0x0000, (err, buf) => {
    assert.ifError(err);
    assert.ok(Buffer.isBuffer(buf));
    done();
  });
});
test('ReadSZLListSync', () => {
  assert.ok(Array.isArray(client.ReadSZLListSync()));
});
test('ReadSZLList (Promise)', async () => {
  assert.ok(Array.isArray(await client.ReadSZLList()));
});
test('ReadSZLList (Callback)', (t, done) => {
  client.ReadSZLList((err, arr) => {
    assert.ifError(err);
    assert.ok(Array.isArray(arr));
    done();
  });
});

// --- Information ---
test('GetCpuInfoSync', () => {
  assert.ok(typeof client.GetCpuInfoSync() === 'object');
});
test('GetCpuInfo (Promise)', async () => {
  assert.ok(typeof (await client.GetCpuInfo()) === 'object');
});
test('GetCpuInfo (Callback)', (t, done) => {
  client.GetCpuInfo((err, info) => {
    assert.ifError(err);
    assert.ok(typeof info === 'object');
    done();
  });
});
// test('GetCpInfoSync', () => {
//   assert.ok(typeof client.GetCpInfoSync() === 'object');
// });
// test('GetCpInfo (Promise)', async () => {
//   console.log('GetCpInfo (Promise) called');
//   assert.ok(typeof (await client.GetCpInfo()) === 'object');
// });
// test('GetCpInfo (Callback)', (t, done) => {
//   console.log('GetCpInfo (Callback) called');
//   client.GetCpInfo((err, info) => {
//     assert.ifError(err);
//     assert.ok(typeof info === 'object');
//     done();
//   });
// });
// test('GetOrderCodeSync', () => {
//   assert.ok(typeof client.GetOrderCodeSync() === 'object');
// });
// test('GetOrderCode (Promise)', async () => {
//   assert.ok(typeof (await client.GetOrderCode()) === 'object');
// });
// test('GetOrderCode (Callback)', (t, done) => {
//   client.GetOrderCode((err, info) => {
//     assert.ifError(err);
//     assert.ok(typeof info === 'object');
//     done();
//   });
// });

// --- Block Info ---
// test('GetAgBlockInfoSync', () => {
//   assert.ok(typeof client.GetAgBlockInfoSync(client.Block_DB, DB_NUMBER) === 'object');
// });
// test('GetAgBlockInfo (Promise)', async () => {
//   assert.ok(typeof (await client.GetAgBlockInfo(client.Block_DB, DB_NUMBER)) === 'object');
// });
// test('GetAgBlockInfo (Callback)', (t, done) => {
//   client.GetAgBlockInfo(client.Block_DB, DB_NUMBER, (err, info) => {
//     assert.ifError(err);
//     assert.ok(typeof info === 'object');
//     done();
//   });
// });
// test('GetPgBlockInfoSync', () => {
//   const buf = Buffer.alloc(SIZE);
//   assert.throws(() => client.GetPgBlockInfoSync(buf));
// });
// test('GetPgBlockInfo (Promise)', async () => {
//   const buf = Buffer.alloc(SIZE);
//   await assert.rejects(client.GetPgBlockInfo(buf));
// });
// test('GetPgBlockInfo (Callback)', (t, done) => {
//   const buf = Buffer.alloc(SIZE);
//   client.GetPgBlockInfo(buf, (err, info) => {
//     assert.ok(err);
//     done();
//   });
// });

// --- Properties and Info ---
test('ExecTime', () => {
  assert.ok(typeof client.ExecTime() === 'number' || typeof client.ExecTime() === 'boolean');
});
test('LastError', () => {
  assert.ok(typeof client.LastError() === 'number' || typeof client.LastError() === 'boolean');
});
test('PDURequested', () => {
  assert.ok(typeof client.PDURequested() === 'number' || typeof client.PDURequested() === 'boolean');
});
test('PDULength', () => {
  assert.ok(typeof client.PDULength() === 'number' || typeof client.PDULength() === 'boolean');
});
test('ErrorText', () => {
  assert.ok(typeof client.ErrorText(0) === 'string');
});
