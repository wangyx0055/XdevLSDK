#include "XdevLWindowX11.h"
#include "XdevLWindowEventServerX11.h"
#include "XdevLDisplayX11.h"
#include "XdevLCursorX11.h"

extern xdl::XdevLPluginDescriptor windowX11PluginDescriptor;
extern xdl::XdevLModuleDescriptor windowX11DisplayDesc;
extern xdl::XdevLModuleDescriptor windowX11Desc;
extern xdl::XdevLModuleDescriptor windowServerX11Desc;
extern xdl::XdevLModuleDescriptor windowEventServerX11Desc;
extern xdl::XdevLModuleDescriptor cursorX11Desc;

//
// The XdevLWindow plugin descriptor.
//

static const xdl::XdevLString windowX11PluginName {
	"XdevLWindowX11"
};

xdl::XdevLPluginDescriptor windowX11PluginDescriptor {
	windowX11PluginName,
	xdl::window_moduleNames,
	XDEVLX11_PLUGIN_MAJOR_VERSION,
	XDEVLX11_PLUGIN_MINOR_VERSION,
	XDEVLX11_PLUGIN_PATCH_VERSION
};

XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_CREATE_MODULE  {
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLDisplayX11, windowX11DisplayDesc)
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLWindowX11, windowX11Desc)
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLWindowServerX11, windowServerX11Desc)
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLWindowEventServerX11, windowEventServerX11Desc)
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLCursorX11, cursorX11Desc)
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}

XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(windowX11PluginDescriptor)

XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DEFINITION(XdevLDisplay, xdl::XdevLDisplayX11, windowX11DisplayDesc)
XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DEFINITION(XdevLWindow, xdl::XdevLWindowX11, windowX11Desc)
XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DEFINITION(XdevLWindowEventServer, xdl::XdevLWindowEventServerX11, windowEventServerX11Desc)
XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DEFINITION(XdevLCursor, xdl::XdevLCursorX11, cursorX11Desc)

