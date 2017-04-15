/*
 * Copyright (c) 2017, Mathias Küsel
 * MIT License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

#ifndef SRC_NODE_SNAP7_SERVER_H_
#define SRC_NODE_SNAP7_SERVER_H_

#include <snap7.h>
#include <node.h>
#include <node_buffer.h>
#include <nan.h>
#include <map>
#include <deque>

namespace node_snap7 {

enum ServerIOFunction {
  START = 1, STARTTO, STOP
};

typedef struct {
  char* pBuffer;
  word size;
}TBufferInfo;

class S7Server : public Nan::ObjectWrap {
 public:
  S7Server();
  static NAN_MODULE_INIT(Init);
  static NAN_METHOD(New);

  static NAN_METHOD(Start);
  static NAN_METHOD(StartTo);
  static NAN_METHOD(Stop);
  static NAN_METHOD(SetResourceless);
  static NAN_METHOD(GetParam);
  static NAN_METHOD(SetParam);
  static NAN_METHOD(GetEventsMask);
  static NAN_METHOD(SetEventsMask);
  static NAN_METHOD(RegisterArea);
  static NAN_METHOD(UnregisterArea);
  static NAN_METHOD(SetArea);
  static NAN_METHOD(GetArea);
  static NAN_METHOD(LockArea);
  static NAN_METHOD(UnlockArea);
  static NAN_METHOD(ServerStatus);
  static NAN_METHOD(ClientsCount);
  static NAN_METHOD(GetCpuStatus);
  static NAN_METHOD(SetCpuStatus);

  static NAN_METHOD(ErrorText);
  static NAN_METHOD(EventText);
  static NAN_METHOD(LastError);

  static int GetByteCountFromWordLen(int WordLen);

#if NODE_VERSION_AT_LEAST(0, 11, 13)
  static void HandleEvent(uv_async_t* handle);
#else
  static void HandleEvent(uv_async_t* handle, int status);
#endif

#if NODE_VERSION_AT_LEAST(0, 11, 13)
  static void HandleReadWriteEvent(uv_async_t* handle);
#else
  static void HandleReadWriteEvent(uv_async_t* handle, int status);
#endif

  static NAN_METHOD(RWBufferCallback);

  uv_mutex_t mutex;
  TS7Server *snap7Server;
  std::map<int, std::map<int, TBufferInfo> > area2buffer;
  int lastError;

 private:
  ~S7Server();
  static Nan::Persistent<v8::FunctionTemplate> constructor;
};

class IOWorkerServer : public Nan::AsyncWorker {
 public:
  // No args
  IOWorkerServer(Nan::Callback *callback, S7Server *s7server, ServerIOFunction caller)
    : Nan::AsyncWorker(callback), s7server(s7server), caller(caller) {}
  // 1 args
  IOWorkerServer(Nan::Callback *callback, S7Server *s7server, ServerIOFunction caller
    , void *arg1)
    : Nan::AsyncWorker(callback), s7server(s7server), caller(caller)
    , pData(arg1) {}

  ~IOWorkerServer() {}

 private:
  void Execute();
  void HandleOKCallback();

  S7Server *s7server;
  ServerIOFunction caller;
  void *pData;
  int ret;
};

}  // namespace node_snap7

#endif  // SRC_NODE_SNAP7_SERVER_H_
