/*
 * Copyright (c) 2019, Mathias Küsel
 * MIT License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

var events = require('events');

module.exports = snap7 = require('../binding');

snap7.S7Client.prototype.DBRead = async function (dbNumber, start, size) {
    return this.ReadArea(this.S7AreaDB, dbNumber, start, size, this.S7WLByte);
}

snap7.S7Client.prototype.DBWrite = async function (dbNumber, start, size, buf) {
    return this.WriteArea(this.S7AreaDB, dbNumber, start, size, this.S7WLByte, buf);
}

snap7.S7Client.prototype.MBRead = async function (start, size) {
    return this.ReadArea(this.S7AreaMK, 0, start, size, this.S7WLByte);
}

snap7.S7Client.prototype.MBWrite = async function (start, size, buf) {
    return this.WriteArea(this.S7AreaMK, 0, start, size, this.S7WLByte, buf);
}

snap7.S7Client.prototype.EBRead = async function (start, size) {
    return this.ReadArea(this.S7AreaPE, 0, start, size, this.S7WLByte);
}

snap7.S7Client.prototype.EBWrite = async function (start, size, buf) {
    return this.WriteArea(this.S7AreaPE, 0, start, size, this.S7WLByte, buf);
}

snap7.S7Client.prototype.ABRead = async function (start, size) {
    return this.ReadArea(this.S7AreaPA, 0, start, size, this.S7WLByte);
}

snap7.S7Client.prototype.ABWrite = async function (start, size, buf) {
    return this.WriteArea(this.S7AreaPA, 0, start, size, this.S7WLByte, buf);
}

snap7.S7Client.prototype.TMRead = async function (start, size) {
    return this.ReadArea(this.S7AreaTM, 0, start, size, this.S7WLTimer);
}

snap7.S7Client.prototype.TMWrite = async function (start, size, buf) {
    return this.WriteArea(this.S7AreaTM, 0, start, size, this.S7WLTimer, buf);
}

snap7.S7Client.prototype.CTRead = async function (start, size) {
    return this.ReadArea(this.S7AreaCT, 0, start, size, this.S7WLCounter);
}

snap7.S7Client.prototype.CTWrite = async function (start, size, buf) {
    return this.WriteArea(this.S7AreaCT, 0, start, size, this.S7WLCounter, buf);
}

//snap7.S7Server.super_ = events.EventEmitter;
//bject.setPrototypeOf(snap7.S7Server.prototype, events.EventEmitter.prototype);
