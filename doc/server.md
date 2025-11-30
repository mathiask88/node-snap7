# S7Server API

Simulate a PLC and handle requests via Snap7. Lifecycle methods return Promises; most other calls are synchronous and throw `Snap7Error` on invalid arguments or runtime failures.

- Promise form: resolves on success, rejects with `Snap7Error` (contains `code`, `errno`).
- Callback form: `(err, result)` when provided; function returns `void`.
- Sync form: returns value or throws `Snap7Error`.

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
  - [Event control codes](#event-control-codes)
  - [Event results](#event-results)
  - [Error codes](#error-codes)
- [Type definitions](#type-definitions)
  - [SrvEvent](#srvevent)
  - [S7Tag](#s7tag)

---

## Usage basics
- `Start()` binds to the last IP set by `StartTo()` or `0.0.0.0` if none was set; `StartTo(ip)` binds explicitly.
- `SetResourceless(true)` enables the `readWrite` hook; the worker waits for your callback to continue.
- `RegisterArea`/`SetArea` expose buffers as PLC areas. For DB areas, `index` is the DB number; otherwise `index` is ignored.
- Sync methods throw on error; otherwise they return `void` or the documented value.

---

## Administrative functions

### Start
```
Start(): Promise<void>
```
Start the server using the current bind address.
- Returns: resolves with `void`, rejects with `Snap7Error`

### StartTo
```
StartTo(ip: string): Promise<void>
```
Start the server and bind to a specific IP address.
- Parameters:
  - `ip`: IPv4 address to bind
- Returns: resolves with `void`, rejects with `Snap7Error`

### Stop
```
Stop(): Promise<void>
```
Stop the server and disconnect clients.
- Returns: resolves with `void`, rejects with `Snap7Error`

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
SetParam(paramNumber: number, value: number): void
```
Write a server parameter.
- Parameters:
  - `paramNumber`: see [Server parameters](#server-parameters)
  - `value`: new value
- Returns: `void`

### SetResourceless
```
SetResourceless(value: boolean): void
```
Enable/disable resourceless mode (required for `readWrite` events).
- Parameters:
  - `value`: `true` to enable, `false` to disable
- Returns: `void`

### GetEventsMask
```
GetEventsMask(): number
```
Read the current events mask.
- Returns: mask value

### SetEventsMask
```
SetEventsMask(mask: number): void
```
Set the events mask.
- Parameters:
  - `mask`: bitmask (see [Event codes](#event-codes) or [Event masks](#event-masks))
- Returns: `void`

---

## Memory functions

### RegisterArea
```
RegisterArea(areaCode: number, index: number, buffer: Buffer): void
```
Expose a buffer as a PLC area.
- Parameters:
  - `areaCode`: see [Areas](#areas)
  - `index`: DB number when `areaCode` is `srvAreaDB`; ignored for other areas
  - `buffer`: data buffer
- Returns: `void`; throws `Snap7Error` on failure

### UnregisterArea
```
UnregisterArea(areaCode: number, index: number): void
```
Remove a previously registered area.
- Parameters:
  - `areaCode`: see [Areas](#areas)
  - `index`: DB number when `areaCode` is `srvAreaDB`; ignored for other areas
- Returns: `void`; throws `Snap7Error` on failure

### GetArea
```
GetArea(areaCode: number, index: number): Buffer
```
Get the buffer for a registered area.
- Parameters:
  - `areaCode`: see [Areas](#areas)
  - `index`: DB number when `areaCode` is `srvAreaDB`; ignored for other areas
- Returns: registered area buffer

### SetArea
```
SetArea(areaCode: number, index: number, buffer: Buffer): void
```
Replace the buffer for a registered area.
- Parameters:
  - `areaCode`: see [Areas](#areas)
  - `index`: DB number when `areaCode` is `srvAreaDB`; ignored for other areas
  - `buffer`: new data buffer
- Returns: `void`; throws `Snap7Error` on failure

### LockArea
```
LockArea(areaCode: number, index: number): void
```
Lock a registered area; worker threads block until unlocked.
- Parameters:
  - `areaCode`: see [Areas](#areas)
  - `index`: DB number when `areaCode` is `srvAreaDB`; ignored for other areas
- Returns: `void`; throws `Snap7Error` on failure

### UnlockArea
```
UnlockArea(areaCode: number, index: number): void
```
Unlock a locked area.
- Parameters:
  - `areaCode`: see [Areas](#areas)
  - `index`: DB number when `areaCode` is `srvAreaDB`; ignored for other areas
- Returns: `void`; throws `Snap7Error` on failure

---

## Events

### event
```
'on("event", (evt: SrvEvent) => void)
```
Emitted for server lifecycle/protocol events.

### readWrite
```
'on("readWrite", (sender: string, operation: number, tag: S7Tag, buffer: Buffer, callback: (buf?: Buffer) => void) => void)
```
Emitted on every data read/write when resourceless mode is enabled. The worker waits until `callback` is invoked; for read operations supply the response buffer.

### error
```
'on("error", (err: Snap7Error) => void)
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
