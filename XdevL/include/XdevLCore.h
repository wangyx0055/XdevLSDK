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

#ifndef XDEVL_CORE_H
#define XDEVL_CORE_H

#include <XdevLModule.h>
#include <XdevLCoreMediator.h>

namespace xdl {

	/**
	  @class XdevLCoreParameters
	  @brief A parameter class for the XdevLCore init method.
	  @author Cengiz Terzibas
	*/
	class XdevLCoreParameters {
		public:

			XdevLCoreParameters() : pluginsPath("."),
				userDataList(nullptr),
				numberOfUserData(0) {
			}

			/// Holds the main XML filename.
			std::string 	xmlFileName;

			/// Holds the path to the plugins folder.
			std::string 	pluginsPath;

			/// A list of user defined data.
			XdevLUserData**	userDataList;

			/// The number of user defined data in the list.
			xdl_uint 		numberOfUserData;

	};

	/**
		@class XdevLCore
		@brief The interface class for XdevLCore
		@author Cengiz Terzibas
	*/
	class XdevLCore : public XdevLCoreMediator, public XdevLModule {
		public:

			virtual ~XdevLCore() {}

			/// Update internal states.
			/**
				Updates internal states, redirects events, internal timer etc.
				@return
				- @b ERR_OK If the process was successful.
				- @b ERR_ERROR An error occurred in the update pipeline.
			*/
			virtual xdl_int update() = 0;

			/// Initialize the core system.
			/**
				@param parameters Parameters for the XdevLCore system.
				@return
				- @b ERR_OK
				The core system is initialized successfully.
				- @b ERR_ERROR
				The core system couldn't be initialized.
			*/
			virtual xdl_int setParameters(const XdevLCoreParameters& parameters) = 0;

	};

	typedef XdevLCore*	IPXdevLCore;
	typedef XdevLCore	IXdevLCore;

	XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DECLARATION(XdevLCore)
}

#endif
