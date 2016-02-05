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

#ifndef XDEVL_STREAM_BUFFER_H
#define XDEVL_STREAM_BUFFER_H

namespace xdl {

	enum XdevLBufferAccessType {
	  XDEVL_BUFFER_READ_ONLY	= 0x88B8,
	  XDEVL_BUFFER_WRITE_ONLY	= 0x88B9,
	  XDEVL_BUFFER_READ_WRITE	= 0x88BA,
	  XDEVL_BUFFER_ACCESS_TYPE_UNKNOWN
	};

	enum XdevLBufferElementTypes {
	  XDEVL_BUFFER_ELEMENT_BYTE 			= 0x1400,
	  XDEVL_BUFFER_ELEMENT_UNSIGNED_BYTE 	= 0x1401,
	  XDEVL_BUFFER_ELEMENT_SHORT 			= 0x1402,
	  XDEVL_BUFFER_ELEMENT_UNSIGNED_SHORT	= 0x1403,
	  XDEVL_BUFFER_ELEMENT_INT 			= 0x1404,
	  XDEVL_BUFFER_ELEMENT_UNSIGNED_INT	= 0x1405,
	  XDEVL_BUFFER_ELEMENT_FLOAT 			= 0x1406,
	  XDEVL_BUFFER_ELEMENT_TYPE_UNKNOWN
	};

	class XdevLStreamBuffer {
		public:
			virtual ~XdevLStreamBuffer() {}

			/// Initialize an empty buffer.
			virtual xdl_int init() = 0;

			/// Initialize a buffer with a specified size.
			/**
				@param src The source data to copy from.
				@param size The number of bytes to copy form the src.
			*/
			virtual xdl_int init(xdl_uint8* src, xdl_uint size) = 0;

			/// Lock the buffer for modification.
			/**
				Locks the buffer for modification. Before one can upload/change data
				the buffer needs to be locked. After finishing the job unlock the buffer.
			*/
			virtual xdl_int lock() = 0;

			/// Unlock the buffer after modification.
			virtual xdl_int unlock() = 0;

			/// Maps the buffer.
			/*
				Works only if the buffer was locked before.
			*/
			virtual xdl_uint8* map(XdevLBufferAccessType bufferAccessType) = 0;

			/// Unmaps the buffer.
			virtual xdl_int unmap() = 0;

			/// Upload streaming data to the buffer.
			/**
				The buffer maybe be rezied dependend on the size value.

				@param src The pointer to the data array to upload.
				@param size The the number of bytes to upload from the 'src'.
			*/
			virtual xdl_int upload(xdl_uint8* src, xdl_uint size) = 0;

			/// Returns the identification code.
			virtual xdl_uint id() = 0;

			/// Returns the size of the buffer in bytes.
			virtual xdl_uint getSize() = 0;
	};

	typedef XdevLStreamBuffer IXdevLStreamBuffer;
	typedef XdevLStreamBuffer* IPXdevLStreamBuffer;

}

#endif
