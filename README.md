# node-snap7

[![npm](https://img.shields.io/npm/v/node-snap7.svg?label=&logo=npm)](https://www.npmjs.com/package/node-snap7)
[![Node version](https://img.shields.io/node/v/node-snap7.svg)](https://www.npmjs.com/package/node-snap7)
[![Test and Release](https://github.com/mathiask88/node-snap7/actions/workflows/test-and-release.yml/badge.svg)](https://github.com/mathiask88/node-snap7/actions/workflows/test-and-release.yml)
[![npm](https://img.shields.io/npm/dm/node-snap7.svg?label=dl)](https://www.npmjs.com/package/node-snap7)

**Current node-snap7 version:** 1.0.8\
**Current snap7 version:** 1.4.2

**In my spare time I am working on a [node-addon-api](https://github.com/nodejs/node-addon-api) rewrite and want to switch from [prebuild-install](https://github.com/prebuild/prebuild-install) to [prebuildify](https://github.com/prebuild/prebuildify).\
The current S7Server implementation has some bugs, please use with caution.**

## About
This is a node.js wrapper for snap7. Snap7 is an open source, 32/64 bit, multi-platform Ethernet communication suite for interfacing natively with Siemens S7 PLCs (See [compatibility](http://snap7.sourceforge.net/snap7_client.html#target_compatibility)).

## Installation
Install with:

    npm install node-snap7

node-snap7 uses `prebuild` and `prebuild-install` for handling prebuilt binaries. See [this list](https://github.com/mathiask88/node-snap7/releases) of supported prebuilt platform binaries. When installing node-snap7 `prebuild-install` will install prebuilt binaries from GitHub if they exist and fallback to a compile step if they don't.

If you don't want to use the `prebuild` for the platform you are installing on, specify the `--build-from-source` flag when you install.

For building from source you need the following requirements:

 - Windows:
    - [Visual Studio 2013 Express or higher](https://www.visualstudio.com/de/vs/visual-studio-express/)
    - [Python 2.7](https://www.python.org/downloads/release/python-2714/)
 - Linux:
    - C++11 toolchain
    - [Python 2.7](https://www.python.org/downloads/release/python-2714/)

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

Have a look at the resourceless server example [here](doc/server.md#event-read-write).

## License & copyright
Copyright (c) 2019, Mathias Küsel

node-snap7 is licensed under the MIT license. All rights not explicitly granted in the MIT license are reserved. See the included LICENSE file for more details.

node-snap7 builds on the excellent work of the snap7 framework from Davide Nardella. Snap7 is issued under the GPL/LGPLv3 (see `./deps/snap7/gpl.txt ./deps/snap7/lgpl-3.0.txt`).
