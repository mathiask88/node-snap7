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

  IOWorker* worker = new IOWorker(env, this, DataIOFunction::CONNECT);
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Client::ConnectTo(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 3) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
  }

  if (!info[0].IsString() || !info[1].IsNumber() || !info[2].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  std::string* remAddress = new std::string(info[0].As<Napi::String>().Utf8Value());
  IOWorker* worker = new IOWorker(env, this, DataIOFunction::CONNECTTO
    , remAddress, info[1].As<Napi::Number>().Int32Value(), info[2].As<Napi::Number>().Int32Value());
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Client::SetConnectionParams(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (!info[0].IsString() || !info[1].IsNumber() ||
      !info[2].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  std::string remAddress = info[0].As<Napi::String>().Utf8Value();
  word LocalTSAP = info[1].As<Napi::Number>().Uint32Value();
  word RemoteTSAP = info[2].As<Napi::Number>().Uint32Value();

  int ret = this->snap7Client->SetConnectionParams(
      remAddress.c_str()
    , LocalTSAP
    , RemoteTSAP);

  return Napi::Boolean::New(env, ret == 0);
}

Napi::Value S7Client::SetConnectionType(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  word type = info[0].As<Napi::Number>().Uint32Value();;

  int  ret = this->snap7Client->SetConnectionType(type);
  return Napi::Boolean::New(env, ret == 0);
}

Napi::Value S7Client::Disconnect(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int ret = this->snap7Client->Disconnect();
  return Napi::Boolean::New(env, ret == 0);
}

Napi::Value S7Client::GetParam(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  int pData;
  int returnValue = this->snap7Client->GetParam(info[0].As<Napi::Number>().Int32Value()
    , &pData);

  if (returnValue == 0) {
    return Napi::Number::New(env, pData);
  } else {
    return Napi::Number::New(env, returnValue);
  }
}

Napi::Value S7Client::SetParam(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (!(info[0].IsNumber() || info[1].IsNumber())) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  int pData = info[1].As<Napi::Number>().Int32Value();
  int ret = this->snap7Client->SetParam(info[0].As<Napi::Number>().Int32Value(), &pData);
  return Napi::Boolean::New(env, ret == 0);
}

// Data I/O Main functions
void IOWorker::Execute() {
  this->s7client->mutex.lock();

  switch (caller) {
  case DataIOFunction::CONNECTTO:
      returnValue = this->s7client->snap7Client->ConnectTo(
        static_cast<std::string*>(pData)->c_str(), int1, int2);
      break;

  case DataIOFunction::CONNECT:
      returnValue = this->s7client->snap7Client->Connect();
      break;

  case DataIOFunction::READAREA:
      returnValue = this->s7client->snap7Client->ReadArea(int1, int2, int3, int4
        , int5, pData);
      break;

  case DataIOFunction::WRITEAREA:
      returnValue = this->s7client->snap7Client->WriteArea(int1, int2, int3, int4
        , int5, pData);
      break;

  case DataIOFunction::READMULTI:
      returnValue = this->s7client->snap7Client->ReadMultiVars(
          static_cast<PS7DataItem>(pData), int1);
      break;

  case DataIOFunction::WRITEMULTI:
      returnValue = this->s7client->snap7Client->WriteMultiVars(
          static_cast<PS7DataItem>(pData), int1);
      break;

  case DataIOFunction::PLCSTATUS:
      returnValue = this->s7client->snap7Client->PlcStatus();
      if ((returnValue == S7CpuStatusUnknown) ||
          (returnValue == S7CpuStatusStop) ||
          (returnValue == S7CpuStatusRun)) {
        int1 = returnValue;
        returnValue = 0;
      }
      break;

  case DataIOFunction::CLEARSESSIONPW:
      returnValue = this->s7client->snap7Client->ClearSessionPassword();
      break;

  case DataIOFunction::SETSESSIONPW:
      returnValue = this->s7client->snap7Client->SetSessionPassword(
        &*static_cast<std::string*>(pData)->begin());
      break;

  case DataIOFunction::GETPROTECTION:
      returnValue = this->s7client->snap7Client->GetProtection(
          static_cast<PS7Protection>(pData));
      break;

  case DataIOFunction::PLCSTOP:
      returnValue = this->s7client->snap7Client->PlcStop();
      break;

  case DataIOFunction::PLCCOLDSTART:
      returnValue = this->s7client->snap7Client->PlcColdStart();
      break;

  case DataIOFunction::PLCHOTSTART:
      returnValue = this->s7client->snap7Client->PlcHotStart();
      break;

  case DataIOFunction::GETCPINFO:
      returnValue = this->s7client->snap7Client->GetCpInfo(
          static_cast<PS7CpInfo>(pData));
      break;

  case DataIOFunction::GETCPUINFO:
      returnValue = this->s7client->snap7Client->GetCpuInfo(
          static_cast<PS7CpuInfo>(pData));
      break;

  case DataIOFunction::GETORDERCODE:
      returnValue = this->s7client->snap7Client->GetOrderCode(
          static_cast<PS7OrderCode>(pData));
      break;

  case DataIOFunction::SETPLCSYSTEMDATETIME:
      returnValue = this->s7client->snap7Client->SetPlcSystemDateTime();
      break;

  case DataIOFunction::GETPLCDATETIME:
      returnValue = this->s7client->snap7Client->GetPlcDateTime(
          static_cast<tm*>(pData));
      break;

  case DataIOFunction::SETPLCDATETIME:
      returnValue = this->s7client->snap7Client->SetPlcDateTime(
          static_cast<tm*>(pData));
      break;

  case DataIOFunction::COMPRESS:
      returnValue = this->s7client->snap7Client->Compress(int1);
      break;

  case DataIOFunction::COPYRAMTOROM:
      returnValue = this->s7client->snap7Client->CopyRamToRom(int1);
      break;

  case DataIOFunction::DBFILL:
      returnValue = this->s7client->snap7Client->DBFill(int1, int2);
      break;

  case DataIOFunction::DBGET:
      returnValue = this->s7client->snap7Client->DBGet(int1, pData, &int2);
      break;

  case DataIOFunction::DELETEBLOCK:
      returnValue = this->s7client->snap7Client->Delete(int1, int2);
      break;

  case DataIOFunction::DOWNLOAD:
      returnValue = this->s7client->snap7Client->Download(int1, pData, int2);
      break;

  case DataIOFunction::FULLUPLOAD:
      returnValue = this->s7client->snap7Client->FullUpload(int1, int2, pData, &int3);
      break;

  case DataIOFunction::UPLOAD:
      returnValue = this->s7client->snap7Client->Upload(int1, int2, pData, &int3);
      break;

  case DataIOFunction::LISTBLOCKSOFTYPE:
      returnValue = this->s7client->snap7Client->ListBlocksOfType(int1
        , static_cast<PS7BlocksOfType>(pData), &int2);
      break;

  case DataIOFunction::GETAGBLOCKINFO:
      returnValue = this->s7client->snap7Client->GetAgBlockInfo(int1, int2
        , static_cast<PS7BlockInfo>(pData));
      break;

  case DataIOFunction::LISTBLOCKS:
      returnValue = this->s7client->snap7Client->ListBlocks(
          static_cast<PS7BlocksList>(pData));
      break;

  case DataIOFunction::READSZLLIST:
      returnValue = this->s7client->snap7Client->ReadSZLList(
          static_cast<PS7SZLList>(pData), &int1);
      break;

  case DataIOFunction::READSZL:
      returnValue = this->s7client->snap7Client->ReadSZL(int1, int2
        , static_cast<PS7SZL>(pData), &int3);
      break;
  }

  this->s7client->mutex.unlock();
}

void IOWorker::OnOK() {
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
    if (returnValue == 0) {
      val = Napi::Buffer<char>::New(Env(),
          static_cast<char*>(pData)
        , static_cast<size_t>(int4) * this->s7client->GetByteCountFromWordLen(int5)
        , S7Client::FreeCallback);
    } else {
      delete[] static_cast<char*>(pData);
    }
    break;

  case DataIOFunction::READMULTI:
      if (returnValue == 0) {
        val = this->s7client->S7DataItemToArray(static_cast<PS7DataItem>(pData)
          , int1, true);
      } else {
        for (int i = 0; i < int1; i++) {
          delete[] static_cast<char*>(static_cast<PS7DataItem>(pData)[i].pdata);
        }
        delete[] static_cast<PS7DataItem>(pData);
      }
      break;

  case DataIOFunction::WRITEMULTI:
      if (returnValue == 0) {
        val = this->s7client->S7DataItemToArray(static_cast<PS7DataItem>(pData)
          , int1, false);
      } else {
        delete[] static_cast<PS7DataItem>(pData);
      }
      break;

  case DataIOFunction::GETPROTECTION:
      if (returnValue == 0) {
        val = this->s7client->S7ProtectionToObject(
          static_cast<PS7Protection>(pData));
      }
      delete static_cast<PS7Protection>(pData);
      break;

  case DataIOFunction::GETCPINFO:
      if (returnValue == 0) {
        val = this->s7client->S7CpInfoToObject(
          static_cast<PS7CpInfo>(pData));
      }
      delete static_cast<PS7CpInfo>(pData);
      break;

  case DataIOFunction::GETCPUINFO:
      if (returnValue == 0) {
        val = this->s7client->S7CpuInfoToObject(
          static_cast<PS7CpuInfo>(pData));
      }
      delete static_cast<PS7CpuInfo>(pData);
      break;

  case DataIOFunction::GETORDERCODE:
      if (returnValue == 0) {
        val = this->s7client->S7OrderCodeToObject(
          static_cast<PS7OrderCode>(pData));
      }
      delete static_cast<PS7OrderCode>(pData);
      break;

  case DataIOFunction::GETPLCDATETIME:
      if (returnValue == 0) {
        double timestamp = static_cast<double>(mktime(static_cast<tm*>(pData)));
        val = Napi::Date::New(Env(), timestamp * 1000);
      }
      delete static_cast<tm*>(pData);
      break;

  case DataIOFunction::SETPLCDATETIME:
      delete static_cast<tm*>(pData);
      break;

  case DataIOFunction::PLCSTATUS:
      if (returnValue == 0) {
        val = Napi::Number::New(Env(), int1);
      }
      break;

  case DataIOFunction::DBGET:
      if (returnValue == 0) {
        val = Napi::Buffer<char>::New(Env(),
            static_cast<char*>(pData)
          , int2
          , S7Client::FreeCallback);
      } else {
        delete[] static_cast<char*>(pData);
      }
      break;

  case DataIOFunction::FULLUPLOAD:
  case DataIOFunction::UPLOAD:
      if (returnValue == 0) {
        val = Napi::Buffer<char>::New(Env(),
            static_cast<char*>(pData)
          , int3
          , S7Client::FreeCallback);
      } else {
        delete[] static_cast<char*>(pData);
      }
      break;

  case DataIOFunction::LISTBLOCKSOFTYPE:
      if (returnValue == 0) {
        val = this->s7client->S7BlocksOfTypeToArray(
            static_cast<PS7BlocksOfType>(pData), int2);
      }
      delete[] static_cast<PS7BlocksOfType>(pData);
      break;

  case DataIOFunction::GETAGBLOCKINFO:
      if (returnValue == 0) {
        Napi::Object block_info = this->s7client->S7BlockInfoToObject(
            static_cast<PS7BlockInfo>(pData));
        val = block_info;
      }
      delete static_cast<PS7BlockInfo>(pData);
      break;

  case DataIOFunction::LISTBLOCKS:
      if (returnValue == 0) {
        Napi::Object blocks_list = this->s7client->S7BlocksListToObject(
            static_cast<PS7BlocksList>(pData));
        val = blocks_list;
      }
      delete static_cast<PS7BlocksList>(pData);
      break;

  case DataIOFunction::READSZLLIST:
      if (returnValue == 0) {
        Napi::Array szl_list = this->s7client->S7SZLListToArray(
            static_cast<PS7SZLList>(pData), int1);
        val = szl_list;
      }
      delete static_cast<PS7SZLList>(pData);
      break;

  case DataIOFunction::READSZL:
      if (returnValue == 0) {
        val = Napi::Buffer<char>::New(Env(),
            reinterpret_cast<char*>(static_cast<PS7SZL>(pData))
          , int3
          , S7Client::FreeCallbackSZL);
      } else {
        delete static_cast<PS7SZL>(pData);
      }
      break;
  }

  if (returnValue == 0){
    m_deferred.Resolve(val);
  } else {
    m_deferred.Reject(Napi::Number::New(Env(), returnValue));
  }
}

Napi::Value S7Client::ReadArea(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 5)
    Napi::TypeError::New(env, "Wrong number of Arguments").ThrowAsJavaScriptException();

  if (!info[0].IsNumber() || !info[1].IsNumber() ||
      !info[2].IsNumber() || !info[3].IsNumber() ||
      !info[4].IsNumber())
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();

  int amount = info[3].As<Napi::Number>().Int32Value();
  int byteCount = this->GetByteCountFromWordLen(info[4].As<Napi::Number>().Int32Value());
  int size = amount * byteCount;
  char *bufferData = new char[size];

  IOWorker* worker = new IOWorker(env, this, DataIOFunction::READAREA
    , bufferData, info[0].As<Napi::Number>().Int32Value()
    , info[1].As<Napi::Number>().Int32Value()
    , info[2].As<Napi::Number>().Int32Value()
    , info[3].As<Napi::Number>().Int32Value()
    , info[4].As<Napi::Number>().Int32Value());
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Client::WriteArea(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 6)
    Napi::TypeError::New(env, "Wrong number of Arguments").ThrowAsJavaScriptException();

  if (!info[0].IsNumber() || !info[1].IsNumber() ||
      !info[2].IsNumber() || !info[3].IsNumber() ||
      !info[4].IsNumber() || !info[5].IsBuffer())
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();

  IOWorker* worker = new IOWorker(env, this, DataIOFunction::WRITEAREA
    , info[5].As<Napi::Buffer<char>>().Data()
    , info[0].As<Napi::Number>().Int32Value()
    , info[1].As<Napi::Number>().Int32Value()
    , info[2].As<Napi::Number>().Int32Value()
    , info[3].As<Napi::Number>().Int32Value()
    , info[4].As<Napi::Number>().Int32Value());
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Client::ReadMultiVars(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
  }

  if (!info[0].IsArray()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  const Napi::Array data_arr = info[0].As<Napi::Array>();
  uint32_t len = data_arr.Length();
  if (len == 0) {
    Napi::TypeError::New(env, "Array needs at least 1 item").ThrowAsJavaScriptException();
  } else if (len > MaxVars) {
    std::stringstream err;
    err << "Array exceeds max variables (" << MaxVars
        << ") that can be transferred with ReadMultiVars()";
    Napi::TypeError::New(env, err.str()).ThrowAsJavaScriptException();
  }

  for (uint32_t i = 0; i < len; i++) {
    if (!data_arr[i].IsObject()) {
      Napi::TypeError::New(env, "Wrong argument structure").ThrowAsJavaScriptException();
    } else {
      Napi::Object data_obj = data_arr[i].As<Napi::Object>();
      if (!data_obj.Has("Area") ||
          !data_obj.Has("WordLen") ||
          !data_obj.Has("Start") ||
          !data_obj.Has("Amount")) {
        Napi::TypeError::New(env, "Wrong argument structure").ThrowAsJavaScriptException();
      } else if (!data_obj.Get("Area").IsNumber() ||
                 !data_obj.Get("WordLen").IsNumber() ||
                 !data_obj.Get("Start").IsNumber() ||
                 !data_obj.Get("Amount").IsNumber()) {
        Napi::TypeError::New(env, "Wrong argument structure").ThrowAsJavaScriptException();
      } else if (data_obj.Get("Area").As<Napi::Number>().Int32Value() == S7AreaDB) {
        if (!data_obj.Has("DBNumber")) {
          Napi::TypeError::New(env, "Wrong argument structure").ThrowAsJavaScriptException();
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

    byteCount = this->GetByteCountFromWordLen(Items[i].WordLen);
    size = Items[i].Amount * byteCount;
    Items[i].pdata = new char[size];
  }

  IOWorker* worker = new IOWorker(env, this, DataIOFunction::READMULTI
    , Items, len);
  worker->Queue();

  return worker->GetPromise();
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
          , Napi::Buffer<char>::New(Env(),
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
  }

  if (!info[0].IsArray()) {
    Napi::TypeError::New(info.Env(), "Wrong arguments").ThrowAsJavaScriptException();
  }

  const Napi::Array data_arr = info[0].As<Napi::Array>();
  uint32_t len = data_arr.Length();
  if (len == 0) {
    Napi::TypeError::New(env, "Array needs at least 1 item").ThrowAsJavaScriptException();
  } else if (len > MaxVars) {
    std::stringstream err;
    err << "Array exceeds max variables (" << MaxVars
        << ") that can be transferred with WriteMultiVars()";
    Napi::TypeError::New(env, err.str()).ThrowAsJavaScriptException();
  }

  for (uint32_t i = 0; i < len; i++) {
    if (!data_arr[i].IsObject()) {
      Napi::TypeError::New(env, "Wrong argument structure").ThrowAsJavaScriptException();
    } else {
      Napi::Object data_obj = data_arr[i].As<Napi::Object>();
      if (!data_obj.Has("Area") ||
          !data_obj.Has("WordLen") ||
          !data_obj.Has("Start") ||
          !data_obj.Has("Amount") ||
          !data_obj.Has("Data")) {
        Napi::TypeError::New(env, "Wrong argument structure").ThrowAsJavaScriptException();
      } else if (!data_obj.Get("Area").IsNumber() ||
                 !data_obj.Get("WordLen").IsNumber() ||
                 !data_obj.Get("Start").IsNumber() ||
                 !data_obj.Get("Amount").IsNumber() ||
                 !data_obj.Get("Data").IsBuffer()) {
        Napi::TypeError::New(env, "Wrong argument structure").ThrowAsJavaScriptException();
      } else if (data_obj.Get("Area").As<Napi::Number>().Int32Value() == S7AreaDB) {
        if (!data_obj.Has("DBNumber")) {
          Napi::TypeError::New(env, "Wrong argument structure").ThrowAsJavaScriptException();
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

  IOWorker* worker = new IOWorker(env, this, DataIOFunction::WRITEMULTI
    , Items, len);
  worker->Queue();

  return worker->GetPromise();
}

// Directory functions
Napi::Value S7Client::ListBlocks(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  PS7BlocksList BlocksList = new TS7BlocksList;
  IOWorker* worker = new IOWorker(env, this, DataIOFunction::LISTBLOCKS
    , BlocksList);
  worker->Queue();

  return worker->GetPromise();
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

  if (!info[0].IsNumber() || !info[1].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  PS7BlockInfo BlockInfo = new TS7BlockInfo;
  IOWorker* worker = new IOWorker(env, this, DataIOFunction::GETAGBLOCKINFO
    , BlockInfo, info[0].As<Napi::Number>().Int32Value(), info[1].As<Napi::Number>().Int32Value());
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Client::GetPgBlockInfo(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (!info[0].IsBuffer()) {
    Napi::TypeError::New(env, "Argument should be a Buffer").ThrowAsJavaScriptException();
  }

  PS7BlockInfo BlockInfo = new TS7BlockInfo;
  Napi::Buffer<char> buffer = info[0].As<Napi::Buffer<char>>();

  int returnValue = this->snap7Client->GetPgBlockInfo(
    buffer.Data(), BlockInfo, buffer.Length());

  if (returnValue == 0) {
    Napi::Object block_info = this->S7BlockInfoToObject(BlockInfo);
    delete BlockInfo;
    return block_info;
  } else {
    delete BlockInfo;
    return Napi::Boolean::New(env, false);
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

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  int BlockNum = sizeof(TS7BlocksOfType) / sizeof(PS7BlocksOfType);
  PS7BlocksOfType BlockList = new TS7BlocksOfType[BlockNum];
  IOWorker* worker = new IOWorker(env, this, DataIOFunction::LISTBLOCKSOFTYPE
    , BlockList, info[0].As<Napi::Number>().Int32Value(), BlockNum);
  worker->Queue();

  return worker->GetPromise();
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

  if (!info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  int size = info[2].As<Napi::Number>().Int32Value();
  char *bufferData = new char[size];

  IOWorker* worker = new IOWorker(env, this, DataIOFunction::UPLOAD
    , bufferData, info[0].As<Napi::Number>().Int32Value(), info[1].As<Napi::Number>().Int32Value(), size);
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Client::FullUpload(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  int size = info[2].As<Napi::Number>().Int32Value();
  char *bufferData = new char[size];

  IOWorker* worker = new IOWorker(env, this, DataIOFunction::FULLUPLOAD
    , bufferData, info[0].As<Napi::Number>().Int32Value(), info[1].As<Napi::Number>().Int32Value(), size);
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Client::Download(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber() || info[1].IsBuffer()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  Napi::Buffer<char> buffer = info[1].As<Napi::Buffer<char>>();

  IOWorker* worker = new IOWorker(env, this, DataIOFunction::DOWNLOAD
    , buffer.Data(), info[0].As<Napi::Number>().Int32Value()
    , buffer.Length());
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Client::Delete(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber() || !info[1].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  IOWorker* worker = new IOWorker(env, this, DataIOFunction::DELETEBLOCK
    , info[0].As<Napi::Number>().Int32Value()
    , info[1].As<Napi::Number>().Int32Value());
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Client::DBGet(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  int size = 65536;
  char *bufferData = new char[size];
  IOWorker* worker = new IOWorker(env, this, DataIOFunction::DBGET
    , bufferData, info[0].As<Napi::Number>().Int32Value(), size);
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Client::DBFill(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber() || !(info[1].IsNumber() || info[1].IsString())) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  int fill;
  if (info[1].IsNumber()) {
    fill = info[1].As<Napi::Number>().Int32Value();
  } else {
    std::string fillstr = info[1].As<Napi::String>().Utf8Value();
    fill = static_cast<int>(*fillstr.c_str());
  }

  IOWorker* worker = new IOWorker(env, this, DataIOFunction::DBFILL
    , info[0].As<Napi::Number>().Int32Value(), fill);
  worker->Queue();

  return worker->GetPromise();
}

// Date/Time functions
Napi::Value S7Client::GetPlcDateTime(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  tm *DateTime = new tm;
  IOWorker* worker = new IOWorker(env, this, DataIOFunction::GETPLCDATETIME
    , DateTime);
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Client::SetPlcDateTime(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (!(info[0].IsObject() || info[0].IsDate())) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
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

  IOWorker* worker = new IOWorker(env, this, DataIOFunction::SETPLCDATETIME
    , DateTime);
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Client::SetPlcSystemDateTime(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  IOWorker* worker = new IOWorker(env, this, DataIOFunction::SETPLCSYSTEMDATETIME);
  worker->Queue();

  return worker->GetPromise();
}

// System Info functions
Napi::Value S7Client::GetOrderCode(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  PS7OrderCode OrderCode = new TS7OrderCode;
  IOWorker* worker = new IOWorker(env, this, DataIOFunction::GETORDERCODE
    , OrderCode);
  worker->Queue();

  return worker->GetPromise();
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
  IOWorker* worker = new IOWorker(env, this, DataIOFunction::GETCPUINFO, CpuInfo);
  worker->Queue();

  return worker->GetPromise();
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
  IOWorker* worker = new IOWorker(env, this, DataIOFunction::GETCPINFO, CpInfo);
  worker->Queue();

  return worker->GetPromise();
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

  if (!(info[0].IsNumber() || info[1].IsNumber())) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  PS7SZL SZL = new TS7SZL;
  int size = sizeof(TS7SZL);
  IOWorker* worker = new IOWorker(env, this, DataIOFunction::READSZL, SZL
    , info[0].As<Napi::Number>().Int32Value()
    , info[1].As<Napi::Number>().Int32Value(), size);
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Client::ReadSZLList(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  PS7SZLList SZLList = new TS7SZLList;
  int size = sizeof(TS7SZLList);
  IOWorker* worker = new IOWorker(env, this, DataIOFunction::READSZLLIST, SZLList
    , size);
  worker->Queue();

  return worker->GetPromise();
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

  IOWorker* worker = new IOWorker(env, this, DataIOFunction::PLCHOTSTART);
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Client::PlcColdStart(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  IOWorker* worker = new IOWorker(env, this, DataIOFunction::PLCCOLDSTART);
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Client::PlcStop(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  IOWorker* worker = new IOWorker(env, this, DataIOFunction::PLCSTOP);
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Client::CopyRamToRom(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  IOWorker* worker = new IOWorker(env, this, DataIOFunction::COPYRAMTOROM
    , info[0].As<Napi::Number>().Int32Value());
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Client::Compress(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  IOWorker* worker = new IOWorker(env, this, DataIOFunction::COMPRESS
    , info[0]);
  worker->Queue();

  return worker->GetPromise();
}

// Security functions
Napi::Value S7Client::GetProtection(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  PS7Protection S7Protection = new TS7Protection;
  IOWorker* worker = new IOWorker(env, this, DataIOFunction::GETPROTECTION
    , S7Protection);
  worker->Queue();

  return worker->GetPromise();
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

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  std::string *password = new std::string(info[0].As<Napi::String>().Utf8Value());
  IOWorker* worker = new IOWorker(env, this, DataIOFunction::SETSESSIONPW
    , password);
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Client::ClearSessionPassword(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  IOWorker* worker = new IOWorker(env, this, DataIOFunction::CLEARSESSIONPW);
  worker->Queue();

  return worker->GetPromise();
}

// Properties
Napi::Value S7Client::ExecTime(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int returnValue = this->snap7Client->ExecTime();
  if (returnValue == errLibInvalidObject) {
    return Napi::Boolean::New(env, false);
  } else {
    return Napi::Number::New(env, returnValue);
  }
}

Napi::Value S7Client::LastError(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  return Napi::Number::New(env,
    this->snap7Client->LastError());
}

Napi::Value S7Client::PDURequested(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int returnValue = this->snap7Client->PDURequested();
  if (returnValue == 0) {
    return Napi::Boolean::New(env, false);
  } else {
    return Napi::Number::New(env, returnValue);
  }
}

Napi::Value S7Client::PDULength(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int returnValue = this->snap7Client->PDULength();
  if (returnValue == 0) {
    return Napi::Boolean::New(env, false);
  } else {
    return Napi::Number::New(env, returnValue);
  }
}

Napi::Value S7Client::PlcStatus(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  IOWorker* worker = new IOWorker(env, this, DataIOFunction::PLCSTATUS);
  worker->Queue();

  return worker->GetPromise();
}

Napi::Value S7Client::Connected(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  return Napi::Boolean::New(env,
    this->snap7Client->Connected());
}

Napi::Value S7Client::ErrorText(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }

  return Napi::String::New(env,
    CliErrorText(info[0].As<Napi::Number>().Int32Value()).c_str());
}

}  // namespace node_snap7
