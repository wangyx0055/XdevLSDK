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

#ifndef XDEVL_MUTEX_WINDOWS_H
#define XDEVL_MUTEX_WINDOWS_H

#include <XdevLPlatform.h>

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

			CRITICAL_SECTION& getNativeMutex();

		protected:
			CRITICAL_SECTION m_mutex;
	};

}
#endif 
