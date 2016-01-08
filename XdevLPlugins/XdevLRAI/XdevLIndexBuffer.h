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

#ifndef XDEVL_INDEX_BUFFER_H
#define XDEVL_INDEX_BUFFER_H

#include <XdevLRAI/XdevLVertexDeclaration.h>
#include <XdevLRAI/XdevLStreamBuffer.h>

namespace xdl {

	class XdevLIndexBuffer : public XdevLStreamBuffer {
		public:
			virtual ~XdevLIndexBuffer() {}

			/// Initialize an empty buffer.
			using XdevLStreamBuffer::init;

			/// Initialize the vertex buffer.
			virtual xdl_int	init(XdevLBufferElementTypes indexBufferType, xdl_uint8* src, xdl_uint numberOfIndices) = 0;

			/// Returns the type of element used in the buffer.
			virtual XdevLBufferElementTypes	getElementType() = 0;

	};
	
	typedef XdevLIndexBuffer IXdevLIndexBuffer;
	typedef XdevLIndexBuffer* IPXdevLIndexBuffer;

}

#endif
