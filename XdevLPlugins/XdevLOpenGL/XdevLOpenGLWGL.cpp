
#include "XdevLOpenGLWGL.h"
#include <XdevLCoreMediator.h>
#include <XdevLWindowDevice/XdevLWindowDevice.h>
#include <XdevLXstring.h>
#include <XdevLPlatform.h>
#include <GL/wglew.h>
extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	// Create the "OpenGL" module.
	if (std::string(xdl::XdevLOpenGLImpl::m_pluginDesc.getName()) == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLOpenGLWGL(parameter);

		if (!obj)
			return xdl::ERR_ERROR;

		parameter->setModuleInstance(obj);

		return xdl::ERR_OK;
	}

	return xdl::ERR_MODULE_NOT_FOUND;
}

namespace xdl {

XdevLOpenGLWGL::XdevLOpenGLWGL(XdevLModuleCreateParameter* parameter) : XdevLOpenGLImpl(parameter),
		m_wnd(NULL),
		m_DC(NULL),
		m_RC(NULL),
		m_ARBMultisampleSupported(false),
		color_alpha_size(0){
}

XdevLOpenGLWGL::~XdevLOpenGLWGL() {
}

xdl_int XdevLOpenGLWGL::attach(XdevLWindowDevice* window) {
	if(window == NULL){
		XDEVL_MODULE_ERROR("Parameter invalid.\n");
		return ERR_ERROR;
	}
	
	m_window = window;
	m_wnd = static_cast<HWND>(m_window->getInternal("WIN32_HWND"));
	if ( m_wnd == NULL) {
		XDEVL_MODULE_ERROR("Get WIN32_HWND failed.\n");
		return ERR_ERROR;
	}
	
	// Let's check if the user wants multisampling(fullscreen anti aliasing)
	if(m_fsaa > 0){
			// Ok, he wants multisampling support. First we have to check if the graphics cards supports
			// multisampling. For this we have to create a dummy window which is hided and create an OpenGL 
			// context. After we did that we can check if the GLEW_ARB_multisample is supported.
			// If yes we can get the Pixelformat. Than destroy the dummy window and create a new OpenGL context
			// on the provided windows with the Pixelformat.
			IPXdevLWindowDevice win = static_cast<IPXdevLWindowDevice>(getMediator()->createModule("XdevLWindow", "XdevLWindowDevice", "DUMMY_WINDOW", NULL));
			// We have to hide the window. The XdevLWindowDevice interface doesn't support a function to hide it directly
			// but supports it internal over an event.
			// TODO: The window is shown first and than hided. This should be fixed because causes flickering.
			XdevLQuark msg("XdevLWindowDeviceHide");
			XdevLEvent ev(this, win, &msg, NULL);
			win->recvEvent(&ev);
			// Get the internal Window handle.
			HWND tmpWnd = static_cast<HWND>(win->getInternal("WIN32_HWND"));
			if (initOpenGL(tmpWnd) == ERR_ERROR) {
				XDEVL_MODULE_ERROR("Failed to initialize OpenGL.\n");
				getMediator()->deleteModule(win->getId());
				return ERR_ERROR;
			}else{
				if (GLEW_ARB_multisample) {
					m_ARBMultisampleSupported = initMultisample();
					if(m_ARBMultisampleSupported == xdl_false){
						m_fsaa = 0;
						XDEVL_MODULE_WARNING("FSSA is not supported. Ignoring request.\n");
					}
				}	
			}
			// Destroy the dummy window.
			getMediator()->deleteModule(win->getId());
	}

	if (initOpenGL(m_wnd) == ERR_ERROR) {
		XDEVL_MODULE_ERROR("Failed to initialize OpenGL.\n");
		return ERR_ERROR;
	}else{
		if(m_ARBMultisampleSupported == xdl_true && (m_fsaa > 0)){
			glEnable(GL_MULTISAMPLE_ARB);
		}
	}

	// Set vertical syncronisation.
	setVSync(m_VSync);

	// Get all OpenGL driver information, like vendor, version and extensions.
	getOpenGLInfos();

	// TODO: This shouldn't be done maybe because it changes the initial state of the OpenGL context.
	glClearColor(0.0, 0.0, 0.0, 1.0);
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

xdl_int XdevLOpenGLWGL::swapBuffers() {

	SwapBuffers(m_DC);
	return ERR_OK;
}

xdl_int XdevLOpenGLWGL::reset(){
	shutdown();
	
	if(initOpenGL(m_wnd) == ERR_ERROR)
		return ERR_ERROR;
	
	return ERR_OK;
}

xdl_int XdevLOpenGLWGL::initOpenGL(HWND hwnd) {
	m_DC = GetDC(hwnd);
	if(m_DC == NULL){
		XDEVL_MODULE_ERROR("GetDC() failed.");
		return ERR_ERROR;
	}

	xdl_int color_element_size 	= m_ColorDepth/8;
	xdl_int color_red_size				= 4;
	xdl_int color_green_size			= 4;
	xdl_int color_blue_size				= 4;
	
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
			color_alpha_size			= 8;			
		}break;
		case 2: // Use 4 bit for each color and alpha = 0
		default:
		break;
	}

	PIXELFORMATDESCRIPTOR m_PFD;
	m_PFD.nSize						= sizeof(PIXELFORMATDESCRIPTOR);
	m_PFD.nVersion        = 1;
	m_PFD.dwFlags         = PFD_DRAW_TO_WINDOW |	// Format Must Support Window
	                        PFD_SUPPORT_OPENGL |	// Format Must Support OpenGL
	                        PFD_DOUBLEBUFFER;			// Double Buffering
	m_PFD.iPixelType      = PFD_TYPE_RGBA;
	m_PFD.cColorBits			= m_ColorDepth;
	m_PFD.cDepthBits      = static_cast<xdl_uchar>(m_ZDepth);
	m_PFD.cStencilBits    = static_cast<xdl_uchar>(m_StencilDepth);
	m_PFD.cRedBits        = 0;
	m_PFD.cRedShift       = 0;
	m_PFD.cGreenBits      = 0;
	m_PFD.cGreenShift     = 0;
	m_PFD.cBlueBits       = 0;
	m_PFD.cBlueShift      = 0;
	m_PFD.cAlphaBits      = color_alpha_size;
	m_PFD.cAlphaShift     = 0;
	m_PFD.cAccumBits      = 0;
	m_PFD.cAccumRedBits   = 0;
	m_PFD.cAccumGreenBits = 0;
	m_PFD.cAccumBlueBits  = 0;
	m_PFD.cAccumAlphaBits = 0;
	m_PFD.cAccumBits      = 0;
	m_PFD.dwLayerMask     = PFD_MAIN_PLANE;
	m_PFD.bReserved       = 0;
	m_PFD.dwLayerMask     = 0;
	m_PFD.dwVisibleMask   = 0;
	m_PFD.dwDamageMask    = 0;


	if (!m_ARBMultisampleSupported) {
			m_ARBMultisampleFormat[0] = ChoosePixelFormat( m_DC, &m_PFD );
			if (m_ARBMultisampleFormat[0]==0) { // Let's choose a default index.
				m_ARBMultisampleFormat[0] = 1;
				if (DescribePixelFormat(m_DC, m_ARBMultisampleFormat[0], sizeof(PIXELFORMATDESCRIPTOR), &m_PFD)==0) {
					return ERR_ERROR;
				}
			}
		// Are We Able To Set The Pixel Format?
		if (SetPixelFormat( m_DC, m_ARBMultisampleFormat[0], &m_PFD ) == FALSE) {
			XDEVL_MODULE_ERROR("Could not set pixel format.\n");
			return ERR_ERROR;
		}

	}
	else{
		xdl_bool found = xdl_false;
		for(UINT num = 0; num < numFormats; ++num){
			// Are We Able To Set The Pixel Format?
			if (SetPixelFormat( m_DC, m_ARBMultisampleFormat[num], &m_PFD ) != TRUE) {
				continue;
			}else
				found = xdl_true;
		}
		if(found == xdl_false){
			XDEVL_MODULE_ERROR("Could not set pixel format.\n");
			return ERR_ERROR;		
		}
	}

	

	if(WGLEW_ARB_create_context){
		std::vector<GLint> attribList;
		if(m_major != -1 && m_minor != -1){
			attribList.push_back(WGL_CONTEXT_MAJOR_VERSION_ARB);
			attribList.push_back(m_major);
			attribList.push_back(WGL_CONTEXT_MINOR_VERSION_ARB);
			attribList.push_back(m_minor);
		}
		
		xdl_int context_flags = 0;
		attribList.push_back(WGL_CONTEXT_FLAGS_ARB);
		if(m_debug){
			context_flags |= WGL_CONTEXT_DEBUG_BIT_ARB;
		}
		attribList.push_back(context_flags);
		
		attribList.push_back(WGL_CONTEXT_PROFILE_MASK_ARB);
		if(m_profile == "core"){
			attribList.push_back(WGL_CONTEXT_CORE_PROFILE_BIT_ARB);
		}else if(m_profile == "compatibility"){
			attribList.push_back(WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB);
		}else{
			attribList.push_back(WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB);
		}
		attribList.push_back(0);


		if (( m_RC=wglCreateContextAttribsARB( m_DC,0, &attribList[0] )) == 0) {
			XDEVL_MODULE_ERROR("Could not create GL context.\n");
			return ERR_ERROR;
		}
	}else{
		// Are We Able To Get A Rendering Context?
		if (( m_RC=wglCreateContext( m_DC )) == 0) {
			XDEVL_MODULE_ERROR("Could not create GL context.\n");
			return ERR_ERROR;
		}
	}


	// Try To Activate The Rendering Context
	if (wglMakeCurrent( m_DC, m_RC) == FALSE) {
		XDEVL_MODULE_ERROR("Could not make GL context to the current context.\n");
		return ERR_ERROR;
	}

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		XDEVL_MODULE_ERROR("GLEW: %s\n" <<  glewGetErrorString(err) << std::endl);
	}


	return ERR_OK;
}



xdl_int XdevLOpenGLWGL::makeCurrent() {
	// Try To Activate The Rendering Context
	if (!wglMakeCurrent( m_DC, m_RC)) {
		return ERR_ERROR;
	}
	return ERR_OK;
}

bool XdevLOpenGLWGL::initMultisample() {

	bool valid;

	float fAttributes[] = {0,0};

	xdl_int color_element_size 	= m_ColorDepth/8;
	xdl_int color_red_size				= 4;
	xdl_int color_green_size			= 4;
	xdl_int color_blue_size				= 4;
	
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
			color_alpha_size			= 8;			
		}break;
		case 2: // Use 4 bit for each color and alpha = 0
		default:
		break;
	}

	xdl_int iAttributes[] = { 		WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
		WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB,m_ColorDepth,
		WGL_ALPHA_BITS_ARB,color_alpha_size,
		WGL_DEPTH_BITS_ARB,m_ZDepth,
		WGL_STENCIL_BITS_ARB,m_StencilDepth,
		WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
		WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
		WGL_SAMPLES_ARB,m_fsaa,
		0,0};

                        
	valid = wglChoosePixelFormatARB(m_DC, iAttributes, fAttributes, 64, m_ARBMultisampleFormat, &numFormats);
	
// If We Returned True, And Our Format Count Is Greater Than 1
	if (valid && numFormats >= 1) {
		m_ARBMultisampleSupported = true;
		return true;
	}
	return  false;
}

xdl_int XdevLOpenGLWGL::setVSync(xdl_bool state){
	if(WGLEW_EXT_swap_control){
		if(state)
			wglSwapIntervalEXT(1);
		else
			wglSwapIntervalEXT(0);
		
		return ERR_OK;
	}else{
		XDEVL_MODULE_WARNING("VSync not supported.\n");
	}
	return ERR_ERROR;
}

xdl_int XdevLOpenGLWGL::setEnableFSAA(xdl_bool state){
	if(m_ARBMultisampleSupported){
		if(state)
			glEnable(GL_MULTISAMPLE_ARB);
		else{
			glDisable(GL_MULTISAMPLE_ARB);
		}
		return ERR_OK;
	}
	return ERR_ERROR;
}

}
