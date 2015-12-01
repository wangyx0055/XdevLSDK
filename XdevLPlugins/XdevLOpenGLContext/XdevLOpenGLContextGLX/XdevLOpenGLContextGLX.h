#ifndef XDEVL_OPENGL_COCOA_H
#define XDEVL_OPENGL_COCOA_H


#include <XdevLPluginImpl.h>
#include <XdevLOpenGLContext/XdevLOpenGLContext.h>
#include <XdevLOpenGLContext/XdevLOpenGLContextBase.h>

#include <X11/Xlib.h>

#include <GL/glx.h>
#include <GL/glxext.h>
#include <GL/glext.h>

namespace xdl {

	static const XdevLString glx_context_vendor {
		"www.codeposer.net"
	};
	static const XdevLString glx_context_author {
		"Cengiz Terzibas"
	};
	static const XdevLString glx_context_copyright {
		"(c) 2005 - 2014 Cengiz Terzibas."
	};
	static const XdevLString glx_description {
		"Module to create a GLX OpenGL context."
	};
	static const XdevLString glx_context_pluginName {
		"XdevLOpenGLContextGLX"
	};

	static const std::vector<XdevLModuleName> glx_context_moduleNames	{
																			XdevLModuleName("XdevLOpenGLContext")
																		};

	// Holds the Major version number.
	const xdl_int XdevLOpenGLContextGLXPluginMajorVersion = XDEVLOPENGLCONTEXT_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_int XdevLOpenGLContextGLXPluginMinorVersion = XDEVLOPENGLCONTEXT_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_int XdevLOpenGLContextGLXPluginPatchVersion = XDEVLOPENGLCONTEXT_PATCH_VERSION;


	// Holds the Major version number.
	const xdl_int XdevLOpenGLContexGLXMajorVersion = XDEVLOPENGLCONTEXT_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_int XdevLOpenGLContextGLXMinorVersion = XDEVLOPENGLCONTEXT_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_int XdevLOpenGLContextGLXPatchVersion = XDEVLOPENGLCONTEXT_MODULE_PATCH_VERSION;


	typedef void ( * PFNGLXSWAPINTERVALEXTPROC) (Display* dpy, GLXDrawable drawable, int interval);
	typedef int ( * PFNGLXSWAPINTERVALSGIPROC) (int interval);

	/**
		@class XdevLOpenGLContextGLX
		@brief
		@author Cengiz Terzibas
	*/

	class XdevLOpenGLContextGLX : public XdevLOpenGLContextBase{

		public:
			XdevLOpenGLContextGLX(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLOpenGLContextGLX();

			static XdevLModuleDescriptor m_moduleDescriptor;

			virtual void* getInternal(const XdevLInternalName& id) override;
			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;

			virtual void* getProcAddress(const xdl_char* func);
			virtual xdl_int getAttributes(XdevLOpenGLContextAttributes& attributes);
			virtual xdl_int setAttributes(const XdevLOpenGLContextAttributes& attributes);

			virtual xdl_int create(XdevLWindow* window);
			virtual xdl_int makeCurrent(XdevLWindow* window);
			virtual xdl_int swapBuffers();

			virtual xdl_int setVSync(xdl_bool enableVSync) override;

		private:
			xdl_bool initMultisample();
			xdl_int initOpenGL(Display* display, Window window);
			virtual xdl_int setEnableFSAA(xdl_bool enableFSAA);
		private:
			//
			// X11 related variables.
			//
			Display* 			m_display;
			Window				m_window;
			xdl_int				m_screenNumber;

			//
			// GLX related variables.
			//
			GLXContext 		m_glxContext;
			xdl_int			m_glxMajorVersion;
			xdl_int			m_glxMinorVersion;
			XVisualInfo* 	m_visualInfo;

			PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribs;
			PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT;
			PFNGLXSWAPINTERVALSGIPROC glXSwapIntervalSGI;
			
			std::vector<XdevLString> extensionsList;
	};

}


#endif
