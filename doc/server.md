# S7Server API

Simulate a PLC and handle requests via Snap7. Lifecycle methods return Promises; most other calls are synchronous and throw `Snap7Error` on invalid arguments or runtime failures.

- Promise form: resolves on success, rejects with `Snap7Error` (contains `code`, `errno`).
- Callback form: `(err, result)` when provided; function returns `undefined`.
- Sync form: returns value or `undefined`, or throws `Snap7Error`.

---

## Table of Contents
- [Usage basics](#usage-basics)
- [Administrative functions](#administrative-functions)
  - [Start](#start)
  - [StartTo](#startto)
  - [Stop](#stop)
  - [GetParam](#getparam)
  - [SetParam](#setparam)
  - [SetResourceless](#setresourceless)
  - [GetEventsMask](#geteventsmask)
  - [SetEventsMask](#seteventsmask)
- [Memory functions](#memory-functions)
  - [RegisterArea](#registerarea)
  - [UnregisterArea](#unregisterarea)
  - [GetArea](#getarea)
  - [SetArea](#setarea)
  - [LockArea](#lockarea)
  - [UnlockArea](#unlockarea)
- [Events](#events)
  - [event](#event)
  - [readWrite](#readwrite)
  - [error](#error)
- [Diagnostics & status](#diagnostics--status)
  - [EventText](#eventtext)
  - [ErrorText](#errortext)
  - [ServerStatus](#serverstatus)
  - [ClientsCount](#clientscount)
  - [GetCpuStatus](#getcpustatus)
  - [SetCpuStatus](#setcpustatus)
- [Constants](#constants)
  - [Areas](#areas)
  - [Operation types](#operation-types)
  - [Server status codes](#server-status-codes)
  - [CPU status codes](#cpu-status-codes)
  - [Server parameters](#server-parameters)
  - [Event masks](#event-masks)
  - [Event codes](#event-codes)
  - [Event subcodes](#event-subcodes)
  - [Event groups](#event-groups)
  - [Event control codes](#event-control-codes)
  - [Event results](#event-results)
  - [Error codes](#error-codes)
- [Type definitions](#type-definitions)
  - [SrvEvent](#srvevent)
  - [S7Tag](#s7tag)

---

## Usage basics
- Use grouped constants from [Constants](#constants), e.g. `const { ServerArea, ServerParameter, ServerOperation, ServerEventCode } = require('node-snap7');`.
- `Start()` binds to the last IP set by `StartTo()` or `0.0.0.0` if none was set; `StartTo(ip)` binds explicitly.
- `SetResourceless(true)` enables the `readWrite` hook; the worker waits for your callback to continue.
- `RegisterArea`/`SetArea` expose buffers as PLC areas. For `ServerArea.DB`, `index` is the DB number; otherwise `index` is ignored.
- Sync methods throw on error; otherwise they return `undefined` or the documented value.

---

## Administrative functions

### Start
```
Start(): Promise<undefined>
```
Start the server using the current bind address.
- Returns: resolves with `undefined`, rejects with `Snap7Error`

### StartTo
```
StartTo(ip: string): Promise<undefined>
```
Start the server and bind to a specific IP address.
- Parameters:
  - `ip`: IPv4 address to bind
- Returns: resolves with `undefined`, rejects with `Snap7Error`

### Stop
```
Stop(): Promise<undefined>
```
Stop the server and disconnect clients.
- Returns: resolves with `undefined`, rejects with `Snap7Error`

### GetParam
```
GetParam(paramNumber: number): number
```
Read a server parameter.
- Parameters:
  - `paramNumber`: see [Server parameters](#server-parameters)
- Returns: parameter value

### SetParam
```
SetParam(paramNumber: number, value: number): undefined
```
Write a server parameter.
- Parameters:
  - `paramNumber`: see [Server parameters](#server-parameters)
  - `value`: new value
- Returns: `undefined`

### SetResourceless
```
SetResourceless(value: boolean): undefined
```
Enable/disable resourceless mode (required for `readWrite` events).
- Parameters:
  - `value`: `true` to enable, `false` to disable
- Returns: `undefined`

### GetEventsMask
```
GetEventsMask(): number
```
Read the current events mask.
- Returns: mask value

### SetEventsMask
```
SetEventsMask(mask: number): undefined
```
Set the events mask.
- Parameters:
  - `mask`: bitmask (see [Event codes](#event-codes) or [Event masks](#event-masks))
- Returns: `undefined`

---

## Memory functions

### RegisterArea
```
RegisterArea(areaCode: number, index: number, buffer: Buffer): undefined
```
Expose a buffer as a PLC area.
- Parameters:
  - `areaCode`: see [Areas](#areas)
  - `index`: DB number when `areaCode` is `ServerArea.DB`; ignored for other areas
  - `buffer`: data buffer
- Returns: `undefined`; throws `Snap7Error` on failure

### UnregisterArea
```
UnregisterArea(areaCode: number, index: number): undefined
```
Remove a previously registered area.
- Parameters:
  - `areaCode`: see [Areas](#areas)
  - `index`: DB number when `areaCode` is `ServerArea.DB`; ignored for other areas
- Returns: `undefined`; throws `Snap7Error` on failure

### GetArea
```
GetArea(areaCode: number, index: number): Buffer
```
Get the buffer for a registered area.
- Parameters:
  - `areaCode`: see [Areas](#areas)
  - `index`: DB number when `areaCode` is `ServerArea.DB`; ignored for other areas
- Returns: registered area buffer

### SetArea
```
SetArea(areaCode: number, index: number, buffer: Buffer): undefined
```
Replace the buffer for a registered area.
- Parameters:
  - `areaCode`: see [Areas](#areas)
  - `index`: DB number when `areaCode` is `ServerArea.DB`; ignored for other areas
  - `buffer`: new data buffer
- Returns: `undefined`; throws `Snap7Error` on failure

### LockArea
```
LockArea(areaCode: number, index: number): undefined
```
Lock a registered area; worker threads block until unlocked.
- Parameters:
  - `areaCode`: see [Areas](#areas)
  - `index`: DB number when `areaCode` is `ServerArea.DB`; ignored for other areas
- Returns: `undefined`; throws `Snap7Error` on failure

### UnlockArea
```
UnlockArea(areaCode: number, index: number): undefined
```
Unlock a locked area.
- Parameters:
  - `areaCode`: see [Areas](#areas)
  - `index`: DB number when `areaCode` is `ServerArea.DB`; ignored for other areas
- Returns: `undefined`; throws `Snap7Error` on failure

---

## Events

### event
```
'on("event", (evt: SrvEvent) => undefined)
```
Emitted for server lifecycle/protocol events.

### readWrite
```
'on("readWrite", (sender: string, operation: number, tag: S7Tag, buffer: Buffer, callback: (buf?: Buffer) => undefined) => undefined)
```
Emitted on every data read/write when resourceless mode is enabled. The worker waits until `callback` is invoked; for read operations supply the response buffer.

### error
```
'on("error", (err: Snap7Error) => undefined)
```
Emitted when the server encounters an error.

---

## Diagnostics & status

### EventText
```
EventText(evt: SrvEvent): string
```
Return human-readable text for an event.
- Parameters:
  - `evt`: event object
- Returns: event text

### ErrorText
```
ErrorText(errNum: number): string
```
Return human-readable text for an error code.
- Parameters:
  - `errNum`: error code
- Returns: error text

### ServerStatus
```
ServerStatus(): number
```
Return the server status (see [Server status codes](#server-status-codes)).
- Returns: server status code

### ClientsCount
```
ClientsCount(): number
```
Return the number of connected clients.
- Returns: client count

### GetCpuStatus
```
GetCpuStatus(): number
```
Return the simulated CPU status (see [CPU status codes](#cpu-status-codes)).
- Returns: CPU status code

### SetCpuStatus
```
SetCpuStatus(status: number): boolean
```
Set the simulated CPU status.
- Parameters:
  - `status`: see [CPU status codes](#cpu-status-codes)
- Returns: `true` on success, otherwise `false`

---

## Constants

All constants are grouped exports; access them as properties (e.g. `ServerArea.DB`, `ServerEventCode.DataRead`).

### Areas
| Name | Value | Description |
|:-----|:----:|:------------|
| `ServerArea.PE` | 0 | Process inputs |
| `ServerArea.PA` | 1 | Process outputs |
| `ServerArea.MK` | 2 | Merkers |
| `ServerArea.CT` | 3 | Counters |
| `ServerArea.TM` | 4 | Timers |
| `ServerArea.DB` | 5 | Data blocks |

### Operation types
| Name | Value | Description |
|:-----|:----:|:------------|
| `ServerOperation.Read` | 0x00 | Read operation |
| `ServerOperation.Write` | 0x01 | Write operation |

### Server status codes
| Name | Value | Description |
|:-----|:----:|:------------|
| `ServerStatus.SrvStopped` | 0x00 | Server stopped |
| `ServerStatus.SrvRunning` | 0x01 | Server running |
| `ServerStatus.SrvError` | 0x02 | Server error |

### CPU status codes
| Name | Value | Description |
|:-----|:----:|:------------|
| `PlcStatus.Unknown` | 0x00 | Status not known |
| `PlcStatus.Run` | 0x08 | CPU is running |
| `PlcStatus.Stop` | 0x04 | CPU is stopped |

### Server parameters
| Name | Value | Description |
|:-----|:----:|:------------|
| `ServerParameter.LocalPort` | 1 | Listener port |
| `ServerParameter.WorkInterval` | 6 | Worker interval |
| `ServerParameter.PDURequest` | 10 | Requested PDU length |
| `ServerParameter.MaxClients` | 11 | Maximum clients |

### Event masks
| Name | Value | Description |
|:-----|:----:|:------------|
| `ServerEventCode.All` | 0xFFFFFFFF | Enable all events |
| `ServerEventCode.None` | 0x00000000 | Disable all events |

### Event codes
| Name | Value | Description |
|:-----|:----:|:------------|
| `ServerEventCode.ServerStarted` | 0x00000001 | Server started |
| `ServerEventCode.ServerStopped` | 0x00000002 | Server stopped |
| `ServerEventCode.ListenerCannotStart` | 0x00000004 | Listener failed to start |
| `ServerEventCode.ClientAdded` | 0x00000008 | Client added |
| `ServerEventCode.ClientRejected` | 0x00000010 | Client rejected |
| `ServerEventCode.ClientNoRoom` | 0x00000020 | No room for client |
| `ServerEventCode.ClientException` | 0x00000040 | Client exception |
| `ServerEventCode.ClientDisconnected` | 0x00000080 | Client disconnected |
| `ServerEventCode.ClientTerminated` | 0x00000100 | Client terminated |
| `ServerEventCode.ClientsDropped` | 0x00000200 | Clients dropped |
| `ServerEventCode.PDUincoming` | 0x00010000 | Incoming PDU |
| `ServerEventCode.DataRead` | 0x00020000 | Data read |
| `ServerEventCode.DataWrite` | 0x00040000 | Data write |
| `ServerEventCode.NegotiatePDU` | 0x00080000 | PDU negotiation |
| `ServerEventCode.ReadSZL` | 0x00100000 | SZL read |
| `ServerEventCode.Clock` | 0x00200000 | Clock event |
| `ServerEventCode.Upload` | 0x00400000 | Upload |
| `ServerEventCode.Download` | 0x00800000 | Download |
| `ServerEventCode.Directory` | 0x01000000 | Directory |
| `ServerEventCode.Security` | 0x02000000 | Security |
| `ServerEventCode.Control` | 0x04000000 | Control |

### Event subcodes
| Name | Value | Description |
|:-----|:----:|:------------|
| `ServerEventSubcode.Unknown` | 0x0000 | Unknown |
| `ServerEventSubcode.StartUpload` | 0x0001 | Start upload |
| `ServerEventSubcode.StartDownload` | 0x0001 | Start download |
| `ServerEventSubcode.GetBlockList` | 0x0001 | Get block list |
| `ServerEventSubcode.StartListBoT` | 0x0002 | Start list BoT |
| `ServerEventSubcode.ListBoT` | 0x0003 | List BoT |
| `ServerEventSubcode.GetBlockInfo` | 0x0004 | Get block info |
| `ServerEventSubcode.GetClock` | 0x0001 | Get clock |
| `ServerEventSubcode.SetClock` | 0x0002 | Set clock |
| `ServerEventSubcode.SetPassword` | 0x0001 | Set password |
| `ServerEventSubcode.ClrPassword` | 0x0002 | Clear password |

### Event groups
| Name | Value | Description |
|:-----|:----:|:------------|
| `ServerEventGroup.Programmer` | 0x0041 | Programmer functions |
| `ServerEventGroup.CyclicData` | 0x0042 | Cyclic data |
| `ServerEventGroup.BlocksInfo` | 0x0043 | Blocks info |
| `ServerEventGroup.SZL` | 0x0044 | System status list |
| `ServerEventGroup.Password` | 0x0045 | Password |
| `ServerEventGroup.BSend` | 0x0046 | Block send |
| `ServerEventGroup.Clock` | 0x0047 | Clock |
| `ServerEventGroup.Security` | 0x0045 | Security |

### Event control codes
| Name | Value | Description |
|:-----|:----:|:------------|
| `ServerEventControl.Unknown` | 0x00 | Unknown control |
| `ServerEventControl.ColdStart` | 0x01 | Cold start |
| `ServerEventControl.WarmStart` | 0x02 | Warm start |
| `ServerEventControl.Stop` | 0x03 | Stop |
| `ServerEventControl.Compress` | 0x04 | Compress |
| `ServerEventControl.CpyRamRom` | 0x05 | Copy RAM to ROM |
| `ServerEventControl.InsDel` | 0x06 | Insert/Delete |

### Event results
| Name | Value | Description |
|:-----|:----:|:------------|
| `ServerEventResult.NoError` | 0x00000000 | No error |
| `ServerEventResult.FragmentRejected` | 0x00000001 | Fragment rejected |
| `ServerEventResult.MalformedPDU` | 0x00000002 | Malformed PDU |
| `ServerEventResult.SparseBytes` | 0x00000003 | Sparse bytes |
| `ServerEventResult.CannotHandlePDU` | 0x00000004 | Cannot handle PDU |
| `ServerEventResult.NotImplemented` | 0x00000005 | Not implemented |
| `ServerEventResult.ErrException` | 0x00000006 | Exception |
| `ServerEventResult.ErrAreaNotFound` | 0x00000007 | Area not found |
| `ServerEventResult.ErrOutOfRange` | 0x00000008 | Out of range |
| `ServerEventResult.ErrOverPDU` | 0x00000009 | Over PDU |
| `ServerEventResult.ErrTransportSize` | 0x0000000A | Transport size error |
| `ServerEventResult.InvalidGroupUData` | 0x0000000B | Invalid group UData |
| `ServerEventResult.InvalidSZL` | 0x0000000C | Invalid SZL |
| `ServerEventResult.DataSizeMismatch` | 0x0000000D | Data size mismatch |
| `ServerEventResult.CannotUpload` | 0x0000000E | Cannot upload |
| `ServerEventResult.CannotDownload` | 0x0000000F | Cannot download |
| `ServerEventResult.UploadInvalidID` | 0x00000010 | Upload invalid ID |
| `ServerEventResult.ResNotFound` | 0x00000011 | Resource not found |

### Error codes
| Name | Value | Description |
|:-----|:----:|:------------|
| `ServerError.CannotStart` | 0x00100000 | Cannot start server |
| `ServerError.DBNullPointer` | 0x00200000 | DB null pointer |
| `ServerError.AreaAlreadyExists` | 0x00300000 | Area already exists |
| `ServerError.UnknownArea` | 0x00400000 | Unknown area |
| `ServerError.InvalidParams` | 0x00500000 | Invalid params |
| `ServerError.TooManyDB` | 0x00600000 | Too many DBs |
| `ServerError.InvalidParamNumber` | 0x00700000 | Invalid param number |
| `ServerError.CannotChangeParam` | 0x00800000 | Cannot change param |

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
| `WordLen` | number | Word length code (see `S7WordLen`) |
