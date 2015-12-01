#ifndef XDEVL_VERTEXBUFFER_IMPL_H
#define XDEVL_VERTEXBUFFER_IMPL_H

#include <XdevLOpenGL/XdevLVertexBuffer.h>
#include <cassert>
#include <string.h>
#include <iostream>

namespace xdl {

	class XdevLVertexBufferImpl : public XdevLVertexBuffer {
		public:
			XdevLVertexBufferImpl() : m_id(0),
				m_size(0),
				m_activated(xdl_false),
				m_locked(xdl_false),
				m_mapped(xdl_false) {}

			virtual ~XdevLVertexBufferImpl() {
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

				glBindBuffer(GL_ARRAY_BUFFER, m_id);
				glBufferData(GL_ARRAY_BUFFER, size, src, GL_STREAM_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				return ERR_OK;
			}

			virtual xdl_int lock() {
				assert(!m_locked && "XdevLVertexBufferImpl::lock: Was locked already.");
				
				glBindBuffer(GL_ARRAY_BUFFER, m_id);
				m_locked = xdl_true;
				
				return ERR_OK;
			}

			virtual xdl_int unlock() {
				assert(m_locked && "XdevLVertexBufferImpl::unlock: Was not locked.");
				
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				m_locked = xdl_false;
				
				return ERR_OK;
			}

			virtual xdl_uint8* map(XdevLBufferAccessType bufferAccessType) {
				assert(m_locked && "XdevLVertexBufferImpl::copy: Was not locked.");

				xdl_uint8* memory = (xdl_uint8*)glMapBuffer(GL_ARRAY_BUFFER, bufferAccessType);
				
				m_mapped = xdl_true;
				
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

			virtual xdl_int activate() {
				assert(!m_activated && "XdevLVertexBufferImpl::activate: Was activated already.");
				
				
				glBindBuffer(GL_ARRAY_BUFFER, m_id);

				m_activated = xdl_true;

				return ERR_OK;
			}

			virtual xdl_int deactivate() {
				assert(m_activated && "XdevLVertexBufferImpl::deactivate: Was not activated.");
				
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				m_activated = xdl_false;
				
				return ERR_OK;
			}

			virtual xdl_uint id() {
				return m_id;
			}

			virtual xdl_uint getSize() {
				return m_size;
			}


			GLuint 									m_id;
			xdl_uint 								m_size;
			xdl_bool								m_activated;
			xdl_bool								m_locked;
			xdl_bool								m_mapped;
	};


}


#endif
