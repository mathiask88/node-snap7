/*
 * Copyright (c) 2019, Mathias Küsel
 * MIT License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

#ifndef SRC_NODE_SNAP7_CLIENT_H_
#define SRC_NODE_SNAP7_CLIENT_H_

#include <napi.h>
#include <snap7.h>
#include <mutex>

namespace node_snap7 {

enum class DataIOFunction { READAREA, WRITEAREA, READMULTI, WRITEMULTI
  , PLCSTATUS, GETPROTECTION, CLEARSESSIONPW, SETSESSIONPW, PLCSTOP
  , PLCCOLDSTART, PLCHOTSTART, GETCPINFO, GETCPUINFO, GETORDERCODE
  , SETPLCSYSTEMDATETIME, GETPLCDATETIME, COMPRESS, COPYRAMTOROM
  , SETPLCDATETIME, DBFILL, DBGET, DELETEBLOCK, DOWNLOAD, FULLUPLOAD
  , UPLOAD, LISTBLOCKSOFTYPE, GETAGBLOCKINFO, LISTBLOCKS, CONNECT
  , CONNECTTO, READSZLLIST, READSZL
};

class S7Client : public Napi::ObjectWrap<S7Client> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  explicit S7Client(const Napi::CallbackInfo &info);
  ~S7Client();

  // Internal Helper functions
  static int GetByteCountFromWordLen(int WordLen);
  Napi::Array S7DataItemToArray(PS7DataItem Items, int len
    , bool readMulti);
  Napi::Object S7ProtectionToObject(PS7Protection S7Protection);
  Napi::Object S7CpInfoToObject(PS7CpInfo CpInfo);
  Napi::Object S7CpuInfoToObject(PS7CpuInfo CpuInfo);
  Napi::Object S7OrderCodeToObject(PS7OrderCode OrderCode);
  Napi::Object S7BlockInfoToObject(PS7BlockInfo BlockInfo);
  Napi::Object S7BlocksListToObject(PS7BlocksList BlocksList);
  Napi::Array S7BlocksOfTypeToArray(PS7BlocksOfType BlocksList
    , int count);
  Napi::Array S7SZLListToArray(PS7SZLList SZLList, int count);

  static void FreeCallback(Napi::Env env, char *finalizeData);
  static void FreeCallbackSZL(Napi::Env env, char *finalizeData);

  std::mutex mutex;
  PS7Client snap7Client;

 private:
   // Control functions
  Napi::Value Connect(const Napi::CallbackInfo &info);
  Napi::Value ConnectTo(const Napi::CallbackInfo &info);
  Napi::Value SetConnectionParams(const Napi::CallbackInfo &info);
  Napi::Value SetConnectionType(const Napi::CallbackInfo &info);
  Napi::Value Disconnect(const Napi::CallbackInfo &info);
  Napi::Value GetParam(const Napi::CallbackInfo &info);
  Napi::Value SetParam(const Napi::CallbackInfo &info);
  // Data I/O Main functions
  Napi::Value ReadArea(const Napi::CallbackInfo &info);
  Napi::Value WriteArea(const Napi::CallbackInfo &info);
  Napi::Value ReadMultiVars(const Napi::CallbackInfo &info);
  Napi::Value WriteMultiVars(const Napi::CallbackInfo &info);
  // Directory functions
  Napi::Value ListBlocks(const Napi::CallbackInfo &info);
  Napi::Value GetAgBlockInfo(const Napi::CallbackInfo &info);
  Napi::Value GetPgBlockInfo(const Napi::CallbackInfo &info);
  Napi::Value ListBlocksOfType(const Napi::CallbackInfo &info);
  // Blocks functions
  Napi::Value Upload(const Napi::CallbackInfo &info);
  Napi::Value FullUpload(const Napi::CallbackInfo &info);
  Napi::Value Download(const Napi::CallbackInfo &info);
  Napi::Value Delete(const Napi::CallbackInfo &info);
  Napi::Value DBGet(const Napi::CallbackInfo &info);
  Napi::Value DBFill(const Napi::CallbackInfo &info);
  // Date/Time functions
  Napi::Value GetPlcDateTime(const Napi::CallbackInfo &info);
  Napi::Value SetPlcDateTime(const Napi::CallbackInfo &info);
  Napi::Value SetPlcSystemDateTime(const Napi::CallbackInfo &info);
  // System Info functions
  Napi::Value GetOrderCode(const Napi::CallbackInfo &info);
  Napi::Value GetCpuInfo(const Napi::CallbackInfo &info);
  Napi::Value GetCpInfo(const Napi::CallbackInfo &info);
  Napi::Value ReadSZL(const Napi::CallbackInfo &info);
  Napi::Value ReadSZLList(const Napi::CallbackInfo &info);
  // Control functions
  Napi::Value PlcHotStart(const Napi::CallbackInfo &info);
  Napi::Value PlcColdStart(const Napi::CallbackInfo &info);
  Napi::Value PlcStop(const Napi::CallbackInfo &info);
  Napi::Value CopyRamToRom(const Napi::CallbackInfo &info);
  Napi::Value Compress(const Napi::CallbackInfo &info);
  // Security functions
  Napi::Value GetProtection(const Napi::CallbackInfo &info);
  Napi::Value SetSessionPassword(const Napi::CallbackInfo &info);
  Napi::Value ClearSessionPassword(const Napi::CallbackInfo &info);
  // Properties
  Napi::Value ExecTime(const Napi::CallbackInfo &info);
  Napi::Value LastError(const Napi::CallbackInfo &info);
  Napi::Value PDURequested(const Napi::CallbackInfo &info);
  Napi::Value PDULength(const Napi::CallbackInfo &info);
  Napi::Value PlcStatus(const Napi::CallbackInfo &info);
  Napi::Value Connected(const Napi::CallbackInfo &info);

  Napi::Value ErrorText(const Napi::CallbackInfo &info);
};

class IOWorker : public Napi::AsyncWorker {
 public:
  // No args
  IOWorker(Napi::Function &callback, S7Client *s7client, DataIOFunction caller)
    : Napi::AsyncWorker(callback, "IOWorker"), s7client(s7client), caller(caller) {}
  // 1 args
  IOWorker(Napi::Function &callback, S7Client *s7client, DataIOFunction caller
    , void *arg1)
    : Napi::AsyncWorker(callback, "IOWorker"), s7client(s7client), caller(caller)
    , pData(arg1) {}
  IOWorker(Napi::Function &callback, S7Client *s7client, DataIOFunction caller
    , int arg1)
    : Napi::AsyncWorker(callback, "IOWorker"), s7client(s7client), caller(caller)
    , int1(arg1) {}
  // 2 args
  IOWorker(Napi::Function &callback, S7Client *s7client, DataIOFunction caller
    , void *arg1, int arg2)
    : Napi::AsyncWorker(callback, "IOWorker"), s7client(s7client), caller(caller)
    , pData(arg1), int1(arg2) {}
  IOWorker(Napi::Function &callback, S7Client *s7client, DataIOFunction caller
    , int arg1, int arg2)
    : Napi::AsyncWorker(callback, "IOWorker"), s7client(s7client), caller(caller)
    , int1(arg1), int2(arg2) {}
  // 3 args
  IOWorker(Napi::Function &callback, S7Client *s7client, DataIOFunction caller
    , void *arg1, int arg2, int arg3)
    : Napi::AsyncWorker(callback, "IOWorker"), s7client(s7client), caller(caller)
    , pData(arg1), int1(arg2), int2(arg3) {}
  // 4 args
  IOWorker(Napi::Function &callback, S7Client *s7client, DataIOFunction caller
    , void *arg1, int arg2, int arg3, int arg4)
    : Napi::AsyncWorker(callback, "IOWorker"), s7client(s7client), caller(caller)
    , pData(arg1), int1(arg2), int2(arg3), int3(arg4) {}
  // 6 args
  IOWorker(Napi::Function &callback, S7Client *s7client, DataIOFunction caller
    , void *arg1, int arg2, int arg3, int arg4, int arg5, int arg6)
    : Napi::AsyncWorker(callback, "IOWorker"), s7client(s7client), caller(caller)
    , pData(arg1), int1(arg2), int2(arg3), int3(arg4), int4(arg5), int5(arg6) {}

 protected:
  void Execute();
  void OnOK();
  ~IOWorker() {}

 private:
  S7Client *s7client;
  DataIOFunction caller;
  void *pData = nullptr;
  int int1 = 0, int2 = 0, int3 = 0, int4 = 0, int5 = 0, ret = 0;
};

}  // namespace node_snap7

#endif  // SRC_NODE_SNAP7_CLIENT_H_
