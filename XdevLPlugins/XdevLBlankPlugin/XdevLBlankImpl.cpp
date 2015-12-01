

#include <XdevLPlatform.h>
#include <XdevLPlugin.h>
#include <XdevLCoreMediator.h>

#include "XdevLBlankImpl.h"

#include <fstream>


xdl::XdevLPluginDescriptor blankPluginDescriptor {
	xdl::pluginName,
	xdl::moduleNames,
	xdl::XdevLBlankPluginMajorVersion,
	xdl::XdevLBlankPluginMinorVersion,
	xdl::XdevLBlankPluginPatchVersion
};

xdl::XdevLModuleDescriptor blankModuleDescriptor {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[0],
	xdl::copyright,
	xdl::moduleDescription,
	xdl::XdevLBlankMajorVersion,
	xdl::XdevLBlankMinorVersion,
	xdl::XdevLBlankPatchVersion
};


extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	if(blankModuleDescriptor.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLBlankModuleImpl(parameter);
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
	return &blankPluginDescriptor;
}


namespace xdl {


	XdevLBlankModuleImpl::XdevLBlankModuleImpl(XdevLModuleCreateParameter* parameter) :
		XdevLModuleImpl<XdevLBlankModule>(parameter, blankModuleDescriptor) {
		XDEVL_MODULE_INFO("Hello, World!\n");
	}

	XdevLBlankModuleImpl::~XdevLBlankModuleImpl() {
	}

}
