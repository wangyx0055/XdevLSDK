

#include <XdevLWindow/XdevLWindow.h>
#include "XdevLOpenGLContextEGL.h"

#if XDEVL_PLATFORM_WINDOWS
#include <Windows.h>
#elif XDEVL_PLATFORM_LINUX
#include <X11/Xlib.h>
#endif

xdl::XdevLPluginDescriptor pluginDescriptor {
	xdl::pluginName,
	xdl::moduleNames,
	XDEVLOPENGLCONTEXTEGL_PLUGIN_MAJOR_VERSION,
	XDEVLOPENGLCONTEXTEGL_PLUGIN_MINOR_VERSION,
	XDEVLOPENGLCONTEXTEGL_PLUGIN_PATCH_VERSION
};

xdl::XdevLModuleDescriptor moduleDescriptor {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[0],
	xdl::copyright,
	xdl::moduleDescription,
	XDEVLOPENGLCONTEXTEGL_MODULE_MAJOR_VERSION,
	XDEVLOPENGLCONTEXTEGL_MODULE_MINOR_VERSION,
	XDEVLOPENGLCONTEXTEGL_MODULE_PATCH_VERSION
};

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	if(moduleDescriptor.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLOpenGLContextEGL(parameter);
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
	return &pluginDescriptor;
}

namespace xdl {

	XdevLOpenGLContextEGL::XdevLOpenGLContextEGL(XdevLModuleCreateParameter* parameter) :
		XdevLOpenGLContextBase(parameter, moduleDescriptor) {
	}

	XdevLOpenGLContextEGL::~XdevLOpenGLContextEGL() {

	}

	xdl_int XdevLOpenGLContextEGL::create(XdevLWindow* window) {

		static const EGLint context_attribs[] = {
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
		};
		const char *extensions;

		EGLint config_attribs[] = {
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RED_SIZE, 1,
			EGL_GREEN_SIZE, 1,
			EGL_BLUE_SIZE, 1,
			EGL_ALPHA_SIZE, 1,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
			EGL_NONE
		};

		EGLint major, minor, n, count, i, size;


		EGLNativeDisplayType nativeDisplay;
		EGLNativeWindowType nativeWindow;

#if XDEVL_PLATFORM_WINDOWS
		HINSTANCE	hInstance = static_cast<HWND>(m_window->getInternal(XdevLInternalName("WIN32_INSTANCE")));
		if(nullptr == hInstance) {
			XDEVL_MODULE_ERROR("Could not get native WIN32_INSTANCE information.\n");
			return ERR_ERROR;
		}

		HWND	hWnd = static_cast<HWND>(m_window->getInternal(XdevLInternalName("WIN32_HWND")));
		if(nullptr == hWnd) {
			XDEVL_MODULE_ERROR("Could not get native WIN32_HWND information.\n");
			return ERR_ERROR;
		}
		HDC hDC = static_cast<HWND>(m_window->getInternal(XdevLInternalName("WIN32_DC")));
		if(nullptr == hDC) {
			XDEVL_MODULE_ERROR("Could not get native WIN32_DC information.\n");
			return ERR_ERROR;
		}
		nativeDisplay = (EGLNativeDisplayType)hDC;
		nativeWindow = (EGLNativeWindowType)hWnd;
#elif XDEVL_PLATFORM_LINUX

		Display* display = static_cast<Display*>(window->getInternal(XdevLInternalName("X11_DISPLAY")));
		if(nullptr == display) {
			XDEVL_MODULE_ERROR("Could not get native X11 display information.\n");
			return ERR_ERROR;
		}

		Window wnd = (Window)(window->getInternal(XdevLInternalName("X11_WINDOW")));
		if(None == wnd) {
			XDEVL_MODULE_ERROR("Could not get native X11 window information.\n");
			return ERR_ERROR;
		}
		nativeDisplay = (EGLNativeDisplayType)display;
		nativeWindow = (EGLNativeWindowType)wnd;
#endif


		m_eglDisplay = eglGetDisplay(nativeDisplay);
		if(m_eglDisplay == EGL_NO_DISPLAY) {
			XDEVL_MODULE_ERROR("Can't create egl display\n");
			return ERR_ERROR;
		}

		EGLBoolean ret = eglInitialize(m_eglDisplay, &m_major, &m_minor);
		if(ret != EGL_TRUE) {
			XDEVL_MODULE_ERROR("Can't initialize egl display\n");
			return ERR_ERROR;
		}

		ret = eglBindAPI(EGL_OPENGL_ES_API);
		if(ret != EGL_TRUE) {
			XDEVL_MODULE_ERROR("Can't bind egl API to EGL_OPENGL_ES_API\n");
			return ERR_ERROR;
		}

		if(!eglGetConfigs(m_eglDisplay, nullptr, 0, &count) || count < 1) {
			return ERR_ERROR;
		}

		EGLConfig* configs = new EGLConfig[count];
		ret = eglChooseConfig(m_eglDisplay, config_attribs, configs, count, &n);
		if(ret != EGL_TRUE) {
			return ERR_ERROR;
		}

		// TODO This is at the moment for only test purpose. As you can see only the first
		// config is used.
		for(i = 0; i < n; i++) {
			eglGetConfigAttrib(m_eglDisplay, configs[i], EGL_BUFFER_SIZE, &size);
			printf("Buffer size for config %d is %d\n", i, size);
			// For now just use the first one.
			m_eglConfig = configs[0];

		}
		delete [] configs;

		// If we do not found any configs stop here.
		if(m_eglConfig == nullptr) {
			return ERR_ERROR;
		}
		m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, EGL_NO_CONTEXT, context_attribs);
		if(m_eglContext == nullptr) {
			return ERR_ERROR;
		}

		m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, nativeWindow, nullptr);
		if(m_eglSurface == nullptr) {
			XDEVL_MODULE_ERROR("eglCreateWindowSurface failed\n");
			return ERR_ERROR;
		}

		if(eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext) == EGL_FALSE) {
			XDEVL_MODULE_ERROR("eglMakeCurrent failed\n");
		}

		return ERR_OK;
	}
	xdl_int XdevLOpenGLContextEGL::getAttributes(XdevLOpenGLContextAttributes& attributes) {
		attributes = m_attributes;
		return ERR_OK;
	}
	xdl_int XdevLOpenGLContextEGL::setAttributes(const XdevLOpenGLContextAttributes& attributes) {
		m_attributes = attributes;
		return ERR_OK;
	}
	xdl_int XdevLOpenGLContextEGL::makeCurrent(XdevLWindow* window) {
		eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);
		return ERR_ERROR;
	}
	xdl_int XdevLOpenGLContextEGL::swapBuffers() {
		eglSwapBuffers(m_eglDisplay, m_eglSurface);
		return ERR_ERROR;
	}
	void* XdevLOpenGLContextEGL::getProcAddress(const xdl_char* func) {
		return (void*)eglGetProcAddress(func);
	}
	xdl_int XdevLOpenGLContextEGL::setVSync(xdl_bool enableVSync) {
		return ERR_ERROR;
	}

}
