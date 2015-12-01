#ifndef XDEVL_FRAMEBUFFER_OBJECT_IMPL_H
#define XDEVL_FRAMEBUFFER_OBJECT_IMPL_H

#include <XdevLOpenGL/XdevLOpenGLImpl.h>
#include <XdevLOpenGL/XdevLFrameBuffer.h>
#include <XdevLTextureImpl.h>
#include <XdevLTextureCubeImpl.h>

namespace xdl {

	class XdevLFrameBufferImpl : public XdevLFrameBuffer {
	public:
		XdevLFrameBufferImpl();
		virtual ~XdevLFrameBufferImpl();
		virtual xdl_int init(	xdl_uint width,
													xdl_uint height);
		virtual xdl_int addColorTarget(xdl_uint target_index, XdevLFrameBufferColorFormat internal_format);
		virtual xdl_int addColorTarget(xdl_uint target_index, XdevLTexture* texture);
		virtual xdl_int addColorTarget(xdl_uint target_index, XdevLTextureCube* textureCube);
		virtual xdl_int addDepthTarget(XdevLFrameBufferDepthFormat internal_format);
		virtual xdl_int addDepthTarget(XdevLTexture* texture);		
		virtual xdl_int addDepthStencilTarget(XdevLFrameBufferDepthStencilFormat internal_format);
		virtual xdl_int activate();		
		virtual xdl_int deactivate();
		virtual xdl_int activateColorTargets(xdl_uint numberOfTargets, xdl_uint* targetList);
		virtual xdl_int activateColorTargetCubePosition(xdl_uint target_index, XdevLCubemapPosition cubemapPosition);		
		virtual xdl_int activateDepthTarget(xdl_bool state);
		virtual xdl_int activateStencilTarget(xdl_bool state);
		virtual xdl_int clearColorTargets(xdl_float r, xdl_float g, xdl_float b, xdl_float a);
		virtual xdl_int clearDepthTarget(xdl_float clear_value);
		virtual xdl_int clearStencilTarget(xdl_int clear_value);
		virtual xdl_uint getWidth();
		virtual xdl_uint getHeight();
		virtual xdl_uint getNumColorTextures();
		virtual xdl_uint id();
		virtual XdevLTexture* getTexture(xdl_uint idx);
		virtual XdevLTexture* getDepthTexture();

		xdl_uint m_id;
		xdl_uint m_size;
		std::vector<XdevLTextureImpl*> 	m_colorTargetTextures;
		XdevLTextureCubeImpl*						m_textureCube;
		std::vector<xdl::xdl_uint>      m_activeColorTargetList;
		XdevLTextureImpl m_depthTexture;
		xdl_bool m_inUse;
		xdl_uint m_width;
		xdl_uint m_height;
	};

}

#endif
