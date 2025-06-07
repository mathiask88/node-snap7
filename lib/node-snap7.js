/*
 * Copyright (c) 2019, Mathias Küsel
 * MIT License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

const EventEmitter = require('events').EventEmitter;
const inherits = require('util').inherits;
const snap7 = require('../binding');

module.exports = snap7;

inherits(snap7.S7Server, EventEmitter);

const methodNames = [
  'Connect',
  'ConnectTo',
  'ReadArea',
  'WriteArea',
  'ReadMultiVars',
  'WriteMultiVars',
  'ListBlocks',
  'ListBlocksOfType',
  'GetAgBlockInfo',
  'FullUpload',
  'Upload',
  'Download',
  'Delete',
  'DBGet',
  'DBFill',
  'GetPlcDateTime',
  'SetPlcDateTime',
  'SetPlcSystemDateTime',
  'ReadSZL',
  'ReadSZLList',
  'GetOrderCode',
  'GetCpuInfo',
  'GetCpInfo',
  'PlcHotStart',
  'PlcColdStart',
  'PlcStop',
  'CopyRamToRom',
  'Compress',
  'SetSessionPassword',
  'ClearSessionPassword',
  'GetProtection',
  'PlcStatus'
];

function createPrototypeMethods(methodName) {
  // Create async version with Promise/Callback support
  snap7.S7Client.prototype[methodName] = function (...args) {
    const callback = typeof args[args.length - 1] === 'function' ? args.pop() : null;

    if (callback) {
      return this[`_${methodName}`](...args, callback);
    } else {
      return new Promise((resolve, reject) => {
        this[`_${methodName}`](...args, (err, res) => {
          if (err) reject(err);
          else resolve(res);
        });
      });
    }
  };

  // Create sync version
  snap7.S7Client.prototype[`${methodName}Sync`] = function (...args) {
    return this[`_${methodName}`](...args);
  };
}

// Apply to all methods
methodNames.forEach(createPrototypeMethods);

const areaConfigs = [
  { prefix: 'DB', area: 'S7AreaDB', wordLen: 'S7WLByte', useDBNumber: true },
  { prefix: 'MB', area: 'S7AreaMK', wordLen: 'S7WLByte' },
  { prefix: 'EB', area: 'S7AreaPE', wordLen: 'S7WLByte' },
  { prefix: 'AB', area: 'S7AreaPA', wordLen: 'S7WLByte' },
  { prefix: 'TM', area: 'S7AreaTM', wordLen: 'S7WLTimer' },
  { prefix: 'CT', area: 'S7AreaCT', wordLen: 'S7WLCounter' }
];

areaConfigs.forEach(({ prefix, area, wordLen, useDBNumber }) => {
  // Async Read with Promise/Callback
  snap7.S7Client.prototype[`${prefix}Read`] = function (...args) {
    const callback = typeof args[args.length - 1] === 'function' ? args.pop() : undefined;
    const [start, size, dbNumber] = useDBNumber ? [args[1], args[2], args[0]] : [args[0], args[1], 0];

    if (callback)
      return this.ReadArea(this[area], dbNumber, start, size, this[wordLen], callback);

    return this.ReadArea(this[area], dbNumber, start, size, this[wordLen]);
  };

  // Sync Read
  snap7.S7Client.prototype[`${prefix}ReadSync`] = function (...args) {
    const [start, size, dbNumber] = useDBNumber ? [args[1], args[2], args[0]] : [args[0], args[1], 0];
    return this.ReadAreaSync(this[area], dbNumber, start, size, this[wordLen]);
  };

  // Async Write with Promise/Callback
  snap7.S7Client.prototype[`${prefix}Write`] = function (...args) {
    const callback = typeof args[args.length - 1] === 'function' ? args.pop() : undefined;
    const [start, size, buf, dbNumber] = useDBNumber ? [args[1], args[2], args[3], args[0]] : [args[0], args[1], args[2], 0];

    if (callback)
      return this.WriteArea(this[area], dbNumber, start, size, this[wordLen], buf, callback);

    return this.WriteArea(this[area], dbNumber, start, size, this[wordLen], buf);
  };

  // Sync Write
  snap7.S7Client.prototype[`${prefix}WriteSync`] = function (...args) {
    const [start, size, buf, dbNumber] = useDBNumber ? [args[1], args[2], args[3], args[0]] : [args[0], args[1], args[2], 0];
    return this.WriteAreaSync(this[area], dbNumber, start, size, this[wordLen], buf);
  };
});
