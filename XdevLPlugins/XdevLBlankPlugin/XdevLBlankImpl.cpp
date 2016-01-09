/*
	Copyright (c) 2005 - 2016 Cengiz Terzibas

	Permission is hereby granted, free of charge, to any person obtaining a copy of 
	this software and associated documentation files (the "Software"), to deal in the 
	Software without restriction, including without limitation the rights to use, copy, 
	modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
	and to permit persons to whom the Software is furnished to do so, subject to the 
	following conditions:

	The above copyright notice and this permission notice shall be included in all copies 
	or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
	DEALINGS IN THE SOFTWARE.

	cengiz@terzibas.de
*/

#include <XdevLPlatform.h>
#include <XdevLPlugin.h>
#include <XdevLCoreMediator.h>

#include "XdevLBlankImpl.h"

#include <fstream>


xdl::XdevLPluginDescriptor blankPluginDescriptor {
	xdl::pluginName,
	xdl::moduleNames,
	XDEVLBLANK_MAJOR_VERSION,
	XDEVLBLANK_MINOR_VERSION,
	XDEVLBLANK_PATCH_VERSION
};

xdl::XdevLModuleDescriptor blankModuleDescriptor {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[0],
	xdl::copyright,
	xdl::moduleDescription,
	XDEVLBLANK_MODULE_MAJOR_VERSION,
	XDEVLBLANK_MODULE_MINOR_VERSION,
	XDEVLBLANK_MODULE_PATCH_VERSION
};

XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(blankPluginDescriptor);

XDEVL_PLUGIN_CREATE_MODULE {
	if(blankModuleDescriptor.getName() == XDEVL_MODULE_PARAMETER_NAME) {

		xdl::IPXdevLModule module = XDEVL_NEW_MODULE(xdl::XdevLBlankModuleImpl,  XDEVL_MODULE_PARAMETER);
		XDEVL_MODULE_SET_MODULE_INSTACE(module);

		return xdl::ERR_OK;
	}

	return xdl::ERR_MODULE_NOT_FOUND;
}

namespace xdl {


	XdevLBlankModuleImpl::XdevLBlankModuleImpl(XdevLModuleCreateParameter* parameter) :
		XdevLModuleImpl<XdevLBlankModule>(parameter, blankModuleDescriptor) {
		XDEVL_MODULE_INFO("Hello, World!\n");
	}

	XdevLBlankModuleImpl::~XdevLBlankModuleImpl() {
	}

}
