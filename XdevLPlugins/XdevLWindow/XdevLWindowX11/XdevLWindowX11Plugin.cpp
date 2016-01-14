#include "XdevLWindowX11.h"

//
// The XdevLWindow plugin descriptor.
//
xdl::XdevLPluginDescriptor windowX11PluginDescriptor {
	xdl::windowX11PluginName,
	xdl::window_moduleNames,
	xdl::XdevLWindowPluginMajorVersion,
	xdl::XdevLWindowPluginMinorVersion,
	xdl::XdevLWindowPluginPatchVersion
};

//
// The XdevLWindow module descriptor.
//
xdl::XdevLModuleDescriptor windowX11ModuleDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_WINDOW_MODULE_NAME],
	xdl::window_copyright,
	xdl::window_x11_description,
	xdl::XdevLWindowMajorVersion,
	xdl::XdevLWindowMinorVersion,
	xdl::XdevLWindowPatchVersion
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
	xdl::XdevLWindowEventServerMajorVersion,
	xdl::XdevLWindowEventServerMinorVersion,
	xdl::XdevLWindowEventServerPatchVersion
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
	xdl::XdevLWindowEventServerMajorVersion,
	xdl::XdevLWindowEventServerMinorVersion,
	xdl::XdevLWindowEventServerPatchVersion
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
	xdl::XdevLWindowEventServerMajorVersion,
	xdl::XdevLWindowEventServerMinorVersion,
	xdl::XdevLWindowEventServerPatchVersion
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
