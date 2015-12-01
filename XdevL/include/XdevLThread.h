/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2015 Cengiz Terzibas

	This library is free software; you can redistribute it and/or modify it under the
	terms of the GNU Lesser General Public License as published by the Free Software
	Foundation; either version 2.1 of the License, or (at your option) any later version.
	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License along with
	this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
	Suite 330, Boston, MA 02111-1307 USA

	I would appreciate if you report all bugs to: cengiz@terzibas.de
*/

#ifndef THREAD_H_
#define THREAD_H_

#include <XdevLPlatform.h>

#ifdef XDEVL_PLATFORM_UNIX
	typedef pthread_t THREAD_THREAD;
	#define THREAD_INIT(THREAD) THREAD(0)
	const int THREAD_INVALID = 0;
#elif defined (XDEVL_PLATFORM_WINDOWS) || defined(XDEVL_PLATFORM_MINGW)
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

#ifdef _WIN32
			/// Don't touch this function
			static unsigned long __stdcall ThreadProc(void* p_this);
			static unsigned long __stdcall ThreadProc2(void* p_this);
			DWORD 		m_threadId;
#else
			/// Don't touch this function.
			static void* ThreadProc(void* p_this);
			static void* ThreadProc2(void* p_this);
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
