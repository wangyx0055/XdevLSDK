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

#ifndef XDEVL_SEMAPHORE_UNIX_H
#define XDEVL_SEMAPHORE_UNIX_H

#include <XdevLTypes.h>

namespace xdl {

	/// Parameter to create the sempahore
	struct XdevLSemaphoreModeCreate {
	};

	/// Parameter to open a semaphore
	struct XdevLSemaphoreModeOpen {
	};

	const XdevLSemaphoreModeCreate XdevLSemaphoreCreate;
	const XdevLSemaphoreModeOpen 	XdevLSemaphoreOpen;

	/**
	 * @enum CreationMode
	 * @brief Creating modes.
	 */
	enum CreationMode {
	  OPEN, 		/// Only start in open mode. Opener is not the owner.
	  CREATE		/// Create the semaphore. Creator will be the owner.
	};

	/**
	 * @class XdevLSemaphore
	 * @brief Semaphore
	 * @author Cengiz Terzibas
	 *
	 * The semaphore is a mutex on the kernel level. Two different processes can access the semaphore to sync
	 * each other.
	 */
	class XdevLSemaphore {
		public:

			XdevLSemaphore();

			~XdevLSemaphore() throw();

			/// Create the semaphore.
			/**
			 * @param create The owner the the semaphore will be this object.
			 * @param name Name of the semaphore.
			 * @param value The initial value for the semaphore.
			 *
			 * Throws exception if not possible.
			 */
			XdevLSemaphore(XdevLSemaphoreModeCreate& create,
			               const xdl_char* name,
			               xdl_uint value) throw();
							
			/// Open the semaphore.
			/**
			 * @param open The owner of the semaphore will be another object.
			 * @param name Name of the semaphore.
			 * @param value The initial value for the semaphore.
			 *
			 * Throws exception if not possible.
			 */
			XdevLSemaphore(	XdevLSemaphoreModeOpen& open,
			                const xdl_char* name,
			                xdl_uint value) throw();

			/// Open a named semaphore.
			xdl_int open(const xdl_char* name, xdl_uint value);

			/// Create a named semaphore.
			xdl_int create(const xdl_char* name, xdl_uint value);

			/// Close the semaphore.
			void close() throw();

			/// Lock the mutex.
			void wait() throw();

			/// Unlock the mutex.
			void post() throw();

		private:

			sem_t* m_smp;
			std::string m_name;
	};


}


#endif
