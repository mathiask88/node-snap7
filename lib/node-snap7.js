/*
 * Copyright (c) 2025, Mathias Küsel
 * MIT License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

const EventEmitter = require('events').EventEmitter;
const inherits = require('util').inherits;
const snap7 = require('../binding');
const { S7Client, S7Server, S7Partner, S7Area, S7WordLen } = snap7;

module.exports = snap7;

inherits(S7Server, EventEmitter);
inherits(S7Partner, EventEmitter);

const serverMethodNames = ['Start', 'StartTo', 'Stop'];

function createServerPrototypeMethods(methodName) {
  // Promise first; optionally bridge to callback
  S7Server.prototype[methodName] = function (...args) {
    const cb = typeof args[args.length - 1] === 'function' ? args.pop() : undefined;

    // Force async path in native by passing a noop so a Promise is returned.
    const promise = this[`_${methodName}`](...args, () => { });

    if (cb) {
      promise.then((res) => cb(null, res), (err) => cb(err));
      return;
    }
    return promise;
  };

  // Sync call-through for users who need blocking behavior.
  S7Server.prototype[`${methodName}Sync`] = function (...args) {
    return this[`_${methodName}`](...args);
  };
}

serverMethodNames.forEach(createServerPrototypeMethods);

const partnerMethodNames = ['Start', 'StartTo', 'Stop', 'BSend', 'BRecv'];

function createPartnerPrototypeMethods(methodName) {
  // Promise first; optionally bridge to callback
  S7Partner.prototype[methodName] = function (...args) {
    const cb = typeof args[args.length - 1] === 'function' ? args.pop() : undefined;

    // Force async path in native by passing a noop so a Promise is returned.
    const promise = this[`_${methodName}`](...args, () => { });

    if (cb) {
      promise.then((res) => cb(null, res), (err) => cb(err));
      return;
    }
    return promise;
  };

  // Sync call-through for users who need blocking behavior.
  S7Partner.prototype[`${methodName}Sync`] = function (...args) {
    return this[`_${methodName}`](...args);
  };
}

partnerMethodNames.forEach(createPartnerPrototypeMethods);

const clientMethodNames = [
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

function createClientPrototypeMethods(methodName) {
  // Promise first; optionally bridge to callback
  S7Client.prototype[methodName] = function (...args) {
    const cb = typeof args[args.length - 1] === 'function' ? args.pop() : undefined;

    // Native promises are returned only when a function arg is present, so pass a noop.
    const promise = this[`_${methodName}`](...args, () => { });

    if (cb) {
      promise.then((res) => cb(null, res), (err) => cb(err));
      return;
    }
    return promise;
  };

  // Keep synchronous call-through for users who need blocking behavior.
  S7Client.prototype[`${methodName}Sync`] = function (...args) {
    return this[`_${methodName}`](...args);
  };
}

// Apply to all methods
clientMethodNames.forEach(createClientPrototypeMethods);

const areaConfigs = [
  { prefix: 'DB', area: S7Area.DB, wordLen: S7WordLen.Byte, useDBNumber: true },
  { prefix: 'MB', area: S7Area.MK, wordLen: S7WordLen.Byte },
  { prefix: 'EB', area: S7Area.PE, wordLen: S7WordLen.Byte },
  { prefix: 'AB', area: S7Area.PA, wordLen: S7WordLen.Byte },
  { prefix: 'TM', area: S7Area.TM, wordLen: S7WordLen.Timer },
  { prefix: 'CT', area: S7Area.CT, wordLen: S7WordLen.Counter }
];

areaConfigs.forEach(({ prefix, area, wordLen, useDBNumber }) => {
  // Async Read with Promise/Callback
  S7Client.prototype[`${prefix}Read`] = function (...args) {
    const callback = typeof args[args.length - 1] === 'function' ? args.pop() : undefined;
    const [start, size, dbNumber] = useDBNumber ? [args[1], args[2], args[0]] : [args[0], args[1], 0];

    if (callback) return this.ReadArea(area, dbNumber, start, size, wordLen, callback);

    return this.ReadArea(area, dbNumber, start, size, wordLen);
  };

  // Sync Read
  S7Client.prototype[`${prefix}ReadSync`] = function (...args) {
    const [start, size, dbNumber] = useDBNumber ? [args[1], args[2], args[0]] : [args[0], args[1], 0];
    return this.ReadAreaSync(area, dbNumber, start, size, wordLen);
  };

  // Async Write with Promise/Callback
  S7Client.prototype[`${prefix}Write`] = function (...args) {
    const callback = typeof args[args.length - 1] === 'function' ? args.pop() : undefined;
    const [start, size, buf, dbNumber] = useDBNumber ? [args[1], args[2], args[3], args[0]] : [args[0], args[1], args[2], 0];

    if (callback) return this.WriteArea(area, dbNumber, start, size, wordLen, buf, callback);

    return this.WriteArea(area, dbNumber, start, size, wordLen, buf);
  };

  // Sync Write
  S7Client.prototype[`${prefix}WriteSync`] = function (...args) {
    const [start, size, buf, dbNumber] = useDBNumber ? [args[1], args[2], args[3], args[0]] : [args[0], args[1], args[2], 0];
    return this.WriteAreaSync(area, dbNumber, start, size, wordLen, buf);
  };
});
