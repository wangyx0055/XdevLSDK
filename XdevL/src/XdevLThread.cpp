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

#include <iostream>
#include <XdevLThread.h>
#include <XdevLMutex.h>
#include <cassert>

namespace thread {

	Thread::Thread() :
		THREAD_INIT(m_thread),
		m_arg(NULL) {
	}

	Thread::Thread(thread_callback_function func) {
		assert(func && "Thread::Thread: Invalid callback function");
		callbackFunction = func;
		StartUsingFuncPointer();
	}

	Thread::~Thread() {
#if XDEVL_PLATFORM_UNIX || XDEVL_PLATFORM_MINGW
#else
		if(m_thread) {
			if(CloseHandle(m_thread) == 0) {
				std::cerr << "Thread::Can't close thread handle.\n";
			} else
				m_thread = NULL;
		}
#endif
	}

	int Thread::Start(ThreadArgument* arg) {
		Arguments(arg);
#if XDEVL_PLATFORM_UNIX || XDEVL_PLATFORM_MINGW
		if(pthread_create(&m_thread, NULL, Thread::ThreadProc, this) != 0) {
			std::cerr << "Thread::Could not create thread." << std::endl;
			return 1;
		}
#else
		m_thread = CreateThread(0, 0, Thread::ThreadProc, this, 0, &m_threadId);
		if(m_thread == NULL) {
			return 1;
		}
#endif
		return 0;
	}

int Thread::StartUsingFuncPointer() {
#if XDEVL_PLATFORM_UNIX || XDEVL_PLATFORM_MINGW
		if(pthread_create(&m_thread, NULL, Thread::ThreadProc2, this) != 0) {
			std::cerr << "Thread::Could not create thread." << std::endl;
			return 1;
		}
#else
		m_thread = CreateThread(0, 0, Thread::ThreadProc2, this, 0, &m_threadId);
		if(m_thread == NULL) {
			return 1;
		}

#endif
		return 0;
	}


	int Thread::Join() {
		if(m_thread == 0)
			return 1;

		int ret = 0;
#if XDEVL_PLATFORM_UNIX || XDEVL_PLATFORM_MINGW
		if(pthread_join(m_thread, NULL) != 0)
			ret = 1;
#else
		if(WaitForSingleObject(m_thread,INFINITE) == WAIT_FAILED)
			ret = 1;
#endif
		return ret;
	}

	void Thread::Exit(int exitCode) {
#if XDEVL_PLATFORM_UNIX || XDEVL_PLATFORM_MINGW
		int tmp = exitCode;
		pthread_exit(&tmp);
#else
		ExitThread((DWORD)exitCode);
#endif
	}

#if XDEVL_PLATFORM_UNIX || XDEVL_PLATFORM_MINGW
	void* Thread::ThreadProc(void* p_this) {
#else
	unsigned long __stdcall Thread::ThreadProc(void* p_this) {
#endif
		Thread* ptr = static_cast<Thread*>(p_this);
#if XDEVL_PLATFORM_UNIX || XDEVL_PLATFORM_MINGW
		return reinterpret_cast<void*>(ptr->RunThread(ptr->Arguments()));
#else
		return static_cast<unsigned long>(ptr->RunThread(ptr->Arguments()));
#endif

	}

#if XDEVL_PLATFORM_UNIX || XDEVL_PLATFORM_MINGW
	void* Thread::ThreadProc2(void* p_this) {
#else
	unsigned long __stdcall Thread::ThreadProc2(void* p_this) {
#endif
		Thread* ptr = static_cast<Thread*>(p_this);

#if XDEVL_PLATFORM_UNIX || XDEVL_PLATFORM_MINGW
		return reinterpret_cast<void*>(ptr->callbackFunction());
#else
		return static_cast<unsigned long>(ptr->callbackFunction());
#endif

	}

	int Thread::RunThread(ThreadArgument* p_arg) {
		return 0;
	}

	ThreadArgument* Thread::Arguments() {
		return m_arg;
	}

	void Thread::Arguments(ThreadArgument* p_arg) {
		m_arg = p_arg;
	}

}
