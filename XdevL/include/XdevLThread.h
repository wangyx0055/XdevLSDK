/*
	Copyright (c) 2005 - 2016 Cengiz Terzibas

	Permission is hereby granted, free of charge, to any person obtaining a copy of 
	this software and associated documentation files (the "Software"), to deal in the 
	Software without restriction, including without limitation the rights to use, copy, 
	modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
	and to permit persons to whom the Software is furnished to do so, subject to the 
	following conditions:

	The above copyright notice and this permission notice shall be included in all copies 
	or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
	DEALINGS IN THE SOFTWARE.

	cengiz@terzibas.de
*/

#ifndef THREAD_H_
#define THREAD_H_

#include <XdevLPlatform.h>

#if defined (XDEVL_PLATFORM_UNIX)  || (defined XDEVL_PLATFORM_MINGW)
	typedef pthread_t THREAD_THREAD;
	#define THREAD_INIT(THREAD) THREAD(0)
	const int THREAD_INVALID = 0;
#elif XDEVL_PLATFORM_WINDOWS
	typedef HANDLE THREAD_THREAD;
	#define THREAD_INIT(THREAD) THREAD(NULL)
	const HANDLE THREAD_INVALID = NULL;

#else
	#error "No implementation for this platform."
#endif

namespace thread {

	class Mutex;

	typedef int (*thread_callback_function)();

	/**
		@class ThreadArgument
		@brief Arguments for threads
	*/
	class ThreadArgument {
		public:
			ThreadArgument() {
			}
			virtual ~ThreadArgument() {
			}
	};


	/**
		@class Thread
		@brief Represents a thread.

		Do you need threading? Hmm, actually noooo you don't :D.

		@section thread_usage_no_argument Thread without passing an argument
		Here a small example how we print a message on the console every one second:

		@code
		class MyThread : public thread::Thread{
			virtual xdl_int RunThread(ThreadArgument* p_arg);
		};

		xdl_int MyThread::RunThread(ThreadArgument* p_arg){
			while(true){
				std::cout << "Hello, Thread World!\n";
				xdl::sleep(1.0);
			}
			return 0;
		}
		@endcode

		As you can see we create a class called MyThread which is derived from
		the thread::Thread class. Override the RunThread function to do your
		threading stuff. In our example printing the "Hello, World!" message.
		Your main function would look like this:

		@code
		xdl_int main(xdl_int argc, xdl_char* argv[]){
			MyThread myThread;
			myThread.Start();
			while(true){
				// do here your stuff while your thread is doing it in the background.
				readFile(...);
				doSomethingElse(...);
				doBoringStuff(...);
			}
			return 0;
		}
		@endcode

		Now what is happening here. You can see that we are creating an instance of our MyThread class
		in the main function. After that we call the Thread::Start() function. This starts the tread.
		After that the main while loop start where you doing different boring things. You will see that
		during the execution of the function in the while loop the "Hello, Thread World!" message will
		be printed on the console. This is our thread which is running in the background.

		@section thread_usage Thread with passing an argument

		It is possible to pass an argument to the thread. This gives you more control over the thread
		and your environment. To pass an argument think what you wanna pass to the thread, make a struct
		or class and derive that from the thread::ThreadArgument interface. If you use the Thread::Start()
		method you cann pass your argument class as an argument to the function. In the main RunThread
		function you can use the passed object.
		@code
			struct MyArgument : public thread::ThreadArgument{
				xdl::xdl_int   val;
				xdl::xdl_float val2;
			};

			...

			MyArgument arg;
			...
			myThread.Start(&arg);
		@endcode
	*/
	class Thread {
		public:
			Thread();

			Thread(thread_callback_function func);

			virtual ~Thread();

			/// Starts the thread.
			int Start(ThreadArgument* arg = NULL);

			/// Returns the thread argument.
			ThreadArgument* Arguments();

			/// Sets the thread argument.
			void Arguments(ThreadArgument* p_arg);

			/// Stops user till the process is finished.
			int Join();

			/// Terminate thread.
			void Exit(int exitCode = 0);

		protected:

			/// Starts the thread using the functions pointer provided by the user.
			int StartUsingFuncPointer();

			/// Override this function for your purpose.
			virtual int RunThread(ThreadArgument* arg);

#if defined (XDEVL_PLATFORM_UNIX)  || (defined XDEVL_PLATFORM_MINGW)
			/// Don't touch this function.
			static void* ThreadProc(void* p_this);
			static void* ThreadProc2(void* p_this);
#else
			/// Don't touch this function
			static unsigned long __stdcall ThreadProc(void* p_this);
			static unsigned long __stdcall ThreadProc2(void* p_this);
			DWORD 		m_threadId;

#endif
			// Holds the thread object.
			THREAD_THREAD m_thread;

			// Holds the thread argument.
			ThreadArgument* m_arg;

			// Holds the callback function.
			thread_callback_function callbackFunction;
	};

}

#endif /* THREAD_H_ */
