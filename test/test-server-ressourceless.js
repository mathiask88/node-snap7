const {
  S7Server,
  S7Client,
  ServerArea,
  ServerParameter,
  ClientParameter,
  ServerOperation
} = require('../lib/node-snap7');
const { test, before, after } = require('node:test');
const assert = require('assert');

const DB_NUMBER = 1;
const SIZE = 16;

let server, client, dynamicPort;
let dbBuffer;

before(async () => {
  // Pick a free high port to avoid collisions when tests run concurrently in CI.
  dynamicPort = await new Promise((resolve, reject) => {
    const net = require('net');
    const s = net.createServer();
    s.on('error', reject);
    s.listen(0, '127.0.0.1', () => {
      const address = s.address();
      s.close(() => resolve(address.port));
    });
  });

  server = new S7Server();
  server.SetResourceless(true);

  dbBuffer = Buffer.alloc(SIZE, 0xAA);

  // Register DB area (required for tag info, but data is handled by event)
  server.RegisterArea(ServerArea.DB, DB_NUMBER, dbBuffer);

  // Handle read/write events
  server.on('readWrite', (sender, operation, tag, buffer, callback) => {
    if (operation === ServerOperation.Read) {
      // Fill buffer with current DB content
      dbBuffer.copy(buffer, 0, tag.Start, tag.Start + tag.Size);
      callback(buffer);
    } else if (operation === ServerOperation.Write) {
      // Write incoming data to DB buffer
      buffer.copy(dbBuffer, tag.Start, 0, tag.Size);
      callback();
    }
  });

  server.SetParam(ServerParameter.LocalPort, dynamicPort);
  await server.StartTo('127.0.0.1');
});

after(async () => {
  await server.Stop();
  server.UnregisterArea(ServerArea.DB, DB_NUMBER);
});

test('ressourceless server: write and read DB', async () => {
  client = new S7Client();
  client.SetParam(ClientParameter.RemotePort, dynamicPort);
  await client.ConnectTo('127.0.0.1', 0, 0);

  const writeBuf = Buffer.from([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]);
  await client.DBWrite(DB_NUMBER, 0, SIZE, writeBuf);

  const readBuf = await client.DBRead(DB_NUMBER, 0, SIZE);
  assert.ok(readBuf.equals(writeBuf), 'Read buffer should match written buffer');

  await client.Disconnect();
});
