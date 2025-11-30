# node-snap7

[![npm](https://img.shields.io/npm/v/node-snap7.svg?label=&logo=npm)](https://www.npmjs.com/package/node-snap7)
[![Node version](https://img.shields.io/node/v/node-snap7.svg)](https://www.npmjs.com/package/node-snap7)
[![npm](https://img.shields.io/npm/dm/node-snap7.svg?label=dl)](https://www.npmjs.com/package/node-snap7)

**Current node-snap7 version:** 2.0.0-beta\
**Current snap7 version:** 1.4.3

node-snap7 now uses [node-addon-api](https://github.com/nodejs/node-addon-api) (N-API) instead of NAN, ships N-API prebuilds generated with [prebuildify](https://github.com/prebuild/prebuildify), and the S7Server implementation is considered stable. Promise, callback, and synchronous call styles are available across the API.

## About
This is a node.js wrapper for [snap7](https://github.com/davenardella/snap7). [snap7](https://github.com/davenardella/snap7) is an open source, 32/64 bit, multi-platform Ethernet communication suite for interfacing natively with Siemens S7 PLCs (see [compatibility](http://snap7.sourceforge.net/snap7_client.html#target_compatibility)).

## Installation
Install with:

```
npm install node-snap7
```

node-snap7 ships N-API prebuilt binaries generated with `prebuildify` and loaded locally through `node-gyp-build` during install. If a prebuild for your platform is not included, installation will fall back to compiling from source.

If you don't want to use the `prebuild` for the platform you are installing on, specify the `--build-from-source` flag when you install.

For building from source you need the following requirements:

 - Windows:
    - Visual Studio Build Tools with Desktop C++ workload (2019 or newer recommended)
    - [Python 3](https://www.python.org/downloads/)
 - Linux:
    - C++11 toolchain
    - [Python 3](https://www.python.org/downloads/)
    - `make` and other common build essentials

## Special thanks to
- Davide Nardella for creating [snap7](https://github.com/davenardella/snap7)

## How to use
### API
- [Client](doc/client.md)
- [Server](doc/server.md)

### Call styles
Each exported method is available as a Promise-returning async function, a callback-style function (when a callback is passed as the last argument), and a synchronous variant (with the `Sync` suffix).

### Client Example
```javascript
const snap7 = require('node-snap7');

const s7client = new snap7.S7Client();

async function main() {
    try {
        await s7client.ConnectTo('192.168.1.12', 0, 1);

        // Read the first byte from PLC process outputs and print it
        const res = await s7client.ABRead(0, 1);
        console.log(res);
    } catch (err) {
        console.error(' >> Operation failed. Code #', err, '-', s7client.ErrorText(err));
    }
}

main();
```

Callback style remains available:
```javascript
s7client.ABRead(0, 1, (err, res) => {
    if (err) console.error(s7client.ErrorText(err));
    else console.log(res);
});
```

Synchronous variants can be used with the `Sync` suffix:
```javascript
const res = s7client.ABReadSync(0, 1);
console.log(res);
```

### Server Example
```javascript
const snap7 = require('node-snap7');
const s7server = new snap7.S7Server();

// Set up event listener
s7server.on("event", function(event) {
    console.log(s7server.EventText(event));
});

// Create a new Buffer and register it to the server as DB1
const db1 = Buffer.alloc(100, 0);
s7server.RegisterArea(s7server.srvAreaDB, 1, db1);

// Start the server
s7server.StartTo('127.0.0.1');

// Close the server after 20s in this example
setTimeout(function() {
    s7server.Stop();
    s7server.UnregisterArea(s7server.srvAreaDB, 1);
}, 20000);
```

### Resourceless server example
```javascript
const snap7 = require('node-snap7');
const s7server = new snap7.S7Server();

// Enable resourceless mode to handle requests manually
s7server.SetResourceless(true);

s7server.on('readWrite', (sender, operation, tag, buffer, done) => {
    console.log(`${operation === s7server.operationRead ? 'Read' : 'Write'} from ${sender}`);
    console.log(tag);

    if (operation === s7server.operationRead) {
        buffer.fill(0x42); // respond with dummy data
        return done(buffer);
    }

    console.log('Payload:', buffer);
    done(); // always call to release the worker thread
});

s7server.StartTo('127.0.0.1');
```

## License & copyright
Copyright (c) 2025, Mathias Küsel

node-snap7 is licensed under the MIT license. All rights not explicitly granted in the MIT license are reserved. See the included LICENSE file for more details.

node-snap7 builds on the excellent work of the [snap7](https://github.com/davenardella/snap7) framework from Davide Nardella. [snap7](https://github.com/davenardella/snap7) is issued under the GPL/LGPLv3 (see [`./deps/snap7/gpl.txt`](./deps/snap7/gpl.txt) and [`./deps/snap7/lgpl-3.0.txt`](./deps/snap7/lgpl-3.0.txt)).
