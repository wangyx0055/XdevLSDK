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

#ifndef XDEVL_VERTEX_ARRAY_H
#define XDEVL_VERTEX_ARRAY_H

#include <XdevLRAI/XdevLVertexDeclaration.h>

namespace xdl {

	class XdevLVertexDeclaration;
	class XdevLShaderProgram;
	class XdevLVertexBuffer;
	class XdevLIndexBuffer;

	/**
		@class XdevLVertexArray
		@brief Stream Buffer Processor.
	*/
	class XdevLVertexArray {
		public:
			virtual ~XdevLVertexArray() {}

			/// Initialize empty vertex array.
			virtual xdl_int init() = 0;

			/// Initialize a vertex array with one vertex buffer.
			virtual xdl_int init(xdl_uint8* src, xdl_uint numberOfVertex, XdevLVertexDeclaration* vd) = 0;

			/// Initialize with a provided vertex buffer.
			virtual xdl_int init(XdevLVertexBuffer* vertexBuffer, XdevLVertexDeclaration* vd) = 0;

			/// Initialize with a list of stream buffers.
			virtual xdl_int init(	xdl_uint8 numberOfStreamBuffers,
			                        xdl_uint8* srcOfSreamBuffers[],
			                        xdl_uint numberOfVertex,
			                        XdevLVertexDeclaration* vd) = 0;

			/// Initialize with a list of stream buffers and a index buffer.
			virtual xdl_int init(	xdl_uint32 numberIndices,
			                        xdl_uint8* srcOfIndices,
			                        xdl_uint8 numberOfStreamBuffers,
			                        xdl_uint8* srcOfSreamBuffers[],
			                        xdl_uint numberOfVertex,
			                        XdevLVertexDeclaration* vd) = 0;

			/// Activate.
			virtual void activate() = 0;

			/// Deactivate.
			virtual void deactivate() = 0;

			/// Sets a stream buffer.
			virtual xdl_int setVertexStreamBuffer(xdl_uint shaderAttribute,
			                                      xdl_uint numberOfComponents,
			                                      XdevLBufferElementTypes itemSizeType,
			                                      XdevLVertexBuffer* vertexBuffer) = 0;

			/// Sets the index buffer.
			virtual xdl_int setIndexBuffer(XdevLIndexBuffer* indexBuffer) = 0;

			/// Returns the specified vertex buffer.
			virtual XdevLVertexBuffer* getVertexBuffer(xdl_uint indexNumber) = 0;

			/// Returns the index buffer.
			virtual XdevLIndexBuffer* getIndexBuffer() = 0;

			/// Returns the vertex declaration.
			virtual XdevLVertexDeclaration* getVertexDeclaration() = 0;

			/// Returns the identification code.
			virtual xdl_uint id() = 0;
	};
	
	typedef XdevLVertexArray IXdevLVertexArray;
	typedef XdevLVertexArray* IPXdevLVertexArray;

}

#endif
