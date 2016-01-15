#include "XdevLWindowX11.h"

extern xdl::XdevLPluginDescriptor windowX11PluginDescriptor;
extern xdl::XdevLModuleDescriptor windowX11ModuleDesc;
extern xdl::XdevLModuleDescriptor windowServerModuleDesc;
extern xdl::XdevLModuleDescriptor windowEventServerModuleDesc;
extern xdl::XdevLModuleDescriptor cursorModuleDesc;

XDEVL_PLUGIN_INIT {
	return xdl::XdevLWindowX11::initX11(XDEVL_PLUGIN_CREATE_PARAMETER_MEDIATOR);
}

XDEVL_PLUGIN_SHUTDOWN {
	return xdl::XdevLWindowX11::shutdownX11();
}

XDEVL_PLUGIN_CREATE_MODULE  {
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLWindowX11, windowX11ModuleDesc)
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLWindowServerX11, windowServerModuleDesc)
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLWindowX11EventServer, windowEventServerModuleDesc)
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLCursorX11, cursorModuleDesc)
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}

XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(windowX11PluginDescriptor);

XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DEFINITION(XdevLWindow, xdl::XdevLWindowX11, windowX11ModuleDesc)
XDEVL_EXPORT_PLUGIN_INIT_FUNCTION_DEFINITION(XdevLWindow) {
	return xdl::XdevLWindowX11::initX11(nullptr);
}