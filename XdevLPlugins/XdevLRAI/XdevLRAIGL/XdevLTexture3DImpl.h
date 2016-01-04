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

#ifndef XDEVL_TEXTURE_3D_IMPL_H
#define XDEVL_TEXTURE_3D_IMPL_H

namespace xdl {

	class XdevLTexture3DImpl: public XdevLTexture3D {
		public:
			XdevLTexture3DImpl() : m_id(0), m_initialized(xdl_false), m_idx(0), m_lock(xdl_false), m_width(0), m_height(0) {}

			virtual ~XdevLTexture3DImpl() {
				if(m_initialized) {
					glDeleteTextures(1, &m_id);
				}
			}

			virtual xdl_int init(xdl_uint width, xdl_uint height, xdl_uint depth, xdl_uint internalFormat) {
				glGenTextures(1, &m_id);
				glBindTexture(GL_TEXTURE_3D, m_id);
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexImage3D(GL_TEXTURE_3D,			// Texture type.
				             0,							    // MipMap Level.
				             internalFormat,		// Internal Format.
				             width,						  // Width.
				             height,						// Height.
				             depth,
				             0,							    // Must be zero.
				             GL_RED,
				             GL_UNSIGNED_BYTE,
				             NULL);

				glBindTexture(GL_TEXTURE_3D, 0);
				m_initialized = xdl_true;
				m_width   = width;
				m_height  = height;
				m_depth   = depth;
				return ERR_OK;
			}

			virtual xdl_int init(xdl_uint width, xdl_uint height, xdl_uint depth, xdl_uint internalFormat, xdl_uint imageFormat, xdl_uint8* data) {
				glGenTextures(1, &m_id);
				glBindTexture(GL_TEXTURE_3D, m_id);
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexImage3D(GL_TEXTURE_3D,			// Texture type.
				             0,							    // MipMap Level.
				             internalFormat,		// Internal Format.
				             width,						  // Width.
				             height,						// Height.
				             depth,
				             0,							    // Must be zero.
				             imageFormat,
				             GL_UNSIGNED_BYTE,
				             (GLvoid*)data);

				glBindTexture(GL_TEXTURE_3D, 0);
				m_initialized = xdl_true;
				m_width   = width;
				m_height  = height;
				m_depth   = depth;
				return ERR_OK;
			}

			virtual xdl_int setTextureFilter(XdevLTextureFilterStage filterType, XdevLTextureFilter filter) {
				assert(m_lock && "XdevLTextureImpl::lock: Texture was not locked.");

				glTexParameteri(GL_TEXTURE_3D, filterType, filter);
				glTexParameteri(GL_TEXTURE_3D, filterType, filter);
				return ERR_OK;
			}

			virtual xdl_int setTextureWrap(XdevLTextureCoord texCoord, XdevLTextureWrap textureWrap) {
				assert(m_lock && "XdevLTextureImpl::lock: Texture was not locked.");

				glTexParameteri(GL_TEXTURE_3D, texCoord, textureWrap);
				return ERR_OK;
			}

			virtual xdl_int setTextureMaxAnisotropy(xdl_float value) {
				assert(m_lock && "XdevLTextureImpl::lock: Texture was not locked.");

				GLfloat largest;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
				if(value < largest) {
					largest = value;
				}
				glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
				return ERR_OK;
			}

      virtual xdl_int generateMipMap() {
        glGenerateMipmap(GL_TEXTURE_3D);
        return ERR_OK;
      }

			virtual xdl_int activate(xdl_uint idx) {
				assert(m_initialized && "XdevLTextureImpl::use: Texture was not initialized with init().");

				glActiveTexture(GL_TEXTURE0 + idx);
				glBindTexture(GL_TEXTURE_3D, m_id);
				m_idx = idx;
				return ERR_OK;
			}

			virtual xdl_int deactivate() {
				assert(m_initialized && "XdevLTextureImpl::use: Texture was not initialized with init().");

				glActiveTexture(GL_TEXTURE0 + m_idx);
				glBindTexture(GL_TEXTURE_3D, 0);
				return ERR_OK;
			}

			virtual xdl_int lock() {
				assert(!m_lock && "XdevLTextureImpl::lock: Texture was already locked.");

				glBindTexture(GL_TEXTURE_3D, m_id);
				m_lock = xdl_true;
				return ERR_OK;
			}

			virtual xdl_int unlock() {
				assert(m_lock && "XdevLTextureImpl::lock: Texture was not locked.");

				glBindTexture(GL_TEXTURE_3D, 0);
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

    virtual xdl_uint getDepth() {
      return m_depth;
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
			xdl_uint m_depth;
	};
}

#endif
