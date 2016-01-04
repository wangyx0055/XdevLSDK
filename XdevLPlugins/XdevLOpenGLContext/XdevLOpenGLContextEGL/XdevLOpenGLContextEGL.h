#ifndef XDEVL_OPENGLCONTEXTEGL_IMPL_H
#define XDEVL_OPENGLCONTEXTEGL_IMPL_H

#include <XdevLPluginImpl.h>
#include <XdevLOpenGLContext/XdevLOpenGLContext.h>
#include <XdevLOpenGLContext/XdevLOpenGLContextBase.h>

#include <XdevLPluginImpl.h>

#if XDEVL_PLATFORM_UNIX
#include <GLES2/gl2.h>
#endif

#include <EGL/egl.h>
#include <EGL/eglext.h>

namespace xdl {

	static const XdevLString vendor {
		"www.codeposer.net"
	};

	static const XdevLString author {
		"Cengiz Terzibas"
	};

	static const XdevLString copyright {
		"(c) 2005 - 2016 Cengiz Terzibas."
	};

	static const XdevLString pluginName {
		"XdevLOpenGLContextEGL"
	};

	static const XdevLString moduleDescription {
		"Module to create a EGL OpenGL context."
	};

	static std::vector<XdevLModuleName>	moduleNames {
		XdevLModuleName("XdevLOpenGLContext"),
	};

	class XdevLOpenGLContextEGL : public XdevLOpenGLContextBase {
		public:

			XdevLOpenGLContextEGL(XdevLModuleCreateParameter* parameter);

			virtual ~XdevLOpenGLContextEGL();
			virtual xdl_int create(XdevLWindow* window) override;
			virtual xdl_int getAttributes(XdevLOpenGLContextAttributes& attributes) override;
			virtual xdl_int setAttributes(const XdevLOpenGLContextAttributes& attributes) override;
			virtual xdl_int makeCurrent(XdevLWindow* window) override;
			virtual xdl_int swapBuffers() override;
			virtual void* getProcAddress(const xdl_char* func) override;
			virtual xdl_int setVSync(xdl_bool enableVSync) override;
			virtual void* getInternal(const XdevLInternalName& id) override;
		private:
			EGLDisplay m_eglDisplay;
			EGLSurface m_eglSurface;
			EGLContext m_eglContext;
			EGLConfig m_eglConfig;
			EGLint m_major;
			EGLint m_minor;
	};


}

#endif
