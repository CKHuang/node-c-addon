#include <nan.h>

using namespace v8;

NAN_METHOD(Method) {
	NanScope();
	NanReturnValue(String::New("hello world"));
}

void Init(Handle<Object> exports) {
	exports->Set(NanSymbol("sayHello"),FunctionTemplate::New(Method)->GetFunction());
}

NODE_MODULE(hello, Init);