#include "XdevLFrameBufferImpl.h"


namespace xdl {


	XdevLFrameBufferImpl::XdevLFrameBufferImpl() :
		m_id(0),
		m_width(0),
		m_height(0),
		m_size(0),
		m_inUse(xdl_false){
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
		glDeleteFramebuffers(1, &m_id);
	}

	xdl_int XdevLFrameBufferImpl::init(xdl_uint width,
	                                   xdl_uint height) {
		m_width 	= width;
		m_height 	= height;

		// Create the framebuffer object and make it current.
		glGenFramebuffers(1, &m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		// Switch off all color targets.
		xdl::xdl_uint list[] = {GL_NONE};
		glDrawBuffers(1, (const GLenum*)list);

		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, width, height, false);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		return ERR_OK;
	}

	xdl_int XdevLFrameBufferImpl::addColorTarget(xdl_uint target_index, XdevLFrameBufferColorFormat internal_format) {

		// Bind the framebuffer.
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		GLuint id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0, GL_RED,  GL_UNSIGNED_BYTE, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + target_index, GL_TEXTURE_2D, id, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_colorTargetTextures[target_index] = std::shared_ptr<XdevLTextureImpl>(new XdevLTextureImpl(id, m_width, m_height));

		// Check if the Framebuffer was build correct.
		GLenum status;
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(status != GL_FRAMEBUFFER_COMPLETE) {
			return ERR_ERROR;
		}

		m_size++;
		return ERR_OK;
	}

	xdl_int  XdevLFrameBufferImpl::addColorTarget(xdl_uint target_index, IPXdevLTexture texture) {


		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
		glBindTexture(GL_TEXTURE_2D, texture->id());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + target_index, GL_TEXTURE_2D, texture->id(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_colorTargetTextures[target_index] = texture;

		// Check if the Framebuffer was build correct.
		GLenum status;
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(status != GL_FRAMEBUFFER_COMPLETE) {
			return ERR_ERROR;
		}

		m_size++;
		return ERR_OK;
	}

	xdl_int  XdevLFrameBufferImpl::addColorTarget(xdl_uint target_index, IPXdevLTextureCube textureCube) {

		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureCube->id());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + target_index, GL_TEXTURE_CUBE_MAP_POSITIVE_X, textureCube->id(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		m_textureCube = textureCube;

		// Check if the Framebuffer was build correct.
		GLenum status;
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(status != GL_FRAMEBUFFER_COMPLETE) {
			return ERR_ERROR;
		}

		m_size++;
		return ERR_OK;
	}

	xdl_int XdevLFrameBufferImpl::addDepthTarget(XdevLFrameBufferDepthFormat internal_format) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		GLuint id;
		glGenTextures(1, &id);

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0, GL_DEPTH_COMPONENT,  GL_UNSIGNED_BYTE, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_depthTexture = std::shared_ptr<XdevLTextureImpl>(new XdevLTextureImpl(id, m_width, m_height));

		return ERR_OK;
	}

	xdl_int XdevLFrameBufferImpl::addDepthTarget(IPXdevLTexture texture) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		glBindTexture(GL_TEXTURE_2D, texture->id());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->id(), 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_depthTexture = std::shared_ptr<XdevLTextureImpl>(new XdevLTextureImpl(texture->id(), texture->getWidth(), texture->getHeight()));

		return ERR_OK;
	}

	xdl_int XdevLFrameBufferImpl::addDepthStencilTarget(XdevLFrameBufferDepthStencilFormat internal_format) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		GLuint id;
		glGenTextures(1, &id);

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0, GL_DEPTH_STENCIL,  GL_UNSIGNED_BYTE, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 		id, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 	id, 0);

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

}
