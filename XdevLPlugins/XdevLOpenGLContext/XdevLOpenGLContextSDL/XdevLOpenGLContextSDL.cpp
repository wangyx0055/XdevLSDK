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

#include <sstream>
#include <cstddef>
#include <iostream>

#include <XdevLCoreMediator.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLPlatform.h>
#include <XdevLXstring.h>
#include <XdevLUtils.h>
#include "XdevLOpenGLContextSDL.h"
#include <tinyxml.h>

xdl::XdevLModuleDescriptor moduleDescriptor {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[0],
	xdl::copyright,
	xdl::description,
	XDEVLOPENGLCONTEXT_MAJOR_VERSION,
	XDEVLOPENGLCONTEXT_MINOR_VERSION,
	XDEVLOPENGLCONTEXT_PATCH_VERSION
};

xdl::XdevLPluginDescriptor pluginDescriptor {
	xdl::pluginName,
	xdl::moduleNames,
	XDEVLOPENGLCONTEXT_MODULE_MAJOR_VERSION,
	XDEVLOPENGLCONTEXT_MODULE_MINOR_VERSION,
	XDEVLOPENGLCONTEXT_MODULE_PATCH_VERSION
};

XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(pluginDescriptor);

XDEVL_PLUGIN_CREATE_MODULE {
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLOpenGLContextSDL, moduleDescriptor)
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}

namespace xdl {

	XdevLOpenGLContextSDL::XdevLOpenGLContextSDL(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		XdevLOpenGLContextBase(parameter, descriptor),
		m_SDLWindow(nullptr),
		m_OpenGLContext(nullptr),
		m_previousOpenGLContext(nullptr) {
	}

	XdevLOpenGLContextSDL::~XdevLOpenGLContextSDL() {
		XDEVL_ASSERT(m_OpenGLContext == nullptr, "XdevLOpenGLContextSDL::shutdown wasn't called. The OpenGL context might be not destroyed.");
	}

	xdl_int XdevLOpenGLContextSDL::shutdown() {
		SDL_GL_DeleteContext(m_OpenGLContext);
		m_OpenGLContext = nullptr;
		return ERR_OK;
	}

	void* XdevLOpenGLContextSDL::getProcAddress(const xdl_char* func) {
		//	return reinterpret_cast<void*>(glXGetProcAddress((const GLubyte *)func));
		return nullptr;
	}

	xdl_int XdevLOpenGLContextSDL::getAttributes(XdevLOpenGLContextAttributes& attributes) {
		attributes = m_attributes;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLContextSDL::setAttributes(const XdevLOpenGLContextAttributes& attributes) {
		m_attributes = attributes;
		return ERR_OK;
	}

	int XdevLOpenGLContextSDL::create(XdevLWindow* window) {
		if(SDL_GL_LoadLibrary(nullptr)) {
			XDEVL_MODULE_ERROR("Could not load OpenGL library: " << SDL_GetError() << "\n");
		}

		m_SDLWindow = (static_cast<SDL_Window*>(window->getInternal(XdevLInternalName("SDL_Window"))));
		if(m_SDLWindow == nullptr) {
			XDEVL_MODULE_INFO("Could not get a valid SDL_Window. SDL_Window: " << window << std::endl);
			return ERR_ERROR;
		}

		XDEVL_MODULE_INFO("Setting OpenGL context to version: " << m_attributes.context_major_version << "." << m_attributes.context_minor_version << "\n")


		if(m_attributes.context_profile_mask == XDEVL_OPENGL_CONTEXT_CORE_PROFILE) {
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, m_attributes.context_major_version);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, m_attributes.context_minor_version);
		} else if(m_attributes.context_profile_mask == XDEVL_OPENGL_CONTEXT_COMPATIBILITY) {
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
		} else if(m_attributes.context_profile_mask == XDEVL_OPENGL_CONTEXT_ES1) {

		}

		if(m_attributes.double_buffer > 0) {
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		}

		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,		m_attributes.color_buffer_size);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,		m_attributes.depth_size);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,	m_attributes.stencil_size);

		if(m_attributes.context_flags & XDEVL_OPENGL_CONTEXT_FLAGS_DEBUG_BIT) {
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
		}

		// TODO Share with context?
		SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

		XDEVL_MODULE_INFO("Creating OpenGL context.\n")
		m_OpenGLContext = SDL_GL_CreateContext(m_SDLWindow);
		if(m_OpenGLContext == NULL) {
			XDEVL_MODULE_ERROR("Could not create OpenGL context.\n");
			return ERR_ERROR;
		}
		XDEVL_MODULE_SUCCESS("Creating OpenGL context was successful.\n")

		SDL_GL_MakeCurrent(m_SDLWindow, m_OpenGLContext);

		setVSync(getVSync());

		return ERR_OK;
	}

	int XdevLOpenGLContextSDL::makeCurrent(XdevLWindow* window) {
		if(SDL_GL_MakeCurrent(m_SDLWindow, m_OpenGLContext) != 0) {
			XDEVL_MODULE_ERROR("SDL_GL_MakeCurrent failed: " << SDL_GetError() << std::endl);
			return ERR_ERROR;
		}
		return ERR_ERROR;
	}

	void* XdevLOpenGLContextSDL::getInternal(const XdevLInternalName& id) {

		return nullptr;
	}

	int XdevLOpenGLContextSDL::swapBuffers() {
		SDL_GL_SwapWindow(m_SDLWindow);
		return ERR_OK;
	}

	xdl_int XdevLOpenGLContextSDL::setVSync(xdl_bool enableVSync) {
		xdl_int state = enableVSync ? SDL_TRUE : SDL_FALSE;
		SDL_GL_SetSwapInterval(state);
		return ERR_OK;
	}


	xdl_bool XdevLOpenGLContextSDL::initMultisample() {
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, m_fsaa);
		return xdl_false;
	}

	xdl_int XdevLOpenGLContextSDL::setEnableFSAA(xdl_bool state) {

		return ERR_OK;
	}

}
