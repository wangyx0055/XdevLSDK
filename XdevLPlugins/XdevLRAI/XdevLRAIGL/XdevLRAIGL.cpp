#include <stdexcept>
#include <fstream>
#include <cmath>

#include <XdevLCoreMediator.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLXstring.h>

#include "XdevLFrameBufferImpl.h"
#include "XdevLRAIGL.h"
#include "XdevLVertexBufferImpl.h"
#include "XdevLIndexBufferImpl.h"
#include "XdevLVertexArrayImpl.h"
#include "XdevLShaderProgramImpl.h"
#include "XdevLShaderImpl.h"
#include "XdevLTextureCubeImpl.h"
#include "XdevLTexture3DImpl.h"
#include "XdevLOpenGLUtils.h"


class vertexArray;
class wrappPrimitiveType;


xdl::XdevLModuleDescriptor openGLModuleDesc {
	xdl::XdevLString("www.codeposer.net"),
	xdl::XdevLString("Cengiz Terzibas"),
	xdl::moduleNames[0],
	xdl::XdevLString("(c) 2005 - 2012 Cengiz Terzibas."),
	xdl::XdevLString("Creates a OpenGL context for rendering 3D graphics. Depending on your graphics card all supported extensions will be avaiable."),
	XDEVLRAIGL_MAJOR_VERSION,
	XDEVLRAIGL_MINOR_VERSION,
	XDEVLRAIGL_PATCH_VERSION
};

xdl::XdevLPluginDescriptor openglDescriptor {
	xdl::XdevLString("XdevLRAIGL"),
	xdl::moduleNames,
	XDEVLRAIGL_MODULE_MAJOR_VERSION,
	XDEVLRAIGL_MODULE_MINOR_VERSION,
	XDEVLRAIGL_MODULE_PATCH_VERSION
};


XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(openglDescriptor)

XDEVL_PLUGIN_CREATE_MODULE {
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLRAIGL, openGLModuleDesc);
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}

XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DEFINITION(XdevLRAI, xdl::XdevLRAIGL, openGLModuleDesc)

namespace xdl {

	std::string vertex_shader_source = {
		"\
layout(location = 0) in vec3 iposition;\
layout(location = 1) in vec2 itexcoord;\
\
out vec2 texcoord;\
\
void main() {\
 texcoord = itexcoord;\
 gl_Position = vec4(iposition * 0.5, 1.0);\
}"
	};

	std::string fragment_shader_source = {
		"\
layout(location = 0) out vec4 ocolor;\
in vec2 texcoord;\
uniform sampler2D tex;\
\
void main() {\
\
 ocolor = texture(tex, texcoord);\
}"
	};

	XdevLRAIGL::XdevLRAIGL(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) : XdevLModuleImpl<XdevLRAI>(parameter, descriptor),
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
		m_useInternalBuffer(xdl_false),
		m_defaultFrameBuffer(nullptr),
		m_activeFrameBuffer(nullptr),
		m_activeVertexArray(nullptr),
		m_activeShaderProgram(nullptr),
		m_use2DFrameBuffer(xdl_false),
		m_2DFrameBuffer(nullptr),
		m_vertexDeclaration(nullptr),
		m_frameBufferArray(nullptr),
		m_frameBufferShaderProgram(nullptr),
		m_frameBufferVertexShader(nullptr),
		m_frameBufferFragmentShader(nullptr) {
		XDEVL_MODULE_INFO("XdevLRAIGL()\n");
	}

	XdevLRAIGL::~XdevLRAIGL() {
		XDEVL_MODULE_INFO("~XdevLRAIGL()\n");
	}

	xdl_int XdevLRAIGL::init() {
		TiXmlDocument xmlDocument;

		if(!xmlDocument.LoadFile(getMediator()->getXmlFilename())) {
			XDEVL_MODULE_WARNING("Could not parse xml file: " << getMediator()->getXmlFilename() << std::endl);
		} else {

			if(readModuleInformation(&xmlDocument) != ERR_OK) {
				return ERR_ERROR;
			}
		}

		return ERR_OK;
	}

	int XdevLRAIGL::create(IPXdevLWindow window) {


		m_gl_context = static_cast<XdevLOpenGLContext*>(getMediator()->createModule(xdl::XdevLModuleName("XdevLOpenGLContext"), xdl::XdevLID("XdevLRAIOpenGLContext")));
		if(nullptr == m_gl_context) {
			XDEVL_MODULE_WARNING("Could not create OpenGL context. Assuming you're using a manually created one.\n");
		} else {
			// Create OpenGL context.
			if(m_gl_context->create(window) != xdl::ERR_OK) {
				return ERR_ERROR;
			}
			m_gl_context->makeCurrent(window);
		}

		XDEVL_MODULE_INFO(glGetString(GL_VENDOR) << std::endl);
		XDEVL_MODULE_INFO(glGetString(GL_RENDERER) << std::endl);
		XDEVL_MODULE_INFO(glGetString(GL_VERSION) << std::endl);


		if(initGLEW() != ERR_OK) {
			return ERR_ERROR;
		}

		//
		// Initialize the default framebuffer.
		//
		m_defaultFrameBuffer = createFrameBuffer();
		m_defaultFrameBuffer->init(320, 200);
		if(m_defaultFrameBuffer->addColorTarget(0, xdl::XDEVL_FB_COLOR_RGBA) != ERR_OK) {
			XDEVL_MODULE_ERROR("Could't create color target for default framebuffer.\n")
		} else {
			auto texture = m_defaultFrameBuffer->getTexture(0);
			texture->lock();
			texture->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_LINEAR);
			texture->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_LINEAR);
			texture->unlock();
			if(m_defaultFrameBuffer->addDepthTarget(xdl::XDEVL_FB_DEPTH_COMPONENT24) != ERR_OK) {
				XDEVL_MODULE_ERROR("Could't create depth target for default framebuffer.\n")
			}

			// Assign the new framebuffer as the active one.
			m_activeFrameBuffer = m_defaultFrameBuffer;
		}

		//
		// Initialize the default framebuffer.
		//
		m_2DFrameBuffer = createFrameBuffer();
		m_2DFrameBuffer->init(window->getWidth(), window->getHeight());
		if(m_2DFrameBuffer->addColorTarget(0, xdl::XDEVL_FB_COLOR_RGBA) != ERR_OK) {
			XDEVL_MODULE_ERROR("Could't create color target for default framebuffer.\n")
		} else {
			auto texture = m_2DFrameBuffer->getTexture(0);
			texture->lock();
			texture->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_LINEAR);
			texture->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_LINEAR);
			texture->unlock();
		}
		if(m_2DFrameBuffer->addDepthTarget(xdl::XDEVL_FB_DEPTH_COMPONENT24) != ERR_OK) {
			XDEVL_MODULE_ERROR("Could't create depth target for default framebuffer.\n")
		}
		m_window = window;

		createScreenVertexArray();

		XDEVL_MODULE_SUCCESS("OpenGL successfully initialized.\n")

		return ERR_OK;
	}

	xdl_int XdevLRAIGL::setActiveInternalFrameBuffer(xdl_bool state) {
		m_useInternalBuffer = state;
		if(state) {
			m_defaultFrameBuffer->activate();

			xdl::xdl_uint list[] = {xdl::XDEVL_COLOR_TARGET0};
			m_defaultFrameBuffer->activateColorTargets(1, list);
		} else {
			m_defaultFrameBuffer->deactivate();
		}
		return ERR_OK;
	}

	xdl_int XdevLRAIGL::setActive2DFrameBuffer(xdl_bool state) {
		m_use2DFrameBuffer = state;

		if(state) {
			m_defaultFrameBuffer->deactivate();
			m_2DFrameBuffer->activate();

			xdl::xdl_uint list[] = {xdl::XDEVL_COLOR_TARGET0};
			m_2DFrameBuffer->activateColorTargets(1, list);
		} else {
			m_2DFrameBuffer->deactivate();
		}
		return ERR_OK;
	}

	xdl_int XdevLRAIGL::readModuleInformation(TiXmlDocument* document) {
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

	xdl_int XdevLRAIGL::initGLEW() {
		glewExperimental= GL_TRUE;
		GLenum err = glewInit();
		if(GLEW_OK != err) {
			XDEVL_MODULE_ERROR(glewGetErrorString(err) << std::endl);
			return ERR_ERROR;
		}

		// Only use if the extenstion is available. In some cases this is not
		// supported so we will use this as a hack.
		if(GLEW_ARB_debug_output) {
			glDebugMessageCallback((GLDEBUGPROC)(openGLDebugOutput), nullptr);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		}

		return ERR_OK;
	}


	void XdevLRAIGL::getOpenGLInfos() {
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

	const xdl_char* XdevLRAIGL::getVersion() {
		return m_version.c_str();
	}

	const xdl_char* XdevLRAIGL::getVendor() {
		return m_vendor.c_str();
	}

	xdl_int XdevLRAIGL::getNumExtensions() const {
		return m_numExtensions;
	}

	const xdl_char* XdevLRAIGL::getExtension(xdl_int idx) {
		const char* tmp = "";
		try {
			return m_extensions.at(idx).c_str();
		} catch(std::out_of_range& e) {
			XDEVL_MODULE_ERROR(e.what());
			return tmp;
		}
	}

	xdl_bool XdevLRAIGL::extensionSupported(const xdl_char* extensionName) {
		for(xdl_int a = 0; a < getNumExtensions(); ++a) {
			if(m_extensions[a] == extensionName)
				return true;
		}
		return false;
	}

	const xdl_char* XdevLRAIGL::getShaderVersion() {
		return (xdl_char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	}

	XdevLFrameBuffer* XdevLRAIGL::getDefaultFrameBuffer() {
		return m_defaultFrameBuffer.get();
	}

	XdevLFrameBuffer* XdevLRAIGL::get2DFrameBuffer() {
		return m_2DFrameBuffer.get();
	}

	void XdevLRAIGL::setPointSize(xdl_float size) {
		glPointSize(size);
	}

	void XdevLRAIGL::setLineSize(xdl_float size) {
		glLineWidth(size);
	}

	void XdevLRAIGL::setActiveDepthTest(xdl_bool enableDepthTest) {
		enableDepthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	}

	void XdevLRAIGL::setActiveBlendMode(xdl_bool enableBlendMode) {
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

	xdl_int XdevLRAIGL::setBlendMode(XdevLBlendModes src, XdevLBlendModes dst) {
		GLint blend_src = XdevLBlendModeToGLBlendMode(src);
		GLint blend_dst = XdevLBlendModeToGLBlendMode(dst);
		glBlendFunc(blend_src, blend_dst);
		return ERR_OK;
	}

	xdl_int XdevLRAIGL::clearColorTargets(xdl_float r, xdl_float g, xdl_float b, xdl_float a) {
		glClearColor(r,g, b, a);
		glClear(GL_COLOR_BUFFER_BIT);
		return ERR_OK;
	}

	xdl_int XdevLRAIGL::clearDepthTarget(xdl_float clear_value) {
		glClearDepth(clear_value);
		glClear(GL_DEPTH_BUFFER_BIT);
		return ERR_OK;
	}

	xdl_int XdevLRAIGL::setViewport(xdl_float x, xdl_float y, xdl_float width, xdl_float height) {
		glViewport(x, y, width, height);
		return ERR_OK;
	}

	xdl_int XdevLRAIGL::setActiveRenderWindow(IPXdevLWindow window) {
		if(m_gl_context) {
			m_gl_context->makeCurrent(window);
		}
		return ERR_OK;
	}

	xdl_int XdevLRAIGL::swapBuffers() {
		if(m_useInternalBuffer) {
			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_defaultFrameBuffer->id());
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

			glDrawBuffer(GL_BACK);
			glBlitFramebuffer(0, 0, m_defaultFrameBuffer->getWidth(), m_defaultFrameBuffer->getHeight(),
			                  0, 0, m_window->getWidth(), m_window->getHeight(),
			                  GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		}

		if(m_use2DFrameBuffer) {
			// Activate the windows default framebuffer (context one).
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// Render the stuff.
			renderFrameBufferPlane();

//			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_2DFrameBuffer->id());
//			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

//			glDrawBuffer(GL_BACK);
//			glBlitFramebuffer(0, 0, m_2DFrameBuffer->getWidth(), m_2DFrameBuffer->getHeight(),
//			                  0, 0, m_window->getWidth(), m_window->getHeight(),
//			                  GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		}

		if(m_gl_context) {
			m_gl_context->swapBuffers();
		}
		return ERR_OK;
	}

	IPXdevLVertexDeclaration XdevLRAIGL::createVertexDeclaration() {
		auto tmp = std::shared_ptr<XdevLVertexDeclaration>(new XdevLVertexDeclaration());
		return tmp;
	}

	IPXdevLVertexShader XdevLRAIGL::createVertexShader() {
		auto tmp = std::shared_ptr<XdevLOpenGLVertexShaderImpl>(new XdevLOpenGLVertexShaderImpl());
		return tmp;
	}

	IPXdevLFragmentShader XdevLRAIGL::createFragmentShader() {
		auto tmp = std::shared_ptr<XdevLOpenGLFragmentShaderImpl>(new XdevLOpenGLFragmentShaderImpl());
		return tmp;
	}

	IPXdevLGeometryShader XdevLRAIGL::createGeometryShader() {
		auto tmp = std::shared_ptr<XdevLOpenGLGeometryShaderImpl>(new XdevLOpenGLGeometryShaderImpl());
		return tmp;;
	}

	IPXdevLShaderProgram XdevLRAIGL::createShaderProgram() {
		auto tmp = std::shared_ptr<XdevLOpenGLProgramImpl>(new XdevLOpenGLProgramImpl());
		return tmp;
	}

	IPXdevLTexture XdevLRAIGL::createTexture() {
		auto tmp = std::shared_ptr<XdevLTextureImpl>(new XdevLTextureImpl());
		return tmp;
	}

	IPXdevLTextureCube XdevLRAIGL::createTextureCube() {
		auto tmp = std::shared_ptr<XdevLTextureCubeImpl>(new XdevLTextureCubeImpl());
		return tmp;
	}

	IPXdevLTexture3D XdevLRAIGL::createTexture3D() {
		auto tmp = std::shared_ptr<XdevLTexture3DImpl>(new XdevLTexture3DImpl());
		return tmp;
	}

	IPXdevLFrameBuffer XdevLRAIGL::createFrameBuffer() {
		auto tmp = std::shared_ptr<XdevLFrameBufferImpl>(new XdevLFrameBufferImpl());
		return tmp;
	}

	IPXdevLVertexArray XdevLRAIGL::createVertexArray() {
		auto tmp = std::shared_ptr<XdevLVertexArrayImpl>(new XdevLVertexArrayImpl());
		return tmp;
	}

	IPXdevLVertexBuffer XdevLRAIGL::createVertexBuffer() {
		auto tmp = std::shared_ptr<XdevLVertexBufferImpl>(new XdevLVertexBufferImpl());
		return tmp;
	}

	IPXdevLIndexBuffer XdevLRAIGL::createIndexBuffer() {
		auto tmp = std::shared_ptr<XdevLIndexBufferImpl>(new XdevLIndexBufferImpl());
		return tmp;
	}

	xdl_int XdevLRAIGL::setActiveFrameBuffer(IPXdevLFrameBuffer frambuffer) {
		assert(frambuffer && "XdevLOpenGLImpl::setActiveFrameBuffer: No valid Framebuffer specified.");
		m_activeFrameBuffer = frambuffer;
		return ERR_OK;
	}


	xdl_int XdevLRAIGL::setActiveVertexArray(IPXdevLVertexArray vertexArray) {
		assert(vertexArray && "XdevLOpenGLImpl::setActiveVertexArray: No valid Vertex Array specified.");
		m_activeVertexArray = vertexArray;
		return ERR_OK;
	}

	xdl_int XdevLRAIGL::setActiveShaderProgram(IPXdevLShaderProgram shaderProgram) {
		assert(shaderProgram && "XdevLOpenGLImpl::setActiveShaderProgram: No valid Shader Program specified.");
		m_activeShaderProgram = shaderProgram;
		return ERR_OK;
	}

	xdl_int XdevLRAIGL::drawVertexArray(XdevLPrimitiveType primitiveType, xdl_uint numberOfElements) {

		glBindVertexArray(m_activeVertexArray->id());
		glUseProgram(m_activeShaderProgram->id());

		if(m_activeVertexArray->getIndexBuffer() == nullptr) {
			glDrawArrays(primitiveType, 0, numberOfElements);
		} else {
			glDrawElements(primitiveType, numberOfElements, m_activeVertexArray->getIndexBuffer()->getElementType(), nullptr);
		}

		glBindVertexArray(0);
//		glUseProgram(0);

		return ERR_OK;

	}

	xdl_int XdevLRAIGL::drawInstancedVertexArray(XdevLPrimitiveType primitiveType, xdl_uint numberOfElements, xdl_uint number) {

		glBindVertexArray(m_activeVertexArray->id());
		glUseProgram(m_activeShaderProgram->id());

		glDrawArraysInstanced(primitiveType, 0, numberOfElements, number);

		glBindVertexArray(0);
//		glUseProgram(0);

		return ERR_OK;

	}

	xdl_int XdevLRAIGL::initExtensions() {
//		glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)m_gl_context->getProcAddress("glGenVertexArrays");
//		glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)m_gl_context->getProcAddress("glDeleteVertexArrays");
//		glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)m_gl_context->getProcAddress("glBindVertexArray");
//		glIsVertexArray = (PFNGLISVERTEXARRAYPROC)m_gl_context->getProcAddress("glIsVertexArray");
//
//		glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC)m_gl_context->getProcAddress("glEnableVertexAttribArray");
//		glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERARBPROC)m_gl_context->getProcAddress("glVertexAttribPointer");
//
//
//
//		glGenBuffers = (PFNGLGENBUFFERSARBPROC)m_gl_context->getProcAddress("glGenBuffers");
//		glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)m_gl_context->getProcAddress("glDeleteBuffers");
//		glBindBuffer = (PFNGLBINDBUFFERARBPROC)m_gl_context->getProcAddress("glBindBuffer");
//		glBufferData = (PFNGLBUFFERDATAPROC)m_gl_context->getProcAddress("glBindBuffer");
//
//		glGetShaderiv = (PFNGLGETSHADERIVPROC)m_gl_context->getProcAddress("glGetShaderiv");
//		glCreateProgram = (PFNGLCREATEPROGRAMPROC)m_gl_context->getProcAddress("glCreateProgram");
//		glLinkProgram = (PFNGLLINKPROGRAMARBPROC)m_gl_context->getProcAddress("glLinkProgram");
//		glUseProgram = (PFNGLUSEPROGRAMPROC)m_gl_context->getProcAddress("glUseProgram");
//
//		glBindProgramARB = (PFNGLBINDPROGRAMARBPROC)m_gl_context->getProcAddress("glBindProgram");
//		glGetProgramiv = (PFNGLGETPROGRAMIVPROC)m_gl_context->getProcAddress("glGetProgramiv");
//		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)m_gl_context->getProcAddress("glGetUniformLocation");
//
//		glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVARBPROC)m_gl_context->getProcAddress("glUniformMatrix2fv");
//		glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVARBPROC)m_gl_context->getProcAddress("glUniformMatrix3fv");
//		glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVARBPROC)m_gl_context->getProcAddress("glUniformMatrix4fv");
//
//		glCreateShader = (PFNGLCREATESHADERPROC)m_gl_context->getProcAddress("glCreateShader");
//		glDeleteShader = (PFNGLDELETESHADERPROC)m_gl_context->getProcAddress("glDeleteShader");
//		glCompileShader = (PFNGLCOMPILESHADERARBPROC)m_gl_context->getProcAddress("glCompileShader");
//		glShaderSource = (PFNGLSHADERSOURCEPROC)m_gl_context->getProcAddress("glShaderSource");
//		glAttachShader = (PFNGLATTACHSHADERPROC)m_gl_context->getProcAddress("glAttachShader");
//		glShaderSource = (PFNGLSHADERSOURCEPROC)m_gl_context->getProcAddress("glShaderSource");
//
//		glDrawArraysEXT = (PFNGLDRAWARRAYSEXTPROC)m_gl_context->getProcAddress("glDrawArraysEXT");

		return ERR_OK;
	}

	void XdevLRAIGL::shaderLog(xdl_uint shaderId) {
		// Get the length of the message.
		GLint Len;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &Len);

		GLsizei ActualLen;
		GLchar *Log = new GLchar[Len];
		glGetShaderInfoLog(shaderId, Len, &ActualLen, Log);
		XDEVL_MODULE_ERROR(Log << "\n");
		delete [] Log;
	}

	xdl_int XdevLRAIGL::createScreenVertexArray() {

		xdl::xdl_float screen_vertex [] = {
			-1.0f, -1.0f,
			-1.0f, 1.0f,
			1.0f, 1.0f,

			1.0f, 1.0f,
			1.0f, -1.0f,
			-1.0f, -1.0f
		};

		xdl::xdl_float screen_uv [] = {
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,

			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f
		};

		m_vertexDeclaration = createVertexDeclaration();
		m_vertexDeclaration->add(2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, 0);
		m_vertexDeclaration->add(2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, 9);

		std::vector<xdl::xdl_uint8*> list;
		list.push_back((xdl::xdl_uint8*)screen_vertex);
		list.push_back((xdl::xdl_uint8*)screen_uv);

		m_frameBufferArray = createVertexArray();
		m_frameBufferArray->init(list.size(), list.data(), 6, m_vertexDeclaration);

		// Create the shader program.
		m_frameBufferShaderProgram = createShaderProgram();
		m_frameBufferVertexShader = createVertexShader();
		m_frameBufferFragmentShader = createFragmentShader();
		m_frameBufferVertexShader->addShaderCode(vertex_shader_source.c_str());
		m_frameBufferFragmentShader->addShaderCode(fragment_shader_source.c_str());
		m_frameBufferVertexShader->compile();
		m_frameBufferFragmentShader->compile();

		m_frameBufferShaderProgram->attach(m_frameBufferVertexShader);
		m_frameBufferShaderProgram->attach(m_frameBufferFragmentShader);
		m_frameBufferShaderProgram->link();

		m_frameBufferTextureShaderId = m_frameBufferShaderProgram->getUniformLocation("tex");

		return ERR_OK;
	}

	xdl_int XdevLRAIGL::renderFrameBufferPlane() {

		setActiveBlendMode(xdl_true);
		setBlendMode(XDEVL_BLEND_SRC_ALPHA, XDEVL_BLEND_ONE_MINUS_SRC_ALPHA);
		setActiveDepthTest(xdl_false);

		m_frameBufferShaderProgram->activate();
		m_frameBufferShaderProgram->setUniformi(m_frameBufferTextureShaderId, 0);
		m_2DFrameBuffer->getTexture(0)->activate(0);
		m_frameBufferShaderProgram->deactivate();

//		auto vd = m_vertexDeclaration;
//
//		glBindVertexArray(m_frameBufferArray->id());
//		glUseProgram(m_frameBufferShaderProgram->id());
//
//		for(xdl_uint idx = 0; idx < vd->getNumber(); idx++) {
//			GLuint shader_attrib = vd->get(idx)->shaderAttribute;
//			glEnableVertexAttribArray(shader_attrib);
//		}
//
//		glDrawArrays(xdl::XDEVL_PRIMITIVE_TRIANGLES, 0, 6);
//
//		for(xdl_uint idx = 0; idx < vd->getNumber(); idx++) {
//			glDisableVertexAttribArray(vd->get(idx)->shaderAttribute);
//		}


		setActiveVertexArray(m_frameBufferArray);
		setActiveShaderProgram(m_frameBufferShaderProgram);

		drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, 6);

		return ERR_OK;
	}
}
