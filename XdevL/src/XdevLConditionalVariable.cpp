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

#include <XdevLConditionalVariable.h>
#include <iostream>
#include <errno.h>
#include <string.h>

	namespace thread {

		ConditionalVariable::ConditionalVariable() {
#if XDEVL_PLATFORM_UNIX || XDEVL_PLATFORM_MINGW
			if ( pthread_cond_init(&m_condition, NULL) != 0 ) {
				throw("Mutex::Could not create mutex");
			}
#elif XDEVL_PLATFORM_WINDOWS
			InitializeConditionVariable(&m_condition);
#else
			#error "Not implemented for this platform"
#endif
		}

		ConditionalVariable::~ConditionalVariable() {

		}

		int ConditionalVariable::signal() {
#if XDEVL_PLATFORM_UNIX || XDEVL_PLATFORM_MINGW
			if (pthread_cond_signal(&m_condition) != 0) {
				std::cerr << "ConditionalVariable::Could not send signal: " << strerror(errno) << std::endl;
				return 1;
			}
#elif XDEVL_PLATFORM_WINDOWS
			WakeConditionVariable(&m_condition);
#else
			#error "Not implemented for this platform"
#endif
			return 0;
		}

		int ConditionalVariable::brodcast() {
#if XDEVL_PLATFORM_UNIX || XDEVL_PLATFORM_MINGW
			if ( pthread_cond_broadcast(&m_condition) != 0) {
				std::cerr << "ConditionalVariable::Could send signal to all: " << strerror(errno) << std::endl;
				return 1;
			}
#elif XDEVL_PLATFORM_WINDOWS
			WakeAllConditionVariable(&m_condition);
#else
			#error "Not implemented for this platform"
#endif
			return 0;
		}

		int ConditionalVariable::wait(Mutex& mutex) {
#if XDEVL_PLATFORM_UNIX || XDEVL_PLATFORM_MINGW
			if (pthread_cond_wait(&m_condition, &mutex.getNativeMutex()) != 0) {
				std::cerr << "ConditionalVariable::Could wait: " << strerror(errno) << std::endl;
				return 1;
			}
#elif XDEVL_PLATFORM_WINDOWS
			SleepConditionVariableCS(&m_condition, &mutex.getNativeMutex(), INFINITE);
#else
			#error "Not implemented for this platform"
#endif
			return 0;
		}

	
}
