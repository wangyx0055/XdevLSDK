#include <XdevLCoreMediator.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLPlatform.h>
#include <XdevLXstring.h>
#include <XdevLUtils.h>
#include "XdevLOpenGLcontextCocoa.h"


xdl::XdevLModuleDescriptor moduleDescriptor {
	xdl::XdevLString("www.codeposer.net"),
	xdl::XdevLString("Cengiz Terzibas"),
	xdl::moduleNames[0],
	xdl::XdevLString("(c) 2005 - 2016 Cengiz Terzibas."),
	xdl::XdevLString("Module to create a GLX OpenGL context."),
	XDEVLOPENGL_CONTEXT_CGL_MODULE_MAJOR_VERSION,
	XDEVLOPENGL_CONTEXT_CGL_MODULE_MINOR_VERSION,
	XDEVLOPENGL_CONTEXT_CGL_MODULE_PATCH_VERSION
};

xdl::XdevLPluginDescriptor cglPluginDescriptor {
	xdl::XdevLString("XdevLOpenGLContextGLX"),
	xdl::moduleNames,
	XDEVLOPENGL_CONTEXT_CGL_MAJOR_VERSION,
	XDEVLOPENGL_CONTEXT_CGL_MINOR_VERSION,
	XDEVLOPENGL_CONTEXT_CGL_PATCH_VERSION
};

XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(cglPluginDescriptor);

XDEVL_PLUGIN_CREATE_MODULE {
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLOpenGLContextCocoa, moduleDescriptor)
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}

XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DEFINITION(XdevLOpenGLContext, xdl::XdevLOpenGLContextCocoa, moduleDescriptor)
XDEVL_EXPORT_PLUGIN_INIT_FUNCTION_DEFINITION_DEFAULT(XdevLOpenGLContext)
XDEVL_EXPORT_PLUGIN_SHUTDOWN_FUNCTION_DEFINITION_DEFAULT(XdevLOpenGLContext)

namespace xdl {


	XdevLOpenGLContextCocoa::XdevLOpenGLContextCocoa(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		XdevLModuleAutoImpl(parameter, descriptor),
		m_openGLContext(nil) {
	}

	XdevLOpenGLContextCocoa::~XdevLOpenGLContextCocoa() {

	}


	xdl_int XdevLOpenGLContextCocoa::init() {

		return ERR_OK;
	}

	xdl_int XdevLOpenGLContextCocoa::shutdown() {
		return ERR_OK;
	}

	void* XdevLOpenGLContextCocoa::getProcAddress(const xdl_char* functionName) {

		NSSymbol symbol;
		char *symbolName;
		symbolName = reinterpret_cast<char*>(malloc(strlen(functionName) + 2));
		strcpy(symbolName + 1, functionName);
		symbolName[0] = '_';
		symbol = nullptr;

		if(NSIsSymbolNameDefined(symbolName)) {
			symbol = NSLookupAndBindSymbol(symbolName);
		}
		free(symbolName);

		return symbol ? NSAddressOfSymbol(symbol) : nullptr;
	}

	xdl_int XdevLOpenGLContextCocoa::getAttributes(XdevLOpenGLContextAttributes& attributes) {
		attributes = m_attributes;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLContextCocoa::setAttributes(const XdevLOpenGLContextAttributes& attributes) {
		m_attributes = attributes;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLContextCocoa::create(XdevLWindow* window) {

		NSView* wnd = (NSView*)(window->getInternal(XdevLInternalName("COCOA_VIEW")));
		if(nil == wnd) {
			return ERR_ERROR;
		}


//		NSAutoreleasePool *pool;
//		pool = [[NSAutoreleasePool alloc] init];
//
//		std::vector<NSOpenGLPixelFormatAttribute> attributes;
//
//		//
//		// Create OpenGL profile
//		//
//		attributes.push_back(NSOpenGLPFAOpenGLProfile);
//		NSOpenGLPixelFormatAttribute profile = NSOpenGLProfileVersionLegacy;
//		if(m_attributes.context_profile_mask == XDEVL_OPENGL_CONTEXT_CORE_PROFILE) {
//			profile = NSOpenGLProfileVersion3_2Core;
//		}
//		attributes.push_back(profile);
//
//		if(m_attributes.double_buffer > 0) {
//			attributes.push_back(NSOpenGLPFADoubleBuffer);
//		}
//
//		// Color size.
//		attributes.push_back(NSOpenGLPFAColorSize);
//		attributes.push_back(m_attributes.color_buffer_size);
//
//		// Alpha size.
//		attributes.push_back(NSOpenGLPFAAlphaSize);
//		attributes.push_back(m_attributes.alpha_size);
//
//		// Depth size.
//		attributes.push_back(NSOpenGLPFADepthSize);
//		attributes.push_back(m_attributes.depth_size);
//
//		attributes.push_back(NSOpenGLPFAStencilSize);
//		attributes.push_back(m_attributes.stencil_size);
//
//		// Accumulation buffer size.
//		attributes.push_back(NSOpenGLPFAAccumSize);
//		attributes.push_back(m_attributes.accum_red_size+
//		                     m_attributes.accum_green_size+
//		                     m_attributes.accum_blue_size+
//		                     m_attributes.accum_alpha_size);
//
//		if(m_attributes.stereo > 0) {
//			attributes.push_back(NSOpenGLPFAStereo);
//		}
//
//
//		if(m_attributes.multisample_buffers > 0) {
//			attributes.push_back(NSOpenGLPFASampleBuffers);
//			attributes.push_back(m_attributes.multisample_buffers);
//		}
//
//		if(m_attributes.multisample_samples > 0) {
//			attributes.push_back(NSOpenGLPFASampleBuffers);
//			attributes.push_back(m_attributes.multisample_samples);
//			attributes.push_back(NSOpenGLPFANoRecovery);
//		}
//
//		attributes.push_back(NSOpenGLPFAAccelerated);
//		attributes.push_back(0);
//
//		NSOpenGLPixelFormat* pixelFormat = nullptr;
//
//		pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes.data()];
//		if(pixelFormat == nil) {
//			XDEVL_MODULE_ERROR("Could not initialize Pixel Format.\n");
//			[pool release];
//			return ERR_ERROR;;
//		}
//
//
//		m_openGLContext = [[NXdevLOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];
//
//
//		[pool release];


		CGLPixelFormatObj pix;
		GLint             npix;
		std::vector<CGLPixelFormatAttribute> attribs {
			kCGLPFAOpenGLProfile, (CGLPixelFormatAttribute)kCGLOGLPVersion_3_2_Core, // This sets the context to 3.2
			kCGLPFAColorSize, (CGLPixelFormatAttribute)m_attributes.color_buffer_size,
			kCGLPFADepthSize, (CGLPixelFormatAttribute)m_attributes.depth_size,
			kCGLPFAStencilSize, (CGLPixelFormatAttribute)m_attributes.stencil_size,
			kCGLPFAAlphaSize, (CGLPixelFormatAttribute)m_attributes.alpha_size,
			kCGLPFAAccelerated,
			kCGLPFASampleBuffers, (CGLPixelFormatAttribute)1,
			kCGLPFASamples, (CGLPixelFormatAttribute)4,
		};

		if(m_attributes.double_buffer > 0) {
			attribs.push_back(kCGLPFADoubleBuffer);
		}

		if(m_attributes.stereo > 0) {
			attribs.push_back(kCGLPFAStereo);
		}


		attribs.push_back((CGLPixelFormatAttribute) 0);

		CGLChoosePixelFormat(attribs.data(), &pix, &npix);
		CGLCreateContext(pix, NULL, &m_context);
		CGLSetCurrentContext(m_context);
m_openGLContext = [[NXdevLOpenGLContext alloc] initWithCGLContextObj:m_context];
		if(m_openGLContext == nil) {
			XDEVL_MODULE_ERROR("initWithCGLContextObj failed\n");
		}


		//
		// Make OpenGL context current.
		//
[m_openGLContext setView:wnd];
		[m_openGLContext makeCurrentContext];


		return ERR_OK;
	}


	int XdevLOpenGLContextCocoa::makeCurrent(XdevLWindow* window) {
		NSView* wnd = (NSView*)(window->getInternal(XdevLInternalName("COCOA_VIEW")));
		if(nil == wnd) {
			return ERR_ERROR;
		}

		NSAutoreleasePool *pool;

		pool = [[NSAutoreleasePool alloc] init];

[m_openGLContext setView:wnd];
		[m_openGLContext makeCurrentContext];

		[pool release];

		return ERR_ERROR;
	}

	int XdevLOpenGLContextCocoa::swapBuffers() {
		NSAutoreleasePool *pool;
		pool = [[NSAutoreleasePool alloc] init];

		[m_openGLContext flushBuffer];

		[pool release];

		return ERR_OK;
	}

	xdl_int XdevLOpenGLContextCocoa::setVSync(xdl_bool enableVSync) {
		return ERR_ERROR;
	}

	void* XdevLOpenGLContextCocoa::getInternal(const char* id) {
		return NULL;
	}

}

@implementation NXdevLOpenGLContext :
NSOpenGLContext

- (id)initWithFormat:
(NSOpenGLPixelFormat *)format
shareContext:
(NSOpenGLContext *)share {
self = [super initWithFormat:format shareContext:share];
	if(self) {
		self->window = NULL;
	}
	return self;
}

- (void)scheduleUpdate {

}

- (void)updateIfNeeded {
	[super update];

}

/* This should only be called on the thread on which a user is using the context. */
- (void)update {
	/* This ensures that regular 'update' calls clear the atomic dirty flag. */
	[self scheduleUpdate];
	[self updateIfNeeded];
}

/* Updates the drawable for the contexts and manages related state. */
- (void)setWindow:
(xdl::XdevLWindow *)newWindow {
	self->window = newWindow;
}

@end
