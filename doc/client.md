 ## S7Client
 - [Control functions](#control-functions)
   - [Connect()](#connect)
   - [ConnectTo()](#connect-to)
   - [SetConnectionParams()](#set-connection-params)
   - [SetConnectionType()](#set-connection-type)
   - [Disconnect()](#disconnect)
   - [GetParam()](#get-param)
   - [SetParam()](#set-param)
 - [Data I/O functions](#data-functions)
   - [ReadArea()](#read-area)
   - [WriteArea()](#write-area)
   - [DBRead()](#dbread)
   - [DBWrite()](#dbwrite)
   - [ABRead()](#abread)
   - [ABWrite()](#abwrite)
   - [EBRead()](#ebread)
   - [EBWrite()](#ebwrite)
   - [MBRead()](#mbread)
   - [MBWrite()](#mbwrite)
   - [TMRead()](#tmread)
   - [TMWrite()](#tmwrite)
   - [CTRead()](#ctread)
   - [CTWrite()](#ctwrite)
   - [ReadMultiVars()](#read-multi-vars)
   - [WriteMultiVars()](#write-multi-vars)
 - [Directory function](#directory-functions)
   - [ListBlocks()](#list-blocks)
   - [ListBlocksOfType()](#list-blocks-of-type)
   - [GetAgBlockInfo()](#get-ag-blockinfo)
   - [GetPgBlockInfo()](#get-pg-blockinfo)
 - [Block oriented functions](#block-functions)
   - [FullUpload()](#full-upload)
   - [Upload()](#upload)
   - [Download()](#download)
   - [Delete()](#delete)
   - [DBGet()](#dbget)
   - [DBFill()](#dbfill)
 - [Date/Time functions](#datetime-functions)
   - [GetPlcDateTime()](#get-plc-datetime)
   - [SetPlcDateTime()](#set-plc-datetime)
   - [SetPlcSystemDateTime()](#set-plc-system-datetime)
 - [System info functions](#systeminfo-functions)
   - [ReadSZL()](#read-szl)
   - [ReadSZLList()](#read-szl-list)
   - [GetOrderCode()](#get-order-code)
   - [GetCpuInfo()](#get-cpu-info)
   - [GetCpInfo()](#get-cp-info)
 - [PLC control functions](#control-functions)
   - [PlcHotStart()](#plc-hot-start)
   - [PlcColdStart()](#plc-cold-start)
   - [PlcStop()](#plc-stop)
   - [CopyRamToRom()](#copy-ram-to-rom)
   - [Compress()](#compress)
 - [Security functions](#security-functions)
   - [SetSessionPassword()](#set-session-password)
   - [ClearSessionPassword()](#clear-session-password)
   - [GetProtection()](#get-protection)
 - [Properties](#properties)
   - [ExecTime()](#exec-time)
   - [LastError()](#last-error)
   - [PDURequested()](#pdu-requested)
   - [PDULength()](#pdu-length)
   - [PlcStatus()](#plc-status)
   - [Connected()](#connected)
   - [ErrorText()](#error-text)

### <a name="control-functions"></a>API - Control functions

----------

#### <a name="connect"></a>S7Client.Connect([callback])
Connects the client to the PLC with the parameters specified in the previous call of `ConnectTo()` or `SetConnectionParams()`.

 - The optional `callback` parameter will be executed after connection attempt

If `callback` is **not** set the function is **blocking** and  returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

#### <a name="connect-to"></a>S7Client.ConnectTo(ip, rack, slot[, callback])
Connects the client to the hardware at `ip`, `rack`, `slot` coordinates.

- `ip` PLC/Equipment IPV4 Address ex. “192.168.1.12”
- `rack` PLC Rack number
- `slot` PLC Slot number
- The optional `callback` parameter will be executed after connection attempt

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

#### <a name="set-connection-params"></a>S7Client.SetConnectionParams(ip, localTSAP, remoteTSAP)
Sets internally `ip`, `localTSAP`, `remoteTSAP` coordinates.

- `ip` PLC/Equipment IPv4 address ex. “192.168.1.12”
- `localTSAP` Local TSAP (PC TSAP)
- `remoteTSAP`  Remote TSAP (PLC TSAP)

Returns `true` on success or `false` on error.

#### <a name="set-connection-type"></a>S7Client.SetConnectionType(type)
Sets the connection resource type, i.e the way in which the Clients connects to a PLC.

- `type`

| ConnectionType            | Value      | Description |
|:--------------------------|:----------:|:------------|
| `S7Client.CONNTYPE_PG`    | 0x01       | PG
| `S7Client.CONNTYPE_OP`    | 0x02       | OP
| `S7Client.CONNTYPE_BASIC` | 0x03..0x10 | S7 Basic

#### <a name="disconnect"></a>S7Client.Disconnect()
Disconnects “gracefully” the Client from the PLC.

Returns `true` on success or `false` on error.

#### <a name="get-param"></a>S7Client.GetParam(paramNumber)
Reads an internal Client object parameter.

 - `paramNumber` One from the parameter list below

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

Returns the `parameter value` on success or `false` on error.

#### <a name="set-param"></a>S7Client.SetParam(paramNumber, value)
Sets an internal Client object parameter.

 - `paramNumber` One from the parameter list above
 - `value` New parameter value

Returns `true` on success or `false` on error.

### <a name="data-functions"></a>API - Data I/O functions

----------

#### <a name="read-area"></a>S7Client.ReadArea(area, dbNumber, start, amount, wordLen[, callback])
This is the main function to read data from a PLC. With it you can read DB, Inputs, Outputs, Merkers, Timers and Counters.

 - `area` Area identifier (see table [below](#table-area))
 - `dbNumber` DB number if area = S7AreaDB, otherwise ignored
 - `start` Offset to start
 - `amount` Amount of **words** to read
 - `wordLen` Word size (see table [below](#table-wordlen))
 - The optional `callback` parameter will be executed after read

If `callback` is **not** set the function is **blocking** and returns a `buffer` object on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

#### <a name="write-area"></a>S7Client.WriteArea(area, dbNumber, start, amount, wordLen, buffer[, callback])
This is the main function to write data into a PLC.

 - `area` Area identifier (see table [below](#table-area))
 - `dbNumber` DB number if area = S7AreaDB, otherwise ignored
 - `start` Offset to start
 - `amount` Amount of **words** to write
 - `wordLen` Word size (see table [below](#table-wordlen))
 - `buffer` User buffer
 - The optional `callback` parameter will be executed after write

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

<a name="table-area"></a>

| Area                   | Value | Description |
|:-----------------------|:-----:|:------------|
| `S7Client.S7AreaPE`    | 0x81  | Process inputs
| `S7Client.S7AreaPA`    | 0x82  | Process outputs
| `S7Client.S7AreaMK`    | 0x83  | Merkers
| `S7Client.S7AreaDB`    | 0x84  | DB
| `S7Client.S7AreaCT`    | 0x1C  | Counters
| `S7Client.S7AreaTM`    | 0x1D  | Timers

<a name="table-wordlen"></a>

| WordLen                | Value | Description |
|:-----------------------|:-----:|:------------|
| `S7Client.S7WLBit`     | 0x01  | Bit (inside a word)
| `S7Client.S7WLByte`    | 0x02  | Byte (8 bit)
| `S7Client.S7WLWord`    | 0x04  | Word (16 bit)
| `S7Client.S7WLDWord`   | 0x06  | Double Word (32 bit)
| `S7Client.S7WLReal`    | 0x08  | Real (32 bit float)
| `S7Client.S7WLCounter` | 0x1C  | Counter (16 bit)
| `S7Client.S7WLTimer`   | 0x1D  | Timer (16 bit)

#### <a name="dbread"></a>S7Client.DBRead(dbNumber, start, size[, callback])
This is a lean function of `ReadArea()` to read PLC DB.
It simply internally calls `ReadArea()` with `area = S7Client.S7AreaDB` and `wordLen = s7client.S7WLByte`.

 - `dbNumber` DB number
 - `start` Offset to start
 - `size` Size to read (bytes)
 - The optional `callback` parameter will be executed after read

If `callback` is **not** set the function is **blocking** and returns a `buffer` object on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

#### <a name="dbwrite"></a>S7Client.DBWrite(dbNumber, start, size, buffer[, callback])
This is a lean function of `WriteArea()` to write PLC DB.
It simply internally calls `WriteArea()` with `area = S7Client.S7AreaDB` and `wordLen = s7client.S7WLByte`.

 - `dbNumber` DB number
 - `start` Offset to start
 - `size` Size to write (bytes)
 - `buffer` User buffer
 - The optional `callback` parameter will be executed after write

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

#### <a name="abread"></a>S7Client.ABRead(start, size[, callback])
This is a lean function of `ReadArea()` to read PLC process outputs.
It simply internally calls `ReadArea()` with `area = S7Client.S7AreaPA` and `wordLen = s7client.S7WLByte`.

 - `start` Offset to start
 - `size` Size to read (bytes)
 - The optional `callback` parameter will be executed after read

If `callback` is **not** set the function is **blocking** and returns a `buffer` object on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

#### <a name="abwrite"></a>S7Client.ABWrite(start, size, buffer[, callback])
This is a lean function of `WriteArea()` to write PLC process outputs.
It simply internally calls `WriteArea()` with `area = S7Client.S7AreaPA` and `wordLen = s7client.S7WLByte`.

 - `start` Offset to start
 - `size` Size to write (bytes)
 - `buffer` User buffer
 - The optional `callback` parameter will be executed after write

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

#### <a name="ebread"></a>S7Client.EBRead(start, size[, callback])
This is a lean function of `ReadArea()` to read PLC process inputs.
It simply internally calls `ReadArea()` with `area = S7Client.S7AreaPE` and `wordLen = s7client.S7WLByte`.

 - `start` Offset to start
 - `size` Size to read (bytes)
 - The optional `callback` parameter will be executed after read

If `callback` is **not** set the function is **blocking** and returns a `buffer` object on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

#### <a name="ebwrite"></a>S7Client.EBWrite(start, size, buffer[, callback])
This is a lean function of `WriteArea()` to write PLC process inputs.
It simply internally calls `WriteArea()` with `area = S7Client.S7AreaPE` and `wordLen = s7client.S7WLByte`.

 - `start` Offset to start
 - `size` Size to write (bytes)
 - `buffer` User buffer
 - The optional `callback` parameter will be executed after write

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

#### <a name="mbread"></a>S7Client.MBRead(start, size[, callback])
This is a lean function of `ReadArea()` to read PLC Merkers.
It simply internally calls `ReadArea()` with `area = S7Client.S7AreaMK` and `wordLen = s7client.S7WLByte`.

 - `start` Offset to start
 - `size` Size to read (bytes)
 - The optional `callback` parameter will be executed after read

If `callback` is **not** set the function is **blocking** and returns a `buffer` object on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

#### <a name="mbwrite"></a>S7Client.MBWrite(start, size, buffer[, callback])
This is a lean function of `WriteArea()` to write PLC Merkers.
It simply internally calls `WriteArea()` with `area = S7Client.S7AreaMK` and `wordLen = s7client.S7WLByte`.

 - `start` Offset to start
 - `size` Size to write (bytes)
 - `buffer` User buffer
 - The optional `callback` parameter will be executed after write

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

#### <a name="tmread"></a>S7Client.TMRead(start, amount[, callback])
This is a lean function of `ReadArea()` to read PLC Timers.
It simply internally calls `ReadArea()` with `area = S7Client.S7AreaTM` and `wordLen = S7Client.S7WLTimer`.

 - `start` Offset to start
 - `amount` Number of timers
 - The optional `callback` parameter will be executed after read

If `callback` is **not** set the function is **blocking** and returns a `buffer` object on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

#### <a name="tmwrite"></a>S7Client.TMWrite(start, amount, buffer[, callback])
This is a lean function of `WriteArea()` to write PLC Timers.
It simply internally calls `WriteArea()` with `area = S7Client.S7AreaTM` and `wordLen = S7Client.S7WLTimer`.

 - `start` Offset to start
 - `amount` Number of timers
 - `buffer` User buffer
 - The optional `callback` parameter will be executed after write

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

#### <a name="ctread"></a>S7Client.CTRead(start, amount[, callback])
This is a lean function of `ReadArea()` to read PLC Counters.
It simply internally calls `ReadArea()` with `area = S7Client.S7AreaCT` and `wordLen = S7Client.S7WLCounter`.

 - `start` Offset to start
 - `amount` Number of counters
 - The optional `callback` parameter will be executed after read

If `callback` is **not** set the function is **blocking** and returns a `buffer` object on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

#### <a name="ctwrite"></a>S7Client.CTWrite(start, amount, buffer[, callback])
This is a lean function of `WriteArea()` to write PLC Counters.
It simply internally calls `WriteArea()` with `area = S7Client.S7AreaCT` and `wordLen = S7Client.S7WLCounter`.

 - `start` Offset to start
 - `amount` Number of counters
 - `buffer` User buffer
 - The optional `callback` parameter will be executed after write

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

#### <a name="read-multi-vars"></a>S7Client.ReadMultiVars(multiVars[, callback])
This is function allows to read different kind of variables from a PLC in a single call. With it you can read DB, Inputs, Outputs, Merkers, Timers and Counters.

 - `multiVars` Array of objects with read information (see structure below)
 - The optional `callback` parameter will be executed after read

If `callback` is **not** set the function is **blocking** and returns an `array` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

Example:
```javascript
// multiVars array structure
[
  {
    "Area": S7Client.S7AreaDB,
    "WordLen": S7Client.S7WLByte,
    "DBNumber": 1,
    "Start": 0,
    "Amount": 1
  },
  {
    "Area": S7Client.S7AreaCT,
    "WordLen": S7Client.S7WLCounter,
    "Start": 0,
    "Amount": 8
  },
  {
    "Area": S7Client.S7AreaPA,
    "WordLen": S7Client.S7WLByte,
    "Start": 0,
    "Amount": 16
  },
  ...
]
// result array
[
  {
    "Result": 0, // Error code
    "Data": ...  // Buffer object or null if Result <> 0
  },
  ...
]
```

Since could happen that some variables are read, some other not because maybe they don't exist in PLC. It is important to check the single item result.

Due the different kind of variables involved , there is no split feature available for this function, so the maximum data size must not exceed the PDU size.
The advantage of this function becomes big when you have many small non-contiguous variables to be read.

#### <a name="write-multi-vars"></a>S7Client.WriteMultiVars(multiVars[, callback])
This is function allows to write different kind of variables into a PLC in a single call. With it you can write DB, Inputs, Outputs, Merkers, Timers and Counters.

 - `multiVars` Array of objects with write information (see structure below)
 - The optional `callback` parameter will be executed after write

If `callback` is **not** set the function is **blocking** and returns an `array` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

Example:
```javascript
// multiVars array structure
[
  {
    "Area": S7Client.S7AreaDB,
    "WordLen": S7Client.S7WLByte,
    "DBNumber": 1,
    "Start": 0,
    "Amount": 1,
    "Data": buffer1 // Buffer variable
  },
  {
    "Area": S7Client.S7AreaCT,
    "WordLen": S7Client.S7WLCounter,
    "Start": 0,
    "Amount": 8,
    "Data": buffer2 // Buffer variable
  },
  {
    "Area": S7Client.S7AreaPA,
    "WordLen": S7Client.S7WLByte,
    "Start": 0,
    "Amount": 16,
    "Data": buffer3 // Buffer variable
  },
  ...
]
// result array
[
  {
    "Result": 0 // Error code
  },
  ...
]
```

### <a name="directory-functions"></a>API - Directory functions

----------

#### <a name="list-blocks"></a>S7Client.ListBlocks([callback])
This function returns an object of the AG blocks amount divided by type.

- The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns an `object` (see below) on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

Example:
```javascript
{
  "OBCount": 0,
  "FBCount": 0,
  "FCCount": 0,
  "SFBCount": 0,
  "SFCCount": 0,
  "DBCount": 0,
  "SDBCount": 0
}
```

#### <a name="list-blocks-of-type"></a>S7Client.ListBlocksOfType(blockType[, callback])
This function returns an array of the AG list of a specified block type.

 - `blockType` Type of block (see table below)
 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns an `array` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

Each item of the result array will contain a block number.

<a name="table-blocktype"></a>

| BlockType              | Value | Description |
|:-----------------------|:-----:|:------------|
| `S7Client.Block_OB`    | 0x38  | OB
| `S7Client.Block_DB`    | 0x41  | DB
| `S7Client.Block_SDB`   | 0x42  | SDB
| `S7Client.Block_FC`    | 0x43  | FC
| `S7Client.Block_SFC`   | 0x44  | SFC
| `S7Client.Block_FB`    | 0x45  | FB
| `S7Client.Block_SFB`   | 0x46  | SFB

#### <a name="get-ag-blockinfo"></a>S7Client.GetAgBlockInfo(blockType, blockNum[, callback])
Returns an object with detailed information about a given AG block.
This function is very useful if you need to read or write data in a DB which you do not know the size in advance (see MC7Size field)

 - `blockType` Type of block (see table [above](#table-blocktype))
 - `blockNum` Number of block
 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns an `object` (see below) on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

Example:
<a name="example-blockinfo"></a>
```javascript
{
  "BlkType": ,   // Block Type (see SubBlkType table)
  "BlkNumber": , // Block number
  "BlkLang": ,   // Block Language (see LangType Table)
  "BlkFlags": ,  // Block flags (bitmapped)
  "MC7Size": ,   // The real size in bytes
  "LoadSize": ,  // Load memory size
  "LocalData": , // Local data
  "SBBLength": , // SBB Length
  "CheckSum": ,  // Checksum
  "Version": ,   // Version (BCD 00<HI><LO>)
  "CodeDate": ,  // Code date
  "IntfDate": ,  // Interface date
  "Author": ,    // Author
  "Family": ,    // Family
  "Header":      // Header
}
```

| SubBlockType           | Value | Description |
|:-----------------------|:-----:|:------------|
| `S7Client.SubBlk_OB`   | 0x08  | OB
| `S7Client.SubBlk_DB`   | 0x0A  | DB
| `S7Client.SubBlk_SDB`  | 0x0B  | SDB
| `S7Client.SubBlk_FC`   | 0x0C  | FC
| `S7Client.SubBlk_SFC`  | 0x0D  | SFC
| `S7Client.SubBlk_FB`   | 0x0E  | FB
| `S7Client.SubBlk_SFB`  | 0x0F  | SFB

| LangType                  | Value | Description |
|:--------------------------|:-----:|:------------|
| `S7Client.BlockLangAWL`   | 0x01  | AWL
| `S7Client.BlockLangKOP`   | 0x02  | KOP
| `S7Client.BlockLangFUP`   | 0x03  | FUP
| `S7Client.BlockLangSCL`   | 0x04  | SCL
| `S7Client.BlockLangDB`    | 0x05  | DB
| `S7Client.BlockLangGRAPH` | 0x06  | GRAPH

#### <a name="get-pg-blockinfo"></a>S7Client.GetPgBlockInfo(buffer)
Returns detailed information about a block present in a user buffer. This function is usually used in conjunction with `FullUpload()`.
An uploaded block saved to disk, could be loaded in a user buffer and checked with this function.

 - `buffer` User buffer

Returns an `object` (see [example](#example-blockinfo) above) on success or `false`on error.

### <a name="block-functions"></a>API - Block oriented functions

----------

#### <a name="full-upload"></a>S7Client.FullUpload(blockType, blockNum[, callback])
Uploads a block from AG. The whole block (including header and footer) is copied into the user buffer.

 - `blockType` Type of block (see table [above](#table-blocktype))
 - `blockNum` Number of block
 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns a `Buffer` object on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

#### <a name="upload"></a>S7Client.Upload(blockType, blockNum[, callback])
Uploads a block body from AG. Only the block body (but header and footer) is copied into the user buffer.

 - `blockType` Type of block (see table [above](#table-blocktype))
 - `blockNum` Number of block
 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns a `Buffer` object on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

#### <a name="download"></a>S7Client.Download(blockNum, buffer[, callback])
Downloads a block into AG. A whole block (including header and footer) must be available into the user buffer.

 - `blockNum` Number of block
 - `buffer` User buffer
 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

If the parameter `blockNum` is `-1`, the block number is not changed else the block is downloaded with the provided number (just like a “Download As…”).

#### <a name="delete"></a>S7Client.Delete(blockType, blockNum[, callback])
Deletes a block into AG.

    !!! There is no undo function available !!!

 - `blockType` Type of block (see table [above](#table-blocktype))
 - `blockNum` Number of block
 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

#### <a name="dbget"></a>S7Client.DBGet(dbNumber[, callback])
Uploads a DB from AG. This function is equivalent to `Upload()` with `BlockType = Block_DB` but it uses a different approach so it’s not subject to the security level set.

Only data is uploaded.

 - `dbNumber` DB number
 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns a `Buffer` object on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

This function first gathers the DB size via `GetAgBlockInfo()` then calls `DBRead()`.

#### <a name="dbfill"></a>S7Client.DBFill(dbNumber, fillChar[, callback])
Fills a DB in AG with a given byte without the need of specifying its size.

 - `dbNumber` DB number
 - `fillChar` char or char code
 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

### <a name="datetime-functions"></a>API - Date/Time functions

----------

#### <a name="get-plc-datetime"></a>S7Client.GetPlcDateTime([callback])
Reads PLC date and time.

- The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns a javascript `Date()` object on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

#### <a name="set-plc-datetime"></a>S7Client.SetPlcDateTime(dateTime[, callback])
Sets the PLC date and time.

 - `dateTime`
 - The optional `callback` parameter will be executed after completion

The `dateTime` argument can be a javascript `Date()` object or an object with the structure below.
```javascript
{
  "year": 2015,  // year
  "month": 4,    // months since January     0-11
  "day": 3,      // day of the month         1-31
  "hours": 19,   // hours since midnight     0-23
  "minutes": 37, // minutes after the hour   0-59
  "seconds": 0   // seconds after the minute 0-59
}
```

If `callback` is **not** set the function is **blocking** and returns a `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

#### <a name="set-plc-system-datetime"></a>S7Client.SetPlcSystemDateTime([callback])
Sets the PLC date and time in accord to the PC system Date/Time.

 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

### <a name="systeminfo-functions"></a>API - System info functions

----------

#### <a name="read-szl"></a>S7Client.ReadSZL(id, index[, callback])
Reads a partial list of given `id`and `index`.

 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns a `buffer` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

#### <a name="read-szl-list"></a>S7Client.ReadSZLList([callback])
Reads the directory of the partial lists.

 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns an `array` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

#### <a name="get-order-code"></a>S7Client.GetOrderCode([callback])
Gets CPU order code and version info.

 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns an `object` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

Example:
```javascript
{
  "Code": , // Order Code
  "V1": ,   // Version V1.V2.V3
  "V2": ,
  "V3":
}
```

#### <a name="get-cpu-info"></a>S7Client.GetCpuInfo([callback])
Gets CPU module name, serial number and other info.

 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns an `object` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

Example:
```javascript
{
  "ModuleTypeName": ,
  "SerialNumber": ,
  "ASName": ,
  "Copyright": ,
  "ModuleName":
}
```

#### <a name="get-cp-info"></a>S7Client.GetCpInfo([callback])
Gets CP (communication processor) info.

 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns an `object` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

Example:
```javascript
{
  "MaxPduLength": ,
  "MaxConnections": ,
  "MaxMpiRate": ,
  "MaxBusRate":
}
```

### <a name="control-functions"></a>API - PLC control functions

----------

#### <a name="plc-hot-start"></a>S7Client.PlcHotStart([callback])
Puts the CPU in RUN mode performing an HOT START.

 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

#### <a name="plc-cold-start"></a>S7Client.PlcColdStart([callback])
Puts the CPU in RUN mode performing a COLD START.

 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

#### <a name="plc-stop"></a>S7Client.PlcStop([callback])
Puts the CPU in STOP mode.

 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

#### <a name="copy-ram-to-rom"></a>S7Client.CopyRamToRom(timeout[, callback])
Performs the Copy Ram to Rom action.

 - `timeout` Maximum time expected to complete the operation (ms)
 - The optional `callback` parameter will be executed after completion or on timeout

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

Not all CPUs support this operation.
The CPU must be in STOP mode.

#### <a name="compress"></a>S7Client.Compress(timeout[, callback])
Performs the Memory compress action.

 - `timeout` Maximum time expected to complete the operation (ms)
 - The optional `callback` parameter will be executed after completion or on timeout

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

### <a name="security-functions"></a>API - Security functions

----------

#### <a name="set-session-password"></a>S7Client.SetSessionPassword(password[, callback])
Send the password to the PLC to meet its security level.

 - `password` Password
 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

A `password` accepted by a PLC is an 8 chars string, a longer password will be trimmed, and a shorter one will be "right space padded".

#### <a name="clear-session-password"></a>S7Client.ClearSessionPassword([callback])
Clears the password set for the current session (logout).

 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

#### <a name="get-protection"></a>S7Client.GetProtection([callback])
Gets the CPU protection level info.

 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns the protection object on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

Example:
```javascript
{
  "sch_schal": 1,
  "sch_par": 0,
  "sch_rel": 0,
  "bart_sch": 1,
  "anl_sch": 0
}
```

| S7Protection | Values  | Description |
|:-------------|:--------|:------------|
| `sch_schal`  | 1,2,3   | Protection level set with the mode selector
| `sch_par`    | 0,1,2,3 | Password level, 0 : no password
| `sch_rel`    | 0,1,2,3 | Valid protection level of the CPU
| `bart_sch`   | 1,2,3,4 | Mode selector setting (1:RUN, 2:RUN-P, 3:STOP, :MRES, 0:undefined or cannot be determined)
| `anl_sch`    | 0,1,2   | Startup switch setting (1:CRST, 2:WRST, 0:undefined, does not exist of cannot be determined)

### <a name="properties"></a>API - Properties

----------

#### <a name="exec-time"></a>S7Client.ExecTime()
Returns the last job execution time in milliseconds or `false`on error.

#### <a name="last-error"></a>S7Client.LastError()
Returns the last job result.

#### <a name="pdu-requested"></a>S7Client.PDURequested()
Returns the PDU length requested by the client or `false` on error. The requested PDU length can be modified with [SetParam()](#set-param).

#### <a name="pdu-length"></a>S7Client.PDULength()
Returns the PDU length negotiated between the client and the PLC during the connection or `false` on error.

It’s useful to know the PDU negotiated when we need to call `ReadMultivar()` or `WriteMultiVar()`. All other data transfer functions handle this information by themselves and split the telegrams automatically if needed.

#### <a name="plc-status"></a>S7Client.PlcStatus([callback])
Returns the CPU status (running/stopped).

 - The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns the CPU status on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` and `result` argument is given to the callback.

| Status                        | Value | Description |
|:------------------------------|:-----:|:------------|
| `S7Client.S7CpuStatusUnknown` | 0x00  | The CPU status is unknown
| `S7Client.S7CpuStatusRun`     | 0x08  | The CPU is running
| `S7Client.S7CpuStatusStop`    | 0x04  | The CPU is stopped

#### <a name="connected"></a>S7Client.Connected()
Returns the connection status.

#### <a name="error-text"></a>S7Client.ErrorText(errNum)
Returns a textual explanation of a given error number.

 - `errNum` Error number
