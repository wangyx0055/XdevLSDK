#include "XdevLOpenGLContextGLX.h"

static const std::vector<xdl::XdevLModuleName> moduleNames	{
	xdl::XdevLModuleName("XdevLOpenGLContext")
};

xdl::XdevLModuleDescriptor moduleDescriptor {
	xdl::XdevLString("www.codeposer.net"),
	xdl::XdevLString("Cengiz Terzibas"),
	moduleNames[0],
	xdl::XdevLString("(c) 2005 - 2016 Cengiz Terzibas."),
	xdl::XdevLString("Module to create a GLX OpenGL context."),
	XDEVLOPENGLCONTEXT_MODULE_MAJOR_VERSION,
	XDEVLOPENGLCONTEXT_MODULE_MINOR_VERSION,
	XDEVLOPENGLCONTEXT_MODULE_PATCH_VERSION
};

xdl::XdevLPluginDescriptor glxPluginDescriptor {
	xdl::XdevLString("XdevLOpenGLContextGLX"),
	moduleNames,
	XDEVLOPENGLCONTEXT_MAJOR_VERSION,
	XDEVLOPENGLCONTEXT_MINOR_VERSION,
	XDEVLOPENGLCONTEXT_PATCH_VERSION
};

XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(glxPluginDescriptor)

XDEVL_PLUGIN_CREATE_MODULE {
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLOpenGLContextGLX, moduleDescriptor)
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}

XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DEFINITION(XdevLOpenGLContext, xdl::XdevLOpenGLContextGLX, moduleDescriptor)
