/*
 * Copyright (c) 2017, Mathias Küsel
 * MIT License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

#ifndef SRC_NODE_SNAP7_CLIENT_H_
#define SRC_NODE_SNAP7_CLIENT_H_

#include <snap7.h>
#include <node.h>
#include <nan.h>

namespace node_snap7 {

enum DataIOFunction { READAREA = 1, WRITEAREA, READMULTI, WRITEMULTI
  , PLCSTATUS, GETPROTECTION, CLEARSESSIONPW, SETSESSIONPW, PLCSTOP
  , PLCCOLDSTART, PLCHOTSTART, GETCPINFO, GETCPUINFO, GETORDERCODE
  , SETPLCSYSTEMDATETIME, GETPLCDATETIME, COMPRESS, COPYRAMTOROM
  , SETPLCDATETIME, DBFILL, DBGET, DELETEBLOCK, DOWNLOAD, FULLUPLOAD
  , UPLOAD, LISTBLOCKSOFTYPE, GETAGBLOCKINFO, LISTBLOCKS, CONNECT
  , CONNECTTO, READSZLLIST, READSZL
};

class S7Client : public Nan::ObjectWrap {
 public:
  S7Client();
  static NAN_MODULE_INIT(Init);
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
  // Internal Helper functions
  static int GetByteCountFromWordLen(int WordLen);
  v8::Local<v8::Array> S7DataItemToArray(PS7DataItem Items, int len
    , bool readMulti);
  v8::Local<v8::Object> S7ProtectionToObject(PS7Protection S7Protection);
  v8::Local<v8::Object> S7CpInfoToObject(PS7CpInfo CpInfo);
  v8::Local<v8::Object> S7CpuInfoToObject(PS7CpuInfo CpuInfo);
  v8::Local<v8::Object> S7OrderCodeToObject(PS7OrderCode OrderCode);
  v8::Local<v8::Object> S7BlockInfoToObject(PS7BlockInfo BlockInfo);
  v8::Local<v8::Object> S7BlocksListToObject(PS7BlocksList BlocksList);
  v8::Local<v8::Array> S7BlocksOfTypeToArray(PS7BlocksOfType BlocksList
    , int count);
  v8::Local<v8::Array> S7SZLListToArray(PS7SZLList SZLList, int count);

  static void FreeCallback(char *data, void* hint);
  static void FreeCallbackSZL(char *data, void* hint);


  uv_mutex_t mutex;
  TS7Client *snap7Client;

 private:
  ~S7Client();
  static Nan::Persistent<v8::FunctionTemplate> constructor;
};

class IOWorker : public Nan::AsyncWorker {
 public:
  // No args
  IOWorker(Nan::Callback *callback, S7Client *s7client, DataIOFunction caller)
      : Nan::AsyncWorker(callback), s7client(s7client), caller(caller) {}
  // 1 args
  IOWorker(Nan::Callback *callback, S7Client *s7client, DataIOFunction caller
    , void *arg1)
    : Nan::AsyncWorker(callback), s7client(s7client), caller(caller)
    , pData(arg1) {}
  IOWorker(Nan::Callback *callback, S7Client *s7client, DataIOFunction caller
    , int arg1)
    : Nan::AsyncWorker(callback), s7client(s7client), caller(caller)
    , int1(arg1) {}
  // 2 args
  IOWorker(Nan::Callback *callback, S7Client *s7client, DataIOFunction caller
    , void *arg1, int arg2)
    : Nan::AsyncWorker(callback), s7client(s7client), caller(caller)
    , pData(arg1), int1(arg2) {}
  IOWorker(Nan::Callback *callback, S7Client *s7client, DataIOFunction caller
    , int arg1, int arg2)
    : Nan::AsyncWorker(callback), s7client(s7client), caller(caller)
    , int1(arg1), int2(arg2) {}
  // 3 args
  IOWorker(Nan::Callback *callback, S7Client *s7client, DataIOFunction caller
    , void *arg1, int arg2, int arg3)
    : Nan::AsyncWorker(callback), s7client(s7client), caller(caller)
    , pData(arg1), int1(arg2), int2(arg3) {}
  // 4 args
  IOWorker(Nan::Callback *callback, S7Client *s7client, DataIOFunction caller
    , void *arg1, int arg2, int arg3, int arg4)
    : Nan::AsyncWorker(callback), s7client(s7client), caller(caller)
    , pData(arg1), int1(arg2), int2(arg3), int3(arg4) {}
  // 6 args
  IOWorker(Nan::Callback *callback, S7Client *s7client, DataIOFunction caller
    , void *arg1, int arg2, int arg3, int arg4, int arg5, int arg6)
    : Nan::AsyncWorker(callback), s7client(s7client), caller(caller)
    , pData(arg1), int1(arg2), int2(arg3), int3(arg4), int4(arg5), int5(arg6) {}

  ~IOWorker() {}

 private:
  void Execute();
  void HandleOKCallback();

  S7Client *s7client;
  DataIOFunction caller;
  void *pData;
  int int1, int2, int3, int4, int5, returnValue;
};

}  // namespace node_snap7

#endif  // SRC_NODE_SNAP7_CLIENT_H_
