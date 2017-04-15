/*
 * Copyright (c) 2017, Mathias Küsel
 * MIT License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

#include <node_snap7_client.h>
#include <node_buffer.h>
#include <sstream>

namespace node_snap7 {

Nan::Persistent<v8::FunctionTemplate> S7Client::constructor;

NAN_MODULE_INIT(S7Client::Init) {
  Nan::HandleScope scope;

  v8::Local<v8::FunctionTemplate> tpl;
  tpl = Nan::New<v8::FunctionTemplate>(S7Client::New);

  v8::Local<v8::String> name = Nan::New<v8::String>("S7Client")
    .ToLocalChecked();

  tpl->SetClassName(name);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Setup the prototype
  // Control functions
  Nan::SetPrototypeMethod(
      tpl
    , "Connect"
    , S7Client::Connect);
  Nan::SetPrototypeMethod(
      tpl
    , "ConnectTo"
    , S7Client::ConnectTo);
  Nan::SetPrototypeMethod(
      tpl
    , "SetConnectionParams"
    , S7Client::SetConnectionParams);
  Nan::SetPrototypeMethod(
      tpl
    , "SetConnectionType"
    , S7Client::SetConnectionType);
  Nan::SetPrototypeMethod(
      tpl
    , "Disconnect"
    , S7Client::Disconnect);
  Nan::SetPrototypeMethod(
      tpl
    , "GetParam"
    , S7Client::GetParam);
  Nan::SetPrototypeMethod(
      tpl
    , "SetParam"
    , S7Client::SetParam);

  // Data I/O Main functions
  Nan::SetPrototypeMethod(
      tpl
    , "ReadArea"
    , S7Client::ReadArea);
  Nan::SetPrototypeMethod(
      tpl
    , "WriteArea"
    , S7Client::WriteArea);
  Nan::SetPrototypeMethod(
      tpl
    , "ReadMultiVars"
    , S7Client::ReadMultiVars);
  Nan::SetPrototypeMethod(
      tpl
    , "WriteMultiVars"
    , S7Client::WriteMultiVars);

  // Directory functions
  Nan::SetPrototypeMethod(
      tpl
    , "ListBlocks"
    , S7Client::ListBlocks);
  Nan::SetPrototypeMethod(
      tpl
    , "GetAgBlockInfo"
    , S7Client::GetAgBlockInfo);
  Nan::SetPrototypeMethod(
      tpl
    , "GetPgBlockInfo"
    , S7Client::GetPgBlockInfo);
  Nan::SetPrototypeMethod(
      tpl
    , "ListBlocksOfType"
    , S7Client::ListBlocksOfType);

  // Blocks functions
  Nan::SetPrototypeMethod(
      tpl
    , "Upload"
    , S7Client::Upload);
  Nan::SetPrototypeMethod(
      tpl
    , "FullUpload"
    , S7Client::FullUpload);
  Nan::SetPrototypeMethod(
      tpl
    , "Download"
    , S7Client::Download);
  Nan::SetPrototypeMethod(
      tpl
    , "Delete"
    , S7Client::Delete);
  Nan::SetPrototypeMethod(
      tpl
    , "DBGet"
    , S7Client::DBGet);
  Nan::SetPrototypeMethod(
      tpl
    , "DBFill"
    , S7Client::DBFill);

  // Date/Time functions
  Nan::SetPrototypeMethod(
      tpl
    , "GetPlcDateTime"
    , S7Client::GetPlcDateTime);
  Nan::SetPrototypeMethod(
      tpl
    , "SetPlcDateTime"
    , S7Client::SetPlcDateTime);
  Nan::SetPrototypeMethod(
      tpl
    , "SetPlcSystemDateTime"
    , S7Client::SetPlcSystemDateTime);

  // System Info functions
  Nan::SetPrototypeMethod(
      tpl
    , "GetOrderCode"
    , S7Client::GetOrderCode);
  Nan::SetPrototypeMethod(
      tpl
    , "GetCpuInfo"
    , S7Client::GetCpuInfo);
  Nan::SetPrototypeMethod(
      tpl
    , "GetCpInfo"
    , S7Client::GetCpInfo);
  Nan::SetPrototypeMethod(
      tpl
    , "ReadSZL"
    , S7Client::ReadSZL);
  Nan::SetPrototypeMethod(
      tpl
    , "ReadSZLList"
    , S7Client::ReadSZLList);

  // Control functions
  Nan::SetPrototypeMethod(
      tpl
    , "PlcHotStart"
    , S7Client::PlcHotStart);
  Nan::SetPrototypeMethod(
      tpl
    , "PlcColdStart"
    , S7Client::PlcColdStart);
  Nan::SetPrototypeMethod(
      tpl
    , "PlcStop"
    , S7Client::PlcStop);
  Nan::SetPrototypeMethod(
      tpl
    , "CopyRamToRom"
    , S7Client::CopyRamToRom);
  Nan::SetPrototypeMethod(
      tpl
    , "Compress"
    , S7Client::Compress);

  // Security functions
  Nan::SetPrototypeMethod(
      tpl
    , "GetProtection"
    , S7Client::GetProtection);
  Nan::SetPrototypeMethod(
      tpl
    , "SetSessionPassword"
    , S7Client::SetSessionPassword);
  Nan::SetPrototypeMethod(
      tpl
    , "ClearSessionPassword"
    , S7Client::ClearSessionPassword);

  // Properties
  Nan::SetPrototypeMethod(
      tpl
    , "ExecTime"
    , S7Client::ExecTime);
  Nan::SetPrototypeMethod(
      tpl
    , "LastError"
    , S7Client::LastError);
  Nan::SetPrototypeMethod(
      tpl
    , "PDURequested"
    , S7Client::PDURequested);
  Nan::SetPrototypeMethod(
      tpl
    , "PDULength"
    , S7Client::PDULength);
  Nan::SetPrototypeMethod(
      tpl
    , "PlcStatus"
    , S7Client::PlcStatus);
  Nan::SetPrototypeMethod(
      tpl
    , "Connected"
    , S7Client::Connected);

  // Error to text function
  Nan::SetPrototypeMethod(
      tpl
    , "ErrorText"
    , S7Client::ErrorText);

  // Error codes
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errNegotiatingPDU").ToLocalChecked()
    , Nan::New<v8::Integer>(errNegotiatingPDU)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliInvalidParams").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliInvalidParams)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliJobPending").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliJobPending)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliTooManyItems").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliTooManyItems)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliInvalidWordLen").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliInvalidWordLen)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliPartialDataWritten").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliPartialDataWritten)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliSizeOverPDU").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliSizeOverPDU)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliInvalidPlcAnswer").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliInvalidPlcAnswer)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliAddressOutOfRange").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliAddressOutOfRange)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliInvalidTransportSize").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliInvalidTransportSize)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliWriteDataSizeMismatch").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliWriteDataSizeMismatch)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliItemNotAvailable").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliItemNotAvailable)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliInvalidValue").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliInvalidValue)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliCannotStartPLC").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliCannotStartPLC)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliAlreadyRun").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliAlreadyRun)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliCannotStopPLC").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliCannotStopPLC)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliCannotCopyRamToRom").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliCannotCopyRamToRom)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliCannotCompress").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliCannotCompress)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliAlreadyStop").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliAlreadyStop)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliFunNotAvailable").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliFunNotAvailable)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliUploadSequenceFailed").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliUploadSequenceFailed)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliInvalidDataSizeRecvd").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliInvalidDataSizeRecvd)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliInvalidBlockType").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliInvalidBlockType)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliInvalidBlockNumber").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliInvalidBlockNumber)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliInvalidBlockSize").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliInvalidBlockSize)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliDownloadSequenceFailed").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliDownloadSequenceFailed)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliInsertRefused").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliInsertRefused)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliDeleteRefused").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliDeleteRefused)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliNeedPassword").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliNeedPassword)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliInvalidPassword").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliInvalidPassword)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliNoPasswordToSetOrClear").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliNoPasswordToSetOrClear)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliJobTimeout").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliJobTimeout)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliPartialDataRead").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliPartialDataRead)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliBufferTooSmall").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliBufferTooSmall)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliFunctionRefused").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliFunctionRefused)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliDestroying").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliDestroying)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliInvalidParamNumber").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliInvalidParamNumber)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("errCliCannotChangeParam").ToLocalChecked()
    , Nan::New<v8::Integer>(errCliCannotChangeParam)
    , v8::ReadOnly);

  // Client Connection Type
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("CONNTYPE_PG").ToLocalChecked()
    , Nan::New<v8::Integer>(CONNTYPE_PG)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("CONNTYPE_OP").ToLocalChecked()
    , Nan::New<v8::Integer>(CONNTYPE_OP)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("CONNTYPE_BASIC").ToLocalChecked()
    , Nan::New<v8::Integer>(CONNTYPE_BASIC)
    , v8::ReadOnly);

  // CPU Status codes
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("S7CpuStatusUnknown").ToLocalChecked()
    , Nan::New<v8::Integer>(S7CpuStatusUnknown)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("S7CpuStatusRun").ToLocalChecked()
    , Nan::New<v8::Integer>(S7CpuStatusRun)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("S7CpuStatusStop").ToLocalChecked()
    , Nan::New<v8::Integer>(S7CpuStatusStop)
    , v8::ReadOnly);

  // Area ID
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("S7AreaPE").ToLocalChecked()
    , Nan::New<v8::Integer>(S7AreaPE)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("S7AreaPA").ToLocalChecked()
    , Nan::New<v8::Integer>(S7AreaPA)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("S7AreaMK").ToLocalChecked()
    , Nan::New<v8::Integer>(S7AreaMK)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("S7AreaDB").ToLocalChecked()
    , Nan::New<v8::Integer>(S7AreaDB)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("S7AreaCT").ToLocalChecked()
    , Nan::New<v8::Integer>(S7AreaCT)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("S7AreaTM").ToLocalChecked()
    , Nan::New<v8::Integer>(S7AreaTM)
    , v8::ReadOnly);

  // Word Length
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("S7WLBit").ToLocalChecked()
    , Nan::New<v8::Integer>(S7WLBit)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("S7WLByte").ToLocalChecked()
    , Nan::New<v8::Integer>(S7WLByte)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("S7WLWord").ToLocalChecked()
    , Nan::New<v8::Integer>(S7WLWord)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("S7WLDWord").ToLocalChecked()
    , Nan::New<v8::Integer>(S7WLDWord)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("S7WLReal").ToLocalChecked()
    , Nan::New<v8::Integer>(S7WLReal)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("S7WLCounter").ToLocalChecked()
    , Nan::New<v8::Integer>(S7WLCounter)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("S7WLTimer").ToLocalChecked()
    , Nan::New<v8::Integer>(S7WLTimer)
    , v8::ReadOnly);

  // Block type
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("Block_OB").ToLocalChecked()
    , Nan::New<v8::Integer>(Block_OB)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("Block_DB").ToLocalChecked()
    , Nan::New<v8::Integer>(Block_DB)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("Block_SDB").ToLocalChecked()
    , Nan::New<v8::Integer>(Block_SDB)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("Block_FC").ToLocalChecked()
    , Nan::New<v8::Integer>(Block_FC)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("Block_SFC").ToLocalChecked()
    , Nan::New<v8::Integer>(Block_SFC)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("Block_FB").ToLocalChecked()
    , Nan::New<v8::Integer>(Block_FB)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("Block_SFB").ToLocalChecked()
    , Nan::New<v8::Integer>(Block_SFB)
    , v8::ReadOnly);

  // Sub Block Type
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("SubBlk_OB").ToLocalChecked()
    , Nan::New<v8::Integer>(SubBlk_OB)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("SubBlk_SDB").ToLocalChecked()
    , Nan::New<v8::Integer>(SubBlk_SDB)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("SubBlk_FC").ToLocalChecked()
    , Nan::New<v8::Integer>(SubBlk_FC)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("SubBlk_SFC").ToLocalChecked()
    , Nan::New<v8::Integer>(SubBlk_SFC)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("SubBlk_FB").ToLocalChecked()
    , Nan::New<v8::Integer>(SubBlk_FB)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("SubBlk_SFB").ToLocalChecked()
    , Nan::New<v8::Integer>(SubBlk_SFB)
    , v8::ReadOnly);

  // Block languages
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("BlockLangAWL").ToLocalChecked()
    , Nan::New<v8::Integer>(BlockLangAWL)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("BlockLangKOP").ToLocalChecked()
    , Nan::New<v8::Integer>(BlockLangKOP)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("BlockLangFUP").ToLocalChecked()
    , Nan::New<v8::Integer>(BlockLangFUP)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("BlockLangSCL").ToLocalChecked()
    , Nan::New<v8::Integer>(BlockLangSCL)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("BlockLangDB").ToLocalChecked()
    , Nan::New<v8::Integer>(BlockLangDB)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("BlockLangGRAPH").ToLocalChecked()
    , Nan::New<v8::Integer>(BlockLangGRAPH)
    , v8::ReadOnly);

  // Parameter
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("LocalPort").ToLocalChecked()
    , Nan::New<v8::Integer>(p_u16_LocalPort)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("RemotePort").ToLocalChecked()
    , Nan::New<v8::Integer>(p_u16_RemotePort)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("PingTimeout").ToLocalChecked()
    , Nan::New<v8::Integer>(p_i32_PingTimeout)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("SendTimeout").ToLocalChecked()
    , Nan::New<v8::Integer>(p_i32_SendTimeout)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("RecvTimeout").ToLocalChecked()
    , Nan::New<v8::Integer>(p_i32_RecvTimeout)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("WorkInterval").ToLocalChecked()
    , Nan::New<v8::Integer>(p_i32_WorkInterval)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("SrcRef").ToLocalChecked()
    , Nan::New<v8::Integer>(p_u16_SrcRef)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("DstRef").ToLocalChecked()
    , Nan::New<v8::Integer>(p_u16_DstRef)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("SrcTSap").ToLocalChecked()
    , Nan::New<v8::Integer>(p_u16_SrcTSap)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("PDURequest").ToLocalChecked()
    , Nan::New<v8::Integer>(p_i32_PDURequest)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("MaxClients").ToLocalChecked()
    , Nan::New<v8::Integer>(p_i32_MaxClients)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("BSendTimeout").ToLocalChecked()
    , Nan::New<v8::Integer>(p_i32_BSendTimeout)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("BRecvTimeout").ToLocalChecked()
    , Nan::New<v8::Integer>(p_i32_BRecvTimeout)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("RecoveryTime").ToLocalChecked()
    , Nan::New<v8::Integer>(p_u32_RecoveryTime)
    , v8::ReadOnly);
  Nan::SetPrototypeTemplate(
      tpl
    , Nan::New<v8::String>("KeepAliveTime").ToLocalChecked()
    , Nan::New<v8::Integer>(p_u32_KeepAliveTime)
    , v8::ReadOnly);

  constructor.Reset(tpl);
  Nan::Set(target, name, tpl->GetFunction());
}

NAN_METHOD(S7Client::New) {
  if (info.IsConstructCall()) {
    S7Client *s7client = new S7Client();

    s7client->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
  } else {
    v8::Local<v8::FunctionTemplate> constructorHandle;
    constructorHandle = Nan::New<v8::FunctionTemplate>(constructor);
  info.GetReturnValue().Set(
    Nan::NewInstance(constructorHandle->GetFunction()).ToLocalChecked());
  }
}

S7Client::S7Client() {
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
NAN_METHOD(S7Client::Connect) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsFunction()) {
    int ret = s7client->snap7Client->Connect();
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(ret == 0));
  } else {
    Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, CONNECT));
    info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(S7Client::ConnectTo) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (info.Length() < 3) {
    return Nan::ThrowTypeError("Wrong number of arguments");
  }

  if (!info[0]->IsString() || !info[1]->IsInt32() || !info[2]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  Nan::Utf8String *remAddress = new Nan::Utf8String(info[0]);
  if (!info[3]->IsFunction()) {
    int ret = s7client->snap7Client->ConnectTo(
        **remAddress
      , info[1]->Int32Value()
      , info[2]->Int32Value());
    delete remAddress;
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(ret == 0));
  } else {
    Nan::Callback *callback = new Nan::Callback(info[3].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, CONNECTTO
      , remAddress, info[1]->Int32Value(), info[2]->Int32Value()));
    info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(S7Client::SetConnectionParams) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsString() || !info[1]->IsUint32() ||
      !info[2]->IsUint32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  Nan::Utf8String remAddress(info[0]);
  word LocalTSAP = info[1]->Uint32Value();
  word RemoteTSAP = info[2]->Uint32Value();

  int ret = s7client->snap7Client->SetConnectionParams(
      *remAddress
    , LocalTSAP
    , RemoteTSAP);
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(ret == 0));
}

NAN_METHOD(S7Client::SetConnectionType) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsUint32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  word type = info[0]->Uint32Value();

  int  ret = s7client->snap7Client->SetConnectionType(type);
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(ret == 0));
}

NAN_METHOD(S7Client::Disconnect) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  int ret = s7client->snap7Client->Disconnect();
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(ret == 0));
}

NAN_METHOD(S7Client::GetParam) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  int pData;
  int returnValue = s7client->snap7Client->GetParam(info[0]->Int32Value()
    , &pData);

  if (returnValue == 0) {
    info.GetReturnValue().Set(Nan::New<v8::Integer>(pData));
  } else {
    info.GetReturnValue().Set(Nan::New<v8::Integer>(returnValue));
  }
}

NAN_METHOD(S7Client::SetParam) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!(info[0]->IsInt32() || info[1]->IsInt32())) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  int pData = info[1]->Int32Value();
  int ret = s7client->snap7Client->SetParam(info[0]->Int32Value(), &pData);
  info.GetReturnValue().Set(Nan::New<v8::Boolean>(ret == 0));
}

// Data I/O Main functions
void IOWorker::Execute() {
  uv_mutex_lock(&s7client->mutex);

  switch (caller) {
  case CONNECTTO:
      returnValue = s7client->snap7Client->ConnectTo(
          **static_cast<Nan::Utf8String*>(pData), int1, int2);
      break;

  case CONNECT:
      returnValue = s7client->snap7Client->Connect();
      break;

  case READAREA:
      returnValue = s7client->snap7Client->ReadArea(int1, int2, int3, int4
        , int5, pData);
      break;

  case WRITEAREA:
      returnValue = s7client->snap7Client->WriteArea(int1, int2, int3, int4
        , int5, pData);
      break;

  case READMULTI:
      returnValue = s7client->snap7Client->ReadMultiVars(
          static_cast<PS7DataItem>(pData), int1);
      break;

  case WRITEMULTI:
      returnValue = s7client->snap7Client->WriteMultiVars(
          static_cast<PS7DataItem>(pData), int1);
      break;

  case PLCSTATUS:
      returnValue = s7client->snap7Client->PlcStatus();
      if ((returnValue == S7CpuStatusUnknown) ||
          (returnValue == S7CpuStatusStop) ||
          (returnValue == S7CpuStatusRun)) {
        int1 = returnValue;
        returnValue = 0;
      }
      break;

  case CLEARSESSIONPW:
      returnValue = s7client->snap7Client->ClearSessionPassword();
      break;

  case SETSESSIONPW:
      returnValue = s7client->snap7Client->SetSessionPassword(
          **static_cast<Nan::Utf8String*>(pData));
      break;

  case GETPROTECTION:
      returnValue = s7client->snap7Client->GetProtection(
          static_cast<PS7Protection>(pData));
      break;

  case PLCSTOP:
      returnValue = s7client->snap7Client->PlcStop();
      break;

  case PLCCOLDSTART:
      returnValue = s7client->snap7Client->PlcColdStart();
      break;

  case PLCHOTSTART:
      returnValue = s7client->snap7Client->PlcHotStart();
      break;

  case GETCPINFO:
      returnValue = s7client->snap7Client->GetCpInfo(
          static_cast<PS7CpInfo>(pData));
      break;

  case GETCPUINFO:
      returnValue = s7client->snap7Client->GetCpuInfo(
          static_cast<PS7CpuInfo>(pData));
      break;

  case GETORDERCODE:
      returnValue = s7client->snap7Client->GetOrderCode(
          static_cast<PS7OrderCode>(pData));
      break;

  case SETPLCSYSTEMDATETIME:
      returnValue = s7client->snap7Client->SetPlcSystemDateTime();
      break;

  case GETPLCDATETIME:
      returnValue = s7client->snap7Client->GetPlcDateTime(
          static_cast<tm*>(pData));
      break;

  case SETPLCDATETIME:
      returnValue = s7client->snap7Client->SetPlcDateTime(
          static_cast<tm*>(pData));
      break;

  case COMPRESS:
      returnValue = s7client->snap7Client->Compress(int1);
      break;

  case COPYRAMTOROM:
      returnValue = s7client->snap7Client->CopyRamToRom(int1);
      break;

  case DBFILL:
      returnValue = s7client->snap7Client->DBFill(int1, int2);
      break;

  case DBGET:
      returnValue = s7client->snap7Client->DBGet(int1, pData, &int2);
      break;

  case DELETEBLOCK:
      returnValue = s7client->snap7Client->Delete(int1, int2);
      break;

  case DOWNLOAD:
      returnValue = s7client->snap7Client->Download(int1, pData, int2);
      break;

  case FULLUPLOAD:
      returnValue = s7client->snap7Client->FullUpload(int1, int2, pData, &int3);
      break;

  case UPLOAD:
      returnValue = s7client->snap7Client->Upload(int1, int2, pData, &int3);
      break;

  case LISTBLOCKSOFTYPE:
      returnValue = s7client->snap7Client->ListBlocksOfType(int1
        , static_cast<PS7BlocksOfType>(pData), &int2);
      break;

  case GETAGBLOCKINFO:
      returnValue = s7client->snap7Client->GetAgBlockInfo(int1, int2
        , static_cast<PS7BlockInfo>(pData));
      break;

  case LISTBLOCKS:
      returnValue = s7client->snap7Client->ListBlocks(
          static_cast<PS7BlocksList>(pData));
      break;

  case READSZLLIST:
      returnValue = s7client->snap7Client->ReadSZLList(
          static_cast<PS7SZLList>(pData), &int1);
      break;

  case READSZL:
      returnValue = s7client->snap7Client->ReadSZL(int1, int2
        , static_cast<PS7SZL>(pData), &int3);
      break;
  }

  uv_mutex_unlock(&s7client->mutex);
}

void IOWorker::HandleOKCallback() {
  Nan::HandleScope scope;

  v8::Local<v8::Value> argv1[1];
  v8::Local<v8::Value> argv2[2];

  if (returnValue == 0) {
    argv2[0] = argv1[0] = Nan::Null();
  } else {
    argv2[0] = argv1[0] = Nan::New<v8::Integer>(returnValue);
  }

  switch (caller) {
  case CONNECTTO:
  case SETSESSIONPW:
      delete static_cast<Nan::Utf8String*>(pData);
      callback->Call(1, argv1);
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
      callback->Call(1, argv1);
      break;

  case READAREA:
    if (returnValue == 0) {
      argv2[1] = Nan::NewBuffer(
          static_cast<char*>(pData)
        , int4 * s7client->GetByteCountFromWordLen(int5)
        , S7Client::FreeCallback,
        NULL).ToLocalChecked();
    } else {
      argv2[1] = Nan::Null();
      delete[] static_cast<char*>(pData);
    }
    callback->Call(2, argv2);
    break;

  case READMULTI:
      if (returnValue == 0) {
        argv2[1] = s7client->S7DataItemToArray(static_cast<PS7DataItem>(pData)
          , int1, true);
      } else {
        for (int i = 0; i < int1; i++) {
          delete[] static_cast<char*>(static_cast<PS7DataItem>(pData)[i].pdata);
        }
        delete[] static_cast<PS7DataItem>(pData);
        argv2[1] = Nan::Null();
      }
      callback->Call(2, argv2);
      break;

  case WRITEMULTI:
      if (returnValue == 0) {
        argv2[1] = s7client->S7DataItemToArray(static_cast<PS7DataItem>(pData)
          , int1, false);
      } else {
        delete[] static_cast<PS7DataItem>(pData);
        argv2[1] = Nan::Null();
      }
      callback->Call(2, argv2);
      break;

  case GETPROTECTION:
      if (returnValue == 0) {
        argv2[1] = s7client->S7ProtectionToObject(
          static_cast<PS7Protection>(pData));
      } else {
        argv2[1] = Nan::Null();
      }
      delete static_cast<PS7Protection>(pData);
      callback->Call(2, argv2);
      break;

  case GETCPINFO:
      if (returnValue == 0) {
        argv2[1] = s7client->S7CpInfoToObject(
          static_cast<PS7CpInfo>(pData));
      } else {
        argv2[1] = Nan::Null();
      }
      delete static_cast<PS7CpInfo>(pData);
      callback->Call(2, argv2);
      break;

  case GETCPUINFO:
      if (returnValue == 0) {
        argv2[1] = s7client->S7CpuInfoToObject(
          static_cast<PS7CpuInfo>(pData));
      } else {
        argv2[1] = Nan::Null();
      }
      delete static_cast<PS7CpuInfo>(pData);
      callback->Call(2, argv2);
      break;

  case GETORDERCODE:
      if (returnValue == 0) {
        argv2[1] = s7client->S7OrderCodeToObject(
          static_cast<PS7OrderCode>(pData));
      } else {
        argv2[1] = Nan::Null();
      }
      delete static_cast<PS7OrderCode>(pData);
      callback->Call(2, argv2);
      break;

  case GETPLCDATETIME:
      if (returnValue == 0) {
        double timestamp = static_cast<double>(mktime(static_cast<tm*>(pData)));
        argv2[1] = Nan::New<v8::Date>(timestamp * 1000).ToLocalChecked();
      } else {
        argv2[1] = Nan::Null();
      }
      delete static_cast<tm*>(pData);
      callback->Call(2, argv2);
      break;

  case SETPLCDATETIME:
      delete static_cast<tm*>(pData);
      callback->Call(1, argv1);
      break;

  case PLCSTATUS:
      if (returnValue == 0) {
        argv2[1] = Nan::New<v8::Integer>(int1);
      } else {
        argv2[1] = Nan::Null();
      }
      callback->Call(2, argv2);
      break;

  case DBGET:
      if (returnValue == 0) {
        argv2[1] = Nan::NewBuffer(
            static_cast<char*>(pData)
          , int2
          , S7Client::FreeCallback
          , NULL).ToLocalChecked();
      } else {
        argv2[1] = Nan::Null();
        delete[] static_cast<char*>(pData);
      }
      callback->Call(2, argv2);
      break;

  case FULLUPLOAD:
  case UPLOAD:
      if (returnValue == 0) {
        argv2[1] = Nan::NewBuffer(
            static_cast<char*>(pData)
          , int3
          , S7Client::FreeCallback
          , NULL).ToLocalChecked();
      } else {
        argv2[1] = Nan::Null();
        delete[] static_cast<char*>(pData);
      }
      callback->Call(2, argv2);
      break;

  case LISTBLOCKSOFTYPE:
      if (returnValue == 0) {
        argv2[1] = s7client->S7BlocksOfTypeToArray(
            static_cast<PS7BlocksOfType>(pData), int2);
      } else {
        argv2[1] = Nan::Null();
      }
      delete[] static_cast<PS7BlocksOfType>(pData);
      callback->Call(2, argv2);
      break;

  case GETAGBLOCKINFO:
      if (returnValue == 0) {
        v8::Local<v8::Object> block_info = s7client->S7BlockInfoToObject(
            static_cast<PS7BlockInfo>(pData));
        argv2[1] = block_info;
      } else {
        argv2[1] = Nan::Null();
      }
      delete static_cast<PS7BlockInfo>(pData);
      callback->Call(2, argv2);
      break;

  case LISTBLOCKS:
      if (returnValue == 0) {
        v8::Local<v8::Object> blocks_list = s7client->S7BlocksListToObject(
            static_cast<PS7BlocksList>(pData));
        argv2[1] = blocks_list;
      } else {
        argv2[1] = Nan::Null();
      }
      delete static_cast<PS7BlocksList>(pData);
      callback->Call(2, argv2);
      break;

  case READSZLLIST:
      if (returnValue == 0) {
        v8::Local<v8::Array> szl_list = s7client->S7SZLListToArray(
            static_cast<PS7SZLList>(pData), int1);
        argv2[1] = szl_list;
      } else {
        argv2[1] = Nan::Null();
      }
      delete static_cast<PS7SZLList>(pData);
      callback->Call(2, argv2);
      break;

  case READSZL:
      if (returnValue == 0) {
        argv2[1] = Nan::NewBuffer(
            reinterpret_cast<char*>(static_cast<PS7SZL>(pData))
          , int3
          , S7Client::FreeCallbackSZL
          , NULL).ToLocalChecked();
      } else {
        argv2[1] = Nan::Null();
        delete static_cast<PS7SZL>(pData);
      }
      callback->Call(2, argv2);
      break;
  }
}

NAN_METHOD(S7Client::ReadArea) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (info.Length() < 5)
    return Nan::ThrowTypeError("Wrong number of Arguments");

  if (!info[0]->IsInt32() || !info[1]->IsInt32() ||
      !info[2]->IsInt32() || !info[3]->IsInt32() ||
      !info[4]->IsInt32())
    return Nan::ThrowTypeError("Wrong arguments");

  int amount = info[3]->Int32Value();
  int byteCount = s7client->GetByteCountFromWordLen(info[4]->Int32Value());
  int size = amount * byteCount;
  char *bufferData = new char[size];

  if (!info[5]->IsFunction()) {
    int returnValue = s7client->snap7Client->ReadArea(
        info[0]->Int32Value(), info[1]->Int32Value()
      , info[2]->Int32Value(), info[3]->Int32Value()
      , info[4]->Int32Value(), bufferData);

    if (returnValue == 0) {
      v8::Local<v8::Object> ret = Nan::NewBuffer(
          bufferData
        , size
        , S7Client::FreeCallback
        , NULL).ToLocalChecked();
      info.GetReturnValue().Set(ret);
    } else {
      delete[] bufferData;
      info.GetReturnValue().Set(Nan::False());
    }

  } else {
    Nan::Callback *callback = new Nan::Callback(info[5].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, READAREA
      , bufferData, info[0]->Int32Value(), info[1]->Int32Value()
      , info[2]->Int32Value(), info[3]->Int32Value(), info[4]->Int32Value()));
    info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(S7Client::WriteArea) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (info.Length() < 6)
    return Nan::ThrowTypeError("Wrong number of Arguments");

  if (!info[0]->IsInt32() || !info[1]->IsInt32() ||
      !info[2]->IsInt32() || !info[3]->IsInt32() ||
      !info[4]->IsInt32() || !node::Buffer::HasInstance(info[5]))
    return Nan::ThrowTypeError("Wrong arguments");

  if (!info[6]->IsFunction()) {
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(
      s7client->snap7Client->WriteArea(info[0]->Int32Value()
        , info[1]->Int32Value(), info[2]->Int32Value()
        , info[3]->Int32Value(), info[4]->Int32Value()
        , node::Buffer::Data(info[5].As<v8::Object>())) == 0));
  } else {
    Nan::Callback *callback = new Nan::Callback(info[6].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, WRITEAREA
      , node::Buffer::Data(info[5].As<v8::Object>()), info[0]->Int32Value()
      , info[1]->Int32Value(), info[2]->Int32Value(), info[3]->Int32Value()
      , info[4]->Int32Value()));
    info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(S7Client::ReadMultiVars) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (info.Length() < 1) {
    return Nan::ThrowTypeError("Wrong number of arguments");
  }

  if (!info[0]->IsArray()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  v8::Local<v8::Array> data_arr = v8::Local<v8::Array>::Cast(info[0]);
  int len = data_arr->Length();
  if (len == 0) {
    return Nan::ThrowTypeError("Array needs at least 1 item");
  } else if (len > MaxVars) {
    std::stringstream err;
    err << "Array exceeds max variables (" << MaxVars
        << ") that can be transferred with ReadMultiVars()";
    return Nan::ThrowTypeError(err.str().c_str());
  }

  for (int i = 0; i < len; i++) {
    if (!Nan::Get(data_arr, i).ToLocalChecked()->IsObject()) {
      return Nan::ThrowTypeError("Wrong argument structure");
    } else {
      v8::Local<v8::Object> data_obj = Nan::Get(data_arr, i).ToLocalChecked()->ToObject();
      if (!Nan::Has(data_obj, Nan::New<v8::String>("Area").ToLocalChecked()).FromJust() ||
          !Nan::Has(data_obj, Nan::New<v8::String>("WordLen").ToLocalChecked()).FromJust() ||
          !Nan::Has(data_obj, Nan::New<v8::String>("Start").ToLocalChecked()).FromJust() ||
          !Nan::Has(data_obj, Nan::New<v8::String>("Amount").ToLocalChecked()).FromJust()) {
        return Nan::ThrowTypeError("Wrong argument structure");
      } else if (!Nan::Get(data_obj, Nan::New<v8::String>("Area").ToLocalChecked()).ToLocalChecked()->IsInt32() ||
                 !Nan::Get(data_obj, Nan::New<v8::String>("WordLen").ToLocalChecked()).ToLocalChecked()->IsInt32() ||
                 !Nan::Get(data_obj, Nan::New<v8::String>("Start").ToLocalChecked()).ToLocalChecked()->IsInt32() ||
                 !Nan::Get(data_obj, Nan::New<v8::String>("Amount").ToLocalChecked()).ToLocalChecked()->IsInt32()) {
        return Nan::ThrowTypeError("Wrong argument structure");
      } else if (Nan::Get(data_obj, Nan::New<v8::String>("Area").ToLocalChecked()).ToLocalChecked()->Int32Value() == S7AreaDB) {
        if (!Nan::Has(data_obj, Nan::New<v8::String>("DBNumber").ToLocalChecked()).FromJust()) {
          return Nan::ThrowTypeError("Wrong argument structure");
        }
      } else {
        Nan::Set(data_obj, Nan::New<v8::String>("DBNumber").ToLocalChecked(), Nan::New<v8::Integer>(0));
      }
    }
  }

  PS7DataItem Items = new TS7DataItem[len];
  v8::Local<v8::Object> data_obj;
  int byteCount, size;

  for (int i = 0; i < len; i++) {
    data_obj = Nan::Get(data_arr, i).ToLocalChecked()->ToObject();

    Items[i].Area = Nan::Get(data_obj,
      Nan::New<v8::String>("Area").ToLocalChecked()).ToLocalChecked()->Int32Value();
    Items[i].WordLen = Nan::Get(data_obj,
      Nan::New<v8::String>("WordLen").ToLocalChecked()).ToLocalChecked()->Int32Value();
    Items[i].DBNumber = Nan::Get(data_obj,
      Nan::New<v8::String>("DBNumber").ToLocalChecked()).ToLocalChecked()->Int32Value();
    Items[i].Start = Nan::Get(data_obj,
      Nan::New<v8::String>("Start").ToLocalChecked()).ToLocalChecked()->Int32Value();
    Items[i].Amount = Nan::Get(data_obj,
      Nan::New<v8::String>("Amount").ToLocalChecked()).ToLocalChecked()->Int32Value();

    byteCount = s7client->GetByteCountFromWordLen(Items[i].WordLen);
    size = Items[i].Amount * byteCount;
    Items[i].pdata = new char[size];
  }

  if (!info[1]->IsFunction()) {
    int returnValue = s7client->snap7Client->ReadMultiVars(Items, len);

    if (returnValue == 0) {
      info.GetReturnValue().Set(s7client->S7DataItemToArray(Items, len, true));
    } else {
      for (int i = 0; i < len; i++) {
        delete[] static_cast<char*>(Items[i].pdata);
      }
      delete[] Items;
      info.GetReturnValue().Set(Nan::False());
    }
  } else {
    Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, READMULTI
      , Items, len));
    info.GetReturnValue().SetUndefined();
  }
}

v8::Local<v8::Array> S7Client::S7DataItemToArray(
    PS7DataItem Items
  , int len
  , bool readMulti
) {
  Nan::EscapableHandleScope scope;

  v8::Local<v8::Array> res_arr = Nan::New<v8::Array>(len);
  v8::Local<v8::Object> res_obj;
  int byteCount, size;

  for (int i = 0; i < len; i++) {
    res_obj = Nan::New<v8::Object>();
    Nan::Set(res_obj, Nan::New<v8::String>("Result").ToLocalChecked()
      , Nan::New<v8::Integer>(Items[i].Result));

    if (readMulti == true) {
      if (Items[i].Result == 0) {
        byteCount = S7Client::GetByteCountFromWordLen(Items[i].WordLen);
        size = byteCount * Items[i].Amount;
        Nan::Set(
            res_obj
          , Nan::New<v8::String>("Data").ToLocalChecked()
          , Nan::NewBuffer(
              static_cast<char*>(Items[i].pdata)
            , size
            , S7Client::FreeCallback
            , NULL).ToLocalChecked());
      } else {
        delete[] static_cast<char*>(Items[i].pdata);
        Nan::Set(res_obj, Nan::New<v8::String>("Data").ToLocalChecked(), Nan::Null());
      }
    }
    Nan::Set(res_arr, i, res_obj);
  }
  delete[] Items;

  return scope.Escape(res_arr);
}

NAN_METHOD(S7Client::WriteMultiVars) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (info.Length() < 1) {
    return Nan::ThrowTypeError("Wrong number of arguments");
  }

  if (!info[0]->IsArray()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  v8::Local<v8::Array> data_arr = v8::Local<v8::Array>::Cast(info[0]);
  int len = data_arr->Length();
  if (len == 0) {
    return Nan::ThrowTypeError("Array needs at least 1 item");
  } else if (len > MaxVars) {
    std::stringstream err;
    err << "Array exceeds max variables (" << MaxVars
        << ") that can be transferred with WriteMultiVars()";
    return Nan::ThrowTypeError(err.str().c_str());
  }

  for (int i = 0; i < len; i++) {
    if (!Nan::Get(data_arr, i).ToLocalChecked()->IsObject()) {
      return Nan::ThrowTypeError("Wrong argument structure");
    } else {
      v8::Local<v8::Object> data_obj = Nan::Get(data_arr, i).ToLocalChecked()->ToObject();
      if (!Nan::Has(data_obj, Nan::New<v8::String>("Area").ToLocalChecked()).FromJust() ||
          !Nan::Has(data_obj, Nan::New<v8::String>("WordLen").ToLocalChecked()).FromJust() ||
          !Nan::Has(data_obj, Nan::New<v8::String>("Start").ToLocalChecked()).FromJust() ||
          !Nan::Has(data_obj, Nan::New<v8::String>("Amount").ToLocalChecked()).FromJust() ||
          !Nan::Has(data_obj, Nan::New<v8::String>("Data").ToLocalChecked()).FromJust()) {
        return Nan::ThrowTypeError("Wrong argument structure");
      } else if (!Nan::Get(data_obj, Nan::New<v8::String>("Area").ToLocalChecked()).ToLocalChecked()->IsInt32() ||
                 !Nan::Get(data_obj, Nan::New<v8::String>("WordLen").ToLocalChecked()).ToLocalChecked()->IsInt32() ||
                 !Nan::Get(data_obj, Nan::New<v8::String>("Start").ToLocalChecked()).ToLocalChecked()->IsInt32() ||
                 !Nan::Get(data_obj, Nan::New<v8::String>("Amount").ToLocalChecked()).ToLocalChecked()->IsInt32() ||
                 !node::Buffer::HasInstance(Nan::Get(data_obj, Nan::New<v8::String>("Data").ToLocalChecked()).ToLocalChecked())) {
        return Nan::ThrowTypeError("Wrong argument structure");
      } else if (Nan::Get(data_obj, Nan::New<v8::String>("Area").ToLocalChecked()).ToLocalChecked()->Int32Value() == S7AreaDB) {
        if (!Nan::Has(data_obj, Nan::New<v8::String>("DBNumber").ToLocalChecked()).FromJust()) {
          return Nan::ThrowTypeError("Wrong argument structure");
        }
      } else {
        Nan::Set(data_obj, Nan::New<v8::String>("DBNumber").ToLocalChecked(), Nan::New<v8::Integer>(0));
      }
    }
  }

  PS7DataItem Items = new TS7DataItem[len];
  v8::Local<v8::Object> data_obj;
  for (int i = 0; i < len; i++) {
    data_obj = Nan::Get(data_arr, i).ToLocalChecked()->ToObject();

    Items[i].Area = Nan::Get(data_obj,
      Nan::New<v8::String>("Area").ToLocalChecked()).ToLocalChecked()->Int32Value();
    Items[i].WordLen = Nan::Get(data_obj,
      Nan::New<v8::String>("WordLen").ToLocalChecked()).ToLocalChecked()->Int32Value();
    Items[i].DBNumber = Nan::Get(data_obj,
      Nan::New<v8::String>("DBNumber").ToLocalChecked()).ToLocalChecked()->Int32Value();
    Items[i].Start = Nan::Get(data_obj,
      Nan::New<v8::String>("Start").ToLocalChecked()).ToLocalChecked()->Int32Value();
    Items[i].Amount = Nan::Get(data_obj,
      Nan::New<v8::String>("Amount").ToLocalChecked()).ToLocalChecked()->Int32Value();
    Items[i].pdata = node::Buffer::Data(Nan::Get(data_obj,
      Nan::New<v8::String>("Data").ToLocalChecked()).ToLocalChecked().As<v8::Object>());
  }

  if (!info[1]->IsFunction()) {
    int returnValue = s7client->snap7Client->WriteMultiVars(Items, len);

    if (returnValue == 0) {
      info.GetReturnValue().Set(s7client->S7DataItemToArray(Items, len, false));
    } else {
      delete[] Items;
      info.GetReturnValue().Set(Nan::False());
    }
  } else {
    Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, WRITEMULTI
      , Items, len));
    info.GetReturnValue().SetUndefined();
  }
}

// Directory functions
NAN_METHOD(S7Client::ListBlocks) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  PS7BlocksList BlocksList = new TS7BlocksList;
  if (!info[0]->IsFunction()) {
    int returnValue = s7client->snap7Client->ListBlocks(BlocksList);

    v8::Local<v8::Object> blocks_list = s7client->S7BlocksListToObject(
        BlocksList);

    if (returnValue == 0) {
      delete BlocksList;
      info.GetReturnValue().Set(blocks_list);
    } else {
      delete BlocksList;
      info.GetReturnValue().Set(Nan::False());
    }
  } else {
    Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, LISTBLOCKS
      , BlocksList));
    info.GetReturnValue().SetUndefined();
  }
}

v8::Local<v8::Object> S7Client::S7BlocksListToObject(
    PS7BlocksList BlocksList
) {
  Nan::EscapableHandleScope scope;

  v8::Local<v8::Object> blocks_list = Nan::New<v8::Object>();
  Nan::Set(blocks_list, Nan::New<v8::String>("OBCount").ToLocalChecked()
    , Nan::New<v8::Number>(BlocksList->OBCount));
  Nan::Set(blocks_list, Nan::New<v8::String>("FBCount").ToLocalChecked()
    , Nan::New<v8::Number>(BlocksList->FBCount));
  Nan::Set(blocks_list, Nan::New<v8::String>("FCCount").ToLocalChecked()
    , Nan::New<v8::Number>(BlocksList->FCCount));
  Nan::Set(blocks_list, Nan::New<v8::String>("SFBCount").ToLocalChecked()
    , Nan::New<v8::Number>(BlocksList->SFBCount));
  Nan::Set(blocks_list, Nan::New<v8::String>("SFCCount").ToLocalChecked()
    , Nan::New<v8::Number>(BlocksList->SFCCount));
  Nan::Set(blocks_list, Nan::New<v8::String>("DBCount").ToLocalChecked()
    , Nan::New<v8::Number>(BlocksList->DBCount));
  Nan::Set(blocks_list, Nan::New<v8::String>("SDBCount").ToLocalChecked()
    , Nan::New<v8::Number>(BlocksList->SDBCount));

  return scope.Escape(blocks_list);
}


NAN_METHOD(S7Client::GetAgBlockInfo) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsInt32() || !info[1]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  PS7BlockInfo BlockInfo = new TS7BlockInfo;
  if (!info[2]->IsFunction()) {
    int returnValue = s7client->snap7Client->GetAgBlockInfo(
      info[0]->Int32Value(), info[1]->Int32Value(), BlockInfo);

    if (returnValue == 0) {
      v8::Local<v8::Object> block_info = s7client->S7BlockInfoToObject(
          BlockInfo);

      delete BlockInfo;
      info.GetReturnValue().Set(block_info);
    } else {
      delete BlockInfo;
      info.GetReturnValue().Set(Nan::False());
    }
  } else {
    Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, GETAGBLOCKINFO
      , BlockInfo, info[0]->Int32Value(), info[1]->Int32Value()));
    info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(S7Client::GetPgBlockInfo) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!node::Buffer::HasInstance(info[0])) {
    return Nan::ThrowTypeError("Argument should be a Buffer");
  }

  PS7BlockInfo BlockInfo = new TS7BlockInfo;

  int returnValue = s7client->snap7Client->GetPgBlockInfo(
    node::Buffer::Data(info[0].As<v8::Object>()), BlockInfo
    , static_cast<int>(node::Buffer::Length(info[0].As<v8::Object>())));

  if (returnValue == 0) {
    v8::Local<v8::Object> block_info = s7client->S7BlockInfoToObject(BlockInfo);
    delete BlockInfo;
    info.GetReturnValue().Set(block_info);
  } else {
    delete BlockInfo;
    info.GetReturnValue().Set(Nan::False());
  }
}

v8::Local<v8::Object> S7Client::S7BlockInfoToObject(PS7BlockInfo BlockInfo) {
  Nan::EscapableHandleScope scope;

  v8::Local<v8::Object> block_info = Nan::New<v8::Object>();
  Nan::Set(block_info, Nan::New<v8::String>("BlkType").ToLocalChecked()
    , Nan::New<v8::Number>(BlockInfo->BlkType));
  Nan::Set(block_info, Nan::New<v8::String>("BlkNumber").ToLocalChecked()
    , Nan::New<v8::Number>(BlockInfo->BlkNumber));
  Nan::Set(block_info, Nan::New<v8::String>("BlkLang").ToLocalChecked()
    , Nan::New<v8::Number>(BlockInfo->BlkLang));
  Nan::Set(block_info, Nan::New<v8::String>("BlkFlags").ToLocalChecked()
    , Nan::New<v8::Number>(BlockInfo->BlkFlags));
  Nan::Set(block_info, Nan::New<v8::String>("MC7Size").ToLocalChecked()
    , Nan::New<v8::Number>(BlockInfo->MC7Size));
  Nan::Set(block_info, Nan::New<v8::String>("LoadSize").ToLocalChecked()
    , Nan::New<v8::Number>(BlockInfo->LoadSize));
  Nan::Set(block_info, Nan::New<v8::String>("LocalData").ToLocalChecked()
    , Nan::New<v8::Number>(BlockInfo->LocalData));
  Nan::Set(block_info, Nan::New<v8::String>("SBBLength").ToLocalChecked()
    , Nan::New<v8::Number>(BlockInfo->SBBLength));
  Nan::Set(block_info, Nan::New<v8::String>("CheckSum").ToLocalChecked()
    , Nan::New<v8::Number>(BlockInfo->CheckSum));
  Nan::Set(block_info, Nan::New<v8::String>("Version").ToLocalChecked()
    , Nan::New<v8::Number>(BlockInfo->Version));
  Nan::Set(block_info, Nan::New<v8::String>("CodeDate").ToLocalChecked()
    , Nan::New<v8::String>(BlockInfo->CodeDate).ToLocalChecked());
  Nan::Set(block_info, Nan::New<v8::String>("IntfDate").ToLocalChecked()
    , Nan::New<v8::String>(BlockInfo->IntfDate).ToLocalChecked());
  Nan::Set(block_info, Nan::New<v8::String>("Author").ToLocalChecked()
    , Nan::New<v8::String>(BlockInfo->Author).ToLocalChecked());
  Nan::Set(block_info, Nan::New<v8::String>("Family").ToLocalChecked()
    , Nan::New<v8::String>(BlockInfo->Family).ToLocalChecked());
  Nan::Set(block_info, Nan::New<v8::String>("Header").ToLocalChecked()
    , Nan::New<v8::String>(BlockInfo->Header).ToLocalChecked());

  return scope.Escape(block_info);
}

NAN_METHOD(S7Client::ListBlocksOfType) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  int BlockNum = sizeof(TS7BlocksOfType) / sizeof(PS7BlocksOfType);
  PS7BlocksOfType BlockList = new TS7BlocksOfType[BlockNum];
  if (!info[1]->IsFunction()) {
    int returnValue = s7client->snap7Client->ListBlocksOfType(
        info[0]->Int32Value(), BlockList, &BlockNum);

    if (returnValue == 0) {
      v8::Local<v8::Array> block_list = s7client->S7BlocksOfTypeToArray(
          BlockList, BlockNum);
      delete[] BlockList;
      info.GetReturnValue().Set(block_list);
    } else {
      delete[] BlockList;
      info.GetReturnValue().Set(Nan::False());
    }
  } else {
    Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, LISTBLOCKSOFTYPE
      , BlockList, info[0]->Int32Value(), BlockNum));
    info.GetReturnValue().SetUndefined();
  }
}

v8::Local<v8::Array> S7Client::S7BlocksOfTypeToArray(
    PS7BlocksOfType BlocksList
  , int count
) {
  Nan::EscapableHandleScope scope;

  v8::Local<v8::Array> block_list = Nan::New<v8::Array>(count);
  for (int i = 0; i < count; i++) {
    Nan::Set(block_list, i, Nan::New<v8::Integer>(*BlocksList[i]));
  }

  return scope.Escape(block_list);
}

// Blocks functions
NAN_METHOD(S7Client::Upload) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsInt32() || !info[1]->IsInt32() || !info[2]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  char *bufferData = new char[info[2]->Int32Value()];
  int size = info[2]->Int32Value();
  if (!info[3]->IsFunction()) {
    int returnValue = s7client->snap7Client->Upload(
      info[0]->Int32Value(), info[1]->Int32Value(), bufferData, &size);

    if (returnValue == 0) {
      v8::Local<v8::Object> ret_buf;
      ret_buf = Nan::NewBuffer(
          bufferData
        , size
        , S7Client::FreeCallback
        , NULL).ToLocalChecked();
      info.GetReturnValue().Set(ret_buf);
    } else {
      delete[] bufferData;
      info.GetReturnValue().Set(Nan::False());
    }
  } else {
    Nan::Callback *callback = new Nan::Callback(info[3].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, UPLOAD
      , bufferData, info[0]->Int32Value(), info[1]->Int32Value(), size));
    info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(S7Client::FullUpload) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsInt32() || !info[1]->IsInt32() || !info[2]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  char *bufferData = new char[info[2]->Int32Value()];
  int size = info[2]->Int32Value();
  if (!info[3]->IsFunction()) {
    int returnValue = s7client->snap7Client->FullUpload(
      info[0]->Int32Value(), info[1]->Int32Value(), bufferData, &size);

    if (returnValue == 0) {
      v8::Local<v8::Object> ret_buf;
      ret_buf = Nan::NewBuffer(
          bufferData
        , size
        , S7Client::FreeCallback
        , NULL).ToLocalChecked();
      info.GetReturnValue().Set(ret_buf);
    } else {
      delete[] bufferData;
      info.GetReturnValue().Set(Nan::False());
    }
  } else {
    Nan::Callback *callback = new Nan::Callback(info[3].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, FULLUPLOAD
      , bufferData, info[0]->Int32Value(), info[1]->Int32Value(), size));
    info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(S7Client::Download) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsInt32() || !node::Buffer::HasInstance(info[1])) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  if (!info[2]->IsFunction()) {
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(s7client->snap7Client->Download(
        info[0]->Int32Value(), node::Buffer::Data(info[1].As<v8::Object>())
      , static_cast<int>(node::Buffer::Length(info[1].As<v8::Object>()))) == 0));
  } else {
    Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, DOWNLOAD
      , node::Buffer::Data(info[1].As<v8::Object>()), info[0]->Int32Value()
      , static_cast<int>(node::Buffer::Length(info[1].As<v8::Object>()))));
    info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(S7Client::Delete) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsInt32() || !info[1]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  if (!info[2]->IsFunction()) {
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(s7client->snap7Client->Delete(
      info[0]->Int32Value(), info[1]->Int32Value()) == 0));
  } else {
    Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, DELETEBLOCK
      , info[0]->Int32Value(), info[1]->Int32Value()));
    info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(S7Client::DBGet) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  int size = 65536;
  char *bufferData = new char[size];
  if (!info[2]->IsFunction()) {
    int returnValue = s7client->snap7Client->DBGet(
      info[0]->Int32Value(), bufferData, &size);

    if (returnValue == 0) {
      v8::Local<v8::Object> ret_buf;
      ret_buf = Nan::NewBuffer(
          bufferData
        , size
        , S7Client::FreeCallback
        , NULL).ToLocalChecked();
      info.GetReturnValue().Set(ret_buf);
    } else {
      delete[] bufferData;
      info.GetReturnValue().Set(Nan::False());
    }
  } else {
    Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, DBGET
      , bufferData, info[0]->Int32Value(), size));
    info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(S7Client::DBFill) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsInt32() || !(info[1]->IsInt32() || info[1]->IsString())) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  int fill;
  if (info[1]->IsInt32()) {
    fill = info[1]->Int32Value();
  } else {
    Nan::Utf8String fillstr(info[1]);
    fill = static_cast<int>(**fillstr);
  }

  if (!info[2]->IsFunction()) {
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(s7client->snap7Client->DBFill(
      info[0]->Int32Value(), fill) == 0));
  } else {
    Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, DBFILL
      , info[0]->Int32Value(), fill));
    info.GetReturnValue().SetUndefined();
  }
}

// Date/Time functions
NAN_METHOD(S7Client::GetPlcDateTime) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  tm *DateTime = new tm;
  if (!info[0]->IsFunction()) {
    int returnValue = s7client->snap7Client->GetPlcDateTime(DateTime);
    double timestamp = static_cast<double>(mktime(DateTime));
    delete DateTime;

    if (returnValue == 0)
      info.GetReturnValue().Set(Nan::New<v8::Date>(timestamp * 1000).ToLocalChecked());
    else
      info.GetReturnValue().Set(Nan::False());
  } else {
    Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, GETPLCDATETIME
      , DateTime));
    info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(S7Client::SetPlcDateTime) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!(info[0]->IsObject() || info[0]->IsDate())) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  tm *DateTime = new tm;
  if (info[0]->IsDate()) {
    v8::Local<v8::Date> date = v8::Local<v8::Date>::Cast(info[0]->ToObject());
    time_t timestamp = static_cast<time_t>(date->NumberValue() / 1000);
    *DateTime = *localtime(&timestamp);
  } else {
    v8::Local<v8::Object> date_time = info[0]->ToObject();
    DateTime->tm_year = Nan::Get(date_time,
      Nan::New<v8::String>("year").ToLocalChecked()).ToLocalChecked()->Int32Value() - 1900;
    DateTime->tm_mon = Nan::Get(date_time,
      Nan::New<v8::String>("month").ToLocalChecked()).ToLocalChecked()->Int32Value();
    DateTime->tm_mday = Nan::Get(date_time,
      Nan::New<v8::String>("day").ToLocalChecked()).ToLocalChecked()->Int32Value();
    DateTime->tm_hour = Nan::Get(date_time,
      Nan::New<v8::String>("hours").ToLocalChecked()).ToLocalChecked()->Int32Value();
    DateTime->tm_min = Nan::Get(date_time,
      Nan::New<v8::String>("minutes").ToLocalChecked()).ToLocalChecked()->Int32Value();
    DateTime->tm_sec = Nan::Get(date_time,
      Nan::New<v8::String>("seconds").ToLocalChecked()).ToLocalChecked()->Int32Value();
  }

  if (!info[1]->IsFunction()) {
    v8::Local<v8::Boolean> ret = Nan::New<v8::Boolean>(
      s7client->snap7Client->SetPlcDateTime(DateTime) == 0);
    delete DateTime;
    info.GetReturnValue().Set(ret);
  } else {
    Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, SETPLCDATETIME
      , DateTime));
    info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(S7Client::SetPlcSystemDateTime) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsFunction()) {
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(
      s7client->snap7Client->SetPlcSystemDateTime() == 0));
  } else {
    Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, SETPLCSYSTEMDATETIME));
    info.GetReturnValue().SetUndefined();
  }
}

// System Info functions
NAN_METHOD(S7Client::GetOrderCode) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  PS7OrderCode OrderCode = new TS7OrderCode;
  if (!info[0]->IsFunction()) {
    int returnValue = s7client->snap7Client->GetOrderCode(OrderCode);

    if (returnValue == 0) {
      v8::Local<v8::Object> order_code = s7client->S7OrderCodeToObject(OrderCode);
      delete OrderCode;
      info.GetReturnValue().Set(order_code);
    } else {
      delete OrderCode;
      info.GetReturnValue().Set(Nan::False());
    }
  } else {
    Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, GETORDERCODE
      , OrderCode));
    info.GetReturnValue().SetUndefined();
  }
}

v8::Local<v8::Object> S7Client::S7OrderCodeToObject(PS7OrderCode OrderCode) {
  Nan::EscapableHandleScope scope;

  v8::Local<v8::Object> order_code = Nan::New<v8::Object>();
  Nan::Set(order_code, Nan::New<v8::String>("Code").ToLocalChecked()
    , Nan::New<v8::String>(OrderCode->Code).ToLocalChecked());
  Nan::Set(order_code, Nan::New<v8::String>("V1").ToLocalChecked()
    , Nan::New<v8::Number>(OrderCode->V1));
  Nan::Set(order_code, Nan::New<v8::String>("V2").ToLocalChecked()
    , Nan::New<v8::Number>(OrderCode->V2));
  Nan::Set(order_code, Nan::New<v8::String>("V3").ToLocalChecked()
    , Nan::New<v8::Number>(OrderCode->V3));

  return scope.Escape(order_code);
}


NAN_METHOD(S7Client::GetCpuInfo) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  PS7CpuInfo CpuInfo = new TS7CpuInfo;
  if (!info[0]->IsFunction()) {
    int returnValue = s7client->snap7Client->GetCpuInfo(CpuInfo);

    if (returnValue == 0) {
      v8::Local<v8::Object> cpu_info = s7client->S7CpuInfoToObject(CpuInfo);
      delete CpuInfo;
      info.GetReturnValue().Set(cpu_info);
    } else {
      delete CpuInfo;
      info.GetReturnValue().Set(Nan::False());
    }
  } else {
    Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, GETCPUINFO, CpuInfo));
    info.GetReturnValue().SetUndefined();
  }
}

v8::Local<v8::Object> S7Client::S7CpuInfoToObject(PS7CpuInfo CpuInfo) {
  Nan::EscapableHandleScope scope;

  v8::Local<v8::Object> cpu_info = Nan::New<v8::Object>();
  Nan::Set(cpu_info, Nan::New<v8::String>("ModuleTypeName").ToLocalChecked()
    , Nan::New<v8::String>(CpuInfo->ModuleTypeName).ToLocalChecked());
  Nan::Set(cpu_info, Nan::New<v8::String>("SerialNumber").ToLocalChecked()
    , Nan::New<v8::String>(CpuInfo->SerialNumber).ToLocalChecked());
  Nan::Set(cpu_info, Nan::New<v8::String>("ASName").ToLocalChecked()
    , Nan::New<v8::String>(CpuInfo->ASName).ToLocalChecked());
  Nan::Set(cpu_info, Nan::New<v8::String>("Copyright").ToLocalChecked()
    , Nan::New<v8::String>(CpuInfo->Copyright).ToLocalChecked());
  Nan::Set(cpu_info, Nan::New<v8::String>("ModuleName").ToLocalChecked()
    , Nan::New<v8::String>(CpuInfo->ModuleName).ToLocalChecked());

  return scope.Escape(cpu_info);
}


NAN_METHOD(S7Client::GetCpInfo) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  PS7CpInfo CpInfo = new TS7CpInfo;
  if (!info[0]->IsFunction()) {
    int returnValue = s7client->snap7Client->GetCpInfo(CpInfo);

    if (returnValue == 0) {
      v8::Local<v8::Object> cp_info = s7client->S7CpInfoToObject(CpInfo);
      delete CpInfo;
      info.GetReturnValue().Set(cp_info);
    } else {
      delete CpInfo;
      info.GetReturnValue().Set(Nan::False());
    }
  } else {
    Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, GETCPINFO, CpInfo));
    info.GetReturnValue().SetUndefined();
  }
}

v8::Local<v8::Object> S7Client::S7CpInfoToObject(PS7CpInfo CpInfo) {
  Nan::EscapableHandleScope scope;

  v8::Local<v8::Object> cp_info = Nan::New<v8::Object>();
  Nan::Set(cp_info, Nan::New<v8::String>("MaxPduLength").ToLocalChecked()
    , Nan::New<v8::Number>(CpInfo->MaxPduLengt));
  Nan::Set(cp_info, Nan::New<v8::String>("MaxConnections").ToLocalChecked()
    , Nan::New<v8::Number>(CpInfo->MaxConnections));
  Nan::Set(cp_info, Nan::New<v8::String>("MaxMpiRate").ToLocalChecked()
    , Nan::New<v8::Number>(CpInfo->MaxMpiRate));
  Nan::Set(cp_info, Nan::New<v8::String>("MaxBusRate").ToLocalChecked()
    , Nan::New<v8::Number>(CpInfo->MaxBusRate));

  return scope.Escape(cp_info);
}

NAN_METHOD(S7Client::ReadSZL) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!(info[0]->IsInt32() || info[1]->IsInt32())) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  PS7SZL SZL = new TS7SZL;
  int size = sizeof(TS7SZL);
  if (!info[2]->IsFunction()) {
    int returnValue = s7client->snap7Client->ReadSZL(info[0]->Int32Value()
      , info[1]->Int32Value(), SZL, &size);

    if (returnValue == 0) {
      v8::Local<v8::Object> ret_buf;
      ret_buf = Nan::NewBuffer(
          reinterpret_cast<char*>(SZL)
        , size
        , S7Client::FreeCallbackSZL
        , NULL).ToLocalChecked();

      info.GetReturnValue().Set(ret_buf);
    } else {
      delete SZL;
      info.GetReturnValue().Set(Nan::False());
    }
  } else {
    Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, READSZL, SZL
      , info[0]->Int32Value(), info[1]->Int32Value(), size));
    info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(S7Client::ReadSZLList) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  PS7SZLList SZLList = new TS7SZLList;
  int size = sizeof(TS7SZLList);
  if (!info[0]->IsFunction()) {
    int returnValue = s7client->snap7Client->ReadSZLList(SZLList, &size);

    if (returnValue == 0) {
      v8::Local<v8::Array> szl_list = s7client->S7SZLListToArray(SZLList, size);

      delete SZLList;
      info.GetReturnValue().Set(szl_list);
    } else {
      delete SZLList;
      info.GetReturnValue().Set(Nan::False());
    }
  } else {
    Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, READSZLLIST, SZLList
      , size));
    info.GetReturnValue().SetUndefined();
  }
}

v8::Local<v8::Array> S7Client::S7SZLListToArray(PS7SZLList SZLList, int count) {
  Nan::EscapableHandleScope scope;

  v8::Local<v8::Array> szl_list = Nan::New<v8::Array>(count);
  for (int i = 0; i < count; i++) {
    Nan::Set(szl_list, i, Nan::New<v8::Integer>((*SZLList).List[i]));
  }

  return scope.Escape(szl_list);
}

// Control functions
NAN_METHOD(S7Client::PlcHotStart) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsFunction()) {
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(
      s7client->snap7Client->PlcHotStart() == 0));
  } else {
    Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, PLCHOTSTART));
    info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(S7Client::PlcColdStart) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsFunction()) {
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(
      s7client->snap7Client->PlcColdStart() == 0));
  } else {
    Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, PLCCOLDSTART));
    info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(S7Client::PlcStop) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsFunction()) {
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(
      s7client->snap7Client->PlcStop() == 0));
  } else {
    Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, PLCSTOP));
    info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(S7Client::CopyRamToRom) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  if (!info[1]->IsFunction()) {
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(
      s7client->snap7Client->CopyRamToRom(info[0]->Int32Value()) == 0));
  } else {
    Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, COPYRAMTOROM
      , info[0]->Int32Value()));
    info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(S7Client::Compress) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  if (!info[1]->IsFunction()) {
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(
      s7client->snap7Client->Compress(info[0]->Int32Value()) == 0));
  } else {
    Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, COMPRESS
      , info[0]->Int32Value()));
    info.GetReturnValue().SetUndefined();
  }
}

// Security functions
NAN_METHOD(S7Client::GetProtection) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  PS7Protection S7Protection = new TS7Protection;
  if (!info[0]->IsFunction()) {
    int returnValue = s7client->snap7Client->GetProtection(S7Protection);

    if (returnValue == 0) {
      v8::Local<v8::Object> protection = s7client->S7ProtectionToObject(
        S7Protection);
      delete S7Protection;
      info.GetReturnValue().Set(protection);
    } else {
      delete S7Protection;
      info.GetReturnValue().Set(Nan::False());
    }
  } else {
    Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, GETPROTECTION
      , S7Protection));
    info.GetReturnValue().SetUndefined();
  }
}

v8::Local<v8::Object> S7Client::S7ProtectionToObject(
    PS7Protection S7Protection
) {
  Nan::EscapableHandleScope scope;

  v8::Local<v8::Object> protection = Nan::New<v8::Object>();
  Nan::Set(protection, Nan::New<v8::String>("sch_schal").ToLocalChecked()
    , Nan::New<v8::Number>(S7Protection->sch_schal));
  Nan::Set(protection, Nan::New<v8::String>("sch_par").ToLocalChecked()
    , Nan::New<v8::Number>(S7Protection->sch_par));
  Nan::Set(protection, Nan::New<v8::String>("sch_rel").ToLocalChecked()
    , Nan::New<v8::Number>(S7Protection->sch_rel));
  Nan::Set(protection, Nan::New<v8::String>("bart_sch").ToLocalChecked()
    , Nan::New<v8::Number>(S7Protection->bart_sch));
  Nan::Set(protection, Nan::New<v8::String>("anl_sch").ToLocalChecked()
    , Nan::New<v8::Number>(S7Protection->anl_sch));

  return scope.Escape(protection);
}

NAN_METHOD(S7Client::SetSessionPassword) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsString()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  Nan::Utf8String *password = new Nan::Utf8String(info[0]);
  if (!info[1]->IsFunction()) {
    v8::Local<v8::Boolean> ret = Nan::New<v8::Boolean>(
      s7client->snap7Client->SetSessionPassword(**password) == 0);
    delete password;
    info.GetReturnValue().Set(ret);
  } else {
    Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, SETSESSIONPW
      , password));
    info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(S7Client::ClearSessionPassword) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsFunction()) {
    info.GetReturnValue().Set(Nan::New<v8::Boolean>(
      s7client->snap7Client->ClearSessionPassword() == 0));
  } else {
    Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, CLEARSESSIONPW));
    info.GetReturnValue().SetUndefined();
  }
}

// Properties
NAN_METHOD(S7Client::ExecTime) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  int returnValue = s7client->snap7Client->ExecTime();
  if (returnValue == errLibInvalidObject) {
    info.GetReturnValue().Set(Nan::False());
  } else {
    info.GetReturnValue().Set(Nan::New<v8::Integer>(returnValue));
  }
}

NAN_METHOD(S7Client::LastError) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  info.GetReturnValue().Set(Nan::New<v8::Integer>(
    s7client->snap7Client->LastError()));
}

NAN_METHOD(S7Client::PDURequested) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  int returnValue = s7client->snap7Client->PDURequested();
  if (returnValue == 0) {
    info.GetReturnValue().Set(Nan::False());
  } else {
    info.GetReturnValue().Set(Nan::New<v8::Integer>(returnValue));
  }
}

NAN_METHOD(S7Client::PDULength) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  int returnValue = s7client->snap7Client->PDULength();
  if (returnValue == 0) {
    info.GetReturnValue().Set(Nan::False());
  } else {
    info.GetReturnValue().Set(Nan::New<v8::Integer>(returnValue));
  }
}

NAN_METHOD(S7Client::PlcStatus) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  if (!info[0]->IsFunction()) {
    int returnValue = s7client->snap7Client->PlcStatus();
    if ((returnValue == S7CpuStatusUnknown) ||
        (returnValue == S7CpuStatusStop) ||
        (returnValue == S7CpuStatusRun)) {
      info.GetReturnValue().Set(Nan::New<v8::Integer>(returnValue));
    } else {
      info.GetReturnValue().Set(Nan::False());
    }
  } else {
    Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new IOWorker(callback, s7client, PLCSTATUS));
    info.GetReturnValue().SetUndefined();
  }
}

NAN_METHOD(S7Client::Connected) {
  S7Client *s7client = ObjectWrap::Unwrap<S7Client>(info.Holder());

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(
    s7client->snap7Client->Connected()));
}

NAN_METHOD(S7Client::ErrorText) {
  if (!info[0]->IsInt32()) {
    return Nan::ThrowTypeError("Wrong arguments");
  }

  info.GetReturnValue().Set(Nan::New<v8::String>(
    CliErrorText(info[0]->Int32Value()).c_str()).ToLocalChecked());
}

}  // namespace node_snap7
