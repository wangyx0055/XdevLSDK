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

#ifndef XDEVL_DEVICE_H
#define XDEVL_DEVICE_H

#include <XdevLModule.h>

namespace xdl {
	
	/**
		@class XdevLStream
		@brief Interface class stream objects.
		@author Cengiz Terzibas

		This can be files, serial port, buses etc.

		@note Normal users shouldn't use this functions.
	*/
	class XdevLStream : public XdevLModule {
		public:
			enum class AccesType {
			  XDEVL_DEVICE_READ_ONLY 	= 1,
			  XDEVL_DEVICE_WRITE_ONLY = 2,
			  XDEVL_DEVICE_READ_WRITE = 4
			};

			virtual ~XdevLStream() {}
			
			/// Open a connection using XML properties.
			virtual xdl_int open() = 0;

			/// Open a connection to the device with a specific name.
			virtual xdl_int open(const XdevLFileName& name) = 0;

			/// Close the connection.
			virtual xdl_int close() = 0;

			/// Read data from the device.
			virtual xdl_int read(xdl_uint8* dst, xdl_int size) = 0;

			/// Write data to the device.
			virtual xdl_int write(xdl_uint8* src, xdl_int size) = 0;

			/// Flush input and output buffer.
			/**
				@return
					- @b ERR_ERROR if it failed.
					- @b ERR_OK if it was successful.
			*/
			virtual xdl_int flush() = 0;
	};

	typedef std::vector<XdevLStream::AccesType> XdevLDeviceModes;
}

#endif
