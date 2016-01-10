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
#include <XdevLOpenGLContext/XdevLOpenGLContextBase.h>

#include <SDL.h>

namespace xdl {

	static const XdevLString vendor {
		"www.codeposer.net"
	};
	static const XdevLString author {
		"Cengiz Terzibas"
	};
	static const XdevLString copyright {
		"(c) 2005 - 2014 Cengiz Terzibas."
	};
	static const XdevLString description {
		"Module to create a SDL OpenGL context."
	};
	static const XdevLString pluginName {
		"XdevLOpenGLContextSDL"
	};

	static const std::vector<XdevLModuleName> moduleNames	{
		XdevLModuleName("XdevLOpenGLContext")
	};

	/**
		@class XdevLOpenGLContextSDL
		@brief
		@author Cengiz Terzibas
	*/

	class XdevLOpenGLContextSDL : public XdevLOpenGLContextBase {

		public:
			XdevLOpenGLContextSDL(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor);
			virtual ~XdevLOpenGLContextSDL();

			virtual void* getInternal(const XdevLInternalName& id) override;
			virtual xdl_int shutdown() override;

			virtual void* getProcAddress(const xdl_char* func) override;
			virtual xdl_int getAttributes(XdevLOpenGLContextAttributes& attributes) override;
			virtual xdl_int setAttributes(const XdevLOpenGLContextAttributes& attributes) override;

			virtual xdl_int create(XdevLWindow* window) override;
			virtual xdl_int makeCurrent(XdevLWindow* window) override;
			virtual xdl_int swapBuffers() override;

			virtual xdl_int setVSync(xdl_bool enableVSync) override;

		private:
			xdl_bool initMultisample();
			virtual xdl_int setEnableFSAA(xdl_bool state);
		private:

			SDL_Window*		m_SDLWindow;
			SDL_GLContext 	m_OpenGLContext;
			SDL_GLContext 	m_previousOpenGLContext;
			xdl_uint m_fsaa;
	};

}


#endif
