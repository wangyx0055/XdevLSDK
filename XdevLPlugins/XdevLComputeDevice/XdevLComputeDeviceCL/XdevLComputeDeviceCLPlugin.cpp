#include "XdevLComputeDeviceCL.h"

static std::vector<xdl::XdevLModuleName>	moduleNames {
	xdl::XdevLModuleName("XdevLComputeDevice"),
};


xdl::XdevLPluginDescriptor computeDevicePluginDescriptor {
	xdl::XdevLString("XdevLComputeDeviceCL"),
	moduleNames,
	XDEVLCOMPUTE_DEVICE_CL_MAJOR_VERSION,
	XDEVLCOMPUTE_DEVICE_CL_MINOR_VERSION,
	XDEVLCOMPUTE_DEVICE_CL_PATCH_VERSION
};

xdl::XdevLModuleDescriptor computeDeviceCLModuleDescriptor {
	xdl::XdevLString("www.codeposer.net"),
	xdl::XdevLString("Cengiz Terzibas"),
	moduleNames[0],
	xdl::XdevLString("(c) 2016 Cengiz Terzibas."),
	xdl::XdevLString("This module supports OpenCL handling."),
	XDEVLCOMPUTE_DEVICE_CL_MODULE_MAJOR_VERSION,
	XDEVLCOMPUTE_DEVICE_CL_MODULE_MINOR_VERSION,
	XDEVLCOMPUTE_DEVICE_CL_MODULE_PATCH_VERSION
};

XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(computeDevicePluginDescriptor)

XDEVL_PLUGIN_CREATE_MODULE {
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLComputeDeviceCL, computeDeviceCLModuleDescriptor)
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}

XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DEFINITION(XdevLComputeDevice, xdl::XdevLComputeDeviceCL, computeDeviceCLModuleDescriptor)
XDEVL_EXPORT_PLUGIN_INIT_FUNCTION_DEFINITION_DEFAULT(XdevLComputeDevice)