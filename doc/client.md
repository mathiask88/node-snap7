# S7Client API

The `S7Client` class provides high-level access to Siemens S7 PLCs via Snap7.  
All methods are available as **Promise**, **callback**, and (where applicable) **Sync** variants.

- If a callback is provided, the method is asynchronous and uses Node.js-style `(err, result)` callbacks.
- If no callback is provided, the method returns a Promise (or, for Sync variants, the result directly or throws on error).
- Errors are thrown as exceptions (Promise/Sync) or passed as the first argument to the callback.

---

## Table of Contents

- [Control functions](#control-functions)
  - [Connect([callback])](#s7clientconnectcallback)
  - [ConnectTo(ip, rack, slot[, callback])](#s7clientconnecttoip-rack-slot-callback)
  - [SetConnectionParams(ip, localTSAP, remoteTSAP)](#s7clientsetconnectionparamsip-localtsap-remotetsap)
  - [SetConnectionType(type)](#s7clientsetconnectiontypetype)
  - [Disconnect()](#s7clientdisconnect)
  - [GetParam(paramNumber)](#s7clientgetparamparamnumber)
  - [SetParam(paramNumber, value)](#s7clientsetparamparamnumber-value)
- [Data I/O functions](#data-io-functions)
  - [ReadArea(area, dbNumber, start, amount, wordLen[, callback])](#s7clientreadareaarea-dbnumber-start-amount-wordlen-callback)
  - [WriteArea(area, dbNumber, start, amount, wordLen, buffer[, callback])](#s7clientwriteareaarea-dbnumber-start-amount-wordlen-buffer-callback)
  - [DBRead(dbNumber, start, size[, callback])](#s7clientdbreaddbnumber-start-size-callback)
  - [DBWrite(dbNumber, start, size, buffer[, callback])](#s7clientdbwritedbnumber-start-size-buffer-callback)
  - [MBRead(start, size[, callback])](#s7clientmbreadstart-size-callback)
  - [MBWrite(start, size, buffer[, callback])](#s7clientmbwritestart-size-buffer-callback)
  - [EBRead(start, size[, callback])](#s7clientebreadstart-size-callback)
  - [EBWrite(start, size, buffer[, callback])](#s7clientebwritestart-size-buffer-callback)
  - [ABRead(start, size[, callback])](#s7clientabreadstart-size-callback)
  - [ABWrite(start, size, buffer[, callback])](#s7clientabwritestart-size-buffer-callback)
  - [TMRead(start, amount[, callback])](#s7clienttmreadstart-amount-callback)
  - [TMWrite(start, amount, buffer[, callback])](#s7clienttmwritestart-amount-buffer-callback)
  - [CTRead(start, amount[, callback])](#s7clientctreadstart-amount-callback)
  - [CTWrite(start, amount, buffer[, callback])](#s7clientctwritestart-amount-buffer-callback)
  - [ReadMultiVars(items[, callback])](#s7clientreadmultivarsitems-callback)
  - [WriteMultiVars(items[, callback])](#s7clientwritemultivarsitems-callback)
- [Directory functions](#directory-functions)
  - [ListBlocks([callback])](#s7clientlistblockscallback)
  - [ListBlocksOfType(blockType[, callback])](#s7clientlistblocksoftypeblocktype-callback)
  - [GetAgBlockInfo(blockType, blockNum[, callback])](#s7clientgetagblockinfoblocktype-blocknum-callback)
  - [GetPgBlockInfo(buffer[, callback])](#s7clientgetpgblockinfobuffer-callback)
- [Block oriented functions](#block-oriented-functions)
  - [FullUpload(blockType, blockNum, size[, callback])](#s7clientfulluploadblocktype-blocknum-size-callback)
  - [Upload(blockType, blockNum, size[, callback])](#s7clientuploadblocktype-blocknum-size-callback)
  - [Download(blockNum, buffer[, callback])](#s7clientdownloadblocknum-buffer-callback)
  - [Delete(blockType, blockNum[, callback])](#s7clientdeleteblocktype-blocknum-callback)
  - [DBGet(dbNumber[, callback])](#s7clientdbgetdbnumber-callback)
  - [DBFill(dbNumber, fillChar[, callback])](#s7clientdbfilldbnumber-fillchar-callback)
- [Date/Time functions](#date-time-functions)
  - [GetPlcDateTime([callback])](#s7clientgetplcdatetimecallback)
  - [SetPlcDateTime(dateTime[, callback])](#s7clientsetplcdatetimedatetime-callback)
  - [SetPlcSystemDateTime([callback])](#s7clientsetplcsystemdatetimecallback)
- [System info functions](#system-info-functions)
  - [ReadSZL(id, index[, callback])](#s7clientreadszlid-index-callback)
  - [ReadSZLList([callback])](#s7clientreadszllistcallback)
  - [GetOrderCode([callback])](#s7clientgetordercodecallback)
  - [GetCpuInfo([callback])](#s7clientgetcpuinfocallback)
  - [GetCpInfo([callback])](#s7clientgetcpinfocallback)
- [PLC control functions](#plc-control-functions)
  - [PlcHotStart([callback])](#s7clientplchotstartcallback)
  - [PlcColdStart([callback])](#s7clientplccoldstartcallback)
  - [PlcStop([callback])](#s7clientplcstopcallback)
  - [CopyRamToRom(timeout[, callback])](#s7clientcopyramtoromtimeout-callback)
  - [Compress(timeout[, callback])](#s7clientcompresstimeout-callback)
  - [PlcStatus([callback])](#s7clientplcstatuscallback)
- [Security functions](#security-functions)
  - [SetSessionPassword(password[, callback])](#s7clientsetsessionpasswordpassword-callback)
  - [ClearSessionPassword([callback])](#s7clientclearsessionpasswordcallback)
  - [GetProtection([callback])](#s7clientgetprotectioncallback)
- [Properties](#properties)
  - [ExecTime()](#s7clientexectime)
  - [LastError()](#s7clientlasterror)
  - [PDURequested()](#s7clientpdurequested)
  - [PDULength()](#s7clientpdulength)
  - [Connected()](#s7clientconnected)
  - [ErrorText(code)](#s7clienterrortextcode)

---

## <a name="control-functions"></a>Control functions

### S7Client.Connect([callback])
Connects the client to the PLC using parameters set by `ConnectTo()` or `SetConnectionParams()`.

- Returns: `Promise<void>` or calls `callback(err)`
- Sync: `ConnectSync()`

**Example:**
```js
// Promise
await client.Connect();
// Callback
client.Connect((err) => { if (err) throw err; });
// Sync
client.ConnectSync();
```

---

### S7Client.ConnectTo(ip, rack, slot[, callback])
Connects to the PLC at the specified IP, rack, and slot.

- `ip`: PLC IPv4 address (e.g. `"192.168.1.12"`)
- `rack`: PLC rack number
- `slot`: PLC slot number
- Returns: `Promise<void>` or calls `callback(err)`
- Sync: `ConnectToSync(ip, rack, slot)`

**Example:**
```js
// Promise
await client.ConnectTo('192.168.1.12', 0, 2);
// Callback
client.ConnectTo('192.168.1.12', 0, 2, (err) => { if (err) throw err; });
// Sync
client.ConnectToSync('192.168.1.12', 0, 2);
```

---

### S7Client.SetConnectionParams(ip, localTSAP, remoteTSAP)
Sets the connection parameters.

- `ip`: PLC IPv4 address
- `localTSAP`: Local TSAP (PC TSAP)
- `remoteTSAP`: Remote TSAP (PLC TSAP)
- Returns: `void`

**Example:**
```js
client.SetConnectionParams('192.168.1.12', 0x1000, 0x2000);
```

---

### S7Client.SetConnectionType(type)
Sets the connection resource type.

- `type`: One of `S7Client.CONNTYPE_PG`, `S7Client.CONNTYPE_OP`, `S7Client.CONNTYPE_BASIC`

| ConnectionType            | Value      | Description |
|:--------------------------|:----------:|:------------|
| `S7Client.CONNTYPE_PG`    | 0x01       | PG          |
| `S7Client.CONNTYPE_OP`    | 0x02       | OP          |
| `S7Client.CONNTYPE_BASIC` | 0x03..0x10 | S7 Basic    |

**Example:**
```js
client.SetConnectionType(S7Client.CONNTYPE_PG);
```

---

### S7Client.Disconnect()
Disconnects the client from the PLC.

- Returns: `void`

**Example:**
```js
client.Disconnect();
```

---

### S7Client.GetParam(paramNumber)
Reads an internal client parameter.

- `paramNumber`: See table below
- Returns: `number`

| Name                   | Value | Description |
|:-----------------------|:-----:|:------------|
| `S7Client.RemotePort`  | 2     | Socket remote Port
| `S7Client.PingTimeout` | 3     | Client Ping timeout
| `S7Client.SendTimeout` | 4     | Socket Send timeout
| `S7Client.RecvTimeout` | 5     | Socket Recv timeout
| `S7Client.SrcRef`      | 7     | ISOTcp Source reference
| `S7Client.DstRef`      | 8     | ISOTcp Destination reference
| `S7Client.SrcTSap`     | 9     | ISOTcp Source TSAP
| `S7Client.PDURequest`  | 10    | Initial PDU length request

**Example:**
```js
const timeout = client.GetParam(S7Client.PingTimeout);
```

---

### S7Client.SetParam(paramNumber, value)
Sets an internal client parameter.

- `paramNumber`: See table above
- `value`: New parameter value
- Returns: `void`

**Example:**
```js
client.SetParam(S7Client.PingTimeout, 500);
```

---

## <a name="data-io-functions"></a>Data I/O functions

### S7Client.ReadArea(area, dbNumber, start, amount, wordLen[, callback])
Reads data from the PLC (DB, Inputs, Outputs, Merkers, Timers, Counters).

- `area`: Area identifier (see [Area Table](#area-table))
- `dbNumber`: DB number if area = S7AreaDB, otherwise ignored
- `start`: Offset to start
- `amount`: Amount of **words** to read
- `wordLen`: Word size (see [WordLen Table](#wordlen-table))
- Returns: `Promise<Buffer>` or calls `callback(err, buffer)`
- Sync: `ReadAreaSync(...)`

#### <a name="area-table"></a>Area Table

| Area                   | Value | Description |
|:-----------------------|:-----:|:------------|
| `S7Client.S7AreaPE`    | 0x81  | Process inputs
| `S7Client.S7AreaPA`    | 0x82  | Process outputs
| `S7Client.S7AreaMK`    | 0x83  | Merkers
| `S7Client.S7AreaDB`    | 0x84  | DB
| `S7Client.S7AreaCT`    | 0x1C  | Counters
| `S7Client.S7AreaTM`    | 0x1D  | Timers

#### <a name="wordlen-table"></a>WordLen Table

| WordLen                | Value | Description |
|:-----------------------|:-----:|:------------|
| `S7Client.S7WLBit`     | 0x01  | Bit (inside a word)
| `S7Client.S7WLByte`    | 0x02  | Byte (8 bit)
| `S7Client.S7WLWord`    | 0x04  | Word (16 bit)
| `S7Client.S7WLDWord`   | 0x06  | Double Word (32 bit)
| `S7Client.S7WLReal`    | 0x08  | Real (32 bit float)
| `S7Client.S7WLCounter` | 0x1C  | Counter (16 bit)
| `S7Client.S7WLTimer`   | 0x1D  | Timer (16 bit)

**Example:**
```js
// Promise
const buf = await client.ReadArea(S7Client.S7AreaDB, 1, 0, 4, S7Client.S7WLWord);
// Callback
client.ReadArea(S7Client.S7AreaDB, 1, 0, 4, S7Client.S7WLWord, (err, buf) => {});
// Sync
const bufSync = client.ReadAreaSync(S7Client.S7AreaDB, 1, 0, 4, S7Client.S7WLWord);
```

---

### S7Client.WriteArea(area, dbNumber, start, amount, wordLen, buffer[, callback])
Writes data to the PLC.

- `area`, `dbNumber`, `start`, `amount`, `wordLen`: See above
- `buffer`: Buffer to write
- Returns: `Promise<void>` or calls `callback(err)`
- Sync: `WriteAreaSync(...)`

**Example:**
```js
// Promise
await client.WriteArea(S7Client.S7AreaDB, 1, 0, 4, S7Client.S7WLWord, Buffer.from([0,1,2,3,4,5,6,7]));
// Callback
client.WriteArea(S7Client.S7AreaDB, 1, 0, 4, S7Client.S7WLWord, Buffer.from([0,1,2,3,4,5,6,7]), (err) => {});
// Sync
client.WriteAreaSync(S7Client.S7AreaDB, 1, 0, 4, S7Client.S7WLWord, Buffer.from([0,1,2,3,4,5,6,7]));
```

---

### S7Client.DBRead(dbNumber, start, size[, callback])
Reads a range of bytes from a Data Block (DB) in the PLC.

- `dbNumber` (number): The DB number to read from.
- `start` (number): The byte offset within the DB.
- `size` (number): Number of bytes to read.
- `callback` (optional): Function `(err, buffer)` called on completion.

**Returns:**  
- If no callback is given: Returns a `Promise<Buffer>`.
- If callback is given: Calls `callback(err, buffer)`.

**Details:**  
This is a convenience wrapper for `ReadArea()` with `area = S7Client.S7AreaDB` and `wordLen = S7Client.S7WLByte`.  
It is the most common way to read DB data as a byte stream.

**Example:**
```js
// Promise
const buf = await client.DBRead(1, 0, 16);
// Callback
client.DBRead(1, 0, 16, (err, buf) => { if (err) throw err; });
// Sync
const bufSync = client.DBReadSync(1, 0, 16);
```

---

### S7Client.DBWrite(dbNumber, start, size, buffer[, callback])
Writes a range of bytes to a Data Block (DB) in the PLC.

- `dbNumber` (number): The DB number to write to.
- `start` (number): The byte offset within the DB.
- `size` (number): Number of bytes to write.
- `buffer` (Buffer): The data to write.
- `callback` (optional): Function `(err)` called on completion.

**Returns:**  
- If no callback is given: Returns a `Promise<void>`.
- If callback is given: Calls `callback(err)`.

**Details:**  
This is a convenience wrapper for `WriteArea()` with `area = S7Client.S7AreaDB` and `wordLen = S7Client.S7WLByte`.  
It is the most common way to write DB data as a byte stream.

**Example:**
```js
// Promise
await client.DBWrite(1, 0, 4, Buffer.from([1,2,3,4]));
// Callback
client.DBWrite(1, 0, 4, Buffer.from([1,2,3,4]), (err) => { if (err) throw err; });
// Sync
client.DBWriteSync(1, 0, 4, Buffer.from([1,2,3,4]));
```

---

### S7Client.MBRead(start, size[, callback])
Reads bytes from the PLC Merker (memory) area.

- `start` (number): Byte offset.
- `size` (number): Number of bytes to read.
- `callback` (optional): Function `(err, buffer)` called on completion.

**Returns:**  
- If no callback is given: Returns a `Promise<Buffer>`.
- If callback is given: Calls `callback(err, buffer)`.

**Details:**  
Convenience wrapper for `ReadArea()` with `area = S7Client.S7AreaMK` and `wordLen = S7Client.S7WLByte`.

**Example:**
```js
// Promise
const buf = await client.MBRead(0, 8);
// Callback
client.MBRead(0, 8, (err, buf) => { if (err) throw err; });
// Sync
const bufSync = client.MBReadSync(0, 8);
```

---

### S7Client.MBWrite(start, size, buffer[, callback])
Writes bytes to the PLC Merker (memory) area.

- `start` (number): Byte offset.
- `size` (number): Number of bytes to write.
- `buffer` (Buffer): Data to write.
- `callback` (optional): Function `(err)` called on completion.

**Returns:**  
- If no callback is given: Returns a `Promise<void>`.
- If callback is given: Calls `callback(err)`.

**Details:**  
Convenience wrapper for `WriteArea()` with `area = S7Client.S7AreaMK` and `wordLen = S7Client.S7WLByte`.

**Example:**
```js
// Promise
await client.MBWrite(0, 4, Buffer.from([1,2,3,4]));
// Callback
client.MBWrite(0, 4, Buffer.from([1,2,3,4]), (err) => { if (err) throw err; });
// Sync
client.MBWriteSync(0, 4, Buffer.from([1,2,3,4]));
```

---

### S7Client.EBRead(start, size[, callback])
Reads bytes from the PLC input (Eingang) area.

- `start` (number): Byte offset.
- `size` (number): Number of bytes to read.
- `callback` (optional): Function `(err, buffer)` called on completion.

**Returns:**  
- If no callback is given: Returns a `Promise<Buffer>`.
- If callback is given: Calls `callback(err, buffer)`.

**Details:**  
Convenience wrapper for `ReadArea()` with `area = S7Client.S7AreaPE` and `wordLen = S7Client.S7WLByte`.

**Example:**
```js
// Promise
const buf = await client.EBRead(0, 8);
// Callback
client.EBRead(0, 8, (err, buf) => { if (err) throw err; });
// Sync
const bufSync = client.EBReadSync(0, 8);
```

---

### S7Client.EBWrite(start, size, buffer[, callback])
Writes bytes to the PLC input (Eingang) area.

- `start` (number): Byte offset.
- `size` (number): Number of bytes to write.
- `buffer` (Buffer): Data to write.
- `callback` (optional): Function `(err)` called on completion.

**Returns:**  
- If no callback is given: Returns a `Promise<void>`.
- If callback is given: Calls `callback(err)`.

**Details:**  
Convenience wrapper for `WriteArea()` with `area = S7Client.S7AreaPE` and `wordLen = S7Client.S7WLByte`.

**Example:**
```js
// Promise
await client.EBWrite(0, 4, Buffer.from([1,2,3,4]));
// Callback
client.EBWrite(0, 4, Buffer.from([1,2,3,4]), (err) => { if (err) throw err; });
// Sync
client.EBWriteSync(0, 4, Buffer.from([1,2,3,4]));
```

---

### S7Client.ABRead(start, size[, callback])
Reads bytes from the PLC output (Ausgang) area.

- `start` (number): Byte offset.
- `size` (number): Number of bytes to read.
- `callback` (optional): Function `(err, buffer)` called on completion.

**Returns:**  
- If no callback is given: Returns a `Promise<Buffer>`.
- If callback is given: Calls `callback(err, buffer)`.

**Details:**  
Convenience wrapper for `ReadArea()` with `area = S7Client.S7AreaPA` and `wordLen = S7Client.S7WLByte`.

**Example:**
```js
// Promise
const buf = await client.ABRead(0, 8);
// Callback
client.ABRead(0, 8, (err, buf) => { if (err) throw err; });
// Sync
const bufSync = client.ABReadSync(0, 8);
```

---

### S7Client.ABWrite(start, size, buffer[, callback])
Writes bytes to the PLC output (Ausgang) area.

- `start` (number): Byte offset.
- `size` (number): Number of bytes to write.
- `buffer` (Buffer): Data to write.
- `callback` (optional): Function `(err)` called on completion.

**Returns:**  
- If no callback is given: Returns a `Promise<void>`.
- If callback is given: Calls `callback(err)`.

**Details:**  
Convenience wrapper for `WriteArea()` with `area = S7Client.S7AreaPA` and `wordLen = S7Client.S7WLByte`.

**Example:**
```js
// Promise
await client.ABWrite(0, 4, Buffer.from([1,2,3,4]));
// Callback
client.ABWrite(0, 4, Buffer.from([1,2,3,4]), (err) => { if (err) throw err; });
// Sync
client.ABWriteSync(0, 4, Buffer.from([1,2,3,4]));
```

---

### S7Client.TMRead(start, amount[, callback])
Reads timer values from the PLC.

- `start` (number): Timer offset.
- `amount` (number): Number of timers to read.
- `callback` (optional): Function `(err, buffer)` called on completion.

**Returns:**  
- If no callback is given: Returns a `Promise<Buffer>`.
- If callback is given: Calls `callback(err, buffer)`.

**Details:**  
Convenience wrapper for `ReadArea()` with `area = S7Client.S7AreaTM` and `wordLen = S7Client.S7WLTimer`.

**Example:**
```js
// Promise
const buf = await client.TMRead(0, 4);
// Callback
client.TMRead(0, 4, (err, buf) => { if (err) throw err; });
// Sync
const bufSync = client.TMReadSync(0, 4);
```

---

### S7Client.TMWrite(start, amount, buffer[, callback])
Writes timer values to the PLC.

- `start` (number): Timer offset.
- `amount` (number): Number of timers to write.
- `buffer` (Buffer): Data to write.
- `callback` (optional): Function `(err)` called on completion.

**Returns:**  
- If no callback is given: Returns a `Promise<void>`.
- If callback is given: Calls `callback(err)`.

**Details:**  
Convenience wrapper for `WriteArea()` with `area = S7Client.S7AreaTM` and `wordLen = S7Client.S7WLTimer`.

**Example:**
```js
// Promise
await client.TMWrite(0, 2, Buffer.from([0x12, 0x34, 0x56, 0x78]));
// Callback
client.TMWrite(0, 2, Buffer.from([0x12, 0x34, 0x56, 0x78]), (err) => { if (err) throw err; });
// Sync
client.TMWriteSync(0, 2, Buffer.from([0x12, 0x34, 0x56, 0x78]));
```

---

### S7Client.CTRead(start, amount[, callback])
Reads counter values from the PLC.

- `start` (number): Counter offset.
- `amount` (number): Number of counters to read.
- `callback` (optional): Function `(err, buffer)` called on completion.

**Returns:**  
- If no callback is given: Returns a `Promise<Buffer>`.
- If callback is given: Calls `callback(err, buffer)`.

**Details:**  
Convenience wrapper for `ReadArea()` with `area = S7Client.S7AreaCT` and `wordLen = S7Client.S7WLCounter`.

**Example:**
```js
// Promise
const buf = await client.CTRead(0, 4);
// Callback
client.CTRead(0, 4, (err, buf) => { if (err) throw err; });
// Sync
const bufSync = client.CTReadSync(0, 4);
```

---

### S7Client.CTWrite(start, amount, buffer[, callback])
Writes counter values to the PLC.

- `start` (number): Counter offset.
- `amount` (number): Number of counters to write.
- `buffer` (Buffer): Data to write.
- `callback` (optional): Function `(err)` called on completion.

**Returns:**  
- If no callback is given: Returns a `Promise<void>`.
- If callback is given: Calls `callback(err)`.

**Details:**  
Convenience wrapper for `WriteArea()` with `area = S7Client.S7AreaCT` and `wordLen = S7Client.S7WLCounter`.

**Example:**
```js
// Promise
await client.CTWrite(0, 2, Buffer.from([0x12, 0x34, 0x56, 0x78]));
// Callback
client.CTWrite(0, 2, Buffer.from([0x12, 0x34, 0x56, 0x78]), (err) => { if (err) throw err; });
// Sync
client.CTWriteSync(0, 2, Buffer.from([0x12, 0x34, 0x56, 0x78]));
```

---

### S7Client.ReadMultiVars(items[, callback])
Reads multiple variables in a single call.

- `items`: `S7MultiVarRead[]` — Array of variable descriptors. Each descriptor is an object with:
  - `Area`: `number` — Memory area (see [Area Table](#area-table))
  - `WordLen`: `number` — Data type/length (see [WordLen Table](#wordlen-table))
  - `DBNumber` (optional): `number` — DB number (required if Area is `S7AreaDB`)
  - `Start`: `number` — Start offset (byte or bit, depending on WordLen)
  - `Amount`: `number` — Number of elements to read

- Returns: `Promise<S7MultiVarReadResult[]>` or calls `callback(err, results)`
- Sync: `ReadMultiVarsSync(items)`

**Type: S7MultiVarReadResult**
- `Result`: `number` — Error code (0 if OK)
- `Data`: `Buffer|null` — Data buffer or null if error

**Example:**
```js
// Promise
const results = await client.ReadMultiVars([
  { Area: S7Client.S7AreaDB, WordLen: S7Client.S7WLByte, DBNumber: 1, Start: 0, Amount: 1 },
  { Area: S7Client.S7AreaCT, WordLen: S7Client.S7WLCounter, Start: 0, Amount: 8 }
]);
// Example result:
[
  { Result: 0, Data: <Buffer ...> },
  { Result: 0, Data: <Buffer ...> }
]

// Callback
client.ReadMultiVars([
  { Area: S7Client.S7AreaDB, WordLen: S7Client.S7WLByte, DBNumber: 1, Start: 0, Amount: 1 },
  { Area: S7Client.S7AreaCT, WordLen: S7Client.S7WLCounter, Start: 0, Amount: 8 }
], (err, results) => {
  // results: same as above
});

// Sync
const resultsSync = client.ReadMultiVarsSync([
  { Area: S7Client.S7AreaDB, WordLen: S7Client.S7WLByte, DBNumber: 1, Start: 0, Amount: 1 },
  { Area: S7Client.S7AreaCT, WordLen: S7Client.S7WLCounter, Start: 0, Amount: 8 }
]);
// resultsSync: same as above
```

---

### S7Client.WriteMultiVars(items[, callback])
Writes multiple variables in a single call.

- `items`: `S7MultiVarWrite[]` — Array of variable descriptors. Each descriptor is an object with:
  - `Area`: `number` — Memory area (see [Area Table](#area-table))
  - `WordLen`: `number` — Data type/length (see [WordLen Table](#wordlen-table))
  - `DBNumber` (optional): `number` — DB number (required if Area is `S7AreaDB`)
  - `Start`: `number` — Start offset (byte or bit, depending on WordLen)
  - `Amount`: `number` — Number of elements to write
  - `Data`: `Buffer` — Data to write

- Returns: `Promise<S7MultiVarWriteResult[]>` or calls `callback(err, results)`
- Sync: `WriteMultiVarsSync(items)`

**Type: S7MultiVarWriteResult**
- `Result`: `number` — Error code (0 if OK)

**Example:**
```js
// Promise
const results = await client.WriteMultiVars([
  { Area: S7Client.S7AreaDB, WordLen: S7Client.S7WLByte, DBNumber: 1, Start: 0, Amount: 1, Data: Buffer.from([1]) },
  { Area: S7Client.S7AreaMK, WordLen: S7Client.S7WLWord, Start: 2, Amount: 2, Data: Buffer.from([0, 1, 0, 2]) }
]);
// Example result:
[
  { Result: 0 },
  { Result: 0 }
]

// Callback
client.WriteMultiVars([
  { Area: S7Client.S7AreaDB, WordLen: S7Client.S7WLByte, DBNumber: 1, Start: 0, Amount: 1, Data: Buffer.from([1]) },
  { Area: S7Client.S7AreaMK, WordLen: S7Client.S7WLWord, Start: 2, Amount: 2, Data: Buffer.from([0, 1, 0, 2]) }
], (err, results) => {
  // results: same as above
});

// Sync
const resultsSync = client.WriteMultiVarsSync([
  { Area: S7Client.S7AreaDB, WordLen: S7Client.S7WLByte, DBNumber: 1, Start: 0, Amount: 1, Data: Buffer.from([1]) },
  { Area: S7Client.S7AreaMK, WordLen: S7Client.S7WLWord, Start: 2, Amount: 2, Data: Buffer.from([0, 1, 0, 2]) }
]);
// resultsSync: same as above
```
