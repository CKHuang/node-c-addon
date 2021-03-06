#define _SVID_SOURCE
#include <node.h>
#include <errno.h>
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace v8;
using namespace std;

namespace shm 
{
	
	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::String;

	/**
	 * 写共享内存
	 * @param args [description]
	 * @use
	 * 		shm.write('key','value')
	 */
	void write ( const FunctionCallbackInfo<Value>& args ) 
	{
		Isolate* isolate = args.GetIsolate();

		key_t key= 10459;

		/**
		 * 申请一片共享内存，
		 * http://github.tiankonguse.com/blog/2015/09/03/shm-study.html
		 */
		int shmid = shmget(key,1024, 0777 | IPC_CREAT);

		/*
		 * 小于0都表示获取的共享内存失败
		 */
		if ( shmid < 0 )
		{
			printf("shmget error , %d\n", errno);

			isolate->ThrowException(Exception::Error(
				String::NewFromUtf8(isolate,"shmget error!")
			));
			return ;
		}

		//printf("shmid : %d\n", shmid);


		/**
		 * 通过shmget方法获取的共享内存海不能直接使用，
		 * 一个进程只能操作自己范围的内存地址，刚申请
		 * 的共享内存地址需要通过映射到对应的进程才能
		 * 为该进程来使用
		 *
		 * void *shmat(int shmid, const void *shmaddr, int shmflg) 这个操作同样需要三个参数.
			shmid : 我们申请内存时, 返回的shmid
			shmaddr 定共享内存出现在进程内存地址的什么位置，直接指定为NULL让内核自己决定一个合适的地址位置.
			shmflg flag 参数. 比如 SHM_RDONLY 代表只读.
			返回值 映射后进程内的地址指针, 代表内存的头部地址
		 */
		char* shmaddr = (char*) shmat(shmid,NULL,0);

		if ((char *) -1 == shmaddr)
		{	
			/*
			 * 调试运行的时候会报errno 13的错误
			 * 详见：http://stackoverflow.com/questions/19463957/shmat-returns-segmentation-falut-with-errno-13eacces
			 */
			printf("shmat error info, %d\n",errno);

			isolate->ThrowException(Exception::Error(
				String::NewFromUtf8(isolate,"shmat error!")
			));
			return ;
		}

		
		/*
		 * 将v8::String内存转为char
		 */
		Local<String> value = Local<String>::Cast(args[0]);
		String::Utf8Value utfValue(value);

		char* str = (char*) *utfValue;
		strncpy(shmaddr, str, 1024);


		/**
		 * 断开连接
		 */
		int ret = shmdt(shmaddr);

		if ( ret != 0 )
		{
			printf("shmdt error info, %d\n",ret);

			isolate->ThrowException(Exception::Error(
				String::NewFromUtf8(isolate,"shmdt error!")
			));
			return ;
		}

		args.GetReturnValue().Set(String::NewFromUtf8(isolate,"success"));
	}

	/**
	 * 删除某个对应的共享内存块
	 * @param args [description]
	 */
	void delt ( const FunctionCallbackInfo<Value>& args ) 
	{
		Isolate* isolate = args.GetIsolate();

		key_t key= 10459;

		int shmid = shmget(key,1024, 0777 | IPC_CREAT);

		if ( shmid < 0 )
		{
			printf("shmget error , %d\n", errno);

			isolate->ThrowException(Exception::Error(
				String::NewFromUtf8(isolate,"shmget error!")
			));
			return ;
		}

		int ret = shmctl(shmid, IPC_RMID, 0);

		printf("shmctl delete %d\n", ret);
	}


	void read ( const FunctionCallbackInfo<Value>& args ) 
	{
		Isolate* isolate = args.GetIsolate();

		key_t key= 10459;

		int shmid = shmget(key,1024, 0777 | IPC_CREAT);

		if ( shmid < 0 )
		{
			printf("shmget error , %d\n", errno);

			isolate->ThrowException(Exception::Error(
				String::NewFromUtf8(isolate,"shmget error!")
			));
			return ;
		}

		//printf("shmid : %d\n", shmid);

		char* shmaddr = (char*) shmat(shmid,NULL,0);

		if ((char *) -1 == shmaddr)
		{	
			/*
			 * 调试运行的时候会报errno 13的错误
			 * 详见：http://stackoverflow.com/questions/19463957/shmat-returns-segmentation-falut-with-errno-13eacces
			 */
			printf("shmat error info, %d\n",errno);

			isolate->ThrowException(Exception::Error(
				String::NewFromUtf8(isolate,"shmat error!")
			));
			return ;
		}

		args.GetReturnValue().Set(String::NewFromUtf8(isolate,shmaddr));

		/**
		 * 断开连接
		 */
		int ret = shmdt(shmaddr);

		if ( ret != 0 )
		{
			printf("shmdt error info, %d\n",ret);

			isolate->ThrowException(Exception::Error(
				String::NewFromUtf8(isolate,"shmdt error!")
			));
			return ;
		}


		//printf("get shm data %s\n", shmaddr);
	}

	void init( Local<Object> exports ) 
	{
		NODE_SET_METHOD(exports, "write", write);
		NODE_SET_METHOD(exports, "read", read);
		NODE_SET_METHOD(exports, "delt", delt);
	}

	NODE_MODULE(shm, init);
}