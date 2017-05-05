#include <node.h>
#include <v8.h>
#include <iostream>

using namespace v8;
using namespace std;

void GetArgument( const FunctionCallbackInfo<Value>& args ) 
{
	Isolate* isolate = args.GetIsolate();

	// 参数的长度
	if (args.Length() < 0)
	{
		isolate->ThrowException(Exception::Error(
			String::NewFromUtf8(isolate,"arguments number must be two")
		));
		return ;
	}

	// 参数类型判断
	if (!args[0]->IsString() || !args[1]->IsString())
	{
		isolate->ThrowException(Exception::Error(
			String::NewFromUtf8(isolate,"arguments type must be string")
		));
		return ;
	}

	// js string 转换为 v8 String 类型
	Local<String> key = Local<String>::Cast(args[0]);
	Local<String> value = Local<String>::Cast(args[1]);

	String::Utf8Value utfKey(key);
	String::Utf8Value utfValue(value);

	// 获取js string的长度
	int length = utfValue.length();

	// 字符串串相加、截取

	// 使用标准输出将内容打印到控制台里面
	cout<<length<<endl;
	cout<<string(*utfValue)<<endl;


	// 数组处理
	Local<Array> input_array = Local<Array>::Cast(args[2]);
	String::Utf8Value utfArraySet( Local<String>::Cast(input_array->Get(1)) );

	printf("%d, %f %s\n", input_array->Length(),input_array->Get(0)->NumberValue(), *utfArraySet);

	// object 类型
	Local<Object> obj = Local<Object>::Cast(args[3]);

	// 获取v8 Object对象的值
	Local<Value> obj_a = obj->Get(String::NewFromUtf8(isolate,"a"));
	Local<Value> obj_b = obj->Get(String::NewFromUtf8(isolate,"b"));

	printf("%f %f\n", obj_a->NumberValue(), obj_b->NumberValue());

	// js boolean 类型
	Local<Boolean> flagTrue = Local<Boolean>::Cast(args[4]);
	cout<<"Flag: "<<flagTrue->BooleanValue()<<endl;

	// js function 类型
	Local<Function> cb = Local<Function>::Cast(args[5]);
	const unsigned argc = 2;
	Local<Value> argv[2];
	argv[0] = String::NewFromUtf8(isolate,"aaa"); // 不能使用单引号
	argv[1] = String::NewFromUtf8(isolate,"bbb");
	argv[2] = args[1];
	cb->Call(Null(isolate), argc, argv);


	//args.GetReturnValue().Set()
}

void init( Local<Object> exports)
{
	NODE_SET_METHOD(exports, "getArguments", GetArgument);
}

NODE_MODULE(arguments,init);