#include "XdevLWindowX11.h"

static const xdl::XdevLString windowX11PluginName {
	"XdevLWindowX11"
};

//
// The XdevLWindow plugin descriptor.
//
xdl::XdevLPluginDescriptor windowX11PluginDescriptor {
	windowX11PluginName,
	xdl::window_moduleNames,
	XDEVLX11_MAJOR_VERSION,
	XDEVLX11_MINOR_VERSION,
	XDEVLX11_PATCH_VERSION
};

//
// The XdevLWindow module descriptor.
//
xdl::XdevLModuleDescriptor windowX11ModuleDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_WINDOW_MODULE_NAME],
	xdl::window_copyright,
	xdl::XdevLString("Support for X11 window creation."),
	XDEVLX11_MODULE_MAJOR_VERSION,
	XDEVLX11_MODULE_MINOR_VERSION,
	XDEVLX11_MODULE_PATCH_VERSION
};

//
// The XdevLWindowEventServer module descriptor.
//
xdl::XdevLModuleDescriptor windowServerModuleDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_WINDOW_SERVER_MODULE_NAME],
	xdl::window_copyright,
	xdl::windowServerDescription,
	XDEVLX11_SERVER_MODULE_MAJOR_VERSION,
	XDEVLX11_SERVER_MODULE_MINOR_VERSION,
	XDEVLX11_SERVER_MODULE_PATCH_VERSION
};

//
// The XdevLWindowEventServer module descriptor.
//
xdl::XdevLModuleDescriptor windowEventServerModuleDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_WINDOW_EVENT_SERVER_MODULE_NAME],
	xdl::window_copyright,
	xdl::windowServerDescription,
	XDEVLX11_EVENT_SERVER_MODULE_MAJOR_VERSION,
	XDEVLX11_EVENT_SERVER_MODULE_MINOR_VERSION,
	XDEVLX11_EVENT_SERVER_MODULE_PATCH_VERSION
};

//
// The XdevLCursor module descriptor.
//
xdl::XdevLModuleDescriptor cursorModuleDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_CURSOR_MODULE_NAME],
	xdl::window_copyright,
	xdl::windowServerDescription,
	XDEVLX11CURSOR_MODULE_MAJOR_VERSION,
	XDEVLX11CURSOR_MODULE_MINOR_VERSION,
	XDEVLX11CURSOR_MODULE_PATCH_VERSION
};

XDEVL_PLUGIN_INIT {
	return xdl::XdevLWindowX11::initX11(XDEVL_PLUGIN_CREATE_PARAMETER);
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
