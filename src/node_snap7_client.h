/*
 * Copyright (c) 2014, Mathias Küsel
 * MIT +no-false-attribs License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

#ifndef SRC_NODE_SNAP7_CLIENT_H_
#define SRC_NODE_SNAP7_CLIENT_H_

#include <node.h>
#include <nan.h>
#include "./snap7.h"

namespace node_snap7 {

  enum DataIOFunction { READAREA = 1, WRITEAREA };

  class S7Client : public node::ObjectWrap {
   public:
    S7Client() {}
    static void Init(v8::Handle<v8::Object> exports);
    static NAN_METHOD(New);
    // Control functions
    static NAN_METHOD(Connect);
    static NAN_METHOD(ConnectTo);
    static NAN_METHOD(SetConnectionParams);
    static NAN_METHOD(SetConnectionType);
    static NAN_METHOD(Disconnect);
    static NAN_METHOD(GetParam);
    static NAN_METHOD(SetParam);
    // Data I/O Main functions
    static NAN_METHOD(ReadArea);
    static NAN_METHOD(WriteArea);
    static NAN_METHOD(ReadMultiVars);
    static NAN_METHOD(WriteMultiVars);
    // Directory functions
    static NAN_METHOD(ListBlocks);
    static NAN_METHOD(GetAgBlockInfo);
    static NAN_METHOD(GetPgBlockInfo);
    static NAN_METHOD(ListBlocksOfType);
    // Blocks functions
    static NAN_METHOD(Upload);
    static NAN_METHOD(FullUpload);
    static NAN_METHOD(Download);
    static NAN_METHOD(Delete);
    static NAN_METHOD(DBGet);
    static NAN_METHOD(DBFill);
    // Date/Time functions
    static NAN_METHOD(GetPlcDateTime);
    static NAN_METHOD(SetPlcDateTime);
    static NAN_METHOD(SetPlcSystemDateTime);
    // System Info functions
    static NAN_METHOD(GetOrderCode);
    static NAN_METHOD(GetCpuInfo);
    static NAN_METHOD(GetCpInfo);
    static NAN_METHOD(ReadSZL);
    static NAN_METHOD(ReadSZLList);
    // Control functions
    static NAN_METHOD(PlcHotStart);
    static NAN_METHOD(PlcColdStart);
    static NAN_METHOD(PlcStop);
    static NAN_METHOD(CopyRamToRom);
    static NAN_METHOD(Compress);
    // Security functions
    static NAN_METHOD(GetProtection);
    static NAN_METHOD(SetSessionPassword);
    static NAN_METHOD(ClearSessionPassword);
    // Properties
    static NAN_METHOD(ExecTime);
    static NAN_METHOD(LastError);
    static NAN_METHOD(PDURequested);
    static NAN_METHOD(PDULength);
    static NAN_METHOD(PlcStatus);
    static NAN_METHOD(Connected);

    static NAN_METHOD(ErrorText);

    uv_mutex_t mutex;
    TS7Client *snap7Client;

   private:
    ~S7Client();
    static v8::Persistent<v8::FunctionTemplate> constructor;
  };

  class ConnectionWorker : public NanAsyncWorker {
   public:
    ConnectionWorker(
      NanCallback *callback,
      S7Client *s7client,
      NanUtf8String *address,
      int rack,
      int slot)
      : NanAsyncWorker(callback), s7client(s7client), address(address)
      , rack(rack), slot(slot) {}
    ~ConnectionWorker() {}

    void Execute();
    void HandleOKCallback();

   private:
    S7Client *s7client;
    NanUtf8String *address;
    int rack;
    int slot;
    int returnValue;
  };

  class IOWorker : public NanAsyncWorker {
   public:
    IOWorker(
      NanCallback *callback,
      S7Client *s7client,
      DataIOFunction caller,
      int area,
      int DB,
      int start,
      int amount,
      int WordLen,
      char *bufferData)
      : NanAsyncWorker(callback), s7client(s7client), caller(caller)
      , area(area), DB(DB), start(start), amount(amount), WordLen(WordLen)
      , bufferData(bufferData) {}
    ~IOWorker() {}

    void Execute();
    void HandleOKCallback();

   private:
    S7Client *s7client;
    DataIOFunction caller;
    int area;
    int DB;
    int start;
    int amount;
    int WordLen;
    char *bufferData;
    int returnValue;
  };

}  // namespace node_snap7

#endif  // SRC_NODE_SNAP7_CLIENT_H_
