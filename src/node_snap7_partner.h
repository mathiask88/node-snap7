/*
 * Copyright (c) 2025, Mathias Küsel
 * MIT License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

#ifndef SRC_NODE_SNAP7_PARTNER_H_
#define SRC_NODE_SNAP7_PARTNER_H_

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

class S7Partner;

enum class PartnerIOFunction {
    START,
    STARTTO,
    STOP,
    BSend,
    BRecv
};

typedef struct {
    int opResult;
    uint32_t rId;
    char* pData;
    int size;
} TPartnerRecv, *PPartnerRecv;

using Context = Napi::Reference<Napi::Value>;
using DataTypeRecv = TPartnerRecv;
void CallJsRecv(Napi::Env env, Napi::Function callback, Context* context, DataTypeRecv* data);
using TSFNRecv = Napi::TypedThreadSafeFunction<Context, DataTypeRecv, CallJsRecv>;

class S7Partner : public Napi::ObjectWrap<S7Partner> {
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    explicit S7Partner(const Napi::CallbackInfo& info);
    ~S7Partner();

    Napi::Value Start(const Napi::CallbackInfo& info);
    Napi::Value StartTo(const Napi::CallbackInfo& info);
    Napi::Value Stop(const Napi::CallbackInfo& info);
    Napi::Value GetParam(const Napi::CallbackInfo& info);
    Napi::Value SetParam(const Napi::CallbackInfo& info);
    Napi::Value BSend(const Napi::CallbackInfo& info);
    Napi::Value BRecv(const Napi::CallbackInfo& info);

    Napi::Value Status(const Napi::CallbackInfo& info);
    Napi::Value GetTimes(const Napi::CallbackInfo& info);
    Napi::Value GetStats(const Napi::CallbackInfo& info);
    static Napi::Error MakeError(Napi::Env env, const std::string& context, int code);
    Napi::Value ErrorText(const Napi::CallbackInfo& info);
    Napi::Value Linked(const Napi::CallbackInfo& info);

    TS7Partner* snap7Partner;
    std::mutex mutex;
    TSFNRecv tsfnRecv;

  private:
    static void S7API
    RecvCallback(void* usrPtr, int opResult, longword R_ID, void* pData, int Size);
};

class IOWorkerPartner : public Napi::AsyncWorker {
  public:
    // No args
    IOWorkerPartner(const Napi::Env& env, S7Partner* s7partner, PartnerIOFunction caller)
        : Napi::AsyncWorker(env, "IOWorkerPartner"), m_deferred(env), s7partner(s7partner),
          caller(caller) {}
    // 1 args
    IOWorkerPartner(const Napi::Env& env, S7Partner* s7partner, PartnerIOFunction caller, int arg1)
        : Napi::AsyncWorker(env, "IOWorkerPartner"), m_deferred(env), s7partner(s7partner),
          caller(caller), int1(arg1) {}
    // 3 args
    IOWorkerPartner(const Napi::Env& env,
                    S7Partner* s7partner,
                    PartnerIOFunction caller,
                    uint32_t arg1,
                    void* arg2,
                    int arg3)
        : Napi::AsyncWorker(env, "IOWorkerPartner"), m_deferred(env), s7partner(s7partner),
          caller(caller), uint1(arg1), pData1(arg2), int1(arg3) {}
    // 4 args
    IOWorkerPartner(const Napi::Env& env,
                    S7Partner* s7partner,
                    PartnerIOFunction caller,
                    void* arg1,
                    void* arg2,
                    int arg3,
                    int arg4)
        : Napi::AsyncWorker(env, "IOWorkerPartner"), m_deferred(env), s7partner(s7partner),
          caller(caller), pData1(arg1), pData2(arg2), int1(arg3), int2(arg4) {}

    Napi::Promise GetPromise() {
        return m_deferred.Promise();
    }

  protected:
    void Execute();
    void OnOK();
    ~IOWorkerPartner() {}

  private:
    Napi::Promise::Deferred m_deferred;
    S7Partner* s7partner;
    PartnerIOFunction caller;
    void *pData1 = nullptr, *pData2 = nullptr;
    int int1 = 0, int2 = 0, ret = 0;
    uint32_t uint1 = 0;
};

} // namespace node_snap7

#endif // SRC_NODE_SNAP7_PARTNER_H_
