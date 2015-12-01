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

#ifndef XDEVL_VERTEX_ARRAY_IMPL_H
#define XDEVL_VERTEX_ARRAY_IMPL_H

#include <XdevLVertexArray.h>

namespace xdl {

	struct XdevLVertexStreamItem {
			XdevLVertexStreamItem(xdl_uint numberOfComponents,
			                      XdevLBufferElementTypes bufferElementType,
			                      xdl_uint shaderAttribute,
			                      XdevLVertexBuffer* vertexBuffer) :
				m_numberOfComponents(numberOfComponents),
				m_bufferElementType(bufferElementType),
				m_shaderAttribute(shaderAttribute),
				m_vertexBuffer(vertexBuffer) {}
		private:
			xdl_uint 				m_numberOfComponents;
			XdevLBufferElementTypes	m_bufferElementType;
			xdl_uint				m_shaderAttribute;
			XdevLVertexBuffer*		m_vertexBuffer;
	};

	class XdevLVertexArrayImpl : public XdevLVertexArray {
		public:
			XdevLVertexArrayImpl();
			virtual ~XdevLVertexArrayImpl();
			virtual xdl_int init();
			virtual xdl_int init(xdl_uint8* src, xdl_uint numberOfVertex, XdevLVertexDeclaration* vd);
			virtual xdl_int init(XdevLVertexBuffer* vertexBuffer, XdevLVertexDeclaration* vd);
			virtual xdl_int init(xdl_uint8 numberOfStreamBuffers,
			                     xdl_uint8* srcOfSreamBuffers[],
			                     xdl_uint numberOfVertex,
			                     XdevLVertexDeclaration* vd);
			virtual xdl_int init(xdl_uint32 numberIndices,
			                     xdl_uint8* srcOfIndices,
			                     xdl_uint8 numberOfStreamBuffers,
			                     xdl_uint8* srcOfSreamBuffers[],
			                     xdl_uint numberOfVertex,
			                     XdevLVertexDeclaration* vd);
			virtual void activate();
			virtual void deactivate();
			virtual xdl_int setVertexStreamBuffer(xdl_uint shaderAttribute,
			                                      xdl_uint numberOfComponents,
			                                      XdevLBufferElementTypes itemSizeType,
			                                      XdevLVertexBuffer* vertexBuffer);
			virtual xdl_int setIndexBuffer(XdevLIndexBuffer* indexBuffer);
			virtual XdevLVertexBuffer* getVertexBuffer(xdl_uint indexNumber);
			virtual XdevLIndexBuffer* getIndexBuffer();
			virtual XdevLVertexDeclaration* getVertexDeclaration();
			virtual xdl_uint id();
		private:
			GLuint 														m_id;
			XdevLVertexDeclaration* 					m_vd;
			std::vector<XdevLVertexBuffer*>		m_vertexBufferList;
			XdevLIndexBuffer*									m_indexBuffer;
			xdl::xdl_bool 										m_activated;

	};

}

#endif
