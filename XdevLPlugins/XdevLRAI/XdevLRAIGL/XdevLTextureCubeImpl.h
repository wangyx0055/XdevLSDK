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

#ifndef XDEVL_TEXTURE_CUBE_IMPL_H
#define XDEVL_TEXTURE_CUBE_IMPL_H

#include <XdevLRAI/XdevLTextureCube.h>

namespace xdl {

	/**
		@class XdevLTextureCubeImpl
		@brief A cube texture implementation.
	*/
	class XdevLTextureCubeImpl : public XdevLTextureCube {
		public:

			XdevLTextureCubeImpl() : 	m_id(0),
				m_width(0),
				m_height(0),
				m_idx(0),
				m_locked(xdl_false),
				m_initialized(xdl_false) {

			}

			virtual ~XdevLTextureCubeImpl() {
				if(m_initialized) {
					glDeleteTextures(1, &m_id);
				}
			}

			virtual xdl_int init(xdl_uint width, xdl_uint height, xdl_uint internalFormat, xdl_uint imageFormat, XdevLCubemapPosition cubemapPositionList[], xdl_uint8* data[]) {
				glGenTextures(1, &m_id);

				glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0); 
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);


				glTexImage2D(cubemapPositionList[0],0,internalFormat,width,height,0,imageFormat,GL_UNSIGNED_BYTE,data[0]);
				glTexImage2D(cubemapPositionList[1],0,internalFormat,width,height,0,imageFormat,GL_UNSIGNED_BYTE,data[1]);
				glTexImage2D(cubemapPositionList[2],0,internalFormat,width,height,0,imageFormat,GL_UNSIGNED_BYTE,data[2]);
				glTexImage2D(cubemapPositionList[3],0,internalFormat,width,height,0,imageFormat,GL_UNSIGNED_BYTE,data[3]);
				glTexImage2D(cubemapPositionList[4],0,internalFormat,width,height,0,imageFormat,GL_UNSIGNED_BYTE,data[4]);
				glTexImage2D(cubemapPositionList[5],0,internalFormat,width,height,0,imageFormat,GL_UNSIGNED_BYTE,data[5]);

				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

				m_width = width;
				m_height = height;
				m_initialized = xdl_true;

				return ERR_OK;
			}
			
			virtual xdl_int upload(XdevLCubemapPosition cubeMapPosition, xdl_uint internalFormat, xdl_uint width, xdl_uint height,  xdl_uint imageFormat, xdl_uint8* data) {
				assert(m_locked && "XdevLTextureImpl::lock: Texture was not locked.");
				
				glTexImage2D(cubeMapPosition,0,internalFormat,width,height,0,imageFormat,GL_UNSIGNED_BYTE,data);
				return ERR_OK;
			}
			
			virtual xdl_int generateMipMap() {
				assert(m_locked && "XdevLTextureImpl::lock: Texture was not locked.");
				glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
				return ERR_OK;
			}

			virtual xdl_int activate(xdl_uint idx) {
				assert(m_initialized && "XdevLTextureImpl::use: Texture was not initialized with init().");

				glActiveTexture(GL_TEXTURE0 + idx);
				glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
				m_idx = idx;
				return ERR_OK;
			}

			virtual xdl_int deactivate() {
				assert(m_initialized && "XdevLTextureImpl::use: Texture was not initialized with init().");

				glActiveTexture(GL_TEXTURE0 + m_idx);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
				return ERR_OK;
			}

			virtual xdl_int lock() {
				assert(!m_locked && "XdevLTextureImpl::lock: Texture was already locked.");

				glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
				m_locked = xdl_true;
				return ERR_OK;
			}

			virtual xdl_int unlock() {
				assert(m_locked && "XdevLTextureImpl::lock: Texture was not locked.");

				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
				m_locked = xdl_false;
				return ERR_OK;
			}

			virtual xdl_uint id() {
				return m_id;
			}

		private:

			GLuint 		m_id;
			xdl_uint	m_width;
			xdl_uint 	m_height;
			xdl_uint	m_idx;
			xdl_uint	m_locked;
			xdl_bool	m_initialized;

	};

}

#endif
