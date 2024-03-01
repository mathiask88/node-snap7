/*
 * Copyright (c) 2019, Mathias Küsel
 * MIT License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

#include <node_snap7_server.h>

namespace node_snap7 {

static std::mutex mutex_rw;
static std::mutex mutex_event;

static struct rw_event_baton_t {
  int Sender;
  int Operation;
  TS7Tag Tag;
  void *pUsrData;
} rw_event_baton_g;

void S7API EventCallBack(void *usrPtr, PSrvEvent PEvent, int Size) {

}

int S7API RWAreaCallBack(void *usrPtr, int Sender, int Operation, PS7Tag PTag
  , void *pUsrData
) {

  return 0;
}

Napi::Object S7Server::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "S7Server", {

  // Setup the prototype
  InstanceMethod("Start", &S7Server::Start),
  InstanceMethod("StartTo", &S7Server::StartTo),
  InstanceMethod("Stop", &S7Server::Stop),
  InstanceMethod("SetParam", &S7Server::SetParam),
  InstanceMethod("GetParam", &S7Server::GetParam),
  InstanceMethod("SetResourceless", &S7Server::SetResourceless),
  InstanceMethod("RegisterArea", &S7Server::RegisterArea),
  InstanceMethod("UnregisterArea", &S7Server::UnregisterArea),
  InstanceMethod("LockArea", &S7Server::LockArea),
  InstanceMethod("UnlockArea", &S7Server::UnlockArea),
  InstanceMethod("SetArea", &S7Server::SetArea),
  InstanceMethod("GetArea", &S7Server::GetArea),
  InstanceMethod("SetEventMask", &S7Server::SetEventsMask),
  InstanceMethod("GetEventsMask", &S7Server::GetEventsMask),
  InstanceMethod("ErrorText", &S7Server::ErrorText),
  InstanceMethod("LastError", &S7Server::LastError),
  InstanceMethod("EventText", &S7Server::EventText),
  InstanceMethod("ServerStatus", &S7Server::ServerStatus),
  InstanceMethod("ClientsCount", &S7Server::ClientsCount),
  InstanceMethod("GetCpuStatus", &S7Server::GetCpuStatus),
  InstanceMethod("SetCpuStatus", &S7Server::SetCpuStatus),

  // Error codes
  InstanceValue("errSrvCannotStart"
    , Napi::Value::From(env, errSrvCannotStart)),
  InstanceValue("errSrvDBNullPointer"
    , Napi::Value::From(env, errSrvDBNullPointer)),
  InstanceValue("errSrvAreaAlreadyExists"
    , Napi::Value::From(env, errSrvAreaAlreadyExists)),
  InstanceValue("errSrvUnknownArea"
    , Napi::Value::From(env, errSrvUnknownArea)),
  InstanceValue("errSrvInvalidParams"
    , Napi::Value::From(env, errSrvInvalidParams)),
  InstanceValue("errSrvTooManyDB"
    , Napi::Value::From(env, errSrvTooManyDB)),
  InstanceValue("errSrvInvalidParamNumber"
    , Napi::Value::From(env, errSrvInvalidParamNumber)),
  InstanceValue("errSrvCannotChangeParam"
    , Napi::Value::From(env, errSrvCannotChangeParam)),

  // Server area IDs
  InstanceValue("srvAreaPE"
    , Napi::Value::From(env, srvAreaPE)),
  InstanceValue("srvAreaPA"
    , Napi::Value::From(env, srvAreaPA)),
  InstanceValue("srvAreaMK"
    , Napi::Value::From(env, srvAreaMK)),
  InstanceValue("srvAreaCT"
    , Napi::Value::From(env, srvAreaCT)),
  InstanceValue("srvAreaTM"
    , Napi::Value::From(env, srvAreaTM)),
  InstanceValue("srvAreaDB"
    , Napi::Value::From(env, srvAreaDB)),
  InstanceValue("operationWrite"
    , Napi::Value::From(env, OperationWrite)),
  InstanceValue("operationRead"
    , Napi::Value::From(env, OperationRead)),
  // TCP server event codes
  InstanceValue("evcServerStarted"
    , Napi::Value::From(env, evcServerStarted)),
  InstanceValue("evcServerStopped"
    , Napi::Value::From(env, evcServerStopped)),
  InstanceValue("evcListenerCannotStart"
    , Napi::Value::From(env, evcListenerCannotStart)),
  InstanceValue("evcClientAdded"
    , Napi::Value::From(env, evcClientAdded)),
  InstanceValue("evcClientRejected"
    , Napi::Value::From(env, evcClientRejected)),
  InstanceValue("evcClientNoRoom"
    , Napi::Value::From(env, evcClientNoRoom)),
  InstanceValue("evcClientException"
    , Napi::Value::From(env, evcClientException)),
  InstanceValue("evcClientDisconnected"
    , Napi::Value::From(env, evcClientDisconnected)),
  InstanceValue("evcClientTerminated"
    , Napi::Value::From(env, evcClientTerminated)),
  InstanceValue("evcClientsDropped"
    , Napi::Value::From(env, evcClientsDropped)),

  // S7 server event codes
  InstanceValue("evcPDUincoming"
    , Napi::Value::From(env, evcPDUincoming)),
  InstanceValue("evcDataRead"
    , Napi::Value::From(env, evcDataRead)),
  InstanceValue("evcDataWrite"
    , Napi::Value::From(env, evcDataWrite)),
  InstanceValue("evcNegotiatePDU"
    , Napi::Value::From(env, evcNegotiatePDU)),
  InstanceValue("evcReadSZL"
    , Napi::Value::From(env, evcReadSZL)),
  InstanceValue("evcClock"
    , Napi::Value::From(env, evcClock)),
  InstanceValue("evcUpload"
    , Napi::Value::From(env, evcUpload)),
  InstanceValue("evcDownload"
    , Napi::Value::From(env, evcDownload)),
  InstanceValue("evcDirectory"
    , Napi::Value::From(env, evcDirectory)),
  InstanceValue("evcSecurity"
    , Napi::Value::From(env, evcSecurity)),
  InstanceValue("evcControl"
    , Napi::Value::From(env, evcControl)),

  // Masks to enable/disable all events
  InstanceValue("evcAll"
    , Napi::Value::From(env, evcAll)),
  InstanceValue("evcNone"
    , Napi::Value::From(env, evcNone)),

  // Event subcodes
  InstanceValue("evsUnknown"
    , Napi::Value::From(env, evsUnknown)),
  InstanceValue("evsStartUpload"
    , Napi::Value::From(env, evsStartUpload)),
  InstanceValue("evsStartDownload"
    , Napi::Value::From(env, evsStartDownload)),
  InstanceValue("evsGetBlockList"
    , Napi::Value::From(env, evsGetBlockList)),
  InstanceValue("evsStartListBoT"
    , Napi::Value::From(env, evsStartListBoT)),
  InstanceValue("evsListBoT"
    , Napi::Value::From(env, evsListBoT)),
  InstanceValue("evsGetBlockInfo"
    , Napi::Value::From(env, evsGetBlockInfo)),
  InstanceValue("evsGetClock"
    , Napi::Value::From(env, evsGetClock)),
  InstanceValue("evsSetClock"
    , Napi::Value::From(env, evsSetClock)),
  InstanceValue("evsSetPassword"
    , Napi::Value::From(env, evsSetPassword)),
  InstanceValue("evsClrPassword"
    , Napi::Value::From(env, evsClrPassword)),

  // Event params : functions group
  InstanceValue("grProgrammer"
    , Napi::Value::From(env, grProgrammer)),
  InstanceValue("grCyclicData"
    , Napi::Value::From(env, grCyclicData)),
  InstanceValue("grBlocksInfo"
    , Napi::Value::From(env, grBlocksInfo)),
  InstanceValue("grSZL"
    , Napi::Value::From(env, grSZL)),
  InstanceValue("grPassword"
    , Napi::Value::From(env, grPassword)),
  InstanceValue("grBSend"
    , Napi::Value::From(env, grBSend)),
  InstanceValue("grClock"
    , Napi::Value::From(env, grClock)),
  InstanceValue("grSecurity"
    , Napi::Value::From(env, grSecurity)),

  // Event params : control codes
  InstanceValue("CodeControlUnknown"
    , Napi::Value::From(env, CodeControlUnknown)),
  InstanceValue("CodeControlColdStart"
    , Napi::Value::From(env, CodeControlColdStart)),
  InstanceValue("CodeControlWarmStart"
    , Napi::Value::From(env, CodeControlWarmStart)),
  InstanceValue("CodeControlStop"
    , Napi::Value::From(env, CodeControlStop)),
  InstanceValue("CodeControlCompress"
    , Napi::Value::From(env, CodeControlCompress)),
  InstanceValue("CodeControlCpyRamRom"
    , Napi::Value::From(env, CodeControlCpyRamRom)),
  InstanceValue("CodeControlInsDel"
    , Napi::Value::From(env, CodeControlInsDel)),

  // Event results
  InstanceValue("evrNoError"
    , Napi::Value::From(env, evrNoError)),
  InstanceValue("evrFragmentRejected"
    , Napi::Value::From(env, evrFragmentRejected)),
  InstanceValue("evrMalformedPDU"
    , Napi::Value::From(env, evrMalformedPDU)),
  InstanceValue("evrSparseBytes"
    , Napi::Value::From(env, evrSparseBytes)),
  InstanceValue("evrCannotHandlePDU"
    , Napi::Value::From(env, evrCannotHandlePDU)),
  InstanceValue("evrNotImplemented"
    , Napi::Value::From(env, evrNotImplemented)),
  InstanceValue("evrErrException"
    , Napi::Value::From(env, evrErrException)),
  InstanceValue("evrErrAreaNotFound"
    , Napi::Value::From(env, evrErrAreaNotFound)),
  InstanceValue("evrErrOutOfRange"
    , Napi::Value::From(env, evrErrOutOfRange)),
  InstanceValue("evrErrOverPDU"
    , Napi::Value::From(env, evrErrOverPDU)),
  InstanceValue("evrErrTransportSize"
    , Napi::Value::From(env, evrErrTransportSize)),
  InstanceValue("evrInvalidGroupUData"
    , Napi::Value::From(env, evrInvalidGroupUData)),
  InstanceValue("evrInvalidSZL"
    , Napi::Value::From(env, evrInvalidSZL)),
  InstanceValue("evrDataSizeMismatch"
    , Napi::Value::From(env, evrDataSizeMismatch)),
  InstanceValue("evrCannotUpload"
    , Napi::Value::From(env, evrCannotUpload)),
  InstanceValue("evrCannotDownload"
    , Napi::Value::From(env, evrCannotDownload)),
  InstanceValue("evrUploadInvalidID"
    , Napi::Value::From(env, evrUploadInvalidID)),
  InstanceValue("evrResNotFound"
    , Napi::Value::From(env, evrResNotFound)),

  // Server parameter
  InstanceValue("LocalPort"
    , Napi::Value::From(env, p_u16_LocalPort)),
  InstanceValue("WorkInterval"
    , Napi::Value::From(env, p_i32_WorkInterval)),
  InstanceValue("PDURequest"
    , Napi::Value::From(env, p_i32_PDURequest)),
  InstanceValue("MaxClients"
    , Napi::Value::From(env, p_i32_MaxClients)),

  // CPU status codes
  InstanceValue("S7CpuStatusUnknown"
    , Napi::Value::From(env, S7CpuStatusUnknown)),
  InstanceValue("S7CpuStatusRun"
    , Napi::Value::From(env, S7CpuStatusRun)),
  InstanceValue("S7CpuStatusStop"
    , Napi::Value::From(env, S7CpuStatusStop)),

  // Server status codes
  InstanceValue("SrvStopped"
    , Napi::Value::From(env, 0)),
  InstanceValue("SrvRunning"
    , Napi::Value::From(env, 1)),
  InstanceValue("SrvError"
    , Napi::Value::From(env, 2))
  });

  Napi::FunctionReference* constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

  exports.Set("S7Server", func);
  return exports;
}

S7Server::S7Server(const Napi::CallbackInfo &info) : Napi::ObjectWrap<S7Server>(info) {
  lastError = 0;
  snap7Server = new TS7Server();

  snap7Server->SetEventsCallback(&EventCallBack, NULL);
}

S7Server::~S7Server() {
  snap7Server->Stop();
  delete snap7Server;
}

int S7Server::GetByteCountFromWordLen(int WordLen) {
  switch (WordLen) {
  case S7WLBit:
  case S7WLByte:
    return 1;
  case S7WLWord:
  case S7WLCounter:
  case S7WLTimer:
    return 2;
  case S7WLReal:
  case S7WLDWord:
    return 4;
  default:
    return 0;
  }
}

void IOWorkerServer::Execute() {
  s7server->mutex.lock();

  switch (caller) {
  case ServerIOFunction::STARTTO:
    returnValue = s7server->snap7Server->StartTo(
      static_cast<std::string*>(pData)->c_str());
    break;

  case ServerIOFunction::START:
    returnValue = s7server->snap7Server->Start();
    break;

  case ServerIOFunction::STOP:
    returnValue = s7server->snap7Server->Stop();
    break;
  }

  s7server->mutex.unlock();
}

void IOWorkerServer::OnOK() {
  switch (caller) {
  case ServerIOFunction::STARTTO:
      delete static_cast<std::string*>(pData);
    break;

  case ServerIOFunction::START:
  case ServerIOFunction::STOP:
    break;
  }

  if (returnValue == 0){
    m_deferred.Resolve(Env().Null());
  } else {
    m_deferred.Reject(Napi::Number::New(Env(), returnValue));
  }
}

Napi::Value S7Server::Start(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  IOWorkerServer* worker = new IOWorkerServer(env, this, ServerIOFunction::START);
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Server::StartTo(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  std::string* address = new std::string(info[0].As<Napi::String>().Utf8Value());
  IOWorkerServer* worker = new IOWorkerServer(env, this, ServerIOFunction::STARTTO
    , address);
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Server::Stop(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  IOWorkerServer* worker = new IOWorkerServer(env, this, ServerIOFunction::STOP);
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Server::SetResourceless(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!info[0].IsBoolean()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  bool resourceless = info[0].ToBoolean().Value();

  int ret;
  if (resourceless) {
    ret = snap7Server->SetRWAreaCallback(&RWAreaCallBack, NULL);
  } else {
    ret = snap7Server->SetRWAreaCallback(NULL, NULL);
  }
  lastError = ret;

  return Napi::Boolean::New(env, ret == 0);
}

Napi::Value S7Server::GetParam(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  int pData;
  int ret = snap7Server->GetParam(info[0].ToNumber().Int32Value()
    , &pData);
  lastError = ret;

  return Napi::Boolean::New(env, ret == 0);
}

Napi::Value S7Server::SetParam(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!(info[0].IsNumber() || info[1].IsNumber())) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  int pData = info[1].ToNumber().Int32Value();
  int ret = snap7Server->SetParam(info[0].ToNumber().Int32Value(), &pData);
  lastError = ret;

  return Napi::Boolean::New(env, ret == 0);
}

Napi::Value S7Server::GetEventsMask(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  int ret = snap7Server->GetEventsMask();

  return Napi::Number::New(env, ret);
}

Napi::Value S7Server::SetEventsMask(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  snap7Server->SetEventsMask(info[0].ToNumber().Uint32Value());

  return env.Undefined();
}

Napi::Value S7Server::RegisterArea(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  int index;
  char *pBuffer;
  size_t len;
  int area = info[0].ToNumber().Int32Value();

  if (area == srvAreaDB) {
    if (!info[1].IsNumber() || !info[2].IsBuffer()) {
      Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    }

    index = info[1].ToNumber().Int32Value();
    len = info[2].As<Napi::Buffer<char>>().Length();
    pBuffer = info[2].As<Napi::Buffer<char>>().Data();
  } else if (!info[1].IsBuffer()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  } else {
    index = 0;
    len = info[1].As<Napi::Buffer<char>>().Length();
    pBuffer = info[1].As<Napi::Buffer<char>>().Data();
  }

  if (len > 0xFFFF) {
    Napi::RangeError::New(env, "Max area buffer size is 65535").ThrowAsJavaScriptException();
  }

  word size = static_cast<word>(len);
  char *data = new char[size];
  memcpy(data, pBuffer, size);

  int ret = snap7Server->RegisterArea(area, index, data, size);
  lastError = ret;

  if (ret == 0) {
    area2buffer[area][index].pBuffer = data;
    area2buffer[area][index].size = size;
  } else {
    delete[] data;
  }

  return Napi::Boolean::New(env, ret == 0);
}

Napi::Value S7Server::UnregisterArea(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  int index = 0;
  int area = info[0].ToNumber().Int32Value();

  if (area == srvAreaDB) {
    if (!info[1].IsNumber()) {
      Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    }

    index = info[1].ToNumber().Int32Value();
  }

  int ret = snap7Server->UnregisterArea(area, index);
  lastError = ret;

  if (ret == 0) {
    delete[] area2buffer[area][index].pBuffer;
    area2buffer[area].erase(index);
  }

  return Napi::Boolean::New(env, ret == 0);
}

Napi::Value S7Server::SetArea(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  int area = info[0].ToNumber().Int32Value();
  if (!area2buffer.count(area)) {
    Napi::Error::New(env, "Unknown area").ThrowAsJavaScriptException();
  }

  int index;
  char *pBuffer;
  size_t len;

  if (area == srvAreaDB) {
    if (!info[1].IsNumber() || !info[2].IsBuffer()) {
      Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    }

    index = info[1].ToNumber().Int32Value();
    if (!area2buffer[area].count(index)) {
      Napi::Error::New(env, "DB index not found").ThrowAsJavaScriptException();
    }

    len =info[2].As<Napi::Buffer<char>>().Length();;
    pBuffer = info[2].As<Napi::Buffer<char>>().Data();
  } else {
    index = 0;
    if (info[1].IsBuffer()) {
      len = info[1].As<Napi::Buffer<char>>().Length();
      pBuffer = info[1].As<Napi::Buffer<char>>().Data();
    } else if (info[2].IsBuffer()) {
      len = info[2].As<Napi::Buffer<char>>().Length();
      pBuffer = info[2].As<Napi::Buffer<char>>().Data();
    } else {
      Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    }
  }

  if (len != area2buffer[area][index].size) {
    Napi::Error::New(env, "Wrong buffer length").ThrowAsJavaScriptException();
  }

  snap7Server->LockArea(area, index);

  memcpy(
    area2buffer[area][index].pBuffer
    , pBuffer, area2buffer[area][index].size);

  snap7Server->UnlockArea(area, index);

  return env.Undefined();
}

Napi::Value S7Server::GetArea(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  int index = 0;
  int area = info[0].ToNumber().Int32Value();

  if (!area2buffer.count(area)) {
    Napi::Error::New(env, "Unknown area").ThrowAsJavaScriptException();
  }

  if (area == srvAreaDB) {
    if (!info[1].IsNumber()) {
      Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    }

    index = info[1].ToNumber().Int32Value();
    if (!area2buffer[area].count(index)) {
      Napi::Error::New(env, "DB index not found").ThrowAsJavaScriptException();
    }
  }

  snap7Server->LockArea(area, index);

  Napi::Buffer<char> buffer = Napi::Buffer<char>::Copy(env,
      area2buffer[area][index].pBuffer
    , area2buffer[area][index].size);

  snap7Server->UnlockArea(area, index);

  return buffer;
}

Napi::Value S7Server::LockArea(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  int index = 0;
  int area = info[0].ToNumber().Int32Value();

  if (area == srvAreaDB) {
    if (!info[1].IsNumber()) {
      Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    }

    index = info[1].ToNumber().Int32Value();
  }

  int ret = snap7Server->LockArea(area, index);
  lastError = ret;

  return Napi::Boolean::New(env, ret == 0);
}

Napi::Value S7Server::UnlockArea(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  int index = 0;
  int area = info[0].ToNumber().Int32Value();

  if (area == srvAreaDB) {
    if (!info[1].IsNumber()) {
      Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    }

    index = info[1].ToNumber().Int32Value();
  }

  int ret = snap7Server->UnlockArea(area, index);
  lastError = ret;

  return Napi::Boolean::New(env, ret == 0);
}

Napi::Value S7Server::ServerStatus(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  int ret = snap7Server->ServerStatus();
  if ((ret == 0) || (ret == 1) || (ret == 2)) {
    lastError = 0;
    return Napi::Number::New(env, ret);
  } else {
    lastError = ret;
    return Napi::Boolean::New(env, false);
  }
}

Napi::Value S7Server::ClientsCount(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  int ret = snap7Server->ClientsCount();
  return Napi::Number::New(env, ret);
}

Napi::Value S7Server::GetCpuStatus(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  int ret = snap7Server->GetCpuStatus();
  if ((ret == S7CpuStatusUnknown) ||
    (ret == S7CpuStatusStop) ||
    (ret == S7CpuStatusRun)) {
    lastError = 0;
    return Napi::Number::New(env, ret);
  } else {
    lastError = ret;
    return Napi::Boolean::New(env, false);
  }
}

Napi::Value S7Server::SetCpuStatus(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  int ret = snap7Server->SetCpuStatus(info[0].ToNumber().Int32Value());
  lastError = ret;

  return Napi::Boolean::New(env, ret == 0);
}

Napi::Value S7Server::ErrorText(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  return Napi::String::New(env,
    SrvErrorText(info[0].ToNumber().Int32Value()).c_str());
}

Napi::Value S7Server::EventText(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  TSrvEvent SrvEvent;

  if (!info[0].IsObject()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  Napi::Object event_obj = info[0].ToObject();
  if (!event_obj.Has("EvtTime") || !event_obj.Has("EvtSender")  ||
    !event_obj.Has("EvtCode")   || !event_obj.Has("EvtRetCode") ||
    !event_obj.Has("EvtParam1") || !event_obj.Has("EvtParam2")  ||
    !event_obj.Has("EvtParam3") || !event_obj.Has("EvtParam4")) {
    Napi::TypeError::New(env, "Wrong argument structure").ThrowAsJavaScriptException();
  }

  if (!event_obj.Get("EvtTime").IsDate() ||
    !event_obj.Get("EvtSender").IsString() ||
    !event_obj.Get("EvtCode").IsNumber() ||
    !event_obj.Get("EvtRetCode").IsNumber() ||
    !event_obj.Get("EvtParam1").IsNumber() ||
    !event_obj.Get("EvtParam2").IsNumber() ||
    !event_obj.Get("EvtParam3").IsNumber() ||
    !event_obj.Get("EvtParam4").IsNumber()) {
    Napi::TypeError::New(env, "Wrong argument types").ThrowAsJavaScriptException();
  }

  Napi::String remAddress = event_obj.Get("EvtSender").ToString();

  SrvEvent.EvtTime = static_cast<time_t>(event_obj.Get("EvtTime").As<Napi::Date>().ValueOf() / 1000);
  SrvEvent.EvtSender = inet_addr(remAddress.Utf8Value().c_str());
  SrvEvent.EvtCode =event_obj.Get("EvtCode").ToNumber().Uint32Value();
  SrvEvent.EvtRetCode = event_obj.Get("EvtRetCode").ToNumber().Uint32Value();
  SrvEvent.EvtParam1 = event_obj.Get("EvtParam1").ToNumber().Uint32Value();
  SrvEvent.EvtParam2 = event_obj.Get("EvtParam2").ToNumber().Uint32Value();
  SrvEvent.EvtParam3 = event_obj.Get("EvtParam3").ToNumber().Uint32Value();
  SrvEvent.EvtParam4 = event_obj.Get("EvtParam4").ToNumber().Uint32Value();

  return Napi::String::New(env,
    SrvEventText(&SrvEvent).c_str());
}

Napi::Value S7Server::LastError(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  return Napi::Number::New(env, lastError);
}

}  // namespace node_snap7
