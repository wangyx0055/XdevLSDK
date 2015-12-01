/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2014 Cengiz Terzibas

	This library is free software; you can redistribute it and/or modify it under the
	terms of the GNU Lesser General Public License as published by the Free Software
	Foundation; either version 2.1 of the License, or (at your option) any later version.
	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License along with
	this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
	Suite 330, Boston, MA 02111-1307 USA

	I would appreciate if you report all bugs to: cengiz@terzibas.de
*/

#ifndef XDEVL_OPENGL_SDL_H
#define XDEVL_OPENGL_SDL_H


#include <tinyxml.h>
#include <XdevLListener.h>
#include "XdevLOpenGLImpl.h"

#include <SDL.h>

namespace xdl {

	/**
		@class XdevLOpenGLSDL
		@brief Core class for opengl support under X11 using SDL.
		@author Cengiz Terzibas
	*/

	class XdevLOpenGLSDL : public XdevLOpenGLImpl {

		public:
			XdevLOpenGLSDL(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLOpenGLSDL();
			virtual void* getInternal(const XdevLInternalName& id) override;
			virtual int swapBuffers();
			virtual int createContext(XdevLWindow* window);
			virtual int makeCurrent();
			virtual xdl_int init();
			virtual xdl_int shutdown();
			virtual xdl_int setVSync(xdl_bool state);
			virtual xdl_int setEnableFSAA(xdl_bool state);
			virtual xdl_int makeCurrent(XdevLWindow* window);
			virtual xdl_int releaseCurrent();
		private:
			xdl_bool initMultisample();
		private:
			SDL_Window*		m_SDLWindow;
			SDL_GLContext m_OpenGLContext;
			SDL_GLContext m_previousOpenGLContext;
	};

}


#endif
