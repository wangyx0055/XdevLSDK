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
		"(c) 2005 - 2016 Cengiz Terzibas."
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
