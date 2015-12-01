#ifndef XDEVL_OPENGL_WGL_H
#define XDEVL_OPENGL_WGL_H

#include <tinyxml.h>
#include <XdevLPlatform.h>
#include <XdevLPlugin.h>
#include "XdevLOpenGLImpl.h"

namespace xdl {

/**
	@class XdevLOpenGLWGL
	@brief Core class for OpenGL support under Windows.
	@author Cengiz Terzibas
*/
class XdevLOpenGLWGL : public XdevLOpenGLImpl {
public:
	XdevLOpenGLWGL(XdevLModuleCreateParameter* parameter);
	virtual ~XdevLOpenGLWGL();
	virtual void* getInternal(const xdl_char* id);
	virtual xdl_int swapBuffers();
	virtual xdl_int attach(XdevLWindowDevice* window);
	virtual xdl_int makeCurrent();
	virtual xdl_int shutdown();
	virtual xdl_int setVSync(xdl_bool state);	
	virtual xdl_int setEnableFSAA(xdl_bool state);	
	virtual xdl_int reset();
protected:
	/// Holds the win32 window handle.
	HWND				m_wnd;
	/// Holds the win32 graphics context.
	HDC					m_DC;
	/// Holds the OpenGL render context.
	HGLRC				m_RC;
	/// Holds  state if FSAA is supported.
	xdl_bool	m_ARBMultisampleSupported;
	///for saving old display settings when changing mode
	DEVMODE m_oldDevMode;
	xdl_int color_alpha_size;	
private:
	///initializes win32 OpenGL
	xdl_int initOpenGL(HWND hwnd);
	/// Initialize FSAA.
	bool initMultisample();
	
	UINT numFormats;
	xdl_int m_ARBMultisampleFormat[64];

};

}

#endif
