/*
 * Copyright (c) 2017, Mathias Küsel
 * MIT License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

var events = require('events');
var util = require('util');

module.exports = snap7 = require('bindings')('node_snap7.node');

snap7.S7Client.prototype.DBRead = function (dbNumber, start, size, cb) {
    return this.ReadArea(this.S7AreaDB, dbNumber, start, size, this.S7WLByte, cb);
}

snap7.S7Client.prototype.DBWrite = function (dbNumber, start, size, buf, cb) {
    return this.WriteArea(this.S7AreaDB, dbNumber, start, size, this.S7WLByte, buf, cb);
}

snap7.S7Client.prototype.MBRead = function (start, size, cb) {
    return this.ReadArea(this.S7AreaMK, 0, start, size, this.S7WLByte, cb);
}

snap7.S7Client.prototype.MBWrite = function (start, size, buf, cb) {
    return this.WriteArea(this.S7AreaMK, 0, start, size, this.S7WLByte, buf, cb);
}

snap7.S7Client.prototype.EBRead = function (start, size, cb) {
    return this.ReadArea(this.S7AreaPE, 0, start, size, this.S7WLByte, cb);
}

snap7.S7Client.prototype.EBWrite = function (start, size, buf, cb) {
    return this.WriteArea(this.S7AreaPE, 0, start, size, this.S7WLByte, buf, cb);
}

snap7.S7Client.prototype.ABRead = function (start, size, cb) {
    return this.ReadArea(this.S7AreaPA, 0, start, size, this.S7WLByte, cb);
}

snap7.S7Client.prototype.ABWrite = function (start, size, buf, cb) {
    return this.WriteArea(this.S7AreaPA, 0, start, size, this.S7WLByte, buf, cb);
}

snap7.S7Client.prototype.TMRead = function (start, size, cb) {
    return this.ReadArea(this.S7AreaTM, 0, start, size, this.S7WLTimer, cb);
}

snap7.S7Client.prototype.TMWrite = function (start, size, buf, cb) {
    return this.WriteArea(this.S7AreaTM, 0, start, size, this.S7WLTimer, buf, cb);
}

snap7.S7Client.prototype.CTRead = function (start, size, cb) {
    return this.ReadArea(this.S7AreaCT, 0, start, size, this.S7WLCounter, cb);
}

snap7.S7Client.prototype.CTWrite = function (start, size, buf, cb) {
    return this.WriteArea(this.S7AreaCT, 0, start, size, this.S7WLCounter, buf, cb);
}

util.inherits(snap7.S7Server, events.EventEmitter);
