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

#include "XdevLComputeDeviceCL.h"

#include <fstream>

namespace xdl {


	XdevLComputeProgramCL::XdevLComputeProgramCL(cl_device_id deviceid, cl_context context) :
		m_deviceId(deviceid),
		m_context(context),
		m_program(nullptr) {

	}

	XdevLComputeProgramCL::~XdevLComputeProgramCL() {
		if(nullptr != m_kernel) {
			clReleaseKernel(m_kernel);
		}
		if(nullptr != m_program) {
			clReleaseProgram(m_program);
		}
	}

	xdl_int XdevLComputeProgramCL::compileFromFile(const XdevLFileName& filename, const XdevLString& kernelName) {
		std::fstream file(filename.toString().c_str());
		if(!file.is_open()) {
			return ERR_ERROR;
		}

		std::stringstream kernel;
		kernel << file.rdbuf();
		file.close();

		std::vector<std::string> source;
		source.push_back(kernel.str());

		std::vector<std::size_t> lengths;
		std::vector<const xdl::xdl_char*> pointers;

		std::vector<std::string>::iterator ib(source.begin());
		while(ib != source.end()) {
			lengths.push_back((*ib).size());
			pointers.push_back((*ib).data());
			ib++;
		}

		cl_int ret;
		m_program = clCreateProgramWithSource(m_context, 1, (const char **)pointers.data(), (const size_t *)lengths.data(), &ret);
		if(CL_SUCCESS != ret) {
			return ERR_ERROR;
		}

		ret = clBuildProgram(m_program, 1, &m_deviceId, nullptr, nullptr, nullptr);
		if(CL_SUCCESS != ret) {
			return ERR_ERROR;
		}

		m_kernel = clCreateKernel(m_program, kernelName.toString().c_str(), &ret);
		if(CL_SUCCESS != ret) {
			return ERR_ERROR;
		}

		return ERR_OK;
	}

	xdl_int XdevLComputeProgramCL::execute(XdevLComputeDeviceQueue* queue) {
		XdevLComputeDeviceQueueCL* tmp = static_cast<XdevLComputeDeviceQueueCL*>(queue);

		cl_int ret = clEnqueueTask(tmp->getCommandQueue(), m_kernel, 0, nullptr, nullptr);
		if(CL_SUCCESS != ret) {
			return ERR_ERROR;
		}
		
		return ERR_OK;
	}

//
//
//


	XdevLComputeDeviceContextCL::XdevLComputeDeviceContextCL(cl_device_id id, cl_context ctx) :
		m_deviceID(id),
		m_context(ctx) {

	}

	XdevLComputeDeviceContextCL::~XdevLComputeDeviceContextCL() {
		clReleaseContext(m_context);
	}

	XdevLComputeDeviceQueue* XdevLComputeDeviceContextCL::createCommandBuffer() {
		cl_int ret;
		cl_command_queue commandQueue = clCreateCommandQueue(m_context, m_deviceID, 0, &ret);
		auto tmp = new XdevLComputeDeviceQueueCL(commandQueue);
		return tmp;
	}

	XdevLComputeProgram* XdevLComputeDeviceContextCL::createProgram() {
		XdevLComputeProgramCL* tmp = new XdevLComputeProgramCL(m_deviceID, m_context);
		return tmp;
	}


	cl_device_id XdevLComputeDeviceContextCL::getDeviceId() {
		return m_deviceID;
	}

	cl_context XdevLComputeDeviceContextCL::getContext() {
		return m_context;
	}

//
//
//

	XdevLComputeDeviceQueueCL::XdevLComputeDeviceQueueCL(cl_command_queue queue) :
		m_commandQueue(queue) {
	}

	XdevLComputeDeviceQueueCL::~XdevLComputeDeviceQueueCL() {

	}

	cl_command_queue XdevLComputeDeviceQueueCL::getCommandQueue() {
		return m_commandQueue;
	}

//
//
//


	       XdevLComputeDeviceCL::XdevLComputeDeviceCL(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		       XdevLModuleImpl<XdevLComputeDevice>(parameter, descriptor),
		       m_platformID(nullptr),
		       m_deviceID(nullptr) {
		XDEVL_MODULE_INFO("XdevLComputeDeviceCL()\n");
	}

	XdevLComputeDeviceCL::~XdevLComputeDeviceCL() {
		XDEVL_MODULE_INFO("~XdevLComputeDeviceCL()\n");
	}

	int XdevLComputeDeviceCL::init() {
		cl_int ret;

		cl_uint numPlatforms;
		ret = clGetPlatformIDs(1, &m_platformID, &numPlatforms);
		std::vector<cl_platform_id> platformIDs(numPlatforms);

		ret = clGetPlatformIDs(1, platformIDs.data(), nullptr);

		for(auto platform : platformIDs) {
			XdevLPlatformInfo info;
			info.id = platform;

			std::size_t size;
			ret = clGetPlatformInfo(platform, CL_PLATFORM_PROFILE, sizeof(info.profile), (void*)info.profile, &size);
			ret = clGetPlatformInfo(platform, CL_PLATFORM_VERSION, sizeof(info.version), (void*)info.version, &size);
			ret = clGetPlatformInfo(platform, CL_PLATFORM_NAME, sizeof(info.name), (void*)info.name, &size);
			ret = clGetPlatformInfo(platform, CL_PLATFORM_VENDOR, sizeof(info.vendor), (void*)info.vendor, &size);

			if(CL_SUCCESS != ret) {
				return -1;
			}

			std::cout << "Platform Name       : " << info.name << std::endl;
			std::cout << "Platform Profile    : " << info.profile << std::endl;
			std::cout << "Platform Version    : " << info.version << std::endl;
			std::cout << "Platform Vendor     : " << info.vendor << std::endl;


			m_platforms.push_back(std::move(info));
		}

		for(auto platform : m_platforms) {
			cl_uint numDevices;
			ret = clGetDeviceIDs(platform.id, CL_DEVICE_TYPE_ALL, 1, &m_deviceID, &numDevices);
			std::cout << "OpenCL Number of devices: " << numDevices << ", ID: " << m_deviceID << std::endl;

			XdevLDeviceInfo device;
			device.id = m_deviceID;

			std::size_t size;
			ret = clGetDeviceInfo(m_deviceID, CL_DEVICE_NAME, sizeof(device.name), (void*)device.name, &size);
			ret = clGetDeviceInfo(m_deviceID, CL_DEVICE_VENDOR, sizeof(device.vendor), (void*)device.vendor, &size);
			ret = clGetDeviceInfo(m_deviceID, CL_DRIVER_VERSION, sizeof(device.version), (void*)device.version, &size);
			ret = clGetDeviceInfo(m_deviceID, CL_DEVICE_PROFILE, sizeof(device.profile), (void*)device.profile, &size);

			if(CL_SUCCESS != ret) {
				return -1;
			}

			std::cout << "Device Profile    : " << device.profile << std::endl;
			std::cout << "Device Name       : " << device.name << std::endl;
			std::cout << "Device Version    : " << device.version << std::endl;
			std::cout << "Device Vendor     : " << device.vendor << std::endl;


			m_devices.push_back(std::move(device));
		}
		return 0;
	}

	int XdevLComputeDeviceCL::shutdown() {

		return 0;
	}

	XdevLComputeDeviceContext* XdevLComputeDeviceCL::createContext(const XdevLComputePlatformId platformId, const XdevLComputeDeviceId& deviceId) {
		cl_int ret;
		const cl_context_properties properties [] = {
			CL_CONTEXT_PLATFORM,
			reinterpret_cast<cl_context_properties>(m_platforms[platformId].id),
			0,
			0
		};

		cl_context ctx = clCreateContext(properties, 1, &m_devices[deviceId].id, nullptr, nullptr, &ret);
		if(nullptr == ctx) {
			return nullptr;
		}

		auto tmp = new XdevLComputeDeviceContextCL(m_devices[deviceId].id, ctx);
		return tmp;
	}

}
