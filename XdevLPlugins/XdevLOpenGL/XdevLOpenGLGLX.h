#ifndef XDEVL_OPENGL_GLX_H
#define XDEVL_OPENGL_GLX_H

#if defined(__linux__)
	#include <X11/Xlib.h>
	#include <X11/extensions/Xrandr.h>
#endif

#include <tinyxml.h>
#include <GL/glxew.h>
#include <XdevLListener.h>
#include "XdevLOpenGLImpl.h"


namespace xdl {

/**
	@class XdevLOpenGLGLX
	@brief Core class for opengl support under x11.
	@author Cengiz Terzibas
*/

class XdevLOpenGLGLX : public XdevLOpenGLImpl {

public:
	XdevLOpenGLGLX(XdevLModuleCreateParameter* parameter);
	virtual ~XdevLOpenGLGLX();
	virtual void* getInternal(const char* id);
	virtual int swapBuffers();
	virtual int attach(XdevLWindowDevice* window);
	virtual int makeCurrent();
	virtual xdl_int shutdown();
	virtual xdl_int setVSync(xdl_bool state);
	xdl_bool initMultisample();
	xdl_int initOpenGL(Display* display, Window window);
	virtual xdl_int setEnableFSAA(xdl_bool state);	
private:
	Display* 			m_Display;
	Window				m_Window;
	int						m_Screen;
	GLXContext 		m_GLContext;
	XVisualInfo* 	m_visualInfo;
	xdl_int 			m_glxMajorVersion;
	xdl_int				m_glxMinorVersion;	
	

	xdl_bool			m_multisampleSupport;
};

}


#endif
