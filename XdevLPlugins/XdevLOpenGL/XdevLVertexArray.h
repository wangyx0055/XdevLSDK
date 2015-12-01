/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2014 Cengiz Terzibas

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

#ifndef XDEVL_VERTEX_ARRAY_H
#define XDEVL_VERTEX_ARRAY_H

#include <XdevLOpenGL/XdevLVertexDeclaration.h>

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

}

#endif
