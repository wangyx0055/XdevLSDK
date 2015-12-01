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

#ifndef XDEVL_MUTEX_H
#define XDEVL_MUTEX_H

#include <XdevLPlatform.h>

#ifdef XDEVL_PLATFORM_UNIX
typedef pthread_mutex_t THREAD_MUTEX;
#elif defined (XDEVL_PLATFORM_WINDOWS) || defined(XDEVL_PLATFORM_MINGW)
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
