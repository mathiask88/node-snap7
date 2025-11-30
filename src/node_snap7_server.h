/*
 * Copyright (c) 2025, Mathias Küsel
 * MIT License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

#ifndef SRC_NODE_SNAP7_SERVER_H_
#define SRC_NODE_SNAP7_SERVER_H_

#include <napi.h>
#include <snap7.h>

#ifdef OS_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#if defined(PLATFORM_UNIX) || defined(OS_OSX)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#include <condition_variable>
#include <map>
#include <mutex>

namespace node_snap7 {

class S7Server;

class Semaphore {
  public:
    explicit Semaphore(int count = 1) : count(count) {}

    void acquire() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]() { return count > 0; });
        --count;
    }

    void release() {
        std::unique_lock<std::mutex> lock(mtx);
        ++count;
        cv.notify_one();
    }

  private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;
};

enum class ServerIOFunction {
    START,
    STARTTO,
    STOP
};

typedef struct {
    char* pBuffer;
    word size;
} TBufferInfo;

typedef struct {
    int Sender;
    int Operation;
    PS7Tag PTag;
    void* pUsrData;
    S7Server* s7server;
} TRWEvent, *PRWEvent;

using Context = Napi::Reference<Napi::Value>;
using DataTypeEvent = TSrvEvent;
using DataTypeRW = TRWEvent;
void CallJsEvent(Napi::Env env, Napi::Function callback, Context* context, DataTypeEvent* data);
void CallJsRW(Napi::Env env, Napi::Function callback, Context* context, DataTypeRW* data);
using TSFN = Napi::TypedThreadSafeFunction<Context, DataTypeEvent, CallJsEvent>;
using TSFNRW = Napi::TypedThreadSafeFunction<Context, DataTypeRW, CallJsRW>;

class S7Server : public Napi::ObjectWrap<S7Server> {
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    explicit S7Server(const Napi::CallbackInfo& info);
    ~S7Server();

    Napi::Value Start(const Napi::CallbackInfo& info);
    Napi::Value StartTo(const Napi::CallbackInfo& info);
    Napi::Value Stop(const Napi::CallbackInfo& info);
    Napi::Value SetResourceless(const Napi::CallbackInfo& info);
    Napi::Value GetParam(const Napi::CallbackInfo& info);
    Napi::Value SetParam(const Napi::CallbackInfo& info);
    Napi::Value GetEventsMask(const Napi::CallbackInfo& info);
    Napi::Value SetEventsMask(const Napi::CallbackInfo& info);
    Napi::Value RegisterArea(const Napi::CallbackInfo& info);
    Napi::Value UnregisterArea(const Napi::CallbackInfo& info);
    Napi::Value SetArea(const Napi::CallbackInfo& info);
    Napi::Value GetArea(const Napi::CallbackInfo& info);
    Napi::Value LockArea(const Napi::CallbackInfo& info);
    Napi::Value UnlockArea(const Napi::CallbackInfo& info);
    Napi::Value ServerStatus(const Napi::CallbackInfo& info);
    Napi::Value ClientsCount(const Napi::CallbackInfo& info);
    Napi::Value GetCpuStatus(const Napi::CallbackInfo& info);
    Napi::Value SetCpuStatus(const Napi::CallbackInfo& info);
    static Napi::Error MakeError(Napi::Env env, const std::string& context, int code);

    Napi::Value ErrorText(const Napi::CallbackInfo& info);
    Napi::Value EventText(const Napi::CallbackInfo& info);

    static int GetByteCountFromWordLen(int WordLen);

    TS7Server* snap7Server;
    std::mutex mutex;
    std::map<int, std::map<int, TBufferInfo>> area2buffer;
    Semaphore sem_rw{0};

  private:
    static void S7API EventCallBack(void* usrPtr, PSrvEvent PEvent, int Size);
    static int S7API
    RWAreaCallBack(void* usrPtr, int Sender, int Operation, PS7Tag PTag, void* pUsrData);
    TSFN tsfn;
    TSFNRW tsfnrw;
};

class IOWorkerServer : public Napi::AsyncWorker {
  public:
    // No args
    IOWorkerServer(const Napi::Env& env, S7Server* s7server, ServerIOFunction caller)
        : Napi::AsyncWorker(env, "IOWorkerServer"), m_deferred(env), s7server(s7server),
          caller(caller) {}
    // 1 args
    IOWorkerServer(const Napi::Env& env, S7Server* s7server, ServerIOFunction caller, void* arg1)
        : Napi::AsyncWorker(env, "IOWorkerServer"), m_deferred(env), s7server(s7server),
          caller(caller), pData(arg1) {}

    Napi::Promise GetPromise() {
        return m_deferred.Promise();
    }

  protected:
    void Execute();
    void OnOK();
    ~IOWorkerServer() {}

  private:
    Napi::Promise::Deferred m_deferred;
    S7Server* s7server;
    ServerIOFunction caller;
    void* pData = nullptr;
    int returnValue = 0;
};

} // namespace node_snap7

#endif // SRC_NODE_SNAP7_SERVER_H_
