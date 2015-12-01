#ifndef XDEVL_OPENGL_WGL_H
#define XDEVL_OPENGL_WGL_H

#include <XdevLWindow/XdevLWindow.h>
#include <XdevLPluginImpl.h>
#include <XdevLOpenGLContext/XdevLOpenGLContext.h>

namespace xdl {

	static const XdevLString vendor							{ "www.codeposer.net" };
	static const XdevLString author							{ "Cengiz Terzibas" };
	static const XdevLString copyright						{ "(c) 2005 - 2015 Cengiz Terzibas." };
	static const XdevLString description					{ "Module to create a WGL OpenGL context." };
	static const XdevLString pluginName						{ "XdevLOpenGLContextWGL" };

	static const std::vector<XdevLModuleName>	moduleNames	{
																XdevLModuleName("XdevLOpenGLContext")
															};

	// Holds the Major version number.
	const xdl_uint XdevLOpenGLContextWGLPluginMajorVersion = 0;
	// Holds the Minor version number.
	const xdl_uint XdevLOpenGLContextWGLPluginMinorVersion = 1;
	// Holds the Patch version number.
	const xdl_uint XdevLOpenGLContextWGLPluginPatchVersion = 0;

	// Holds the Major version number.
	const xdl_uint XdevLOpenGLContextWGLMajorVersion = 0;
	// Holds the Minor version number.
	const xdl_uint XdevLOpenGLContextWGLMinorVersion = 1;
	// Holds the Patch version number.
	const xdl_uint XdevLOpenGLContextWGLPatchVersion = 0;

/**
	@class XdevLOpenGLWGL
	@brief Core class for OpenGL support under Windows.
	@author Cengiz Terzibas
*/
	class XdevLOpenGLWGL : public XdevLModuleAutoImpl<XdevLOpenGLContext> {
public:
	XdevLOpenGLWGL(XdevLModuleCreateParameter* parameter);

	static XdevLModuleDescriptor m_moduleDescriptor;

	virtual ~XdevLOpenGLWGL();
	virtual void* getInternal(const xdl_char* id);

	virtual xdl_int getAttributes(XdevLOpenGLContextAttributes& attributes);
	virtual xdl_int setAttributes(const XdevLOpenGLContextAttributes& attributes);
	virtual xdl_int create(XdevLWindow* window);
	virtual xdl_int makeCurrent(XdevLWindow* window);

	virtual xdl_int swapBuffers();

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

	IPXdevLWindow m_window;
	xdl_uint m_ColorDepth;
	xdl_uint m_ZDepth;
	xdl_uint m_StencilDepth;
	xdl_uint m_major, m_minor;
	xdl_uint m_fsaa;
	xdl_bool m_debug;
	XdevLString m_profile;
	xdl_int m_VSync;
	XdevLOpenGLContextAttributes m_attributes;
};

}

#endif
