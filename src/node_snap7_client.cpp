/*
 * Copyright (c) 2019, Mathias Küsel
 * MIT License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

#include <node_snap7_client.h>
#include <sstream>
#include <string>

namespace node_snap7 {

Napi::Object S7Client::Init(Napi::Env env, Napi::Object exports) {

  Napi::Function func = DefineClass(env, "S7Client", {
    // Control functions
    InstanceMethod("_Connect", &S7Client::Connect),
    InstanceMethod("_ConnectTo", &S7Client::ConnectTo),
    InstanceMethod("SetConnectionParams", &S7Client::SetConnectionParams),
    InstanceMethod("SetConnectionType", &S7Client::SetConnectionType),
    InstanceMethod("Disconnect", &S7Client::Disconnect),
    InstanceMethod("GetParam", &S7Client::GetParam),
    InstanceMethod("SetParam", &S7Client::SetParam),

    // Data I/O Main functions
    InstanceMethod("_ReadArea", &S7Client::ReadArea),
    InstanceMethod("_WriteArea", &S7Client::WriteArea),
    InstanceMethod("_ReadMultiVars", &S7Client::ReadMultiVars),
    InstanceMethod("_WriteMultiVars", &S7Client::WriteMultiVars),

    // Directory functions
    InstanceMethod("_ListBlocks", &S7Client::ListBlocks),
    InstanceMethod("_GetAgBlockInfo", &S7Client::GetAgBlockInfo),
    InstanceMethod("GetPgBlockInfo", &S7Client::GetPgBlockInfo),
    InstanceMethod("_ListBlocksOfType", &S7Client::ListBlocksOfType),

    // Blocks functions
    InstanceMethod("_Upload", &S7Client::Upload),
    InstanceMethod("_FullUpload", &S7Client::FullUpload),
    InstanceMethod("_Download", &S7Client::Download),
    InstanceMethod("_Delete", &S7Client::Delete),
    InstanceMethod("_DBGet", &S7Client::DBGet),
    InstanceMethod("_DBFill", &S7Client::DBFill),

    // Date/Time functions
    InstanceMethod("_GetPlcDateTime", &S7Client::GetPlcDateTime),
    InstanceMethod("_SetPlcDateTime", &S7Client::SetPlcDateTime),
    InstanceMethod("_SetPlcSystemDateTime", &S7Client::SetPlcSystemDateTime),

    // System Info functions
    InstanceMethod("_GetOrderCode", &S7Client::GetOrderCode),
    InstanceMethod("_GetCpuInfo", &S7Client::GetCpuInfo),
    InstanceMethod("_GetCpInfo", &S7Client::GetCpInfo),
    InstanceMethod("_ReadSZL", &S7Client::ReadSZL),
    InstanceMethod("_ReadSZLList", &S7Client::ReadSZLList),

    // Control functions
    InstanceMethod("_PlcHotStart", &S7Client::PlcHotStart),
    InstanceMethod("_PlcColdStart", &S7Client::PlcColdStart),
    InstanceMethod("_PlcStop", &S7Client::PlcStop),
    InstanceMethod("_CopyRamToRom", &S7Client::CopyRamToRom),
    InstanceMethod("_Compress", &S7Client::Compress),

    // Security functions
    InstanceMethod("_GetProtection", &S7Client::GetProtection),
    InstanceMethod("_SetSessionPassword", &S7Client::SetSessionPassword),
    InstanceMethod("_ClearSessionPassword", &S7Client::ClearSessionPassword),

    // Properties
    InstanceMethod("ExecTime", &S7Client::ExecTime),
    InstanceMethod("LastError", &S7Client::LastError),
    InstanceMethod("PDURequested", &S7Client::PDURequested),
    InstanceMethod("PDULength", &S7Client::PDULength),
    InstanceMethod("_PlcStatus", &S7Client::PlcStatus),
    InstanceMethod("Connected", &S7Client::Connected),

    // Error to text function
    InstanceMethod("ErrorText", &S7Client::ErrorText),

    // Error codes
    InstanceValue("errNegotiatingPDU", Napi::Value::From(env, errNegotiatingPDU)),
    InstanceValue("errCliInvalidParams", Napi::Value::From(env, errCliInvalidParams)),
    InstanceValue("errCliJobPending", Napi::Value::From(env, errCliJobPending)),
    InstanceValue("errCliTooManyItems", Napi::Value::From(env, errCliTooManyItems)),
    InstanceValue("errCliInvalidWordLen", Napi::Value::From(env, errCliInvalidWordLen)),
    InstanceValue("errCliPartialDataWritten", Napi::Value::From(env, errCliPartialDataWritten)),
    InstanceValue("errCliSizeOverPDU", Napi::Value::From(env, errCliSizeOverPDU)),
    InstanceValue("errCliInvalidPlcAnswer", Napi::Value::From(env, errCliInvalidPlcAnswer)),
    InstanceValue("errCliAddressOutOfRange", Napi::Value::From(env, errCliAddressOutOfRange)),
    InstanceValue("errCliInvalidTransportSize", Napi::Value::From(env, errCliInvalidTransportSize)),
    InstanceValue("errCliWriteDataSizeMismatch", Napi::Value::From(env, errCliWriteDataSizeMismatch)),
    InstanceValue("errCliItemNotAvailable", Napi::Value::From(env, errCliItemNotAvailable)),
    InstanceValue("errCliInvalidValue", Napi::Value::From(env, errCliInvalidValue)),
    InstanceValue("errCliCannotStartPLC", Napi::Value::From(env, errCliCannotStartPLC)),
    InstanceValue("errCliAlreadyRun", Napi::Value::From(env, errCliAlreadyRun)),
    InstanceValue("errCliCannotStopPLC", Napi::Value::From(env, errCliCannotStopPLC)),
    InstanceValue("errCliCannotCopyRamToRom", Napi::Value::From(env, errCliCannotCopyRamToRom)),
    InstanceValue("errCliCannotCompress", Napi::Value::From(env, errCliCannotCompress)),
    InstanceValue("errCliAlreadyStop", Napi::Value::From(env, errCliAlreadyStop)),
    InstanceValue("errCliFunNotAvailable", Napi::Value::From(env, errCliFunNotAvailable)),
    InstanceValue("errCliUploadSequenceFailed", Napi::Value::From(env, errCliUploadSequenceFailed)),
    InstanceValue("errCliInvalidDataSizeRecvd", Napi::Value::From(env, errCliInvalidDataSizeRecvd)),
    InstanceValue("errCliInvalidBlockType", Napi::Value::From(env, errCliInvalidBlockType)),
    InstanceValue("errCliInvalidBlockNumber", Napi::Value::From(env, errCliInvalidBlockNumber)),
    InstanceValue("errCliInvalidBlockSize", Napi::Value::From(env, errCliInvalidBlockSize)),
    InstanceValue("errCliDownloadSequenceFailed", Napi::Value::From(env, errCliDownloadSequenceFailed)),
    InstanceValue("errCliInsertRefused", Napi::Value::From(env, errCliInsertRefused)),
    InstanceValue("errCliDeleteRefused", Napi::Value::From(env, errCliDeleteRefused)),
    InstanceValue("errCliNeedPassword", Napi::Value::From(env, errCliNeedPassword)),
    InstanceValue("errCliInvalidPassword", Napi::Value::From(env, errCliInvalidPassword)),
    InstanceValue("errCliNoPasswordToSetOrClear", Napi::Value::From(env, errCliNoPasswordToSetOrClear)),
    InstanceValue("errCliJobTimeout", Napi::Value::From(env, errCliJobTimeout)),
    InstanceValue("errCliPartialDataRead", Napi::Value::From(env, errCliPartialDataRead)),
    InstanceValue("errCliBufferTooSmall", Napi::Value::From(env, errCliBufferTooSmall)),
    InstanceValue("errCliFunctionRefused", Napi::Value::From(env, errCliFunctionRefused)),
    InstanceValue("errCliDestroying", Napi::Value::From(env, errCliDestroying)),
    InstanceValue("errCliInvalidParamNumber", Napi::Value::From(env, errCliInvalidParamNumber)),
    InstanceValue("errCliCannotChangeParam", Napi::Value::From(env, errCliCannotChangeParam)),

    // Client Connection Type
    InstanceValue("CONNTYPE_PG", Napi::Value::From(env, CONNTYPE_PG)),
    InstanceValue("CONNTYPE_OP", Napi::Value::From(env, CONNTYPE_OP)),
    InstanceValue("CONNTYPE_BASIC", Napi::Value::From(env, CONNTYPE_BASIC)),

    // CPU Status codes
    InstanceValue("S7CpuStatusUnknown", Napi::Value::From(env, S7CpuStatusUnknown)),
    InstanceValue("S7CpuStatusRun", Napi::Value::From(env, S7CpuStatusRun)),
    InstanceValue("S7CpuStatusStop", Napi::Value::From(env, S7CpuStatusStop)),

    // Area ID
    InstanceValue("S7AreaPE", Napi::Value::From(env, S7AreaPE)),
    InstanceValue("S7AreaPA", Napi::Value::From(env, S7AreaPA)),
    InstanceValue("S7AreaMK", Napi::Value::From(env, S7AreaMK)),
    InstanceValue("S7AreaDB", Napi::Value::From(env, S7AreaDB)),
    InstanceValue("S7AreaCT", Napi::Value::From(env, S7AreaCT)),
    InstanceValue("S7AreaTM", Napi::Value::From(env, S7AreaTM)),

    // Word Length
    InstanceValue("S7WLBit", Napi::Value::From(env, S7WLBit)),
    InstanceValue("S7WLByte", Napi::Value::From(env, S7WLByte)),
    InstanceValue("S7WLWord", Napi::Value::From(env, S7WLWord)),
    InstanceValue("S7WLDWord", Napi::Value::From(env, S7WLDWord)),
    InstanceValue("S7WLReal", Napi::Value::From(env, S7WLReal)),
    InstanceValue("S7WLCounter", Napi::Value::From(env, S7WLCounter)),
    InstanceValue("S7WLTimer", Napi::Value::From(env, S7WLTimer)),

    // Block type
    InstanceValue("Block_OB", Napi::Value::From(env, Block_OB)),
    InstanceValue("Block_DB", Napi::Value::From(env, Block_DB)),
    InstanceValue("Block_SDB", Napi::Value::From(env, Block_SDB)),
    InstanceValue("Block_FC", Napi::Value::From(env, Block_FC)),
    InstanceValue("Block_SFC", Napi::Value::From(env, Block_SFC)),
    InstanceValue("Block_FB", Napi::Value::From(env, Block_FB)),
    InstanceValue("Block_SFB", Napi::Value::From(env, Block_SFB)),

    // Sub Block Type
    InstanceValue("SubBlk_OB", Napi::Value::From(env, SubBlk_OB)),
    InstanceValue("SubBlk_SDB", Napi::Value::From(env, SubBlk_SDB)),
    InstanceValue("SubBlk_FC", Napi::Value::From(env, SubBlk_FC)),
    InstanceValue("SubBlk_SFC", Napi::Value::From(env, SubBlk_SFC)),
    InstanceValue("SubBlk_FB", Napi::Value::From(env, SubBlk_FB)),
    InstanceValue("SubBlk_SFB", Napi::Value::From(env, SubBlk_SFB)),

    // Block languages
    InstanceValue("BlockLangAWL", Napi::Value::From(env, BlockLangAWL)),
    InstanceValue("BlockLangKOP", Napi::Value::From(env, BlockLangKOP)),
    InstanceValue("BlockLangFUP", Napi::Value::From(env, BlockLangFUP)),
    InstanceValue("BlockLangSCL", Napi::Value::From(env, BlockLangSCL)),
    InstanceValue("BlockLangDB", Napi::Value::From(env, BlockLangDB)),
    InstanceValue("BlockLangGRAPH", Napi::Value::From(env, BlockLangGRAPH)),

    // Parameter
    InstanceValue("LocalPort", Napi::Value::From(env, p_u16_LocalPort)),
    InstanceValue("RemotePort", Napi::Value::From(env, p_u16_RemotePort)),
    InstanceValue("PingTimeout", Napi::Value::From(env, p_i32_PingTimeout)),
    InstanceValue("SendTimeout", Napi::Value::From(env, p_i32_SendTimeout)),
    InstanceValue("RecvTimeout", Napi::Value::From(env, p_i32_RecvTimeout)),
    InstanceValue("WorkInterval", Napi::Value::From(env, p_i32_WorkInterval)),
    InstanceValue("SrcRef", Napi::Value::From(env, p_u16_SrcRef)),
    InstanceValue("DstRef", Napi::Value::From(env, p_u16_DstRef)),
    InstanceValue("SrcTSap", Napi::Value::From(env, p_u16_SrcTSap)),
    InstanceValue("PDURequest", Napi::Value::From(env, p_i32_PDURequest)),
    InstanceValue("MaxClients", Napi::Value::From(env, p_i32_MaxClients)),
    InstanceValue("BSendTimeout", Napi::Value::From(env, p_i32_BSendTimeout)),
    InstanceValue("BRecvTimeout", Napi::Value::From(env, p_i32_BRecvTimeout)),
    InstanceValue("RecoveryTime", Napi::Value::From(env, p_u32_RecoveryTime)),
    InstanceValue("KeepAliveTime", Napi::Value::From(env, p_u32_KeepAliveTime))
  });

  Napi::FunctionReference* constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

  exports.Set("S7Client", func);
  return exports;
}

S7Client::S7Client(const Napi::CallbackInfo &info) : Napi::ObjectWrap<S7Client>(info) {
  snap7Client = new TS7Client();
}

S7Client::~S7Client() {
  snap7Client->Disconnect();
  delete snap7Client;
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

void S7Client::FreeCallback(Napi::Env, char* finalizeData) {
  delete[] finalizeData;
}

void S7Client::FreeCallbackSZL(Napi::Env, char* finalizeData) {
  delete reinterpret_cast<PS7SZL>(finalizeData);
}

// Control functions
Napi::Value S7Client::Connect(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() > 0 && info[0].IsFunction()) {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::CONNECT);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->Connect();
    if (ret == 0) {
      return env.Undefined();
    } else {
      std::stringstream err;
      err << "Connect failed with error code: " << ret;
      Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Value S7Client::ConnectTo(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 3) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsString() || !info[1].IsNumber() || !info[2].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  std::string* remAddress = new std::string(info[0].As<Napi::String>().Utf8Value());
  int rack = info[1].As<Napi::Number>().Int32Value();
  int slot = info[2].As<Napi::Number>().Int32Value();

  if (info.Length() > 3 && info[3].IsFunction()) {
    Napi::Function callback = info[3].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::CONNECTTO, remAddress, rack, slot);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->ConnectTo(remAddress->c_str(), rack, slot);
    delete remAddress;
    if (ret == 0) {
      return env.Undefined();
    } else {
      std::stringstream err;
      err << "ConnectTo failed with error code: " << ret;
      Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Value S7Client::SetConnectionParams(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 3) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsString() || !info[1].IsNumber() || !info[2].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  std::string remAddress = info[0].As<Napi::String>().Utf8Value();
  word LocalTSAP = info[1].As<Napi::Number>().Uint32Value();
  word RemoteTSAP = info[2].As<Napi::Number>().Uint32Value();
  int ret = snap7Client->SetConnectionParams(remAddress.c_str(), LocalTSAP, RemoteTSAP);
  if (ret == 0) {
    return env.Undefined();
  } else {
    std::stringstream err;
    err << "SetConnectionParams failed with error code: " << ret;
    Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
    return env.Undefined();
  }
}

Napi::Value S7Client::SetConnectionType(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  word type = info[0].As<Napi::Number>().Uint32Value();
  int ret = snap7Client->SetConnectionType(type);
  if (ret == 0) {
    return env.Undefined();
  } else {
    std::stringstream err;
    err << "SetConnectionType failed with error code: " << ret;
    Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
    return env.Undefined();
  }
}

Napi::Value S7Client::Disconnect(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int ret = snap7Client->Disconnect();
  if (ret == 0) {
    return env.Undefined();
  } else {
    std::stringstream err;
    err << "Disconnect failed with error code: " << ret;
    Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
    return env.Undefined();
  }
}

Napi::Value S7Client::GetParam(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  int pData;
  int ret = snap7Client->GetParam(info[0].As<Napi::Number>().Int32Value()
    , &pData);

  if (ret == 0) {
    return Napi::Number::New(env, pData);
  } else {
    Napi::Error::New(env, "GetParam failed with error code: " + std::to_string(ret)).ThrowAsJavaScriptException();
    return env.Undefined();
  }
}

Napi::Value S7Client::SetParam(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsNumber() || !info[1].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  int pData = info[1].As<Napi::Number>().Int32Value();
  int ret = snap7Client->SetParam(info[0].As<Napi::Number>().Int32Value(), &pData);
  if (ret == 0) {
    return env.Undefined();
  } else {
    Napi::Error::New(env, "SetParam failed with error code: " + std::to_string(ret)).ThrowAsJavaScriptException();
    return env.Undefined();
  }
}

// Data I/O Main functions
void IOWorkerClient::Execute() {
  s7client->mutex.lock();

  switch (caller) {
  case DataIOFunction::CONNECTTO:
      ret = s7client->snap7Client->ConnectTo(
        static_cast<std::string*>(pData)->c_str(), int1, int2);
      break;

  case DataIOFunction::CONNECT:
      ret = s7client->snap7Client->Connect();
      break;

  case DataIOFunction::READAREA:
      ret = s7client->snap7Client->ReadArea(int1, int2, int3, int4
        , int5, pData);
      break;

  case DataIOFunction::WRITEAREA:
      ret = s7client->snap7Client->WriteArea(int1, int2, int3, int4
        , int5, pData);
      break;

  case DataIOFunction::READMULTI:
      ret = s7client->snap7Client->ReadMultiVars(
          static_cast<PS7DataItem>(pData), int1);
      break;

  case DataIOFunction::WRITEMULTI:
      ret = s7client->snap7Client->WriteMultiVars(
          static_cast<PS7DataItem>(pData), int1);
      break;

  case DataIOFunction::PLCSTATUS:
      ret = s7client->snap7Client->PlcStatus();
      if ((ret == S7CpuStatusUnknown) ||
          (ret == S7CpuStatusStop) ||
          (ret == S7CpuStatusRun)) {
        int1 = ret;
        ret = 0;
      }
      break;

  case DataIOFunction::CLEARSESSIONPW:
      ret = s7client->snap7Client->ClearSessionPassword();
      break;

  case DataIOFunction::SETSESSIONPW:
      ret = s7client->snap7Client->SetSessionPassword(
        &*static_cast<std::string*>(pData)->begin());
      break;

  case DataIOFunction::GETPROTECTION:
      ret = s7client->snap7Client->GetProtection(
          static_cast<PS7Protection>(pData));
      break;

  case DataIOFunction::PLCSTOP:
      ret = s7client->snap7Client->PlcStop();
      break;

  case DataIOFunction::PLCCOLDSTART:
      ret = s7client->snap7Client->PlcColdStart();
      break;

  case DataIOFunction::PLCHOTSTART:
      ret = s7client->snap7Client->PlcHotStart();
      break;

  case DataIOFunction::GETCPINFO:
      ret = s7client->snap7Client->GetCpInfo(
          static_cast<PS7CpInfo>(pData));
      break;

  case DataIOFunction::GETCPUINFO:
      ret = s7client->snap7Client->GetCpuInfo(
          static_cast<PS7CpuInfo>(pData));
      break;

  case DataIOFunction::GETORDERCODE:
      ret = s7client->snap7Client->GetOrderCode(
          static_cast<PS7OrderCode>(pData));
      break;

  case DataIOFunction::SETPLCSYSTEMDATETIME:
      ret = s7client->snap7Client->SetPlcSystemDateTime();
      break;

  case DataIOFunction::GETPLCDATETIME:
      ret = s7client->snap7Client->GetPlcDateTime(
          static_cast<tm*>(pData));
      break;

  case DataIOFunction::SETPLCDATETIME:
      ret = s7client->snap7Client->SetPlcDateTime(
          static_cast<tm*>(pData));
      break;

  case DataIOFunction::COMPRESS:
      ret = s7client->snap7Client->Compress(int1);
      break;

  case DataIOFunction::COPYRAMTOROM:
      ret = s7client->snap7Client->CopyRamToRom(int1);
      break;

  case DataIOFunction::DBFILL:
      ret = s7client->snap7Client->DBFill(int1, int2);
      break;

  case DataIOFunction::DBGET:
      ret = s7client->snap7Client->DBGet(int1, pData, &int2);
      break;

  case DataIOFunction::DELETEBLOCK:
      ret = s7client->snap7Client->Delete(int1, int2);
      break;

  case DataIOFunction::DOWNLOAD:
      ret = s7client->snap7Client->Download(int1, pData, int2);
      break;

  case DataIOFunction::FULLUPLOAD:
      ret = s7client->snap7Client->FullUpload(int1, int2, pData, &int3);
      break;

  case DataIOFunction::UPLOAD:
      ret = s7client->snap7Client->Upload(int1, int2, pData, &int3);
      break;

  case DataIOFunction::LISTBLOCKSOFTYPE:
      ret = s7client->snap7Client->ListBlocksOfType(int1
        , static_cast<PS7BlocksOfType>(pData), &int2);
      break;

  case DataIOFunction::GETAGBLOCKINFO:
      ret = s7client->snap7Client->GetAgBlockInfo(int1, int2
        , static_cast<PS7BlockInfo>(pData));
      break;

  case DataIOFunction::LISTBLOCKS:
      ret = s7client->snap7Client->ListBlocks(
          static_cast<PS7BlocksList>(pData));
      break;

  case DataIOFunction::READSZLLIST:
      ret = s7client->snap7Client->ReadSZLList(
          static_cast<PS7SZLList>(pData), &int1);
      break;

  case DataIOFunction::READSZL:
      ret = s7client->snap7Client->ReadSZL(int1, int2
        , static_cast<PS7SZL>(pData), &int3);
      break;
  }

  s7client->mutex.unlock();
}

void IOWorkerClient::OnOK() {
  Napi::Value val = Env().Null();

  switch (caller) {
  case DataIOFunction::CONNECTTO:
  case DataIOFunction::SETSESSIONPW:
      delete static_cast<std::string*>(pData);
      break;

  case DataIOFunction::CONNECT:
  case DataIOFunction::WRITEAREA:
  case DataIOFunction::CLEARSESSIONPW:
  case DataIOFunction::PLCSTOP:
  case DataIOFunction::PLCCOLDSTART:
  case DataIOFunction::PLCHOTSTART:
  case DataIOFunction::SETPLCSYSTEMDATETIME:
  case DataIOFunction::COPYRAMTOROM:
  case DataIOFunction::COMPRESS:
  case DataIOFunction::DBFILL:
  case DataIOFunction::DELETEBLOCK:
  case DataIOFunction::DOWNLOAD:
      break;

  case DataIOFunction::READAREA:
    if (ret == 0) {
      val = Napi::Buffer<char>::NewOrCopy(Env(),
          static_cast<char*>(pData)
        , static_cast<size_t>(int4) * s7client->GetByteCountFromWordLen(int5)
        , S7Client::FreeCallback);
    } else {
      delete[] static_cast<char*>(pData);
    }
    break;

  case DataIOFunction::READMULTI:
      if (ret == 0) {
        val = s7client->S7DataItemToArray(static_cast<PS7DataItem>(pData)
          , int1, true);
      } else {
        for (int i = 0; i < int1; i++) {
          delete[] static_cast<char*>(static_cast<PS7DataItem>(pData)[i].pdata);
        }
        delete[] static_cast<PS7DataItem>(pData);
      }
      break;

  case DataIOFunction::WRITEMULTI:
      if (ret == 0) {
        val = s7client->S7DataItemToArray(static_cast<PS7DataItem>(pData)
          , int1, false);
      } else {
        delete[] static_cast<PS7DataItem>(pData);
      }
      break;

  case DataIOFunction::GETPROTECTION:
      if (ret == 0) {
        val = s7client->S7ProtectionToObject(
          static_cast<PS7Protection>(pData));
      }
      delete static_cast<PS7Protection>(pData);
      break;

  case DataIOFunction::GETCPINFO:
      if (ret == 0) {
        val = s7client->S7CpInfoToObject(
          static_cast<PS7CpInfo>(pData));
      }
      delete static_cast<PS7CpInfo>(pData);
      break;

  case DataIOFunction::GETCPUINFO:
      if (ret == 0) {
        val = s7client->S7CpuInfoToObject(
          static_cast<PS7CpuInfo>(pData));
      }
      delete static_cast<PS7CpuInfo>(pData);
      break;

  case DataIOFunction::GETORDERCODE:
      if (ret == 0) {
        val = s7client->S7OrderCodeToObject(
          static_cast<PS7OrderCode>(pData));
      }
      delete static_cast<PS7OrderCode>(pData);
      break;

  case DataIOFunction::GETPLCDATETIME:
      if (ret == 0) {
        double timestamp = static_cast<double>(mktime(static_cast<tm*>(pData)));
        val = Napi::Date::New(Env(), timestamp * 1000);
      }
      delete static_cast<tm*>(pData);
      break;

  case DataIOFunction::SETPLCDATETIME:
      delete static_cast<tm*>(pData);
      break;

  case DataIOFunction::PLCSTATUS:
      if (ret == 0) {
        val = Napi::Number::New(Env(), int1);
      }
      break;

  case DataIOFunction::DBGET:
      if (ret == 0) {
        val = Napi::Buffer<char>::NewOrCopy(Env(),
            static_cast<char*>(pData)
          , int2
          , S7Client::FreeCallback);
      } else {
        delete[] static_cast<char*>(pData);
      }
      break;

  case DataIOFunction::FULLUPLOAD:
  case DataIOFunction::UPLOAD:
      if (ret == 0) {
        val = Napi::Buffer<char>::NewOrCopy(Env(),
            static_cast<char*>(pData)
          , int3
          , S7Client::FreeCallback);
      } else {
        delete[] static_cast<char*>(pData);
      }
      break;

  case DataIOFunction::LISTBLOCKSOFTYPE:
      if (ret == 0) {
        val = s7client->S7BlocksOfTypeToArray(
            static_cast<PS7BlocksOfType>(pData), int2);
      }
      delete[] static_cast<PS7BlocksOfType>(pData);
      break;

  case DataIOFunction::GETAGBLOCKINFO:
      if (ret == 0) {
        Napi::Object block_info = s7client->S7BlockInfoToObject(
            static_cast<PS7BlockInfo>(pData));
        val = block_info;
      }
      delete static_cast<PS7BlockInfo>(pData);
      break;

  case DataIOFunction::LISTBLOCKS:
      if (ret == 0) {
        Napi::Object blocks_list = s7client->S7BlocksListToObject(
            static_cast<PS7BlocksList>(pData));
        val = blocks_list;
      }
      delete static_cast<PS7BlocksList>(pData);
      break;

  case DataIOFunction::READSZLLIST:
      if (ret == 0) {
        Napi::Array szl_list = s7client->S7SZLListToArray(
            static_cast<PS7SZLList>(pData), int1);
        val = szl_list;
      }
      delete static_cast<PS7SZLList>(pData);
      break;

  case DataIOFunction::READSZL:
      if (ret == 0) {
        val = Napi::Buffer<char>::NewOrCopy(Env(),
            reinterpret_cast<char*>(static_cast<PS7SZL>(pData))
          , int3
          , S7Client::FreeCallbackSZL);
      } else {
        delete static_cast<PS7SZL>(pData);
      }
      break;
  }

  if (ret == 0) {
    m_deferred.Resolve(Env().Null());
  } else {
    m_deferred.Reject(Napi::Number::New(Env(), ret));
  }
}

Napi::Value S7Client::ReadArea(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 5) {
    Napi::TypeError::New(env, "Wrong number of Arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsNumber() || !info[1].IsNumber() ||
      !info[2].IsNumber() || !info[3].IsNumber() ||
      !info[4].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  int area = info[0].As<Napi::Number>().Int32Value();
  int dbNumber = info[1].As<Napi::Number>().Int32Value();
  int start = info[2].As<Napi::Number>().Int32Value();
  int amount = info[3].As<Napi::Number>().Int32Value();
  int wordLen = info[4].As<Napi::Number>().Int32Value();

  int byteCount = GetByteCountFromWordLen(wordLen);
  size_t size = amount * byteCount;
  char *bufferData = new char[size];

  
  if (info.Length() > 5 && info[5].IsFunction()) {
    Napi::Function callback = info[5].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::READAREA
      , bufferData, area, dbNumber, start, amount, wordLen);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->ReadArea(area, dbNumber, start, amount, wordLen, bufferData);
    if (ret == 0) {
      return Napi::Buffer<char>::NewOrCopy(env, bufferData, size, S7Client::FreeCallback);
    } else {
      delete[] bufferData;
      std::stringstream err;
      err << "ReadArea failed with error code: " << ret;
      Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Value S7Client::WriteArea(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 6) {
    Napi::TypeError::New(env, "Wrong number of Arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsNumber() || !info[1].IsNumber() ||
      !info[2].IsNumber() || !info[3].IsNumber() ||
      !info[4].IsNumber() || !info[5].IsBuffer()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  int area = info[0].As<Napi::Number>().Int32Value();
  int dbNumber = info[1].As<Napi::Number>().Int32Value();
  int start = info[2].As<Napi::Number>().Int32Value();
  int amount = info[3].As<Napi::Number>().Int32Value();
  int wordLen = info[4].As<Napi::Number>().Int32Value();
  Napi::Buffer<char> buffer = info[5].As<Napi::Buffer<char>>();
  
  if (info.Length() > 6 && info[6].IsFunction()) {
    Napi::Function callback = info[6].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::WRITEAREA
      , buffer.Data(), area, dbNumber, start, amount, wordLen);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->WriteArea(area, dbNumber, start, amount, wordLen, buffer.Data());
    if (ret == 0) {
      return env.Undefined();
    } else {
      std::stringstream err;
      err << "WriteArea failed with error code: " << ret;
      Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Value S7Client::ReadMultiVars(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsArray()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  const Napi::Array data_arr = info[0].As<Napi::Array>();
  uint32_t len = data_arr.Length();
  if (len == 0) {
    Napi::TypeError::New(env, "Array needs at least 1 item").ThrowAsJavaScriptException();
    return env.Undefined();
  } else if (len > MaxVars) {
    std::stringstream err;
    err << "Array exceeds max variables (" << MaxVars
        << ") that can be transferred with ReadMultiVars()";
    Napi::TypeError::New(env, err.str()).ThrowAsJavaScriptException();
    return env.Undefined();
  }

  for (uint32_t i = 0; i < len; i++) {
    if (!data_arr[i].IsObject()) {
      Napi::TypeError::New(env, "Wrong argument structure").ThrowAsJavaScriptException();
      return env.Undefined();
    } else {
      Napi::Object data_obj = data_arr[i].As<Napi::Object>();
      if (!data_obj.Has("Area") ||
          !data_obj.Has("WordLen") ||
          !data_obj.Has("Start") ||
          !data_obj.Has("Amount")) {
        Napi::TypeError::New(env, "Wrong argument structure").ThrowAsJavaScriptException();
        return env.Undefined();
      } else if (!data_obj.Get("Area").IsNumber() ||
                 !data_obj.Get("WordLen").IsNumber() ||
                 !data_obj.Get("Start").IsNumber() ||
                 !data_obj.Get("Amount").IsNumber()) {
        Napi::TypeError::New(env, "Wrong argument structure").ThrowAsJavaScriptException();
        return env.Undefined();
      } else if (data_obj.Get("Area").As<Napi::Number>().Int32Value() == S7AreaDB) {
        if (!data_obj.Has("DBNumber")) {
          Napi::TypeError::New(env, "Wrong argument structure").ThrowAsJavaScriptException();
          return env.Undefined();
        }
      } else {
        data_obj.Set("DBNumber", Napi::Number::New(env, 0));
      }
    }
  }

  PS7DataItem Items = new TS7DataItem[len];
  Napi::Object data_obj;
  int byteCount, size;

  for (uint32_t i = 0; i < len; i++) {
    data_obj = data_arr[i].As<Napi::Object>();

    Items[i].Area = data_obj.Get("Area").As<Napi::Number>().Int32Value();
    Items[i].WordLen = data_obj.Get("WordLen").As<Napi::Number>().Int32Value();
    Items[i].DBNumber = data_obj.Get("DBNumber").As<Napi::Number>().Int32Value();
    Items[i].Start = data_obj.Get("Start").As<Napi::Number>().Int32Value();
    Items[i].Amount = data_obj.Get("Amount").As<Napi::Number>().Int32Value();

    byteCount = GetByteCountFromWordLen(Items[i].WordLen);
    size = Items[i].Amount * byteCount;
    Items[i].pdata = new char[size];
  }

  if (info.Length() > 1 && info[1].IsFunction()) {
    Napi::Function callback = info[1].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::READMULTI
      , Items, len);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->ReadMultiVars(Items, len);
    if (ret == 0) {
      Napi::Array res_arr = S7DataItemToArray(Items, len, true);
      return res_arr;
    } else {
      for (uint32_t i = 0; i < len; i++) {
        delete[] static_cast<char*>(Items[i].pdata);
      }
      delete[] Items;
      std::stringstream err;
      err << "ReadMultiVars failed with error code: " << ret;
      Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Array S7Client::S7DataItemToArray(
    PS7DataItem Items
  , int len
  , bool readMulti
) {
  Napi::EscapableHandleScope scope(Env());

  Napi::Array res_arr = Napi::Array::New(Env(), len);
  Napi::Object res_obj;
  int byteCount, size;

  for (int i = 0; i < len; i++) {
    res_obj = Napi::Object::New(Env());
    res_obj.Set("Result", Napi::Number::New(Env(), Items[i].Result));

    if (readMulti == true) {
      if (Items[i].Result == 0) {
        byteCount = S7Client::GetByteCountFromWordLen(Items[i].WordLen);
        size = byteCount * Items[i].Amount;
        res_obj.Set("Data"
          , Napi::Buffer<char>::NewOrCopy(Env(),
              static_cast<char*>(Items[i].pdata)
            , size
            , S7Client::FreeCallback));
      } else {
        delete[] static_cast<char*>(Items[i].pdata);
        res_obj.Set("Data", Env().Null());
      }
    }
    res_arr.Set(i, res_obj);
  }
  delete[] Items;

  return scope.Escape(res_arr).As<Napi::Array>();
}

Napi::Value S7Client::WriteMultiVars(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsArray()) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  const Napi::Array data_arr = info[0].As<Napi::Array>();
  uint32_t len = data_arr.Length();
  if (len == 0) {
    Napi::TypeError::New(env, "Array needs at least 1 item").ThrowAsJavaScriptException();
    return env.Undefined();
  } else if (len > MaxVars) {
    std::stringstream err;
    err << "Array exceeds max variables (" << MaxVars
        << ") that can be transferred with WriteMultiVars()";
    Napi::TypeError::New(env, err.str()).ThrowAsJavaScriptException();
    return env.Undefined();
  }

  for (uint32_t i = 0; i < len; i++) {
    if (!data_arr[i].IsObject()) {
      Napi::TypeError::New(env, "Wrong argument structure").ThrowAsJavaScriptException();
      return env.Undefined();
    } else {
      Napi::Object data_obj = data_arr[i].As<Napi::Object>();
      if (!data_obj.Has("Area") ||
          !data_obj.Has("WordLen") ||
          !data_obj.Has("Start") ||
          !data_obj.Has("Amount") ||
          !data_obj.Has("Data")) {
        Napi::TypeError::New(env, "Wrong argument structure").ThrowAsJavaScriptException();
        return env.Undefined();
      } else if (!data_obj.Get("Area").IsNumber() ||
                 !data_obj.Get("WordLen").IsNumber() ||
                 !data_obj.Get("Start").IsNumber() ||
                 !data_obj.Get("Amount").IsNumber() ||
                 !data_obj.Get("Data").IsBuffer()) {
        Napi::TypeError::New(env, "Wrong argument structure").ThrowAsJavaScriptException();
        return env.Undefined();
      } else if (data_obj.Get("Area").As<Napi::Number>().Int32Value() == S7AreaDB) {
        if (!data_obj.Has("DBNumber")) {
          Napi::TypeError::New(env, "Wrong argument structure").ThrowAsJavaScriptException();
          return env.Undefined();
        }
      } else {
        data_obj.Set("DBNumber", Napi::Number::New(env, 0));
      }
    }
  }

  PS7DataItem Items = new TS7DataItem[len];
  Napi::Object data_obj;
  for (uint32_t i = 0; i < len; i++) {
    data_obj = data_arr[i].As<Napi::Object>();

    Items[i].Area = data_obj.Get("Area").As<Napi::Number>().Int32Value();
    Items[i].WordLen = data_obj.Get("WordLen").As<Napi::Number>().Int32Value();
    Items[i].DBNumber = data_obj.Get("DBNumber").As<Napi::Number>().Int32Value();
    Items[i].Start = data_obj.Get("Start").As<Napi::Number>().Int32Value();
    Items[i].Amount = data_obj.Get("Amount").As<Napi::Number>().Int32Value();
    Items[i].pdata = data_obj.Get("Data").As<Napi::Buffer<char>>().Data();
  }

  if (info.Length() > 1 && info[1].IsFunction()) {
    Napi::Function callback = info[1].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::WRITEMULTI
      , Items, len);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->WriteMultiVars(Items, len);
    if (ret == 0) {
      Napi::Array res_arr = S7DataItemToArray(Items, len, false);
      return res_arr;
    } else {
      delete[] Items;
      std::stringstream err;
      err << "WriteMultiVars failed with error code: " << ret;
      Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

// Directory functions
Napi::Value S7Client::ListBlocks(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  PS7BlocksList BlocksList = new TS7BlocksList;
  if (info.Length() > 0 && info[0].IsFunction()) {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::LISTBLOCKS
      , BlocksList);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->ListBlocks(BlocksList);
    if (ret == 0) {
      Napi::Object blocks_list = S7BlocksListToObject(BlocksList);
      delete BlocksList;
      return blocks_list;
    } else {
      delete BlocksList;
      std::stringstream err;
      err << "ListBlocks failed with error code: " << ret;
      Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Object S7Client::S7BlocksListToObject(
    PS7BlocksList BlocksList
) {
  Napi::EscapableHandleScope scope(Env());

  Napi::Object blocks_list = Napi::Object::New(Env());
  blocks_list.Set("OBCount", Napi::Number::New(Env(), BlocksList->OBCount));
  blocks_list.Set("FBCount", Napi::Number::New(Env(), BlocksList->FBCount));
  blocks_list.Set("FCCount", Napi::Number::New(Env(), BlocksList->FCCount));
  blocks_list.Set("SFBCount", Napi::Number::New(Env(), BlocksList->SFBCount));
  blocks_list.Set("SFCCount", Napi::Number::New(Env(), BlocksList->SFCCount));
  blocks_list.Set("DBCount", Napi::Number::New(Env(), BlocksList->DBCount));
  blocks_list.Set("SDBCount", Napi::Number::New(Env(), BlocksList->SDBCount));

  return scope.Escape(blocks_list).As<Napi::Object>();
}


Napi::Value S7Client::GetAgBlockInfo(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsNumber() || !info[1].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  int BlockType = info[0].As<Napi::Number>().Int32Value();
  int BlockNum = info[1].As<Napi::Number>().Int32Value();

  PS7BlockInfo BlockInfo = new TS7BlockInfo;
  if (info.Length() > 2 && info[2].IsFunction()) {
    Napi::Function callback = info[2].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::GETAGBLOCKINFO
      , BlockInfo, BlockType, BlockNum);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->GetAgBlockInfo(BlockType, BlockNum, BlockInfo);
    if (ret == 0) {
      Napi::Object block_info = S7BlockInfoToObject(BlockInfo);
      delete BlockInfo;
      return block_info;
    } else {
      delete BlockInfo;
      std::stringstream err;
      err << "GetAgBlockInfo failed with error code: " << ret;
      Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Value S7Client::GetPgBlockInfo(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (!info[0].IsBuffer()) {
    Napi::TypeError::New(env, "Argument should be a Buffer").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  PS7BlockInfo BlockInfo = new TS7BlockInfo;
  Napi::Buffer<char> buffer = info[0].As<Napi::Buffer<char>>();

  int ret = snap7Client->GetPgBlockInfo(
    buffer.Data(), BlockInfo, buffer.Length());

  if (ret == 0) {
    Napi::Object block_info = S7BlockInfoToObject(BlockInfo);
    delete BlockInfo;
    return block_info;
  } else {
    delete BlockInfo;
    std::stringstream err;
    err << "GetPgBlockInfo failed with error code: " << ret;
    Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
    return env.Undefined();
  }
}

Napi::Object S7Client::S7BlockInfoToObject(PS7BlockInfo BlockInfo) {
  Napi::EscapableHandleScope scope(Env());

  Napi::Object block_info = Napi::Object::New(Env());
  block_info.Set("BlkType", Napi::Number::New(Env(), BlockInfo->BlkType));
  block_info.Set("BlkNumber", Napi::Number::New(Env(), BlockInfo->BlkNumber));
  block_info.Set("BlkLang", Napi::Number::New(Env(), BlockInfo->BlkLang));
  block_info.Set("BlkFlags", Napi::Number::New(Env(), BlockInfo->BlkFlags));
  block_info.Set("MC7Size", Napi::Number::New(Env(), BlockInfo->MC7Size));
  block_info.Set("LoadSize", Napi::Number::New(Env(), BlockInfo->LoadSize));
  block_info.Set("LocalData", Napi::Number::New(Env(), BlockInfo->LocalData));
  block_info.Set("SBBLength", Napi::Number::New(Env(), BlockInfo->SBBLength));
  block_info.Set("CheckSum", Napi::Number::New(Env(), BlockInfo->CheckSum));
  block_info.Set("Version", Napi::Number::New(Env(), BlockInfo->Version));
  block_info.Set("CodeDate", Napi::String::New(Env(), BlockInfo->CodeDate));
  block_info.Set("IntfDate", Napi::String::New(Env(), BlockInfo->IntfDate));
  block_info.Set("Author", Napi::String::New(Env(), BlockInfo->Author));
  block_info.Set("Family", Napi::String::New(Env(), BlockInfo->Family));
  block_info.Set("Header", Napi::String::New(Env(), BlockInfo->Header));

  return scope.Escape(block_info).As<Napi::Object>();
}

Napi::Value S7Client::ListBlocksOfType(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  int BlockType = info[0].As<Napi::Number>().Int32Value();
  int BlockNum = sizeof(TS7BlocksOfType) / sizeof(word);
  PS7BlocksOfType BlockList = new TS7BlocksOfType[BlockNum];

  if (info.Length() > 1 && info[1].IsFunction()) {
    Napi::Function callback = info[1].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::LISTBLOCKSOFTYPE
      , BlockList, BlockType, BlockNum);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->ListBlocksOfType(BlockType, BlockList, &BlockNum);
    if (ret == 0) {
      Napi::Array block_list = S7BlocksOfTypeToArray(BlockList, BlockNum);
      delete[] BlockList;
      return block_list;
    } else {
      delete[] BlockList;
      std::stringstream err;
      err << "ListBlocksOfType failed with error code: " << ret;
      Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Array S7Client::S7BlocksOfTypeToArray(
  PS7BlocksOfType BlocksList
  , int count
) {
  Napi::EscapableHandleScope scope(Env());

  Napi::Array block_list = Napi::Array::New(Env(), count);
  for (int i = 0; i < count; i++) {
    block_list.Set(i, Napi::Number::New(Env(), (*BlocksList)[i]));
  }

  return scope.Escape(block_list).As<Napi::Array>();
}

// Blocks functions
Napi::Value S7Client::Upload(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 3) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  int blockType = info[0].As<Napi::Number>().Int32Value();
  int blockNum = info[1].As<Napi::Number>().Int32Value();
  int size = info[2].As<Napi::Number>().Int32Value();
  char *bufferData = new char[size];

  if (info.Length() > 3 && info[3].IsFunction()) {
    Napi::Function callback = info[3].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::UPLOAD
      , bufferData, blockType, blockNum, size);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->Upload(blockType, blockNum, bufferData, &size);
    if (ret == 0) {
      return Napi::Buffer<char>::NewOrCopy(env, bufferData, size, S7Client::FreeCallback);
    } else {
      delete[] bufferData;
      std::stringstream err;
      err << "Upload failed with error code: " << ret;
      Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Value S7Client::FullUpload(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 3) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  int blockType = info[0].As<Napi::Number>().Int32Value();
  int blockNum = info[1].As<Napi::Number>().Int32Value();
  int size = info[2].As<Napi::Number>().Int32Value();
  char *bufferData = new char[size];

  if (info.Length() > 3 && info[3].IsFunction()) {
    Napi::Function callback = info[3].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::FULLUPLOAD
      , bufferData, blockType, blockNum, size);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->FullUpload(blockType, blockNum, bufferData, &size);
    if (ret == 0) {
      return Napi::Buffer<char>::NewOrCopy(env, bufferData, size, S7Client::FreeCallback);
    } else {
      delete[] bufferData;
      std::stringstream err;
      err << "FullUpload failed with error code: " << ret;
      Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Value S7Client::Download(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsNumber() || !info[1].IsBuffer()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  int blockNum = info[0].As<Napi::Number>().Int32Value();
  Napi::Buffer<char> buffer = info[1].As<Napi::Buffer<char>>();

  if (info.Length() > 2 && info[2].IsFunction()) {
    Napi::Function callback = info[2].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::DOWNLOAD
      , buffer.Data(), blockNum, buffer.Length());
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->Download(blockNum, buffer.Data(), buffer.Length());
    if (ret == 0) {
      return env.Undefined();
    } else {
      Napi::Error::New(env, "Download failed with error code: " + std::to_string(ret)).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Value S7Client::Delete(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsNumber() || !info[1].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  int blockType = info[0].As<Napi::Number>().Int32Value();
  int blockNum = info[1].As<Napi::Number>().Int32Value();

  if (info.Length() > 2 && info[2].IsFunction()) {
    Napi::Function callback = info[2].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::DELETEBLOCK
      , blockType, blockNum);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->Delete(blockType, blockNum);
    if (ret == 0) {
      return env.Undefined();
    } else {
      Napi::Error::New(env, "Delete failed with error code: " + std::to_string(ret)).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Value S7Client::DBGet(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  int dbNumber = info[0].As<Napi::Number>().Int32Value();
  int size = 65536;
  char *bufferData = new char[size];

  if (info.Length() > 1 && info[1].IsFunction()) {
    Napi::Function callback = info[1].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::DBGET
      , bufferData, dbNumber, size);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->DBGet(dbNumber, bufferData, &size);
    if (ret == 0) {
      return Napi::Buffer<char>::NewOrCopy(env, bufferData, size, S7Client::FreeCallback);
    } else {
      delete[] bufferData;
      std::stringstream err;
      err << "DBGet failed with error code: " << ret;
      Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Value S7Client::DBFill(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsNumber() || !(info[1].IsNumber() || info[1].IsString())) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  int dbNumber = info[0].As<Napi::Number>().Int32Value();
  int fill;
  if (info[1].IsNumber()) {
    fill = info[1].As<Napi::Number>().Int32Value();
  } else {
    std::string fillstr = info[1].As<Napi::String>().Utf8Value();
    fill = static_cast<int>(*fillstr.c_str());
  }

  if (info.Length() > 2 && info[2].IsFunction()) {
    Napi::Function callback = info[2].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::DBFILL
      , dbNumber, fill);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->DBFill(dbNumber, fill);
    if (ret == 0) {
      return env.Undefined();
    } else {
      Napi::Error::New(env, "DBFill failed with error code: " + std::to_string(ret)).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

// Date/Time functions
Napi::Value S7Client::GetPlcDateTime(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  tm *DateTime = new tm;
  if (info.Length() > 0 && info[0].IsFunction()) {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::GETPLCDATETIME
      , DateTime);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->GetPlcDateTime(DateTime);
    if (ret == 0) {
      double timestamp = static_cast<double>(mktime(DateTime));
      delete DateTime;
      return Napi::Date::New(env, timestamp * 1000);
    } else {
      delete DateTime;
      Napi::Error::New(env, "GetPlcDateTime failed with error code: " + std::to_string(ret)).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Value S7Client::SetPlcDateTime(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!(info[0].IsObject() || info[0].IsDate())) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  tm *DateTime = new tm;
  if (info[0].IsDate()) {
    Napi::Date date = info[0].As<Napi::Date>();
    time_t timestamp = static_cast<time_t>(date.ValueOf()) / 1000;
    *DateTime = *localtime(&timestamp);
  } else {
    Napi::Object date_time = info[0].As<Napi::Object>();
    DateTime->tm_year = date_time.Get("year").As<Napi::Number>().DoubleValue() - 1900;
    DateTime->tm_mon = date_time.Get("month").As<Napi::Number>().DoubleValue();
    DateTime->tm_mday = date_time.Get("day").As<Napi::Number>().DoubleValue();
    DateTime->tm_hour = date_time.Get("hours").As<Napi::Number>().DoubleValue();
    DateTime->tm_min = date_time.Get("minutes").As<Napi::Number>().DoubleValue();
    DateTime->tm_sec = date_time.Get("seconds").As<Napi::Number>().DoubleValue();
  }

  if (info.Length() > 1 && info[1].IsFunction()) {
    Napi::Function callback = info[1].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::SETPLCDATETIME
      , DateTime);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->SetPlcDateTime(DateTime);
    delete DateTime;
    if (ret == 0) {
      return env.Undefined();
    } else {
      Napi::Error::New(env, "SetPlcDateTime failed with error code: " + std::to_string(ret)).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Value S7Client::SetPlcSystemDateTime(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() > 0 && info[0].IsFunction()) {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::SETPLCSYSTEMDATETIME);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->SetPlcSystemDateTime();
    if (ret == 0) {
      return env.Undefined();
    } else {
      Napi::Error::New(env, "SetPlcSystemDateTime failed with error code: " + std::to_string(ret)).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

// System Info functions
Napi::Value S7Client::GetOrderCode(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  PS7OrderCode OrderCode = new TS7OrderCode;
  if (info.Length() > 0 && info[0].IsFunction()) {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::GETORDERCODE
      , OrderCode);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->GetOrderCode(OrderCode);
    if (ret == 0) {
      Napi::Object order_code = S7OrderCodeToObject(OrderCode);
      delete OrderCode;
      return order_code;
    } else {
      delete OrderCode;
      std::stringstream err;
      err << "GetOrderCode failed with error code: " << ret;
      Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Object S7Client::S7OrderCodeToObject(PS7OrderCode OrderCode) {
  Napi::EscapableHandleScope scope(Env());

  Napi::Object order_code =Napi::Object::New(Env());
  order_code.Set("Code", Napi::String::New(Env(), OrderCode->Code));
  order_code.Set("V1", Napi::Number::New(Env(), OrderCode->V1));
  order_code.Set("V2", Napi::Number::New(Env(), OrderCode->V2));
  order_code.Set("V3", Napi::Number::New(Env(), OrderCode->V3));

  return scope.Escape(order_code).As<Napi::Object>();
}

Napi::Value S7Client::GetCpuInfo(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  PS7CpuInfo CpuInfo = new TS7CpuInfo;
  if (info.Length() > 0 && info[0].IsFunction()) {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::GETCPUINFO, CpuInfo);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->GetCpuInfo(CpuInfo);
    if (ret == 0) {
      Napi::Object cpu_info = S7CpuInfoToObject(CpuInfo);
      delete CpuInfo;
      return cpu_info;
    } else {
      delete CpuInfo;
      std::stringstream err;
      err << "GetCpuInfo failed with error code: " << ret;
      Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Object S7Client::S7CpuInfoToObject(PS7CpuInfo CpuInfo) {
  Napi::EscapableHandleScope scope(Env());

  Napi::Object cpu_info = Napi::Object::New(Env());
  cpu_info.Set("ModuleTypeName", Napi::String::New(Env(), CpuInfo->ModuleTypeName));
  cpu_info.Set("SerialNumber", Napi::String::New(Env(), CpuInfo->SerialNumber));
  cpu_info.Set("ASName", Napi::String::New(Env(), CpuInfo->ASName));
  cpu_info.Set("Copyright", Napi::String::New(Env(), CpuInfo->Copyright));
  cpu_info.Set("ModuleName", Napi::String::New(Env(), CpuInfo->ModuleName));

  return scope.Escape(cpu_info).As<Napi::Object>();
}


Napi::Value S7Client::GetCpInfo(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  PS7CpInfo CpInfo = new TS7CpInfo;
  if (info.Length() > 0 && info[0].IsFunction()) {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::GETCPINFO, CpInfo);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->GetCpInfo(CpInfo);
    if (ret == 0) {
      Napi::Object cp_info = S7CpInfoToObject(CpInfo);
      delete CpInfo;
      return cp_info;
    } else {
      delete CpInfo;
      std::stringstream err;
      err << "GetCpInfo failed with error code: " << ret;
      Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Object S7Client::S7CpInfoToObject(PS7CpInfo CpInfo) {
  Napi::EscapableHandleScope scope(Env());

  Napi::Object cp_info = Napi::Object::New(Env());
  cp_info.Set("MaxPduLength", Napi::Number::New(Env(), CpInfo->MaxPduLengt));
  cp_info.Set("MaxConnections", Napi::Number::New(Env(), CpInfo->MaxConnections));
  cp_info.Set("MaxMpiRate", Napi::Number::New(Env(), CpInfo->MaxMpiRate));
  cp_info.Set("MaxBusRate", Napi::Number::New(Env(), CpInfo->MaxBusRate));

  return scope.Escape(cp_info).As<Napi::Object>();
}

Napi::Value S7Client::ReadSZL(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!(info[0].IsNumber() || info[1].IsNumber())) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  int id = info[0].As<Napi::Number>().Int32Value();
  int index = info[1].As<Napi::Number>().Int32Value();
  PS7SZL SZL = new TS7SZL;
  int size = sizeof(TS7SZL);

  if (info.Length() > 2 && info[2].IsFunction()) {
    Napi::Function callback = info[2].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::READSZL, SZL
      , id, index, size);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->ReadSZL(id, index, SZL, &size);
    if (ret == 0) {
      Napi:: Buffer ret_buffer = Napi::Buffer<char>::NewOrCopy(env, reinterpret_cast<char*>(SZL), size, S7Client::FreeCallbackSZL);
      delete SZL;
      return ret_buffer;
    } else {
      delete SZL;
      std::stringstream err;
      err << "ReadSZL failed with error code: " << ret;
      Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Value S7Client::ReadSZLList(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  PS7SZLList SZLList = new TS7SZLList;
  int size = sizeof(TS7SZLList);

  if (info.Length() > 0 && info[0].IsFunction()) {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::READSZLLIST, SZLList
      , size);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->ReadSZLList(SZLList, &size);
    if (ret == 0) {
      Napi::Array szl_list = S7SZLListToArray(SZLList, size);
      delete SZLList;
      return szl_list;
    } else {
      delete SZLList;
      std::stringstream err;
      err << "ReadSZLList failed with error code: " << ret;
      Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Array S7Client::S7SZLListToArray(PS7SZLList SZLList, int count) {
  Napi::EscapableHandleScope scope(Env());

  Napi::Array szl_list = Napi::Array::New(Env(), count);
  for (int i = 0; i < count; i++) {
    szl_list.Set(i, Napi::Number::New(Env(), (*SZLList).List[i]));
  }

  return scope.Escape(szl_list).As<Napi::Array>();
}

// Control functions
Napi::Value S7Client::PlcHotStart(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() > 0 && info[0].IsFunction()) {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::PLCHOTSTART);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->PlcHotStart();
    if (ret == 0) {
      return env.Undefined();
    } else {
      Napi::Error::New(env, "PlcHotStart failed with error code: " + std::to_string(ret)).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Value S7Client::PlcColdStart(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() > 0 && info[0].IsFunction()) {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::PLCCOLDSTART);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->PlcColdStart();
    if (ret == 0) {
      return env.Undefined();
    } else {
      Napi::Error::New(env, "PlcColdStart failed with error code: " + std::to_string(ret)).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Value S7Client::PlcStop(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() > 0 && info[0].IsFunction()) {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::PLCSTOP);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->PlcStop();
    if (ret == 0) {
      return env.Undefined();
    } else {
      Napi::Error::New(env, "PlcStop failed with error code: " + std::to_string(ret)).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Value S7Client::CopyRamToRom(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  int timeout = info[0].As<Napi::Number>().Int32Value();

  if (info.Length() > 1 && info[1].IsFunction()) {
    Napi::Function callback = info[1].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::COPYRAMTOROM
      , timeout);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->CopyRamToRom(timeout);
    if (ret == 0) {
      return env.Undefined();
    } else {
      Napi::Error::New(env, "CopyRamToRom failed with error code: " + std::to_string(ret)).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Value S7Client::Compress(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  int timeout = info[0].As<Napi::Number>().Int32Value();

  if (info.Length() > 1 && info[1].IsFunction()) {
    Napi::Function callback = info[1].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::COMPRESS
      , timeout);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->Compress(timeout);

    if (ret == 0) {
      return env.Undefined();
    } else {
      Napi::Error::New(env, "Compress failed with error code: " + std::to_string(ret)).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

// Security functions
Napi::Value S7Client::GetProtection(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  PS7Protection S7Protection = new TS7Protection;

  if (info.Length() > 0 && info[0].IsFunction()) {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::GETPROTECTION
      , S7Protection);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->GetProtection(S7Protection);
    if (ret == 0) {
      Napi::Object protection = S7ProtectionToObject(S7Protection);
      delete S7Protection;
      return protection;
    } else {
      delete S7Protection;
      std::stringstream err;
      err << "GetProtection failed with error code: " << ret;
      Napi::Error::New(env, err.str()).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Object S7Client::S7ProtectionToObject(
    PS7Protection S7Protection
) {
  Napi::EscapableHandleScope scope(Env());

  Napi::Object protection = Napi::Object::New(Env());
  protection.Set("sch_schal", Napi::Number::New(Env(), S7Protection->sch_schal));
  protection.Set("sch_par", Napi::Number::New(Env(), S7Protection->sch_par));
  protection.Set("sch_rel", Napi::Number::New(Env(), S7Protection->sch_rel));
  protection.Set("bart_sch", Napi::Number::New(Env(), S7Protection->bart_sch));
  protection.Set("anl_sch", Napi::Number::New(Env(), S7Protection->anl_sch));

  return scope.Escape(protection).As<Napi::Object>();
}

Napi::Value S7Client::SetSessionPassword(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  std::string *password = new std::string(info[0].As<Napi::String>().Utf8Value());

  if (info.Length() > 1 && info[1].IsFunction()) {
    Napi::Function callback = info[1].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::SETSESSIONPW
      , password);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->SetSessionPassword(&*password->begin());
    delete password;
    if (ret == 0) {
      return env.Undefined();
    } else {
      Napi::Error::New(env, "SetSessionPassword failed with error code: " + std::to_string(ret)).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Value S7Client::ClearSessionPassword(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() > 0 && info[0].IsFunction()) {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::CLEARSESSIONPW);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->ClearSessionPassword();
    if (ret == 0) {
      return env.Undefined();
    } else {
      Napi::Error::New(env, "ClearSessionPassword failed with error code: " + std::to_string(ret)).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

// Properties
Napi::Value S7Client::ExecTime(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int ret = snap7Client->ExecTime();
  if (ret == errLibInvalidObject) {
    Napi::Error::New(env, "ExecTime failed with error code:" + std::to_string(errLibInvalidObject)).ThrowAsJavaScriptException();
    return env.Undefined();
  } else {
    return Napi::Number::New(env, ret);
  }
}

Napi::Value S7Client::LastError(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int ret = snap7Client->LastError();
  if (ret == errLibInvalidObject) {
    Napi::Error::New(env, "LastError failed with error code:" + std::to_string(errLibInvalidObject)).ThrowAsJavaScriptException();
    return env.Undefined();
  } else {
    return Napi::Number::New(env, ret);
  }
}

Napi::Value S7Client::PDURequested(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int ret = snap7Client->PDURequested();
  if (ret == 0) {
    Napi::Error::New(env, "PDURequested failed.").ThrowAsJavaScriptException();
    return env.Undefined();
  } else {
    return Napi::Number::New(env, ret);
  }
}

Napi::Value S7Client::PDULength(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int ret = snap7Client->PDULength();
  if (ret == 0) {
    Napi::Error::New(env, "PDULength failed.").ThrowAsJavaScriptException();
    return env.Undefined();
  } else {
    return Napi::Number::New(env, ret);
  }
}

Napi::Value S7Client::PlcStatus(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() > 0 && info[0].IsFunction()) {
    Napi::Function callback = info[0].As<Napi::Function>();
    IOWorkerClient* worker = new IOWorkerClient(env, this, DataIOFunction::PLCSTATUS);
    worker->Queue();
    return worker->GetPromise();
  } else {
    int ret = snap7Client->PlcStatus();
    if ((ret == S7CpuStatusUnknown) ||
        (ret == S7CpuStatusStop) ||
        (ret == S7CpuStatusRun)) {
      return Napi::Number::New(env, ret);
    } else {
      Napi::Error::New(env, "PlcStatus failed with error code:" + std::to_string(ret)).ThrowAsJavaScriptException();
      return env.Undefined();
    }
  }
}

Napi::Value S7Client::Connected(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  return Napi::Boolean::New(env,
    snap7Client->Connected());
}

Napi::Value S7Client::ErrorText(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  return Napi::String::New(env,
    CliErrorText(info[0].As<Napi::Number>().Int32Value()).c_str());
}

}  // namespace node_snap7
