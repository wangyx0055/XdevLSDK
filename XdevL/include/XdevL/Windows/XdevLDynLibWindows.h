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

#ifndef XDEVL_DYNLIB_UNIX_H
#define XDEVL_DYNLIB_UNIX_H

#include <XdevLTypes.h>

#include <windows.h> 

namespace xdl {

	typedef FARPROC SHARED_FUNCTION;

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

			HINSTANCE m_Module;

		protected:

			bool hasValidExtension(std::string& str);

	};

}

#endif
