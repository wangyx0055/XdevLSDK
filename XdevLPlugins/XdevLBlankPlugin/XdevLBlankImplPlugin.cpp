#include "XdevLBlankImpl.h"

static std::vector<xdl::XdevLModuleName>	moduleNames {
	xdl::XdevLModuleName("XdevLBlank"),
};


xdl::XdevLPluginDescriptor blankPluginDescriptor {
	xdl::XdevLString("XdevLBlank"),
	moduleNames,
	XDEVLBLANK_MAJOR_VERSION,
	XDEVLBLANK_MINOR_VERSION,
	XDEVLBLANK_PATCH_VERSION
};

xdl::XdevLModuleDescriptor blankModuleDescriptor {
	xdl::XdevLString("www.codeposer.net"),
	xdl::XdevLString("Cengiz Terzibas"),
	moduleNames[0],
	xdl::XdevLString("(c) 2005 - 2016 Cengiz Terzibas."),
	xdl::XdevLString("This is a description of an empty module"),
	XDEVLBLANK_MODULE_MAJOR_VERSION,
	XDEVLBLANK_MODULE_MINOR_VERSION,
	XDEVLBLANK_MODULE_PATCH_VERSION
};

XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(blankPluginDescriptor)

XDEVL_PLUGIN_CREATE_MODULE {
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLBlankImpl, blankModuleDescriptor)
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}

XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DEFINITION(XdevLBlank, xdl::XdevLBlankImpl, blankModuleDescriptor)
XDEVL_EXPORT_PLUGIN_INIT_FUNCTION_DEFINITION_DEFAULT(XdevLBlank)