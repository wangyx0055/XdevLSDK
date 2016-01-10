#ifndef XDEVL_OPENGL_CONTEXT_CGL_H
#define XDEVL_OPENGL_CONTEXT_CGL_H

#include <XdevLPluginImpl.h>
#include <XdevLOpenGLContext/XdevLOpenGLContext.h>

#import <Cocoa/Cocoa.h>
#include "mach-o/dyld.h"
#include <OpenGL/CGLTypes.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/CGLRenderers.h>
#include <OpenGL/gl3.h>


@interface NXdevLOpenGLContext : NSOpenGLContext {
	xdl::XdevLWindow* window;
}

- (id)initWithFormat: (NSOpenGLPixelFormat *)format shareContext:(NSOpenGLContext *)share;
- (void)scheduleUpdate;
- (void)updateIfNeeded;
- (void)setWindow:
(xdl::XdevLWindow *)window;

@end

namespace xdl {


	static const std::vector<XdevLModuleName> moduleNames {
		XdevLModuleName("XdevLOpenGLContext")
	};

	/**
		@class XdevLOpenGLContextCocoa
		@brief
		@author Cengiz Terzibas
	*/
	class XdevLOpenGLContextCocoa : public XdevLModuleAutoImpl<XdevLOpenGLContext> {

		public:
			XdevLOpenGLContextCocoa(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor);
			virtual ~XdevLOpenGLContextCocoa();

			static XdevLModuleDescriptor moduleDescriptor;

			virtual xdl_int init();

			virtual xdl_int shutdown();

			virtual void* getInternal(const char* id);

			virtual void* getProcAddress(const xdl_char* func);
			virtual xdl_int getAttributes(XdevLOpenGLContextAttributes& attributes);
			virtual xdl_int setAttributes(const XdevLOpenGLContextAttributes& attributes);
			virtual xdl_int create(XdevLWindow* window);
			virtual xdl_int makeCurrent(XdevLWindow* window);
			virtual xdl_int swapBuffers();
			virtual xdl_int setVSync(xdl_bool enableVSync) override;

		private:
			CGLContextObj m_context;
			NSOpenGLContext* m_openGLContext;
			XdevLOpenGLContextAttributes m_attributes;
	};

}


#endif
