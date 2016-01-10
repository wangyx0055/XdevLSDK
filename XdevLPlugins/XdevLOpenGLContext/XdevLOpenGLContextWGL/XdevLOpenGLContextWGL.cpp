
#include "XdevLOpenGLContextWGL.h"
#include <XdevLCoreMediator.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLXstring.h>
#include <XdevLPlatform.h>
#include <GL/GL.h>
#include <GL/wglext.h>
//#include <GL/glew.h>
//#include <GL/wglew.h>
#include <tinyxml.h>

xdl::XdevLModuleDescriptor moduleDescriptor {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[0],
	xdl::copyright,
	xdl::description,
	XDEVLOPENGLCONTEXT_WGL_MAJOR_VERSION,
	XDEVLOPENGLCONTEXT_WGL_MINOR_VERSION,
	XDEVLOPENGLCONTEXT_WGL_PATCH_VERSION
};

xdl::XdevLPluginDescriptor wglPluginDescriptor {
	xdl::pluginName,
	xdl::moduleNames,
	XDEVLOPENGLCONTEXT_WGL_MODULE_MAJOR_VERSION,
	XDEVLOPENGLCONTEXT_WGL_MODULE_MINOR_VERSION,
	XDEVLOPENGLCONTEXT_WGL_MODULE_PATCH_VERSION
};

XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(wglPluginDescriptor);

XDEVL_PLUGIN_CREATE_MODULE {
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLOpenGLWGL, moduleDescriptor)
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}


namespace xdl {

	XdevLOpenGLWGL::XdevLOpenGLWGL(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		XdevLOpenGLContextBase(parameter, descriptor),
		m_wnd(NULL),
		m_DC(NULL),
		m_RC(NULL),
		m_ARBMultisampleSupported(false),
		color_alpha_size(0) {
	}

	XdevLOpenGLWGL::~XdevLOpenGLWGL() {
	}

	void* XdevLOpenGLWGL::getProcAddress(const xdl_char* func) {
		return (void*)wglGetProcAddress(func);
	}

	xdl_int XdevLOpenGLWGL::getAttributes(XdevLOpenGLContextAttributes& attributes) {
		attributes = m_attributes;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLWGL::setAttributes(const XdevLOpenGLContextAttributes& attributes) {
		m_attributes = attributes;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLWGL::init() {
		if (XdevLOpenGLContextBase::init() == ERR_ERROR) {
			return ERR_ERROR;
		}
		//
		// Let's get the extensions
		//




		//HMODULE module = LoadLibraryA("opengl32.dll");
		//wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)GetProcAddress(module, "wglCreateContextAttribsARB");
		//wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)GetProcAddress(module, "wglSwapIntervalEXT");
		//wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)GetProcAddress(module, "wglChoosePixelFormatARB");

		//	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		//	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		//	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

		//	wglMakeCurrent(hDC, NULL);
		//	wglDeleteContext(hRC);

		return ERR_OK;
	}

	xdl_int XdevLOpenGLWGL::create(XdevLWindow* window) {

		if(window == NULL) {
			XDEVL_MODULE_ERROR("Parameter invalid.\n");
			return ERR_ERROR;
		}

		m_window = window;
		m_wnd = static_cast<HWND>(m_window->getInternal(XdevLInternalName("WIN32_HWND")));
		if ( m_wnd == NULL) {
			XDEVL_MODULE_ERROR("Get WIN32_HWND failed.\n");
			return ERR_ERROR;
		}

		if (initOpenGL() == ERR_ERROR) {
			XDEVL_MODULE_ERROR("Failed to initialize OpenGL.\n");
			return ERR_ERROR;
		}

		// Set vertical syncronisation.
		setVSync(getVSync());

		// TODO: This shouldn't be done maybe because it changes the initial state of the OpenGL context.
		glClearColor(1.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		SwapBuffers(m_DC);

		return ERR_OK;
	}

	void* XdevLOpenGLWGL::getInternal(const xdl_char* id) {
		std::string data(id);
		if (data == "WIN32_DC")
			return m_DC;
		if (data == "WIN32_RC")
			return m_RC;
		return NULL;
	}

	xdl_int XdevLOpenGLWGL::shutdown() {
		if (m_RC) {
			if (!wglMakeCurrent(NULL, NULL)) {
				XDEVL_MODULE_ERROR("wglMakeCurrent() failed.\n");
				return ERR_ERROR;
			}
			if (!wglDeleteContext( m_RC)) {
				XDEVL_MODULE_ERROR("wglDeleteContext() failed.\n");
				return ERR_ERROR;
			}
		}
		m_RC = NULL;
		if ( m_DC && !ReleaseDC( m_wnd, m_DC)) {
			XDEVL_MODULE_ERROR("ReleaseDC() failed.\n");
			return ERR_ERROR;
		}
		m_DC = NULL;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLWGL::reset() {
		shutdown();

		if(initOpenGL() == ERR_ERROR)
			return ERR_ERROR;

		return ERR_OK;
	}

	xdl_int XdevLOpenGLWGL::initOpenGL() {
		m_DC = GetDC(m_wnd);
		if(m_DC == NULL) {
			XDEVL_MODULE_ERROR("GetDC() failed.");
			return ERR_ERROR;
		}

		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = m_attributes.color_buffer_size;
		pfd.cDepthBits = m_attributes.depth_size;
		pfd.cStencilBits = m_attributes.stencil_size;
		pfd.cRedBits = m_attributes.red_size;
		pfd.cGreenBits = m_attributes.green_size;
		pfd.cBlueBits = m_attributes.blue_size;
		pfd.cAlphaBits = m_attributes.alpha_size;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int iPixelFormat = ChoosePixelFormat(m_DC, &pfd);
		if (FALSE == iPixelFormat) {
			XDEVL_MODULE_ERROR("ChoosePixelFormat failed.\n");
			return ERR_ERROR;
		}

		if (SetPixelFormat(m_DC, iPixelFormat, &pfd) != TRUE) {
			XDEVL_MODULE_ERROR("SetPixelFormat failed.\n");
			return ERR_ERROR;
		}

		// Create the old style context (OpenGL 2.1 and before)
		HGLRC hRC = wglCreateContext(m_DC);
		wglMakeCurrent(m_DC, hRC);

		wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(hRC);

		if (wglCreateContextAttribsARB) {
			const int iPixelFormatAttribList[] = {
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
				WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
				WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
				WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB,
				WGL_COLOR_BITS_ARB, 32,
				WGL_DEPTH_BITS_ARB, 24,
				WGL_STENCIL_BITS_ARB, 8,
				WGL_ACCUM_BITS_ARB, 0,
				WGL_RED_BITS_ARB, m_attributes.red_size,
				WGL_GREEN_BITS_ARB, m_attributes.green_size,
				WGL_BLUE_BITS_ARB, m_attributes.blue_size,
				WGL_ALPHA_BITS_ARB, m_attributes.alpha_size,
				0 // End of attributes list
			};

			int iPixelFormat, iNumFormats;
			if (wglChoosePixelFormatARB(m_DC, iPixelFormatAttribList, NULL, 1, &iPixelFormat, (UINT*)&iNumFormats) != TRUE) {
				XDEVL_MODULE_ERROR("wglChoosePixelFormatARB failed.\n");
				return ERR_ERROR;
			}

			// PFD seems to be only redundant parameter now
			if (SetPixelFormat(m_DC, iPixelFormat, &pfd) != TRUE) {
				XDEVL_MODULE_ERROR("SetPixelFormat failed.\n");
				return ERR_ERROR;
			}


			//
			// Set the core profile attributes.
			//
			std::vector<xdl_int> contextAttributes;
			contextAttributes.push_back(WGL_CONTEXT_MAJOR_VERSION_ARB);
			contextAttributes.push_back(m_attributes.context_major_version);
			contextAttributes.push_back(WGL_CONTEXT_MINOR_VERSION_ARB);
			contextAttributes.push_back(m_attributes.context_minor_version);

			contextAttributes.push_back(WGL_CONTEXT_PROFILE_MASK_ARB);
			if (m_attributes.context_profile_mask == XDEVL_OPENGL_CONTEXT_CORE_PROFILE) {
				contextAttributes.push_back(WGL_CONTEXT_CORE_PROFILE_BIT_ARB);
			} else if (m_attributes.context_profile_mask == XDEVL_OPENGL_CONTEXT_COMPATIBILITY) {
				contextAttributes.push_back(WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB);
			} else if (m_attributes.context_profile_mask == XDEVL_OPENGL_CONTEXT_ES1) {
				XDEVL_MODULE_ERROR("Not supported WGL_CONTEXT_PROFILE_MASK_ARB .\n");
				return ERR_ERROR;
			} else if (m_attributes.context_profile_mask == XDEVL_OPENGL_CONTEXT_ES2) {
				XDEVL_MODULE_ERROR("Not supported WGL_CONTEXT_PROFILE_MASK_ARB .\n");
				return ERR_ERROR;
			} else {
				XDEVL_MODULE_ERROR("Not supported WGL_CONTEXT_PROFILE_MASK_ARB .\n");
				return ERR_ERROR;
			}

			//
			// Set the WGL_CONTEXT_FLAGS_ARB
			//
			if (m_attributes.context_flags != XDEVL_OPENGL_CONTEXT_FLAGS_NONE || (m_debugMode == xdl_true)) {
				contextAttributes.push_back(WGL_CONTEXT_FLAGS_ARB);
				if (m_debugMode == xdl_true) {
					contextAttributes.push_back(WGL_CONTEXT_DEBUG_BIT_ARB);
				}
				if (m_attributes.context_flags == XDEVL_OPENGL_CONTEXT_FLAGS_FORWARD_COMPATIBLE_BIT) {
					contextAttributes.push_back(WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB);
				}
			}

			contextAttributes.push_back(0);


			m_RC = wglCreateContextAttribsARB(m_DC, 0, contextAttributes.data());
			if (nullptr == m_RC) {
				XDEVL_MODULE_ERROR("wglCreateContextAttribsARB failed.\n");
				return ERR_ERROR;
			}
			// If everything went OK
			if (wglMakeCurrent(m_DC, m_RC) != TRUE) {
				XDEVL_MODULE_ERROR("wglMakeCurrent failed.\n");
				return ERR_ERROR;
			}

		} else {
			// Are We Able To Get A Rendering Context?
			m_RC = wglCreateContext(m_DC);
			if (nullptr == m_RC) {
				XDEVL_MODULE_ERROR("Could not create GL context.\n");
				return ERR_ERROR;
			}
		}
		return ERR_OK;
	}

	xdl_int XdevLOpenGLWGL::swapBuffers() {
		//	wglSwapLayerBuffers(m_DC, 1);
		SwapBuffers(m_DC);
		return ERR_OK;
	}

	xdl_int XdevLOpenGLWGL::makeCurrent(XdevLWindow* window) {
		// Try To Activate The Rendering Context
		if (wglMakeCurrent(m_DC, m_RC) != TRUE) {
			XDEVL_MODULE_ERROR("wglMakeCurrent failed.\n");
			return ERR_ERROR;
		}
		return ERR_OK;
	}

	xdl_int XdevLOpenGLWGL::setVSync(xdl_bool state) {
		if (wglSwapIntervalEXT) {
			if(state)
				wglSwapIntervalEXT(1);
			else
				wglSwapIntervalEXT(0);

			return ERR_OK;
		} else {
			XDEVL_MODULE_WARNING("VSync not supported.\n");
		}
		return ERR_ERROR;
	}

	xdl_int XdevLOpenGLWGL::setEnableFSAA(xdl_bool state) {
		return ERR_ERROR;
	}

}
