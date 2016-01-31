#include "XdevLFrameBufferImpl.h"


namespace xdl {

	XdevLFrameBufferImpl::XdevLFrameBufferImpl() :
		m_id(0),
		m_width(0),
		m_height(0),
		m_size(0),
		m_inUse(xdl_false) {
		m_colorTargetTextures.reserve(4);
		m_colorTargetTextures.resize(4);
		m_activeColorTargetList.reserve(4);
		m_activeColorTargetList.resize(4);

		std::vector<xdl_uint>::iterator ib(m_activeColorTargetList.begin());
		while(ib != m_activeColorTargetList.end()) {
			(*ib) = GL_NONE;
			ib++;
		}
	}

	XdevLFrameBufferImpl::~XdevLFrameBufferImpl() {
		if(glIsFramebuffer(m_id)) {
			glDeleteFramebuffers(1, &m_id);
		}
	}

	xdl_int XdevLFrameBufferImpl::init(xdl_uint width, xdl_uint height) {
		m_width 	= width;
		m_height 	= height;

		// Create the framebuffer object and make it current.
		glGenFramebuffers(1, &m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		// Switch off all color targets.
		xdl::xdl_uint list[] = {GL_NONE};
		glDrawBuffers(1, (const GLenum*)list);

		// Unbind this framebuffer object to make sure that nothing will be done accidentally.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return ERR_OK;
	}

	xdl_int XdevLFrameBufferImpl::addColorTarget(xdl_uint target_index, XdevLFrameBufferColorFormat internal_format) {

		// Bind the framebuffer.
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		// Create the texture that is going to be used a color target.
		GLuint id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0, GL_RED,  GL_UNSIGNED_BYTE, nullptr);

		// Attach the created texture to the framebuffer.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + target_index, GL_TEXTURE_2D, id, 0);

		// Check if the framebuffer was build correct.
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(GL_FRAMEBUFFER_COMPLETE != status) {
			framebufferErrorAsString(status);
			return ERR_ERROR;
		}

		// Unbind framebuffer and texture object to make sure that nothing will be done accidentally.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_colorTargetTextures[target_index] = std::shared_ptr<XdevLTextureImpl>(new XdevLTextureImpl(id, m_width, m_height));

		m_size++;
		return ERR_OK;
	}

	xdl_int  XdevLFrameBufferImpl::addColorTarget(xdl_uint target_index, IPXdevLTexture texture) {

		// Bind the framebuffer.
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		// We use glFramebufferTexture2D wich means we don't have to bind the texture object itself.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + target_index, GL_TEXTURE_2D, texture->id(), 0);

		// Check if the framebuffer was build correct.
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(GL_FRAMEBUFFER_COMPLETE != status) {
			framebufferErrorAsString(status);
			return ERR_ERROR;
		}

		// Unbind framebuffer object to make sure that nothing will be done accidentally.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_colorTargetTextures[target_index] = texture;

		m_size++;
		return ERR_OK;
	}

	xdl_int  XdevLFrameBufferImpl::addColorTarget(xdl_uint target_index, IPXdevLTextureCube textureCube) {

		// Bind the framebuffer.
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		// We use glFramebufferTexture2D wich means we don't have to bind the texture object itself.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + target_index, GL_TEXTURE_CUBE_MAP_POSITIVE_X, textureCube->id(), 0);

		// Check if the framebuffer was build correct.
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(GL_FRAMEBUFFER_COMPLETE != status) {
			framebufferErrorAsString(status);
			return ERR_ERROR;
		}

		// Unbind framebuffer object to make sure that nothing will be done accidentally.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_textureCube = textureCube;

		m_size++;
		return ERR_OK;
	}

	xdl_int XdevLFrameBufferImpl::addDepthTarget(XdevLFrameBufferDepthFormat internal_format) {

		// Bind the framebuffer.
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		// Create depth texture.
		GLuint id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0, GL_DEPTH_COMPONENT,  GL_UNSIGNED_BYTE, NULL);

		// Attach it to the framebuffer.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id, 0);

		// Check if the framebuffer was build correct.
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(GL_FRAMEBUFFER_COMPLETE != status) {
			framebufferErrorAsString(status);
			return ERR_ERROR;
		}

		// Unbind framebuffer and texture object to make sure that nothing will be done accidentally.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_depthTexture = std::shared_ptr<XdevLTextureImpl>(new XdevLTextureImpl(id, m_width, m_height));

		return ERR_OK;
	}

	xdl_int XdevLFrameBufferImpl::addDepthTarget(IPXdevLTexture texture) {

		// Bind the framebuffer.
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->id(), 0);

		// Check if the framebuffer was build correct.
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(GL_FRAMEBUFFER_COMPLETE != status) {
			framebufferErrorAsString(status);
			return ERR_ERROR;
		}

		// Unbind framebuffer object to make sure that nothing will be done accidentally.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_depthTexture = std::shared_ptr<XdevLTextureImpl>(new XdevLTextureImpl(texture->id(), texture->getWidth(), texture->getHeight()));

		return ERR_OK;
	}

	xdl_int XdevLFrameBufferImpl::addDepthStencilTarget(XdevLFrameBufferDepthStencilFormat internal_format) {

		// Bind the framebuffer.
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		// Create the depth/stencil texture.
		GLuint id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0, GL_DEPTH_STENCIL,  GL_UNSIGNED_BYTE, NULL);

		// Attach them to the framebuffer.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 		id, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 	id, 0);

		// Check if the framebuffer was build correct.
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(GL_FRAMEBUFFER_COMPLETE != status) {
			framebufferErrorAsString(status);
			return ERR_ERROR;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_depthTexture = std::shared_ptr<XdevLTextureImpl>(new XdevLTextureImpl(id, m_width, m_height));

		return ERR_OK;
	}

	xdl_int XdevLFrameBufferImpl::activateColorTargets(xdl_uint numberOfTargets, xdl_uint* targetList) {
		assert(m_inUse && "XdevLFrameBufferImpl::draw: Frame Buffer is not activated.");
		glDrawBuffers(numberOfTargets, targetList);
		return ERR_OK;
	}

	xdl_int  XdevLFrameBufferImpl::activateDepthTarget(xdl_bool state) {
		if(state == xdl_true) {
			glEnable(GL_DEPTH_TEST);
		} else {
			glDisable(GL_DEPTH_TEST);
		}
		return ERR_OK;
	}

	xdl_int XdevLFrameBufferImpl::activateColorTargetCubePosition(xdl_uint target_index, XdevLCubemapPosition cubemapPosition) {
		assert(m_inUse && "XdevLFrameBufferImpl::draw: Framebuffer not activated.");
		assert(m_textureCube && "XdevLFrameBufferImpl::activateColorTargetCube: Cube Texture not added.");

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + target_index, cubemapPosition, m_textureCube->id(), 0);
		return ERR_OK;
	}

	xdl_int  XdevLFrameBufferImpl::clearColorTargets(xdl_float r, xdl_float g, xdl_float b, xdl_float a) {
		glClearColor(r,g,b,a);
		glClear(GL_COLOR_BUFFER_BIT);
		return ERR_OK;
	}

	xdl_int  XdevLFrameBufferImpl::clearDepthTarget(xdl_float clear_value) {
		glClearDepth(clear_value);
		glClear(GL_DEPTH_BUFFER_BIT);
		return ERR_OK;
	}

	xdl_int XdevLFrameBufferImpl::activateStencilTarget(xdl_bool state) {
		if(state == xdl_true) {
			glEnable(GL_STENCIL_TEST);
		} else {
			glDisable(GL_STENCIL_TEST);
		}
		return ERR_OK;
	}

	xdl_int XdevLFrameBufferImpl::clearStencilTarget(xdl_int clear_value) {
		glClearStencil(clear_value);
		glClear(GL_STENCIL_BUFFER_BIT);
		return ERR_OK;
	}

	xdl_int XdevLFrameBufferImpl::activate()  {
		// Activate for read and write.
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
		glViewport(0, 0, m_width, m_height);
		m_inUse = xdl_true;
		return ERR_OK;
	}

	xdl_int XdevLFrameBufferImpl::deactivate()  {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
		glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		m_inUse = xdl_false;
		return ERR_OK;
	}

	xdl_uint XdevLFrameBufferImpl::getWidth() {
		return m_width;
	}

	xdl_uint XdevLFrameBufferImpl::getHeight() {
		return m_height;
	}

	xdl_uint XdevLFrameBufferImpl::getNumColorTextures() {
		return m_size;
	}

	xdl_uint XdevLFrameBufferImpl::id() {
		return m_id;
	}

	IPXdevLTexture XdevLFrameBufferImpl::getTexture(xdl_uint idx) {
		auto texture = m_colorTargetTextures.at(idx);
		return texture;
	}

	IPXdevLTexture XdevLFrameBufferImpl::getDepthTexture() {
		return m_depthTexture;
	}

	void XdevLFrameBufferImpl::blit(xdl_int x, xdl_int y, xdl_uint width, xdl_uint height) {
		glBlitFramebuffer(0, 0, m_width, m_height, x, y, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	void XdevLFrameBufferImpl::framebufferErrorAsString(GLenum status) {
		switch(status) {
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: {
				XDEVL_MODULEX_ERROR(XdevLFrameBufferImpl, "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: Not all framebuffer attachment points are framebuffer attachment complete" << std::endl);
			}
			break;
//				case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS: {
//					XDEVL_MODULEX_ERROR(XdevLFrameBufferImpl, "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:Not all attached images have the same width and height." << std::endl);
//				}
//				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: {
				XDEVL_MODULEX_ERROR(XdevLFrameBufferImpl, "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: No images are attached to the framebuffer." << std::endl);
			}
			break;
			case GL_FRAMEBUFFER_UNSUPPORTED: {
				XDEVL_MODULEX_ERROR(XdevLFrameBufferImpl, "GL_FRAMEBUFFER_UNSUPPORTED: The combination of internal formats of the attached images violates an implementation-dependent set of restrictions." << std::endl);
			}
			break;
		}
	}
}
