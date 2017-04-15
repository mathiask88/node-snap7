# node-snap7

[![NPM](https://nodei.co/npm/node-snap7.png?downloads=true)](https://nodei.co/npm/node-snap7/) [![NPM](https://nodei.co/npm-dl/node-snap7.png?months=1)](https://nodei.co/npm/node-snap7/)

**Current node-snap7 version:** 0.3.0
**Current snap7 version:** 1.4.1

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
    - [Python 2.7](https://www.python.org/downloads/release/python-2711/)
 - Linux:
     - gcc/g++ & make
     - [Python 2.7](https://www.python.org/downloads/release/python-2711/)

Building for Node.js 4.x or higher requires C++11 features so the minimum is VS2013 / gcc4.8

## Special thanks to
- Davide Nardella for creating snap7

## How to use
### API
- [Client](doc/client.md)
- [Server](doc/server.md)

### Client Example
```javascript
var snap7 = require('node-snap7');

var s7client = new snap7.S7Client();
s7client.ConnectTo('192.168.1.12', 0, 1, function(err) {
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

### Server Example
```javascript
var snap7 = require('node-snap7');

var s7server = new snap7.S7Server();

// Set up event listener
s7server.on("event", function(event) {
    console.log(s7server.EventText(event));
});

// Create a new Buffer and register it to the server as DB1
var db1 = new Buffer(100).fill('ÿ');
s7server.RegisterArea(s7server.srvAreaDB, 1, db1);

// Start the server
s7server.StartTo('127.0.0.1');

// Close the server after 20s in this example
setTimeout(function() {
    s7server.Stop();
    s7server.UnregisterArea(s7server.srvAreaDB, 1);
}, 20000);
```

## License & copyright
Copyright (c) 2017, Mathias Küsel

node-snap7 is licensed under the MIT license. All rights not explicitly granted in the MIT license are reserved. See the included LICENSE file for more details.

node-snap7 builds on the excellent work of the snap7 framework from Davide Nardella. Snap7 is issued under the GPL/LGPLv3 (see `./deps/snap7/gpl.txt ./deps/snap7/lgpl-3.0.txt`).
