#include <XdevLCoreMediator.h>
#include <XdevLWindow/XdevLWindow.h>
#include "XdevLFrameBufferImpl.h"
#include "XdevLOpenGLImpl.h"
#include "XdevLVertexBufferImpl.h"
#include "XdevLIndexBufferImpl.h"
#include "XdevLVertexArrayImpl.h"
#include "XdevLShaderProgramImpl.h"
#include "XdevLShaderImpl.h"
#include "XdevLTextureCubeImpl.h"
#include "XdevLTexture3DImpl.h"
#include "XdevLOpenGLUtils.h"
#include <XdevLXstring.h>
#include <stdexcept>
#include <fstream>
#include <cmath>

class vertexArray;
class wrappPrimitiveType;


xdl::XdevLModuleDescriptor openGLModuleDesc {xdl::vendor,
    xdl::author,
    xdl::moduleNames[0],
    xdl::copyright,
    xdl::description,
    xdl::XdevLRAIGLMajorVersion,
    xdl::XdevLRAIGLMinorVersion,
    xdl::XdevLRAIGLPatchVersion
                                            };

xdl::XdevLPluginDescriptor m_openglDescriptor {	xdl::pluginName,
    xdl::moduleNames,
    xdl::XdevLRAIGLPluginMajorVersion,
    xdl::XdevLRAIGLPluginMinorVersion,
    xdl::XdevLRAIGLPluginPatchVersion
                                              };

extern "C" XDEVL_EXPORT xdl::xdl_int _init_plugin(xdl::XdevLPluginCreateParameter* parameter) {
	return xdl::ERR_OK;
}

extern "C" XDEVL_EXPORT xdl::xdl_int _shutdown_plugin() {
	return xdl::ERR_OK;
}

extern "C" XDEVL_EXPORT int _create(xdl::XdevLModuleCreateParameter* parameter) {
	// Create the "OpenGL" module.
	if(openGLModuleDesc.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLOpenGLImpl(parameter);

		if(!obj)
			return xdl::ERR_ERROR;

		parameter->setModuleInstance(obj);

		return xdl::ERR_OK;
	}

	return xdl::ERR_MODULE_NOT_FOUND;
}

extern "C" XDEVL_EXPORT void _delete(xdl::XdevLModule* obj) {
	if(obj)
		delete obj;
}

extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor() {
	return &m_openglDescriptor;
}

namespace xdl {

	XdevLOpenGLImpl::XdevLOpenGLImpl(XdevLModuleCreateParameter* parameter) : XdevLModuleImpl<XdevLRAI>(parameter, openGLModuleDesc),
		m_window(nullptr),
		m_gl_context(nullptr),
		m_StencilDepth(8),
		m_ColorDepth(32),
		m_ZDepth(24),
		m_VSync(false),
		m_fsaa(false),
		m_major(3),
		m_minor(2),
		mXDEVL_DEBUG(false),
		m_profile("core"),
		m_vertex_shader_version(330),
		m_fragment_shader_version(330),
		m_geometry_shader_version(330),
		m_activeFrameBuffer(nullptr),
		m_activeVertexArray(nullptr),
		m_activeShaderProgram(nullptr) {
	}

	XdevLOpenGLImpl::~XdevLOpenGLImpl() {
	}

	xdl_int XdevLOpenGLImpl::init() {
		TiXmlDocument xmlDocument;

		if(!xmlDocument.LoadFile(getMediator()->getXmlFilename())) {
			XDEVL_MODULE_ERROR("Could not parse xml file: " << getMediator()->getXmlFilename() << std::endl);
			return ERR_ERROR;
		}

		if(readModuleInformation(&xmlDocument) != ERR_OK) {
			return ERR_ERROR;
		}

		m_gl_context = static_cast<XdevLOpenGLContext*>(getMediator()->createModule(xdl::XdevLModuleName("XdevLOpenGLContext"), xdl::XdevLID("XdevLRAIOpenGLContext")));
		if(nullptr == m_gl_context) {
			XDEVL_MODULE_ERROR("Could not create OpenGL context.\n");
			return ERR_ERROR;
		}
		return ERR_OK;
	}

	int XdevLOpenGLImpl::create(XdevLWindow* window) {

		// Create OpenGL context.
		if(m_gl_context->create(window) != xdl::ERR_OK) {
			return ERR_ERROR;
		}
		m_gl_context->makeCurrent(window);

		XDEVL_MODULE_INFO(glGetString(GL_VENDOR) << std::endl);
		XDEVL_MODULE_INFO(glGetString(GL_RENDERER) << std::endl);
		XDEVL_MODULE_INFO(glGetString(GL_VERSION) << std::endl);


		if(initGLEW() != ERR_OK) {
			return ERR_ERROR;
		}

		XDEVL_MODULE_SUCCESS("OpenGL successfully initialized.\n")

		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::readModuleInformation(TiXmlDocument* document) {
		TiXmlHandle docHandle(document);
		TiXmlElement* root = docHandle.FirstChild(XdevLCorePropertiesName.c_str()).FirstChildElement("XdevLOpenGL").ToElement();

		if(!root) {
			XDEVL_MODULE_INFO("<XdevLOpenGL> section not found, skipping proccess.\n");
			return ERR_OK;
		}
		while(root != NULL) {
			if(root->Attribute("id")) {
				XdevLID id(root->Attribute("id"));
				if(getID() == id) {
					if(root->Attribute("framebuffer_depth")) {
						m_ColorDepth = xstd::from_string<xdl_int>(root->Attribute("framebuffer_depth"));
						XDEVL_MODULE_INFO("Framebuffer depth request: " << m_ColorDepth << std::endl);
					}
					if(root->Attribute("z_buffer_depth")) {
						m_ZDepth = xstd::from_string<xdl_int>(root->Attribute("z_buffer_depth"));
						XDEVL_MODULE_INFO("Z-Buffer depth request: " << m_ZDepth << std::endl);
					}
					if(root->Attribute("stencil_buffer_depth")) {
						m_StencilDepth = xstd::from_string<xdl_int>(root->Attribute("stencil_buffer_depth"));
						XDEVL_MODULE_INFO("Stencilbuffer depth request: " << m_StencilDepth << std::endl);
					}
					if(root->Attribute("vsync"))
						m_VSync = xstd::from_string<xdl_bool>(root->Attribute("vsync"));
					if(root->Attribute("fsaa"))
						m_fsaa = xstd::from_string<xdl_int>(root->Attribute("fsaa"));
					if(root->Attribute("major"))
						m_major = xstd::from_string<xdl_int>(root->Attribute("major"));
					if(root->Attribute("minor"))
						m_minor = xstd::from_string<xdl_int>(root->Attribute("minor"));
					if(root->Attribute("debug"))
						mXDEVL_DEBUG = xstd::from_string<xdl_bool>(root->Attribute("debug"));
					if(root->Attribute("profile"))
						m_profile = root->Attribute("profile");
				}
			} else
				XDEVL_MODULE_ERROR("No 'id' attribute specified. Using default values for the device\n");

			root = root->NextSiblingElement();
		}
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::initGLEW() {
		glewExperimental= GL_TRUE;
		GLenum err = glewInit();
		if(GLEW_OK != err) {
			XDEVL_MODULE_ERROR(glewGetErrorString(err) << std::endl);
			return ERR_ERROR;
		}

//		setVSync(m_VSync);

//		getOpenGLInfos();

		// Only use if the extenstion is available. In some cases this is not
		// supported so we will use this as a hack.
		if(GLEW_ARB_debug_output) {
			glDebugMessageCallback((GLDEBUGPROC)(openGLDebugOutput), nullptr);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		}

		return ERR_OK;
	}


	void XdevLOpenGLImpl::getOpenGLInfos() {
		std::string tmp;

		// The WGL_CONTEXT_CORE_PROFILE_BIT_ARB context profile doesn't support
		// glGetString(GL_EXTENSIONS) anymore. We have to do it in the propper way.
		if(m_profile == "core") {
			glGetIntegerv(GL_NUM_EXTENSIONS, &m_numExtensions);
			for(xdl_int idx = 0; idx < m_numExtensions; ++idx) {
				xdl_char* str_buffer = (xdl_char*)glGetStringi(GL_EXTENSIONS, idx);
				if(str_buffer != NULL)
					m_extensions.push_back((xdl_char*)glGetStringi(GL_EXTENSIONS, idx));
				else
					XDEVL_MODULE_INFO("GL_EXTENSIONS ERROR: " << idx << std::endl);


			}
		} else if(m_profile == "compatibility") {
			tmp = (xdl_char*)glGetString(GL_EXTENSIONS);
			m_numExtensions = xstd::tokenize(tmp, m_extensions, " ");
		}

		m_version 			= (xdl_char*)glGetString(GL_VERSION);
		m_vendor			=(xdl_char*)glGetString(GL_VENDOR);
	}

	const xdl_char* XdevLOpenGLImpl::getVersion() {
		return m_version.c_str();
	}

	const xdl_char* XdevLOpenGLImpl::getVendor() {
		return m_vendor.c_str();
	}

	xdl_int XdevLOpenGLImpl::getNumExtensions() const {
		return m_numExtensions;
	}

	const xdl_char* XdevLOpenGLImpl::getExtension(xdl_int idx) {
		const char* tmp = "";
		try {
			return m_extensions.at(idx).c_str();
		} catch(std::out_of_range& e) {
			XDEVL_MODULE_ERROR(e.what());
			return tmp;
		}
	}

	xdl_bool XdevLOpenGLImpl::extensionSupported(const xdl_char* extensionName) {
		for(xdl_int a = 0; a < getNumExtensions(); ++a) {
			if(m_extensions[a] == extensionName)
				return true;
		}
		return false;
	}

	const xdl_char* XdevLOpenGLImpl::getShaderVersion() {
		return (xdl_char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	}

	void XdevLOpenGLImpl::setPointSize(xdl_float size) {
		glPointSize(size);
	}

	void XdevLOpenGLImpl::setLineSize(xdl_float size) {
		glLineWidth(size);
	}

	void XdevLOpenGLImpl::setActiveDepthTest(xdl_bool enableDepthTest) {
		enableDepthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	}

	void XdevLOpenGLImpl::setActiveBlendMode(xdl_bool enableBlendMode) {
		enableBlendMode ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
	}

	GLint XdevLBlendModeToGLBlendMode(XdevLBlendModes blendMode) {
		switch(blendMode) {
			case XDEVL_BLEND_ZERO:
				return GL_ZERO;
			case XDEVL_BLEND_ONE:
				return GL_ONE;
			case XDEVL_BLEND_SRC_COLOR:
				return GL_SRC_COLOR;
			case XDEVL_BLEND_ONE_MINUS_SRC_COLOR:
				return GL_ONE_MINUS_SRC_COLOR;
			case XDEVL_BLEND_DST_COLOR:
				return GL_DST_COLOR;
			case XDEVL_BLEND_ONE_MINUS_DST_COLOR:
				return GL_ONE_MINUS_DST_COLOR;
			case XDEVL_BLEND_SRC_ALPHA:
				return GL_SRC_ALPHA;
			case XDEVL_BLEND_ONE_MINUS_SRC_ALPHA:
				return GL_ONE_MINUS_SRC_ALPHA;
			case XDEVL_BLEND_DST_ALPHA:
				return GL_DST_ALPHA;
			case XDEVL_BLEND_ONE_MINUS_DST_ALPHA:
				return GL_ONE_MINUS_DST_ALPHA;
			case XDEVL_BLEND_CONSTANT_COLOR:
				return GL_CONSTANT_COLOR;
			case XDEVL_BLEND_ONE_MINUS_CONSTANT_COLOR:
				return GL_ONE_MINUS_CONSTANT_COLOR;
			case XDEVL_BLEND_CONSTANT_ALPHA:
				return GL_CONSTANT_ALPHA;
			case XDEVL_BLEND_ONE_MINUS_CONSTANT_ALPHA:
				return GL_ONE_MINUS_CONSTANT_ALPHA;
			case XDEVL_BLEND_SRC_ALPHA_SATURATE:
				return GL_SRC_ALPHA_SATURATE;
			default:
				return GL_ZERO;
		};
	}

	xdl_int XdevLOpenGLImpl::setBlendMode(XdevLBlendModes src, XdevLBlendModes dst) {
		GLint blend_src = XdevLBlendModeToGLBlendMode(src);
		GLint blend_dst = XdevLBlendModeToGLBlendMode(dst);
		glBlendFunc(blend_src, blend_dst);
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::clearColorTargets(xdl_float r, xdl_float g, xdl_float b, xdl_float a) {
		glClearColor(r,g, b, a);
		glClear(GL_COLOR_BUFFER_BIT);
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::clearDepthTarget(xdl_float clear_value) {
		glClearDepth(clear_value);
		glClear(GL_DEPTH_BUFFER_BIT);
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::setViewport(xdl_float x, xdl_float y, xdl_float width, xdl_float height) {
		glViewport(x, y, width, height);
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::setActiveRenderWindow(XdevLWindow* window) {
		m_gl_context->makeCurrent(window);
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::swapBuffers() {
		m_gl_context->swapBuffers();
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::createVertexDeclaration(XdevLVertexDeclaration** vertexDeclaration) {
		XdevLVertexDeclaration* vd = new XdevLVertexDeclaration();
		*vertexDeclaration = vd;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::createVertexShader(XdevLVertexShader** vertex_shader) {
		XdevLOpenGLVertexShaderImpl* vs = new XdevLOpenGLVertexShaderImpl();
		*vertex_shader = vs;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::createFragmentShader(XdevLFragmentShader** fragment_shader) {

		XdevLOpenGLFragmentShaderImpl* fs = new XdevLOpenGLFragmentShaderImpl();
		*fragment_shader = fs;

		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::createGeometryShader(XdevLGeometryShader** geometry_shader) {
		XdevLOpenGLGeometryShaderImpl* fs = new XdevLOpenGLGeometryShaderImpl();
		*geometry_shader = fs;
		return ERR_OK;;
	}


	xdl_int XdevLOpenGLImpl::createShaderProgram(XdevLShaderProgram** program) {
		XdevLOpenGLProgramImpl* prg = new XdevLOpenGLProgramImpl();
		*program = prg;
		return ERR_OK;
	}


	void XdevLOpenGLImpl::shaderLog(xdl_uint shaderId) {
		// Get the length of the message.
		GLint Len;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &Len);

		GLsizei ActualLen;
		GLchar *Log = new GLchar[Len];
		glGetShaderInfoLog(shaderId, Len, &ActualLen, Log);
		XDEVL_MODULE_ERROR(Log << "\n");
		delete [] Log;
	}


	xdl_int XdevLOpenGLImpl::createTexture(XdevLTexture** texture) {
		XdevLTextureImpl* tmp = new XdevLTextureImpl();
		*texture = tmp;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::createTextureCube(XdevLTextureCube** textureCube) {
		XdevLTextureCubeImpl* tmp = new XdevLTextureCubeImpl();
		*textureCube = tmp;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::createTexture3D(XdevLTexture3D** texture3D) {
		XdevLTexture3DImpl* tmp = new XdevLTexture3DImpl();
		*texture3D = tmp;
		return ERR_OK;
	}


	xdl_int XdevLOpenGLImpl::createFrameBuffer(XdevLFrameBuffer** fbo) {

		XdevLFrameBufferImpl* tmp = new XdevLFrameBufferImpl();

		// Return back the framebuffer.
		*fbo = tmp;

		XDEVL_MODULE_SUCCESS("Framebuffer object created successfully.\n");
		return ERR_OK;
	}


	xdl_int XdevLOpenGLImpl::createVertexArray(XdevLVertexArray** vertexArray) {
		XdevLVertexArrayImpl* vba = new XdevLVertexArrayImpl();
		*vertexArray = vba;

		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::createVertexBuffer(XdevLVertexBuffer** vertexBuffer) {

		XdevLVertexBufferImpl* vbo = new XdevLVertexBufferImpl();

		*vertexBuffer = vbo;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::createIndexBuffer(XdevLIndexBuffer** indexBuffer) {

		XdevLIndexBufferImpl* ibo = new XdevLIndexBufferImpl();

		*indexBuffer = ibo;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::setActiveFrameBuffer(XdevLFrameBuffer* frambuffer) {
		assert(frambuffer && "XdevLOpenGLImpl::setActiveFrameBuffer: No valid Framebuffer specified.");
		frambuffer->activate();
		m_activeFrameBuffer = frambuffer;
		return ERR_OK;
	}


	xdl_int XdevLOpenGLImpl::setActiveVertexArray(XdevLVertexArray* vertexArray) {
		assert(vertexArray && "XdevLOpenGLImpl::setActiveVertexArray: No valid Vertex Array specified.");
		vertexArray->activate();
		m_activeVertexArray = vertexArray;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::setActiveShaderProgram(XdevLShaderProgram* shaderProgram) {
		assert(shaderProgram && "XdevLOpenGLImpl::setActiveShaderProgram: No valid Shader Program specified.");
		shaderProgram->activate();
		m_activeShaderProgram = shaderProgram;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::drawVertexArray(XdevLPrimitiveType primitiveType, xdl_uint numberOfElements) {

		XdevLVertexDeclaration* vd = m_activeVertexArray->getVertexDeclaration();
		assert(vd && "XdevLOpenGLImpl::drawVertexArray: Vertex Declaration not found in XdevLVertexArray object.");

		glBindVertexArray(m_activeVertexArray->id());

		for(xdl_uint idx = 0; idx < vd->getNumber(); idx++) {
			GLuint shader_attrib = vd->get(idx)->shaderAttribute;
			glEnableVertexAttribArray(shader_attrib);
		}

		if(m_activeVertexArray->getIndexBuffer() == NULL) {
			glDrawArrays(primitiveType, 0, numberOfElements);
		} else {
			glDrawElements(primitiveType, numberOfElements, GL_UNSIGNED_INT, (void*)0);
		}

		for(xdl_uint idx = 0; idx < vd->getNumber(); idx++) {
			glDisableVertexAttribArray(vd->get(idx)->shaderAttribute);
		}


		return ERR_OK;

	}

	xdl_int XdevLOpenGLImpl::drawInstancedVertexArray(XdevLPrimitiveType primitiveType, xdl_uint numberOfElements, xdl_uint number) {

		glBindVertexArray(m_activeVertexArray->id());

		XdevLVertexDeclaration* vd = m_activeVertexArray->getVertexDeclaration();
		assert(vd && "XdevLOpenGLImpl::drawVertexArray: Vertex Declaration not found in XdevLVertexArray object.");

		for(xdl_uint idx = 0; idx < vd->getNumber(); idx++) {
			GLuint shader_attrib = vd->get(idx)->shaderAttribute;
			glEnableVertexAttribArray(shader_attrib);
		}

		glDrawArraysInstanced(primitiveType, 0, numberOfElements, number);

		for(xdl_uint idx = 0; idx < vd->getNumber(); idx++) {
			glDisableVertexAttribArray(vd->get(idx)->shaderAttribute);
		}

		return ERR_OK;

	}


	xdl_int XdevLOpenGLImpl::drawVertexBuffer(XdevLPrimitiveType primitiveType,
	    xdl_uint numberOfElements,
	    XdevLVertexBuffer* vertexBuffer,
	    XdevLVertexDeclaration* vertexDeclaration) {

		glBindVertexArray(m_activeVertexArray->id());
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->id());

		xdl_uint64 pos = 0;
		for(xdl_uint idx = 0; idx < vertexDeclaration->getNumber(); idx++) {

			GLuint shader_attrib = vertexDeclaration->get(idx)->shaderAttribute;
			glEnableVertexAttribArray(shader_attrib);
			glVertexAttribPointer(shader_attrib,
			                      vertexDeclaration->get(idx)->numberOfComponents,
			                      vertexDeclaration->get(idx)->elementType,
			                      GL_FALSE,
			                      vertexDeclaration->vertexStride(),
			                      BUFFER_OFFSET(pos));

			pos += vertexDeclaration->get(idx)->numberOfComponents*vertexDeclaration->get(idx)->elementTypeSizeInBytes;
		}

		glDrawArrays(primitiveType, 0, numberOfElements);

		for(xdl_uint idx = 0; idx < vertexDeclaration->getNumber(); idx++) {
			glDisableVertexAttribArray(vertexDeclaration->get(idx)->shaderAttribute);
		}

		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::drawVertexBuffer(XdevLPrimitiveType primitiveType,
	    xdl_uint numberOfElements,
	    XdevLVertexBuffer* vertexBuffer,
	    XdevLVertexDeclaration* vertexDeclaration,
	    XdevLIndexBuffer* indexBuffer) {

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->id());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->id());

		xdl_uint64 pos = 0;
		for(xdl_uint idx = 0; idx < vertexDeclaration->getNumber(); idx++) {

			GLuint shader_attrib = vertexDeclaration->get(idx)->shaderAttribute;
			glEnableVertexAttribArray(shader_attrib);
			glVertexAttribPointer(shader_attrib,
			                      vertexDeclaration->get(idx)->numberOfComponents,
			                      vertexDeclaration->get(idx)->elementType,
			                      GL_FALSE,
			                      vertexDeclaration->vertexStride(),
			                      BUFFER_OFFSET(pos));

			pos += vertexDeclaration->get(idx)->numberOfComponents*vertexDeclaration->get(idx)->elementTypeSizeInBytes;
		}

		glDrawElements(primitiveType, numberOfElements, indexBuffer->getElementType(), 0);

		for(xdl_uint idx = 0; idx < vertexDeclaration->getNumber(); idx++) {
			glDisableVertexAttribArray(vertexDeclaration->get(idx)->shaderAttribute);
		}

		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::destroy(XdevLVertexDeclaration* vertexDeclaration) {
		delete vertexDeclaration;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::destroy(XdevLShaderProgram* shaderProgram) {
		delete shaderProgram;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::destroy(XdevLShader* shader) {
		delete shader;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::destroy(XdevLVertexArray* vertexArray) {
		delete vertexArray;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::destroy(XdevLVertexBuffer* vertexBuffer) {
		delete vertexBuffer;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::destroy(XdevLIndexBuffer* indexBuffer) {
		delete indexBuffer;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::destroy(XdevLFrameBuffer* frameBuffer) {
		delete frameBuffer;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::destroy(XdevLTexture* texture) {
		delete texture;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::destroy(XdevLTextureCube* textureCube) {
		delete textureCube;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLImpl::initExtensions() {
		glGenVertexArrays =  (PFNGLGENVERTEXARRAYSPROC)m_gl_context->getProcAddress("glGenVertexArrays");
		glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)m_gl_context->getProcAddress("glDeleteVertexArrays");
		glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)m_gl_context->getProcAddress("glBindVertexArray");
		glIsVertexArray = (PFNGLISVERTEXARRAYPROC)m_gl_context->getProcAddress("glIsVertexArray");

		glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC)m_gl_context->getProcAddress("glEnableVertexAttribArray");
		glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERARBPROC)m_gl_context->getProcAddress("glVertexAttribPointer");



		glGenBuffers = (PFNGLGENBUFFERSARBPROC)m_gl_context->getProcAddress("glGenBuffers");
		glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)m_gl_context->getProcAddress("glDeleteBuffers");
		glBindBuffer = (PFNGLBINDBUFFERARBPROC)m_gl_context->getProcAddress("glBindBuffer");
		glBufferData = (PFNGLBUFFERDATAPROC)m_gl_context->getProcAddress("glBindBuffer");

		glGetShaderiv = (PFNGLGETSHADERIVPROC)m_gl_context->getProcAddress("glGetShaderiv");
		glCreateProgram = (PFNGLCREATEPROGRAMPROC)m_gl_context->getProcAddress("glCreateProgram");
		glLinkProgram = (PFNGLLINKPROGRAMARBPROC)m_gl_context->getProcAddress("glLinkProgram");
		glUseProgram = (PFNGLUSEPROGRAMPROC)m_gl_context->getProcAddress("glUseProgram");

		glBindProgramARB = (PFNGLBINDPROGRAMARBPROC)m_gl_context->getProcAddress("glBindProgram");
		glGetProgramiv = (PFNGLGETPROGRAMIVPROC)m_gl_context->getProcAddress("glGetProgramiv");
		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)m_gl_context->getProcAddress("glGetUniformLocation");

		glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVARBPROC)m_gl_context->getProcAddress("glUniformMatrix2fv");
		glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVARBPROC)m_gl_context->getProcAddress("glUniformMatrix3fv");
		glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVARBPROC)m_gl_context->getProcAddress("glUniformMatrix4fv");

		glCreateShader = (PFNGLCREATESHADERPROC)m_gl_context->getProcAddress("glCreateShader");
		glDeleteShader = (PFNGLDELETESHADERPROC)m_gl_context->getProcAddress("glDeleteShader");
		glCompileShader = (PFNGLCOMPILESHADERARBPROC)m_gl_context->getProcAddress("glCompileShader");
		glShaderSource = (PFNGLSHADERSOURCEPROC)m_gl_context->getProcAddress("glShaderSource");
		glAttachShader = (PFNGLATTACHSHADERPROC)m_gl_context->getProcAddress("glAttachShader");
		glShaderSource = (PFNGLSHADERSOURCEPROC)m_gl_context->getProcAddress("glShaderSource");

		glDrawArraysEXT = (PFNGLDRAWARRAYSEXTPROC)m_gl_context->getProcAddress("glDrawArraysEXT");
		
		return ERR_OK;
	}
}
