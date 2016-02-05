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

#ifndef XDEVL_VERTEXBUFFER_IMPL_H
#define XDEVL_VERTEXBUFFER_IMPL_H

#include <XdevLRAI/XdevLVertexBuffer.h>
#include "XdevLOpenGLUtils.h"
#include <cassert>
#include <string.h>
#include <iostream>

namespace xdl {

	class XdevLVertexBufferImpl : public XdevLVertexBuffer {
		public:
			XdevLVertexBufferImpl() : m_id(0),
				m_size(0),
				m_locked(xdl_false),
				m_mapped(xdl_false) {}

			virtual ~XdevLVertexBufferImpl() {
				if(0 != m_id) {
					glDeleteBuffers(1, &m_id);
				}
			}

			virtual xdl_int init() {
				glGenBuffers(1, &m_id);
				if(m_id == 0) {
					return ERR_ERROR;
				}
				return ERR_OK;
			}

			virtual xdl_int init(xdl_uint8* src, xdl_uint size) {

				// Create and empty VBO name and bind it.
				glGenBuffers(1, &m_id);
				glBindBuffer(GL_ARRAY_BUFFER, m_id);
				if(!glIsBuffer(m_id)) {
					return ERR_ERROR;
				}

				// Created and initialize the data store of the VBO.
				glBufferData(GL_ARRAY_BUFFER, size, src, GL_STATIC_DRAW);

				// Unbind to prevent override in other OpenGL code fragments.
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				m_size = size;

				return ERR_OK;
			}

			virtual xdl_int lock() {
				assert(!m_locked && "XdevLVertexBufferImpl::lock: Was locked already.");

				GL_CHECK_VAO_BOUND("A VAO is bound. Locking this VBO will cause to be bound to the VAO.");

				glBindBuffer(GL_ARRAY_BUFFER, m_id);
				if(!glIsBuffer(m_id)) {
					return ERR_ERROR;
				}

				m_locked = xdl_true;

				return ERR_OK;
			}

			virtual xdl_int unlock() {
				assert(m_locked && "XdevLVertexBufferImpl::unlock: Was not locked.");

				GL_CHECK_VAO_BOUND("A VAO is bound. Unlocking this VBO will cause to be unbound from the VAO.");

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				xdl_int ret = ERR_OK;
				if(glIsBuffer(m_id)) {
					ret = ERR_ERROR;
				}

				m_locked = xdl_false;

				return ret;
			}

			virtual xdl_uint8* map(XdevLBufferAccessType bufferAccessType) {
				assert(m_locked && "XdevLVertexBufferImpl::copy: Was not locked.");

				xdl_uint8* memory = (xdl_uint8*)glMapBuffer(GL_ARRAY_BUFFER, bufferAccessType);
				if(nullptr != memory) {
					m_mapped = xdl_true;
				}

				return memory;
			}

			virtual xdl_int unmap() {
				if(glUnmapBuffer(GL_ARRAY_BUFFER) != GL_TRUE) {
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
					m_size 		= size;
					std::cout << "XdevLVertexBufferImpl::upload: Resizing Vertex Buffer to: " << m_size << std::endl;
					glBufferData(GL_ARRAY_BUFFER, size, src, GL_DYNAMIC_DRAW);
				} else {
					glBufferSubData(GL_ARRAY_BUFFER, 0, size, src);
				}

				m_mapped 	= xdl_false;

				return ERR_ERROR;
			}

			virtual xdl_uint id() {
				return m_id;
			}

			virtual xdl_uint getSize() {
				return m_size;
			}


			GLuint 									m_id;
			xdl_uint 								m_size;
			xdl_bool								m_locked;
			xdl_bool								m_mapped;
	};


}


#endif
