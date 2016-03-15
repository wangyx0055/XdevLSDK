#include "XdevLWindowEventServerCocoa.h"

namespace xdl {

	XdevLWindowEventServerCocoa::XdevLWindowEventServerCocoa(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor) :
		XdevLWindowEventServerImpl(parameter, desriptor)
	{}


	xdl_int XdevLWindowEventServerCocoa::registerWindowForEvents(XdevLWindow* window) {
		return XdevLWindowEventServerImpl::registerWindowForEvents(window);
	}

	xdl_int XdevLWindowEventServerCocoa::unregisterWindowFromEvents(XdevLWindow* window) {
		return XdevLWindowEventServerImpl::unregisterWindowFromEvents(window);
	}

	xdl_int XdevLWindowEventServerCocoa::init() {
		XDEVL_MODULE_SUCCESS("Window created successfully" << std::endl);
		return ERR_OK;
	}

	void* XdevLWindowEventServerCocoa::getInternal(const XdevLInternalName& id) {

		return nullptr;
	}

	xdl_int XdevLWindowEventServerCocoa::shutdown() {
		XDEVL_MODULE_SUCCESS("Shutdown process was successful.\n");
		return ERR_OK;
	}

	xdl_int XdevLWindowEventServerCocoa::update() {
		return ERR_ERROR;
	}


	void XdevLWindowEventServerCocoa::flush() {

	}

}
