/*
 * Copyright (c) 2019, Mathias Küsel
 * MIT License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

#include <node_snap7_client.h>
#include <sstream>

namespace node_snap7 {

Napi::Object S7Client::Init(Napi::Env env, Napi::Object exports) {
	Napi::Function func = DefineClass(env, "S7Client", {
		// Control functions
		InstanceMethod("Connect", &S7Client::Connect),
		InstanceMethod("ConnectTo", &S7Client::ConnectTo),
		InstanceMethod("SetConnectionParams", &S7Client::SetConnectionParams),
		InstanceMethod("SetConnectionType", &S7Client::SetConnectionType),
		InstanceMethod("Disconnect", &S7Client::Disconnect),
		InstanceMethod("GetParam", &S7Client::GetParam),
		InstanceMethod("SetParam", &S7Client::SetParam),

		// Data I/O Main functions
		InstanceMethod("ReadArea", &S7Client::ReadArea),
		InstanceMethod("WriteArea", &S7Client::WriteArea),
		InstanceMethod("ReadMultiVars", &S7Client::ReadMultiVars),
		InstanceMethod("WriteMultiVars", &S7Client::WriteMultiVars),

		// Directory functions
		InstanceMethod("ListBlocks", &S7Client::ListBlocks),
		InstanceMethod("GetAgBlockInfo", &S7Client::GetAgBlockInfo),
		InstanceMethod("GetPgBlockInfo", &S7Client::GetPgBlockInfo),
		InstanceMethod("ListBlocksOfType", &S7Client::ListBlocksOfType),

		// Blocks functions
		InstanceMethod("Upload", &S7Client::Upload),
		InstanceMethod("FullUpload", &S7Client::FullUpload),
		InstanceMethod("Download", &S7Client::Download),
		InstanceMethod("Delete", &S7Client::Delete),
		InstanceMethod("DBGet", &S7Client::DBGet),
		InstanceMethod("DBFill", &S7Client::DBFill),

		// Date/Time functions
		InstanceMethod("GetPlcDateTime", &S7Client::GetPlcDateTime),
		InstanceMethod("SetPlcDateTime", &S7Client::SetPlcDateTime),
		InstanceMethod("SetPlcSystemDateTime", &S7Client::SetPlcSystemDateTime),

		// System Info functions
		InstanceMethod("GetOrderCode", &S7Client::GetOrderCode),
		InstanceMethod("GetCpuInfo", &S7Client::GetCpuInfo),
		InstanceMethod("GetCpInfo", &S7Client::GetCpInfo),
		InstanceMethod("ReadSZL", &S7Client::ReadSZL),
		InstanceMethod("ReadSZLList", &S7Client::ReadSZLList),

		// Control functions
		InstanceMethod("PlcHotStart", &S7Client::PlcHotStart),
		InstanceMethod("PlcColdStart", &S7Client::PlcColdStart),
		InstanceMethod("PlcStop", &S7Client::PlcStop),
		InstanceMethod("CopyRamToRom", &S7Client::CopyRamToRom),
		InstanceMethod("Compress", &S7Client::Compress),

		// Security functions
		InstanceMethod("GetProtection", &S7Client::GetProtection),
		InstanceMethod("SetSessionPassword", &S7Client::SetSessionPassword),
		InstanceMethod("ClearSessionPassword", &S7Client::ClearSessionPassword),

		// Properties
		InstanceMethod("ExecTime", &S7Client::ExecTime),
		InstanceMethod("LastError", &S7Client::LastError),
		InstanceMethod("PDURequested", &S7Client::PDURequested),
		InstanceMethod("PDULength", &S7Client::PDULength),
		InstanceMethod("PlcStatus", &S7Client::PlcStatus),
		InstanceMethod("Connected", &S7Client::Connected),

		// Error to text function
		InstanceMethod("ErrorText", &S7Client::ErrorText),

		// Error codes
		StaticValue("errNegotiatingPDU", Napi::Value::From(env, errNegotiatingPDU)),
    StaticValue("errCliInvalidParams", Napi::Value::From(env, errCliInvalidParams)),
    StaticValue("errCliJobPending", Napi::Value::From(env, errCliJobPending)),
    StaticValue("errCliTooManyItems", Napi::Value::From(env, errCliTooManyItems)),
    StaticValue("errCliInvalidWordLen", Napi::Value::From(env, errCliInvalidWordLen)),
    StaticValue("errCliPartialDataWritten", Napi::Value::From(env, errCliPartialDataWritten)),
    StaticValue("errCliSizeOverPDU", Napi::Value::From(env, errCliSizeOverPDU)),
    StaticValue("errCliInvalidPlcAnswer", Napi::Value::From(env, errCliInvalidPlcAnswer)),
    StaticValue("errCliAddressOutOfRange", Napi::Value::From(env, errCliAddressOutOfRange)),
    StaticValue("errCliInvalidTransportSize", Napi::Value::From(env, errCliInvalidTransportSize)),
    StaticValue("errCliWriteDataSizeMismatch", Napi::Value::From(env, errCliWriteDataSizeMismatch)),
    StaticValue("errCliItemNotAvailable", Napi::Value::From(env, errCliItemNotAvailable)),
    StaticValue("errCliInvalidValue", Napi::Value::From(env, errCliInvalidValue)),
    StaticValue("errCliCannotStartPLC", Napi::Value::From(env, errCliCannotStartPLC)),
    StaticValue("errCliAlreadyRun", Napi::Value::From(env, errCliAlreadyRun)),
    StaticValue("errCliCannotStopPLC", Napi::Value::From(env, errCliCannotStopPLC)),
    StaticValue("errCliCannotCopyRamToRom", Napi::Value::From(env, errCliCannotCopyRamToRom)),
    StaticValue("errCliCannotCompress", Napi::Value::From(env, errCliCannotCompress)),
    StaticValue("errCliAlreadyStop", Napi::Value::From(env, errCliAlreadyStop)),
    StaticValue("errCliFunNotAvailable", Napi::Value::From(env, errCliFunNotAvailable)),
    StaticValue("errCliUploadSequenceFailed", Napi::Value::From(env, errCliUploadSequenceFailed)),
    StaticValue("errCliInvalidDataSizeRecvd", Napi::Value::From(env, errCliInvalidDataSizeRecvd)),
    StaticValue("errCliInvalidBlockType", Napi::Value::From(env, errCliInvalidBlockType)),
    StaticValue("errCliInvalidBlockNumber", Napi::Value::From(env, errCliInvalidBlockNumber)),
    StaticValue("errCliInvalidBlockSize", Napi::Value::From(env, errCliInvalidBlockSize)),
    StaticValue("errCliDownloadSequenceFailed", Napi::Value::From(env, errCliDownloadSequenceFailed)),
    StaticValue("errCliInsertRefused", Napi::Value::From(env, errCliInsertRefused)),
    StaticValue("errCliDeleteRefused", Napi::Value::From(env, errCliDeleteRefused)),
    StaticValue("errCliNeedPassword", Napi::Value::From(env, errCliNeedPassword)),
    StaticValue("errCliInvalidPassword", Napi::Value::From(env, errCliInvalidPassword)),
    StaticValue("errCliNoPasswordToSetOrClear", Napi::Value::From(env, errCliNoPasswordToSetOrClear)),
    StaticValue("errCliJobTimeout", Napi::Value::From(env, errCliJobTimeout)),
    StaticValue("errCliPartialDataRead", Napi::Value::From(env, errCliPartialDataRead)),
    StaticValue("errCliBufferTooSmall", Napi::Value::From(env, errCliBufferTooSmall)),
    StaticValue("errCliFunctionRefused", Napi::Value::From(env, errCliFunctionRefused)),
    StaticValue("errCliDestroying", Napi::Value::From(env, errCliDestroying)),
    StaticValue("errCliInvalidParamNumber", Napi::Value::From(env, errCliInvalidParamNumber)),
    StaticValue("errCliCannotChangeParam", Napi::Value::From(env, errCliCannotChangeParam)),

    // Client Connection Type
    StaticValue("CONNTYPE_PG", Napi::Value::From(env, CONNTYPE_PG)),
    StaticValue("CONNTYPE_OP", Napi::Value::From(env, CONNTYPE_OP)),
    StaticValue("CONNTYPE_BASIC", Napi::Value::From(env, CONNTYPE_BASIC)),

    // CPU Status codes
    StaticValue("S7CpuStatusUnknown", Napi::Value::From(env, S7CpuStatusUnknown)),
    StaticValue("S7CpuStatusRun", Napi::Value::From(env, S7CpuStatusRun)),
    StaticValue("S7CpuStatusStop", Napi::Value::From(env, S7CpuStatusStop)),

    // Area ID
    StaticValue("S7AreaPE", Napi::Value::From(env, S7AreaPE)),
    StaticValue("S7AreaPA", Napi::Value::From(env, S7AreaPA)),
    StaticValue("S7AreaMK", Napi::Value::From(env, S7AreaMK)),
    StaticValue("S7AreaDB", Napi::Value::From(env, S7AreaDB)),
    StaticValue("S7AreaCT", Napi::Value::From(env, S7AreaCT)),
    StaticValue("S7AreaTM", Napi::Value::From(env, S7AreaTM)),

    // Word Length
    StaticValue("S7WLBit", Napi::Value::From(env, S7WLBit)),
    StaticValue("S7WLByte", Napi::Value::From(env, S7WLByte)),
    StaticValue("S7WLWord", Napi::Value::From(env, S7WLWord)),
    StaticValue("S7WLDWord", Napi::Value::From(env, S7WLDWord)),
    StaticValue("S7WLReal", Napi::Value::From(env, S7WLReal)),
    StaticValue("S7WLCounter", Napi::Value::From(env, S7WLCounter)),
    StaticValue("S7WLTimer", Napi::Value::From(env, S7WLTimer)),

    // Block type
    StaticValue("Block_OB", Napi::Value::From(env, Block_OB)),
    StaticValue("Block_DB", Napi::Value::From(env, Block_DB)),
    StaticValue("Block_SDB", Napi::Value::From(env, Block_SDB)),
    StaticValue("Block_FC", Napi::Value::From(env, Block_FC)),
    StaticValue("Block_SFC", Napi::Value::From(env, Block_SFC)),
    StaticValue("Block_FB", Napi::Value::From(env, Block_FB)),
    StaticValue("Block_SFB", Napi::Value::From(env, Block_SFB)),

    // Sub Block Type
    StaticValue("SubBlk_OB", Napi::Value::From(env, SubBlk_OB)),
    StaticValue("SubBlk_SDB", Napi::Value::From(env, SubBlk_SDB)),
    StaticValue("SubBlk_FC", Napi::Value::From(env, SubBlk_FC)),
    StaticValue("SubBlk_SFC", Napi::Value::From(env, SubBlk_SFC)),
    StaticValue("SubBlk_FB", Napi::Value::From(env, SubBlk_FB)),
    StaticValue("SubBlk_SFB", Napi::Value::From(env, SubBlk_SFB)),

    // Block languages
    StaticValue("BlockLangAWL", Napi::Value::From(env, BlockLangAWL)),
    StaticValue("BlockLangKOP", Napi::Value::From(env, BlockLangKOP)),
    StaticValue("BlockLangFUP", Napi::Value::From(env, BlockLangFUP)),
    StaticValue("BlockLangSCL", Napi::Value::From(env, BlockLangSCL)),
    StaticValue("BlockLangDB", Napi::Value::From(env, BlockLangDB)),
    StaticValue("BlockLangGRAPH", Napi::Value::From(env, BlockLangGRAPH)),

    // Parameter
    StaticValue("LocalPort", Napi::Value::From(env, p_u16_LocalPort)),
    StaticValue("RemotePort", Napi::Value::From(env, p_u16_RemotePort)),
    StaticValue("PingTimeout", Napi::Value::From(env, p_i32_PingTimeout)),
    StaticValue("SendTimeout", Napi::Value::From(env, p_i32_SendTimeout)),
    StaticValue("RecvTimeout", Napi::Value::From(env, p_i32_RecvTimeout)),
    StaticValue("WorkInterval", Napi::Value::From(env, p_i32_WorkInterval)),
    StaticValue("SrcRef", Napi::Value::From(env, p_u16_SrcRef)),
    StaticValue("DstRef", Napi::Value::From(env, p_u16_DstRef)),
    StaticValue("SrcTSap", Napi::Value::From(env, p_u16_SrcTSap)),
    StaticValue("PDURequest", Napi::Value::From(env, p_i32_PDURequest)),
    StaticValue("MaxClients", Napi::Value::From(env, p_i32_MaxClients)),
    StaticValue("BSendTimeout", Napi::Value::From(env, p_i32_BSendTimeout)),
    StaticValue("BRecvTimeout", Napi::Value::From(env, p_i32_BRecvTimeout)),
    StaticValue("RecoveryTime", Napi::Value::From(env, p_u32_RecoveryTime)),
    StaticValue("KeepAliveTime", Napi::Value::From(env, p_u32_KeepAliveTime))
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
  exports.Set("S7Client", func);
  return exports;
}

S7Client::S7Client(const Napi::CallbackInfo &info) : Napi::ObjectWrap<S7Client>(info) {
  Napi::Env env = info.Env();

  snap7Client = new TS7Client();
  uv_mutex_init(&mutex);
}

S7Client::~S7Client() {
  snap7Client->Disconnect();
  delete snap7Client;
  constructor.Reset();
  uv_mutex_destroy(&mutex);
}

int S7Client::GetByteCountFromWordLen(int WordLen) {
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

void S7Client::FreeCallback(char *data, void *hint) {
  delete[] data;
}

void S7Client::FreeCallbackSZL(char *data, void *hint) {
  delete reinterpret_cast<PS7SZL>(data);
}

// Control functions
Napi::Value S7Client::Connect(const Napi::CallbackInfo &info) {
  if (!info[0].IsFunction()) {
    int ret = this->snap7Client->Connect();
    return Napi::Boolean::New(info.Env(), ret == 0);
  } else {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, this, CONNECT);
    return info.Env().Undefined();
  }
}

Napi::Value S7Client::ConnectTo(const Napi::CallbackInfo &info) {
  if (info.Length() < 3) {
    Napi::TypeError::New(info.Env(), "Wrong number of arguments").ThrowAsJavaScriptException();
  }

  if (!info[0].IsString() || !info[1].IsNumber() || !info[2].IsNumber()) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  std::string remAddress = info[0].As<Napi::String>().Utf8Value();
  if (!info[3].IsFunction()) {
    int ret = this->snap7Client->ConnectTo(
        remAddress.c_str()
      , info[1].As<Napi::Number>().Int32Value()
      , info[2].As<Napi::Number>().Int32Value());
    return Napi::Boolean::New(info.Env(), ret == 0);
  } else {
    Napi::Function callback = info[3].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, CONNECTTO
      , remAddress, info[2].As<Napi::Number>().Int32Value());
    return info.Env().Undefined();
  }
}

Napi::Value S7Client::SetConnectionParams(const Napi::CallbackInfo &info) {
  if (!info[0].IsString() || !info[1].IsNumber() ||
      !info[2].IsNumber()) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  std::string remAddress = info[0].As<Napi::String>().Utf8Value();
  word LocalTSAP = info[1].As<Napi::Number>().Uint32Value();
  word RemoteTSAP = info[2].As<Napi::Number>().Uint32Value();

  int ret = this->snap7Client->SetConnectionParams(
      remAddress.c_str()
    , LocalTSAP
    , RemoteTSAP);
  return Napi::Boolean::New(info.Env(), ret == 0);
}

Napi::Value S7Client::SetConnectionType(const Napi::CallbackInfo &info) {
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  word type = info[0].As<Napi::Number>().Uint32Value();;

  int  ret = this->snap7Client->SetConnectionType(type);
  return Napi::Boolean::New(info.Env(), ret == 0);
}

Napi::Value S7Client::Disconnect(const Napi::CallbackInfo &info) {
  int ret = this->snap7Client->Disconnect();
  return Napi::Boolean::New(info.Env(), ret == 0);
}

Napi::Value S7Client::GetParam(const Napi::CallbackInfo &info) {
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  int pData;
  int returnValue = this->snap7Client->GetParam(info[0].As<Napi::Number>().Int32Value()
    , &pData);

  if (returnValue == 0) {
    return Napi::Number::New(info.Env(), pData);
  } else {
    return Napi::Number::New(info.Env(), returnValue);
  }
}

Napi::Value S7Client::SetParam(const Napi::CallbackInfo &info) {
  if (!(info[0].IsNumber() || info[1].IsNumber())) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  int pData = info[1].As<Napi::Number>().Int32Value();
  int ret = this->snap7Client->SetParam(info[0].As<Napi::Number>().Int32Value(), &pData);
  return Napi::Boolean::New(info.Env(), ret == 0);
}

// Data I/O Main functions
void IOWorker::Execute() {
  uv_mutex_lock(this->Receiver());

  switch (caller) {
  case CONNECTTO:
      returnValue = this->snap7Client->ConnectTo(
          **static_cast<Nan::Utf8String*>(pData), int1, int2);
      break;

  case CONNECT:
      returnValue = this->snap7Client->Connect();
      break;

  case READAREA:
      returnValue = this->snap7Client->ReadArea(int1, int2, int3, int4
        , int5, pData);
      break;

  case WRITEAREA:
      returnValue = this->snap7Client->WriteArea(int1, int2, int3, int4
        , int5, pData);
      break;

  case READMULTI:
      returnValue = this->snap7Client->ReadMultiVars(
          static_cast<PS7DataItem>(pData), int1);
      break;

  case WRITEMULTI:
      returnValue = this->snap7Client->WriteMultiVars(
          static_cast<PS7DataItem>(pData), int1);
      break;

  case PLCSTATUS:
      returnValue = this->snap7Client->PlcStatus();
      if ((returnValue == S7CpuStatusUnknown) ||
          (returnValue == S7CpuStatusStop) ||
          (returnValue == S7CpuStatusRun)) {
        int1 = returnValue;
        returnValue = 0;
      }
      break;

  case CLEARSESSIONPW:
      returnValue = this->snap7Client->ClearSessionPassword();
      break;

  case SETSESSIONPW:
      returnValue = this->snap7Client->SetSessionPassword(
          **static_cast<Nan::Utf8String*>(pData));
      break;

  case GETPROTECTION:
      returnValue = this->snap7Client->GetProtection(
          static_cast<PS7Protection>(pData));
      break;

  case PLCSTOP:
      returnValue = this->snap7Client->PlcStop();
      break;

  case PLCCOLDSTART:
      returnValue = this->snap7Client->PlcColdStart();
      break;

  case PLCHOTSTART:
      returnValue = this->snap7Client->PlcHotStart();
      break;

  case GETCPINFO:
      returnValue = this->snap7Client->GetCpInfo(
          static_cast<PS7CpInfo>(pData));
      break;

  case GETCPUINFO:
      returnValue = this->snap7Client->GetCpuInfo(
          static_cast<PS7CpuInfo>(pData));
      break;

  case GETORDERCODE:
      returnValue = this->snap7Client->GetOrderCode(
          static_cast<PS7OrderCode>(pData));
      break;

  case SETPLCSYSTEMDATETIME:
      returnValue = this->snap7Client->SetPlcSystemDateTime();
      break;

  case GETPLCDATETIME:
      returnValue = this->snap7Client->GetPlcDateTime(
          static_cast<tm*>(pData));
      break;

  case SETPLCDATETIME:
      returnValue = this->snap7Client->SetPlcDateTime(
          static_cast<tm*>(pData));
      break;

  case COMPRESS:
      returnValue = this->snap7Client->Compress(int1);
      break;

  case COPYRAMTOROM:
      returnValue = this->snap7Client->CopyRamToRom(int1);
      break;

  case DBFILL:
      returnValue = this->snap7Client->DBFill(int1, int2);
      break;

  case DBGET:
      returnValue = this->snap7Client->DBGet(int1, pData, &int2);
      break;

  case DELETEBLOCK:
      returnValue = this->snap7Client->Delete(int1, int2);
      break;

  case DOWNLOAD:
      returnValue = this->snap7Client->Download(int1, pData, int2);
      break;

  case FULLUPLOAD:
      returnValue = this->snap7Client->FullUpload(int1, int2, pData, &int3);
      break;

  case UPLOAD:
      returnValue = this->snap7Client->Upload(int1, int2, pData, &int3);
      break;

  case LISTBLOCKSOFTYPE:
      returnValue = this->snap7Client->ListBlocksOfType(int1
        , static_cast<PS7BlocksOfType>(pData), &int2);
      break;

  case GETAGBLOCKINFO:
      returnValue = this->snap7Client->GetAgBlockInfo(int1, int2
        , static_cast<PS7BlockInfo>(pData));
      break;

  case LISTBLOCKS:
      returnValue = this->snap7Client->ListBlocks(
          static_cast<PS7BlocksList>(pData));
      break;

  case READSZLLIST:
      returnValue = this->snap7Client->ReadSZLList(
          static_cast<PS7SZLList>(pData), &int1);
      break;

  case READSZL:
      returnValue = this->snap7Client->ReadSZL(int1, int2
        , static_cast<PS7SZL>(pData), &int3);
      break;
  }

  uv_mutex_unlock(&this->mutex);
}

void IOWorker::OnOK() {
  Napi::HandleScope scope(Env());

  Napi::Value argv1[1];
  Napi::Value argv2[2];

  if (returnValue == 0) {
    argv2[0] = argv1[0] = Env().Null();
  } else {
    argv2[0] = argv1[0] = Napi::Number::New(Env(), returnValue);
  }

  switch (caller) {
  case CONNECTTO:
  case SETSESSIONPW:
      delete static_cast<Nan::Utf8String*>(pData);
      callback->Call(1, argv1, async_resource);
      break;

  case CONNECT:
  case WRITEAREA:
  case CLEARSESSIONPW:
  case PLCSTOP:
  case PLCCOLDSTART:
  case PLCHOTSTART:
  case SETPLCSYSTEMDATETIME:
  case COPYRAMTOROM:
  case COMPRESS:
  case DBFILL:
  case DELETEBLOCK:
  case DOWNLOAD:
      callback->Call(1, argv1, async_resource);
      break;

  case READAREA:
    if (returnValue == 0) {
      argv2[1] = Nan::NewBuffer(
          static_cast<char*>(pData)
        , int4 * this->s7client->GetByteCountFromWordLen(int5)
        , S7Client::FreeCallback,
        NULL);
    } else {
      argv2[1] = Env().Null();
      delete[] static_cast<char*>(pData);
    }
    callback->Call(2, argv2, async_resource);
    break;

  case READMULTI:
      if (returnValue == 0) {
        argv2[1] = this->s7client->S7DataItemToArray(static_cast<PS7DataItem>(pData)
          , int1, true);
      } else {
        for (int i = 0; i < int1; i++) {
          delete[] static_cast<char*>(static_cast<PS7DataItem>(pData)[i].pdata);
        }
        delete[] static_cast<PS7DataItem>(pData);
        argv2[1] = Env().Null();
      }
      callback->Call(2, argv2, async_resource);
      break;

  case WRITEMULTI:
      if (returnValue == 0) {
        argv2[1] = this->s7client->S7DataItemToArray(static_cast<PS7DataItem>(pData)
          , int1, false);
      } else {
        delete[] static_cast<PS7DataItem>(pData);
        argv2[1] = Env().Null();
      }
      callback->Call(2, argv2, async_resource);
      break;

  case GETPROTECTION:
      if (returnValue == 0) {
        argv2[1] = this->s7client->S7ProtectionToObject(
          static_cast<PS7Protection>(pData));
      } else {
        argv2[1] = Env().Null();
      }
      delete static_cast<PS7Protection>(pData);
      callback->Call(2, argv2, async_resource);
      break;

  case GETCPINFO:
      if (returnValue == 0) {
        argv2[1] = this->s7client->S7CpInfoToObject(
          static_cast<PS7CpInfo>(pData));
      } else {
        argv2[1] = Env().Null();
      }
      delete static_cast<PS7CpInfo>(pData);
      callback->Call(2, argv2, async_resource);
      break;

  case GETCPUINFO:
      if (returnValue == 0) {
        argv2[1] = this->s7client->S7CpuInfoToObject(
          static_cast<PS7CpuInfo>(pData));
      } else {
        argv2[1] = Env().Null();
      }
      delete static_cast<PS7CpuInfo>(pData);
      callback->Call(2, argv2, async_resource);
      break;

  case GETORDERCODE:
      if (returnValue == 0) {
        argv2[1] = this->s7client->S7OrderCodeToObject(
          static_cast<PS7OrderCode>(pData));
      } else {
        argv2[1] = Env().Null();
      }
      delete static_cast<PS7OrderCode>(pData);
      callback->Call(2, argv2, async_resource);
      break;

  case GETPLCDATETIME:
      if (returnValue == 0) {
        double timestamp = static_cast<double>(mktime(static_cast<tm*>(pData)));
        argv2[1] = Nan::New<v8::Date>(timestamp * 1000);
      } else {
        argv2[1] = Env().Null();
      }
      delete static_cast<tm*>(pData);
      callback->Call(2, argv2, async_resource);
      break;

  case SETPLCDATETIME:
      delete static_cast<tm*>(pData);
      callback->Call(1, argv1, async_resource);
      break;

  case PLCSTATUS:
      if (returnValue == 0) {
        argv2[1] = Napi::Number::New(Env(), int1);
      } else {
        argv2[1] = Env().Null();
      }
      callback->Call(2, argv2, async_resource);
      break;

  case DBGET:
      if (returnValue == 0) {
        argv2[1] = Nan::NewBuffer(
            static_cast<char*>(pData)
          , int2
          , S7Client::FreeCallback
          , NULL);
      } else {
        argv2[1] = Env().Null();
        delete[] static_cast<char*>(pData);
      }
      callback->Call(2, argv2, async_resource);
      break;

  case FULLUPLOAD:
  case UPLOAD:
      if (returnValue == 0) {
        argv2[1] = Nan::NewBuffer(
            static_cast<char*>(pData)
          , int3
          , S7Client::FreeCallback
          , NULL);
      } else {
        argv2[1] = Env().Null();
        delete[] static_cast<char*>(pData);
      }
      callback->Call(2, argv2, async_resource);
      break;

  case LISTBLOCKSOFTYPE:
      if (returnValue == 0) {
        argv2[1] = this->s7client->S7BlocksOfTypeToArray(
            static_cast<PS7BlocksOfType>(pData), int2);
      } else {
        argv2[1] = Env().Null();
      }
      delete[] static_cast<PS7BlocksOfType>(pData);
      callback->Call(2, argv2, async_resource);
      break;

  case GETAGBLOCKINFO:
      if (returnValue == 0) {
        Napi::Object block_info = this->s7client->S7BlockInfoToObject(
            static_cast<PS7BlockInfo>(pData));
        argv2[1] = block_info;
      } else {
        argv2[1] = Env().Null();
      }
      delete static_cast<PS7BlockInfo>(pData);
      callback->Call(2, argv2, async_resource);
      break;

  case LISTBLOCKS:
      if (returnValue == 0) {
        Napi::Object blocks_list = this->s7client->S7BlocksListToObject(
            static_cast<PS7BlocksList>(pData));
        argv2[1] = blocks_list;
      } else {
        argv2[1] = Env().Null();
      }
      delete static_cast<PS7BlocksList>(pData);
      callback->Call(2, argv2, async_resource);
      break;

  case READSZLLIST:
      if (returnValue == 0) {
        Napi::Array szl_list = this->S7SZLListToArray(
            static_cast<PS7SZLList>(pData), int1);
        argv2[1] = szl_list;
      } else {
        argv2[1] = Env().Null();
      }
      delete static_cast<PS7SZLList>(pData);
      callback->Call(2, argv2, async_resource);
      break;

  case READSZL:
      if (returnValue == 0) {
        argv2[1] = Nan::NewBuffer(
            reinterpret_cast<char*>(static_cast<PS7SZL>(pData))
          , int3
          , S7Client::FreeCallbackSZL
          , NULL);
      } else {
        argv2[1] = Env().Null();
        delete static_cast<PS7SZL>(pData);
      }
      callback->Call(2, argv2, async_resource);
      break;
  }
}

Napi::Value S7Client::ReadArea(const Napi::CallbackInfo &info) {
  

  if (info.Length() < 5)
    Napi::TypeError::New(info.Env(), "Wrong number of Arguments").ThrowAsJavaScriptException();

  if (!info[0].IsNumber() || !info[1].IsNumber() ||
      !info[2].IsNumber() || !info[3].IsNumber() ||
      !info[4].IsNumber())
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();

  int amount = info[3].As<Napi::Number>().Int32Value();
  int byteCount = this->GetByteCountFromWordLen(info[4].As<Napi::Number>().Int32Value());
  int size = amount * byteCount;
  char *bufferData = new char[size];

  if (!info[5].IsFunction()) {
    int returnValue = this->snap7Client->ReadArea(
      info[0].As<Napi::Number>().Int32Value()
		  , info[1].As<Napi::Number>().Int32Value()
      , info[2].As<Napi::Number>().Int32Value()
      , info[3].As<Napi::Number>().Int32Value()
      , info[4].As<Napi::Number>().Int32Value()
      , bufferData);

    if (returnValue == 0) {
      Napi::Object ret = Nan::NewBuffer(
          bufferData
        , size
        , S7Client::FreeCallback
        , NULL);
      return ret;
    } else {
      delete[] bufferData;
      return Napi::Boolean::New(info.Env(), false);
    }

  } else {
    Napi::Function callback = info[5].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, READAREA
      , bufferData, info[1]
      , info[4]);
    return info.Env().Undefined();
  }
}

Napi::Value S7Client::WriteArea(const Napi::CallbackInfo &info) {
  if (info.Length() < 6)
    Napi::TypeError::New(info.Env(), "Wrong number of Arguments").ThrowAsJavaScriptException();

  if (!info[0].IsNumber() || !info[1].IsNumber() ||
      !info[2].IsNumber() || !info[3].IsNumber() ||
      !info[4].IsNumber() || !info[5].IsBuffer())
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();

  if (!info[6].IsFunction()) {
    return Napi::Boolean::New(info.Env(), 
      this->snap7Client->WriteArea(
        info[0].As<Napi::Number>().Int32Value()
        , info[1].As<Napi::Number>().Int32Value()
        , info[2].As<Napi::Number>().Int32Value()
        , info[3].As<Napi::Number>().Int32Value()
        , info[4].As<Napi::Number>().Int32Value()
        , info[5].As<Napi::Buffer<uint16_t>>().Data()) == 0);
  } else {
    Napi::Function callback = info[6].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, WRITEAREA
      , info[5].As<Napi::Buffer<uint16_t>>().Data()
      , info[0].As<Napi::Number>().Int32Value()
      , info[1].As<Napi::Number>().Int32Value()
      , info[2].As<Napi::Number>().Int32Value()
      , info[3].As<Napi::Number>().Int32Value()
      , info[4].As<Napi::Number>().Int32Value());
    return info.Env().Undefined();
  }
}

Napi::Value S7Client::ReadMultiVars(const Napi::CallbackInfo &info) {
  

  if (info.Length() < 1) {
    Napi::TypeError::New(info.Env(), "Wrong number of arguments").ThrowAsJavaScriptException();
  }

  if (!info[0].IsArray()) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  Napi::Array data_arr = info[0].As<Napi::Array>();
  int len = data_arr.Length();
  if (len == 0) {
    Napi::TypeError::New(info.Env(), "Array needs at least 1 item").ThrowAsJavaScriptException();
  } else if (len > MaxVars) {
    std::stringstream err;
    err << "Array exceeds max variables (" << MaxVars
        << ") that can be transferred with ReadMultiVars()";
    Napi::TypeError::New(info.Env(), err.str().c_str()).ThrowAsJavaScriptException();
  }

  for (int i = 0; i < len; i++) {
    if (!data_arr[i].IsObject()) {
      Napi::TypeError::New(info.Env(), "Wrong argument structure").ThrowAsJavaScriptException();
    } else {
      Napi::Object data_obj = data_arr[i];
      if (!data_obj.Has("Area") ||
          !data_obj.Has("WordLen") ||
          !data_obj.Has("Start") ||
          !data_obj.Has("Amount")) {
        Napi::TypeError::New(info.Env(), "Wrong argument structure").ThrowAsJavaScriptException();
      } else if (!data_obj.Get("Area").IsNumber() ||
                 !data_obj.Get("WordLen").IsNumber() ||
                 !data_obj.Get("Start").IsNumber() ||
                 !data_obj.Get("Amount").IsNumber()){
        Napi::TypeError::New(info.Env(), "Wrong argument structure").ThrowAsJavaScriptException();
      } else if (data_obj.Get("Area").As<Napi::Number>().Int32Value == S7AreaDB) {
        if (!data_obj.Has("DBNumber")) {
          Napi::TypeError::New(info.Env(), "Wrong argument structure").ThrowAsJavaScriptException();
        }
      } else {
        data_obj.Set("DBNumber", Napi::Number::New(info.Env(), 0));
      }
    }
  }

  PS7DataItem Items = new TS7DataItem[len];
  Napi::Object data_obj;
  int byteCount, size;

  for (int i = 0; i < len; i++) {
    data_obj = data_arr[i];

    Items[i].Area = data_obj.Get("Area").As<Napi::Number>().Int32Value();
    Items[i].WordLen = data_obj.Get("WordLen").As<Napi::Number>().Int32Value();
    Items[i].DBNumber = data_obj.Get("DBNumber").As<Napi::Number>().Int32Value();
    Items[i].Start = data_obj.Get("Start").As<Napi::Number>().Int32Value();
    Items[i].Amount = data_obj.Get("Amount").As<Napi::Number>().Int32Value();

    byteCount = this->GetByteCountFromWordLen(Items[i].WordLen);
    size = Items[i].Amount * byteCount;
    Items[i].pdata = new char[size];
  }

  if (!info[1].IsFunction()) {
    int returnValue = this->snap7Client->ReadMultiVars(Items, len);

    if (returnValue == 0) {
      return this->S7DataItemToArray(Items, len, true);
    } else {
      for (int i = 0; i < len; i++) {
        delete[] static_cast<char*>(Items[i].pdata);
      }
      delete[] Items;
      return Napi::Boolean::New(info.Env(), false);
    }
  } else {
    Napi::Function callback = info[1].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, READMULTI
      , Items, len);
    worker->Queue();
    return info.Env().Undefined();
  }
}

Napi::Array S7Client::S7DataItemToArray(
    PS7DataItem Items
  , int len
  , bool readMulti
) {
  Napi::EscapableHandleScope scope(this->Env());

  Napi::Array res_arr = Napi::Array::New(this->Env(), len);
  Napi::Object res_obj;
  int byteCount, size;

  for (int i = 0; i < len; i++) {
    res_obj = Napi::Object::New(this->Env());
    res_obj.Set("Result", Napi::Number::New(this->Env(), Items[i].Result));

    if (readMulti == true) {
      if (Items[i].Result == 0) {
        byteCount = S7Client::GetByteCountFromWordLen(Items[i].WordLen);
        size = byteCount * Items[i].Amount;
        res_obj.Set("Data"
          , Napi::Buffer<char>::New(this->Env,
              static_cast<char*>(Items[i].pdata)
            , size
            , [](Napi::Env /*env*/, char* finalizeData) {
              delete[] finalizeData; }));
      } else {
        delete[] static_cast<char*>(Items[i].pdata);
        res_obj.Set("Data", this->Env().Null());
      }
    }
    res_arr.Set(i, res_obj);
  }
  delete[] Items;

  return scope.Escape(res_arr);
}

Napi::Value S7Client::WriteMultiVars(const Napi::CallbackInfo &info) {
  

  if (info.Length() < 1) {
    Napi::TypeError::New(info.Env(), "Wrong number of arguments").ThrowAsJavaScriptException();
  }

  if (!info[0].IsArray()) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  Napi::Array data_arr = info[0].As<Napi::Array>();
  int len = data_arr.Length();
  if (len == 0) {
    Napi::TypeError::New(info.Env(), "Array needs at least 1 item").ThrowAsJavaScriptException();
  } else if (len > MaxVars) {
    std::stringstream err;
    err << "Array exceeds max variables (" << MaxVars
        << ") that can be transferred with WriteMultiVars()";
    Napi::TypeError::New(info.Env(), err.str().c_str()).ThrowAsJavaScriptException();
  }

  for (int i = 0; i < len; i++) {
    if (!data_arr[i].IsObject()) {
      Napi::TypeError::New(info.Env(), "Wrong argument structure").ThrowAsJavaScriptException();
    } else {
      Napi::Object data_obj = data_arr[i];
      if (!data_obj.Has("Area") ||
          !data_obj.Has("WordLen") ||
          !data_obj.Has("Start") ||
          !data_obj.Has("Amount") ||
          !data_obj.Has("Data")) {
        Napi::TypeError::New(info.Env(), "Wrong argument structure").ThrowAsJavaScriptException();
      } else if (!data_obj.Get("Area").IsNumber() ||
                 !data_obj.Get("WordLen").IsNumber() ||
                 !data_obj.Get("Start").IsNumber() ||
                 !data_obj.Get("Amount").IsNumber() ||
                 !data_obj.Get("Data").IsBuffer()) {
        Napi::TypeError::New(info.Env(), "Wrong argument structure").ThrowAsJavaScriptException();
      } else if (data_obj.Get("Area").As<Napi::Number>().Int32Value == S7AreaDB) {
        if (!data_obj.Has("DBNumber")) {
          Napi::TypeError::New(info.Env(), "Wrong argument structure").ThrowAsJavaScriptException();
        }
      } else {
        data_obj.Set("DBNumber", Napi::Number::New(info.Env(), 0));
      }
    }
  }

  PS7DataItem Items = new TS7DataItem[len];
  Napi::Object data_obj;
  for (int i = 0; i < len; i++) {
    data_obj = data_arr[i];

    Items[i].Area = data_obj.Get("Area").As<Napi::Number>().Int32Value;
    Items[i].WordLen = data_obj.Get("WordLen").As<Napi::Number>().Int32Value;
    Items[i].DBNumber = data_obj.Get("DBNumber").As<Napi::Number>().Int32Value;
    Items[i].Start = data_obj.Get("Start").As<Napi::Number>().Int32Value;
    Items[i].Amount = data_obj.Get("Amount").As<Napi::Number>().Int32Value;
    Items[i].pdata = data_obj.Get("Data").As<Napi::Buffer<char>>().Data();
  }

  if (!info[1].IsFunction()) {
    int returnValue = this->snap7Client->WriteMultiVars(Items, len);

    if (returnValue == 0) {
      return this->S7DataItemToArray(Items, len, false);
    } else {
      delete[] Items;
      return Napi::Boolean::New(info.Env(), false);
    }
  } else {
    Napi::Function callback = info[1].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, WRITEMULTI
      , Items, len));
    return info.Env().Undefined();
  }
}

// Directory functions
Napi::Value S7Client::ListBlocks(const Napi::CallbackInfo &info) {
  

  PS7BlocksList BlocksList = new TS7BlocksList;
  if (!info[0].IsFunction()) {
    int returnValue = this->snap7Client->ListBlocks(BlocksList);

    Napi::Object blocks_list = this->S7BlocksListToObject(
        BlocksList);

    if (returnValue == 0) {
      delete BlocksList;
      return blocks_list;
    } else {
      delete BlocksList;
      return Napi::Boolean::New(info.Env(), false);
    }
  } else {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, LISTBLOCKS
      , BlocksList));
    return info.Env().Undefined();
  }
}

Napi::Object S7Client::S7BlocksListToObject(
    PS7BlocksList BlocksList
) {
  Napi::EscapableHandleScope scope(this->Env());

  Napi::Object blocks_list = Napi::Object::New(this->Env());
  blocks_list.Set("OBCount", Napi::Number::New(this->Env(), BlocksList->OBCount));
  blocks_list.Set("FBCount", Napi::Number::New(this->Env(), BlocksList->FBCount));
  blocks_list.Set("FCCount", Napi::Number::New(this->Env(), BlocksList->FCCount));
  blocks_list.Set("SFBCount", Napi::Number::New(this->Env(), BlocksList->SFBCount));
  blocks_list.Set("SFCCount", Napi::Number::New(this->Env(), BlocksList->SFCCount));
  blocks_list.Set("DBCount", Napi::Number::New(this->Env(), BlocksList->DBCount));
  blocks_list.Set("SDBCount", Napi::Number::New(this->Env(), BlocksList->SDBCount));

  return scope.Escape(blocks_list);
}


Napi::Value S7Client::GetAgBlockInfo(const Napi::CallbackInfo &info) {
  

  if (!info[0].IsInt32() || !info[1].IsInt32()) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  PS7BlockInfo BlockInfo = new TS7BlockInfo;
  if (!info[2].IsFunction()) {
    int returnValue = this->snap7Client->GetAgBlockInfo(
		info[1] BlockInfo);

    if (returnValue == 0) {
      Napi::Object block_info = this->S7BlockInfoToObject(
          BlockInfo);

      delete BlockInfo;
      return block_info;
    } else {
      delete BlockInfo;
      return Napi::Boolean::New(info.Env(), false);
    }
  } else {
    Napi::Function callback = info[2].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, GETAGBLOCKINFO
      , BlockInfo, info[1]);
    return info.Env().Undefined();
  }
}

Napi::Value S7Client::GetPgBlockInfo(const Napi::CallbackInfo &info) {
  

  if (!info[0].IsBuffer()) {
    Napi::TypeError::New(info.Env(), "Argument should be a Buffer").ThrowAsJavaScriptException();
  }

  PS7BlockInfo BlockInfo = new TS7BlockInfo;

  int returnValue = this->snap7Client->GetPgBlockInfo(
    node::Buffer::Data(info[0].As<Napi::Object>()), BlockInfo
    , static_cast<int>(node::Buffer::Length(info[0].As<Napi::Object>())));

  if (returnValue == 0) {
    Napi::Object block_info = this->S7BlockInfoToObject(BlockInfo);
    delete BlockInfo;
    return block_info;
  } else {
    delete BlockInfo;
    return Napi::Boolean::New(info.Env(), false);
  }
}

Napi::Object S7Client::S7BlockInfoToObject(PS7BlockInfo BlockInfo) {
  Nan::EscapableHandleScope scope;

  Napi::Object block_info = Nan::New<Napi::Object>();
  Nan::Set(block_info, Nan::New<v8::String>("BlkType")
    , Nan::New<Napi::Number>(BlockInfo->BlkType));
  Nan::Set(block_info, Nan::New<v8::String>("BlkNumber")
    , Nan::New<Napi::Number>(BlockInfo->BlkNumber));
  Nan::Set(block_info, Nan::New<v8::String>("BlkLang")
    , Nan::New<Napi::Number>(BlockInfo->BlkLang));
  Nan::Set(block_info, Nan::New<v8::String>("BlkFlags")
    , Nan::New<Napi::Number>(BlockInfo->BlkFlags));
  Nan::Set(block_info, Nan::New<v8::String>("MC7Size")
    , Nan::New<Napi::Number>(BlockInfo->MC7Size));
  Nan::Set(block_info, Nan::New<v8::String>("LoadSize")
    , Nan::New<Napi::Number>(BlockInfo->LoadSize));
  Nan::Set(block_info, Nan::New<v8::String>("LocalData")
    , Nan::New<Napi::Number>(BlockInfo->LocalData));
  Nan::Set(block_info, Nan::New<v8::String>("SBBLength")
    , Nan::New<Napi::Number>(BlockInfo->SBBLength));
  Nan::Set(block_info, Nan::New<v8::String>("CheckSum")
    , Nan::New<Napi::Number>(BlockInfo->CheckSum));
  Nan::Set(block_info, Nan::New<v8::String>("Version")
    , Nan::New<Napi::Number>(BlockInfo->Version));
  Nan::Set(block_info, Nan::New<v8::String>("CodeDate")
    , Nan::New<v8::String>(BlockInfo->CodeDate));
  Nan::Set(block_info, Nan::New<v8::String>("IntfDate")
    , Nan::New<v8::String>(BlockInfo->IntfDate));
  Nan::Set(block_info, Nan::New<v8::String>("Author")
    , Nan::New<v8::String>(BlockInfo->Author));
  Nan::Set(block_info, Nan::New<v8::String>("Family")
    , Nan::New<v8::String>(BlockInfo->Family));
  Nan::Set(block_info, Nan::New<v8::String>("Header")
    , Nan::New<v8::String>(BlockInfo->Header));

  return scope.Escape(block_info);
}

Napi::Value S7Client::ListBlocksOfType(const Napi::CallbackInfo &info) {
  

  if (!info[0].IsInt32()) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  int BlockNum = sizeof(TS7BlocksOfType) / sizeof(PS7BlocksOfType);
  PS7BlocksOfType BlockList = new TS7BlocksOfType[BlockNum];
  if (!info[1].IsFunction()) {
    int returnValue = this->snap7Client->ListBlocksOfType(
		info[0], BlockList, &BlockNum);

    if (returnValue == 0) {
      Napi::Array block_list = this->S7BlocksOfTypeToArray(
          BlockList, BlockNum);
      delete[] BlockList;
      return block_list;
    } else {
      delete[] BlockList;
      return Napi::Boolean::New(info.Env(), false);
    }
  } else {
    Napi::Function callback = info[1].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, LISTBLOCKSOFTYPE
      , BlockList, info[0], BlockNum));
    return info.Env().Undefined();
  }
}

Napi::Array S7Client::S7BlocksOfTypeToArray(
    PS7BlocksOfType BlocksList
  , int count
) {
  Nan::EscapableHandleScope scope;

  Napi::Array block_list = Nan::New<v8::Array>(count);
  for (int i = 0; i < count; i++) {
    Nan::Set(block_list, i, Napi::Number::New(info.Env(), (*BlocksList)[i]));
  }

  return scope.Escape(block_list);
}

// Blocks functions
Napi::Value S7Client::Upload(const Napi::CallbackInfo &info) {
  

  if (!info[0].IsInt32() || !info[1].IsInt32() || !info[2].IsInt32()) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  int size = info[2].As<Napi::Number>().Int32Value();
  char *bufferData = new char[size];

  if (!info[3].IsFunction()) {
    int returnValue = this->snap7Client->Upload(
      info[0].As<Napi::Number>().Int32Value()
		  , info[1].As<Napi::Number>().Int32Value(), bufferData, &size);

    if (returnValue == 0) {
      Napi::Object ret_buf;
      ret_buf = Nan::NewBuffer(
          bufferData
        , size
        , S7Client::FreeCallback
        , NULL);
      return ret_buf;
    } else {
      delete[] bufferData;
      return Napi::Boolean::New(info.Env(), false);
    }
  } else {
    Napi::Function callback = info[3].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, UPLOAD
      , bufferData, info[1], size));
    return info.Env().Undefined();
  }
}

Napi::Value S7Client::FullUpload(const Napi::CallbackInfo &info) {
  

  if (!info[0].IsInt32() || !info[1].IsInt32() || !info[2].IsInt32()) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  int size = info[2].As<Napi::Number>().Int32Value();
  char *bufferData = new char[size];

  if (!info[3].IsFunction()) {
    int returnValue = this->snap7Client->FullUpload(
      info[0].As<Napi::Number>().Int32Value()
		  , info[1].As<Napi::Number>().Int32Value(), bufferData, &size);

    if (returnValue == 0) {
      Napi::Object ret_buf;
      ret_buf = Nan::NewBuffer(
          bufferData
        , size
        , S7Client::FreeCallback
        , NULL);
      return ret_buf;
    } else {
      delete[] bufferData;
      return Napi::Boolean::New(info.Env(), false);
    }
  } else {
    Napi::Function callback = info[3].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, FULLUPLOAD
      , bufferData, info[1] size));
    return info.Env().Undefined();
  }
}

Napi::Value S7Client::Download(const Napi::CallbackInfo &info) {
  

  if (!info[0].IsInt32() || !node::Buffer::HasInstance(info[1])) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  if (!info[2].IsFunction()) {
    return Napi::Boolean::New(info.Env(), this->snap7Client->Download(
      info[0].As<Napi::Number>().Int32Value()
      , static_cast<int>(node::Buffer::Length(info[1].As<Napi::Object>())) == 0));
  } else {
    Napi::Function callback = info[2].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, DOWNLOAD
      , node::Buffer::Data(info[1].As<Napi::Object>()), info[0]
      , static_cast<int>(node::Buffer::Length(info[1].As<Napi::Object>()))));
    return info.Env().Undefined();
  }
}

Napi::Value S7Client::Delete(const Napi::CallbackInfo &info) {
  

  if (!info[0].IsInt32() || !info[1].IsInt32()) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  if (!info[2].IsFunction()) {
    return Napi::Boolean::New(info.Env(), this->snap7Client->Delete(
      info[0].As<Napi::Number>().Int32Value()
		  , info[1].As<Napi::Number>().Int32Value()) == 0);
  } else {
    Napi::Function callback = info[2].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, DELETEBLOCK
      , info[1]);
    return info.Env().Undefined();
  }
}

Napi::Value S7Client::DBGet(const Napi::CallbackInfo &info) {
  

  if (!info[0].IsInt32()) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  int size = 65536;
  char *bufferData = new char[size];
  if (!info[1].IsFunction()) {
    int returnValue = this->snap7Client->DBGet(
		info[0].As<Napi::Number>().Int32Value(), bufferData, &size);

    if (returnValue == 0) {
      Napi::Object ret_buf;
      ret_buf = Nan::NewBuffer(
          bufferData
        , size
        , S7Client::FreeCallback
        , NULL);
      return ret_buf;
    } else {
      delete[] bufferData;
      return Napi::Boolean::New(info.Env(), false);
    }
  } else {
    Napi::Function callback = info[1].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, DBGET
      , bufferData, info[0].As<Napi::Number>().Int32Value(), size);
    return info.Env().Undefined();
  }
}

Napi::Value S7Client::DBFill(const Napi::CallbackInfo &info) {
  

  if (!info[0].IsInt32() || !(info[1].IsInt32() || info[1].IsString())) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  int fill;
  if (info[1].IsInt32()) {
    fill = info[1].As<Napi::Number>().Int32Value()
  } else {
    Nan::Utf8String fillstr(info[1]);
    fill = static_cast<int>(**fillstr);
  }

  if (!info[2].IsFunction()) {
    return Napi::Boolean::New(info.Env(), this->snap7Client->DBFill(
		info[0].As<Napi::Number>().Int32Value(), fill) == 0);
  } else {
    Napi::Function callback = info[2].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, DBFILL
      , info[0].As<Napi::Number>().Int32Value(), fill));
    return info.Env().Undefined();
  }
}

// Date/Time functions
Napi::Value S7Client::GetPlcDateTime(const Napi::CallbackInfo &info) {
  

  tm *DateTime = new tm;
  if (!info[0].IsFunction()) {
    int returnValue = this->snap7Client->GetPlcDateTime(DateTime);
    double timestamp = static_cast<double>(mktime(DateTime));
    delete DateTime;

    if (returnValue == 0)
      return Nan::New<v8::Date>(timestamp * 1000);
    else
      return Napi::Boolean::New(info.Env(), false);
  } else {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, GETPLCDATETIME
      , DateTime));
    return info.Env().Undefined();
  }
}

Napi::Value S7Client::SetPlcDateTime(const Napi::CallbackInfo &info) {
  

  if (!(info[0].IsObject() || info[0].IsDate())) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  tm *DateTime = new tm;
  if (info[0].IsDate()) {
    v8::Local<v8::Date> date = v8::Local<v8::Date>::Cast(info[0]));
    time_t timestamp = static_cast<time_t>(date)() / 1000);
    *DateTime = *localtime(&timestamp);
  } else {
    Napi::Object date_time = info[0]);
    DateTime->tm_year = date_time,
      Nan::New<v8::String>("year")))() - 1900;
    DateTime->tm_mon = date_time,
      Nan::New<v8::String>("month")))();
    DateTime->tm_mday = date_time,
      Nan::New<v8::String>("day")))();
    DateTime->tm_hour = date_time,
      Nan::New<v8::String>("hours")))();
    DateTime->tm_min = date_time,
      Nan::New<v8::String>("minutes")))();
    DateTime->tm_sec = date_time,
      Nan::New<v8::String>("seconds")))();
  }

  if (!info[1].IsFunction()) {
    v8::Local<v8::Boolean> ret = Napi::Boolean::New(info.Env(), 
      this->snap7Client->SetPlcDateTime(DateTime) == 0);
    delete DateTime;
    return Set(ret);
  } else {
    Napi::Function callback = info[1].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, SETPLCDATETIME
      , DateTime));
    return info.Env().Undefined();
  }
}

Napi::Value S7Client::SetPlcSystemDateTime(const Napi::CallbackInfo &info) {
  

  if (!info[0].IsFunction()) {
    return Set(Napi::Boolean::New(info.Env(), 
      this->snap7Client->SetPlcSystemDateTime() == 0));
  } else {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, SETPLCSYSTEMDATETIME));
    return info.Env().Undefined();
  }
}

// System Info functions
Napi::Value S7Client::GetOrderCode(const Napi::CallbackInfo &info) {
  

  PS7OrderCode OrderCode = new TS7OrderCode;
  if (!info[0].IsFunction()) {
    int returnValue = this->snap7Client->GetOrderCode(OrderCode);

    if (returnValue == 0) {
      Napi::Object order_code = this->S7OrderCodeToObject(OrderCode);
      delete OrderCode;
      return Set(order_code);
    } else {
      delete OrderCode;
      return Set(Napi::Boolean::New(info.Env(), false));
    }
  } else {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, GETORDERCODE
      , OrderCode));
    return info.Env().Undefined();
  }
}

Napi::Object S7Client::S7OrderCodeToObject(PS7OrderCode OrderCode) {
  Nan::EscapableHandleScope scope;

  Napi::Object order_code = Nan::New<Napi::Object>();
  Nan::Set(order_code, Nan::New<v8::String>("Code")
    , Nan::New<v8::String>(OrderCode->Code));
  Nan::Set(order_code, Nan::New<v8::String>("V1")
    , Nan::New<Napi::Number>(OrderCode->V1));
  Nan::Set(order_code, Nan::New<v8::String>("V2")
    , Nan::New<Napi::Number>(OrderCode->V2));
  Nan::Set(order_code, Nan::New<v8::String>("V3")
    , Nan::New<Napi::Number>(OrderCode->V3));

  return scope.Escape(order_code);
}


Napi::Value S7Client::GetCpuInfo(const Napi::CallbackInfo &info) {
  

  PS7CpuInfo CpuInfo = new TS7CpuInfo;
  if (!info[0].IsFunction()) {
    int returnValue = this->snap7Client->GetCpuInfo(CpuInfo);

    if (returnValue == 0) {
      Napi::Object cpu_info = this->S7CpuInfoToObject(CpuInfo);
      delete CpuInfo;
      return Set(cpu_info);
    } else {
      delete CpuInfo;
      return Set(Napi::Boolean::New(info.Env(), false));
    }
  } else {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, GETCPUINFO, CpuInfo));
    return info.Env().Undefined();
  }
}

Napi::Object S7Client::S7CpuInfoToObject(PS7CpuInfo CpuInfo) {
  Nan::EscapableHandleScope scope;

  Napi::Object cpu_info = Nan::New<Napi::Object>();
  Nan::Set(cpu_info, Nan::New<v8::String>("ModuleTypeName")
    , Nan::New<v8::String>(CpuInfo->ModuleTypeName));
  Nan::Set(cpu_info, Nan::New<v8::String>("SerialNumber")
    , Nan::New<v8::String>(CpuInfo->SerialNumber));
  Nan::Set(cpu_info, Nan::New<v8::String>("ASName")
    , Nan::New<v8::String>(CpuInfo->ASName));
  Nan::Set(cpu_info, Nan::New<v8::String>("Copyright")
    , Nan::New<v8::String>(CpuInfo->Copyright));
  Nan::Set(cpu_info, Nan::New<v8::String>("ModuleName")
    , Nan::New<v8::String>(CpuInfo->ModuleName));

  return scope.Escape(cpu_info);
}


Napi::Value S7Client::GetCpInfo(const Napi::CallbackInfo &info) {
  

  PS7CpInfo CpInfo = new TS7CpInfo;
  if (!info[0].IsFunction()) {
    int returnValue = this->snap7Client->GetCpInfo(CpInfo);

    if (returnValue == 0) {
      Napi::Object cp_info = this->S7CpInfoToObject(CpInfo);
      delete CpInfo;
      return Set(cp_info);
    } else {
      delete CpInfo;
      return Set(Napi::Boolean::New(info.Env(), false));
    }
  } else {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, GETCPINFO, CpInfo));
    return info.Env().Undefined();
  }
}

Napi::Object S7Client::S7CpInfoToObject(PS7CpInfo CpInfo) {
  Nan::EscapableHandleScope scope;

  Napi::Object cp_info = Nan::New<Napi::Object>();
  Nan::Set(cp_info, Nan::New<v8::String>("MaxPduLength")
    , Nan::New<Napi::Number>(CpInfo->MaxPduLengt));
  Nan::Set(cp_info, Nan::New<v8::String>("MaxConnections")
    , Nan::New<Napi::Number>(CpInfo->MaxConnections));
  Nan::Set(cp_info, Nan::New<v8::String>("MaxMpiRate")
    , Nan::New<Napi::Number>(CpInfo->MaxMpiRate));
  Nan::Set(cp_info, Nan::New<v8::String>("MaxBusRate")
    , Nan::New<Napi::Number>(CpInfo->MaxBusRate));

  return scope.Escape(cp_info);
}

Napi::Value S7Client::ReadSZL(const Napi::CallbackInfo &info) {
  

  if (!(info[0].IsInt32() || info[1].IsInt32())) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  PS7SZL SZL = new TS7SZL;
  int size = sizeof(TS7SZL);
  if (!info[2].IsFunction()) {
    int returnValue = this->snap7Client->ReadSZL(info[0]
      , info[1] SZL, &size);

    if (returnValue == 0) {
      Napi::Object ret_buf;
      ret_buf = Nan::NewBuffer(
          reinterpret_cast<char*>(SZL)
        , size
        , S7Client::FreeCallbackSZL
        , NULL);

      return Set(ret_buf);
    } else {
      delete SZL;
      return Set(Napi::Boolean::New(info.Env(), false));
    }
  } else {
    Napi::Function callback = info[2].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, READSZL, SZL
      , info[1] size));
    return info.Env().Undefined();
  }
}

Napi::Value S7Client::ReadSZLList(const Napi::CallbackInfo &info) {
  

  PS7SZLList SZLList = new TS7SZLList;
  int size = sizeof(TS7SZLList);
  if (!info[0].IsFunction()) {
    int returnValue = this->snap7Client->ReadSZLList(SZLList, &size);

    if (returnValue == 0) {
      Napi::Array szl_list = this->S7SZLListToArray(SZLList, size);

      delete SZLList;
      return Set(szl_list);
    } else {
      delete SZLList;
      return Set(Napi::Boolean::New(info.Env(), false));
    }
  } else {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, READSZLLIST, SZLList
      , size));
    return info.Env().Undefined();
  }
}

Napi::Array S7Client::S7SZLListToArray(PS7SZLList SZLList, int count) {
  Nan::EscapableHandleScope scope;

  Napi::Array szl_list = Nan::New<v8::Array>(count);
  for (int i = 0; i < count; i++) {
    Nan::Set(szl_list, i, Napi::Number::New(info.Env(), (*SZLList).List[i]));
  }

  return scope.Escape(szl_list);
}

// Control functions
Napi::Value S7Client::PlcHotStart(const Napi::CallbackInfo &info) {
  

  if (!info[0].IsFunction()) {
    return Set(Napi::Boolean::New(info.Env(), 
      this->snap7Client->PlcHotStart() == 0));
  } else {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, PLCHOTSTART));
    return info.Env().Undefined();
  }
}

Napi::Value S7Client::PlcColdStart(const Napi::CallbackInfo &info) {
  

  if (!info[0].IsFunction()) {
    return Set(Napi::Boolean::New(info.Env(), 
      this->snap7Client->PlcColdStart() == 0));
  } else {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, PLCCOLDSTART));
    return info.Env().Undefined();
  }
}

Napi::Value S7Client::PlcStop(const Napi::CallbackInfo &info) {
  

  if (!info[0].IsFunction()) {
    return Set(Napi::Boolean::New(info.Env(), 
      this->snap7Client->PlcStop() == 0));
  } else {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, PLCSTOP));
    return info.Env().Undefined();
  }
}

Napi::Value S7Client::CopyRamToRom(const Napi::CallbackInfo &info) {
  

  if (!info[0].IsInt32()) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  if (!info[1].IsFunction()) {
    return Set(Napi::Boolean::New(info.Env(), 
      this->snap7Client->CopyRamToRom(info[0] == 0));
  } else {
    Napi::Function callback = info[1].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, COPYRAMTOROM
      , info[0]);
    return info.Env().Undefined();
  }
}

Napi::Value S7Client::Compress(const Napi::CallbackInfo &info) {
  

  if (!info[0].IsInt32()) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  if (!info[1].IsFunction()) {
    return Set(Napi::Boolean::New(info.Env(), 
      this->snap7Client->Compress(info[0] == 0));
  } else {
    Napi::Function callback = info[1].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, COMPRESS
      , info[0]);
    return info.Env().Undefined();
  }
}

// Security functions
Napi::Value S7Client::GetProtection(const Napi::CallbackInfo &info) {
  

  PS7Protection S7Protection = new TS7Protection;
  if (!info[0].IsFunction()) {
    int returnValue = this->snap7Client->GetProtection(S7Protection);

    if (returnValue == 0) {
      Napi::Object protection = this->S7ProtectionToObject(
        S7Protection);
      delete S7Protection;
      return Set(protection);
    } else {
      delete S7Protection;
      return Set(Napi::Boolean::New(info.Env(), false));
    }
  } else {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, GETPROTECTION
      , S7Protection));
    return info.Env().Undefined();
  }
}

Napi::Object S7Client::S7ProtectionToObject(
    PS7Protection S7Protection
) {
  Nan::EscapableHandleScope scope;

  Napi::Object protection = Nan::New<Napi::Object>();
  Nan::Set(protection, Nan::New<v8::String>("sch_schal")
    , Nan::New<Napi::Number>(S7Protection->sch_schal));
  Nan::Set(protection, Nan::New<v8::String>("sch_par")
    , Nan::New<Napi::Number>(S7Protection->sch_par));
  Nan::Set(protection, Nan::New<v8::String>("sch_rel")
    , Nan::New<Napi::Number>(S7Protection->sch_rel));
  Nan::Set(protection, Nan::New<v8::String>("bart_sch")
    , Nan::New<Napi::Number>(S7Protection->bart_sch));
  Nan::Set(protection, Nan::New<v8::String>("anl_sch")
    , Nan::New<Napi::Number>(S7Protection->anl_sch));

  return scope.Escape(protection);
}

Napi::Value S7Client::SetSessionPassword(const Napi::CallbackInfo &info) {
  if (!info[0].IsString()) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  Nan::Utf8String *password = new Nan::Utf8String(info[0]);
  if (!info[1].IsFunction()) {
    v8::Local<v8::Boolean> ret = Napi::Boolean::New(info.Env(), 
      this->snap7Client->SetSessionPassword(**password) == 0);
    delete password;
    return Set(ret);
  } else {
    Napi::Function callback = info[1].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, SETSESSIONPW
      , password));
    return info.Env().Undefined();
  }
}

Napi::Value S7Client::ClearSessionPassword(const Napi::CallbackInfo &info) {
  if (!info[0].IsFunction()) {
    return Set(Napi::Boolean::New(info.Env(), 
      this->snap7Client->ClearSessionPassword() == 0));
  } else {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, CLEARSESSIONPW));
    return info.Env().Undefined();
  }
}

// Properties
Napi::Value S7Client::ExecTime(const Napi::CallbackInfo &info) {
  int returnValue = this->snap7Client->ExecTime();
  if (returnValue == errLibInvalidObject) {
    return Set(Napi::Boolean::New(info.Env(), false));
  } else {
    return Set(Napi::Number::New(info.Env(), returnValue));
  }
}

Napi::Value S7Client::LastError(const Napi::CallbackInfo &info) {
  return Set(Napi::Number::New(info.Env(), 
    this->snap7Client->LastError()));
}

Napi::Value S7Client::PDURequested(const Napi::CallbackInfo &info) {
  

  int returnValue = this->snap7Client->PDURequested();
  if (returnValue == 0) {
    return Set(Napi::Boolean::New(info.Env(), false));
  } else {
    return Set(Napi::Number::New(info.Env(), returnValue));
  }
}

Napi::Value S7Client::PDULength(const Napi::CallbackInfo &info) {
  

  int returnValue = this->snap7Client->PDULength();
  if (returnValue == 0) {
    return Set(Napi::Boolean::New(info.Env(), false));
  } else {
    return Set(Napi::Number::New(info.Env(), returnValue));
  }
}

Napi::Value S7Client::PlcStatus(const Napi::CallbackInfo &info) {
  

  if (!info[0].IsFunction()) {
    int returnValue = this->snap7Client->PlcStatus();
    if ((returnValue == S7CpuStatusUnknown) ||
        (returnValue == S7CpuStatusStop) ||
        (returnValue == S7CpuStatusRun)) {
      return Set(Napi::Number::New(info.Env(), returnValue));
    } else {
      return Set(Napi::Boolean::New(info.Env(), false));
    }
  } else {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorker* worker = new IOWorker(this, callback, PLCSTATUS));
    return info.Env().Undefined();
  }
}

Napi::Value S7Client::Connected(const Napi::CallbackInfo &info) {
  

  return Set(Napi::Boolean::New(info.Env(), 
    this->snap7Client->Connected()));
}

Napi::Value S7Client::ErrorText(const Napi::CallbackInfo &info) {
  if (!info[0].IsInt32()) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  return Set(Nan::New<v8::String>(
    CliErrorText(info[0].c_str()));
}

}  // namespace node_snap7
