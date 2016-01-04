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

#ifndef XDEVL_INDEXBUFFER_IMPL_H
#define XDEVL_INDEXBUFFER_IMPL_H

#include <XdevLRAI/XdevLIndexBuffer.h>

namespace xdl {

	class XdevLIndexBufferImpl : public XdevLIndexBuffer {
		public:
			XdevLIndexBufferImpl() : 	m_id(0), 
																m_size(0), 
																m_elementType(XDEVL_BUFFER_ELEMENT_UNSIGNED_INT), 
																m_activated(xdl_false), 
																m_locked(xdl_false), 
																m_mapped(xdl_false) {}

			virtual ~XdevLIndexBufferImpl() {
				glDeleteBuffers(1, &m_id);
			}

			virtual xdl_int init() {
				glGenBuffers(1, &m_id);
				if(m_id == -1) {
					return ERR_ERROR;
				}
				
				return ERR_OK;
			}

			virtual xdl_int init(xdl_uint8* src, xdl_uint size) {
				glGenBuffers(1, &m_id);
				if(m_id == -1) {
					return ERR_ERROR;
				}

				m_size = size;

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, src, GL_STREAM_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				return ERR_OK;
			}

			virtual xdl_int init(XdevLBufferElementTypes indexBufferType, xdl_uint8* src, xdl_uint numberOfIndices) {
				glGenBuffers(1, &m_id);
				if(m_id == -1) {
					return ERR_ERROR;
				}

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, getSizeOfElementType(indexBufferType)*numberOfIndices, src, GL_STREAM_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				m_size = numberOfIndices;
				m_elementType = indexBufferType;
				
				return ERR_OK;
			}

			virtual xdl_int lock() {
				assert(!m_locked && "XdevLIndexBufferImpl::lock: Was locked already.");
				
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
				m_locked = xdl_true;
				
				return ERR_OK;
			}

			virtual xdl_int unlock() {
				assert(m_locked && "XdevLIndexBufferImpl::unlock: Was not locked.");
				
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				m_locked = xdl_false;
				
				return ERR_OK;
			}


			virtual xdl_uint8* map(XdevLBufferAccessType bufferAccessType) {
				assert(m_locked && "XdevLVertexBufferImpl::copy: Was not locked.");

				xdl_uint8* memory = (xdl_uint8*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferAccessType);
				m_mapped = xdl_true;

				return memory;
			}

			virtual xdl_int unmap() {
				if(glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) != GL_TRUE) {
					return ERR_ERROR;
				}
				return ERR_OK;
			}

			xdl_int upload(xdl_uint8* src, xdl_uint size) {
				assert(m_locked && "XdevLVertexBufferImpl::copy: Was not locked.");
				
				// What do we here? Only glBufferData is creating the buffer. Only
				// recreate a new one if the specified size is bigger than this buffer
				// can hold.
				// TODO GL_DYNAMIC_DRAW is it neccessary?
				if(m_size < size) {
					std::cout << "Resizing ... " << std::endl;
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, src, GL_DYNAMIC_DRAW);
					m_size 		= size;
				} else {
					glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, src);
				}
				m_mapped 	= xdl_false;

				return ERR_ERROR;
			}

			virtual xdl_int activate() {
				assert(!m_activated && "XdevLIndexBufferImpl::activate: Was activated already.");
				
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
				m_activated = xdl_true;
				m_locked 		= xdl_true;
				return ERR_OK;
			}

			virtual xdl_int deactivate() {
				assert(m_activated && "XdevLIndexBufferImpl::deactivate: Was not activated.");
				
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				m_activated = xdl_false;
				m_locked 		= xdl_false;
				return ERR_OK;
			}

			virtual xdl_uint id() {
				return m_id;
			}

			virtual xdl_uint getSize() {
				return m_size;
			}

			virtual XdevLBufferElementTypes		getElementType() {
				return m_elementType;
			}


			xdl_uint getSizeOfElementType(XdevLBufferElementTypes type) {
				switch(type) {
					case XDEVL_BUFFER_ELEMENT_BYTE:
					case XDEVL_BUFFER_ELEMENT_UNSIGNED_BYTE:
						return 1;
					case XDEVL_BUFFER_ELEMENT_SHORT:
					case XDEVL_BUFFER_ELEMENT_UNSIGNED_SHORT:
						return 2;
					case XDEVL_BUFFER_ELEMENT_INT:
					case XDEVL_BUFFER_ELEMENT_UNSIGNED_INT:
						return 4;
					case XDEVL_BUFFER_ELEMENT_TYPE_UNKNOWN:
					default:
						assert(0 && "XdevLIndexBufferImpl::getSizeOfElementType: XDEVL_BUFFER_ELEMENT_TYPE_UNKNOWN.");
				}
				return 0;
			}


			GLuint 											m_id;
			xdl_uint 										m_size;
			XdevLBufferElementTypes			m_elementType;
			xdl_bool										m_activated;
			xdl_bool										m_locked;
			xdl_bool										m_mapped;
	};


}


#endif
