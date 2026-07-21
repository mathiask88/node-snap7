/*
 * Copyright (c) 2025, Mathias Küsel
 * MIT License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

#include <node_snap7_client.h>
#include <node_snap7_partner.h>
#include <node_snap7_server.h>
#include <snap7.h>

namespace node_snap7 {

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    // Client grouped constants
    Napi::Object clientError = Napi::Object::New(env);
    clientError.Set("NegotiatingPDU", Napi::Value::From(env, errNegotiatingPDU));
    clientError.Set("InvalidParams", Napi::Value::From(env, errCliInvalidParams));
    clientError.Set("JobPending", Napi::Value::From(env, errCliJobPending));
    clientError.Set("TooManyItems", Napi::Value::From(env, errCliTooManyItems));
    clientError.Set("InvalidWordLen", Napi::Value::From(env, errCliInvalidWordLen));
    clientError.Set("PartialDataWritten", Napi::Value::From(env, errCliPartialDataWritten));
    clientError.Set("SizeOverPDU", Napi::Value::From(env, errCliSizeOverPDU));
    clientError.Set("InvalidPlcAnswer", Napi::Value::From(env, errCliInvalidPlcAnswer));
    clientError.Set("AddressOutOfRange", Napi::Value::From(env, errCliAddressOutOfRange));
    clientError.Set("InvalidTransportSize", Napi::Value::From(env, errCliInvalidTransportSize));
    clientError.Set("WriteDataSizeMismatch", Napi::Value::From(env, errCliWriteDataSizeMismatch));
    clientError.Set("ItemNotAvailable", Napi::Value::From(env, errCliItemNotAvailable));
    clientError.Set("InvalidValue", Napi::Value::From(env, errCliInvalidValue));
    clientError.Set("CannotStartPLC", Napi::Value::From(env, errCliCannotStartPLC));
    clientError.Set("AlreadyRun", Napi::Value::From(env, errCliAlreadyRun));
    clientError.Set("CannotStopPLC", Napi::Value::From(env, errCliCannotStopPLC));
    clientError.Set("CannotCopyRamToRom", Napi::Value::From(env, errCliCannotCopyRamToRom));
    clientError.Set("CannotCompress", Napi::Value::From(env, errCliCannotCompress));
    clientError.Set("AlreadyStop", Napi::Value::From(env, errCliAlreadyStop));
    clientError.Set("FunNotAvailable", Napi::Value::From(env, errCliFunNotAvailable));
    clientError.Set("UploadSequenceFailed", Napi::Value::From(env, errCliUploadSequenceFailed));
    clientError.Set("InvalidDataSizeRecvd", Napi::Value::From(env, errCliInvalidDataSizeRecvd));
    clientError.Set("InvalidBlockType", Napi::Value::From(env, errCliInvalidBlockType));
    clientError.Set("InvalidBlockNumber", Napi::Value::From(env, errCliInvalidBlockNumber));
    clientError.Set("InvalidBlockSize", Napi::Value::From(env, errCliInvalidBlockSize));
    clientError.Set("DownloadSequenceFailed", Napi::Value::From(env, errCliDownloadSequenceFailed));
    clientError.Set("InsertRefused", Napi::Value::From(env, errCliInsertRefused));
    clientError.Set("DeleteRefused", Napi::Value::From(env, errCliDeleteRefused));
    clientError.Set("NeedPassword", Napi::Value::From(env, errCliNeedPassword));
    clientError.Set("InvalidPassword", Napi::Value::From(env, errCliInvalidPassword));
    clientError.Set("NoPasswordToSetOrClear", Napi::Value::From(env, errCliNoPasswordToSetOrClear));
    clientError.Set("JobTimeout", Napi::Value::From(env, errCliJobTimeout));
    clientError.Set("PartialDataRead", Napi::Value::From(env, errCliPartialDataRead));
    clientError.Set("BufferTooSmall", Napi::Value::From(env, errCliBufferTooSmall));
    clientError.Set("FunctionRefused", Napi::Value::From(env, errCliFunctionRefused));
    clientError.Set("Destroying", Napi::Value::From(env, errCliDestroying));
    clientError.Set("InvalidParamNumber", Napi::Value::From(env, errCliInvalidParamNumber));
    clientError.Set("CannotChangeParam", Napi::Value::From(env, errCliCannotChangeParam));

    Napi::Object connectionType = Napi::Object::New(env);
    connectionType.Set("PG", Napi::Value::From(env, CONNTYPE_PG));
    connectionType.Set("OP", Napi::Value::From(env, CONNTYPE_OP));
    connectionType.Set("BASIC", Napi::Value::From(env, CONNTYPE_BASIC));

    Napi::Object plcStatus = Napi::Object::New(env);
    plcStatus.Set("Unknown", Napi::Value::From(env, S7CpuStatusUnknown));
    plcStatus.Set("Run", Napi::Value::From(env, S7CpuStatusRun));
    plcStatus.Set("Stop", Napi::Value::From(env, S7CpuStatusStop));

    Napi::Object s7Area = Napi::Object::New(env);
    s7Area.Set("PE", Napi::Value::From(env, S7AreaPE));
    s7Area.Set("PA", Napi::Value::From(env, S7AreaPA));
    s7Area.Set("MK", Napi::Value::From(env, S7AreaMK));
    s7Area.Set("DB", Napi::Value::From(env, S7AreaDB));
    s7Area.Set("CT", Napi::Value::From(env, S7AreaCT));
    s7Area.Set("TM", Napi::Value::From(env, S7AreaTM));

    Napi::Object s7WordLen = Napi::Object::New(env);
    s7WordLen.Set("Bit", Napi::Value::From(env, S7WLBit));
    s7WordLen.Set("Byte", Napi::Value::From(env, S7WLByte));
    s7WordLen.Set("Word", Napi::Value::From(env, S7WLWord));
    s7WordLen.Set("DWord", Napi::Value::From(env, S7WLDWord));
    s7WordLen.Set("Real", Napi::Value::From(env, S7WLReal));
    s7WordLen.Set("Counter", Napi::Value::From(env, S7WLCounter));
    s7WordLen.Set("Timer", Napi::Value::From(env, S7WLTimer));

    Napi::Object blockType = Napi::Object::New(env);
    blockType.Set("OB", Napi::Value::From(env, Block_OB));
    blockType.Set("DB", Napi::Value::From(env, Block_DB));
    blockType.Set("SDB", Napi::Value::From(env, Block_SDB));
    blockType.Set("FC", Napi::Value::From(env, Block_FC));
    blockType.Set("SFC", Napi::Value::From(env, Block_SFC));
    blockType.Set("FB", Napi::Value::From(env, Block_FB));
    blockType.Set("SFB", Napi::Value::From(env, Block_SFB));

    Napi::Object subBlockType = Napi::Object::New(env);
    subBlockType.Set("OB", Napi::Value::From(env, SubBlk_OB));
    subBlockType.Set("DB", Napi::Value::From(env, SubBlk_DB));
    subBlockType.Set("SDB", Napi::Value::From(env, SubBlk_SDB));
    subBlockType.Set("FC", Napi::Value::From(env, SubBlk_FC));
    subBlockType.Set("SFC", Napi::Value::From(env, SubBlk_SFC));
    subBlockType.Set("FB", Napi::Value::From(env, SubBlk_FB));
    subBlockType.Set("SFB", Napi::Value::From(env, SubBlk_SFB));

    Napi::Object blockLang = Napi::Object::New(env);
    blockLang.Set("AWL", Napi::Value::From(env, BlockLangAWL));
    blockLang.Set("KOP", Napi::Value::From(env, BlockLangKOP));
    blockLang.Set("FUP", Napi::Value::From(env, BlockLangFUP));
    blockLang.Set("SCL", Napi::Value::From(env, BlockLangSCL));
    blockLang.Set("DB", Napi::Value::From(env, BlockLangDB));
    blockLang.Set("GRAPH", Napi::Value::From(env, BlockLangGRAPH));

    Napi::Object clientParameter = Napi::Object::New(env);
    clientParameter.Set("RemotePort", Napi::Value::From(env, p_u16_RemotePort));
    clientParameter.Set("PingTimeout", Napi::Value::From(env, p_i32_PingTimeout));
    clientParameter.Set("SendTimeout", Napi::Value::From(env, p_i32_SendTimeout));
    clientParameter.Set("RecvTimeout", Napi::Value::From(env, p_i32_RecvTimeout));
    clientParameter.Set("SrcRef", Napi::Value::From(env, p_u16_SrcRef));
    clientParameter.Set("DstRef", Napi::Value::From(env, p_u16_DstRef));
    clientParameter.Set("SrcTSap", Napi::Value::From(env, p_u16_SrcTSap));
    clientParameter.Set("PDURequest", Napi::Value::From(env, p_i32_PDURequest));

    // Server grouped constants
    Napi::Object serverParameter = Napi::Object::New(env);
    serverParameter.Set("LocalPort", Napi::Value::From(env, p_u16_LocalPort));
    serverParameter.Set("WorkInterval", Napi::Value::From(env, p_i32_WorkInterval));
    serverParameter.Set("PDURequest", Napi::Value::From(env, p_i32_PDURequest));
    serverParameter.Set("MaxClients", Napi::Value::From(env, p_i32_MaxClients));

    Napi::Object serverError = Napi::Object::New(env);
    serverError.Set("CannotStart", Napi::Value::From(env, errSrvCannotStart));
    serverError.Set("DBNullPointer", Napi::Value::From(env, errSrvDBNullPointer));
    serverError.Set("AreaAlreadyExists", Napi::Value::From(env, errSrvAreaAlreadyExists));
    serverError.Set("UnknownArea", Napi::Value::From(env, errSrvUnknownArea));
    serverError.Set("InvalidParams", Napi::Value::From(env, errSrvInvalidParams));
    serverError.Set("TooManyDB", Napi::Value::From(env, errSrvTooManyDB));
    serverError.Set("InvalidParamNumber", Napi::Value::From(env, errSrvInvalidParamNumber));
    serverError.Set("CannotChangeParam", Napi::Value::From(env, errSrvCannotChangeParam));

    Napi::Object serverArea = Napi::Object::New(env);
    serverArea.Set("PE", Napi::Value::From(env, srvAreaPE));
    serverArea.Set("PA", Napi::Value::From(env, srvAreaPA));
    serverArea.Set("MK", Napi::Value::From(env, srvAreaMK));
    serverArea.Set("CT", Napi::Value::From(env, srvAreaCT));
    serverArea.Set("TM", Napi::Value::From(env, srvAreaTM));
    serverArea.Set("DB", Napi::Value::From(env, srvAreaDB));

    Napi::Object serverEventCode = Napi::Object::New(env);
    serverEventCode.Set("ServerStarted", Napi::Value::From(env, evcServerStarted));
    serverEventCode.Set("ServerStopped", Napi::Value::From(env, evcServerStopped));
    serverEventCode.Set("ListenerCannotStart", Napi::Value::From(env, evcListenerCannotStart));
    serverEventCode.Set("ClientAdded", Napi::Value::From(env, evcClientAdded));
    serverEventCode.Set("ClientRejected", Napi::Value::From(env, evcClientRejected));
    serverEventCode.Set("ClientNoRoom", Napi::Value::From(env, evcClientNoRoom));
    serverEventCode.Set("ClientException", Napi::Value::From(env, evcClientException));
    serverEventCode.Set("ClientDisconnected", Napi::Value::From(env, evcClientDisconnected));
    serverEventCode.Set("ClientTerminated", Napi::Value::From(env, evcClientTerminated));
    serverEventCode.Set("ClientsDropped", Napi::Value::From(env, evcClientsDropped));
    serverEventCode.Set("PDUincoming", Napi::Value::From(env, evcPDUincoming));
    serverEventCode.Set("DataRead", Napi::Value::From(env, evcDataRead));
    serverEventCode.Set("DataWrite", Napi::Value::From(env, evcDataWrite));
    serverEventCode.Set("NegotiatePDU", Napi::Value::From(env, evcNegotiatePDU));
    serverEventCode.Set("ReadSZL", Napi::Value::From(env, evcReadSZL));
    serverEventCode.Set("Clock", Napi::Value::From(env, evcClock));
    serverEventCode.Set("Upload", Napi::Value::From(env, evcUpload));
    serverEventCode.Set("Download", Napi::Value::From(env, evcDownload));
    serverEventCode.Set("Directory", Napi::Value::From(env, evcDirectory));
    serverEventCode.Set("Security", Napi::Value::From(env, evcSecurity));
    serverEventCode.Set("Control", Napi::Value::From(env, evcControl));
    serverEventCode.Set("All", Napi::Value::From(env, evcAll));
    serverEventCode.Set("None", Napi::Value::From(env, evcNone));

    Napi::Object serverEventSubcode = Napi::Object::New(env);
    serverEventSubcode.Set("Unknown", Napi::Value::From(env, evsUnknown));
    serverEventSubcode.Set("StartUpload", Napi::Value::From(env, evsStartUpload));
    serverEventSubcode.Set("StartDownload", Napi::Value::From(env, evsStartDownload));
    serverEventSubcode.Set("GetBlockList", Napi::Value::From(env, evsGetBlockList));
    serverEventSubcode.Set("StartListBoT", Napi::Value::From(env, evsStartListBoT));
    serverEventSubcode.Set("ListBoT", Napi::Value::From(env, evsListBoT));
    serverEventSubcode.Set("GetBlockInfo", Napi::Value::From(env, evsGetBlockInfo));
    serverEventSubcode.Set("GetClock", Napi::Value::From(env, evsGetClock));
    serverEventSubcode.Set("SetClock", Napi::Value::From(env, evsSetClock));
    serverEventSubcode.Set("SetPassword", Napi::Value::From(env, evsSetPassword));
    serverEventSubcode.Set("ClrPassword", Napi::Value::From(env, evsClrPassword));

    Napi::Object serverEventGroup = Napi::Object::New(env);
    serverEventGroup.Set("Programmer", Napi::Value::From(env, grProgrammer));
    serverEventGroup.Set("CyclicData", Napi::Value::From(env, grCyclicData));
    serverEventGroup.Set("BlocksInfo", Napi::Value::From(env, grBlocksInfo));
    serverEventGroup.Set("SZL", Napi::Value::From(env, grSZL));
    serverEventGroup.Set("Password", Napi::Value::From(env, grPassword));
    serverEventGroup.Set("BSend", Napi::Value::From(env, grBSend));
    serverEventGroup.Set("Clock", Napi::Value::From(env, grClock));
    serverEventGroup.Set("Security", Napi::Value::From(env, grSecurity));

    Napi::Object serverEventControl = Napi::Object::New(env);
    serverEventControl.Set("Unknown", Napi::Value::From(env, CodeControlUnknown));
    serverEventControl.Set("ColdStart", Napi::Value::From(env, CodeControlColdStart));
    serverEventControl.Set("WarmStart", Napi::Value::From(env, CodeControlWarmStart));
    serverEventControl.Set("Stop", Napi::Value::From(env, CodeControlStop));
    serverEventControl.Set("Compress", Napi::Value::From(env, CodeControlCompress));
    serverEventControl.Set("CpyRamRom", Napi::Value::From(env, CodeControlCpyRamRom));
    serverEventControl.Set("InsDel", Napi::Value::From(env, CodeControlInsDel));

    Napi::Object serverEventResult = Napi::Object::New(env);
    serverEventResult.Set("NoError", Napi::Value::From(env, evrNoError));
    serverEventResult.Set("FragmentRejected", Napi::Value::From(env, evrFragmentRejected));
    serverEventResult.Set("MalformedPDU", Napi::Value::From(env, evrMalformedPDU));
    serverEventResult.Set("SparseBytes", Napi::Value::From(env, evrSparseBytes));
    serverEventResult.Set("CannotHandlePDU", Napi::Value::From(env, evrCannotHandlePDU));
    serverEventResult.Set("NotImplemented", Napi::Value::From(env, evrNotImplemented));
    serverEventResult.Set("ErrException", Napi::Value::From(env, evrErrException));
    serverEventResult.Set("ErrAreaNotFound", Napi::Value::From(env, evrErrAreaNotFound));
    serverEventResult.Set("ErrOutOfRange", Napi::Value::From(env, evrErrOutOfRange));
    serverEventResult.Set("ErrOverPDU", Napi::Value::From(env, evrErrOverPDU));
    serverEventResult.Set("ErrTransportSize", Napi::Value::From(env, evrErrTransportSize));
    serverEventResult.Set("InvalidGroupUData", Napi::Value::From(env, evrInvalidGroupUData));
    serverEventResult.Set("InvalidSZL", Napi::Value::From(env, evrInvalidSZL));
    serverEventResult.Set("DataSizeMismatch", Napi::Value::From(env, evrDataSizeMismatch));
    serverEventResult.Set("CannotUpload", Napi::Value::From(env, evrCannotUpload));
    serverEventResult.Set("CannotDownload", Napi::Value::From(env, evrCannotDownload));
    serverEventResult.Set("UploadInvalidID", Napi::Value::From(env, evrUploadInvalidID));
    serverEventResult.Set("ResNotFound", Napi::Value::From(env, evrResNotFound));

    Napi::Object serverStatus = Napi::Object::New(env);
    serverStatus.Set("SrvStopped", Napi::Value::From(env, 0));
    serverStatus.Set("SrvRunning", Napi::Value::From(env, 1));
    serverStatus.Set("SrvError", Napi::Value::From(env, 2));

    Napi::Object serverOperation = Napi::Object::New(env);
    serverOperation.Set("Read", Napi::Value::From(env, OperationRead));
    serverOperation.Set("Write", Napi::Value::From(env, OperationWrite));

    Napi::Object partnerError = Napi::Object::New(env);
    partnerError.Set("BindError", Napi::Value::From(env, errParBindError));
    partnerError.Set("Destroying", Napi::Value::From(env, errParDestroying));
    partnerError.Set("InvalidParamNumber", Napi::Value::From(env, errParInvalidParamNumber));
    partnerError.Set("CannotChangeParam", Napi::Value::From(env, errParCannotChangeParam));
    partnerError.Set("BufferTooSmall", Napi::Value::From(env, errParBufferTooSmall));

    Napi::Object partnerParameter = Napi::Object::New(env);
    partnerParameter.Set("PDURequest", Napi::Value::From(env, p_i32_PDURequest));
    partnerParameter.Set("SrcTSap", Napi::Value::From(env, p_u16_SrcTSap));
    partnerParameter.Set("DstRef", Napi::Value::From(env, p_u16_DstRef));
    partnerParameter.Set("SrcRef", Napi::Value::From(env, p_u16_SrcRef));
    partnerParameter.Set("WorkInterval", Napi::Value::From(env, p_i32_WorkInterval));
    partnerParameter.Set("RecvTimeout", Napi::Value::From(env, p_i32_RecvTimeout));
    partnerParameter.Set("SendTimeout", Napi::Value::From(env, p_i32_SendTimeout));
    partnerParameter.Set("PingTimeout", Napi::Value::From(env, p_i32_PingTimeout));
    partnerParameter.Set("RemotePort", Napi::Value::From(env, p_u16_RemotePort));
    partnerParameter.Set("BSendTimeout", Napi::Value::From(env, p_i32_BSendTimeout));
    partnerParameter.Set("BRecvTimeout", Napi::Value::From(env, p_i32_BRecvTimeout));
    partnerParameter.Set("RecoveryTime", Napi::Value::From(env, p_u32_RecoveryTime));
    partnerParameter.Set("KeepAliveTime", Napi::Value::From(env, p_u32_KeepAliveTime));

    Napi::Object partnerStatus = Napi::Object::New(env);
    partnerStatus.Set("Stopped", Napi::Value::From(env, par_stopped));
    partnerStatus.Set("Connecting", Napi::Value::From(env, par_connecting));
    partnerStatus.Set("Waiting", Napi::Value::From(env, par_waiting));
    partnerStatus.Set("Linked", Napi::Value::From(env, par_linked));
    partnerStatus.Set("Sending", Napi::Value::From(env, par_sending));
    partnerStatus.Set("Receiving", Napi::Value::From(env, par_receiving));
    partnerStatus.Set("BindError", Napi::Value::From(env, par_binderror));

    // Export grouped constants
    exports.Set("ClientError", clientError);
    exports.Set("ClientParameter", clientParameter);
    exports.Set("ConnectionType", connectionType);
    exports.Set("S7Area", s7Area);
    exports.Set("S7WordLen", s7WordLen);
    exports.Set("BlockType", blockType);
    exports.Set("SubBlockType", subBlockType);
    exports.Set("BlockLang", blockLang);
    exports.Set("PlcStatus", plcStatus);

    exports.Set("ServerError", serverError);
    exports.Set("ServerParameter", serverParameter);
    exports.Set("ServerArea", serverArea);
    exports.Set("ServerOperation", serverOperation);
    exports.Set("ServerEventCode", serverEventCode);
    exports.Set("ServerEventSubcode", serverEventSubcode);
    exports.Set("ServerEventGroup", serverEventGroup);
    exports.Set("ServerEventControl", serverEventControl);
    exports.Set("ServerEventResult", serverEventResult);
    exports.Set("ServerStatus", serverStatus);

    exports.Set("PartnerError", partnerError);
    exports.Set("PartnerParameter", partnerParameter);
    exports.Set("PartnerStatus", partnerStatus);

    S7Client::Init(env, exports);
    S7Server::Init(env, exports);
    S7Partner::Init(env, exports);

    return exports;
}

NODE_API_MODULE(node_snap7, InitAll)

} // namespace node_snap7
