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

#ifndef XDEVL_VERTEX_DECLARATION_H
#define XDEVL_VERTEX_DECLARATION_H

#include <XdevLTypes.h>
#include <XdevLRAI/XdevLStreamBuffer.h>
#include <vector>
#include <cassert>

namespace xdl {


	struct XdevLVertexDeclarationItem {
		XdevLVertexDeclarationItem(xdl_uint n, XdevLBufferElementTypes s, xdl_uint sa, xdl_uint etsib) : elementType(s),
			numberOfComponents(n), shaderAttribute(sa), elementTypeSizeInBytes(etsib) {}
		/// The type of the item.
		XdevLBufferElementTypes	elementType;
		xdl_uint numberOfComponents;
		xdl_uint shaderAttribute;
		xdl_uint elementTypeSizeInBytes;
	};

	class XdevLVertexDeclaration {
		public:
			~XdevLVertexDeclaration() {
				for(auto& vertexDeclerationitem : m_list) {
					delete vertexDeclerationitem;
				}
			}

			void add( xdl_uint number, XdevLBufferElementTypes size, xdl_uint shader_attribute) {
				m_list.push_back(new XdevLVertexDeclarationItem( number, size, shader_attribute, bufferElementSize(size)));
			}

			xdl_uint getNumber() {
				return m_list.size();
			}

			XdevLVertexDeclarationItem* get(xdl_uint idx) {
				return m_list[idx];
			}

			xdl_uint vertexStride() {
				if(m_list.size() == 1) {
					return 0;
				}
				return vertexSize();
			}

			xdl_uint vertexSize() {
				xdl_uint tmp = 0;
				std::vector<XdevLVertexDeclarationItem*>::iterator ib(m_list.begin());
				while(ib != m_list.end()) {
					tmp += bufferElementSize((*ib)->elementType) * (*ib)->numberOfComponents;
					ib++;
				}
				return tmp;
			}
		private:
			xdl_uint bufferElementSize(XdevLBufferElementTypes type) {
				switch(type) {
					case XDEVL_BUFFER_ELEMENT_BYTE:
						return 1;
					case XDEVL_BUFFER_ELEMENT_UNSIGNED_BYTE:
						return 1;
					case XDEVL_BUFFER_ELEMENT_SHORT:
						return 2;
					case XDEVL_BUFFER_ELEMENT_UNSIGNED_SHORT:
						return 2;
					case XDEVL_BUFFER_ELEMENT_INT:
					case XDEVL_BUFFER_ELEMENT_UNSIGNED_INT:
					case XDEVL_BUFFER_ELEMENT_FLOAT:
						return 4;
					case XDEVL_BUFFER_ELEMENT_TYPE_UNKNOWN:
					default:
						assert(0 && "XdevLVertexDeclaration::vertexSize: Unknown Buffer Element");
				}
				return 0;
			}
			std::vector<XdevLVertexDeclarationItem*> m_list;
	};

}

#endif
