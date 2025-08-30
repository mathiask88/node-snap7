const snap7 = require('../lib/node-snap7');
const { test, before, after } = require('node:test');
const assert = require('assert');

const DB_NUMBER = 1;
const SIZE = 16;

let server, client;
let dbBuffer;

before(async () => {
  server = new snap7.S7Server();
  server.SetResourceless(false);

  dbBuffer = Buffer.alloc(SIZE, 0xAA);

  // Register DB area (server manages the buffer directly)
  server.RegisterArea(server.srvAreaDB, DB_NUMBER, dbBuffer);

  await server.StartTo('127.0.0.1');
});

after(async () => {
  await server.Stop();
  server.UnregisterArea(server.srvAreaDB, DB_NUMBER);
});

test('resource server: write and read DB', async () => {
  client = new snap7.S7Client();
  await client.ConnectTo('127.0.0.1', 0, 0);

  const writeBuf = Buffer.from([1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]);
  await client.DBWrite(DB_NUMBER, 0, SIZE, writeBuf);

  const readBuf = await client.DBRead(DB_NUMBER, 0, SIZE);
  assert.ok(readBuf.equals(writeBuf), 'Read buffer should match written buffer');

  await client.Disconnect();
});
