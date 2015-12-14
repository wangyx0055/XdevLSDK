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
