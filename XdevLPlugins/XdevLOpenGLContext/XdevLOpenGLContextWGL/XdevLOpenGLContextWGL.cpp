
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

xdl::XdevLModuleDescriptor xdl::XdevLOpenGLWGL::m_moduleDescriptor{ vendor,
																	author,
																	moduleNames[0],
																	copyright,
																	description,
																	XdevLOpenGLContextWGLMajorVersion,
																	XdevLOpenGLContextWGLMinorVersion,
																	XdevLOpenGLContextWGLPatchVersion };

xdl::XdevLPluginDescriptor wglPluginDescriptor{ xdl::pluginName,
												xdl::moduleNames,
												xdl::XdevLOpenGLContextWGLPluginMajorVersion,
												xdl::XdevLOpenGLContextWGLPluginMinorVersion,
												xdl::XdevLOpenGLContextWGLPluginPatchVersion };

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	// Create the "OpenGL" module.
	if (xdl:: XdevLOpenGLWGL::m_moduleDescriptor.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLOpenGLWGL(parameter);

		if (!obj)
			return xdl::ERR_ERROR;

		parameter->setModuleInstance(obj);

		return xdl::ERR_OK;
	}

	return xdl::ERR_MODULE_NOT_FOUND;
}

extern "C" XDEVL_EXPORT void _delete(xdl::XdevLModule* obj) {
	if (obj)
		delete obj;
}

extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor()  {
	return &wglPluginDescriptor;
}


namespace xdl {

	XdevLOpenGLWGL::XdevLOpenGLWGL(XdevLModuleCreateParameter* parameter) : 
		XdevLOpenGLContextBase(parameter, m_moduleDescriptor),
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

	if(window == NULL){
		XDEVL_MODULE_ERROR("Parameter invalid.\n");
		return ERR_ERROR;
	}
	
	m_window = window;
	m_wnd = static_cast<HWND>(m_window->getInternal(XdevLInternalName("WIN32_HWND")));
	if ( m_wnd == NULL) {
		XDEVL_MODULE_ERROR("Get WIN32_HWND failed.\n");
		return ERR_ERROR;
	}

	if (initOpenGL(m_wnd) == ERR_ERROR) {
		XDEVL_MODULE_ERROR("Failed to initialize OpenGL.\n");
		return ERR_ERROR;
	}

	// Set vertical syncronisation.
	setVSync(getVSync());

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
//	wglSwapLayerBuffers(m_DC, 1);
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




	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int iPixelFormat = ChoosePixelFormat(m_DC, &pfd);
	if (iPixelFormat == 0)return false;

	if (!SetPixelFormat(m_DC, iPixelFormat, &pfd))return false;

	// Create the old style context (OpenGL 2.1 and before)
	HGLRC hRC = wglCreateContext(m_DC);
	wglMakeCurrent(m_DC, hRC);

	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(hRC);

	if (wglCreateContextAttribsARB) {
		const int iPixelFormatAttribList[] =
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			0 // End of attributes list
		};
		int iContextAttribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 2,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0 // End of attributes list
		};

		int iPixelFormat, iNumFormats;
		wglChoosePixelFormatARB(m_DC, iPixelFormatAttribList, NULL, 1, &iPixelFormat, (UINT*)&iNumFormats);

		// PFD seems to be only redundant parameter now
		if (SetPixelFormat(m_DC, iPixelFormat, &pfd) != TRUE) {
			return ERR_ERROR;
		}

		m_RC = wglCreateContextAttribsARB(m_DC, 0, iContextAttribs);
		// If everything went OK
		 wglMakeCurrent(m_DC, m_RC);

	}




	//PIXELFORMATDESCRIPTOR m_PFD;
	//m_PFD.nSize						= sizeof(PIXELFORMATDESCRIPTOR);
	//m_PFD.nVersion        = 1;
	//m_PFD.dwFlags = PFD_DRAW_TO_WINDOW |	// Format Must Support Window
	//				PFD_SUPPORT_OPENGL;	// Format Must Support OpenGL
	//if (m_attributes.double_buffer > 0) {
	//	m_PFD.dwFlags |= PFD_DOUBLEBUFFER;
	//}
	//m_PFD.iPixelType      = PFD_TYPE_RGBA;


	//m_PFD.cColorBits	= m_attributes.color_buffer_size;
	//m_PFD.cDepthBits    = m_attributes.depth_size;
	//m_PFD.cStencilBits  = m_attributes.stencil_size;

	//
	//m_PFD.cRedBits = m_attributes.red_size;
	//m_PFD.cRedShift       = 0;
	//m_PFD.cGreenBits = m_attributes.green_size;
	//m_PFD.cGreenShift     = 0;
	//m_PFD.cBlueBits = m_attributes.blue_size;
	//m_PFD.cBlueShift      = 0;
	//m_PFD.cAlphaBits = m_attributes.alpha_size;
	//m_PFD.cAlphaShift     = 0;
	//m_PFD.cAccumBits      = 0;
	//m_PFD.cAccumRedBits   = 0;
	//m_PFD.cAccumGreenBits = 0;
	//m_PFD.cAccumBlueBits  = 0;
	//m_PFD.cAccumAlphaBits = 0;
	//m_PFD.cAccumBits      = 0;
	//m_PFD.dwLayerMask     = PFD_MAIN_PLANE;
	//m_PFD.bReserved       = 0;
	//m_PFD.dwVisibleMask   = 0;
	//m_PFD.dwDamageMask    = 0;


	//if (!m_ARBMultisampleSupported) {
	//		m_ARBMultisampleFormat[0] = ChoosePixelFormat( m_DC, &m_PFD );
	//		if (m_ARBMultisampleFormat[0]==0) { // Let's choose a default index.
	//			m_ARBMultisampleFormat[0] = 1;
	//			if (DescribePixelFormat(m_DC, m_ARBMultisampleFormat[0], sizeof(PIXELFORMATDESCRIPTOR), &m_PFD)==0) {
	//				return ERR_ERROR;
	//			}
	//		}
	//	// Are We Able To Set The Pixel Format?
	//	if (SetPixelFormat( m_DC, m_ARBMultisampleFormat[0], &m_PFD ) == FALSE) {
	//		XDEVL_MODULE_ERROR("Could not set pixel format.\n");
	//		return ERR_ERROR;
	//	}

	//}
	//else{
		//xdl_bool found = xdl_false;
		//for(UINT num = 0; num < numFormats; ++num){
		//	// Are We Able To Set The Pixel Format?
		//	if (SetPixelFormat( m_DC, m_ARBMultisampleFormat[num], &m_PFD ) != TRUE) {
		//		continue;
		//	}else
		//		found = xdl_true;
		//}
		//if(found == xdl_false){
		//	XDEVL_MODULE_ERROR("Could not set pixel format.\n");
		//	return ERR_ERROR;		
		//}
	//}

	

	if (wglCreateContextAttribsARB){
		std::vector<GLint> attribList;
		if(m_major != -1 && m_minor != -1){
			attribList.push_back(WGL_CONTEXT_MAJOR_VERSION_ARB);
			attribList.push_back(m_major);
			attribList.push_back(WGL_CONTEXT_MINOR_VERSION_ARB);
			attribList.push_back(m_minor);
		}
		
		xdl_int context_flags = 0;
		attribList.push_back(WGL_CONTEXT_FLAGS_ARB);
		if(m_debugMode){
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
	return ERR_OK;
}



xdl_int XdevLOpenGLWGL::makeCurrent(XdevLWindow* window) {
	// Try To Activate The Rendering Context
	if (!wglMakeCurrent(m_DC, m_RC)) {
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

	xdl_int iAttributes[] = { 		
		WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
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
	if (wglSwapIntervalEXT){
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
//	if(m_ARBMultisampleSupported){
//		if(state)
//			glEnable(GL_MULTISAMPLE_ARB);
//		else{
//			glDisable(GL_MULTISAMPLE_ARB);
//		}
//		return ERR_OK;
//	}
	return ERR_ERROR;
}

}
