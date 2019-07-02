/*
 * Copyright (c) 2019, Mathias Küsel
 * MIT License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

#include <node_snap7_server.h>

namespace node_snap7 {

static uv_async_t event_async_g;
static uv_async_t rw_async_g;
static std::mutex mutex_rw;
static std::mutex mutex_event;
static std::condition_variable sem_rw;
static std::deque<TSrvEvent> event_list_g;

static struct rw_event_baton_t {
  int Sender;
  int Operation;
  TS7Tag Tag;
  void *pUsrData;
} rw_event_baton_g;

void S7API EventCallBack(void *usrPtr, PSrvEvent PEvent, int Size) {
  mutex_event.lock();
  event_list_g.push_back(*PEvent);
  mutex_event.unlock();

  uv_async_send(&event_async_g);
}

int S7API RWAreaCallBack(void *usrPtr, int Sender, int Operation, PS7Tag PTag
  , void *pUsrData
) {
  uv_mutex_lock(&mutex_rw);
  rw_event_baton_g.Sender = Sender;
  rw_event_baton_g.Operation = Operation;
  rw_event_baton_g.Tag = *PTag;
  rw_event_baton_g.pUsrData = pUsrData;

  uv_async_send(&rw_async_g);

  sem_rw.wait();
  uv_mutex_unlock(&mutex_rw);

  return 0;
}

Napi::FunctionReference S7Server::constructor;

Napi::FunctionReference S7Server::constructor;

Napi::Object S7Server::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "S7Client", {

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
  StaticValue("errSrvCannotStart"
    , Napi::Value::From(env, errSrvCannotStart)),
  StaticValue("errSrvDBNullPointer"
    , Napi::Value::From(env, errSrvDBNullPointer)),
  StaticValue("errSrvAreaAlreadyExists"
    , Napi::Value::From(env, errSrvAreaAlreadyExists)),
  StaticValue("errSrvUnknownArea"
    , Napi::Value::From(env, errSrvUnknownArea)),
  StaticValue("errSrvInvalidParams"
    , Napi::Value::From(env, errSrvInvalidParams)),
  StaticValue("errSrvTooManyDB"
    , Napi::Value::From(env, errSrvTooManyDB)),
  StaticValue("errSrvInvalidParamNumber"
    , Napi::Value::From(env, errSrvInvalidParamNumber)),
  StaticValue("errSrvCannotChangeParam"
    , Napi::Value::From(env, errSrvCannotChangeParam)),

  // Server area IDs
  StaticValue("srvAreaPE"
    , Napi::Value::From(env, srvAreaPE)),
  StaticValue("srvAreaPA"
    , Napi::Value::From(env, srvAreaPA)),
  StaticValue("srvAreaMK"
    , Napi::Value::From(env, srvAreaMK)),
  StaticValue("srvAreaCT"
    , Napi::Value::From(env, srvAreaCT)),
  StaticValue("srvAreaTM"
    , Napi::Value::From(env, srvAreaTM)),
  StaticValue("srvAreaDB"
    , Napi::Value::From(env, srvAreaDB)),
  StaticValue("operationWrite"
    , Napi::Value::From(env, OperationWrite)),
  StaticValue("operationRead"
    , Napi::Value::From(env, OperationRead)),
  // TCP server event codes
  StaticValue("evcServerStarted"
    , Napi::Value::From(env, evcServerStarted)),
  StaticValue("evcServerStopped"
    , Napi::Value::From(env, evcServerStopped)),
  StaticValue("evcListenerCannotStart"
    , Napi::Value::From(env, evcListenerCannotStart)),
  StaticValue("evcClientAdded"
    , Napi::Value::From(env, evcClientAdded)),
  StaticValue("evcClientRejected"
    , Napi::Value::From(env, evcClientRejected)),
  StaticValue("evcClientNoRoom"
    , Napi::Value::From(env, evcClientNoRoom)),
  StaticValue("evcClientException"
    , Napi::Value::From(env, evcClientException)),
  StaticValue("evcClientDisconnected"
    , Napi::Value::From(env, evcClientDisconnected)),
  StaticValue("evcClientTerminated"
    , Napi::Value::From(env, evcClientTerminated)),
  StaticValue("evcClientsDropped"
    , Napi::Value::From(env, evcClientsDropped)),

  // S7 server event codes
  StaticValue("evcPDUincoming"
    , Napi::Value::From(env, evcPDUincoming)),
  StaticValue("evcDataRead"
    , Napi::Value::From(env, evcDataRead)),
  StaticValue("evcDataWrite"
    , Napi::Value::From(env, evcDataWrite)),
  StaticValue("evcNegotiatePDU"
    , Napi::Value::From(env, evcNegotiatePDU)),
  StaticValue("evcReadSZL"
    , Napi::Value::From(env, evcReadSZL)),
  StaticValue("evcClock"
    , Napi::Value::From(env, evcClock)),
  StaticValue("evcUpload"
    , Napi::Value::From(env, evcUpload)),
  StaticValue("evcDownload"
    , Napi::Value::From(env, evcDownload)),
  StaticValue("evcDirectory"
    , Napi::Value::From(env, evcDirectory)),
  StaticValue("evcSecurity"
    , Napi::Value::From(env, evcSecurity)),
  StaticValue("evcControl"
    , Napi::Value::From(env, evcControl)),

  // Masks to enable/disable all events
  StaticValue("evcAll"
    , Napi::Value::From(env, evcAll)),
  StaticValue("evcNone"
    , Napi::Value::From(env, evcNone)),

  // Event subcodes
  StaticValue("evsUnknown"
    , Napi::Value::From(env, evsUnknown)),
  StaticValue("evsStartUpload"
    , Napi::Value::From(env, evsStartUpload)),
  StaticValue("evsStartDownload"
    , Napi::Value::From(env, evsStartDownload)),
  StaticValue("evsGetBlockList"
    , Napi::Value::From(env, evsGetBlockList)),
  StaticValue("evsStartListBoT"
    , Napi::Value::From(env, evsStartListBoT)),
  StaticValue("evsListBoT"
    , Napi::Value::From(env, evsListBoT)),
  StaticValue("evsGetBlockInfo"
    , Napi::Value::From(env, evsGetBlockInfo)),
  StaticValue("evsGetClock"
    , Napi::Value::From(env, evsGetClock)),
  StaticValue("evsSetClock"
    , Napi::Value::From(env, evsSetClock)),
  StaticValue("evsSetPassword"
    , Napi::Value::From(env, evsSetPassword)),
  StaticValue("evsClrPassword"
    , Napi::Value::From(env, evsClrPassword)),

  // Event params : functions group
  StaticValue("grProgrammer"
    , Napi::Value::From(env, grProgrammer)),
  StaticValue("grCyclicData"
    , Napi::Value::From(env, grCyclicData)),
  StaticValue("grBlocksInfo"
    , Napi::Value::From(env, grBlocksInfo)),
  StaticValue("grSZL"
    , Napi::Value::From(env, grSZL)),
  StaticValue("grPassword"
    , Napi::Value::From(env, grPassword)),
  StaticValue("grBSend"
    , Napi::Value::From(env, grBSend)),
  StaticValue("grClock"
    , Napi::Value::From(env, grClock)),
  StaticValue("grSecurity"
    , Napi::Value::From(env, grSecurity)),

  // Event params : control codes
  StaticValue("CodeControlUnknown"
    , Napi::Value::From(env, CodeControlUnknown)),
  StaticValue("CodeControlColdStart"
    , Napi::Value::From(env, CodeControlColdStart)),
  StaticValue("CodeControlWarmStart"
    , Napi::Value::From(env, CodeControlWarmStart)),
  StaticValue("CodeControlStop"
    , Napi::Value::From(env, CodeControlStop)),
  StaticValue("CodeControlCompress"
    , Napi::Value::From(env, CodeControlCompress)),
  StaticValue("CodeControlCpyRamRom"
    , Napi::Value::From(env, CodeControlCpyRamRom)),
  StaticValue("CodeControlInsDel"
    , Napi::Value::From(env, CodeControlInsDel)),

  // Event results
  StaticValue("evrNoError"
    , Napi::Value::From(env, evrNoError)),
  StaticValue("evrFragmentRejected"
    , Napi::Value::From(env, evrFragmentRejected)),
  StaticValue("evrMalformedPDU"
    , Napi::Value::From(env, evrMalformedPDU)),
  StaticValue("evrSparseBytes"
    , Napi::Value::From(env, evrSparseBytes)),
  StaticValue("evrCannotHandlePDU"
    , Napi::Value::From(env, evrCannotHandlePDU)),
  StaticValue("evrNotImplemented"
    , Napi::Value::From(env, evrNotImplemented)),
  StaticValue("evrErrException"
    , Napi::Value::From(env, evrErrException)),
  StaticValue("evrErrAreaNotFound"
    , Napi::Value::From(env, evrErrAreaNotFound)),
  StaticValue("evrErrOutOfRange"
    , Napi::Value::From(env, evrErrOutOfRange)),
  StaticValue("evrErrOverPDU"
    , Napi::Value::From(env, evrErrOverPDU)),
  StaticValue("evrErrTransportSize"
    , Napi::Value::From(env, evrErrTransportSize)),
  StaticValue("evrInvalidGroupUData"
    , Napi::Value::From(env, evrInvalidGroupUData)),
  StaticValue("evrInvalidSZL"
    , Napi::Value::From(env, evrInvalidSZL)),
  StaticValue("evrDataSizeMismatch"
    , Napi::Value::From(env, evrDataSizeMismatch)),
  StaticValue("evrCannotUpload"
    , Napi::Value::From(env, evrCannotUpload)),
  StaticValue("evrCannotDownload"
    , Napi::Value::From(env, evrCannotDownload)),
  StaticValue("evrUploadInvalidID"
    , Napi::Value::From(env, evrUploadInvalidID)),
  StaticValue("evrResNotFound"
    , Napi::Value::From(env, evrResNotFound)),

  // Server parameter
  StaticValue("LocalPort"
    , Napi::Value::From(env, p_u16_LocalPort)),
  StaticValue("WorkInterval"
    , Napi::Value::From(env, p_i32_WorkInterval)),
  StaticValue("PDURequest"
    , Napi::Value::From(env, p_i32_PDURequest)),
  StaticValue("MaxClients"
    , Napi::Value::From(env, p_i32_MaxClients)),

  // CPU status codes
  StaticValue("S7CpuStatusUnknown"
    , Napi::Value::From(env, S7CpuStatusUnknown)),
  StaticValue("S7CpuStatusRun"
    , Napi::Value::From(env, S7CpuStatusRun)),
  StaticValue("S7CpuStatusStop"
    , Napi::Value::From(env, S7CpuStatusStop)),

  // Server status codes
  StaticValue("SrvStopped"
    , Napi::Value::From(env, 0)),
  StaticValue("SrvRunning"
    , Napi::Value::From(env, 1)),
  StaticValue("SrvError"
    , Napi::Value::From(env, 2))
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("S7Client", func);
  return exports;
}

S7Server::S7Server(const Napi::CallbackInfo& info) {
  lastError = 0;
  snap7Server = new TS7Server();

  event_async_g.data = rw_async_g.data = this;

  uv_async_init(uv_default_loop(), &event_async_g, S7Server::HandleEvent);
  uv_async_init(uv_default_loop(), &rw_async_g, S7Server::HandleReadWriteEvent);

  uv_unref(reinterpret_cast<uv_handle_t *>(&event_async_g));
  uv_unref(reinterpret_cast<uv_handle_t *>(&rw_async_g));
  uv_mutex_init(&mutex);
  uv_mutex_init(&mutex_rw);
  uv_mutex_init(&mutex_event);
  uv_sem_init(&sem_rw, 0);

  snap7Server->SetEventsCallback(&EventCallBack, NULL);
}

S7Server::~S7Server() {
  snap7Server->Stop();
  delete snap7Server;

  constructor.Reset();

  uv_close(reinterpret_cast<uv_handle_t *>(&event_async_g), 0);
  uv_close(reinterpret_cast<uv_handle_t *>(&rw_async_g), 0);
  uv_sem_destroy(&sem_rw);
  uv_mutex_destroy(&mutex_event);
  uv_mutex_destroy(&mutex_rw);
  uv_mutex_destroy(&mutex);
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

Napi::Value S7Server::RWBufferCallback(const Napi::CallbackInfo& info) {
  Nan::HandleScope scope;

  if (rw_event_baton_g.Operation == OperationRead) {
    if (!node::Buffer::HasInstance(info[0])) {
      return Nan::ThrowTypeError("Wrong argument");
    }

    int byteCount, size;
    byteCount = S7Server::GetByteCountFromWordLen(rw_event_baton_g.Tag.WordLen);
    size = byteCount * rw_event_baton_g.Tag.Size;

    if (node::Buffer::Length(info[0].As<v8::Object>()) < size) {
      return Nan::ThrowTypeError("Buffer length too small");
    }

    memcpy(
        rw_event_baton_g.pUsrData
      , node::Buffer::Data(info[0].As<v8::Object>())
      , size);
  }

  uv_sem_post(&sem_rw);
}

void S7Server::HandleEvent(uv_async_t* handle, int status) {
  Nan::HandleScope scope;

  S7Server *s7server = static_cast<S7Server*>(handle->data);

  uv_mutex_lock(&mutex_event);
  while (!event_list_g.empty()) {
    PSrvEvent Event = &event_list_g.front();
    in_addr sin;
    sin.s_addr = Event->EvtSender;
    double time = static_cast<double>(Event->EvtTime * 1000);

    v8::Local<v8::Object> event_obj = Nan::New<v8::Object>();
    Nan::Set(event_obj, Nan::New<v8::String>("EvtTime")
      , Nan::New<v8::Date>(time));
    Nan::Set(event_obj, Nan::New<v8::String>("EvtSender")
      , Nan::New<v8::String>(inet_ntoa(sin)));
    Nan::Set(event_obj, Nan::New<v8::String>("EvtCode")
      , Nan::New<v8::Uint32>(Event->EvtCode));
    Nan::Set(event_obj, Nan::New<v8::String>("EvtRetCode")
      , Nan::New<v8::Integer>(Event->EvtRetCode));
    Nan::Set(event_obj, Nan::New<v8::String>("EvtParam1")
      , Nan::New<v8::Integer>(Event->EvtParam1));
    Nan::Set(event_obj, Nan::New<v8::String>("EvtParam2")
      , Nan::New<v8::Integer>(Event->EvtParam2));
    Nan::Set(event_obj, Nan::New<v8::String>("EvtParam3")
      , Nan::New<v8::Integer>(Event->EvtParam3));
    Nan::Set(event_obj, Nan::New<v8::String>("EvtParam4")
      , Nan::New<v8::Integer>(Event->EvtParam4));

    v8::Local<v8::Value> argv[2] = {
      Nan::New("event"),
      event_obj
    };

    s7server->async_resource.runInAsyncScope(s7server->handle(), "emit", 2, argv);
    event_list_g.pop_front();
  }
  uv_mutex_unlock(&mutex_event);
}


void S7Server::HandleReadWriteEvent(uv_async_t* handle, int status) {
  Napi::AsyncContext context(info.Env(), "S7Server:emit", resource);

  S7Server *s7server = static_cast<S7Server*>(handle->data);
  in_addr sin;
  sin.s_addr = rw_event_baton_g.Sender;

  v8::Local<v8::Object> rw_tag_obj = Nan::New<v8::Object>();
  Nan::Set(rw_tag_obj, Nan::New<v8::String>("Area")
    , Nan::New<v8::Integer>(rw_event_baton_g.Tag.Area));
  Nan::Set(rw_tag_obj, Nan::New<v8::String>("DBNumber")
    , Nan::New<v8::Integer>(rw_event_baton_g.Tag.DBNumber));
  Nan::Set(rw_tag_obj, Nan::New<v8::String>("Start")
    , Nan::New<v8::Integer>(rw_event_baton_g.Tag.Start));
  Nan::Set(rw_tag_obj, Nan::New<v8::String>("Size")
    , Nan::New<v8::Integer>(rw_event_baton_g.Tag.Size));
  Nan::Set(rw_tag_obj, Nan::New<v8::String>("WordLen")
    , Nan::New<v8::Integer>(rw_event_baton_g.Tag.WordLen));

  int byteCount, size;
  byteCount = S7Server::GetByteCountFromWordLen(rw_event_baton_g.Tag.WordLen);
  size = byteCount * rw_event_baton_g.Tag.Size;

  v8::Local<v8::Object> buffer;

  if (rw_event_baton_g.Operation == OperationWrite) {
    buffer = Nan::CopyBuffer(
      static_cast<char*>(rw_event_baton_g.pUsrData),
      size);
  } else {
    buffer = Nan::NewBuffer(size);
    memset(node::Buffer::Data(buffer), 0, size);
  }

  v8::Local<v8::Value> argv[6] = {
    Nan::New("readWrite"),
    Nan::New<v8::String>(inet_ntoa(sin)),
    Nan::New<v8::Integer>(rw_event_baton_g.Operation),
    rw_tag_obj,
    buffer,
    Nan::New<v8::Function>(S7Server::RWBufferCallback)
  };

  s7server->async_resource.runInAsyncScope(s7server->handle(), "emit", 6, argv);
}

void IOWorkerServer::Execute() {
  uv_mutex_lock(&s7server->mutex);

  switch (caller) {
  case STARTTO:
    ret = s7server->snap7Server->StartTo(
      **static_cast<Nan::Utf8String*>(pData));

    if (ret == 0) {
      uv_ref(reinterpret_cast<uv_handle_t *>(&event_async_g));
    }
    break;

  case START:
    ret = s7server->snap7Server->Start();

    if (ret == 0) {
      uv_ref(reinterpret_cast<uv_handle_t *>(&event_async_g));
    }
    break;

  case STOP:
    ret = s7server->snap7Server->Stop();

    if (ret == 0) {
      uv_unref(reinterpret_cast<uv_handle_t *>(&event_async_g));
    }
    break;
  }

  uv_mutex_unlock(&s7server->mutex);
}

void IOWorkerServer::HandleOKCallback() {
  Nan::HandleScope scope;

  v8::Local<v8::Value> argv1[1];
  v8::Local<v8::Value> argv2[2];

  if (ret == 0) {
    argv2[0] = argv1[0] = Nan::Null();
  } else {
    argv2[0] = argv1[0] = Nan::New<v8::Integer>(ret);
  }

  switch (caller) {
  case STARTTO:
    delete static_cast<Nan::Utf8String*>(pData);
    callback->Call(1, argv1, async_resource);
    break;

  case START:
  case STOP:
    callback->Call(1, argv1, async_resource);
    break;
  }
}

Napi::Value S7Server::Start(const Napi::CallbackInfo& info) {
  S7Server *s7server = ObjectWrap::Unwrap<S7Server>(info.Holder());

  if (!info[0]->IsFunction()) {
    int ret = s7server->snap7Server->Start();

    if (ret == 0) {
      uv_ref(reinterpret_cast<uv_handle_t *>(&event_async_g));
    }

    s7server->lastError = ret;

    info.GetReturnValue().Set(Nan::New<v8::Boolean>(ret == 0));
  } else {
    Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorkerServer(callback, s7server, START));
    info.GetReturnValue().SetUndefined();
  }
}

Napi::Value S7Server::StartTo(const Napi::CallbackInfo& info) {
  S7Server *s7server = ObjectWrap::Unwrap<S7Server>(info.Holder());

  if (info.Length() < 1) {
    return Nan::ThrowTypeError("Wrong number of arguments");
  }

  if (!info[0]->IsString()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  Nan::Utf8String *address = new Nan::Utf8String(info[0]);
  if (!info[1]->IsFunction()) {
    int ret = s7server->snap7Server->StartTo(**address);
    delete address;

    if (ret == 0) {
      uv_ref(reinterpret_cast<uv_handle_t *>(&event_async_g));
    }

    s7server->lastError = ret;

    info.GetReturnValue().Set(Nan::New<v8::Boolean>(ret == 0));
  } else {
    Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorkerServer(callback, s7server, STARTTO
      , address));
    info.GetReturnValue().SetUndefined();
  }
}

Napi::Value S7Server::Stop(const Napi::CallbackInfo& info) {
  S7Server *s7server = ObjectWrap::Unwrap<S7Server>(info.Holder());

  if (!info[0]->IsFunction()) {
    int ret = s7server->snap7Server->Stop();

    if (ret == 0) {
      uv_unref(reinterpret_cast<uv_handle_t *>(&event_async_g));
    }

    s7server->lastError = ret;

    info.GetReturnValue().Set(Nan::New<v8::Boolean>(ret == 0));
  } else {
    Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorkerServer(callback, s7server, STOP));
    info.GetReturnValue().SetUndefined();
  }
}

Napi::Value S7Server::SetResourceless(const Napi::CallbackInfo& info) {
  S7Server *s7server = ObjectWrap::Unwrap<S7Server>(info.Holder());

  if (!info[0]->IsBoolean()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  bool resourceless = Nan::To<bool>(info[0]).FromJust();

  int ret;
  if (resourceless) {
    ret = s7server->snap7Server->SetRWAreaCallback(&RWAreaCallBack, NULL);
  } else {
    ret = s7server->snap7Server->SetRWAreaCallback(NULL, NULL);
  }
  s7server->lastError = ret;

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(ret == 0));
}

Napi::Value S7Server::GetParam(const Napi::CallbackInfo& info) {
  S7Server *s7server = ObjectWrap::Unwrap<S7Server>(info.Holder());

  if (!info[0]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  int pData;
  int ret = s7server->snap7Server->GetParam(Nan::To<int32_t>(info[0]).FromJust()
    , &pData);
  s7server->lastError = ret;

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(ret == 0));
}

Napi::Value S7Server::SetParam(const Napi::CallbackInfo& info) {
  S7Server *s7server = ObjectWrap::Unwrap<S7Server>(info.Holder());

  if (!(info[0]->IsInt32() || info[1]->IsInt32())) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  int pData = Nan::To<int32_t>(info[1]).FromJust();
  int ret = s7server->snap7Server->SetParam(Nan::To<int32_t>(info[0]).FromJust(), &pData);
  s7server->lastError = ret;

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(ret == 0));
}

Napi::Value S7Server::GetEventsMask(const Napi::CallbackInfo& info) {
  S7Server *s7server = ObjectWrap::Unwrap<S7Server>(info.Holder());

  int ret = s7server->snap7Server->GetEventsMask();

  info.GetReturnValue().Set(Nan::New<v8::Uint32>(ret));
}

Napi::Value S7Server::SetEventsMask(const Napi::CallbackInfo& info) {
  S7Server *s7server = ObjectWrap::Unwrap<S7Server>(info.Holder());

  if (!info[0]->IsUint32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  s7server->snap7Server->SetEventsMask(Nan::To<uint32_t>(info[0]).FromJust());
  info.GetReturnValue().SetUndefined();
}

Napi::Value S7Server::RegisterArea(const Napi::CallbackInfo& info) {
  S7Server *s7server = ObjectWrap::Unwrap<S7Server>(info.Holder());

  if (!info[0]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  int index;
  char *pBuffer;
  size_t len;
  int area = Nan::To<int32_t>(info[0]).FromJust();

  if (area == srvAreaDB) {
    if (!info[1]->IsInt32() || !node::Buffer::HasInstance(info[2])) {
      return Nan::ThrowTypeError("Wrong arguments");
    }

    index = Nan::To<int32_t>(info[1]).FromJust();
    len = node::Buffer::Length(info[2].As<v8::Object>());
    pBuffer = node::Buffer::Data(info[2].As<v8::Object>());
  } else if (!node::Buffer::HasInstance(info[1])) {
    return Nan::ThrowTypeError("Wrong arguments");
  } else {
    index = 0;
    len = node::Buffer::Length(info[1].As<v8::Object>());
    pBuffer = node::Buffer::Data(info[1].As<v8::Object>());
  }

  if (len > 0xFFFF) {
    return Nan::ThrowRangeError("Max area buffer size is 65535");
  }

  word size = static_cast<word>(len);
  char *data = new char[size];
  memcpy(data, pBuffer, size);

  int ret = s7server->snap7Server->RegisterArea(area, index, data, size);
  s7server->lastError = ret;

  if (ret == 0) {
    s7server->area2buffer[area][index].pBuffer = data;
    s7server->area2buffer[area][index].size = size;
  } else {
    delete[] data;
  }

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(ret == 0));
}

Napi::Value S7Server::UnregisterArea(const Napi::CallbackInfo& info) {
  S7Server *s7server = ObjectWrap::Unwrap<S7Server>(info.Holder());

  if (!info[0]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  int index = 0;
  int area = Nan::To<int32_t>(info[0]).FromJust();

  if (area == srvAreaDB) {
    if (!info[1]->IsInt32()) {
      return Nan::ThrowTypeError("Wrong arguments");
    }

    index = Nan::To<int32_t>(info[1]).FromJust();
  }

  int ret = s7server->snap7Server->UnregisterArea(area, index);
  s7server->lastError = ret;

  if (ret == 0) {
    delete[] s7server->area2buffer[area][index].pBuffer;
    s7server->area2buffer[area].erase(index);
  }

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(ret == 0));
}

Napi::Value S7Server::SetArea(const Napi::CallbackInfo& info) {
  S7Server *s7server = ObjectWrap::Unwrap<S7Server>(info.Holder());

  if (!info[0]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  int area = Nan::To<int32_t>(info[0]).FromJust();
  if (!s7server->area2buffer.count(area)) {
    return Nan::ThrowError("Unknown area");
  }

  int index;
  char *pBuffer;
  size_t len;

  if (area == srvAreaDB) {
    if (!info[1]->IsInt32() || !node::Buffer::HasInstance(info[2])) {
      return Nan::ThrowTypeError("Wrong arguments");
    }

    index = Nan::To<int32_t>(info[1]).FromJust();
    if (!s7server->area2buffer[area].count(index)) {
      return Nan::ThrowError("DB index not found");
    }

    len = node::Buffer::Length(info[2].As<v8::Object>());
    pBuffer = node::Buffer::Data(info[2].As<v8::Object>());
  } else {
    index = 0;
    if (node::Buffer::HasInstance(info[1])) {
      len = node::Buffer::Length(info[1].As<v8::Object>());
      pBuffer = node::Buffer::Data(info[1].As<v8::Object>());
    } else if (node::Buffer::HasInstance(info[2])) {
      len = node::Buffer::Length(info[2].As<v8::Object>());
      pBuffer = node::Buffer::Data(info[2].As<v8::Object>());
    } else {
      return Nan::ThrowTypeError("Wrong arguments");
    }
  }

  if (len != s7server->area2buffer[area][index].size) {
    return Nan::ThrowError("Wrong buffer length");
  }

  s7server->snap7Server->LockArea(area, index);

  memcpy(
    s7server->area2buffer[area][index].pBuffer
    , pBuffer, s7server->area2buffer[area][index].size);

  s7server->snap7Server->UnlockArea(area, index);

  info.GetReturnValue().SetUndefined();
}

Napi::Value S7Server::GetArea(const Napi::CallbackInfo& info) {
  S7Server *s7server = ObjectWrap::Unwrap<S7Server>(info.Holder());

  if (!info[0]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  int index = 0;
  int area = Nan::To<int32_t>(info[0]).FromJust();

  if (!s7server->area2buffer.count(area)) {
    return Nan::ThrowError("Unknown area");
  }

  if (area == srvAreaDB) {
    if (!info[1]->IsInt32()) {
      return Nan::ThrowTypeError("Wrong arguments");
    }

    index = Nan::To<int32_t>(info[1]).FromJust();
    if (!s7server->area2buffer[area].count(index)) {
      return Nan::ThrowError("DB index not found");
    }
  }

  s7server->snap7Server->LockArea(area, index);

  v8::Local<v8::Object> buffer = Nan::CopyBuffer(
      s7server->area2buffer[area][index].pBuffer
    , s7server->area2buffer[area][index].size);

  s7server->snap7Server->UnlockArea(area, index);

  info.GetReturnValue().Set(buffer);
}

Napi::Value S7Server::LockArea(const Napi::CallbackInfo& info) {
  S7Server *s7server = ObjectWrap::Unwrap<S7Server>(info.Holder());

  if (!info[0]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  int index = 0;
  int area = Nan::To<int32_t>(info[0]).FromJust();

  if (area == srvAreaDB) {
    if (!info[1]->IsInt32()) {
      return Nan::ThrowTypeError("Wrong arguments");
    }

    index = Nan::To<int32_t>(info[1]).FromJust();
  }

  int ret = s7server->snap7Server->LockArea(area, index);
  s7server->lastError = ret;

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(ret == 0));
}

Napi::Value S7Server::UnlockArea(const Napi::CallbackInfo& info) {
  S7Server *s7server = ObjectWrap::Unwrap<S7Server>(info.Holder());

  if (!info[0]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  int index = 0;
  int area = Nan::To<int32_t>(info[0]).FromJust();

  if (area == srvAreaDB) {
    if (!info[1]->IsInt32()) {
      return Nan::ThrowTypeError("Wrong arguments");
    }

    index = Nan::To<int32_t>(info[1]).FromJust();
  }

  int ret = s7server->snap7Server->UnlockArea(area, index);
  s7server->lastError = ret;

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(ret == 0));
}

Napi::Value S7Server::ServerStatus(const Napi::CallbackInfo& info) {
  S7Server *s7server = ObjectWrap::Unwrap<S7Server>(info.Holder());

  int ret = s7server->snap7Server->ServerStatus();
  if ((ret == 0) || (ret == 1) || (ret == 2)) {
    s7server->lastError = 0;
    info.GetReturnValue().Set(Nan::New<v8::Integer>(ret));
  } else {
    s7server->lastError = ret;
    info.GetReturnValue().Set(Nan::False());
  }
}

Napi::Value S7Server::ClientsCount(const Napi::CallbackInfo& info) {
  S7Server *s7server = ObjectWrap::Unwrap<S7Server>(info.Holder());

  int ret = s7server->snap7Server->ClientsCount();
  info.GetReturnValue().Set(Nan::New<v8::Integer>(ret));
}

Napi::Value S7Server::GetCpuStatus(const Napi::CallbackInfo& info) {
  S7Server *s7server = ObjectWrap::Unwrap<S7Server>(info.Holder());

  int ret = s7server->snap7Server->GetCpuStatus();
  if ((ret == S7CpuStatusUnknown) ||
    (ret == S7CpuStatusStop) ||
    (ret == S7CpuStatusRun)) {
    s7server->lastError = 0;
    info.GetReturnValue().Set(Nan::New<v8::Integer>(ret));
  } else {
    s7server->lastError = ret;
    info.GetReturnValue().Set(Nan::False());
  }
}

Napi::Value S7Server::SetCpuStatus(const Napi::CallbackInfo& info) {
  S7Server *s7server = ObjectWrap::Unwrap<S7Server>(info.Holder());

  if (!info[0]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  int ret = s7server->snap7Server->SetCpuStatus(Nan::To<int32_t>(info[0]).FromJust());
  s7server->lastError = ret;

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(ret == 0));
}

Napi::Value S7Server::ErrorText(const Napi::CallbackInfo& info) {
  if (!info[0]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  info.GetReturnValue().Set(Nan::New<v8::String>(
    SrvErrorText(Nan::To<int32_t>(info[0]).FromJust()).c_str()));
}

Napi::Value S7Server::EventText(const Napi::CallbackInfo& info) {
  TSrvEvent SrvEvent;

  if (!info[0]->IsObject()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  v8::Local<v8::Object> event_obj = v8::Local<v8::Object>::Cast(info[0]);
  if (!Nan::Has(event_obj, Nan::New<v8::String>("EvtTime")).FromJust() ||
    !Nan::Has(event_obj, Nan::New<v8::String>("EvtSender")).FromJust() ||
    !Nan::Has(event_obj, Nan::New<v8::String>("EvtCode")).FromJust() ||
    !Nan::Has(event_obj, Nan::New<v8::String>("EvtRetCode")).FromJust() ||
    !Nan::Has(event_obj, Nan::New<v8::String>("EvtParam1")).FromJust() ||
    !Nan::Has(event_obj, Nan::New<v8::String>("EvtParam2")).FromJust() ||
    !Nan::Has(event_obj, Nan::New<v8::String>("EvtParam3")).FromJust() ||
    !Nan::Has(event_obj, Nan::New<v8::String>("EvtParam4")).FromJust()) {
    return Nan::ThrowTypeError("Wrong argument structure");
  }

  if (!Nan::Get(event_obj, Nan::New<v8::String>("EvtTime"))->IsDate() ||
    !Nan::Get(event_obj, Nan::New<v8::String>("EvtSender"))->IsString() ||
    !Nan::Get(event_obj, Nan::New<v8::String>("EvtCode"))->IsUint32() ||
    !Nan::Get(event_obj, Nan::New<v8::String>("EvtRetCode"))->IsInt32() ||
    !Nan::Get(event_obj, Nan::New<v8::String>("EvtParam1"))->IsInt32() ||
    !Nan::Get(event_obj, Nan::New<v8::String>("EvtParam2"))->IsInt32() ||
    !Nan::Get(event_obj, Nan::New<v8::String>("EvtParam3"))->IsInt32() ||
    !Nan::Get(event_obj, Nan::New<v8::String>("EvtParam4"))->IsInt32()) {
    return Nan::ThrowTypeError("Wrong argument types");
  }

  Nan::Utf8String *remAddress = new Nan::Utf8String(
    Nan::Get(
        event_obj
      , Nan::New<v8::String>("EvtSender")));

  SrvEvent.EvtTime = static_cast<time_t>(Nan::To<double>(v8::Local<v8::Date>::Cast(Nan::Get(event_obj, Nan::New<v8::String>("EvtTime")))).FromJust() / 1000);
  SrvEvent.EvtSender = inet_addr(**remAddress);
  SrvEvent.EvtCode = Nan::To<uint32_t>(Nan::Get(event_obj, Nan::New<v8::String>("EvtCode"))).FromJust();
  SrvEvent.EvtRetCode = Nan::To<int32_t>(Nan::Get(event_obj, Nan::New<v8::String>("EvtRetCode"))).FromJust();
  SrvEvent.EvtParam1 = Nan::To<int32_t>(Nan::Get(event_obj, Nan::New<v8::String>("EvtParam1"))).FromJust();
  SrvEvent.EvtParam2 = Nan::To<int32_t>(Nan::Get(event_obj, Nan::New<v8::String>("EvtParam2"))).FromJust();
  SrvEvent.EvtParam3 = Nan::To<int32_t>(Nan::Get(event_obj, Nan::New<v8::String>("EvtParam3"))).FromJust();
  SrvEvent.EvtParam4 = Nan::To<int32_t>(Nan::Get(event_obj, Nan::New<v8::String>("EvtParam4"))).FromJust();

  delete remAddress;

  info.GetReturnValue().Set(Nan::New<v8::String>(
    SrvEventText(&SrvEvent).c_str()));
}

Napi::Value S7Server::LastError(const Napi::CallbackInfo& info) {
  S7Server *s7server = ObjectWrap::Unwrap<S7Server>(info.Holder());

  info.GetReturnValue().Set(Nan::New<v8::Integer>(s7server->lastError));
}

}  // namespace node_snap7
