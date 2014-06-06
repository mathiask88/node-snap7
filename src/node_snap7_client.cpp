/*
* Copyright (c) 2014, Mathias Küsel
* MIT +no-false-attribs License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
*/

#include "./node_snap7_client.h"
#include "nan.h"
#include <node_buffer.h>
#include <sstream>
#include <cstdlib>


namespace node_snap7{

	v8::Persistent<v8::FunctionTemplate> S7Client::constructor;

	void S7Client::Init(v8::Handle<v8::Object> exports) {
		NanScope();

		v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(S7Client::New);

		v8::Local<v8::String> name = NanNew<v8::String>("S7Client");
		tpl->SetClassName(name);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		// Setup the prototype
		// Control functions
		NODE_SET_PROTOTYPE_METHOD(tpl, "Connect", S7Client::Connect);
		NODE_SET_PROTOTYPE_METHOD(tpl, "ConnectTo", S7Client::ConnectTo);
		NODE_SET_PROTOTYPE_METHOD(tpl, "SetConnectionParams", S7Client::SetConnectionParams);
		NODE_SET_PROTOTYPE_METHOD(tpl, "SetConnectionType", S7Client::SetConnectionType);
		NODE_SET_PROTOTYPE_METHOD(tpl, "Disconnect", S7Client::Disconnect);
		NODE_SET_PROTOTYPE_METHOD(tpl, "GetParam", S7Client::GetParam);
		NODE_SET_PROTOTYPE_METHOD(tpl, "SetParam", S7Client::SetParam);
		// Data I/O Main functions
		NODE_SET_PROTOTYPE_METHOD(tpl, "ReadArea", S7Client::ReadArea);
		NODE_SET_PROTOTYPE_METHOD(tpl, "WriteArea", S7Client::WriteArea);
		NODE_SET_PROTOTYPE_METHOD(tpl, "ReadMultiVars", S7Client::ReadMultiVars);
		NODE_SET_PROTOTYPE_METHOD(tpl, "WriteMultiVars", S7Client::WriteMultiVars);
		// Directory functions
		NODE_SET_PROTOTYPE_METHOD(tpl, "ListBlocks", S7Client::ListBlocks);
		NODE_SET_PROTOTYPE_METHOD(tpl, "GetAgBlockInfo", S7Client::GetAgBlockInfo);
		NODE_SET_PROTOTYPE_METHOD(tpl, "GetPgBlockInfo", S7Client::GetPgBlockInfo);
		NODE_SET_PROTOTYPE_METHOD(tpl, "ListBlocksOfType", S7Client::ListBlocksOfType);
		// Blocks functions
		NODE_SET_PROTOTYPE_METHOD(tpl, "Upload", S7Client::Upload);
		NODE_SET_PROTOTYPE_METHOD(tpl, "FullUpload", S7Client::FullUpload);
		NODE_SET_PROTOTYPE_METHOD(tpl, "Download", S7Client::Download);
		NODE_SET_PROTOTYPE_METHOD(tpl, "Delete", S7Client::Delete);
		NODE_SET_PROTOTYPE_METHOD(tpl, "DBGet", S7Client::DBGet);
		NODE_SET_PROTOTYPE_METHOD(tpl, "DBFill", S7Client::DBFill);
		// Date/Time functions
		NODE_SET_PROTOTYPE_METHOD(tpl, "GetPlcDateTime", S7Client::GetPlcDateTime);
		NODE_SET_PROTOTYPE_METHOD(tpl, "SetPlcDateTime", S7Client::SetPlcDateTime);
		NODE_SET_PROTOTYPE_METHOD(tpl, "SetPlcSystemDateTime", S7Client::SetPlcSystemDateTime);
		// System Info functions
		NODE_SET_PROTOTYPE_METHOD(tpl, "GetOrderCode", S7Client::GetOrderCode);
		NODE_SET_PROTOTYPE_METHOD(tpl, "GetCpuInfo", S7Client::GetCpuInfo);
		NODE_SET_PROTOTYPE_METHOD(tpl, "GetCpInfo", S7Client::GetCpInfo);
		NODE_SET_PROTOTYPE_METHOD(tpl, "ReadSZL", S7Client::ReadSZL);
		NODE_SET_PROTOTYPE_METHOD(tpl, "ReadSZLList", S7Client::ReadSZLList);
		// Control functions
		NODE_SET_PROTOTYPE_METHOD(tpl, "PlcHotStart", S7Client::PlcHotStart);
		NODE_SET_PROTOTYPE_METHOD(tpl, "PlcColdStart", S7Client::PlcColdStart);
		NODE_SET_PROTOTYPE_METHOD(tpl, "PlcStop", S7Client::PlcStop);
		NODE_SET_PROTOTYPE_METHOD(tpl, "CopyRamToRom", S7Client::CopyRamToRom);
		NODE_SET_PROTOTYPE_METHOD(tpl, "Compress", S7Client::Compress);
		// Security functions
		NODE_SET_PROTOTYPE_METHOD(tpl, "GetProtection", S7Client::GetProtection);
		NODE_SET_PROTOTYPE_METHOD(tpl, "SetSessionPassword", S7Client::SetSessionPassword);
		NODE_SET_PROTOTYPE_METHOD(tpl, "ClearSessionPassword", S7Client::ClearSessionPassword);
		// Properties
		NODE_SET_PROTOTYPE_METHOD(tpl, "ExecTime", S7Client::ExecTime);
		NODE_SET_PROTOTYPE_METHOD(tpl, "LastError", S7Client::LastError);
		NODE_SET_PROTOTYPE_METHOD(tpl, "PDURequested", S7Client::PDURequested);
		NODE_SET_PROTOTYPE_METHOD(tpl, "PDULength", S7Client::PDULength);
		NODE_SET_PROTOTYPE_METHOD(tpl, "PlcStatus", S7Client::PlcStatus);
		NODE_SET_PROTOTYPE_METHOD(tpl, "Connected", S7Client::Connected);
		// Error to text function
		NODE_SET_PROTOTYPE_METHOD(tpl, "ErrorText", S7Client::ErrorText);
		// Error codes
		NanSetPrototypeTemplate(tpl, "errNegotiatingPDU", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliInvalidParams", NanNew<v8::Integer>(errCliInvalidParams));
		NanSetPrototypeTemplate(tpl, "errCliJobPending", NanNew<v8::Integer>(errCliJobPending));
		NanSetPrototypeTemplate(tpl, "errCliTooManyItems", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliInvalidWordLen", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliPartialDataWritten", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliSizeOverPDU", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliInvalidPlcAnswer", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliAddressOutOfRange", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliInvalidTransportSize", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliWriteDataSizeMismatch", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliItemNotAvailable", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliInvalidValue", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliCannotStartPLC", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliAlreadyRun", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliCannotStopPLC", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliCannotCopyRamToRom", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliCannotCompress", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliAlreadyStop", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliFunNotAvailable", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliUploadSequenceFailed", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliInvalidDataSizeRecvd", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliInvalidBlockType", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliInvalidBlockNumber", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliInvalidBlockSize", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliDownloadSequenceFailed", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliInsertRefused", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliDeleteRefused", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliNeedPassword", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliInvalidPassword", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliNoPasswordToSetOrClear", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliJobTimeout", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliPartialDataRead", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliBufferTooSmall", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliFunctionRefused", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliDestroying", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliInvalidParamNumber", NanNew<v8::Integer>(errNegotiatingPDU));
		NanSetPrototypeTemplate(tpl, "errCliCannotChangeParam", NanNew<v8::Integer>(errNegotiatingPDU));
		// Client Connection Type
		NanSetPrototypeTemplate(tpl, "CONNTYPE_PG", NanNew<v8::Integer>(CONNTYPE_PG));
		NanSetPrototypeTemplate(tpl, "CONNTYPE_OP", NanNew<v8::Integer>(CONNTYPE_OP));
		NanSetPrototypeTemplate(tpl, "CONNTYPE_BASIC", NanNew<v8::Integer>(CONNTYPE_BASIC));

		// Area ID
		NanSetPrototypeTemplate(tpl, "S7AreaPE", NanNew<v8::Integer>(S7AreaPE));
		NanSetPrototypeTemplate(tpl, "S7AreaPA", NanNew<v8::Integer>(S7AreaPA));
		NanSetPrototypeTemplate(tpl, "S7AreaMK", NanNew<v8::Integer>(S7AreaMK));
		NanSetPrototypeTemplate(tpl, "S7AreaDB", NanNew<v8::Integer>(S7AreaDB));
		NanSetPrototypeTemplate(tpl, "S7AreaCT", NanNew<v8::Integer>(S7AreaCT));
		NanSetPrototypeTemplate(tpl, "S7AreaTM", NanNew<v8::Integer>(S7AreaTM));

		// Word Length
		NanSetPrototypeTemplate(tpl, "S7WLBit", NanNew<v8::Integer>(S7WLBit));
		NanSetPrototypeTemplate(tpl, "S7WLByte", NanNew<v8::Integer>(S7WLByte));
		NanSetPrototypeTemplate(tpl, "S7WLWord", NanNew<v8::Integer>(S7WLWord));
		NanSetPrototypeTemplate(tpl, "S7WLDWord", NanNew<v8::Integer>(S7WLDWord));
		NanSetPrototypeTemplate(tpl, "S7WLReal", NanNew<v8::Integer>(S7WLReal));
		NanSetPrototypeTemplate(tpl, "S7WLCounter", NanNew<v8::Integer>(S7WLCounter));
		NanSetPrototypeTemplate(tpl, "S7WLTimer", NanNew<v8::Integer>(S7WLTimer));

		// Block type
		NanSetPrototypeTemplate(tpl, "Block_OB", NanNew<v8::Integer>(Block_OB));
		NanSetPrototypeTemplate(tpl, "Block_DB", NanNew<v8::Integer>(Block_DB));
		NanSetPrototypeTemplate(tpl, "Block_SDB", NanNew<v8::Integer>(Block_SDB));
		NanSetPrototypeTemplate(tpl, "Block_FC", NanNew<v8::Integer>(Block_FC));
		NanSetPrototypeTemplate(tpl, "Block_SFC", NanNew<v8::Integer>(Block_SFC));
		NanSetPrototypeTemplate(tpl, "Block_FB", NanNew<v8::Integer>(Block_FB));
		NanSetPrototypeTemplate(tpl, "Block_SFB", NanNew<v8::Integer>(Block_SFB));

		// Sub Block Type
		NanSetPrototypeTemplate(tpl, "SubBlk_OB", NanNew<v8::Integer>(SubBlk_OB));
		NanSetPrototypeTemplate(tpl, "SubBlk_SDB", NanNew<v8::Integer>(SubBlk_SDB));
		NanSetPrototypeTemplate(tpl, "SubBlk_FC", NanNew<v8::Integer>(SubBlk_FC));
		NanSetPrototypeTemplate(tpl, "SubBlk_SFC", NanNew<v8::Integer>(SubBlk_SFC));
		NanSetPrototypeTemplate(tpl, "SubBlk_FB", NanNew<v8::Integer>(SubBlk_FB));
		NanSetPrototypeTemplate(tpl, "SubBlk_SFB", NanNew<v8::Integer>(SubBlk_SFB));

		// Block languages
		NanSetPrototypeTemplate(tpl, "BlockLangAWL", NanNew<v8::Integer>(BlockLangAWL));
		NanSetPrototypeTemplate(tpl, "BlockLangKOP", NanNew<v8::Integer>(BlockLangKOP));
		NanSetPrototypeTemplate(tpl, "BlockLangFUP", NanNew<v8::Integer>(BlockLangFUP));
		NanSetPrototypeTemplate(tpl, "BlockLangSCL", NanNew<v8::Integer>(BlockLangSCL));
		NanSetPrototypeTemplate(tpl, "BlockLangDB", NanNew<v8::Integer>(BlockLangDB));
		NanSetPrototypeTemplate(tpl, "BlockLangGRAPH", NanNew<v8::Integer>(BlockLangGRAPH));

		// Parameter
		NanSetPrototypeTemplate(tpl, "LocalPort", NanNew<v8::Integer>(p_u16_LocalPort));
		NanSetPrototypeTemplate(tpl, "RemotePort", NanNew<v8::Integer>(p_u16_RemotePort));
		NanSetPrototypeTemplate(tpl, "PingTimeout", NanNew<v8::Integer>(p_i32_PingTimeout));
		NanSetPrototypeTemplate(tpl, "SendTimeout", NanNew<v8::Integer>(p_i32_SendTimeout));
		NanSetPrototypeTemplate(tpl, "RecvTimeout", NanNew<v8::Integer>(p_i32_RecvTimeout));
		NanSetPrototypeTemplate(tpl, "WorkInterval", NanNew<v8::Integer>(p_i32_WorkInterval));
		NanSetPrototypeTemplate(tpl, "SrcRef", NanNew<v8::Integer>(p_u16_SrcRef));
		NanSetPrototypeTemplate(tpl, "DstRef", NanNew<v8::Integer>(p_u16_DstRef));
		NanSetPrototypeTemplate(tpl, "SrcTSap", NanNew<v8::Integer>(p_u16_SrcTSap));
		NanSetPrototypeTemplate(tpl, "PDURequest", NanNew<v8::Integer>(p_i32_PDURequest));
		NanSetPrototypeTemplate(tpl, "MaxClients", NanNew<v8::Integer>(p_i32_MaxClients));
		NanSetPrototypeTemplate(tpl, "BSendTimeout", NanNew<v8::Integer>(p_i32_BSendTimeout));
		NanSetPrototypeTemplate(tpl, "BRecvTimeout", NanNew<v8::Integer>(p_i32_BRecvTimeout));
		NanSetPrototypeTemplate(tpl, "RecoveryTime", NanNew<v8::Integer>(p_u32_RecoveryTime));
		NanSetPrototypeTemplate(tpl, "KeepAliveTime", NanNew<v8::Integer>(p_u32_KeepAliveTime));

		NanAssignPersistent<v8::FunctionTemplate>(constructor, tpl);
		exports->Set(name, tpl->GetFunction());
	}

	NAN_METHOD(S7Client::New) {
		NanScope();
		if (args.IsConstructCall()) {
			S7Client *s7client = new S7Client();
			s7client->snap7Client = new TS7Client();
			uv_mutex_init(&s7client->mutex);

			s7client->Wrap(args.This());
			NanReturnValue(args.This());
		}
		else{
			v8::Local<v8::FunctionTemplate> constructorHandle = NanNew<v8::FunctionTemplate>(constructor);
			NanReturnValue(constructorHandle->GetFunction()->NewInstance());
		}
	}

	S7Client::~S7Client(){
		delete snap7Client;
		NanDisposePersistent(constructor);
		uv_mutex_destroy(&mutex);
	}

	int GetByteCountFromWordLen(int WordLen){
		switch (WordLen){
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
			NanThrowRangeError("Unknown wordlen");
			return 0;
		}
	}

	// Control functions
	void ConnectionWorker::Execute(){
		if (strcmp(address, "") == 0)
			returnValue = s7client->snap7Client->Connect();
		else
			returnValue = s7client->snap7Client->ConnectTo(address, rack, slot);

		delete[] address;
	}

	void ConnectionWorker::HandleOKCallback(){
		NanScope();

		v8::Local<v8::Value> argv[1];
		if (returnValue == 0)
			argv[0] = NanNull();
		else
			argv[0] = NanNew<v8::Integer>(returnValue);

		callback->Call(1, argv);
	}

	NAN_METHOD(S7Client::Connect){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsFunction()){
			NanReturnValue(NanNew<v8::Boolean>(s7client->snap7Client->Connect() == 0));
		}
		else{
			NanCallback *callback = new NanCallback(args[0].As<v8::Function>());
			NanAsyncQueueWorker(new ConnectionWorker(callback, s7client, "", 0, 0));
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Client::ConnectTo){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (args.Length() < 3) {
			return NanThrowTypeError("Wrong number of arguments");
		}

		if (!args[0]->IsString() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
			return NanThrowTypeError("Wrong arguments");
		}

		size_t len;
		char* remAddress = NanCString(args[0], &len);

		if (!args[3]->IsFunction()){
			v8::Local<v8::Boolean> ret = NanNew<v8::Boolean>(s7client->snap7Client->ConnectTo(remAddress, args[1]->Int32Value(), args[2]->Int32Value()) == 0);
			delete[] remAddress;
			NanReturnValue(ret);
		}
		else{
			NanCallback *callback = new NanCallback(args[3].As<v8::Function>());
			NanAsyncQueueWorker(new ConnectionWorker(callback, s7client, remAddress, args[1]->Int32Value(), args[2]->Int32Value()));
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Client::SetConnectionParams){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsString() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
			return NanThrowTypeError("Wrong arguments");
		}

		size_t len;
		char* remAddress = NanCString(args[0], &len);

		word LocalTSAP = args[1]->Int32Value();
		word RemoteTSAP = args[2]->Int32Value();

		v8::Local<v8::Boolean> ret = NanNew<v8::Boolean>(s7client->snap7Client->SetConnectionParams(remAddress, LocalTSAP, RemoteTSAP) == 0);
		delete[] remAddress;
		NanReturnValue(ret);
	}

	NAN_METHOD(S7Client::SetConnectionType){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsNumber()) {
			return NanThrowTypeError("Wrong arguments");
		}

		word type = args[0]->Uint32Value();

		NanReturnValue(NanNew<v8::Boolean>(s7client->snap7Client->SetConnectionType(type) == 0));
	}

	NAN_METHOD(S7Client::Disconnect){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		NanReturnValue(NanNew<v8::Boolean>(s7client->snap7Client->Disconnect() == 0));
	}

	NAN_METHOD(S7Client::GetParam){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsNumber() || !args[2]->IsObject()) {
			return NanThrowTypeError("Wrong arguments");
		}

		NanReturnUndefined();
	}

	NAN_METHOD(S7Client::SetParam){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		NanReturnUndefined();
	}

	// Data I/O Main functions
	void IOWorker::Execute(){
		uv_mutex_lock(&s7client->mutex);

		switch (caller){
		case READAREA: returnValue = s7client->snap7Client->ReadArea(area, DB, start, amount, WordLen, bufferData);  break;
		case WRITEAREA: returnValue = s7client->snap7Client->WriteArea(area, DB, start, amount, WordLen, bufferData); break;
		}
	}

	void IOWorker::HandleOKCallback(){
		NanScope();

		v8::Local<v8::Value> argv1[1];
		v8::Local<v8::Value> argv2[2];
		if (returnValue == 0){
			argv2[0] = argv1[0] = NanNull();
		}
		else{
			argv2[0] = argv1[0] = NanNew<v8::Integer>(returnValue);
		}

		int size = amount * GetByteCountFromWordLen(WordLen);
		switch (caller){
		case READAREA:
			argv2[1] = NanNewBufferHandle(bufferData, size);
			delete[] bufferData;
			callback->Call(2, argv2);
			break;
		case WRITEAREA:
			callback->Call(1, argv1);
			break;
		}

		uv_mutex_unlock(&s7client->mutex);
	}

	NAN_METHOD(S7Client::ReadArea){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (args.Length() < 5)
			return NanThrowTypeError("Wrong number of Arguments");

		if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber() || !args[3]->IsNumber() || !args[4]->IsNumber())
			return NanThrowTypeError("Wrong arguments");

		int amount = args[3]->Int32Value();
		int byteCount = GetByteCountFromWordLen(args[4]->Int32Value());
		if (byteCount == 0) NanReturnValue(NanFalse());
		int size = amount * byteCount;
		char* bufferData = new char[size];

		if (!args[5]->IsFunction()){
			int returnValue = s7client->snap7Client->ReadArea(args[0]->Int32Value(), args[1]->Int32Value(), args[2]->Int32Value(), args[3]->Int32Value(), args[4]->Int32Value(), bufferData);

			if (returnValue == 0){
				v8::Handle<v8::Value> ret = NanNewBufferHandle(bufferData, size);
				delete[] bufferData;
				NanReturnValue(ret);
			}
			else{
				delete[] bufferData;
				NanReturnValue(NanFalse());
			}

		}
		else{
			NanCallback *callback = new NanCallback(args[5].As<v8::Function>());
			NanAsyncQueueWorker(new IOWorker(callback, s7client, READAREA, args[0]->Int32Value(), args[1]->Int32Value(), args[2]->Int32Value(), args[3]->Int32Value(), args[4]->Uint32Value(), bufferData));
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Client::WriteArea){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (args.Length() < 6)
			return NanThrowTypeError("Wrong number of Arguments");

		if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber() || !args[3]->IsNumber() || !args[4]->IsNumber() || !node::Buffer::HasInstance(args[5]))
			return NanThrowTypeError("Wrong arguments");

		if (!args[6]->IsFunction()){
			NanReturnValue(NanNew<v8::Boolean>(s7client->snap7Client->WriteArea(args[0]->Int32Value(), args[1]->Int32Value(), args[2]->Int32Value(), args[3]->Int32Value(), args[4]->Int32Value(), node::Buffer::Data(args[5].As<v8::Object>())) == 0));
		}
		else{
			NanCallback *callback = new NanCallback(args[6].As<v8::Function>());
			NanAsyncQueueWorker(new IOWorker(callback, s7client, WRITEAREA, args[0]->Int32Value(), args[1]->Int32Value(), args[2]->Int32Value(), args[3]->Int32Value(), args[4]->Uint32Value(), node::Buffer::Data(args[5].As<v8::Object>())));
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Client::ReadMultiVars){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (args.Length() < 1) {
			return NanThrowTypeError("Wrong number of arguments");
		}

		if (!args[0]->IsArray()) {
			return NanThrowTypeError("Wrong arguments");
		}

		v8::Local<v8::Array> data_arr = v8::Local<v8::Array>::Cast(args[0]);
		int len = data_arr->Length();
		if (len == 0){
			return NanThrowTypeError("Array needs at least 1 item");
		}
		else if (len > MaxVars){
			std::stringstream err;
			err << "Array exceeds max variables (" << MaxVars << ") that can be transferred with ReadMultiVars()";
			return NanThrowTypeError(err.str().c_str());
		}

		for (int i = 0; i < len; i++) {
			if (!data_arr->Get(i)->IsObject()){
				return NanThrowTypeError("Wrong argument structure");
			}
			else{
				v8::Local<v8::Object> data_obj = data_arr->Get(i)->ToObject();
				if (!data_obj->Has(NanNew<v8::String>("Area")) || !data_obj->Has(NanNew<v8::String>("WordLen")) || !data_obj->Has(NanNew<v8::String>("Start")) || !data_obj->Has(NanNew<v8::String>("Amount"))){
					return NanThrowTypeError("Wrong argument structure");
				}
				else if (!data_obj->Get(NanNew<v8::String>("Area"))->IsNumber() || !data_obj->Get(NanNew<v8::String>("WordLen"))->IsNumber() || !data_obj->Get(NanNew<v8::String>("Start"))->IsNumber() || !data_obj->Get(NanNew<v8::String>("Amount"))->IsNumber()){
					return NanThrowTypeError("Wrong argument structure");
				}
				else if (data_obj->Get(NanNew<v8::String>("Area"))->Int32Value() == static_cast<int>(S7AreaDB)){
					if (!data_obj->Has(NanNew<v8::String>("DBNumber"))){
						return NanThrowTypeError("Wrong argument structure");
					}
				}
				else data_obj->Set(NanNew<v8::String>("DBNumber"), NanNew<v8::Integer>(0));
			}
		}
		if (!args[1]->IsFunction()){
			PS7DataItem Items = new TS7DataItem[len];
			v8::Local<v8::Object> data_obj;
			int byteCount;
			int size;
			for (int i = 0; i < len; i++){
				data_obj = data_arr->Get(i)->ToObject();

				Items[i].Area = data_obj->Get(NanNew<v8::String>("Area"))->Int32Value();
				Items[i].WordLen = data_obj->Get(NanNew<v8::String>("WordLen"))->Int32Value();
				Items[i].DBNumber = data_obj->Get(NanNew<v8::String>("DBNumber"))->Int32Value();
				Items[i].Start = data_obj->Get(NanNew<v8::String>("Start"))->Int32Value();
				Items[i].Amount = data_obj->Get(NanNew<v8::String>("Amount"))->Int32Value();

				byteCount = GetByteCountFromWordLen(Items[i].WordLen);
				if (byteCount == 0) NanReturnValue(NanFalse());
				size = Items[i].Amount * byteCount;
				Items[i].pdata = std::malloc(size);
			}

			int returnValue = s7client->snap7Client->ReadMultiVars(Items, len);

			if (returnValue == 0){
				v8::Handle<v8::Array> res_arr = NanNew<v8::Array>(len);
				v8::Local<v8::Object> res_obj;
				for (int i = 0; i < len; i++){
					res_obj = NanNew<v8::Object>();
					res_obj->Set(NanNew<v8::String>("Result"), NanNew<v8::Integer>(Items[i].Result));

					byteCount = GetByteCountFromWordLen(Items[i].WordLen);
					size = byteCount * Items[i].Amount;
					res_obj->Set(NanNew<v8::String>("Buffer"), NanNewBufferHandle(static_cast<char*>(Items[i].pdata), size));

					res_arr->Set(i, res_obj);
					std::free(Items[i].pdata);
				}
				delete[] Items;
				NanReturnValue(res_arr);
			}
			else{
				for (int i = 0; i < len; i++){
					std::free(Items[i].pdata);
				}
				delete[] Items;
				NanReturnValue(NanFalse());
			}
		}
		else{
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Client::WriteMultiVars){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (args.Length() < 1) {
			return NanThrowTypeError("Wrong number of arguments");
		}

		if (!args[0]->IsArray()) {
			return NanThrowTypeError("Wrong arguments");
		}

		v8::Local<v8::Array> data_arr = v8::Local<v8::Array>::Cast(args[0]);
		int len = data_arr->Length();
		if (len == 0){
			return NanThrowTypeError("Array needs at least 1 item");
		}
		else if (len > MaxVars){
			std::stringstream err;
			err << "Array exceeds max variables (" << MaxVars << ") that can be transferred with WriteMultiVars()";
			return NanThrowTypeError(err.str().c_str());
		}

		for (int i = 0; i < len; i++) {
			if (!data_arr->Get(i)->IsObject()){
				return NanThrowTypeError("Wrong argument structure");
			}
			else{
				v8::Local<v8::Object> data_obj = data_arr->Get(i)->ToObject();
				if (!data_obj->Has(NanNew<v8::String>("Area")) || !data_obj->Has(NanNew<v8::String>("WordLen")) || !data_obj->Has(NanNew<v8::String>("Start")) || !data_obj->Has(NanNew<v8::String>("Amount")) || !data_obj->Has(NanNew<v8::String>("Data"))){
					return NanThrowTypeError("Wrong argument structure");

				}
				else if (!data_obj->Get(NanNew<v8::String>("Area"))->IsNumber() || !data_obj->Get(NanNew<v8::String>("WordLen"))->IsNumber() || !data_obj->Get(NanNew<v8::String>("Start"))->IsNumber() || !data_obj->Get(NanNew<v8::String>("Amount"))->IsNumber() || !node::Buffer::HasInstance(data_obj->Get(NanNew<v8::String>("Data")))){
					return NanThrowTypeError("Wrong argument structure");
				}
				else if (data_obj->Get(NanNew<v8::String>("Area"))->Int32Value() == static_cast<int>(S7AreaDB)){
					if (!data_obj->Has(NanNew<v8::String>("DBNumber"))){
						return NanThrowTypeError("Wrong argument structure");
					}
				}
				else data_obj->Set(NanNew<v8::String>("DBNumber"), NanNew<v8::Integer>(0));
			}
		}
		if (!args[1]->IsFunction()){
			PS7DataItem Items = new TS7DataItem[len];
			v8::Local<v8::Object> data_obj;
			for (int i = 0; i < len; i++){
				data_obj = data_arr->Get(i)->ToObject();

				Items[i].Area = data_obj->Get(NanNew<v8::String>("Area"))->Int32Value();
				Items[i].WordLen = data_obj->Get(NanNew<v8::String>("WordLen"))->Int32Value();
				Items[i].DBNumber = data_obj->Get(NanNew<v8::String>("DBNumber"))->Int32Value();
				Items[i].Start = data_obj->Get(NanNew<v8::String>("Start"))->Int32Value();
				Items[i].Amount = data_obj->Get(NanNew<v8::String>("Amount"))->Int32Value();
				Items[i].pdata = node::Buffer::Data(data_obj->Get(NanNew<v8::String>("Data")).As<v8::Object>());
			}

			int returnValue = s7client->snap7Client->WriteMultiVars(Items, len);

			if (returnValue == 0){
				v8::Handle<v8::Array> res_arr = NanNew<v8::Array>(len);
				v8::Local<v8::Object> res_obj;
				for (int i = 0; i < len; i++){
					res_obj = NanNew<v8::Object>();
					res_obj->Set(NanNew<v8::String>("Result"), NanNew<v8::Integer>(Items[i].Result));
					res_arr->Set(i, res_obj);
				}
				delete[] Items;
				NanReturnValue(res_arr);
			}
			else{
				delete[] Items;
				NanReturnValue(NanFalse());
			}
		}
		else
		{
			NanReturnUndefined();
		}
	}

	// Directory functions
	NAN_METHOD(S7Client::ListBlocks){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsFunction()){
			PS7BlocksList BlockList = new TS7BlocksList;

			int returnValue = s7client->snap7Client->ListBlocks(BlockList);

			v8::Local<v8::Object> block_list = NanNew<v8::Object>();
			block_list->Set(NanNew<v8::String>("OBCount"), NanNew<v8::Number>(BlockList->OBCount));
			block_list->Set(NanNew<v8::String>("FBCount"), NanNew<v8::Number>(BlockList->FBCount));
			block_list->Set(NanNew<v8::String>("FCCount"), NanNew<v8::Number>(BlockList->FCCount));
			block_list->Set(NanNew<v8::String>("SFBCount"), NanNew<v8::Number>(BlockList->SFBCount));
			block_list->Set(NanNew<v8::String>("SFCCount"), NanNew<v8::Number>(BlockList->SFCCount));
			block_list->Set(NanNew<v8::String>("DBCount"), NanNew<v8::Number>(BlockList->DBCount));
			block_list->Set(NanNew<v8::String>("SDBCount"), NanNew<v8::Number>(BlockList->SDBCount));

			delete BlockList;
			if (returnValue == 0)
				NanReturnValue(block_list);
			else
				NanReturnValue(NanFalse());
		}
		else{
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Client::GetAgBlockInfo){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
			return NanThrowTypeError("Wrong arguments");
		}

		if (!args[2]->IsFunction()){
			PS7BlockInfo BlockInfo = new TS7BlockInfo;

			int returnValue = s7client->snap7Client->GetAgBlockInfo(args[0]->Uint32Value(), args[1]->Uint32Value(), BlockInfo);

			v8::Local<v8::Object> block_info = NanNew<v8::Object>();
			block_info->Set(NanNew<v8::String>("BlkType"), NanNew<v8::Number>(BlockInfo->BlkType));
			block_info->Set(NanNew<v8::String>("BlkNumber"), NanNew<v8::Number>(BlockInfo->BlkNumber));
			block_info->Set(NanNew<v8::String>("BlkLang"), NanNew<v8::Number>(BlockInfo->BlkLang));
			block_info->Set(NanNew<v8::String>("BlkFlags"), NanNew<v8::Number>(BlockInfo->BlkFlags));
			block_info->Set(NanNew<v8::String>("MC7Size"), NanNew<v8::Number>(BlockInfo->MC7Size));
			block_info->Set(NanNew<v8::String>("LoadSize"), NanNew<v8::Number>(BlockInfo->LoadSize));
			block_info->Set(NanNew<v8::String>("LocalData"), NanNew<v8::Number>(BlockInfo->LocalData));
			block_info->Set(NanNew<v8::String>("SBBLength"), NanNew<v8::Number>(BlockInfo->SBBLength));
			block_info->Set(NanNew<v8::String>("CheckSum"), NanNew<v8::Number>(BlockInfo->CheckSum));
			block_info->Set(NanNew<v8::String>("Version"), NanNew<v8::Number>(BlockInfo->Version));
			block_info->Set(NanNew<v8::String>("CodeDate"), NanNew<v8::String>(BlockInfo->CodeDate));
			block_info->Set(NanNew<v8::String>("IntfDate"), NanNew<v8::String>(BlockInfo->IntfDate));
			block_info->Set(NanNew<v8::String>("Author"), NanNew<v8::String>(BlockInfo->Author));
			block_info->Set(NanNew<v8::String>("Family"), NanNew<v8::String>(BlockInfo->Family));
			block_info->Set(NanNew<v8::String>("Header"), NanNew<v8::String>(BlockInfo->Header));

			delete BlockInfo;
			if (returnValue == 0)
				NanReturnValue(block_info);
			else
				NanReturnValue(NanFalse());
		}
		else{
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Client::GetPgBlockInfo){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!node::Buffer::HasInstance(args[0])) {
			return NanThrowTypeError("Argument should be a Buffer");
		}

		PS7BlockInfo BlockInfo = new TS7BlockInfo;

		int returnValue = s7client->snap7Client->GetPgBlockInfo(node::Buffer::Data(args[0].As<v8::Object>()), BlockInfo, node::Buffer::Length(args[0].As<v8::Object>()));

		v8::Local<v8::Object> block_info = NanNew<v8::Object>();
		block_info->Set(NanNew<v8::String>("BlkType"), NanNew<v8::Number>(BlockInfo->BlkType));
		block_info->Set(NanNew<v8::String>("BlkNumber"), NanNew<v8::Number>(BlockInfo->BlkNumber));
		block_info->Set(NanNew<v8::String>("BlkLang"), NanNew<v8::Number>(BlockInfo->BlkLang));
		block_info->Set(NanNew<v8::String>("BlkFlags"), NanNew<v8::Number>(BlockInfo->BlkFlags));
		block_info->Set(NanNew<v8::String>("MC7Size"), NanNew<v8::Number>(BlockInfo->MC7Size));
		block_info->Set(NanNew<v8::String>("LoadSize"), NanNew<v8::Number>(BlockInfo->LoadSize));
		block_info->Set(NanNew<v8::String>("LocalData"), NanNew<v8::Number>(BlockInfo->LocalData));
		block_info->Set(NanNew<v8::String>("SBBLength"), NanNew<v8::Number>(BlockInfo->SBBLength));
		block_info->Set(NanNew<v8::String>("CheckSum"), NanNew<v8::Number>(BlockInfo->CheckSum));
		block_info->Set(NanNew<v8::String>("Version"), NanNew<v8::Number>(BlockInfo->Version));
		block_info->Set(NanNew<v8::String>("CodeDate"), NanNew<v8::String>(BlockInfo->CodeDate));
		block_info->Set(NanNew<v8::String>("IntfDate"), NanNew<v8::String>(BlockInfo->IntfDate));
		block_info->Set(NanNew<v8::String>("Author"), NanNew<v8::String>(BlockInfo->Author));
		block_info->Set(NanNew<v8::String>("Family"), NanNew<v8::String>(BlockInfo->Family));
		block_info->Set(NanNew<v8::String>("Header"), NanNew<v8::String>(BlockInfo->Header));

		delete BlockInfo;

		if (returnValue == 0)
			NanReturnValue(block_info);
		else
			NanReturnValue(NanFalse());
	}

	NAN_METHOD(S7Client::ListBlocksOfType){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsNumber()) {
			return NanThrowTypeError("Wrong arguments");
		}

		if (!args[1]->IsFunction()){
			TS7BlocksOfType BlockList;
			int BlockNum = sizeof(BlockList) / sizeof(*BlockList);

			int returnValue = s7client->snap7Client->ListBlocksOfType(args[0]->Uint32Value(), &BlockList, &BlockNum);

			v8::Local<v8::Array> block_list = NanNew<v8::Array>(BlockNum);
			for (int i = 0; i < BlockNum; i++) {
				block_list->Set(i, NanNew<v8::Integer>(BlockList[i]));
			}

			if (returnValue == 0)
				NanReturnValue(block_list);
			else
				NanReturnValue(NanFalse());
		}
		else{
			NanReturnUndefined();
		}
	}

	// Blocks functions
	NAN_METHOD(S7Client::Upload){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
			return NanThrowTypeError("Wrong arguments");
		}


		if (!args[3]->IsFunction()){
			char* bufferData = new char[args[2]->Int32Value()];
			int size = args[2]->Int32Value();
			int returnValue = s7client->snap7Client->Upload(args[0]->Int32Value(), args[1]->Int32Value(), bufferData, &size);

			if (returnValue == 0){
				v8::Handle<v8::Value> ret_buf = NanNewBufferHandle(bufferData, size);
				delete[] bufferData;
				NanReturnValue(ret_buf);
			}
			else
				NanReturnValue(NanFalse());
		}
		else{
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Client::FullUpload){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
			return NanThrowTypeError("Wrong arguments");
		}


		if (!args[3]->IsFunction()){
			char* bufferData = new char[args[2]->Int32Value()];
			int size = args[2]->Int32Value();
			int returnValue = s7client->snap7Client->FullUpload(args[0]->Int32Value(), args[1]->Int32Value(), bufferData, &size);

			if (returnValue == 0){
				v8::Handle<v8::Value> ret_buf = NanNewBufferHandle(bufferData, size);
				delete[] bufferData;
				NanReturnValue(ret_buf);
			}
			else
				NanReturnValue(NanFalse());
		}
		else{
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Client::Download){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsNumber() || !node::Buffer::HasInstance(args[1])) {
			return NanThrowTypeError("Wrong arguments");
		}


		if (!args[2]->IsFunction()){
			NanReturnValue(NanNew<v8::Boolean>(s7client->snap7Client->Download(args[0]->Int32Value(), node::Buffer::Data(args[1].As<v8::Object>()), node::Buffer::Length(args[1].As<v8::Object>())) == 0));
		}
		else{
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Client::Delete){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
			return NanThrowTypeError("Wrong arguments");
		}

		if (!args[2]->IsFunction()){
			NanReturnValue(NanNew<v8::Boolean>(s7client->snap7Client->Delete(args[0]->Int32Value(), args[1]->Int32Value()) == 0));
		}
		else{
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Client::DBGet){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
			return NanThrowTypeError("Wrong arguments");
		}


		if (!args[2]->IsFunction()){
			char* bufferData = new char[args[1]->Int32Value()];
			int size = args[1]->Int32Value();
			int returnValue = s7client->snap7Client->DBGet(args[0]->Int32Value(), bufferData, &size);

			if (returnValue == 0){
				v8::Handle<v8::Value> ret_buf = NanNewBufferHandle(bufferData, size);
				delete[] bufferData;
				NanReturnValue(ret_buf);
			}
			else
				NanReturnValue(NanFalse());
		}
		else{
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Client::DBFill){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsNumber() || !(args[1]->IsNumber() || args[1]->IsString())) {
			return NanThrowTypeError("Wrong arguments");
		}

		if (!args[2]->IsFunction()){
			int fill;
			if (args[1]->IsNumber())
				fill = args[1]->Int32Value();
			else{
				size_t len;
				char* fillstr = NanCString(args[1], &len);
				fill = static_cast<int>(fillstr[0]);
				delete[] fillstr;
			}
			NanReturnValue(NanNew<v8::Boolean>(s7client->snap7Client->DBFill(args[0]->Int32Value(), fill) == 0));
		}
		else{
			NanReturnUndefined();
		}
	}

	// Date/Time functions
	NAN_METHOD(S7Client::GetPlcDateTime){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsFunction()){
			tm DateTime;
			int returnValue = s7client->snap7Client->GetPlcDateTime(&DateTime);
			double timestamp = static_cast<double>(mktime(&DateTime));

			if (returnValue == 0)
				NanReturnValue(NanNew<v8::Date>(timestamp * 1000));
			else
				NanReturnValue(NanFalse());
		}
		else{
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Client::SetPlcDateTime){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!((args[0]->IsObject() && args[0]->IsDate()) || args[0]->IsFunction())) {
			return NanThrowTypeError("Wrong arguments");
		}

		if (!args[0]->IsFunction()){
			if (args[0]->IsDate()){
				v8::Local<v8::Date> date = v8::Local<v8::Date>::Cast(args[0]->ToObject());
				time_t timestamp = static_cast<time_t>(date->NumberValue() / 1000);
				tm* DateTime = localtime(&timestamp);

				NanReturnValue(NanNew<v8::Boolean>(s7client->snap7Client->SetPlcDateTime(DateTime) == 0));
			}
			else{
				tm DateTime;
				v8::Local<v8::Object> date_time = args[0]->ToObject();
				DateTime.tm_year = date_time->Get(NanNew<v8::String>("year"))->Int32Value() - 1900;
				DateTime.tm_mon = date_time->Get(NanNew<v8::String>("month"))->Int32Value();
				DateTime.tm_mday = date_time->Get(NanNew<v8::String>("day"))->Int32Value();
				DateTime.tm_hour = date_time->Get(NanNew<v8::String>("hour"))->Int32Value();
				DateTime.tm_min = date_time->Get(NanNew<v8::String>("minute"))->Int32Value();
				DateTime.tm_sec = date_time->Get(NanNew<v8::String>("second"))->Int32Value();

				NanReturnValue(NanNew<v8::Boolean>(s7client->snap7Client->SetPlcDateTime(&DateTime) == 0));
			}
		}
		else{
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Client::SetPlcSystemDateTime){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		NanReturnValue(NanNew<v8::Boolean>(s7client->snap7Client->SetPlcSystemDateTime() == 0));
	}

	// System Info functions
	NAN_METHOD(S7Client::GetOrderCode){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsFunction()){
			PS7OrderCode OrderCode = new TS7OrderCode;

			int returnValue = s7client->snap7Client->GetOrderCode(OrderCode);

			v8::Local<v8::Object> order_code = NanNew<v8::Object>();
			order_code->Set(NanNew<v8::String>("Code"), NanNew<v8::String>(OrderCode->Code));
			order_code->Set(NanNew<v8::String>("V1"), NanNew<v8::Number>(OrderCode->V1));
			order_code->Set(NanNew<v8::String>("V2"), NanNew<v8::Number>(OrderCode->V2));
			order_code->Set(NanNew<v8::String>("V3"), NanNew<v8::Number>(OrderCode->V3));

			delete OrderCode;

			if (returnValue == 0)
				NanReturnValue(order_code);
			else
				NanReturnValue(NanFalse());
		}
		else{
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Client::GetCpuInfo){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsFunction()){
			PS7CpuInfo CpuInfo = new TS7CpuInfo;

			int returnValue = s7client->snap7Client->GetCpuInfo(CpuInfo);

			v8::Local<v8::Object> cpu_info = NanNew<v8::Object>();
			cpu_info->Set(NanNew<v8::String>("ModuleTypeName"), NanNew<v8::String>(CpuInfo->ModuleTypeName));
			cpu_info->Set(NanNew<v8::String>("SerialNumber"), NanNew<v8::String>(CpuInfo->SerialNumber));
			cpu_info->Set(NanNew<v8::String>("ASName"), NanNew<v8::String>(CpuInfo->ASName));
			cpu_info->Set(NanNew<v8::String>("Copyright"), NanNew<v8::String>(CpuInfo->Copyright));
			cpu_info->Set(NanNew<v8::String>("ModuleName"), NanNew<v8::String>(CpuInfo->ModuleName));

			delete CpuInfo;

			if (returnValue == 0)
				NanReturnValue(cpu_info);
			else
				NanReturnValue(NanFalse());
		}
		else{
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Client::GetCpInfo){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsFunction()){
			PS7CpInfo CpInfo = new TS7CpInfo;

			int returnValue = s7client->snap7Client->GetCpInfo(CpInfo);

			v8::Local<v8::Object> cp_info = NanNew<v8::Object>();
			cp_info->Set(NanNew<v8::String>("MaxPduLengt"), NanNew<v8::Number>(CpInfo->MaxPduLengt));
			cp_info->Set(NanNew<v8::String>("MaxConnections"), NanNew<v8::Number>(CpInfo->MaxConnections));
			cp_info->Set(NanNew<v8::String>("MaxMpiRate"), NanNew<v8::Number>(CpInfo->MaxMpiRate));
			cp_info->Set(NanNew<v8::String>("MaxBusRate"), NanNew<v8::Number>(CpInfo->MaxBusRate));

			delete CpInfo;

			if (returnValue == 0)
				NanReturnValue(cp_info);
			else
				NanReturnValue(NanFalse());
		}
		else{
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Client::ReadSZL){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		NanReturnUndefined();
	}

	NAN_METHOD(S7Client::ReadSZLList){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		NanReturnUndefined();
	}

	// Control functions
	NAN_METHOD(S7Client::PlcHotStart){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		NanReturnValue(NanNew<v8::Boolean>(s7client->snap7Client->PlcHotStart() == 0));
	}

	NAN_METHOD(S7Client::PlcColdStart){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		NanReturnValue(NanNew<v8::Boolean>(s7client->snap7Client->PlcColdStart() == 0));
	}

	NAN_METHOD(S7Client::PlcStop){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		NanReturnValue(NanNew<v8::Boolean>(s7client->snap7Client->PlcStop() == 0));
	}

	NAN_METHOD(S7Client::CopyRamToRom){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsNumber()) {
			return NanThrowTypeError("Wrong arguments");
		}

		NanReturnValue(NanNew<v8::Boolean>(s7client->snap7Client->CopyRamToRom(args[0]->Int32Value()) == 0));
	}

	NAN_METHOD(S7Client::Compress){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsNumber()) {
			return NanThrowTypeError("Wrong arguments");
		}

		NanReturnValue(NanNew<v8::Boolean>(s7client->snap7Client->Compress(args[0]->Int32Value()) == 0));
	}

	// Security functions
	NAN_METHOD(S7Client::GetProtection){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!args[0]->IsFunction()){
			PS7Protection S7Protection = new TS7Protection;

			int returnValue = s7client->snap7Client->GetProtection(S7Protection);

			v8::Local<v8::Object> protection = NanNew<v8::Object>();
			protection->Set(NanNew<v8::String>("sch_schal"), NanNew<v8::Number>(S7Protection->sch_schal));
			protection->Set(NanNew<v8::String>("sch_par"), NanNew<v8::Number>(S7Protection->sch_par));
			protection->Set(NanNew<v8::String>("sch_rel"), NanNew<v8::Number>(S7Protection->sch_rel));
			protection->Set(NanNew<v8::String>("bart_sch"), NanNew<v8::Number>(S7Protection->bart_sch));
			protection->Set(NanNew<v8::String>("anl_sch"), NanNew<v8::Number>(S7Protection->anl_sch));

			delete S7Protection;

			if (returnValue == 0)
				NanReturnValue(protection);
			else
				NanReturnValue(NanFalse());
		}
		else{
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Client::SetSessionPassword){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		if (!(args[0]->IsString() || args[0]->IsFunction())) {
			return NanThrowTypeError("Wrong arguments");
		}

		if (!args[0]->IsFunction()){
			size_t len;
			char* password = NanCString(args[0], &len);

			v8::Local<v8::Boolean> ret = NanNew<v8::Boolean>(s7client->snap7Client->SetSessionPassword(password) == 0);
			delete[] password;
			NanReturnValue(ret);
		}
		else{
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Client::ClearSessionPassword){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		NanReturnValue(NanNew<v8::Boolean>(s7client->snap7Client->ClearSessionPassword() == 0));
	}

	// Properties
	NAN_METHOD(S7Client::ExecTime){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		NanReturnValue(NanNew<v8::Integer>(s7client->snap7Client->ExecTime()));
	}

	NAN_METHOD(S7Client::LastError){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		NanReturnValue(NanNew<v8::Integer>(s7client->snap7Client->LastError()));
	}

	NAN_METHOD(S7Client::PDURequested){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		NanReturnValue(NanNew<v8::Integer>(s7client->snap7Client->PDURequested()));
	}

	NAN_METHOD(S7Client::PDULength){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		NanReturnValue(NanNew<v8::Integer>(s7client->snap7Client->PDULength()));
	}

	NAN_METHOD(S7Client::PlcStatus){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		NanReturnValue(NanNew<v8::Integer>(s7client->snap7Client->PlcStatus()));
	}

	NAN_METHOD(S7Client::Connected){
		NanScope();
		S7Client *s7client = ObjectWrap::Unwrap<S7Client>(args.This());

		NanReturnValue(NanNew<v8::Boolean>(s7client->snap7Client->Connected()));
	}

	NAN_METHOD(S7Client::ErrorText) {
		NanScope();

		if (!args[0]->IsNumber()) {
			return NanThrowTypeError("Wrong arguments");
		}

		NanReturnValue(NanNew<v8::String>(CliErrorText(args[0]->Int32Value()).c_str()));
	}
}