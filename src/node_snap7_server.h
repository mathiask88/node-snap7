/*
 * Copyright (c) 2019, Mathias Küsel
 * MIT License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

#ifndef SRC_NODE_SNAP7_SERVER_H_
#define SRC_NODE_SNAP7_SERVER_H_

#include <napi.h>
#include <snap7.h>

namespace node_snap7 {

enum class ServerIOFunction {
  START, STARTTO, STOP
};

typedef struct {
  char* pBuffer;
  word size;
}TBufferInfo;

class S7Server : public Napi::ObjectWrap<S7Server> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  explicit S7Server(const Napi::CallbackInfo &info);
  ~S7Server();

  Napi::Value Start(const Napi::CallbackInfo &info);
  Napi::Value StartTo(const Napi::CallbackInfo &info);
  Napi::Value Stop(const Napi::CallbackInfo &info);
  Napi::Value SetResourceless(const Napi::CallbackInfo &info);
  Napi::Value GetParam(const Napi::CallbackInfo &info);
  Napi::Value SetParam(const Napi::CallbackInfo &info);
  Napi::Value GetEventsMask(const Napi::CallbackInfo &info);
  Napi::Value SetEventsMask(const Napi::CallbackInfo &info);
  Napi::Value RegisterArea(const Napi::CallbackInfo &info);
  Napi::Value UnregisterArea(const Napi::CallbackInfo &info);
  Napi::Value SetArea(const Napi::CallbackInfo &info);
  Napi::Value GetArea(const Napi::CallbackInfo &info);
  Napi::Value LockArea(const Napi::CallbackInfo &info);
  Napi::Value UnlockArea(const Napi::CallbackInfo &info);
  Napi::Value ServerStatus(const Napi::CallbackInfo &info);
  Napi::Value ClientsCount(const Napi::CallbackInfo &info);
  Napi::Value GetCpuStatus(const Napi::CallbackInfo &info);
  Napi::Value SetCpuStatus(const Napi::CallbackInfo &info);

  Napi::Value ErrorText(const Napi::CallbackInfo &info);
  Napi::Value EventText(const Napi::CallbackInfo &info);
  Napi::Value LastError(const Napi::CallbackInfo &info);

  static int GetByteCountFromWordLen(int WordLen);

  TS7Server *snap7Server;
  int lastError;

 private:
  Napi::ThreadSafeFunction tsfn;
};

class IOWorkerServer : public Napi::AsyncWorker {
 public:
  // No args
  IOWorkerServer(Napi::Function& callback, S7Server *s7server, ServerIOFunction caller)
    : Napi::AsyncWorker(callback), s7server(s7server), caller(caller) {}
  // 1 args
  IOWorkerServer(Napi::Function& callback, S7Server *s7server, ServerIOFunction caller
    , void *arg1)
    : Napi::AsyncWorker(callback), s7server(s7server), caller(caller)
    , pData(arg1) {}

  void Execute();
  void OnOK();
  ~IOWorkerServer() {}

 private:
  S7Server *s7server;
  ServerIOFunction caller;
  void *pData = nullptr;
  int ret = 0;
};

}  // namespace node_snap7

#endif  // SRC_NODE_SNAP7_SERVER_H_
