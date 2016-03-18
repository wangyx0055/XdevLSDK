/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2016 Cengiz Terzibas

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

#ifndef XDEVL_CONDITIONAL_VARIABLE_H
#define XDEVL_CONDITIONAL_VARIABLE_H

#include <XdevLTypes.h>
#include <XdevLMutex.h>

#if defined (XDEVL_PLATFORM_UNIX)  || (defined XDEVL_PLATFORM_MINGW)
typedef pthread_cond_t THREAD_COND;
#elif XDEVL_PLATFORM_WINDOWS 
typedef CONDITION_VARIABLE THREAD_COND;
#else
#error "No implementation for this platform."
#endif


namespace thread {

	/**
	 @class ConditionalVariable
	 @brief Conditional variable.
	*/
	class ConditionalVariable {
		public:
			/// Initialize the conditional variable.
			ConditionalVariable();

			/// Destroy the conditional variable.
			~ConditionalVariable();

			/// Send signal.
			int signal();

			/// Send signal to all.
			int brodcast();

			/// Wait for signal.
			int wait(Mutex& mutex);

		private:

			THREAD_COND m_condition;

	};


}

#endif
