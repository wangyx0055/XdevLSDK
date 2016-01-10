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

#include <XdevLWindow/XdevLWindow.h>
#include "XdevLOpenGLContextEGL.h"

#if XDEVL_PLATFORM_WINDOWS
#include <Windows.h>

#elif XDEVL_PLATFORM_LINUX
#include <X11/Xlib.h>
#include <wayland-client.h>
#include <wayland-egl.h>
#endif

xdl::XdevLPluginDescriptor eglPluginDescriptor {
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

XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(eglPluginDescriptor);

XDEVL_PLUGIN_CREATE_MODULE {
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLOpenGLContextEGL, moduleDescriptor)
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}

namespace xdl {

	XdevLString eglGetErrorString(EGLenum error) {
		switch(error) {
			case EGL_SUCCESS:
				return XdevLString("EGL_SUCCESS");
			case EGL_NOT_INITIALIZED:
				return XdevLString("EGL_NOT_INITIALIZED");
			case EGL_BAD_ACCESS:
				return XdevLString("EGL_BAD_ACCESS");
			case EGL_BAD_ALLOC:
				return XdevLString("EGL_BAD_ALLOC");
			case EGL_BAD_ATTRIBUTE:
				return XdevLString("EGL_BAD_ATTRIBUTE");
			case EGL_BAD_CONTEXT:
				return XdevLString("EGL_BAD_CONTEXT");
			case EGL_BAD_CONFIG:
				return XdevLString("EGL_BAD_CONFIG");
			case EGL_BAD_CURRENT_SURFACE:
				return XdevLString("EGL_BAD_CURRENT_SURFACE");
			case EGL_BAD_DISPLAY:
				return XdevLString("EGL_BAD_DISPLAY");
			case EGL_BAD_SURFACE:
				return XdevLString("EGL_BAD_SURFACE");
			case EGL_BAD_MATCH:
				return XdevLString("EGL_BAD_MATCH");
			case EGL_BAD_PARAMETER:
				return XdevLString("EGL_BAD_PARAMETER");
			case EGL_BAD_NATIVE_PIXMAP:
				return XdevLString("EGL_BAD_NATIVE_PIXMAP");
			case EGL_BAD_NATIVE_WINDOW:
				return XdevLString("EGL_BAD_NATIVE_WINDOW");
			case EGL_CONTEXT_LOST:
				return XdevLString("EGL_CONTEXT_LOST");
		}
	}


	XdevLOpenGLContextEGL::XdevLOpenGLContextEGL(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		XdevLOpenGLContextBase(parameter, descriptor) {
	}

	XdevLOpenGLContextEGL::~XdevLOpenGLContextEGL() {

	}

	void* XdevLOpenGLContextEGL::getInternal(const XdevLInternalName& id) {
		if(id == XdevLString("EGL_DISPLAY")) {
			return (void*)m_eglDisplay;
		} else if(id == XdevLString("EGL_SURFACE")) {
			return (void*)m_eglSurface;
		} else if(id == XdevLString("EGL_CONTEXT")) {
			return (void*)m_eglContext;
		}
		return nullptr;
	}

	xdl_int XdevLOpenGLContextEGL::create(XdevLWindow* window) {

		EGLint major, minor, n, count;


		EGLNativeDisplayType nativeDisplay;
		EGLNativeWindowType nativeWindow;

#if XDEVL_PLATFORM_WINDOWS
		HINSTANCE hInstance = static_cast<HINSTANCE>(window->getInternal(XdevLInternalName("WIN32_INSTANCE")));
		if(nullptr == hInstance) {
			XDEVL_MODULE_ERROR("Could not get native WIN32_INSTANCE information.\n");
			return ERR_ERROR;
		}

		HWND	hWnd = static_cast<HWND>(window->getInternal(XdevLInternalName("WIN32_HWND")));
		if(nullptr == hWnd) {
			XDEVL_MODULE_ERROR("Could not get native WIN32_HWND information.\n");
			return ERR_ERROR;
		}
		HDC hDC = static_cast<HDC>(window->getInternal(XdevLInternalName("WIN32_DC")));
		if(nullptr == hDC) {
			XDEVL_MODULE_ERROR("Could not get native WIN32_DC information.\n");
			return ERR_ERROR;
		}
		nativeDisplay = (EGLNativeDisplayType)hDC;
		nativeWindow = (EGLNativeWindowType)hWnd;
#elif XDEVL_PLATFORM_LINUX

		Display* display = static_cast<Display*>(window->getInternal(XdevLInternalName("X11_DISPLAY")));
		if(nullptr == display) {
			wl_display* display = static_cast<wl_display*>(window->getInternal(XdevLInternalName("WAYLAND_DISPLAY")));
			if(nullptr == display) {
				XDEVL_MODULE_ERROR("Could not get native display information.\n");
				return ERR_ERROR;
			}
			wl_egl_window* wnd = (wl_egl_window*)(window->getInternal(XdevLInternalName("WAYLAND_WINDOW")));
			if(nullptr == wnd) {
				XDEVL_MODULE_ERROR("Could not get native window information.\n");
				return ERR_ERROR;
			}
			nativeDisplay = (EGLNativeDisplayType)display;
			nativeWindow = (EGLNativeWindowType)wnd;
		} else {

			Window wnd = (Window)(window->getInternal(XdevLInternalName("X11_WINDOW")));
			if(None == wnd) {
				XDEVL_MODULE_ERROR("Could not get native X11 window information.\n");
				return ERR_ERROR;
			}
			nativeDisplay = (EGLNativeDisplayType)display;
			nativeWindow = (EGLNativeWindowType)wnd;
		}
#endif

		//
		// Get EGL display from the native display type.
		//
		m_eglDisplay = eglGetDisplay(nativeDisplay);
		if(m_eglDisplay == EGL_NO_DISPLAY) {
			XDEVL_MODULE_ERROR("Can't create egl display\n");
			return ERR_ERROR;
		}

		//
		// Now initialize EGL platform.
		//
		EGLBoolean ret = eglInitialize(m_eglDisplay, &m_major, &m_minor);
		if(ret != EGL_TRUE) {
			XDEVL_MODULE_ERROR("eglInitialize failed: " << eglGetErrorString(eglGetError()) << "\n");
			return ERR_ERROR;
		}

		xdl_int api = 0;
		if( (m_attributes.context_profile_mask == XDEVL_OPENGL_CONTEXT_ES1) ||
		    (m_attributes.context_profile_mask == XDEVL_OPENGL_CONTEXT_ES2)) {
			api = EGL_OPENGL_ES_API;
		} else if(	(m_attributes.context_profile_mask == XDEVL_OPENGL_CONTEXT_CORE_PROFILE) ||
		            (m_attributes.context_profile_mask == XDEVL_OPENGL_CONTEXT_COMPATIBILITY) ) {
			api = EGL_OPENGL_API;
		} else {
			XDEVL_MODULE_ERROR("Unkown context profile.\n");
			return ERR_ERROR;
		}

		ret = eglBindAPI(api);
		if(ret != EGL_TRUE) {
			XDEVL_MODULE_ERROR("eglBindAPI failed: " << eglGetErrorString(eglGetError()) << "\n");
			return ERR_ERROR;
		}

		if(!eglGetConfigs(m_eglDisplay, nullptr, 0, &count) || count < 1) {
			return ERR_ERROR;
		}


		// ---------------------------------------------------------------------------
		// Prepare the attribute values for the glXChooseVisual function
		//
		std::vector<int> attribute_list {
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RED_SIZE, m_attributes.red_size,
			EGL_GREEN_SIZE, m_attributes.green_size,
			EGL_BLUE_SIZE, m_attributes.blue_size,
			EGL_ALPHA_SIZE, m_attributes.alpha_size,
			EGL_BUFFER_SIZE, m_attributes.color_buffer_size,
			EGL_DEPTH_SIZE, m_attributes.depth_size,
			EGL_STENCIL_SIZE,m_attributes.stencil_size,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
			EGL_NONE
		};

		xdl_int numberOfConfigs = 0;
		EGLConfig* configs = new EGLConfig[count];
		ret = eglChooseConfig(m_eglDisplay, attribute_list.data(), configs, count, &numberOfConfigs);
		if(ret != EGL_TRUE) {
			XDEVL_MODULE_ERROR("eglChooseConfig failed: " << eglGetErrorString(eglGetError()) << "\n");
			return ERR_ERROR;
		}

		// TODO This is at the moment for only test purpose. As you can see only the first
		// config is used.
		for(xdl_int i = 0; i < numberOfConfigs; i++) {
			xdl_int bsize = 0;
			xdl_int dsize = 0;
			xdl_int ssize = 0;

			eglGetConfigAttrib(m_eglDisplay, configs[i], EGL_BUFFER_SIZE, &bsize);
			eglGetConfigAttrib(m_eglDisplay, configs[i], EGL_DEPTH_SIZE, &dsize);
			eglGetConfigAttrib(m_eglDisplay, configs[i], EGL_STENCIL_SIZE, &ssize);
			printf("Configuration: %d color buffer: %d, depth buffer: %d, stencil buffer: %d\n", i, bsize, dsize, ssize);
			// For now just use the first one.
			m_eglConfig = configs[0];

		}
		delete [] configs;

		// If we do not found any configs stop here.
		if(m_eglConfig == nullptr) {
			return ERR_ERROR;
		}

		//
		// Create EGL context.
		//
		std::vector<EGLint> context_attribs {
//			EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
//			EGL_CONTEXT_MAJOR_VERSION, 3,
//			EGL_CONTEXT_MINOR_VERSION, 2,
//			EGL_NONE
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
		};

		m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, EGL_NO_CONTEXT, context_attribs.data());
		if(m_eglContext == nullptr) {
			XDEVL_MODULE_ERROR("eglCreateContext failed: " << eglGetErrorString(eglGetError()) << "\n");
			return ERR_ERROR;
		}

		m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, nativeWindow, nullptr);
		if(m_eglSurface == nullptr) {
			XDEVL_MODULE_ERROR("eglCreateWindowSurface failed: " << eglGetErrorString(eglGetError()) << "\n");
			return ERR_ERROR;
		}

		if(eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext) == EGL_FALSE) {
			XDEVL_MODULE_ERROR("eglMakeCurrent failed: " << eglGetErrorString(eglGetError()) << "\n");
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
		if(eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext) == EGL_FALSE) {
			XDEVL_MODULE_ERROR("eglMakeCurrent failed\n");
			return ERR_ERROR;
		}
		return ERR_OK;
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
