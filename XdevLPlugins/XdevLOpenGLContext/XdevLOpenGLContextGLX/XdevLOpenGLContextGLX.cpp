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

xdl::XdevLModuleDescriptor xdl::XdevLOpenGLContextGLX::m_moduleDescriptor{	glx_context_vendor, 
																																						glx_context_author, 
																																						glx_context_moduleNames[0], 
																																						glx_context_copyright, 
																																						glx_description, 
																																						XdevLOpenGLContexGLXMajorVersion, 
																																						XdevLOpenGLContextGLXMinorVersion,
																																						XdevLOpenGLContextGLXPatchVersion};

xdl::XdevLPluginDescriptor pluginDescriptor{	xdl::glx_context_pluginName,
																								xdl::glx_context_moduleNames,
																								xdl::XdevLOpenGLContextGLXPluginMajorVersion,
																								xdl::XdevLOpenGLContextGLXPluginMinorVersion,
																								xdl::XdevLOpenGLContextGLXPluginPatchVersion};


extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	if(xdl::XdevLOpenGLContextGLX::m_moduleDescriptor.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLOpenGLContextGLX(parameter);
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


	XdevLOpenGLContextGLX::XdevLOpenGLContextGLX(XdevLModuleCreateParameter* parameter) :
		XdevLModuleAutoImpl(parameter, m_moduleDescriptor),
		m_visualInfo(NULL) {
	}

	XdevLOpenGLContextGLX::~XdevLOpenGLContextGLX() {
		shutdown();
	}

	xdl_int XdevLOpenGLContextGLX::shutdown() {
		glXDestroyContext(m_display, m_glxContext);
		return ERR_OK;
	}

//	void* XdevLOpenGLContextGLX::getProcAddress(const xdl_char* func) {
//		return glXGetProcAddress((const GLubyte *)func);
//	}

	xdl_int XdevLOpenGLContextGLX::getAttributes(XdevLOpenGLContextAttributes& attributes) {
		attributes = m_attributes;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLContextGLX::setAttributes(const XdevLOpenGLContextAttributes& attributes) {
		m_attributes = attributes;
		return ERR_OK;
	}

	int XdevLOpenGLContextGLX::create(XdevLWindow* window) {

//		// Let's check if the user wants multisampling(fullscreen anti aliasing)
//		if(m_fsaa > 0) {
//			// Ok, he wants multisampling support. First we have to check if the graphics cards supports
//			// multisampling. For this we have to create a dummy window which is hided and create an OpenGL
//			// context. After we did that we can check if the GLEW_ARB_multisample is supported.
//			// If yes we can get the Pixelformat. Than destroy the dummy window and create a new OpenGL context
//			// on the provided windows with the Pixelformat.
//			IPXdevLWindowDevice win = static_cast<IPXdevLWindowDevice>(getMediator()->createModule("XdevLWindowDevice", "DUMMY_WINDOW", NULL));
//			// We have to hide the window. The XdevLWindowDevice interface doesn't support a function to hide it directly
//			// but supports it internal over an event.
//			// TODO: The window is shown first and than hided. This should be fixed because causes flickering.
//			XdevLQuark msg("XdevLWindowDeviceHide");
//			XdevLEvent data;
//
//			data.type = XDEVL_CORE_EVENT;
//
//			XdevLInternalEvent ev(this, win, &msg, data);
//			win->recvEvent(&ev);
//			// Get the internal Window handle.
//			Display* displaytmp = static_cast<Display*>(win->getInternal("UNIX_DISPLAY"));
//			Window windowtmp = *(static_cast<Window*>(win->getInternal("UNIX_WINDOW")));
//			if(initOpenGL(displaytmp, windowtmp) == ERR_ERROR) {
//				XDEVL_MODULE_ERROR("Failed to initialize OpenGL.\n");
//				getMediator()->deleteModule(win->getId());
//				return ERR_ERROR;
//			} else {
//				initMultisample();
//				if(m_multisampleSupport == xdl_false)
//					m_fsaa = 0;
//			}
//			glXDestroyContext(displaytmp, m_glContext);
//			m_glContext = NULL;
//			// Destroy the dummy window.
//			getMediator()->deleteModule(win->getId());
//		}

		m_display = static_cast<Display*>(window->getInternal(XdevLInternalName("UNIX_DISPLAY")));
		if(NULL == m_display) {
			XDEVL_MODULE_ERROR("Could not get native UNIX_DISPLAY.\n");
			return ERR_ERROR;
		}

		m_window = *(static_cast<Window*>(window->getInternal(XdevLInternalName("UNIX_WINDOW"))));
		if(None == m_display) {
			XDEVL_MODULE_ERROR("Could not get native UNIX_WINDOW.\n");
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


//		setVSync(m_VSync);

		//getOpenGLInfos();

		return ERR_OK;
	}

	xdl_int XdevLOpenGLContextGLX::initOpenGL(Display* display, Window window) {

		std::vector<int> double_buffer_attribute_list;
		// ---------------------------------------------------------------------------
		// Prepare the attribute values for the glXChooseVisual function
		//

		double_buffer_attribute_list.push_back(GLX_DRAWABLE_TYPE);
		double_buffer_attribute_list.push_back(GLX_WINDOW_BIT);
		double_buffer_attribute_list.push_back(GLX_RENDER_TYPE);
		double_buffer_attribute_list.push_back(GLX_RGBA_BIT);
		double_buffer_attribute_list.push_back(GLX_RGBA);

		if(m_attributes.double_buffer > 0) {
			double_buffer_attribute_list.push_back(GLX_DOUBLEBUFFER);
			double_buffer_attribute_list.push_back(True);
		}

		double_buffer_attribute_list.push_back(GLX_RED_SIZE);
		double_buffer_attribute_list.push_back(m_attributes.red_size);

		double_buffer_attribute_list.push_back(GLX_GREEN_SIZE);
		double_buffer_attribute_list.push_back(m_attributes.green_size);

		double_buffer_attribute_list.push_back(GLX_BLUE_SIZE);
		double_buffer_attribute_list.push_back(m_attributes.blue_size);

		double_buffer_attribute_list.push_back(GLX_ALPHA_SIZE);
		double_buffer_attribute_list.push_back(m_attributes.alpha_size);

		double_buffer_attribute_list.push_back(GLX_ACCUM_RED_SIZE);
		double_buffer_attribute_list.push_back(m_attributes.accum_red_size);

		double_buffer_attribute_list.push_back(GLX_ACCUM_GREEN_SIZE);
		double_buffer_attribute_list.push_back(m_attributes.accum_green_size);

		double_buffer_attribute_list.push_back(GLX_ACCUM_BLUE_SIZE);
		double_buffer_attribute_list.push_back(m_attributes.accum_blue_size);

		double_buffer_attribute_list.push_back(GLX_ACCUM_ALPHA_SIZE);
		double_buffer_attribute_list.push_back(m_attributes.accum_alpha_size);

		double_buffer_attribute_list.push_back(GLX_DEPTH_SIZE);
		double_buffer_attribute_list.push_back(m_attributes.depth_size);

		double_buffer_attribute_list.push_back(GLX_STENCIL_SIZE);
		double_buffer_attribute_list.push_back(m_attributes.stencil_size);


		if(m_attributes.stereo > 0) {
			double_buffer_attribute_list.push_back(GLX_STEREO);
			double_buffer_attribute_list.push_back(True);
		}
		
		if(m_attributes.multisample_buffers > 0) {
			double_buffer_attribute_list.push_back(GLX_SAMPLE_BUFFERS_ARB);
			double_buffer_attribute_list.push_back(GL_TRUE);
			double_buffer_attribute_list.push_back(GLX_SAMPLES_ARB);
			double_buffer_attribute_list.push_back(GL_TRUE);
			double_buffer_attribute_list.push_back(m_attributes.multisample_buffers);
		}

//		if(m_multisampleSupport) {
//			if(GLXEW_ARB_multisample) {
//				XDEVL_MODULE_INFO("GLXEW_ARB_multisample supported.\n");
//				double_buffer_attribute_list.push_back(GLX_SAMPLE_BUFFERS_ARB);
//				double_buffer_attribute_list.push_back(GL_TRUE);
//				double_buffer_attribute_list.push_back(GLX_SAMPLES_ARB);
//				double_buffer_attribute_list.push_back(m_fsaa);
//			} else if(GLXEW_SGIS_multisample) {
//				XDEVL_MODULE_INFO("GLX_SAMPLE_BUFFERS_SGIS supported.\n");
//				double_buffer_attribute_list.push_back(GLX_SAMPLE_BUFFERS_SGIS);
//				double_buffer_attribute_list.push_back(GL_TRUE);
//				double_buffer_attribute_list.push_back(GLX_SAMPLES_SGIS);
//				double_buffer_attribute_list.push_back(m_fsaa);
//			}
//		}

		double_buffer_attribute_list.push_back(None);

		//
		// Set OpenGL 3.0 > attributes.
		//
		std::vector<int> opengl_profile_attribute_list;
		opengl_profile_attribute_list.push_back(GLX_CONTEXT_MAJOR_VERSION_ARB);
		opengl_profile_attribute_list.push_back(m_attributes.context_major_version);
		opengl_profile_attribute_list.push_back(GLX_CONTEXT_MINOR_VERSION_ARB);
		opengl_profile_attribute_list.push_back(m_attributes.context_minor_version);

		if(m_attributes.context_flags != XDEVL_OPENGL_CONTEXT_FLAGS_NONE) {
			opengl_profile_attribute_list.push_back(GLX_CONTEXT_FLAGS_ARB);
			opengl_profile_attribute_list.push_back(m_attributes.context_flags);

		}
		opengl_profile_attribute_list.push_back(None);


		xdl_int elemc;
		GLXFBConfig *fbcfg = glXChooseFBConfig(display, DefaultScreen(display), NULL, &elemc);
		if(!fbcfg) {
			throw std::string("Couldn't get FB configs\n");
		} else {
			XDEVL_MODULE_INFO("Got " << elemc << " FBConfigs\n");
		}

		// TODO: This is the easiest way to create a visual for x11. You should do that in a better way cengiz.
		m_visualInfo = glXChooseVisual(display, DefaultScreen(display), &double_buffer_attribute_list[0]);
		if(NULL == m_visualInfo) {
			XDEVL_MODULE_ERROR("glXChooseVisual failed. Please try different framebuffer, depthbuffer, stencilbuffer values.\n");
			return ERR_ERROR;
		}

		PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribs = (PFNGLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddress((GLubyte *)"glXCreateContextAttribsARB");
		glXCreateContextAttribs(display, fbcfg[0], NULL, true, &opengl_profile_attribute_list[0]);

		m_glxContext = glXCreateContext(display, m_visualInfo, 0, GL_TRUE);
		if(NULL == m_glxContext) {
			XDEVL_MODULE_ERROR("glXCreateContext failed.\n");
			return ERR_ERROR;
		}

		return ERR_OK;
	}

	int XdevLOpenGLContextGLX::makeCurrent(XdevLWindow* window) {

		Display* xdisplay = static_cast<Display*>(window->getInternal(XdevLInternalName("UNIX_DISPLAY")));
		if(NULL == xdisplay) {
			return ERR_ERROR;
		}

		Window xwindow = *(static_cast<Window*>(window->getInternal(XdevLInternalName("UNIX_WINDOW"))));
		if(None == xdisplay) {
			return ERR_ERROR;
		}

		if(glXMakeCurrent(xdisplay, xwindow, m_glxContext) == True)
			return ERR_OK;

		return ERR_ERROR;
	}

	void* XdevLOpenGLContextGLX::getInternal(const XdevLInternalName& id) {

		if(id.toString() == "GLX_CONTEXT")
			return m_glxContext;

		return NULL;
	}

	int XdevLOpenGLContextGLX::swapBuffers() {
		glXSwapBuffers(m_display, m_window);
		return ERR_OK;
	}

	xdl_int XdevLOpenGLContextGLX::setVSync(xdl_bool state) {
		/*if(GLXEW_EXT_swap_control){
			if(state)
					glXSwapIntervalEXT(m_Display, m_glContext,1);
				else
					glXSwapIntervalEXT(m_Display, m_glContext,0);

		    return ERR_OK;
		}else*/

//		if(GLXEW_SGI_swap_control) {
//			if(state)
//				glXSwapIntervalSGI(1);
//			else
//				glXSwapIntervalSGI(0);
//
//			return ERR_OK;
//		} else {
//			XDEVL_MODULE_WARNING("No VSync swap supported.\n");
//		}

		return ERR_ERROR;
	}

	xdl_bool XdevLOpenGLContextGLX::initMultisample() {
//		if(GLXEW_ARB_multisample) {
//			m_multisampleSupport = xdl_true;
//			return xdl_true;
//		} else if(GLXEW_SGIS_multisample) {
//			m_multisampleSupport = xdl_true;
//			return xdl_true;
//		}
//		m_multisampleSupport = xdl_false;
		return xdl_false;
	}

	xdl_int XdevLOpenGLContextGLX::setEnableFSAA(xdl_bool state) {
//		if(m_multisampleSupport == xdl_false)
//			return ERR_ERROR;
//
//		if(state)
//			glEnable(GL_MULTISAMPLE);
//		else
//			glDisable(GL_MULTISAMPLE);

		return ERR_OK;
	}

}
