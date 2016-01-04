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

#ifndef XDEVL_TEXTURE_IMPL_H
#define XDEVL_TEXTURE_IMPL_H

namespace xdl {

	class XdevLTextureImpl: public XdevLTexture {
		public:
			XdevLTextureImpl() : m_id(0), m_initialized(xdl_false), m_idx(0), m_lock(xdl_false), m_width(0), m_height(0) {}

			virtual ~XdevLTextureImpl() {
				if(m_initialized) {
					glDeleteTextures(1, &m_id);
				}
			}

			virtual xdl_int init(xdl_uint width, xdl_uint height, xdl_uint internalFormat) {
				glGenTextures(1, &m_id);
				glBindTexture(GL_TEXTURE_2D, m_id);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				GLuint format;
				switch(internalFormat) {
				case GL_DEPTH_COMPONENT:
				case GL_DEPTH_COMPONENT16:
				case GL_DEPTH_COMPONENT24:
				case GL_DEPTH_COMPONENT32:
				case GL_DEPTH_COMPONENT32F:
					format = GL_DEPTH_COMPONENT;
					break;
				case GL_DEPTH_STENCIL:
				case XDEVL_DEPTH24_STENCIL8:
				case XDEVL_DEPTH32F_STENCIL8:
					format = GL_DEPTH_STENCIL;
					break;
				default:
					format = GL_RED;
					break;
				}

				glTexImage2D(GL_TEXTURE_2D,
				             0,
				             internalFormat,
				             width,
				             height,
				             0,
				             format,
				             GL_UNSIGNED_BYTE,
				             NULL);

				glBindTexture(GL_TEXTURE_2D, 0);
				m_initialized = xdl_true;
				m_width = width;
				m_height = height;
				return ERR_OK;
			}

			virtual xdl_int init(xdl_uint width, xdl_uint height, xdl_uint internalFormat, xdl_uint imageFormat, xdl_uint8* data) {
				glGenTextures(1, &m_id);
				glBindTexture(GL_TEXTURE_2D, m_id);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

				GLuint format = imageFormat;
				switch(internalFormat) {
				case GL_DEPTH_COMPONENT:
				case GL_DEPTH_COMPONENT16:
				case GL_DEPTH_COMPONENT24:
				case GL_DEPTH_COMPONENT32:
				case GL_DEPTH_COMPONENT32F:
					format = GL_DEPTH_COMPONENT;
					break;
				case GL_DEPTH_STENCIL:
				case XDEVL_DEPTH24_STENCIL8:
					format = GL_DEPTH_STENCIL;
					break;
				default:
					format = GL_RED;
					break;
				}

				glTexImage2D(GL_TEXTURE_2D,
				             0,
				             internalFormat,
				             width,
				             height,
				             0,
				             imageFormat,
				             GL_UNSIGNED_BYTE,
				             (GLvoid*)data);

				glBindTexture(GL_TEXTURE_2D, 0);
				m_initialized = xdl_true;
				m_width = width;
				m_height = height;
				return ERR_OK;
			}

			virtual xdl_int setTextureFilter(XdevLTextureFilterStage filterType, XdevLTextureFilter filter) {
				assert(m_lock && "XdevLTextureImpl::lock: Texture was not locked.");

				glTexParameteri(GL_TEXTURE_2D, filterType, filter);
				glTexParameteri(GL_TEXTURE_2D, filterType, filter);
				return ERR_OK;
			}

			virtual xdl_int setTextureWrap(XdevLTextureCoord texCoord, XdevLTextureWrap textureWrap) {
				assert(m_lock && "XdevLTextureImpl::lock: Texture was not locked.");

				glTexParameteri(GL_TEXTURE_2D, texCoord, textureWrap);
				return ERR_OK;
			}

			virtual xdl_int setTextureMaxAnisotropy(xdl_float value) {
				assert(m_lock && "XdevLTextureImpl::lock: Texture was not locked.");

				GLfloat largest;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
				if(value < largest) {
					largest = value;
				}
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
				return ERR_OK;
			}

			virtual xdl_int generateMipMap() {
				glGenerateMipmap(GL_TEXTURE_2D);
				return ERR_OK;
			}

			virtual xdl_int activate(xdl_uint idx) {
				assert(m_initialized && "XdevLTextureImpl::use: Texture was not initialized with init().");

				glActiveTexture(GL_TEXTURE0 + idx);
				glBindTexture(GL_TEXTURE_2D, m_id);
				m_idx = idx;
				return ERR_OK;
			}

			virtual xdl_int deactivate() {
				assert(m_initialized && "XdevLTextureImpl::use: Texture was not initialized with init().");

				glActiveTexture(GL_TEXTURE0 + m_idx);
				glBindTexture(GL_TEXTURE_2D, 0);
				return ERR_OK;
			}

			virtual xdl_int lock() {
				assert(!m_lock && "XdevLTextureImpl::lock: Texture was already locked.");

				glBindTexture(GL_TEXTURE_2D, m_id);
				m_lock = xdl_true;
				return ERR_OK;
			}

			virtual xdl_int unlock() {
				assert(m_lock && "XdevLTextureImpl::lock: Texture was not locked.");

				glBindTexture(GL_TEXTURE_2D, 0);
				m_lock = xdl_false;
				return ERR_OK;
			}

			virtual xdl_uint id() {
				return m_id;
			};

			virtual void id(xdl_uint id) {
				m_id = id;
			}
			virtual xdl_uint getWidth() {
				return m_width;
			}

			virtual xdl_uint getHeight() {
				return m_height;
			}

			void setInitialized(xdl_bool state) {
				m_initialized = state;
			}

			GLuint m_id;
			xdl_bool m_initialized;
			xdl_uint m_idx;
			xdl_bool m_lock;
			xdl_uint m_width;
			xdl_uint m_height;
	};
}

#endif
