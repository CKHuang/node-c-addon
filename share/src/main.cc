#include <node.h>
#include <iostream>
using namespace v8;
using namespace std;

namespace share {

	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;

	void add(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();

		Local<Value> argData = args.Data();

		if ( argData->IsArgumentsObject() )
		{
			
		}

		args.GetReturnValue().Set(String::NewFromUtf8(isolate,"type array"));
		return ;


		// 获取参数个数
		double len = args.Length();
		Local<Number> cout = Number::New(isolate,len);
		args.GetReturnValue().Set(cout);
		return ;


		// 数据参数相加
		if (args.Length() < 2)
		{
			/**
			 * 抛出错误,Exception::TypeError类型错误
			 */
			isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate,"Wrong arguments length")));
			return ;
		}

		//检测数据类型
		if (!args[0]->IsNumber() || !args[1]->IsNumber())
		{
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate,"Wrong argumenst type")
			));
			return ;
		}

		double value = args[0]->NumberValue() + args[1]->NumberValue();
		Local<Number> num = Number::New(isolate, value);
		
		args.GetReturnValue().Set(num);
	}


	/**
	 * 回调函数返回数据
	 */
	void RunCallback(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		Local<Function> callback = Local<Function>::Cast(args[0]);
		const unsigned argc = 1;
		Local<Value> argv[argc] = { String::NewFromUtf8(isolate, "hello world") };
		callback->Call(Null(isolate), argc, argv);
	}


	void init(Local<Object> exports) {
		/**
		 * NODE_SET_METHOD
		 * 设置变异后抛出来的方法，会被封装到对象里面提供js去调用
		 * @params {Local<Object>} exports 
		 * @params {String} name
		 * @params {Function} function
		 */
		NODE_SET_METHOD(exports, "add", add);
		NODE_SET_METHOD(exports, "exports", RunCallback);
	}

	/**
	 * NODE_MODULE
	 * 转化成对外抛出的Node.js模块
	 * @params {?} 命名空间，随意
	 * @params {Function} function
	 */
	NODE_MODULE(share, init);
}