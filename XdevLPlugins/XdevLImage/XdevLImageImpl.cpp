#include "XdevLImageImpl.h"

xdl::XdevLPluginDescriptor pluginDescriptor {
	xdl::pluginName,
	xdl::moduleNames,
	XDEVLIMAGE_PLUGIN_MAJOR_VERSION,
	XDEVLIMAGE_PLUGIN_MINOR_VERSION,
	XDEVLIMAGE_PLUGIN_PATCH_VERSION
};

xdl::XdevLModuleDescriptor moduleDescriptor {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[0],
	xdl::copyright,
	xdl::moduleDescription,
	XDEVLBMP_MODULE_MAJOR_VERSION,
	XDEVLBMP_MODULE_MINOR_VERSION,
	XDEVLBMP_MODULE_PATCH_VERSION
};

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	if(moduleDescriptor.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLImageImpl(parameter);
		if(!obj)
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

	XdevLImageImpl::XdevLImageImpl(XdevLModuleCreateParameter* parameter) :
		XdevLModuleImpl<XdevLImageServer> (parameter, moduleDescriptor) {
	}

	XdevLImageImpl::~XdevLImageImpl() {
	}

	xdl_int XdevLImageImpl::readInfo(const char* pFilename, XdevLImageObject* pInfo) {
		return ERR_OK;
	}

	xdl_int XdevLImageImpl::load(const char* pFilename, XdevLImageObject* pImageObject) {
		return ERR_OK;
	}

}
