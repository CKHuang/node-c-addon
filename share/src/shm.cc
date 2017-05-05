#include <node.h>
#include <sys/shm.h>
using namespace v8;

namespace shm 
{
	
	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::String;

	const char* ToCString( const String::Utf8Value& value ) 
	{
		return *value ? *value : "<string conversion fail>";
	}

	/**
	 * 写共享内存
	 * @param args [description]
	 * @use
	 * 		shm.write('key','value')
	 */
	void write ( const FunctionCallbackInfo<Value>& args ) 
	{
		Isolate* isolate = args.GetIsolate();

		
	}

	void init( Local<Object> exports ) 
	{
		NODE_SET_METHOD(exports, "write", write);
	}

	NODE_MODULE(shm, init);
}