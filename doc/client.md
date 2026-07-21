# S7Client API

High-level access to Siemens S7 PLCs via Snap7. Most methods support Promise and callback forms plus a `Sync` variant.

- Promise form: resolves with the value below; rejects with `Snap7Error` (`code`, `errno`).
- Callback form: pass `(err, result)` as the last argument; function returns `undefined`.
- Sync form: `*Sync` variant returns the value or `undefined`, or throws `Snap7Error`.

---

## Table of Contents
- [Usage basics](#usage-basics)
- [Control functions](#control-functions)
  - [Connect](#connect)
  - [ConnectTo](#connectto)
  - [SetConnectionParams](#setconnectionparams)
  - [SetConnectionType](#setconnectiontype)
  - [Disconnect](#disconnect)
  - [GetParam](#getparam)
  - [SetParam](#setparam)
- [Data I/O functions](#data-io-functions)
  - [ReadArea](#readarea)
  - [WriteArea](#writearea)
  - [DBRead](#dbread)
  - [DBWrite](#dbwrite)
  - [MBRead](#mbread)
  - [MBWrite](#mbwrite)
  - [EBRead](#ebread)
  - [EBWrite](#ebwrite)
  - [ABRead](#abread)
  - [ABWrite](#abwrite)
  - [TMRead](#tmread)
  - [TMWrite](#tmwrite)
  - [CTRead](#ctread)
  - [CTWrite](#ctwrite)
  - [ReadMultiVars](#readmultivars)
  - [WriteMultiVars](#writemultivars)
- [Directory functions](#directory-functions)
  - [ListBlocks](#listblocks)
  - [ListBlocksOfType](#listblocksoftype)
  - [GetAgBlockInfo](#getagblockinfo)
  - [GetPgBlockInfo](#getpgblockinfo)
- [Block operations](#block-operations)
  - [FullUpload](#fullupload)
  - [Upload](#upload)
  - [Download](#download)
  - [Delete](#delete)
  - [DBGet](#dbget)
  - [DBFill](#dbfill)
- [Date/Time functions](#datetime-functions)
  - [GetPlcDateTime](#getplcdatetime)
  - [SetPlcDateTime](#setplcdatetime)
  - [SetPlcSystemDateTime](#setplcsystemdatetime)
- [System info functions](#system-info-functions)
  - [ReadSZL](#readszl)
  - [ReadSZLList](#readszllist)
  - [GetOrderCode](#getordercode)
  - [GetCpuInfo](#getcpuinfo)
  - [GetCpInfo](#getcpinfo)
- [PLC control functions](#plc-control-functions)
  - [PlcHotStart](#plchotstart)
  - [PlcColdStart](#plccoldstart)
  - [PlcStop](#plcstop)
  - [CopyRamToRom](#copyramtorom)
  - [Compress](#compress)
  - [PlcStatus](#plcstatus)
- [Security functions](#security-functions)
  - [SetSessionPassword](#setsessionpassword)
  - [ClearSessionPassword](#clearsessionpassword)
  - [GetProtection](#getprotection)
- [Properties & diagnostics](#properties--diagnostics)
- [Constants](#constants)
  - [Areas](#areas)
  - [Word lengths](#word-lengths)
  - [Connection types](#connection-types)
  - [Block types](#block-types)
  - [Block languages](#block-languages)
  - [Sub block types](#sub-block-types)
  - [PLC status codes](#plc-status-codes)
  - [Client parameters](#client-parameters)
- [Type definitions](#type-definitions)
  - [BlocksList](#blockslist)
  - [BlockInfo](#blockinfo)
  - [OrderCode](#ordercode)
  - [CpuInfo](#cpuinfo)
  - [CpInfo](#cpinfo)
  - [Protection](#protection)
  - [DateTimeObject](#datetimeobject)
  - [S7MultiVarReadResult](#s7multivarreadresult)
  - [S7MultiVarWriteResult](#s7multivarwriteresult)

---

## Usage basics
- Use grouped constants from [Constants](#constants), e.g. `const { S7Area, S7WordLen, ConnectionType } = require('node-snap7');`.
- `start` is a byte offset unless `wordLen` is `S7WordLen.Bit`, in which case it is a bit index within the byte.
- `amount` counts elements of the chosen `S7WordLen` (bytes for `S7WordLen.Byte`, words for `S7WordLen.Word`, etc.).
- Buffers you pass must be large enough for `amount * S7WordLen` sized elements.

---

## Control functions

### Connect
```
Connect(): Promise<undefined>
Connect(callback: (err: Snap7Error | null) => undefined): undefined
ConnectSync(): undefined
```
Connect using the parameters set via `ConnectTo` or `SetConnectionParams`.
- Returns: Promise resolves with `undefined`; callback receives `(err)`; Sync returns `undefined` (throws on error)

### ConnectTo
```
ConnectTo(ip: string, rack: number, slot: number): Promise<undefined>
ConnectTo(ip: string, rack: number, slot: number, callback: (err: Snap7Error | null) => undefined): undefined
ConnectToSync(ip: string, rack: number, slot: number): undefined
```
Connect directly to the given IP, rack, and slot.
- Parameters:
  - `ip`: PLC IPv4 address
  - `rack`: rack number
  - `slot`: slot number
- Returns: Promise resolves with `undefined`; callback receives `(err)`; Sync returns `undefined` (throws on error)

### SetConnectionParams
```
SetConnectionParams(ip: string, localTSAP: number, remoteTSAP: number): undefined
```
Set ISO-on-TCP parameters prior to connecting.
- Parameters:
  - `ip`: PLC IPv4 address
  - `localTSAP`: local TSAP
  - `remoteTSAP`: remote TSAP
- Returns: `undefined`

### SetConnectionType
```
SetConnectionType(type: number): undefined
```
Set the connection resource type.
- Parameters:
  - `type`: see [Connection types](#connection-types)
- Returns: `undefined`

### Disconnect
```
Disconnect(): undefined
```
Close the connection.
- Returns: `undefined`

### GetParam
```
GetParam(paramNumber: number): number
```
Read a client parameter.
- Parameters:
  - `paramNumber`: see [Client parameters](#client-parameters)
- Returns: parameter value

### SetParam
```
SetParam(paramNumber: number, value: number): undefined
```
Write a client parameter.
- Parameters:
  - `paramNumber`: see [Client parameters](#client-parameters)
  - `value`: new value
- Returns: `undefined`

---

## Data I/O functions

### ReadArea
```
ReadArea(area: S7Area, dbNumber: number, start: number, amount: number, wordLen: S7WordLen): Promise<Buffer>
ReadArea(area: S7Area, dbNumber: number, start: number, amount: number, wordLen: S7WordLen, callback: (err: Snap7Error | null, data: Buffer) => undefined): undefined
ReadAreaSync(area: S7Area, dbNumber: number, start: number, amount: number, wordLen: S7WordLen): Buffer
```
Read raw data from the PLC.
- Parameters:
  - `area`: memory area
  - `dbNumber`: DB number (only for `S7Area.DB`)
  - `start`: byte offset (bit index if `S7WordLen.Bit`)
  - `amount`: element count in units of `wordLen`
  - `wordLen`: element type/size
- Returns: Promise resolves with `Buffer`; callback receives `(err, buffer)`; Sync returns `Buffer` (throws on error)

### WriteArea
```
WriteArea(area: S7Area, dbNumber: number, start: number, amount: number, wordLen: S7WordLen, buffer: Buffer): Promise<undefined>
WriteArea(area: S7Area, dbNumber: number, start: number, amount: number, wordLen: S7WordLen, buffer: Buffer, callback: (err: Snap7Error | null) => undefined): undefined
WriteAreaSync(area: S7Area, dbNumber: number, start: number, amount: number, wordLen: S7WordLen, buffer: Buffer): undefined
```
Write raw data to the PLC.
- Parameters:
  - `area`: memory area
  - `dbNumber`: DB number (only for `S7Area.DB`)
  - `start`: byte offset (bit index if `S7WordLen.Bit`)
  - `amount`: element count in units of `wordLen`
  - `wordLen`: element type/size
  - `buffer`: data to write
- Returns: Promise resolves with `undefined`; callback receives `(err)`; Sync returns `undefined` (throws on error)

### Convenience read/write wrappers
Each wrapper supports Promise/Callback/Sync variants like `Foo(...) -> Promise<...>`, `Foo(..., callback) -> undefined`, `FooSync(...) -> ...`.
- DB: `DBRead`, `DBWrite`
- Merker: `MBRead`, `MBWrite`
- Inputs: `EBRead`, `EBWrite`
- Outputs: `ABRead`, `ABWrite`
- Timers: `TMRead`, `TMWrite`
- Counters: `CTRead`, `CTWrite`

### ReadMultiVars
```
ReadMultiVars(items: { Area: S7Area; WordLen: S7WordLen; DBNumber?: number; Start: number; Amount: number }[]): Promise<S7MultiVarReadResult[]>
ReadMultiVars(items: { Area: S7Area; WordLen: S7WordLen; DBNumber?: number; Start: number; Amount: number }[], callback: (err: Snap7Error | null, data: S7MultiVarReadResult[]) => undefined): undefined
ReadMultiVarsSync(items: { Area: S7Area; WordLen: S7WordLen; DBNumber?: number; Start: number; Amount: number }[]): S7MultiVarReadResult[]
```
Read multiple addresses in one request.
- Parameters:
  - `items`: `{ Area: S7Area, WordLen: S7WordLen, DBNumber?, Start, Amount }`
- Returns: Promise resolves with `S7MultiVarReadResult[]`; callback receives `(err, results)`; Sync returns `S7MultiVarReadResult[]` (throws on error)

### WriteMultiVars
```
WriteMultiVars(items: { Area: S7Area; WordLen: S7WordLen; DBNumber?: number; Start: number; Amount: number; Data: Buffer }[]): Promise<S7MultiVarWriteResult[]>
WriteMultiVars(items: { Area: S7Area; WordLen: S7WordLen; DBNumber?: number; Start: number; Amount: number; Data: Buffer }[], callback: (err: Snap7Error | null, data: S7MultiVarWriteResult[]) => undefined): undefined
WriteMultiVarsSync(items: { Area: S7Area; WordLen: S7WordLen; DBNumber?: number; Start: number; Amount: number; Data: Buffer }[]): S7MultiVarWriteResult[]
```
Write multiple addresses in one request.
- Parameters:
  - `items`: `{ Area: S7Area, WordLen: S7WordLen, DBNumber?, Start, Amount, Data: Buffer }`
- Returns: Promise resolves with `S7MultiVarWriteResult[]`; callback receives `(err, results)`; Sync returns `S7MultiVarWriteResult[]` (throws on error)

---

## Directory functions

### ListBlocks
```
ListBlocks(): Promise<BlocksList>
ListBlocks(callback: (err: Snap7Error | null, data: BlocksList) => undefined): undefined
ListBlocksSync(): BlocksList
```
Get a count of blocks on the PLC.
- Returns: Promise resolves with `BlocksList`; callback receives `(err, blocks)`; Sync returns `BlocksList` (throws on error)

### ListBlocksOfType
```
ListBlocksOfType(blockType: BlockType): Promise<number[]>
ListBlocksOfType(blockType: BlockType, callback: (err: Snap7Error | null, data: number[]) => undefined): undefined
ListBlocksOfTypeSync(blockType: BlockType): number[]
```
List block numbers of a specific type.
- Parameters:
  - `blockType`: block type code
- Returns: Promise resolves with `number[]`; callback receives `(err, numbers)`; Sync returns `number[]` (throws on error)

### GetAgBlockInfo
```
GetAgBlockInfo(blockType: BlockType, blockNum: number): Promise<BlockInfo>
GetAgBlockInfo(blockType: BlockType, blockNum: number, callback: (err: Snap7Error | null, data: BlockInfo) => undefined): undefined
GetAgBlockInfoSync(blockType: BlockType, blockNum: number): BlockInfo
```
Get metadata for a block on the PLC.
- Parameters:
  - `blockType`: block type code
  - `blockNum`: block number
- Returns: Promise resolves with `BlockInfo`; callback receives `(err, info)`; Sync returns `BlockInfo` (throws on error)

### GetPgBlockInfo
```
GetPgBlockInfo(buffer: Buffer): BlockInfo
```
Get metadata for a block stored in a buffer.
- Parameters:
  - `buffer`: block buffer
- Returns: `BlockInfo` (throws on error)

---

## Block operations

### FullUpload
```
FullUpload(blockType: BlockType, blockNum: number, size: number): Promise<Buffer>
FullUpload(blockType: BlockType, blockNum: number, size: number, callback: (err: Snap7Error | null, data: Buffer) => undefined): undefined
FullUploadSync(blockType: BlockType, blockNum: number, size: number): Buffer
```
Download an entire block including headers.
- Parameters:
  - `blockType`: block type code
  - `blockNum`: block number
  - `size`: max bytes to read
- Returns: Promise resolves with `Buffer`; callback receives `(err, buffer)`; Sync returns `Buffer` (throws on error)

### Upload
```
Upload(blockType: BlockType, blockNum: number, size: number): Promise<Buffer>
Upload(blockType: BlockType, blockNum: number, size: number, callback: (err: Snap7Error | null, data: Buffer) => undefined): undefined
UploadSync(blockType: BlockType, blockNum: number, size: number): Buffer
```
Download the MC7 code portion of a block.
- Parameters:
  - `blockType`: block type code
  - `blockNum`: block number
  - `size`: max bytes to read
- Returns: Promise resolves with `Buffer`; callback receives `(err, buffer)`; Sync returns `Buffer` (throws on error)

### Download
```
Download(blockNum: number, buffer: Buffer): Promise<undefined>
Download(blockNum: number, buffer: Buffer, callback: (err: Snap7Error | null) => undefined): undefined
DownloadSync(blockNum: number, buffer: Buffer): undefined
```
Upload a compiled block buffer to the PLC.
- Parameters:
  - `blockNum`: block number
  - `buffer`: compiled block data
- Returns: Promise resolves with `undefined`; callback receives `(err)`; Sync returns `undefined` (throws on error)

### Delete
```
Delete(blockType: BlockType, blockNum: number): Promise<undefined>
Delete(blockType: BlockType, blockNum: number, callback: (err: Snap7Error | null) => undefined): undefined
DeleteSync(blockType: BlockType, blockNum: number): undefined
```
Delete a block on the PLC.
- Parameters:
  - `blockType`: block type code
  - `blockNum`: block number
- Returns: Promise resolves with `undefined`; callback receives `(err)`; Sync returns `undefined` (throws on error)

### DBGet
```
DBGet(dbNumber: number): Promise<Buffer>
DBGet(dbNumber: number, callback: (err: Snap7Error | null, data: Buffer) => undefined): undefined
DBGetSync(dbNumber: number): Buffer
```
Fetch the raw content of a DB.
- Parameters:
  - `dbNumber`: DB number
- Returns: Promise resolves with `Buffer`; callback receives `(err, buffer)`; Sync returns `Buffer` (throws on error)

### DBFill
```
DBFill(dbNumber: number, fillChar: number | string): Promise<undefined>
DBFill(dbNumber: number, fillChar: number | string, callback: (err: Snap7Error | null) => undefined): undefined
DBFillSync(dbNumber: number, fillChar: number | string): undefined
```
Fill an entire DB with a byte or character.
- Parameters:
  - `dbNumber`: DB number
  - `fillChar`: fill byte or single-character string
- Returns: Promise resolves with `undefined`; callback receives `(err)`; Sync returns `undefined` (throws on error)

---

## Date/Time functions

### GetPlcDateTime
```
GetPlcDateTime(): Promise<Date>
GetPlcDateTime(callback: (err: Snap7Error | null, data: Date) => undefined): undefined
GetPlcDateTimeSync(): Date
```
Read the PLC system clock.
- Returns: Promise resolves with `Date`; callback receives `(err, date)`; Sync returns `Date` (throws on error)

### SetPlcDateTime
```
SetPlcDateTime(dateTime: Date | [DateTimeObject](#datetimeobject)): Promise<undefined>
SetPlcDateTime(dateTime: Date | [DateTimeObject](#datetimeobject), callback: (err: Snap7Error | null) => undefined): undefined
SetPlcDateTimeSync(dateTime: Date | [DateTimeObject](#datetimeobject)): undefined
```
Set the PLC system clock.
- Parameters:
  - `dateTime`: desired time (Date or [DateTimeObject](#datetimeobject))
- Returns: Promise resolves with `undefined`; callback receives `(err)`; Sync returns `undefined` (throws on error)

### SetPlcSystemDateTime
```
SetPlcSystemDateTime(): Promise<undefined>
SetPlcSystemDateTime(callback: (err: Snap7Error | null) => undefined): undefined
SetPlcSystemDateTimeSync(): undefined
```
Set the PLC system clock to the host time.
- Returns: Promise resolves with `undefined`; callback receives `(err)`; Sync returns `undefined` (throws on error)

---

## System info functions

### ReadSZL
```
ReadSZL(id: number, index: number): Promise<Buffer>
ReadSZL(id: number, index: number, callback: (err: Snap7Error | null, data: Buffer) => undefined): undefined
ReadSZLSync(id: number, index: number): Buffer
```
Read a System-Zustands-Listen (SZL) record.
- Parameters:
  - `id`: SZL ID
  - `index`: SZL index
- Returns: Promise resolves with `Buffer`; callback receives `(err, buffer)`; Sync returns `Buffer` (throws on error)

### ReadSZLList
```
ReadSZLList(): Promise<number[]>
ReadSZLList(callback: (err: Snap7Error | null, data: number[]) => undefined): undefined
ReadSZLListSync(): number[]
```
List available SZL IDs.
- Returns: Promise resolves with `number[]`; callback receives `(err, ids)`; Sync returns `number[]` (throws on error)

### GetOrderCode
```
GetOrderCode(): Promise<OrderCode>
GetOrderCode(callback: (err: Snap7Error | null, data: OrderCode) => undefined): undefined
GetOrderCodeSync(): OrderCode
```
Retrieve the PLC order code.
- Returns: Promise resolves with `OrderCode`; callback receives `(err, code)`; Sync returns `OrderCode` (throws on error)

### GetCpuInfo
```
GetCpuInfo(): Promise<CpuInfo>
GetCpuInfo(callback: (err: Snap7Error | null, data: CpuInfo) => undefined): undefined
GetCpuInfoSync(): CpuInfo
```
Get CPU identification data.
- Returns: Promise resolves with `CpuInfo`; callback receives `(err, info)`; Sync returns `CpuInfo` (throws on error)

### GetCpInfo
```
GetCpInfo(): Promise<CpInfo>
GetCpInfo(callback: (err: Snap7Error | null, data: CpInfo) => undefined): undefined
GetCpInfoSync(): CpInfo
```
Get communication processor information.
- Returns: Promise resolves with `CpInfo`; callback receives `(err, info)`; Sync returns `CpInfo` (throws on error)

---

## PLC control functions

### PlcHotStart
```
PlcHotStart(): Promise<undefined>
PlcHotStart(callback: (err: Snap7Error | null) => undefined): undefined
PlcHotStartSync(): undefined
```
Warm-start the PLC.
- Returns: Promise resolves with `undefined`; callback receives `(err)`; Sync returns `undefined` (throws on error)

### PlcColdStart
```
PlcColdStart(): Promise<undefined>
PlcColdStart(callback: (err: Snap7Error | null) => undefined): undefined
PlcColdStartSync(): undefined
```
Cold-start the PLC.
- Returns: Promise resolves with `undefined`; callback receives `(err)`; Sync returns `undefined` (throws on error)

### PlcStop
```
PlcStop(): Promise<undefined>
PlcStop(callback: (err: Snap7Error | null) => undefined): undefined
PlcStopSync(): undefined
```
Stop the PLC CPU.
- Returns: Promise resolves with `undefined`; callback receives `(err)`; Sync returns `undefined` (throws on error)

### CopyRamToRom
```
CopyRamToRom(timeout: number): Promise<undefined>
CopyRamToRom(timeout: number, callback: (err: Snap7Error | null) => undefined): undefined
CopyRamToRomSync(timeout: number): undefined
```
Copy RAM to ROM on the PLC.
- Parameters:
  - `timeout`: timeout in milliseconds
- Returns: Promise resolves with `undefined`; callback receives `(err)`; Sync returns `undefined` (throws on error)

### Compress
```
Compress(timeout: number): Promise<undefined>
Compress(timeout: number, callback: (err: Snap7Error | null) => undefined): undefined
CompressSync(timeout: number): undefined
```
Compress PLC memory.
- Parameters:
  - `timeout`: timeout in milliseconds
- Returns: Promise resolves with `undefined`; callback receives `(err)`; Sync returns `undefined` (throws on error)

### PlcStatus
```
PlcStatus(): Promise<PlcStatus>
PlcStatus(callback: (err: Snap7Error | null, data: PlcStatus) => undefined): undefined
PlcStatusSync(): PlcStatus
```
Read the current CPU status.
- Returns: Promise resolves with `PlcStatus`; callback receives `(err, status)`; Sync returns `PlcStatus` (throws on error)

---

## Security functions

### SetSessionPassword
```
SetSessionPassword(password: string): Promise<undefined>
SetSessionPassword(password: string, callback: (err: Snap7Error | null) => undefined): undefined
SetSessionPasswordSync(password: string): undefined
```
Set a session password for the current connection.
- Parameters:
  - `password`: session password
- Returns: Promise resolves with `undefined`; callback receives `(err)`; Sync returns `undefined` (throws on error)

### ClearSessionPassword
```
ClearSessionPassword(): Promise<undefined>
ClearSessionPassword(callback: (err: Snap7Error | null) => undefined): undefined
ClearSessionPasswordSync(): undefined
```
Clear the active session password.
- Returns: Promise resolves with `undefined`; callback receives `(err)`; Sync returns `undefined` (throws on error)

### GetProtection
```
GetProtection(): Promise<Protection>
GetProtection(callback: (err: Snap7Error | null, data: Protection) => undefined): undefined
GetProtectionSync(): Protection
```
Read the PLC protection levels.
- Returns: Promise resolves with `Protection`; callback receives `(err, protection)`; Sync returns `Protection` (throws on error)

---

## Properties & diagnostics
- `ExecTime() -> number` — execution time (ms) of the last job.
- `PDURequested() -> number` — requested PDU length during negotiation.
- `PDULength() -> number` — negotiated PDU length.
- `Connected() -> boolean` — connection state.
- `ErrorText(code: number) -> string` — human-readable text for a Snap7 error code.

---

## Constants

All constants are grouped exports; use the object/property names below (e.g. `S7Area.DB`, `S7WordLen.Real`).

### Areas
| Name | Value | Description |
|:-----|:----:|:------------|
| `S7Area.PE` | 0x81 | Process inputs |
| `S7Area.PA` | 0x82 | Process outputs |
| `S7Area.MK` | 0x83 | Merkers |
| `S7Area.CT` | 0x1C | Counters |
| `S7Area.TM` | 0x1D | Timers |
| `S7Area.DB` | 0x84 | Data blocks |

### Word lengths
| Name | Value | Description |
|:-----|:----:|:------------|
| `S7WordLen.Bit` | 0x01 | Bit |
| `S7WordLen.Byte` | 0x02 | Byte |
| `S7WordLen.Word` | 0x04 | Word (2 bytes) |
| `S7WordLen.DWord` | 0x06 | Double word (4 bytes) |
| `S7WordLen.Real` | 0x08 | Real (float) |
| `S7WordLen.Counter` | 0x1C | Counter (word) |
| `S7WordLen.Timer` | 0x1D | Timer (word) |

### Connection types
| Name | Value | Description |
|:-----|:----:|:------------|
| `ConnectionType.PG` | 0x0001 | Programming device |
| `ConnectionType.OP` | 0x0002 | Operator panel |
| `ConnectionType.BASIC` | 0x0003 | Basic connection |

### Block types
| Name | Value | Description |
|:-----|:----:|:------------|
| `BlockType.OB` | 0x38 | Organization block |
| `BlockType.DB` | 0x41 | Data block |
| `BlockType.SDB` | 0x42 | System data block |
| `BlockType.FC` | 0x43 | Function |
| `BlockType.SFC` | 0x44 | System function |
| `BlockType.FB` | 0x45 | Function block |
| `BlockType.SFB` | 0x46 | System function block |

### Block languages
| Name | Value | Description |
|:-----|:----:|:------------|
| `BlockLang.AWL` | 0x01 | AWL |
| `BlockLang.KOP` | 0x02 | KOP |
| `BlockLang.FUP` | 0x03 | FUP |
| `BlockLang.SCL` | 0x04 | SCL |
| `BlockLang.DB` | 0x05 | DB |
| `BlockLang.GRAPH` | 0x06 | GRAPH |

### Sub block types
| Name | Value | Description |
|:-----|:----:|:------------|
| `SubBlockType.OB` | 0x08 | Organization sub-block |
| `SubBlockType.DB` | 0x0A | Data sub-block |
| `SubBlockType.SDB` | 0x0B | System data sub-block |
| `SubBlockType.FC` | 0x0C | Function sub-block |
| `SubBlockType.SFC` | 0x0D | System function sub-block |
| `SubBlockType.FB` | 0x0E | Function block sub-block |
| `SubBlockType.SFB` | 0x0F | System function block sub-block |

### PLC status codes
| Name | Value | Description |
|:-----|:----:|:------------|
| `PlcStatus.Unknown` | 0x00 | Status not known |
| `PlcStatus.Run` | 0x08 | CPU running |
| `PlcStatus.Stop` | 0x04 | CPU stopped |

### Client parameters
| Name | Value | Description |
|:-----|:----:|:------------|
| `ClientParameter.RemotePort` | 2 | Remote TSAP port |
| `ClientParameter.PingTimeout` | 3 | Ping timeout (ms) |
| `ClientParameter.SendTimeout` | 4 | Send timeout (ms) |
| `ClientParameter.RecvTimeout` | 5 | Receive timeout (ms) |
| `ClientParameter.SrcRef` | 7 | Source reference |
| `ClientParameter.DstRef` | 8 | Destination reference |
| `ClientParameter.SrcTSap` | 9 | Local TSAP |
| `ClientParameter.PDURequest` | 10 | Requested PDU length |

---

## Type definitions

### BlocksList
| Field | Type | Description |
|:------|:-----|:------------|
| `OBCount` | number | Organization blocks |
| `FBCount` | number | Function blocks |
| `FCCount` | number | Functions |
| `SFBCount` | number | System function blocks |
| `SFCCount` | number | System functions |
| `DBCount` | number | Data blocks |
| `SDBCount` | number | System data blocks |

### BlockInfo
| Field | Type | Description |
|:------|:-----|:------------|
| `BlkType` | number | Block type |
| `BlkNumber` | number | Block number |
| `BlkLang` | number | Language |
| `BlkFlags` | number | Flags |
| `MC7Size` | number | MC7 size |
| `LoadSize` | number | Load size |
| `LocalData` | number | Local data size |
| `SBBLength` | number | SBB length |
| `CheckSum` | number | Checksum |
| `Version` | number | Version |
| `CodeDate` | string | Code date |
| `IntfDate` | string | Interface date |
| `Author` | string | Author |
| `Family` | string | Family |
| `Header` | string | Header |

### OrderCode
| Field | Type | Description |
|:------|:-----|:------------|
| `Code` | string | Order code |
| `V1` | number | Version major |
| `V2` | number | Version minor |
| `V3` | number | Version patch |

### CpuInfo
| Field | Type | Description |
|:------|:-----|:------------|
| `ModuleTypeName` | string | Module type name |
| `SerialNumber` | string | Serial number |
| `ASName` | string | AS name |
| `Copyright` | string | Copyright |
| `ModuleName` | string | Module name |

### CpInfo
| Field | Type | Description |
|:------|:-----|:------------|
| `MaxPduLength` | number | Max PDU length |
| `MaxConnections` | number | Max connections |
| `MaxMpiRate` | number | Max MPI rate |
| `MaxBusRate` | number | Max bus rate |

### Protection
| Field | Type | Description |
|:------|:-----|:------------|
| `sch_schal` | number | Switch-on protection |
| `sch_par` | number | Parameter protection |
| `sch_rel` | number | Release protection |
| `bart_sch` | number |??? |
| `anl_sch` | number |??? |

### DateTimeObject
| Field | Type | Description |
|:------|:-----|:------------|
| `year` | number | Year |
| `month` | number | Month (1-12) |
| `day` | number | Day (1-31) |
| `hour` | number | Hour (0-23) |
| `minute` | number | Minute (0-59) |
| `second` | number | Second (0-59) |
| `millis` | number | Milliseconds (0-999) |

### S7MultiVarReadResult
| Field | Type | Description |
|:------|:-----|:------------|
| `Result` | number | Snap7 result code |
| `Data` | Buffer \| null | Returned data for the item |

### S7MultiVarWriteResult
| Field | Type | Description |
|:------|:-----|:------------|
| `Result` | number | Snap7 result code |
