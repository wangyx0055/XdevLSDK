#include <XdevLCoreMediator.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLPlatform.h>
#include <XdevLXstring.h>
#include <XdevLUtils.h>
#include "XdevLOpenGLcontextCocoa.h"


xdl::XdevLModuleDescriptor xdl::XdevLOpenGLContextCocoa::moduleDescriptor{cocoa_context_vendor, 
																																						cocoa_context_author, 
																																						cocoa_context_moduleNames[0], 
																																						cocoa_context_copyright, 
																																						cocoa_context_description, 
																																						XdevLOpenGLContexCocoaMajorVersion, 
																																						XdevLOpenGLContextCocoaMinorVersion,
																																						XdevLOpenGLContextCocoaPatchVersion};


xdl::XdevLPluginDescriptor pluginDescriptor{xdl::cocoa_context_pluginName,
																						xdl::cocoa_context_moduleNames,
																						xdl::XdevLOpenGLContextCocoaPluginMajorVersion,
																						xdl::XdevLOpenGLContextCocoaPluginMinorVersion,
																						xdl::XdevLOpenGLContextCocoaPluginPatchVersion};

extern "C" XDEVL_EXPORT int _create(xdl::XdevLModuleCreateParameter* parameter) {

	if (xdl::XdevLOpenGLContextCocoa::moduleDescriptor.getName() == parameter->getModuleName()) {
		xdl::XdevLOpenGLContextCocoa* obj  = new xdl::XdevLOpenGLContextCocoa(parameter);

		if (!obj)
			return xdl::ERR_ERROR;

		parameter->setModuleInstance(obj);

		return xdl::ERR_OK;
	}

	return xdl::ERR_MODULE_NOT_FOUND;
}

extern "C" XDEVL_EXPORT void _delete(xdl::XdevLModule* obj) {
	if(obj)
		delete obj;
}

extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor() {
	return &pluginDescriptor;
}

namespace xdl {


	XdevLOpenGLContextCocoa::XdevLOpenGLContextCocoa(XdevLModuleCreateParameter* parameter) :
		XdevLModuleAutoImpl(parameter, moduleDescriptor),
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

	xdl_int XdevLOpenGLContextCocoa::getAttributes(XdevLOpenGLContextAttributes& attributes) {
		attributes = m_attributes;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLContextCocoa::setAttributes(const XdevLOpenGLContextAttributes& attributes) {
		m_attributes = attributes;
		return ERR_OK;
	}

	xdl_int XdevLOpenGLContextCocoa::create(XdevLWindow* window) {
		NSAutoreleasePool *pool;
		pool = [[NSAutoreleasePool alloc] init];

		std::vector<NSOpenGLPixelFormatAttribute> attributes;

		//
		// Create OpenGL profile
		//
		attributes.push_back(NSOpenGLPFAOpenGLProfile);
		NSOpenGLPixelFormatAttribute profile = NSOpenGLProfileVersionLegacy;
		if(m_attributes.context_profile_mask == XDEVL_OPENGL_CONTEXT_CORE_PROFILE) {
			profile = NSOpenGLProfileVersion3_2Core;
		}

		if(m_attributes.double_buffer > 0) {
			attributes.push_back(NSOpenGLPFADoubleBuffer);
		}

		// Color size.
		attributes.push_back(NSOpenGLPFAColorSize);
		attributes.push_back(m_attributes.color_buffer_size);

		// Alpha size.
		attributes.push_back(NSOpenGLPFAAlphaSize);
		attributes.push_back(m_attributes.alpha_size);

		// Depth size.
		attributes.push_back(NSOpenGLPFADepthSize);
		attributes.push_back(m_attributes.depth_size);

		attributes.push_back(NSOpenGLPFAStencilSize);
		attributes.push_back(m_attributes.stencil_size);

		// Accumulation buffer size.
		attributes.push_back(NSOpenGLPFAAccumSize);
		attributes.push_back(m_attributes.accum_red_size+
		                     m_attributes.accum_green_size+
		                     m_attributes.accum_blue_size+
		                     m_attributes.accum_alpha_size);

		if(m_attributes.stereo > 0) {
			attributes.push_back(NSOpenGLPFAStereo);
		}


		if(m_attributes.multisample_buffers > 0) {
			attributes.push_back(NSOpenGLPFASampleBuffers);
			attributes.push_back(m_attributes.multisample_buffers);
		}

		if(m_attributes.multisample_samples > 0) {
			attributes.push_back(NSOpenGLPFASampleBuffers);
			attributes.push_back(m_attributes.multisample_samples);
			attributes.push_back(NSOpenGLPFANoRecovery);
		}

		attributes.push_back(NSOpenGLPFAAccelerated);

		NSOpenGLPixelFormatAttribute attr[32];
		NSOpenGLPixelFormat *fmt;
		
		fmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:attr];
    if (fmt == nil) {
        XDEVL_MODULE_ERROR("Could not initialize Pixel Format.\n");
        [pool release];
        return ERR_ERROR;;
    }

		
		m_openGLContext = [[NXdevLOpenGLContext alloc] initWithFormat:fmt shareContext:nil];

		
		[pool release];
		return ERR_OK;
	}


	int XdevLOpenGLContextCocoa::makeCurrent(XdevLWindow* window) {
		NSAutoreleasePool *pool;

		pool = [[NSAutoreleasePool alloc] init];

[m_openGLContext setWindow:window];
		[m_openGLContext updateIfNeeded];
		[m_openGLContext makeCurrentContext];

		[pool release];
		return ERR_ERROR;
	}

	int XdevLOpenGLContextCocoa::swapBuffers() {
		NSAutoreleasePool *pool;
		pool = [[NSAutoreleasePool alloc] init];

		[m_openGLContext flushBuffer];
		[m_openGLContext updateIfNeeded];

		[pool release];

		return ERR_OK;
	}

	void* XdevLOpenGLContextCocoa::getInternal(const char* id) {
		return NULL;
	}

}

@implementation NXdevLOpenGLContext : NSOpenGLContext

- (id)initWithFormat:(NSOpenGLPixelFormat *)format
        shareContext:(NSOpenGLContext *)share
{
    self = [super initWithFormat:format shareContext:share];
    if (self) {
        self->window = NULL;
    }
    return self;
}

- (void)scheduleUpdate
{

}

/* This should only be called on the thread on which a user is using the context. */
- (void)updateIfNeeded
{
        /* We call the real underlying update here, since -[SDLOpenGLContext update] just calls us. */
        [super update];
	
}

/* This should only be called on the thread on which a user is using the context. */
- (void)update
{
    /* This ensures that regular 'update' calls clear the atomic dirty flag. */
    [self scheduleUpdate];
    [self updateIfNeeded];
}

/* Updates the drawable for the contexts and manages related state. */
- (void)setWindow:(xdl::XdevLWindow *)newWindow
{
    self->window = newWindow;
}

@end
