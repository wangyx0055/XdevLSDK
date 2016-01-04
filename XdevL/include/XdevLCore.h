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

#ifndef XDEVL_CORE_H
#define XDEVL_CORE_H

#include <XdevLModule.h>
#include <XdevLCoreMediator.h>

namespace xdl {

	/**
	  @class XdevLCoreInitParameters
	  @brief A parameter class for the XdevLCore init method.
	  @author Cengiz Terzibas
	*/
	class XdevLCoreInitParameters {
		public:

			XdevLCoreInitParameters() : pluginsPath("."),
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
			virtual xdl_int init(const XdevLCoreInitParameters& parameters) = 0;

	};

	typedef XdevLCore*	IPXdevLCore;
	typedef XdevLCore	IXdevLCore;

}

#endif
