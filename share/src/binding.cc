#include <node.h>

#include "tc_shm.h"
#include "ProcessLock.h"
#include "ProxyFactory.h"

PROCESS_LOCK::CProcessLock m_stLock;
bool m_bSetValue;


namespace shmbuffer 
{

	using v8::Isolate;
	using v8::FunctionCallbackInfo;
	using v8::Buffer;
	using v8::String;

	void write( const FunctionCallbackInfo<Value>& args ) 
	{

		// 1 key length [0 - 3]
		uint16_t* ptr = (uint16_t*)m_stShm.getPointer();
		*ptr = args[0]->ToString().Length();
		ptr += sizeof(uint16_t);
		// 2 key value
		memcpy(ptr, args[0]->ToString().c_str(), args[0]->ToString().Length());
		ptr += args[0]->ToString().Length();

		// 3 value length
		*(uint32_t *)ptr = args[1]->ToString().Length();
		ptr += sizeof(uint32_t);
		// 4 value value
		memcpy(ptr, args[1]->ToString().c_str(), args[1]->ToString().Length());

		m_stLock.Lock();
		pShmData->iLength = args[0]->ToString().Length();
		memcpy(pShmData->szData, args[0]->ToString(), args[0]->ToString().Length());
		m_bSetValue = true;
		m_stLock.UnLock();

		Local<Number> result = Number::New(isolate,0);
		args.GetReturnValue().Set(result);
	}


	void read( const FunctionCallbackInfo<Value>& args)
	{
		if (!m_bSetValue)
		{
			isloate->ThrowException(
				Exception::Error(String::NewFromUtf8(isloate,"writing this mem"))
			);
			return ;
		}
		ShmData *pShmData = (ShmData *)m_stShm.getPointer();
		if (pShmData->iLength <= 0 )
		{
			isloate->ThrowException(
				Exception::Error(String::NewFromUtf8(isloate,"length less then zero"))
			);
			return ;
		}
		string sValue = string(pShmData->szData, pShmData->iLength);

		Local<String> result = String::NewFromUtf8(isloate,sValue);
		args.GetReturnValue().Set(result);
	}


	void init( Local<Object> exports ) {

		NODE_SET_METHOD(exports,"write",write);
		NODE_SET_METHOD(exports,"read",read);
	}

	NODE_MODULE(shmbuffer,init);
}