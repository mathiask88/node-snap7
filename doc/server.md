## S7Server
- [Administrative functions](#administrative-functions)
  - [Start()](#start)
  - [StartTo()](#start-to)
  - [Stop()](#stop)
  - [GetParam()](#get-param)
  - [SetParam()](#set-param)
  - [SetResourceless()](#set-resourceless)
- [Memory functions](#memory-functions)
  - [RegisterArea()](#register-area)
  - [UnregisterArea()](#unregister-area)
  - [GetArea()](#get-area)
  - [SetArea()](#set-area)
  - [LockArea()](#lock-area)
  - [UnlockArea()](#unlock-area)
- [Event functions](#event-functions)
  - [Event 'event'](#event-event)
  - [Event 'readWrite'](#event-read-write)
  - [GetEventMask()](#get-event-mask)
  - [SetEventMask()](#set-event-mask)
- [Miscellaneous functions](#miscellaneous-functions)
  - [LastError()](#last-error)
  - [EventText()](#event-text)
  - [ErrorText()](#error-text)
  - [ServerStatus()](#server-status)
  - [ClientsCount()](#clients-count)
  - [GetCpuStatus()](#get-cpu-status)
  - [SetCpuStatus()](#set-cpu-status)

### <a name="administrative-functions"></a>API - Administrative functions

----------

#### <a name="start"></a>S7Server.Start([callback])
Starts the server and binds it to the IP address specified in the previous call of `StartTo()`. If `StartTo()` was not previously called, `0.0.0.0` is assumed as IP address.

- The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns `true` on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

#### <a name="start-to"></a>S7Server.StartTo(ip[, callback])
Starts the server and binds it to the specified IP address and the IsoTCP port.

- `ip` PLC/Equipment IPV4 Address ex. “192.168.1.12”
- The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns the CPU status on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

#### <a name="stop"></a>S7Server.Stop([callback])
Stops the server, disconnects gracefully all clients, destroys al S7 workers and unbinds the listener socket from its address.

- The optional `callback` parameter will be executed after completion

If `callback` is **not** set the function is **blocking** and returns the CPU status on success or `false` on error.<br />
If `callback` is set the function is **non-blocking** and an `error` argument is given to the callback.

#### <a name="get-param"></a>S7Server.GetParam(paramNumber)
Returns an internal server parameter.

 - `paramNumber` One from the parameter list [below](#table-area)

<a name="table-parameter"></a>

| Name                    | Value | Description |
|:------------------------|:-----:|:------------|
| `S7Server.LocalPort`    | 1     | Socket local port
| `S7Server.WorkInterval` | 6     | Socket worker interval
| `S7Server.PDURequest`   | 10    | Initial PDU length request
| `S7Server.MaxClients`   | 11    | Max clients allowed

Returns the `parameter value` on success or `false` on error.

#### <a name="set-param"></a>S7Server.SetParam(paramNumber, value)
Sets an internal server parameter.

 - `paramNumber` One from the parameter list [above](#table-area)
 - `value` New parameter value

Returns `true` on success or `false` on error.

#### <a name="set-resourceless"></a>S7Server.SetResourceless(value)
Sets the server to resourceless mode.

 - `value` new value

Returns `true` on success or `false` on error.

### <a name="memory-functions"></a>API - Memory functions

----------

#### <a name="register-area"></a>S7Server.RegisterArea(areaCode[, index], buffer)
Registers a memory area in the server. That memory block will be visible by the clients.

 - `areaCode` Area identifier (see table [below](#table-area))
 - `index` DB number if `areaCode` equals `srvAreaDB`, otherwise ignored
 - `buffer` User buffer

Returns `true` on success or `false` on error.

#### <a name="unregister-area"></a>S7Server.UnregisterArea(areaCode[, index])
Unregisters a memory area in the server.

 - `areaCode` Area identifier (see table [below](#table-area))
 - `index` DB number if `areaCode` equals `srvAreaDB`, otherwise ignored

Returns `true` on success or `false` on error.

#### <a name="get-area"></a>S7Server.GetArea(areaCode[, index])
Gets the content of a previously registered memory area block.

 - `areaCode` Area identifier (see table [below](#table-area))
 - `index` DB number if `areaCode` equals `srvAreaDB`, otherwise ignored

Returns a `buffer` object.

#### <a name="set-area"></a>S7Server.SetArea(areaCode[, index], buffer)
Sets the content of a previously registered memory area block.

 - `areaCode` Area identifier (see table [below](#table-area))
 - `index` DB number if `areaCode` equals `srvAreaDB`, otherwise ignored
 - `buffer` Buffer object

#### <a name="lock-area"></a>S7Server.LockArea(areaCode[, index])
Locks the memory area so that a server worker thread is blocked on access attempt until the lock is released with [UnlockArea()](#unlock-area).

 - `areaCode` Area identifier (see table [below](#table-area))
 - `index` DB number if `areaCode` equals `srvAreaDB`, otherwise ignored

#### <a name="unlock-area"></a>S7Server.UnlockArea(areaCode[, index])
Unlocks a previously locked memory area.


 - `areaCode` Area identifier (see table [below](#table-area))
 - `index` DB number if `areaCode` equals `srvAreaDB`, otherwise ignored

<a name="table-area"></a>

| Area                   | Value | Description |
|:-----------------------|:-----:|:------------|
| `S7Server.srvAreaPE`   |   0   | Process inputs
| `S7Server.srvAreaPA`   |   1   | Process outputs
| `S7Server.srvAreaMK`   |   2   | Merkers
| `S7Server.srvAreaCT`   |   3   | Counters
| `S7Server.srvAreaTM`   |   4   | Timers
| `S7Server.srvAreaDB`   |   5   | DB

### <a name="event-functions"></a>API - Event functions

----------

#### <a name="event-event"></a>S7Server event: 'event'
Emitted on server events.

 - `event` Event object

<a name="event-object"></a> Event object:

```javascript
{
  EvtTime;    // <Date>   Date
  EvtSender;  // <String> Sender
  EvtCode;    // <Number> Event code
  EvtRetCode; // <Number> Event result
  EvtParam1;  // <Number> Param 1 (if available)
  EvtParam2;  // <Number> Param 2 (if available)
  EvtParam3;  // <Number> Param 3 (if available)
  EvtParam4;  // <Number> Param 4 (if available)
}
```
Example:

```javascript
var s7server = new snap7.S7Server();

s7server.on("event", function(event) {
    console.log(s7server.EventText(event));
});

s7server.StartTo('127.0.0.1');
```

#### <a name="event-read-write"></a>S7Server event: 'readWrite'
Emitted on every read/write event. Only available in resourceless mode.

 - `sender` IPv4 address of the sender
 - `operation` Operation type
 - `tagObj` Tag object
 - `buffer` Buffer object
 - `callback` Callback function

The server worker thread is **blocked** until `callback` is called. Therefore **calling is crucial**, to prevent a deadlock in the worker thread.<br />
On a read event the `callback` expects a buffer as argument that is provided to the client. You can use the `buffer` argument which is an empty buffer of the correct size.

<a name="table-operation-type"></a>

| Operation type            | Value | Description          |
|:--------------------------|:-----:|:---------------------|
| `S7Server.operationRead`  | 0x00  | Read operation
| `S7Server.operationWrite` | 0x01  | Write operation

<a name="tag-object"></a> Tag object:

```javascript
{
  Area;     // <Number> Area code (DB, MK,…)
  DBNumber; // <Number> DB number (if any or 0)
  Start;    // <Number> Offset start
  Size;     // <Number> Number of elements
  WordLen;  // <Number> Tag WordLength
}
```

Example:

```javascript
var s7server = new snap7.S7Server();

s7server.SetResourceless(true);

s7server.on("readWrite", function(sender, operation, tagObj, buffer, callback) {
  console.log((operation === s7server.operationRead ? 'Read' : 'Write') + ' event from ' + sender);
  console.log('Area     : ' + tagObj.Area);
  console.log('DBNumber : ' + tagObj.DBNumber);
  console.log('Start    : ' + tagObj.Start);
  console.log('Size     : ' + tagObj.Size);
  console.log('WordLen  : ' + tagObj.WordLen);

  if (operation === s7server.operationRead) {
    buffer.fill(255);
    return callback(buffer);
  } else {
    console.log('Buffer   : ' + buffer);
    return callback();
  }
});

s7server.StartTo('127.0.0.1');
```

#### <a name="get-event-mask"></a>S7Server.GetEventMask()
Returns the server event filter mask.


#### <a name="set-event-mask"></a>S7Server.SetEventMask(mask)
Sets the server event filter mask.

 - `mask` Bit mask (see table [below](#table-mask))

<a name="table-mask"></a>

| Event code                          | Value          |
|:------------------------------------|:--------------:|
| `S7Server.evcAll`                   |   0xFFFFFFFF
| `S7Server.evcNone`                  |   0x00000000
| `S7Server.evcServerStarted`         |   0x00000001
| `S7Server.evcServerStopped`         |   0x00000002
| `S7Server.evcListenerCannotStart`   |   0x00000004
| `S7Server.evcClientAdded`           |   0x00000008
| `S7Server.evcClientRejected`        |   0x00000010
| `S7Server.evcClientNoRoom`          |   0x00000020
| `S7Server.evcClientException`       |   0x00000040
| `S7Server.evcClientDisconnected`    |   0x00000080
| `S7Server.evcClientTerminated`      |   0x00000100
| `S7Server.evcClientsDropped`        |   0x00000200
| `S7Server.evcPDUincoming`           |   0x00010000
| `S7Server.evcDataRead`              |   0x00020000
| `S7Server.evcDataWrite`             |   0x00040000
| `S7Server.evcNegotiatePDU`          |   0x00080000
| `S7Server.evcReadSZL`               |   0x00100000
| `S7Server.evcClock`                 |   0x00200000
| `S7Server.evcUpload`                |   0x00400000
| `S7Server.evcDownload`              |   0x00800000
| `S7Server.evcDirectory`             |   0x01000000
| `S7Server.evcSecurity`              |   0x02000000
| `S7Server.evcControl`               |   0x04000000


### <a name="miscellaneous-functions"></a>API - Miscellaneous functions

----------

#### <a name="last-error"></a>S7Server.LastError()
Returns the last job result.

#### <a name="event-text"></a>S7Server.EventText(eventObj)
Returns a textual explanation of a given event.

 - `eventObj` Event object (example [here](#event-object))

#### <a name="error-text"></a>S7Server.ErrorText(errNum)
Returns a textual explanation of a given error number.

 - `errNum` Error number

#### <a name="server-status"></a>S7Server.ServerStatus()
Returns the server status. (see table [below](#table-server-status))

<a name="table-server-status"></a>

| Status                | Value | Description          |
|:----------------------|:-----:|:---------------------|
| `S7Server.SrvStopped` | 0x00  | The Server is stopped
| `S7Server.SrvRunning` | 0x01  | The Server is Running
| `S7Server.SrvError`   | 0x02  | Server Error

#### <a name="clients-count"></a>S7Server.ClientsCount()
Returns the number of clients connected to the server.

#### <a name="get-cpu-status"></a>S7Server.GetCpuStatus()
Returns the Virtual CPU status. (see table [below](#table-cpu-status))

#### <a name="set-cpu-status"></a>S7Server.SetCpuStatus(cpuStatus)
Sets the Virtual CPU status.

 - `cpuStatus` Status value (see table [below](#table-cpu-status))

<a name="table-cpu-status"></a>

| Status                        | Value | Description              |
|:------------------------------|:-----:|:-------------------------|
| `S7Server.S7CpuStatusUnknown` | 0x00  | The CPU status is unknown
| `S7Server.S7CpuStatusRun`     | 0x08  | The CPU is running
| `S7Server.S7CpuStatusStop`    | 0x04  | The CPU is stopped
