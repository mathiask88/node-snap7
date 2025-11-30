# S7Client API

High-level access to Siemens S7 PLCs via Snap7. Most methods support Promise and callback forms plus a `Sync` variant.

- Promise form: resolves with the value below; rejects with `Snap7Error` (`code`, `errno`).
- Callback form: pass `(err, result)` as the last argument; function returns `void`.
- Sync form: `*Sync` variant returns the value or throws `Snap7Error`.

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
- Use constants from [Constants](#constants) for `Area`, `WordLen`, connection types, etc.
- `start` is a byte offset unless `WordLen` is `S7WLBit`, in which case it is a bit index within the byte.
- `amount` counts elements of the chosen `WordLen` (bytes for `S7WLByte`, words for `S7WLWord`, etc.).
- Buffers you pass must be large enough for `amount * WordLen`.

---

## Control functions

### Connect
```
Connect(): Promise<void>
Connect(callback: (err: Snap7Error | null) => void): void
ConnectSync(): void
```
Connect using the parameters set via `ConnectTo` or `SetConnectionParams`.
- Returns: Promise resolves with `void`; callback receives `(err)`; Sync returns `void` (throws on error)

### ConnectTo
```
ConnectTo(ip: string, rack: number, slot: number): Promise<void>
ConnectTo(ip: string, rack: number, slot: number, callback: (err: Snap7Error | null) => void): void
ConnectToSync(ip: string, rack: number, slot: number): void
```
Connect directly to the given IP, rack, and slot.
- Parameters:
  - `ip`: PLC IPv4 address
  - `rack`: rack number
  - `slot`: slot number
- Returns: Promise resolves with `void`; callback receives `(err)`; Sync returns `void` (throws on error)

### SetConnectionParams
```
SetConnectionParams(ip: string, localTSAP: number, remoteTSAP: number): void
```
Set ISO-on-TCP parameters prior to connecting.
- Parameters:
  - `ip`: PLC IPv4 address
  - `localTSAP`: local TSAP
  - `remoteTSAP`: remote TSAP
- Returns: `void`

### SetConnectionType
```
SetConnectionType(type: number): void
```
Set the connection resource type.
- Parameters:
  - `type`: see [Connection types](#connection-types)
- Returns: `void`

### Disconnect
```
Disconnect(): void
```
Close the connection.
- Returns: `void`

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
SetParam(paramNumber: number, value: number): void
```
Write a client parameter.
- Parameters:
  - `paramNumber`: see [Client parameters](#client-parameters)
  - `value`: new value
- Returns: `void`

---

## Data I/O functions

### ReadArea
```
ReadArea(area: Area, dbNumber: number, start: number, amount: number, wordLen: WordLen): Promise<Buffer>
ReadArea(area: Area, dbNumber: number, start: number, amount: number, wordLen: WordLen, callback: (err: Snap7Error | null, data: Buffer) => void): void
ReadAreaSync(area: Area, dbNumber: number, start: number, amount: number, wordLen: WordLen): Buffer
```
Read raw data from the PLC.
- Parameters:
  - `area`: memory area
  - `dbNumber`: DB number (only for `S7AreaDB`)
  - `start`: byte offset (bit index if `S7WLBit`)
  - `amount`: element count in units of `wordLen`
  - `wordLen`: element type/size
- Returns: Promise resolves with `Buffer`; callback receives `(err, buffer)`; Sync returns `Buffer` (throws on error)

### WriteArea
```
WriteArea(area: Area, dbNumber: number, start: number, amount: number, wordLen: WordLen, buffer: Buffer): Promise<void>
WriteArea(area: Area, dbNumber: number, start: number, amount: number, wordLen: WordLen, buffer: Buffer, callback: (err: Snap7Error | null) => void): void
WriteAreaSync(area: Area, dbNumber: number, start: number, amount: number, wordLen: WordLen, buffer: Buffer): void
```
Write raw data to the PLC.
- Parameters:
  - `area`: memory area
  - `dbNumber`: DB number (only for `S7AreaDB`)
  - `start`: byte offset (bit index if `S7WLBit`)
  - `amount`: element count in units of `wordLen`
  - `wordLen`: element type/size
  - `buffer`: data to write
- Returns: Promise resolves with `void`; callback receives `(err)`; Sync returns `void` (throws on error)

### Convenience read/write wrappers
Each wrapper supports Promise/Callback/Sync variants like `Foo(...) -> Promise<...>`, `Foo(..., callback) -> void`, `FooSync(...) -> ...`.
- DB: `DBRead`, `DBWrite`
- Merker: `MBRead`, `MBWrite`
- Inputs: `EBRead`, `EBWrite`
- Outputs: `ABRead`, `ABWrite`
- Timers: `TMRead`, `TMWrite`
- Counters: `CTRead`, `CTWrite`

### ReadMultiVars
```
ReadMultiVars(items: { Area: Area; WordLen: WordLen; DBNumber?: number; Start: number; Amount: number }[]): Promise<S7MultiVarReadResult[]>
ReadMultiVars(items: { Area: Area; WordLen: WordLen; DBNumber?: number; Start: number; Amount: number }[], callback: (err: Snap7Error | null, data: S7MultiVarReadResult[]) => void): void
ReadMultiVarsSync(items: { Area: Area; WordLen: WordLen; DBNumber?: number; Start: number; Amount: number }[]): S7MultiVarReadResult[]
```
Read multiple addresses in one request.
- Parameters:
  - `items`: `{ Area, WordLen, DBNumber?, Start, Amount }`
- Returns: Promise resolves with `S7MultiVarReadResult[]`; callback receives `(err, results)`; Sync returns `S7MultiVarReadResult[]` (throws on error)

### WriteMultiVars
```
WriteMultiVars(items: { Area: Area; WordLen: WordLen; DBNumber?: number; Start: number; Amount: number; Data: Buffer }[]): Promise<S7MultiVarWriteResult[]>
WriteMultiVars(items: { Area: Area; WordLen: WordLen; DBNumber?: number; Start: number; Amount: number; Data: Buffer }[], callback: (err: Snap7Error | null, data: S7MultiVarWriteResult[]) => void): void
WriteMultiVarsSync(items: { Area: Area; WordLen: WordLen; DBNumber?: number; Start: number; Amount: number; Data: Buffer }[]): S7MultiVarWriteResult[]
```
Write multiple addresses in one request.
- Parameters:
  - `items`: `{ Area, WordLen, DBNumber?, Start, Amount, Data: Buffer }`
- Returns: Promise resolves with `S7MultiVarWriteResult[]`; callback receives `(err, results)`; Sync returns `S7MultiVarWriteResult[]` (throws on error)

---

## Directory functions

### ListBlocks
```
ListBlocks(): Promise<BlocksList>
ListBlocks(callback: (err: Snap7Error | null, data: BlocksList) => void): void
ListBlocksSync(): BlocksList
```
Get a count of blocks on the PLC.
- Returns: Promise resolves with `BlocksList`; callback receives `(err, blocks)`; Sync returns `BlocksList` (throws on error)

### ListBlocksOfType
```
ListBlocksOfType(blockType: BlockType): Promise<number[]>
ListBlocksOfType(blockType: BlockType, callback: (err: Snap7Error | null, data: number[]) => void): void
ListBlocksOfTypeSync(blockType: BlockType): number[]
```
List block numbers of a specific type.
- Parameters:
  - `blockType`: block type code
- Returns: Promise resolves with `number[]`; callback receives `(err, numbers)`; Sync returns `number[]` (throws on error)

### GetAgBlockInfo
```
GetAgBlockInfo(blockType: BlockType, blockNum: number): Promise<BlockInfo>
GetAgBlockInfo(blockType: BlockType, blockNum: number, callback: (err: Snap7Error | null, data: BlockInfo) => void): void
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
FullUpload(blockType: BlockType, blockNum: number, size: number, callback: (err: Snap7Error | null, data: Buffer) => void): void
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
Upload(blockType: BlockType, blockNum: number, size: number, callback: (err: Snap7Error | null, data: Buffer) => void): void
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
Download(blockNum: number, buffer: Buffer): Promise<void>
Download(blockNum: number, buffer: Buffer, callback: (err: Snap7Error | null) => void): void
DownloadSync(blockNum: number, buffer: Buffer): void
```
Upload a compiled block buffer to the PLC.
- Parameters:
  - `blockNum`: block number
  - `buffer`: compiled block data
- Returns: Promise resolves with `void`; callback receives `(err)`; Sync returns `void` (throws on error)

### Delete
```
Delete(blockType: BlockType, blockNum: number): Promise<void>
Delete(blockType: BlockType, blockNum: number, callback: (err: Snap7Error | null) => void): void
DeleteSync(blockType: BlockType, blockNum: number): void
```
Delete a block on the PLC.
- Parameters:
  - `blockType`: block type code
  - `blockNum`: block number
- Returns: Promise resolves with `void`; callback receives `(err)`; Sync returns `void` (throws on error)

### DBGet
```
DBGet(dbNumber: number): Promise<Buffer>
DBGet(dbNumber: number, callback: (err: Snap7Error | null, data: Buffer) => void): void
DBGetSync(dbNumber: number): Buffer
```
Fetch the raw content of a DB.
- Parameters:
  - `dbNumber`: DB number
- Returns: Promise resolves with `Buffer`; callback receives `(err, buffer)`; Sync returns `Buffer` (throws on error)

### DBFill
```
DBFill(dbNumber: number, fillChar: number | string): Promise<void>
DBFill(dbNumber: number, fillChar: number | string, callback: (err: Snap7Error | null) => void): void
DBFillSync(dbNumber: number, fillChar: number | string): void
```
Fill an entire DB with a byte or character.
- Parameters:
  - `dbNumber`: DB number
  - `fillChar`: fill byte or single-character string
- Returns: Promise resolves with `void`; callback receives `(err)`; Sync returns `void` (throws on error)

---

## Date/Time functions

### GetPlcDateTime
```
GetPlcDateTime(): Promise<Date>
GetPlcDateTime(callback: (err: Snap7Error | null, data: Date) => void): void
GetPlcDateTimeSync(): Date
```
Read the PLC system clock.
- Returns: Promise resolves with `Date`; callback receives `(err, date)`; Sync returns `Date` (throws on error)

### SetPlcDateTime
```
SetPlcDateTime(dateTime: Date | [DateTimeObject](#datetimeobject)): Promise<void>
SetPlcDateTime(dateTime: Date | [DateTimeObject](#datetimeobject), callback: (err: Snap7Error | null) => void): void
SetPlcDateTimeSync(dateTime: Date | [DateTimeObject](#datetimeobject)): void
```
Set the PLC system clock.
- Parameters:
  - `dateTime`: desired time (Date or [DateTimeObject](#datetimeobject))
- Returns: Promise resolves with `void`; callback receives `(err)`; Sync returns `void` (throws on error)

### SetPlcSystemDateTime
```
SetPlcSystemDateTime(): Promise<void>
SetPlcSystemDateTime(callback: (err: Snap7Error | null) => void): void
SetPlcSystemDateTimeSync(): void
```
Set the PLC system clock to the host time.
- Returns: Promise resolves with `void`; callback receives `(err)`; Sync returns `void` (throws on error)

---

## System info functions

### ReadSZL
```
ReadSZL(id: number, index: number): Promise<Buffer>
ReadSZL(id: number, index: number, callback: (err: Snap7Error | null, data: Buffer) => void): void
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
ReadSZLList(callback: (err: Snap7Error | null, data: number[]) => void): void
ReadSZLListSync(): number[]
```
List available SZL IDs.
- Returns: Promise resolves with `number[]`; callback receives `(err, ids)`; Sync returns `number[]` (throws on error)

### GetOrderCode
```
GetOrderCode(): Promise<OrderCode>
GetOrderCode(callback: (err: Snap7Error | null, data: OrderCode) => void): void
GetOrderCodeSync(): OrderCode
```
Retrieve the PLC order code.
- Returns: Promise resolves with `OrderCode`; callback receives `(err, code)`; Sync returns `OrderCode` (throws on error)

### GetCpuInfo
```
GetCpuInfo(): Promise<CpuInfo>
GetCpuInfo(callback: (err: Snap7Error | null, data: CpuInfo) => void): void
GetCpuInfoSync(): CpuInfo
```
Get CPU identification data.
- Returns: Promise resolves with `CpuInfo`; callback receives `(err, info)`; Sync returns `CpuInfo` (throws on error)

### GetCpInfo
```
GetCpInfo(): Promise<CpInfo>
GetCpInfo(callback: (err: Snap7Error | null, data: CpInfo) => void): void
GetCpInfoSync(): CpInfo
```
Get communication processor information.
- Returns: Promise resolves with `CpInfo`; callback receives `(err, info)`; Sync returns `CpInfo` (throws on error)

---

## PLC control functions

### PlcHotStart
```
PlcHotStart(): Promise<void>
PlcHotStart(callback: (err: Snap7Error | null) => void): void
PlcHotStartSync(): void
```
Warm-start the PLC.
- Returns: Promise resolves with `void`; callback receives `(err)`; Sync returns `void` (throws on error)

### PlcColdStart
```
PlcColdStart(): Promise<void>
PlcColdStart(callback: (err: Snap7Error | null) => void): void
PlcColdStartSync(): void
```
Cold-start the PLC.
- Returns: Promise resolves with `void`; callback receives `(err)`; Sync returns `void` (throws on error)

### PlcStop
```
PlcStop(): Promise<void>
PlcStop(callback: (err: Snap7Error | null) => void): void
PlcStopSync(): void
```
Stop the PLC CPU.
- Returns: Promise resolves with `void`; callback receives `(err)`; Sync returns `void` (throws on error)

### CopyRamToRom
```
CopyRamToRom(timeout: number): Promise<void>
CopyRamToRom(timeout: number, callback: (err: Snap7Error | null) => void): void
CopyRamToRomSync(timeout: number): void
```
Copy RAM to ROM on the PLC.
- Parameters:
  - `timeout`: timeout in milliseconds
- Returns: Promise resolves with `void`; callback receives `(err)`; Sync returns `void` (throws on error)

### Compress
```
Compress(timeout: number): Promise<void>
Compress(timeout: number, callback: (err: Snap7Error | null) => void): void
CompressSync(timeout: number): void
```
Compress PLC memory.
- Parameters:
  - `timeout`: timeout in milliseconds
- Returns: Promise resolves with `void`; callback receives `(err)`; Sync returns `void` (throws on error)

### PlcStatus
```
PlcStatus(): Promise<PlcStatus>
PlcStatus(callback: (err: Snap7Error | null, data: PlcStatus) => void): void
PlcStatusSync(): PlcStatus
```
Read the current CPU status.
- Returns: Promise resolves with `PlcStatus`; callback receives `(err, status)`; Sync returns `PlcStatus` (throws on error)

---

## Security functions

### SetSessionPassword
```
SetSessionPassword(password: string): Promise<void>
SetSessionPassword(password: string, callback: (err: Snap7Error | null) => void): void
SetSessionPasswordSync(password: string): void
```
Set a session password for the current connection.
- Parameters:
  - `password`: session password
- Returns: Promise resolves with `void`; callback receives `(err)`; Sync returns `void` (throws on error)

### ClearSessionPassword
```
ClearSessionPassword(): Promise<void>
ClearSessionPassword(callback: (err: Snap7Error | null) => void): void
ClearSessionPasswordSync(): void
```
Clear the active session password.
- Returns: Promise resolves with `void`; callback receives `(err)`; Sync returns `void` (throws on error)

### GetProtection
```
GetProtection(): Promise<Protection>
GetProtection(callback: (err: Snap7Error | null, data: Protection) => void): void
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

### Areas
| Name | Value | Description |
|:-----|:----:|:------------|
| `srvAreaPE` | 0 | Process inputs |
| `srvAreaPA` | 1 | Process outputs |
| `srvAreaMK` | 2 | Merkers |
| `srvAreaCT` | 3 | Counters |
| `srvAreaTM` | 4 | Timers |
| `srvAreaDB` | 5 | Data blocks |

### Operation types
| Name | Value | Description |
|:-----|:----:|:------------|
| `operationRead` | 0x00 | Read operation |
| `operationWrite` | 0x01 | Write operation |

### Server status codes
| Name | Value | Description |
|:-----|:----:|:------------|
| `SrvStopped` | 0x00 | Server stopped |
| `SrvRunning` | 0x01 | Server running |
| `SrvError` | 0x02 | Server error |

### CPU status codes
| Name | Value | Description |
|:-----|:----:|:------------|
| `S7CpuStatusUnknown` | 0x00 | Status not known |
| `S7CpuStatusRun` | 0x08 | CPU is running |
| `S7CpuStatusStop` | 0x04 | CPU is stopped |

### Server parameters
| Name | Value | Description |
|:-----|:----:|:------------|
| `LocalPort` | 1 | Listener port |
| `WorkInterval` | 6 | Worker interval |
| `PDURequest` | 10 | Requested PDU length |
| `MaxClients` | 11 | Maximum clients |

### Event masks
| Name | Value | Description |
|:-----|:----:|:------------|
| `evcAll` | 0xFFFFFFFF | Enable all events |
| `evcNone` | 0x00000000 | Disable all events |

### Event codes
| Name | Value | Description |
|:-----|:----:|:------------|
| `evcServerStarted` | 0x00000001 | Server started |
| `evcServerStopped` | 0x00000002 | Server stopped |
| `evcListenerCannotStart` | 0x00000004 | Listener failed to start |
| `evcClientAdded` | 0x00000008 | Client added |
| `evcClientRejected` | 0x00000010 | Client rejected |
| `evcClientNoRoom` | 0x00000020 | No room for client |
| `evcClientException` | 0x00000040 | Client exception |
| `evcClientDisconnected` | 0x00000080 | Client disconnected |
| `evcClientTerminated` | 0x00000100 | Client terminated |
| `evcClientsDropped` | 0x00000200 | Clients dropped |
| `evcPDUincoming` | 0x00010000 | Incoming PDU |
| `evcDataRead` | 0x00020000 | Data read |
| `evcDataWrite` | 0x00040000 | Data write |
| `evcNegotiatePDU` | 0x00080000 | PDU negotiation |
| `evcReadSZL` | 0x00100000 | SZL read |
| `evcClock` | 0x00200000 | Clock event |
| `evcUpload` | 0x00400000 | Upload |
| `evcDownload` | 0x00800000 | Download |
| `evcDirectory` | 0x01000000 | Directory |
| `evcSecurity` | 0x02000000 | Security |
| `evcControl` | 0x04000000 | Control |

### Event subcodes
| Name | Value | Description |
|:-----|:----:|:------------|
| `evsUnknown` | 0x00000000 | Unknown |
| `evsStartUpload` | 0x00000001 | Start upload |
| `evsStartDownload` | 0x00000002 | Start download |
| `evsGetBlockList` | 0x00000003 | Get block list |
| `evsStartListBoT` | 0x00000004 | Start list BoT |
| `evsListBoT` | 0x00000005 | List BoT |
| `evsGetBlockInfo` | 0x00000006 | Get block info |
| `evsGetClock` | 0x00000007 | Get clock |
| `evsSetClock` | 0x00000008 | Set clock |
| `evsSetPassword` | 0x00000009 | Set password |
| `evsClrPassword` | 0x0000000A | Clear password |

### Event control codes
| Name | Value | Description |
|:-----|:----:|:------------|
| `CodeControlUnknown` | 0x00 | Unknown control |
| `CodeControlColdStart` | 0x01 | Cold start |
| `CodeControlWarmStart` | 0x02 | Warm start |
| `CodeControlStop` | 0x03 | Stop |
| `CodeControlCompress` | 0x04 | Compress |
| `CodeControlCpyRamRom` | 0x05 | Copy RAM to ROM |
| `CodeControlInsDel` | 0x06 | Insert/Delete |

### Event results
| Name | Value | Description |
|:-----|:----:|:------------|
| `evrNoError` | 0x00000000 | No error |
| `evrFragmentRejected` | 0x00000001 | Fragment rejected |
| `evrMalformedPDU` | 0x00000002 | Malformed PDU |
| `evrSparseBytes` | 0x00000003 | Sparse bytes |
| `evrCannotHandlePDU` | 0x00000004 | Cannot handle PDU |
| `evrNotImplemented` | 0x00000005 | Not implemented |
| `evrErrException` | 0x00000006 | Exception |
| `evrErrAreaNotFound` | 0x00000007 | Area not found |
| `evrErrOutOfRange` | 0x00000008 | Out of range |
| `evrErrOverPDU` | 0x00000009 | Over PDU |
| `evrErrTransportSize` | 0x0000000A | Transport size error |
| `evrInvalidGroupUData` | 0x0000000B | Invalid group UData |
| `evrInvalidSZL` | 0x0000000C | Invalid SZL |
| `evrDataSizeMismatch` | 0x0000000D | Data size mismatch |
| `evrCannotUpload` | 0x0000000E | Cannot upload |
| `evrCannotDownload` | 0x0000000F | Cannot download |
| `evrUploadInvalidID` | 0x00000010 | Upload invalid ID |
| `evrResNotFound` | 0x00000011 | Resource not found |

### Error codes
| Name | Value | Description |
|:-----|:----:|:------------|
| `errSrvCannotStart` | 0x00100000 | Cannot start server |
| `errSrvDBNullPointer` | 0x00200000 | DB null pointer |
| `errSrvAreaAlreadyExists` | 0x00300000 | Area already exists |
| `errSrvUnknownArea` | 0x00400000 | Unknown area |
| `errSrvInvalidParams` | 0x00500000 | Invalid params |
| `errSrvTooManyDB` | 0x00600000 | Too many DBs |
| `errSrvInvalidParamNumber` | 0x00700000 | Invalid param number |
| `errSrvCannotChangeParam` | 0x00800000 | Cannot change param |

---

## Type definitions

### SrvEvent
| Field | Type | Description |
|:------|:-----|:------------|
| `EvtTime` | Date | Event timestamp |
| `EvtSender` | string | Sender IP |
| `EvtCode` | number | Event code (see [Event codes](#event-codes)) |
| `EvtRetCode` | number | Return code (see [Event results](#event-results)) |
| `EvtParam1` | number | Event parameter 1 |
| `EvtParam2` | number | Event parameter 2 |
| `EvtParam3` | number | Event parameter 3 |
| `EvtParam4` | number | Event parameter 4 |

### S7Tag
| Field | Type | Description |
|:------|:-----|:------------|
| `Area` | number | Area code (see [Areas](#areas)) |
| `DBNumber` | number | DB number (for DB areas) |
| `Start` | number | Start offset |
| `Size` | number | Number of elements |
| `WordLen` | number | Word length code |



