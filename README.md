# node-snap7

[![NPM](https://nodei.co/npm/node-snap7.png?downloads=true)](https://nodei.co/npm/node-snap7/) [![NPM](https://nodei.co/npm-dl/node-snap7.png?months=1)](https://nodei.co/npm/node-snap7/)

**Build status:** Linux [![Build Status](https://travis-ci.org/mathiask88/node-snap7.svg?branch=master)](https://travis-ci.org/mathiask88/node-snap7) Windows [![Build status](https://ci.appveyor.com/api/projects/status/ca1kepectsn3wkrc)](https://ci.appveyor.com/project/mathiask88/node-snap7)

**!!! node-snap7 is still a work in progress !!!**

## About
This is a node.js wrapper for snap7. Snap7 is an open source, 32/64 bit, multi-platform Ethernet communication suite for interfacing natively with Siemens S7 PLCs (See [compatibility](http://snap7.sourceforge.net/snap7_client.html#target_compatibility)).

## Installation
Install with:

    npm install node-snap7

Requirements:  

 - Windows:
    - Visual Studio 2010 Express or higher e.g [Visual Studio 2013 Express](http://www.microsoft.com/visualstudio/eng/2013-downloads#d-2013-express)
    - [Python 2.7](http://www.python.org/download/releases/2.7.6/)
 - Linux:
     - gcc & make
     - [Python 2.7](http://www.python.org/download/releases/2.7.6/)

## Special thanks to
- Davide Nardella for creating snap7

## How to use
### Example
```javascript
var snap7 = require('node-snap7');

var s7client = new snap7.S7Client();
s7client.ConnectTo('127.0.0.1', 0, 1, function(err) {
    if(err)
        return console.log(' >> Connection failed. Code #' + err + ' - ' + s7client.ErrorText(err));
    
    // Read the first byte from PLC process outputs...
    s7client.ABRead(0, 1, function(err, res) {
        if(err)
            return console.log(' >> ABRead failed. Code #' + err + ' - ' + s7client.ErrorText(err));
        
        // ... and write it to stdout
        console.log(res)
    });
});
```

### API
 - [Control functions](#control-functions)
     - [Connect()](#connect)
     - [ConnectTo()](#connect-to)
     - [SetConnectionParams()](#set-connection-params)
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
     - [PduRequested()](#pdu-requested)
     - [PduLength()](#pdu-length)
     - [PlcStatus()](#plc-status)
     - [Connected()](#connected)
     - [ErrorText()](#error-text)

### <a name="control-functions"></a>API - Control functions

----------

#### <a name="connect"></a>S7Client.Connect([callback])
Connects the client to the PLC with the parameters specified in the previous call of `ConnectTo()` or `SetConnectionParams()`.  

 - The optional `callback` parameter will be executed after connection attempt
 
If `callback` is **not** set the function returns `true` on success or `false` on error.  
If `callback` is set an `error` argument is given to the callback

#### <a name="connect-to"></a>S7Client.ConnectTo(ip, rack, slot, [callback])
Connects the client to the hardware at (`IP`, `Rack`, `Slot`) Coordinates.

- `ip` PLC/Equipment IPV4 Address ex. “192.168.1.12”
- `rack` PLC Rack number
- `slot` PLC Slot number
- The optional `callback` parameter will be executed after connection attempt

If `callback` is **not** set the function returns `true` on success or `false` on error.  
If `callback` is set an `error` argument is given to the callback

#### <a name="set-connection-params"></a>S7Client.SetConnectionParams(ip, localTSAP, remoteTSAP)
Sets internally (IP, LocalTSAP, RemoteTSAP) Coordinates.

- `ipP` PLC/Equipment IPV4 Address ex. “192.168.1.12”
- `localTSAP` Local TSAP (PC TSAP)
- `remoteTSAP`  Remote TSAP (PLC TSAP)

Returns `true` on success or `false` on error.

#### <a name="disconnect"></a>S7Client.Disconnect()
Disconnects “gracefully” the Client from the PLC.  

Returns `true` on success or `false` on error.

#### <a name="get-param"></a>S7Client.GetParam(paramNumber)
Reads an internal Client object parameter.

 - `paramNumber` One from the parameter list below
    
| Name                   | Value | Description |
|:-----------------------|:-----:|:------------|
| `S7Client.RemotePort`  | 2     | Socket remote Port.     
| `S7Client.PingTimeout` | 3     | Client Ping timeout.    
| `S7Client.SendTimeout` | 4     | Socket Send timeout.     
| `S7Client.RecvTimeout` | 5     | Socket Recv timeout.      
| `S7Client.SrcRef`      | 7     | ISOTcp Source reference.    
| `S7Client.DstRef`      | 8     | ISOTcp Destination reference
| `S7Client.SrcTSap`     | 9     | ISOTcp Source TSAP.
| `S7Client.PDURequest`  | 10    | Initial PDU length request.

Returns the `parameter value` on success or `false` on error.

#### <a name="set-param"></a>S7Client.SetParam(paramNumber, value)
Sets an internal Client object parameter.

 - `paramNumber` One from the parameter list above
 - `value` New parameter value
 
Returns `true` on success or `false` on error.

### <a name="data-functions"></a>API - Data I/O functions

----------

#### <a name="read-area"></a>S7Client.ReadArea(area, dbNumber, start, amount, wordLen, [callback])
This is the main function to read data from a PLC. With it you can read DB, Inputs, Outputs, Merkers, Timers and Counters.

 - `area`
 - `dbNumber`
 - `start`
 - `amount`
 - `wordLen`
 - `callback`

#### <a name="write-area"></a>S7Client.WriteArea(area, dbNumber, start, amount, wordLen, buffer, [callback])
This is the main function to write data into a PLC.

 - `area`
 - `dbNumber`
 - `start`
 - `amount`
 - `wordLen`
 - `buffer`
 - `callback`

#### <a name="dbread"></a>S7Client.DBRead(dbNumber, start, size, [callback])
This is a lean function of `ReadArea()` to read PLC DB.  
It simply internally calls `ReadArea()` with `area = S7Client.S7AreaDB` and `wordLen = S7Client.S7WLBytes`.

 - `dbNumber`
 - `start`
 - `size`
 - `callback`

#### <a name="dbwrite"></a>S7Client.DBWrite(dbNumber, start, size, buffer, [callback])
This is a lean function of `WriteArea()` to write PLC DB.  
It simply internally calls `WriteArea()` with `area = S7Client.S7AreaDB` and `wordLen = S7Client.S7WLBytes`.

 - `dbNumber`
 - `start`
 - `size`
 - `buffer`
 - `callback`
 
#### <a name="abread"></a>S7Client.ABRead(start, size, [callback])
This is a lean function of `ReadArea()` to read PLC process outputs.  
It simply internally calls `ReadArea()` with `area = S7Client.S7AreaPA` and `wordLen = S7Client.S7WLBytes`.

 - `start`
 - `size`
 - `callback`
 
#### <a name="abwrite"></a>S7Client.ABWrite(start, size, buffer, [callback])
This is a lean function of `WriteArea()` to write PLC process outputs.  
It simply internally calls `WriteArea()` with `area = S7Client.S7AreaPA` and `wordLen = S7Client.S7WLBytes`.

 - `start`
 - `size`
 - `buffer`
 - `callback`
 
#### <a name="ebread"></a>S7Client.EBRead(start, size, [callback])
This is a lean function of `ReadArea()` to read PLC process inputs.  
It simply internally calls `ReadArea()` with `area = S7Client.S7AreaPE` and `wordLen = S7Client.S7WLBytes`.

 - `start`
 - `size`
 - `callback`
 
#### <a name="ebwrite"></a>S7Client.EBWrite(start, size, buffer, [callback])
This is a lean function of `WriteArea()` to write PLC process inputs.  
It simply internally calls `WriteArea()` with `area = S7Client.S7AreaPE` and `wordLen = S7Client.S7WLBytes`.

 - `start`
 - `size`
 - `buffer`
 - `callback`
 
#### <a name="mbread"></a>S7Client.MBRead(start, size, [callback])
This is a lean function of `ReadArea()` to read PLC Merkers.  
It simply internally calls `ReadArea()` with `area = S7Client.S7AreaMK` and `wordLen = S7Client.S7WLBytes`.

 - `start`
 - `size`
 - `callback`
 
#### <a name="mbwrite"></a>S7Client.MBWrite(start, size, buffer, [callback])
This is a lean function of `WriteArea()` to write PLC Merkers.  
It simply internally calls `WriteArea()` with `area = S7Client.S7AreaMK` and `wordLen = S7Client.S7WLBytes`.

 - `start`
 - `size`
 - `buffer`
 - `callback`
 
#### <a name="tmread"></a>S7Client.TMRead(start, amount, [callback])
This is a lean function of `ReadArea()` to read PLC Timers.  
It simply internally calls `ReadArea()` with `area = S7Client.S7AreaTM` and `wordLen = S7Client.S7WLTimer`.

 - `start`
 - `amount`
 - `callback`
 
#### <a name="tmwrite"></a>S7Client.TMWrite(start, amount, buffer, [callback])
This is a lean function of `WriteArea()` to write PLC Timers.  
It simply internally calls `WriteArea()` with `area = S7Client.S7AreaTM` and `wordLen = S7Client.S7WLTimer`.

 - `start`
 - `amount`
 - `buffer`
 - `callback`

#### <a name="ctread"></a>S7Client.CTRead(start, amount, [callback])
This is a lean function of `ReadArea()` to read PLC Counters.  
It simply internally calls `ReadArea()` with `area = S7Client.S7AreaCT` and `wordLen = S7Client.S7WLCounter`.

 - `start`
 - `amount`
 - `callback`
 
#### <a name="ctwrite"></a>S7Client.CTWrite(start, amount, buffer, [callback])
This is a lean function of `WriteArea()` to write PLC Counters.  
It simply internally calls `WriteArea()` with `area = S7Client.S7AreaCT` and `wordLen = S7Client.S7WLCounter`.

 - `start`
 - `amount`
 - `buffer`
 - `callback`
  
#### <a name="read-multi-vars"></a>S7Client.ReadMultiVars(multiVars, [callback])
This is function allows to read different kind of variables from a PLC in a single call. With it you can read DB, Inputs, Outputs, Merkers, Timers and Counters.

 - ...

#### <a name="write-multi-vars"></a>S7Client.WriteMultiVars(multiVars, [callback])
This is function allows to write different kind of variables into a PLC in a single call. With it you can write DB, Inputs, Outputs, Merkers, Timers and Counters.

 - ...

### <a name="directory-functions"></a>API - Directory functions

----------

#### <a name="list-blocks"></a>S7Client.ListBlocks()
This function returns the AG blocks amount divided by type.

#### <a name="list-blocks-of-type"></a>S7Client.ListBlocksOfType(blockType)
This function returns the AG list of a specified block type.

 - `blockType`

#### <a name="get-ag-blockinfo"></a>S7Client.GetAgBlockInfo(blockType, blockNum)
Returns detailed information about an AG given block.
This function is very useful if you need to read or write data in a DB which you do not know the size in advance (see MC7Size field)

 - `blockType`
 - `blockNum`
 
#### <a name="get-pg-blockinfo"></a>S7Client.GetPgBlockInfo(buffer)
Returns detailed information about a block present in a user buffer. This function is usually used in conjunction with `FullUpload()`.
An uploaded block saved to disk, could be loaded in a user buffer and checked with this function.

 - `buffer`
 
### <a name="block-functions"></a>API - Block oriented functions

----------

#### <a name="full-upload"></a>S7Client.FullUpload(blockType, blockNum, [callback])
Uploads a block from AG. The whole block (including header and footer) is copied into the user buffer.

 - `blockType`
 - `blockNum`
 - `callback`
 
#### <a name="upload"></a>S7Client.Upload(blockType, blockNum, [callback])
Uploads a block body from AG. Only the block body (but header and footer) is copied into the user buffer.

 - `blockType`
 - `blockNum`
 - `callback`
 
#### <a name="download"></a>S7Client.Download(blockNum, buffer, [callback])
Downloads a block into AG. A whole block (including header and footer) must be available into the user buffer.

 - `blockNum`
 - `buffer`
 - `callback`
 
#### <a name="delete"></a>S7Client.Delete(blockType, blockNum, [callback])
Deletes a block into AG.

    !!! There is no undo function available. !!!
    
 - `blockType`
 - `blockNum`
 - `callback`
 
#### <a name="dbget"></a>S7Client.DBGet(dbNumber, [callback])
Uploads a DB from AG. This function is equivalent to `Upload()` with `BlockType = Block_DB` but it uses a different approach so it’s not subject to the security level set. Only data is uploaded.

 - `dbNumber`
 - `callback`
 
#### <a name="dbfill"></a>S7Client.DBFill(dbNumber, fillChar, [callback])
Fills a DB in AG with a given byte without the need of specifying its size.

 - `dbNumber`
 - `fillChar`
 - `callback`
 
### <a name="datetime-functions"></a>API - Date/Time functions

----------

#### <a name="get-plc-datetime"></a>S7Client.GetPlcDateTime()
Reads PLC date and time.

#### <a name="set-plc-datetime"></a>S7Client.SetPlcDateTime(dateTime)
Sets the PLC date and time.

 - `dateTime`
 
#### <a name="set-plc-system-datetime"></a>S7Client.SetPlcSystemDateTime()
Sets the PLC date and time in accord to the PC system Date/Time.

### <a name="systeminfo-functions"></a>API - System info functions

----------

#### <a name="read-szl"></a>S7Client.ReadSZL()
Reads a partial list of given ID and INDEX.

#### <a name="read-szl-list"></a>S7Client.ReadSZLList()
Reads the directory of the partial lists.

#### <a name="get-order-code"></a>S7Client.GetOrderCode()
Gets CPU order code and version info.

#### <a name="get-cpu-info"></a>S7Client.GetCpuInfo()
Gets CPU module name, serial number and other info.

#### <a name="get-cp-info"></a>S7Client.GetCpInfo()
Gets CP (communication processor) info.

### <a name="control-functions"></a>API - PLC control functions

----------

#### <a name="plc-hot-start"></a>S7Client.PlcHotStart()
Puts the CPU in RUN mode performing an HOT START.

#### <a name="plc-cold-start"></a>S7Client.PlcColdStart()
Puts the CPU in RUN mode performing a COLD START.

#### <a name="plc-stop"></a>S7Client.PlcStop()
Puts the CPU in STOP mode.

#### <a name="copy-ram-to-rom"></a>S7Client.CopyRamToRom(timeout)
Performs the Copy Ram to Rom action.

#### <a name="compress"></a>S7Client.Compress(timeout)
Performs the Memory compress action.

### <a name="security-functions"></a>API - Security functions

----------

#### <a name="set-session-password"></a>S7Client.SetSessionPassword(password)
Send the password to the PLC to meet its security level.

#### <a name="clear-session-password"></a>S7Client.ClearSessionPassword
Clears the password set for the current session (logout).

#### <a name="get-protection"></a>S7Client.GetProtection()
Gets the CPU protection level info.

### <a name="properties"></a>API - Properties

----------

#### <a name="exec-time"></a>S7Client.ExecTime()
Returns the last job execution time in milliseconds.

#### <a name="last-error"></a>S7Client.LastError()
Returns the last job result.

#### <a name="pdu-requested"></a>S7Client.PduRequested()

#### <a name="pdu-length"></a>S7Client.PduLength()
Returns info about the PDU length.

#### <a name="plc-status"></a>S7Client.PlcStatus()
Returns the CPU status (running/stopped).

#### <a name="connected"></a>S7Client.Connected()
Returns the connection status

#### <a name="error-text"></a>S7Client.ErrorText(errNum)
Returns a textual explanation of a given error number.

 - `errNum`

## Licence & copyright
Copyright (c) 2014, Mathias Küsel

node-snap7 is licensed under an MIT +no-false-attribs license. All rights not explicitly granted in the MIT license are reserved. See the included LICENSE file for more details.

node-snap7 builds on the excellent work of the snap7 framework from Davide Nardella. Snap7 is issued under the GPL/LGPLv3 (see ```./deps/snap7/gpl.txt ./deps/snap7/lgpl-3.0.txt```).
