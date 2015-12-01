#include <GL/glew.h>

#include <sstream>
#include <cstddef>
#include <iostream>

#include <XdevLCoreMediator.h>
#include <XdevLWindowDevice/XdevLWindowDevice.h>
#include <XdevLPlatform.h>
#include <XdevLXstring.h>
#include <XdevLUtils.h>
#include "XdevLOpenGLGLX.h"
#include <tinyxml.h>



extern "C" XDEVL_EXPORT int _create(xdl::XdevLModuleCreateParameter* parameter) {
	// Create the "OpenGL" module.
	if (std::string(xdl::XdevLOpenGLImpl::m_pluginDesc.getName()) == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLOpenGLGLX(parameter);

		if (!obj)
			return xdl::ERR_ERROR;

		parameter->setModuleInstance(obj);

		return xdl::ERR_OK;
	}

	return xdl::ERR_MODULE_NOT_FOUND;
}

namespace xdl {


XdevLOpenGLGLX::XdevLOpenGLGLX(XdevLModuleCreateParameter* parameter) : 
	XdevLOpenGLImpl(parameter),
	m_visualInfo(NULL),
	m_multisampleSupport(xdl_false){
}

XdevLOpenGLGLX::~XdevLOpenGLGLX() {
	shutdown();
}

xdl_int XdevLOpenGLGLX::shutdown() {
//	glXDestroyContext(m_Display, m_GLContext);
	return ERR_OK;
}

int XdevLOpenGLGLX::attach(XdevLWindowDevice* window) {
	
	// Let's check if the user wants multisampling(fullscreen anti aliasing)
	if(m_fsaa > 0){
			// Ok, he wants multisampling support. First we have to check if the graphics cards supports
			// multisampling. For this we have to create a dummy window which is hided and create an OpenGL 
			// context. After we did that we can check if the GLEW_ARB_multisample is supported.
			// If yes we can get the Pixelformat. Than destroy the dummy window and create a new OpenGL context
			// on the provided windows with the Pixelformat.
			IPXdevLWindowDevice win = static_cast<IPXdevLWindowDevice>(getMediator()->createModule("XdevLWindowDevice", "DUMMY_WINDOW", NULL));
			// We have to hide the window. The XdevLWindowDevice interface doesn't support a function to hide it directly
			// but supports it internal over an event.
			// TODO: The window is shown first and than hided. This should be fixed because causes flickering.
			XdevLQuark msg("XdevLWindowDeviceHide");
			XdevLEvent data;
			
			data.type = XDEVL_CORE_EVENT;
			
			XdevLInternalEvent ev(this, win, &msg, data);
			win->recvEvent(&ev);
			// Get the internal Window handle.
			Display* displaytmp = static_cast<Display*>(win->getInternal("UNIX_DISPLAY"));
			Window windowtmp = *(static_cast<Window*>(win->getInternal("UNIX_WINDOW")));
			if (initOpenGL(displaytmp, windowtmp) == ERR_ERROR) {
				XDEVL_MODULE_ERROR("Failed to initialize OpenGL.\n");
				getMediator()->deleteModule(win->getId());
				return ERR_ERROR;
			}else{
					initMultisample();
					if(m_multisampleSupport == xdl_false)
						m_fsaa = 0;
			}
			glXDestroyContext(displaytmp, m_GLContext);
			m_GLContext = NULL;
			// Destroy the dummy window.
			getMediator()->deleteModule(win->getId());
	}
	
	m_Display = static_cast<Display*>(window->getInternal("UNIX_DISPLAY"));
	if(NULL == m_Display){
		XDEVL_MODULE_ERROR("Could not get native UNIX_DISPLAY from XdevLWindowDevice.\n");
		return ERR_ERROR;
	}
	m_Window = *(static_cast<Window*>(window->getInternal("UNIX_WINDOW")));
		
	if(glXQueryVersion(m_Display, &m_glxMajorVersion, &m_glxMinorVersion) == False){
		XDEVL_MODULE_ERROR("glXQueryVersion failed.\n");
		return ERR_ERROR;
	}

	XDEVL_MODULE_INFO("GLX-Version " << m_glxMajorVersion << "."<< m_glxMajorVersion << std::endl);

	if (initOpenGL(m_Display, m_Window) == ERR_ERROR) {
		XDEVL_MODULE_ERROR("Failed to initialize OpenGL.\n");
		return ERR_ERROR;
	}

	if(glXIsDirect(m_Display, m_GLContext)) {
		XDEVL_MODULE_INFO("Direct Rendering supported.\n");

	} else {
		XDEVL_MODULE_WARNING("Direct Rendering not supported.\n");
	}


	setVSync(m_VSync);
	
	getOpenGLInfos();
	
	return ERR_OK;
}
xdl_int XdevLOpenGLGLX::initOpenGL(Display* display, Window window){
	// ---------------------------------------------------------------------------
	// Prepare the attribute values for the glXChooseVisual function
	//
	xdl_int color_element_size 	= m_ColorDepth/8;
	xdl_int color_red_size				= 4;
	xdl_int color_green_size			= 4;
	xdl_int color_blue_size				= 4;
	xdl_int color_alpha_size			= 0;
	
	switch(color_element_size){
		case 3:{ // We have RGB 8 bit values.
			color_red_size				= 8;
			color_green_size			= 8;
			color_blue_size				= 8;
			color_alpha_size			= 0;
		}break;
		case 4:{ // We have RGBA 8 bit values.
			color_red_size				= 8;
			color_green_size			= 8;
			color_blue_size				= 8;
			color_alpha_size			= 0;			
		}break;
		case 2: // Use 4 bit for each color and alpha = 0
		default:
		break;
	}
	
	std::vector<int> double_buffer_attribute_list;
	double_buffer_attribute_list.push_back(GLX_DOUBLEBUFFER);
	double_buffer_attribute_list.push_back(GLX_RGBA);
	double_buffer_attribute_list.push_back(GLX_RED_SIZE);
	double_buffer_attribute_list.push_back(color_red_size);
	double_buffer_attribute_list.push_back(GLX_GREEN_SIZE);
	double_buffer_attribute_list.push_back(color_green_size);
	double_buffer_attribute_list.push_back(GLX_BLUE_SIZE);
	double_buffer_attribute_list.push_back(color_blue_size);
	double_buffer_attribute_list.push_back(GLX_ALPHA_SIZE);
	double_buffer_attribute_list.push_back(color_alpha_size);	
	double_buffer_attribute_list.push_back(GLX_DEPTH_SIZE);
	double_buffer_attribute_list.push_back(m_ZDepth);
	double_buffer_attribute_list.push_back(GLX_STENCIL_SIZE);
	double_buffer_attribute_list.push_back(m_StencilDepth);
	if(m_multisampleSupport){
		if(GLXEW_ARB_multisample){
			XDEVL_MODULE_INFO("GLXEW_ARB_multisample supported.\n");
			double_buffer_attribute_list.push_back(GLX_SAMPLE_BUFFERS_ARB);
			double_buffer_attribute_list.push_back(GL_TRUE);
			double_buffer_attribute_list.push_back(GLX_SAMPLES_ARB);
			double_buffer_attribute_list.push_back(m_fsaa);	
		}
		else if(GLXEW_SGIS_multisample){
			XDEVL_MODULE_INFO("GLX_SAMPLE_BUFFERS_SGIS supported.\n");
			double_buffer_attribute_list.push_back(GLX_SAMPLE_BUFFERS_SGIS);
			double_buffer_attribute_list.push_back(GL_TRUE);
			double_buffer_attribute_list.push_back(GLX_SAMPLES_SGIS);
			double_buffer_attribute_list.push_back(m_fsaa);	
		}		
	}
		
	double_buffer_attribute_list.push_back(None);

	// TODO: This is the easiest way to create a visual for x11. You should do that in a better way cengiz.
	m_visualInfo = glXChooseVisual(display, DefaultScreen(display), &double_buffer_attribute_list[0]);
	if (NULL == m_visualInfo) {
		XDEVL_MODULE_ERROR("glXChooseVisual failed. Please try different framebuffer, depthbuffer, stencilbuffer values.\n");
		return ERR_ERROR;
	} 

	m_GLContext = glXCreateContext(display, m_visualInfo, 0, GL_TRUE);
	if(NULL == m_GLContext){
		XDEVL_MODULE_ERROR("glXCreateContext failed.\n");
		return ERR_ERROR;
	}
	
//	Colormap cmap = XCreateColormap(display, window, m_visualInfo->visual, AllocNone);
//	XSetWindowColormap(display, window, cmap);
	
	glXMakeCurrent(display, window, m_GLContext);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		XDEVL_MODULE_ERROR(glewGetErrorString(err) << std::endl);
	}
	
	return ERR_OK;
	
}

int XdevLOpenGLGLX::makeCurrent() {
	if(glXMakeCurrent(m_Display, m_Window, m_GLContext) == True)
		return ERR_OK;
	
	return ERR_ERROR;
}

void* XdevLOpenGLGLX::getInternal(const char* id) {
	std::string data(id);

	if (data == "GLX_CONTEXT")
		return m_GLContext;

	return NULL;
}

int XdevLOpenGLGLX::swapBuffers() {
	glXSwapBuffers(m_Display, m_Window);
	return ERR_OK;
}

xdl_int XdevLOpenGLGLX::setVSync(xdl_bool state){
	/*if(GLXEW_EXT_swap_control){
		if(state)
				glXSwapIntervalEXT(m_Display, m_GLContext,1);
			else
				glXSwapIntervalEXT(m_Display, m_GLContext,0);
	
      return ERR_OK;
	}else*/
	
  if(GLXEW_SGI_swap_control){
			if(state)
				glXSwapIntervalSGI(1);
			else
				glXSwapIntervalSGI(0);
	
      return ERR_OK;
  }else{
	  XDEVL_MODULE_WARNING("No VSync swap supported.\n");
  }

  return ERR_ERROR;
}

xdl_bool XdevLOpenGLGLX::initMultisample(){
	if(GLXEW_ARB_multisample){
		m_multisampleSupport = xdl_true;
		return xdl_true;		
	}
	else if(GLXEW_SGIS_multisample){
		m_multisampleSupport = xdl_true;
		return xdl_true;
	}
	m_multisampleSupport = xdl_false;
	return xdl_false;
}

xdl_int XdevLOpenGLGLX::setEnableFSAA(xdl_bool state){
	if(m_multisampleSupport == xdl_false)
		return ERR_ERROR;
		
	if(state)
		glEnable(GL_MULTISAMPLE);
	else
		glDisable(GL_MULTISAMPLE);
		
	return ERR_OK;
}

}
