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

#ifndef XDEVL_MUTEX_H
#define XDEVL_MUTEX_H

#include <XdevLPlatform.h>

#if XDEVL_PLATFORM_UNIX || XDEVL_PLATFORM_MINGW
typedef pthread_mutex_t THREAD_MUTEX;
#elif XDEVL_PLATFORM_WINDOWS
typedef CRITICAL_SECTION THREAD_MUTEX;
#else
#error "No implementation for this platform."
#endif

namespace thread {

	/**
		@class Mutex
		@brief Mutex class
		@author Cengiz Terzibas
	 */
	class Mutex {
		public:
			/// Constructor for the mutex
			/**
				If you create an instance of the mutex class you have to use the exception paradigm of c++ to
				check whether it was successful or not.

				@code
				try{
					Mutex m_mutex;
				}catch(const char* ecp){
				}
				@endcode

			*/
			Mutex();
			virtual ~Mutex();
			/// Lock mutex
			/**
				@return
					-# @b 0 if it was successful.
					-# @b 1 if an error occurred.
			*/
			virtual int Lock();
			/// Unlock mutex
			/**
				@return
					-# @b 0 if it was successful.
					-# @b 1 if an error occurred.
			*/
			virtual int Unlock();

			THREAD_MUTEX& getNativeMutex();

		protected:
			THREAD_MUTEX m_mutex;
	};

	/**
	 @class ScopeLock
	 @brief Locks and unlocks a mutex automatically.
	 @author Cengiz Terzibas
	*/
	class XdevLScopeLock {
		public:
			XdevLScopeLock(const Mutex& mutex) : m_mutex(mutex) {
				m_mutex.Lock();
			}
			
			~XdevLScopeLock() {
				m_mutex.Unlock();
			}
			
			void Lock() {
				m_mutex.Lock();
			}
			
			void Unlock() {
				m_mutex.Unlock();
			}
			
			operator Mutex& () {
				return m_mutex;
			}

	private:
		
			Mutex m_mutex;
	};

}
#endif /* MUTEX_H_ */
