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
