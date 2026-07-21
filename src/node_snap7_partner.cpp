/*
 * Copyright (c) 2025, Mathias Küsel
 * MIT License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

#include <node_snap7_helpers.h>
#include <node_snap7_partner.h>

namespace node_snap7 {

Napi::Object S7Partner::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env,
                                      "S7Partner",
                                      {
                                          // Setup the prototype
                                          InstanceMethod("_Start", &S7Partner::Start),
                                          InstanceMethod("_StartTo", &S7Partner::StartTo),
                                          InstanceMethod("_Stop", &S7Partner::Stop),
                                          InstanceMethod("SetParam", &S7Partner::SetParam),
                                          InstanceMethod("GetParam", &S7Partner::GetParam),
                                          InstanceMethod("_BSend", &S7Partner::BSend),
                                          InstanceMethod("_BRecv", &S7Partner::BRecv),
                                          InstanceMethod("Status", &S7Partner::Status),
                                          InstanceMethod("GetTimes", &S7Partner::GetTimes),
                                          InstanceMethod("GetStats", &S7Partner::GetStats),
                                          InstanceMethod("ErrorText", &S7Partner::ErrorText),
                                          InstanceMethod("Linked", &S7Partner::Linked),
                                      });

    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);

    exports.Set("S7Partner", func);
    return exports;
}

S7Partner::S7Partner(const Napi::CallbackInfo& info) : Napi::ObjectWrap<S7Partner>(info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        ThrowArgCountError(env, 1, info.Length());
        return;
    }
    if (!info[0].IsBoolean()) {
        ThrowArgTypeError(env, info, 0, "boolean");
        return;
    }

    bool active = info[0].As<Napi::Boolean>().Value();
    snap7Partner = new TS7Partner(active);

    Napi::Function Emit = info.This().As<Napi::Object>().Get("emit").As<Napi::Function>();
    Napi::Reference<Napi::Value>* TsfnRecvCtx =
        new Napi::Reference<Napi::Value>(Napi::Persistent(info.This()));

    tsfnRecv = TSFNRecv::New(
        env,        // Environment
        Emit,       // JS function from caller
        "TsfnRecv", // Resource name
        0,          // Max queue size (0 = unlimited).
        1,          // Initial thread count
        TsfnRecvCtx,
        [](Napi::Env, void* /*finalizeData*/, Napi::Reference<Napi::Value>* ctx) { delete ctx; });

    tsfnRecv.Unref(env);

    snap7Partner->SetRecvCallback(reinterpret_cast<pfn_ParRecvCallBack>(&RecvCallback), this);
}

S7Partner::~S7Partner() {
    snap7Partner->Stop();
    snap7Partner->SetRecvCallback(nullptr, nullptr);

    delete snap7Partner;
}

Napi::Error S7Partner::MakeError(Napi::Env env, const std::string& context, int code) {
    std::string text = ParErrorText(code);
    std::ostringstream msg;
    msg << context << " (" << code << ")";
    if (!text.empty()) {
        msg << ": " << text;
    }
    Napi::Error err = Napi::Error::New(env, msg.str());
    err.Set("name", Napi::String::New(env, "Snap7Error"));
    err.Set("code", Napi::String::New(env, "SNAP7_PARTNER_CODE_" + std::to_string(code)));
    err.Set("errno", Napi::Number::New(env, code));
    return err;
}

void IOWorkerPartner::Execute() {
    std::lock_guard<std::mutex> lock(s7partner->mutex);

    switch (caller) {
    case PartnerIOFunction::STARTTO:
        ret = s7partner->snap7Partner->StartTo(static_cast<std::string*>(pData1)->c_str(),
                                               static_cast<std::string*>(pData2)->c_str(),
                                               int1,
                                               int2);
        break;

    case PartnerIOFunction::START:
        ret = s7partner->snap7Partner->Start();
        break;

    case PartnerIOFunction::STOP:
        ret = s7partner->snap7Partner->Stop();
        break;

    case PartnerIOFunction::BSend:
        ret = s7partner->snap7Partner->BSend(uint1, pData1, int1);
        break;

    case PartnerIOFunction::BRecv:
        pData1 = new char[65536]; // Allocate maximum buffer size
        ret = s7partner->snap7Partner->BRecv(&uint1, pData1, &int1, int2);
        break;
    }
}

void IOWorkerPartner::OnOK() {
    Napi::Value val = Env().Null();

    switch (caller) {
    case PartnerIOFunction::STARTTO:
        delete static_cast<std::string*>(pData1);
        delete static_cast<std::string*>(pData2);
        break;

    case PartnerIOFunction::START:
    case PartnerIOFunction::STOP:
        break;
    case PartnerIOFunction::BSend:
        if (pData1 != nullptr) {
            delete[] static_cast<char*>(pData1);
            pData1 = nullptr;
        }
        break;
    case PartnerIOFunction::BRecv:
        if (ret == 0) {
            Napi::Buffer<char> buffer =
                Napi::Buffer<char>::Copy(Env(), static_cast<char*>(pData1), int1);
            delete[] static_cast<char*>(pData1);

            Napi::Object res_obj = Napi::Object::New(Env());
            res_obj.Set("R_ID", uint1);
            res_obj.Set("data", buffer);

            val = res_obj;
        } else if (pData1 != nullptr) {
            delete[] static_cast<char*>(pData1);
        }
        break;
    }

    if (ret == 0) {
        m_deferred.Resolve(val);
        return;
    }

    Napi::Error err = S7Partner::MakeError(Env(), "Snap7 Partner operation failed", ret);
    m_deferred.Reject(err.Value());
}

Napi::Value S7Partner::Start(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    bool isAsyncCall = info.Length() > 0 && info[info.Length() - 1].IsFunction();

    if (!isAsyncCall) {
        std::lock_guard<std::mutex> lock(mutex);
        int ret = snap7Partner->Start();
        if (ret != 0) {
            MakeError(env, "Start failed", ret).ThrowAsJavaScriptException();
        }
        return env.Undefined();
    }

    IOWorkerPartner* worker = new IOWorkerPartner(env, this, PartnerIOFunction::START);
    worker->Queue();

    return worker->GetPromise();
}

Napi::Value S7Partner::StartTo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    REQUIRE_MIN_ARGS(env, info, 4);
    REQUIRE_ARG(env, info, 0, IsString);
    REQUIRE_ARG(env, info, 1, IsString);
    REQUIRE_ARG(env, info, 2, IsNumber);
    REQUIRE_ARG(env, info, 3, IsNumber);

    bool isAsyncCall = info.Length() > 4 && info[info.Length() - 1].IsFunction();

    if (!isAsyncCall) {
        std::string localAddress = info[0].As<Napi::String>().Utf8Value();
        std::string remoteAddress = info[1].As<Napi::String>().Utf8Value();
        int localTSAP = info[2].As<Napi::Number>().Int32Value();
        int remoteTSAP = info[3].As<Napi::Number>().Int32Value();

        std::lock_guard<std::mutex> lock(mutex);
        int ret = snap7Partner->StartTo(localAddress.c_str(),
                                        remoteAddress.c_str(),
                                        localTSAP,
                                        remoteTSAP);
        if (ret != 0) {
            MakeError(env, "StartTo failed", ret).ThrowAsJavaScriptException();
        }
        return env.Undefined();
    }

    std::string* localAddress = new std::string(info[0].As<Napi::String>().Utf8Value());
    std::string* remoteAddress = new std::string(info[1].As<Napi::String>().Utf8Value());
    int localTSAP = info[2].As<Napi::Number>().Int32Value();
    int remoteTSAP = info[3].As<Napi::Number>().Int32Value();

    IOWorkerPartner* worker = new IOWorkerPartner(env,
                                                  this,
                                                  PartnerIOFunction::STARTTO,
                                                  localAddress,
                                                  remoteAddress,
                                                  localTSAP,
                                                  remoteTSAP);
    worker->Queue();

    return worker->GetPromise();
}

Napi::Value S7Partner::Stop(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    bool isAsyncCall = info.Length() > 0 && info[info.Length() - 1].IsFunction();

    if (!isAsyncCall) {
        std::lock_guard<std::mutex> lock(mutex);
        int ret = snap7Partner->Stop();
        if (ret != 0) {
            MakeError(env, "Stop failed", ret).ThrowAsJavaScriptException();
        }
        return env.Undefined();
    }

    IOWorkerPartner* worker = new IOWorkerPartner(env, this, PartnerIOFunction::STOP);
    worker->Queue();

    return worker->GetPromise();
}

Napi::Value S7Partner::GetParam(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    REQUIRE_MIN_ARGS(env, info, 1);
    REQUIRE_ARG(env, info, 0, IsNumber);

    int pData;
    int ret = snap7Partner->GetParam(info[0].As<Napi::Number>().Int32Value(), &pData);

    if (ret == 0) {
        return Napi::Number::New(env, pData);
    }

    MakeError(env, "GetParam failed", ret).ThrowAsJavaScriptException();
    return env.Undefined();
}

Napi::Value S7Partner::SetParam(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    REQUIRE_MIN_ARGS(env, info, 2);
    REQUIRE_ARG(env, info, 0, IsNumber);
    REQUIRE_ARG(env, info, 1, IsNumber);

    int pData = info[1].As<Napi::Number>().Int32Value();
    int ret = snap7Partner->SetParam(info[0].As<Napi::Number>().Int32Value(), &pData);

    if (ret == 0) {
        return env.Undefined();
    }

    MakeError(env, "SetParam failed", ret).ThrowAsJavaScriptException();
    return env.Undefined();
}

Napi::Value S7Partner::BSend(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    REQUIRE_MIN_ARGS(env, info, 2);
    REQUIRE_ARG(env, info, 0, IsNumber);
    REQUIRE_ARG(env, info, 1, IsBuffer);

    uint32_t r_id = info[0].As<Napi::Number>().Uint32Value();
    Napi::Buffer<char> buffer = info[1].As<Napi::Buffer<char>>();

    bool isAsyncCall = info.Length() > 2 && info[info.Length() - 1].IsFunction();

    if (!isAsyncCall) {
        std::lock_guard<std::mutex> lock(mutex);
        int ret = snap7Partner->BSend(r_id, buffer.Data(), buffer.Length());
        if (ret != 0) {
            MakeError(env, "BSend failed", ret).ThrowAsJavaScriptException();
        }
        return env.Undefined();
    }

    // Copy data to heap since async worker may outlive the JS Buffer lifetime
    char* dataCopy = new char[buffer.Length()];
    memcpy(dataCopy, buffer.Data(), buffer.Length());

    IOWorkerPartner* worker =
        new IOWorkerPartner(env, this, PartnerIOFunction::BSend, r_id, dataCopy, buffer.Length());
    worker->Queue();

    return worker->GetPromise();
}

Napi::Value S7Partner::BRecv(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    REQUIRE_MIN_ARGS(env, info, 1);
    REQUIRE_ARG(env, info, 0, IsNumber);

    int timeout = info[0].As<Napi::Number>().Int32Value();

    bool isAsyncCall = info.Length() > 1 && info[info.Length() - 1].IsFunction();

    if (!isAsyncCall) {
        uint32_t r_id = 0;
        int size = 65536; // maximum
        char* buffer = new char[size];

        std::lock_guard<std::mutex> lock(mutex);
        int ret = snap7Partner->BRecv(&r_id, buffer, &size, timeout);
        if (ret == 0) {
            Napi::Object res_obj = Napi::Object::New(env);
            res_obj.Set("R_ID", Napi::Number::New(env, r_id));
            res_obj.Set("data",
                        Napi::Buffer<char>::New(env, buffer, size, [](Napi::Env, char* buf) {
                            delete[] buf;
                        }));
            return res_obj;
        }

        delete[] buffer;
        MakeError(env, "BRecv failed", ret).ThrowAsJavaScriptException();
        return env.Undefined();
    }

    IOWorkerPartner* worker =
        new IOWorkerPartner(env, this, PartnerIOFunction::BRecv, nullptr, nullptr, 0, timeout);
    worker->Queue();

    return worker->GetPromise();
}

void CallJsRecv(Napi::Env env, Napi::Function callback, Context* context, DataTypeRecv* data) {
    Napi::Object recvObj = Napi::Object::New(env);
    recvObj.Set("result", Napi::Number::New(env, data->opResult));
    recvObj.Set("R_ID", Napi::Number::New(env, data->rId));
    recvObj.Set("data",
                Napi::Buffer<char>::New(env, data->pData, data->size, [](Napi::Env, char* buf) {
                    delete[] buf;
                }));

    Napi::Value receiver = context->Value();
    callback.Call(receiver.As<Napi::Object>(), {Napi::String::New(env, "recv"), recvObj});

    delete data;
}

void S7API
S7Partner::RecvCallback(void* usrPtr, int opResult, longword R_ID, void* pData, int Size) {
    S7Partner* partner = static_cast<S7Partner*>(usrPtr);

    // Copy payload to heap so JS can safely consume it.
    char* dataCopy = new char[Size];
    memcpy(dataCopy, pData, Size);

    auto* recvData = new TPartnerRecv{opResult, static_cast<uint32_t>(R_ID), dataCopy, Size};

    napi_status status = partner->tsfnRecv.BlockingCall(recvData);

    if (status != napi_ok) {
        delete[] dataCopy;
        delete recvData;
    }
}

Napi::Value S7Partner::Status(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    int ret = snap7Partner->Status();

    // Snap7 returns errLibInvalidObject on failure; otherwise ret is the status.
    if (ret == errLibInvalidObject) {
        MakeError(env, "Status failed", ret).ThrowAsJavaScriptException();
        return env.Undefined();
    }

    return Napi::Number::New(env, ret);
}

Napi::Value S7Partner::GetTimes(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    uint32_t SendTime, RecvTime;
    int ret = snap7Partner->GetTimes(&SendTime, &RecvTime);

    if (ret != 0) {
        MakeError(env, "GetTimes failed", ret).ThrowAsJavaScriptException();
    }

    Napi::Object res_obj = Napi::Object::New(env);
    res_obj.Set("SendTime", Napi::Number::New(env, SendTime));
    res_obj.Set("RecvTime", Napi::Number::New(env, RecvTime));

    return res_obj;
}

Napi::Value S7Partner::GetStats(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    uint32_t BytesSent, BytesRecv, ErrSend, ErrRecv;
    int ret = snap7Partner->GetStats(&BytesSent, &BytesRecv, &ErrSend, &ErrRecv);

    if (ret != 0) {
        MakeError(env, "GetStats failed", ret).ThrowAsJavaScriptException();
    }

    Napi::Object res_obj = Napi::Object::New(env);
    res_obj.Set("BytesSent", Napi::Number::New(env, BytesSent));
    res_obj.Set("BytesRecv", Napi::Number::New(env, BytesRecv));
    res_obj.Set("ErrSend", Napi::Number::New(env, ErrSend));
    res_obj.Set("ErrRecv", Napi::Number::New(env, ErrRecv));

    return res_obj;
}

Napi::Value S7Partner::Linked(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    return Napi::Boolean::New(env, snap7Partner->Linked());
}

Napi::Value S7Partner::ErrorText(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    REQUIRE_MIN_ARGS(env, info, 1);
    REQUIRE_ARG(env, info, 0, IsNumber);

    return Napi::String::New(env, ParErrorText(info[0].As<Napi::Number>().Int32Value()).c_str());
}

} // namespace node_snap7
