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

#ifndef XDEVL_ERROR_H
#define XDEVL_ERROR_H

#include <XdevLTypes.h>
#include <iostream>

namespace xdl {

	/**
		@enum XdevLErrorCodes
		@brief Error codes that can occur in the core system.
	*/
	enum XdevLErrorCodes {

	  /// No problem occurred.
	  ERR_OK = 0,

	  /// Some unspecific error occurred.
	  ERR_ERROR,

	  /// Plugin not found.
	  ERR_PLUGIN_NOT_FOUND,

	  /// Plugin exist already.
	  ERR_PLUGIN_EXIST_ALREADY,

	  /// No valid module found.
	  ERR_MODULE_NOT_FOUND,

	  /// Module exist already.
	  ERR_MODULE_EXIST_ALREADY,

	  /// Something unspecific, but fatal.
	  ERR_FATAL,

	  /// Device is busy but the core may work correctly.
	  ERR_DEVICE_BUSY,

	  /// The device does not work at all.
	  ERR_DEVICE_NOT_WORKING,

	  /// No memory available.
	  ERR_OUT_OF_MEMORY,

	  /// Input/Output operation failed.
	  ERR_IO,

	  /// The user has forgotten to specify the main xml filename.
	  ERR_MAIN_XML_FILE_NOT_SPECIFIED,

	  /// The main xml file could not be found.
	  ERR_MAIN_XML_FILE_NOT_FOUND,

	  /// The main xml has a wrong format.
	  ERR_WRONG_MAIN_XML_FORMAT,

	  /// The core plugin could not be found.
	  ERR_CORE_PLUGIN_NOT_FOUND,

	  /// The core plugin could not be loaded.
	  ERR_CORE_PLUGIN_LOADING_FAILED,

	  /// Critical plugin error. The address of a function could not be returned.
	  ERR_GET_FUNCTION_ADDRESS_FAILED,

	  /// The core system could not be created.
	  ERR_CORE_CREATION_FAILED,

	  /// The core system could not be initialized.
	  ERR_CORE_INITIALIZATION_FAILED,

	  /// Unknown error.
	  ERR_UNKNOWN

	};

	/**
		@class XdevLError
		@brief A XdevL error object that describes an error.
	*/
	class XdevLError {
		public:
			XdevLError(XdevLErrorCodes code, const XdevLString& desc, const XdevLString& filename, xdl_int line = 0) :
				m_errorCode(code),
				m_description(desc),
				m_filename(filename),
				m_line(line) {}

			/// Returns the description of the error.
			const XdevLString& getDescription() const {
				return m_description;
			}
			
			/// Returns the filename of the error.
			const XdevLString& getFilename() const {
				return m_description;
			}
			
			/// Returns the error code.
			XdevLErrorCodes getErrorCode() const {
				return m_errorCode;
			}
			
			/// Returns the line number in the file.
			xdl_int getLine() const {
				return m_line;
			}

			friend std::ostream& operator << (std::ostream& os, const XdevLError& err) {
				os << "[ Error Code: " << err.m_errorCode << ": " << err.m_filename << ":" << err.m_line << ": " << err.m_description << " ]";
				return os;
			}

		private:
			XdevLErrorCodes	m_errorCode;
			XdevLString			m_description;
			XdevLString			m_filename;
			xdl_int					m_line;
	};

	#define XDEVL_ERROR(CODE, DESC) xdl::XdevLError(CODE, DESC, xdl::XdevLString(__FILE__), __LINE__)
}

#endif
