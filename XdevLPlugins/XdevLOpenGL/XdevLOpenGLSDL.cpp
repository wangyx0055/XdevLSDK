#include <GL/glew.h>

#include <sstream>
#include <cstddef>
#include <iostream>

#include <XdevLCoreMediator.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLPlatform.h>
#include <XdevLXstring.h>
#include <XdevLUtils.h>
#include "XdevLOpenGLSDL.h"
#include "XdevLOpenGLUtils.h"
#include <tinyxml.h>


extern xdl::XdevLModuleDescriptor openGLModuleDesc;

extern "C" XDEVL_EXPORT int _create(xdl::XdevLModuleCreateParameter* parameter) {
	// Create the "OpenGL" module.
	if(openGLModuleDesc.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLOpenGLSDL(parameter);

		if(!obj)
			return xdl::ERR_ERROR;

		parameter->setModuleInstance(obj);

		return xdl::ERR_OK;
	}

	return xdl::ERR_MODULE_NOT_FOUND;
}

namespace xdl {


	XdevLOpenGLSDL::XdevLOpenGLSDL(XdevLModuleCreateParameter* parameter) :
		XdevLOpenGLImpl(parameter) {
	}

	XdevLOpenGLSDL::~XdevLOpenGLSDL() {

	}

	xdl_int XdevLOpenGLSDL::init() {
		if(XdevLOpenGLImpl::init() != ERR_OK) {
			return ERR_ERROR;
		}
		return ERR_OK;
	}

	xdl_int XdevLOpenGLSDL::shutdown() {
		SDL_GL_DeleteContext(m_OpenGLContext);
		return ERR_OK;
	}

	int XdevLOpenGLSDL::createContext(XdevLWindow* window) {

		if (SDL_GL_LoadLibrary(NULL)) {
			XDEVL_MODULE_ERROR("Could not load OpenGL library: " << SDL_GetError() << "\n");
		}		
		
		m_SDLWindow = (static_cast<SDL_Window*>(window->getInternal(XdevLInternalName("SDL_Window"))));
		if(m_SDLWindow == NULL) {

			void* p = window->getInternal(XdevLInternalName("X11_WINDOW"));
			m_SDLWindow = SDL_CreateWindowFrom(p);
			if(m_SDLWindow == NULL) {
				XDEVL_MODULE_INFO("Could not get a valid SDL_Window. SDL_Window: " << m_window << std::endl);
				return ERR_ERROR;
			}
		}

		XDEVL_MODULE_INFO("Setting OpenGL context to version: " << m_major << "." << m_minor << "\n")
		if(m_profile == "core") {
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, m_major);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, m_minor);
		} else if(m_profile == "compatibility") {
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
		}


		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, m_ColorDepth);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, m_ZDepth);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, m_StencilDepth);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
		SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1 );

		if(m_fsaa > 0) {
			initMultisample();
		}

		XDEVL_MODULE_INFO("Creating OpenGL context.\n")
		m_OpenGLContext = SDL_GL_CreateContext(m_SDLWindow);
		if(m_OpenGLContext == NULL) {
			XDEVL_MODULE_ERROR("Could not create OpenGL context.\n");
			return ERR_ERROR;
		}
		XDEVL_MODULE_SUCCESS("Creating OpenGL context was successful.\n")

		SDL_GL_MakeCurrent(m_SDLWindow, m_OpenGLContext);

		initGLEW();

		XDEVL_MODULE_SUCCESS("OpenGL successfully initialized.\n")

		return ERR_OK;
	}


	int XdevLOpenGLSDL::makeCurrent() {
		m_previousOpenGLContext = SDL_GL_GetCurrentContext();
		
		SDL_GL_MakeCurrent(m_SDLWindow, m_OpenGLContext);
		
		return ERR_OK;
	}
	
	xdl_int XdevLOpenGLSDL::releaseCurrent() {
		if(NULL != m_previousOpenGLContext) {
			SDL_GL_MakeCurrent(m_SDLWindow, m_previousOpenGLContext);
		}
		return ERR_OK;
	}

	void* XdevLOpenGLSDL::getInternal(const XdevLInternalName& id) {
		return NULL;
	}

	int XdevLOpenGLSDL::swapBuffers() {
		SDL_GL_SwapWindow(m_SDLWindow);
		return ERR_OK;
	}

	xdl_int XdevLOpenGLSDL::setVSync(xdl_bool state) {
		SDL_GL_SetSwapInterval(state);
		return ERR_ERROR;
	}

	xdl_bool XdevLOpenGLSDL::initMultisample() {
		XDEVL_MODULE_INFO("Switch on FSAA. Samples: " << m_fsaa << "\n");
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, m_fsaa);
		return xdl_false;
	}

	xdl_int XdevLOpenGLSDL::setEnableFSAA(xdl_bool state) {

		return ERR_OK;
	}

	xdl_int XdevLOpenGLSDL::makeCurrent(XdevLWindow* window) {
		m_SDLWindow = (static_cast<SDL_Window*>(window->getInternal(XdevLInternalName("SDL_Window"))));
		if(m_SDLWindow == NULL) {
			return ERR_ERROR;
		}
		SDL_GL_MakeCurrent(m_SDLWindow, m_OpenGLContext);
		return ERR_OK;
	}

}
