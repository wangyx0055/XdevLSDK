#ifndef XDEVL_OPENGL_CONTEXT_WGL_H
#define XDEVL_OPENGL_CONTEXT_WGL_H

#include <XdevLPluginImpl.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLOpenGLContext/XdevLOpenGLContextBase.h>

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

	//
	// Create Context ARB
	//
	#define WGL_CONTEXT_DEBUG_BIT_ARB 0x0001
	#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
	#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
	#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
	#define WGL_CONTEXT_FLAGS_ARB 0x2094



	//
	// WGL_ARB_create_context_profile
	//
	#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
	#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
	#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126


	typedef HGLRC(WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);
	typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);
	typedef BOOL(WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int* piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);







	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;

/**
	@class XdevLOpenGLWGL
	@brief Core class for OpenGL support under Windows.
	@author Cengiz Terzibas
*/
	class XdevLOpenGLWGL : public XdevLOpenGLContextBase {
public:
	XdevLOpenGLWGL(XdevLModuleCreateParameter* parameter);

	static XdevLModuleDescriptor m_moduleDescriptor;

	virtual ~XdevLOpenGLWGL();

	virtual xdl_int init();
	virtual xdl_int shutdown();
	virtual void* getInternal(const xdl_char* id);

	virtual xdl_int create(XdevLWindow* window);
	virtual xdl_int getAttributes(XdevLOpenGLContextAttributes& attributes);
	virtual xdl_int setAttributes(const XdevLOpenGLContextAttributes& attributes);
	virtual xdl_int makeCurrent(XdevLWindow* window);
	virtual xdl_int swapBuffers();
	virtual void* getProcAddress(const xdl_char* func);
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
	XdevLString m_profile;
};

}

#endif
