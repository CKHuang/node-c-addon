#include <node.h>
using namespace v8;

namespace share {

	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;


	void init(Local<Object> exports) {
		/**
		 * NODE_SET_METHOD
		 * 设置变异后抛出来的方法，会被封装到对象里面提供js去调用
		 * @params {Local<Object>} exports 
		 * @params {String} name
		 * @params {Function} function
		 */
		NODE_SET_METHOD(exports, "sayHello", sayHello);
	}

	/**
	 * NODE_MODULE
	 * 转化成对外抛出的Node.js模块
	 * @params {?} 命名空间，随意
	 * @params {Function} function
	 */
	NODE_MODULE(share, init);
}