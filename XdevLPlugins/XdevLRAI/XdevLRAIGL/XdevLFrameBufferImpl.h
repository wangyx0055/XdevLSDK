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

#ifndef XDEVL_FRAMEBUFFER_OBJECT_IMPL_H
#define XDEVL_FRAMEBUFFER_OBJECT_IMPL_H

#include "XdevLRAIGL.h"
#include <XdevLRAI/XdevLFrameBuffer.h>
#include "XdevLTextureImpl.h"
#include "XdevLTextureCubeImpl.h"

namespace xdl {

	class XdevLFrameBufferImpl : public XdevLFrameBuffer {
		public:
			XdevLFrameBufferImpl();
			virtual ~XdevLFrameBufferImpl();
			virtual xdl_int init(xdl_uint width, xdl_uint height) override;
			virtual xdl_int addColorTarget(xdl_uint target_index, XdevLFrameBufferColorFormat internal_format) override;
			virtual xdl_int addColorTarget(xdl_uint target_index, IPXdevLTexture texture) override;
			virtual xdl_int addColorTarget(xdl_uint target_index, IPXdevLTextureCube textureCube) override;
			virtual xdl_int addDepthTarget(XdevLFrameBufferDepthFormat internal_format) override;
			virtual xdl_int addDepthTarget(IPXdevLTexture texture) override;
			virtual xdl_int addDepthStencilTarget(XdevLFrameBufferDepthStencilFormat internal_format) override;
			virtual xdl_int activate() override;
			virtual xdl_int deactivate() override;
			virtual xdl_int activateColorTargets(xdl_uint numberOfTargets, xdl_uint* targetList) override;
			virtual xdl_int activateColorTargetCubePosition(xdl_uint target_index, XdevLCubemapPosition cubemapPosition) override;
			virtual xdl_int setActiveDepthTest(xdl_bool state) override;
			virtual xdl_int activateStencilTarget(xdl_bool state) override;
			virtual xdl_int clearColorTargets(xdl_float r, xdl_float g, xdl_float b, xdl_float a) override;
			virtual xdl_int clearDepthTarget(xdl_float clear_value) override;
			virtual xdl_int clearStencilTarget(xdl_int clear_value) override;
			virtual xdl_uint getWidth() override;
			virtual xdl_uint getHeight() override;
			virtual xdl_uint getNumColorTextures() override;
			virtual IPXdevLTexture getTexture(xdl_uint idx) override;
			virtual IPXdevLTexture getDepthTexture() override;
			virtual xdl_uint id() override;
			virtual void blit(XdevLFrameBuffer* framebuffer, XdevLFrameBufferColorTargets colortarget = XDEVL_COLOR_TARGET0) override;

		private:

			void framebufferErrorAsString(GLenum status);

		private:

			xdl_uint m_id;
			xdl_uint m_width;
			xdl_uint m_height;
			xdl_uint m_size;
			xdl_bool m_inUse;
			GLint m_previousViewport[4];
			IPXdevLTextureCube m_textureCube;
			IPXdevLTexture m_depthTexture;
			std::vector<IPXdevLTexture>		m_colorTargetTextures;
			std::vector<xdl::xdl_uint>		m_activeColorTargetList;

	};

}

#endif
