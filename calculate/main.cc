#include <node.h>

using namespace v8;

void sum( const FunctionCallbackInfo<Value>& args )
{
	Isolate* isolate = args.GetIsolate();

	float a = args[0]->NumberValue();
	float b = args[1]->NumberValue();

	float c = a + b;
	Local<Value> ret = Number::New(isolate,c);

	args.GetReturnValue().Set(ret);

}

void init( Local<Object> exports ) 
{
	NODE_SET_METHOD(exports,"sum",sum);
}

NODE_MODULE(sum,init);