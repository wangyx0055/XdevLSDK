#ifndef XDEVL_OPENGL_COCOA_H
#define XDEVL_OPENGL_COCOA_H


#include <XdevLPluginImpl.h>
#include <XdevLOpenGLContext/XdevLOpenGLContextBase.h>

#include <SDL.h>

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
		"Module to create a SDL OpenGL context."
	};
	static const XdevLString glx_context_pluginName {
		"XdevLOpenGLContextSDL"
	};

	static const std::vector<XdevLModuleName> glx_context_moduleNames	{
																			XdevLModuleName("XdevLOpenGLContext")
																		};

	// Holds the Major version number.
	const xdl_int XdevLOpenGLContextSDLPluginMajorVersion = XDEVLOPENGLCONTEXT_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_int XdevLOpenGLContextSDLPluginMinorVersion = XDEVLOPENGLCONTEXT_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_int XdevLOpenGLContextSDLPluginPatchVersion = XDEVLOPENGLCONTEXT_PATCH_VERSION;


	// Holds the Major version number.
	const xdl_int XdevLOpenGLContexSDLMajorVersion = XDEVLOPENGLCONTEXT_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_int XdevLOpenGLContextSDLMinorVersion = XDEVLOPENGLCONTEXT_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_int XdevLOpenGLContextSDLPatchVersion = XDEVLOPENGLCONTEXT_MODULE_PATCH_VERSION;


	/**
		@class XdevLOpenGLContextSDL
		@brief
		@author Cengiz Terzibas
	*/

	class XdevLOpenGLContextSDL : public XdevLOpenGLContextBase {

		public:
			XdevLOpenGLContextSDL(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLOpenGLContextSDL();

			static XdevLModuleDescriptor m_moduleDescriptor;

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
