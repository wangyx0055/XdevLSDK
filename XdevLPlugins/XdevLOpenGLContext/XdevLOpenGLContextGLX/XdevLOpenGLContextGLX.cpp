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

#include <sstream>
#include <cstddef>
#include <iostream>

#include <XdevLCoreMediator.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLPlatform.h>
#include <XdevLXstring.h>
#include <XdevLUtils.h>
#include "XdevLOpenGLContextGLX.h"
#include <tinyxml.h>



xdl::XdevLModuleDescriptor moduleDescriptor {
	xdl::XdevLString("www.codeposer.net"),
	xdl::XdevLString("Cengiz Terzibas"),
	xdl::moduleNames[0],
	xdl::XdevLString("(c) 2005 - 2016 Cengiz Terzibas."),
	xdl::XdevLString("Module to create a GLX OpenGL context."),
	XDEVLOPENGLCONTEXT_MAJOR_VERSION,
	XDEVLOPENGLCONTEXT_MINOR_VERSION,
	XDEVLOPENGLCONTEXT_PATCH_VERSION
};

xdl::XdevLPluginDescriptor glxPluginDescriptor {
	xdl::XdevLString("XdevLOpenGLContextGLX"),
	xdl::moduleNames,
	XDEVLOPENGLCONTEXT_MODULE_MAJOR_VERSION,
	XDEVLOPENGLCONTEXT_MODULE_MINOR_VERSION,
	XDEVLOPENGLCONTEXT_MODULE_PATCH_VERSION
};

XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(glxPluginDescriptor);

XDEVL_PLUGIN_CREATE_MODULE {
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLOpenGLContextGLX, moduleDescriptor)
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}

namespace xdl {

	static xdl_bool contextErrorOccured = xdl_false;
	static int contextErrorHanlder(Display* display, XErrorEvent* ev) {
		contextErrorOccured = xdl_true;
		return 0;
	}

	XdevLOpenGLContextGLX::XdevLOpenGLContextGLX(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		XdevLOpenGLContextBase(parameter, descriptor),
		m_display(nullptr),
		m_window(None),
		m_screenNumber(0),
		m_glxContext(nullptr),
		m_glxMajorVersion(0),
		m_glxMinorVersion(0),
		m_visualInfo(nullptr) {
	}

	XdevLOpenGLContextGLX::~XdevLOpenGLContextGLX() {
		XDEVL_ASSERT(m_glxContext == nullptr, "XdevLOpenGLContextGLX::shutdown wasn't called. The OpenGL context might be not destroyed.");
	}

	xdl_int XdevLOpenGLContextGLX::init() {

		glXCreateContextAttribs = (PFNGLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddress((GLubyte *)"glXCreateContextAttribsARB");
		glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddress((GLubyte *)"glXSwapIntervalEXT");
		glXSwapIntervalSGI = (PFNGLXSWAPINTERVALSGIPROC)glXGetProcAddress((GLubyte *)"glXSwapIntervalSGI");


		return XdevLOpenGLContextBase::init();
	}

	xdl_int XdevLOpenGLContextGLX::shutdown() {

		//
		// Free resources
		//
		glXDestroyContext(m_display, m_glxContext);
		XFree(m_visualInfo);

		//
		// Reset all used variables.
		//
		m_display = nullptr;
		m_window = None;
		m_screenNumber = 0;
		m_glxMajorVersion = 0;
		m_glxMinorVersion = 0;
		m_glxContext = nullptr;
		m_visualInfo = nullptr;
		return ERR_OK;
	}

	void* XdevLOpenGLContextGLX::getProcAddress(const xdl_char* func) {
		return reinterpret_cast<void*>(glXGetProcAddress((const GLubyte *)func));
	}

	xdl_int XdevLOpenGLContextGLX::getAttributes(XdevLOpenGLContextAttributes& attributes) {
		attributes = m_attributes;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLContextGLX::setAttributes(const XdevLOpenGLContextAttributes& attributes) {
		m_attributes = attributes;
		return ERR_OK;
	}

	int XdevLOpenGLContextGLX::create(XdevLWindow* window) {
		XDEVL_ASSERT(m_display == nullptr, "XdevLOpenGLContextGLX already created.");

		m_display = static_cast<Display*>(window->getInternal(XdevLInternalName("X11_DISPLAY")));
		if(nullptr == m_display) {
			XDEVL_MODULE_ERROR("Could not get native X11 display information.\n");
			return ERR_ERROR;
		}

		m_window = (Window)(window->getInternal(XdevLInternalName("X11_WINDOW")));
		if(None == m_display) {
			XDEVL_MODULE_ERROR("Could not get native X11 window information.\n");
			return ERR_ERROR;
		}

		if(glXQueryVersion(m_display, &m_glxMajorVersion, &m_glxMinorVersion) == False) {
			XDEVL_MODULE_ERROR("glXQueryVersion failed.\n");
			return ERR_ERROR;
		}

		if(initOpenGL(m_display, m_window) == ERR_ERROR) {
			XDEVL_MODULE_ERROR("Failed to initialize OpenGL.\n");
			return ERR_ERROR;
		}

		if(glXIsDirect(m_display, m_glxContext)) {
			XDEVL_MODULE_INFO("Direct Rendering supported.\n");

		} else {
			XDEVL_MODULE_WARNING("Direct Rendering not supported.\n");
		}

		setVSync(getVSync());

		return ERR_OK;
	}

	xdl_int XdevLOpenGLContextGLX::initOpenGL(Display* display, Window window) {

		XDEVL_MODULE_INFO("Supported GLX extensions ---------------\n");
		std::string tmp(glXQueryExtensionsString(display, DefaultScreen(display)));
		std::vector<std::string> exlist;
		xstd::tokenize(tmp, exlist, " ");
		for(auto extension : exlist) {
			extensionsList.push_back(XdevLString(extension));
			std::cout << extension << std::endl;
		}
		XDEVL_MODULE_INFO("-----------------------------------------\n");


		std::vector<int> attribute_list;
		// ---------------------------------------------------------------------------
		// Prepare the attribute values for the glXChooseVisual function
		//

		// We are going to use the normal RGBA color type, not the color indext type.
		attribute_list.push_back(GLX_RENDER_TYPE);
		attribute_list.push_back(GLX_RGBA_BIT);

		// This window is going to use only Window type, that means we can't use PBuffers.
		// Valid values are GLX WINDOW BIT, GLX PIXMAP BIT, GLX PBUFFER BIT. All can be used at the same time.
		attribute_list.push_back(GLX_DRAWABLE_TYPE);
		attribute_list.push_back(GLX_WINDOW_BIT);

		if(m_attributes.double_buffer > 0) {
			attribute_list.push_back(GLX_DOUBLEBUFFER);
		}

		attribute_list.push_back(GLX_RED_SIZE);
		attribute_list.push_back(m_attributes.red_size);

		attribute_list.push_back(GLX_GREEN_SIZE);
		attribute_list.push_back(m_attributes.green_size);

		attribute_list.push_back(GLX_BLUE_SIZE);
		attribute_list.push_back(m_attributes.blue_size);

		attribute_list.push_back(GLX_ALPHA_SIZE);
		attribute_list.push_back(m_attributes.alpha_size);

		attribute_list.push_back(GLX_ACCUM_RED_SIZE);
		attribute_list.push_back(m_attributes.accum_red_size);

		attribute_list.push_back(GLX_ACCUM_GREEN_SIZE);
		attribute_list.push_back(m_attributes.accum_green_size);

		attribute_list.push_back(GLX_ACCUM_BLUE_SIZE);
		attribute_list.push_back(m_attributes.accum_blue_size);

		attribute_list.push_back(GLX_ACCUM_ALPHA_SIZE);
		attribute_list.push_back(m_attributes.accum_alpha_size);

		attribute_list.push_back(GLX_BUFFER_SIZE);
		attribute_list.push_back(m_attributes.color_buffer_size);

		attribute_list.push_back(GLX_DEPTH_SIZE);
		attribute_list.push_back(m_attributes.depth_size);

		attribute_list.push_back(GLX_STENCIL_SIZE);
		attribute_list.push_back(m_attributes.stencil_size);


		if(m_attributes.stereo > 0) {
			attribute_list.push_back(GLX_STEREO);
			attribute_list.push_back(True);
		}

		if(m_attributes.multisample_buffers > 0) {
			attribute_list.push_back(GLX_SAMPLE_BUFFERS_ARB);
			attribute_list.push_back(GL_TRUE);
			attribute_list.push_back(GLX_SAMPLES_ARB);
			attribute_list.push_back(m_attributes.multisample_samples);
		}

		attribute_list.push_back(None);


		//
		// Get all supported visual configurations.
		//
		xdl_int elemc;
		GLXFBConfig *fbcfg = glXChooseFBConfig(display, DefaultScreen(display), nullptr, &elemc);
		if(!fbcfg) {
			XDEVL_MODULE_ERROR("glXChooseFBConfig failed\n");
			return ERR_ERROR;
		}

		//
		// Not get the most close one the user specified.
		//
		int best_fbc = -1;
		int worst_fbc = -1;
		int best_num_samples = -1;
		int worst_num_samples = 999;

		for(auto i = 0; i < elemc; i++) {
			XVisualInfo* vi = glXGetVisualFromFBConfig(display, fbcfg[i]);
			if(nullptr != vi) {
				int sample_buffer, samples;
				glXGetFBConfigAttrib(display, fbcfg[i], GLX_SAMPLE_BUFFERS, &sample_buffer);
				glXGetFBConfigAttrib(display, fbcfg[i], GLX_SAMPLES, &samples);
				if(best_fbc < 0 || sample_buffer && samples > best_num_samples) {
					best_fbc = i;
					best_num_samples = samples;
				}
				if(worst_fbc < 0 || !sample_buffer && samples < worst_num_samples) {
					worst_fbc = i;
					worst_num_samples = samples;
				}
			}
			XFree(vi);
		}

		GLXFBConfig bestFbc = fbcfg[best_fbc];
		XFree(fbcfg);

		m_visualInfo = glXGetVisualFromFBConfig(display, bestFbc);

//		// TODO: This is the easiest way to create a visual for X11. You should do that in a better way cengiz.
//		m_visualInfo = glXChooseVisual(display, DefaultScreen(display), attribute_list.data());
//		if(nullptr == m_visualInfo) {
//			XDEVL_MODULE_ERROR("glXChooseVisual failed. Please try different framebuffer, depthbuffer, stencilbuffer values.\n");
//			return ERR_ERROR;
//		}


		//
		// Set OpenGL 3.0 > attributes.
		//
		std::vector<int> opengl_profile_attribute_list;
		opengl_profile_attribute_list.push_back(GLX_CONTEXT_MAJOR_VERSION_ARB);
		opengl_profile_attribute_list.push_back(m_attributes.context_major_version);
		opengl_profile_attribute_list.push_back(GLX_CONTEXT_MINOR_VERSION_ARB);
		opengl_profile_attribute_list.push_back(m_attributes.context_minor_version);

		if( (m_attributes.context_flags & XDEVL_OPENGL_CONTEXT_FLAGS_DEBUG_BIT) || (m_attributes.context_flags & XDEVL_OPENGL_CONTEXT_FLAGS_FORWARD_COMPATIBLE_BIT)) {
			opengl_profile_attribute_list.push_back(GLX_CONTEXT_FLAGS_ARB);

			if(m_attributes.context_flags == XDEVL_OPENGL_CONTEXT_FLAGS_DEBUG_BIT) {
				opengl_profile_attribute_list.push_back(GLX_CONTEXT_DEBUG_BIT_ARB);
			} else if(m_attributes.context_flags == XDEVL_OPENGL_CONTEXT_FLAGS_FORWARD_COMPATIBLE_BIT) {
				opengl_profile_attribute_list.push_back(GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB);
			}
		}

		if( (m_attributes.context_profile_mask == XDEVL_OPENGL_CONTEXT_CORE_PROFILE) || (m_attributes.context_profile_mask == XDEVL_OPENGL_CONTEXT_COMPATIBILITY)) {
			opengl_profile_attribute_list.push_back(GLX_CONTEXT_PROFILE_MASK_ARB);
			if(m_attributes.context_profile_mask == XDEVL_OPENGL_CONTEXT_CORE_PROFILE) {
				opengl_profile_attribute_list.push_back(GLX_CONTEXT_CORE_PROFILE_BIT_ARB);
			} else if(m_attributes.context_profile_mask == XDEVL_OPENGL_CONTEXT_COMPATIBILITY) {
				opengl_profile_attribute_list.push_back(GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB);
			}
		}
		opengl_profile_attribute_list.push_back(None);

		contextErrorOccured = false;
		int (*oldErrorHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&contextErrorHanlder);

		if(nullptr != glXCreateContextAttribs) {
			m_glxContext = glXCreateContextAttribs(display, bestFbc, nullptr, true, opengl_profile_attribute_list.data());
		} else {
			m_glxContext = glXCreateContext(display, m_visualInfo, nullptr, GL_TRUE);
		}
		XSync(display, False);
		if(xdl_true == contextErrorOccured) {
			m_glxContext = glXCreateContext(display, m_visualInfo, nullptr, GL_TRUE);
		}

		if(nullptr == m_glxContext) {
			XDEVL_MODULE_ERROR("glXCreateContext failed.\n");
			return ERR_ERROR;
		}

		XSync(display, False);
		XSetErrorHandler(oldErrorHandler);

		setEnableFSAA(xdl_true);

		//
		// Make it current.
		//
		if(glXMakeCurrent(display, window, m_glxContext) == False) {
			return ERR_ERROR;
		}

		return ERR_OK;
	}

	int XdevLOpenGLContextGLX::makeCurrent(XdevLWindow* window) {

		Display* xdisplay = static_cast<Display*>(window->getInternal(XdevLInternalName("X11_DISPLAY")));
		if(nullptr == xdisplay) {
			return ERR_ERROR;
		}

		Window xwindow = (Window)(window->getInternal(XdevLInternalName("X11_WINDOW")));
		if(None == xdisplay) {
			return ERR_ERROR;
		}

		if(glXMakeCurrent(xdisplay, xwindow, m_glxContext) == True)
			return ERR_OK;

		return ERR_ERROR;
	}

	void* XdevLOpenGLContextGLX::getInternal(const XdevLInternalName& id) {

		if(id.toString() == "GLX_CONTEXT") {
			return m_glxContext;
		} else if(id.toString() == "X11_VisualInfo") {
			return m_visualInfo;
		}
		return nullptr;
	}

	int XdevLOpenGLContextGLX::swapBuffers() {
		glXSwapBuffers(m_display, m_window);
		return ERR_OK;
	}

	xdl_int XdevLOpenGLContextGLX::setVSync(xdl_bool enableVSync) {
		xdl_int state = enableVSync ? 1 : 0;
		if(glXSwapIntervalEXT) {
			GLXDrawable drawable = glXGetCurrentDrawable();
			glXSwapIntervalEXT(m_display, drawable, state);
		}
		return ERR_OK;
	}


	xdl_bool XdevLOpenGLContextGLX::initMultisample() {
		return xdl_false;
	}

	xdl_int XdevLOpenGLContextGLX::setEnableFSAA(xdl_bool enableFSAA) {
		enableFSAA ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);
		return ERR_OK;
	}

}
