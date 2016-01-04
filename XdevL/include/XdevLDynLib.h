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

#ifndef XDEVL_DYNLIB_H
#define XDEVL_DYNLIB_H

#include <XdevLPlatform.h>
#include <XdevLTypes.h>

namespace xdl {

	// TODO The SHARED_FUNCTION typedef holds the address of a upt 64 bit value.
	// This may be later a problem and should be changed.
	typedef void*	SHARED_LIB;
	typedef xdl_uint64	SHARED_FUNCTION;

	/**
		@class XdevLSharedLibrary
		@brief Class to handle shared libraries
		@author (c) 2007-2014 Cengiz Terzibas & Mesut Bueyuektokatli
	*/
	class XdevLSharedLibrary {
		public:

			XdevLSharedLibrary();

			~XdevLSharedLibrary();

			static XdevLString extension;

			/// Loads the specified library
			/**
				@param pName The name of the library. The name must be without the extension, like .dll or .so .
				The Core system will do that automatically.
				@return
					- @b 0 if the loading was successful.
					- @b 1 if an error occurred.
			*/
			xdl_int open(const xdl_char* pName);


			/// Free the internal dynamic library.
			/**
			If you want to load another dynamic library you have to free the previous one.
			If you don't do that the LoadLib() method will do that automatically.
			@return
				- \b 0 if the loading was successful.
				- \b 1 if an error occurred.
			*/
			xdl_int close();

			/// Returns the specified function address in the dynamic library.
			/**
				@param pName The name of the function in the library
				@return a SHARED_FUNCTION (void*) pointer to the specified function.
			*/
			SHARED_FUNCTION getFunctionAddress(const xdl_char* pName);

		protected:

			/// Holds the XdevLModule of the dynamic library
			SHARED_LIB m_Module;

		protected:

			bool hasValidExtension(std::string& str);

	};

}

#endif
