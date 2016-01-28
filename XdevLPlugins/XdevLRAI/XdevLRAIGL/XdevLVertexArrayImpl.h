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
			virtual xdl_int init(xdl_uint8* src, xdl_uint numberOfVertex, IPXdevLVertexDeclaration vd);
			virtual xdl_int init(IPXdevLVertexBuffer vertexBuffer, IPXdevLVertexDeclaration vd);
			virtual xdl_int init(IPXdevLVertexBuffer vertexBuffer, IPXdevLIndexBuffer indexBuffer, IPXdevLVertexDeclaration vd) override;
			virtual xdl_int init(xdl_uint8 numberOfStreamBuffers,
			                     xdl_uint8* srcOfSreamBuffers[],
			                     xdl_uint numberOfVertex,
			                     IPXdevLVertexDeclaration vd);
			virtual xdl_int init(xdl_uint32 numberIndices,
			                     xdl_uint8* srcOfIndices,
			                     xdl_uint8 numberOfStreamBuffers,
			                     xdl_uint8* srcOfSreamBuffers[],
			                     xdl_uint numberOfVertex,
			                     IPXdevLVertexDeclaration vd);
			virtual void activate();
			virtual void deactivate();
			virtual xdl_int setVertexStreamBuffer(xdl_uint shaderAttribute,
			                                      xdl_uint numberOfComponents,
			                                      XdevLBufferElementTypes itemSizeType,
			                                      IPXdevLVertexBuffer vertexBuffer);
			virtual xdl_int setIndexBuffer(IPXdevLIndexBuffer indexBuffer);
			virtual IPXdevLVertexBuffer getVertexBufferRef(xdl_uint indexNumber);
			virtual IPXdevLIndexBuffer getIndexBufferRef();
			virtual XdevLVertexBuffer* getVertexBuffer(xdl_uint indexNumber);
			virtual XdevLIndexBuffer* getIndexBuffer();
			virtual IPXdevLVertexDeclaration getVertexDeclarationRef();
			virtual XdevLVertexDeclaration* getVertexDeclaration();
			virtual xdl_uint id();
		private:
			GLuint 														m_id;
			IPXdevLVertexDeclaration 					m_vd;
			std::vector<IPXdevLVertexBuffer>	m_vertexBufferList;
			IPXdevLIndexBuffer								m_indexBuffer;
			xdl::xdl_bool 										m_activated;

	};

}

#endif
