/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2014 Cengiz Terzibas

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
		if(m_thread) {
			if(CloseHandle(m_thread) == 0) {
				std::cerr << "Thread::Can't close thread handle.\n";
			} else
				m_thread = NULL;
		}
	}

	int Thread::Start(ThreadArgument* arg) {
		Arguments(arg);

		m_thread = CreateThread(0, 0, Thread::ThreadProc, this, 0, &m_threadId);
		if(m_thread == NULL) {
			return 1;
		}

		return 0;
	}

int Thread::StartUsingFuncPointer() {

		m_thread = CreateThread(0, 0, Thread::ThreadProc2, this, 0, &m_threadId);
		if(m_thread == NULL) {
			return 1;
		}

		return 0;
	}


	int Thread::Join() {
		if(m_thread == 0)
			return 1;

		int ret = 0;

		if(WaitForSingleObject(m_thread,INFINITE) == WAIT_FAILED)
			ret = 1;

		return ret;
	}

	void Thread::Exit(int exitCode) {

		ExitThread((DWORD)exitCode);

	}


	unsigned long __stdcall Thread::ThreadProc(void* p_this) {

		Thread* ptr = static_cast<Thread*>(p_this);

		return static_cast<unsigned long>(ptr->RunThread(ptr->Arguments()));


	}


	unsigned long __stdcall Thread::ThreadProc2(void* p_this) {

		Thread* ptr = static_cast<Thread*>(p_this);

		return static_cast<unsigned long>(ptr->callbackFunction());

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
