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

	const xdl_char* clErrorAsString(cl_int error) {
		switch(error) {
			case CL_SUCCESS: return "CL_SUCCESS";
			case CL_DEVICE_NOT_FOUND: return "CL_DEVICE_NOT_FOUND";
			case CL_DEVICE_NOT_AVAILABLE: return "CL_DEVICE_NOT_AVAILABLE";
			case CL_COMPILER_NOT_AVAILABLE: return "CL_COMPILER_NOT_AVAILABLE";
			case CL_MEM_OBJECT_ALLOCATION_FAILURE: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
			case CL_OUT_OF_RESOURCES: return "CL_OUT_OF_RESOURCES";
			case CL_OUT_OF_HOST_MEMORY: return "CL_OUT_OF_HOST_MEMORY";
			case CL_PROFILING_INFO_NOT_AVAILABLE: return "CL_PROFILING_INFO_NOT_AVAILABLE";
			case CL_MEM_COPY_OVERLAP: return "CL_MEM_COPY_OVERLAP";
			case CL_IMAGE_FORMAT_MISMATCH: return "CL_IMAGE_FORMAT_MISMATCH";
			case CL_IMAGE_FORMAT_NOT_SUPPORTED: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
			case CL_BUILD_PROGRAM_FAILURE: return "CL_BUILD_PROGRAM_FAILURE";
			case CL_MAP_FAILURE: return "CL_MAP_FAILURE";
			case CL_MISALIGNED_SUB_BUFFER_OFFSET: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
			case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";

			case CL_INVALID_VALUE: return "CL_INVALID_VALUE";
			case CL_INVALID_DEVICE_TYPE: return "CL_INVALID_DEVICE_TYPE";
			case CL_INVALID_PLATFORM: return "CL_INVALID_PLATFORM";
			case CL_INVALID_DEVICE: return "CL_INVALID_DEVICE";
			case CL_INVALID_CONTEXT: return "CL_INVALID_CONTEXT";
			case CL_INVALID_QUEUE_PROPERTIES: return "CL_INVALID_QUEUE_PROPERTIES";
			case CL_INVALID_COMMAND_QUEUE: return "CL_INVALID_COMMAND_QUEUE";
			case CL_INVALID_HOST_PTR: return "CL_INVALID_HOST_PTR";
			case CL_INVALID_MEM_OBJECT: return "CL_INVALID_MEM_OBJECT";
			case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
			case CL_INVALID_IMAGE_SIZE: return "CL_INVALID_IMAGE_SIZE";
			case CL_INVALID_SAMPLER: return "CL_INVALID_SAMPLER";
			case CL_INVALID_BINARY: return "CL_INVALID_BINARY";
			case CL_INVALID_BUILD_OPTIONS: return "CL_INVALID_BUILD_OPTIONS";
			case CL_INVALID_PROGRAM: return "CL_INVALID_PROGRAM";
			case CL_INVALID_PROGRAM_EXECUTABLE: return "CL_INVALID_PROGRAM_EXECUTABLE";
			case CL_INVALID_KERNEL_NAME: return "CL_INVALID_KERNEL_NAME";
			case CL_INVALID_KERNEL_DEFINITION: return "CL_INVALID_KERNEL_DEFINITION";
			case CL_INVALID_KERNEL: return "CL_INVALID_KERNEL";
			case CL_INVALID_ARG_INDEX: return "CL_INVALID_ARG_INDEX";
			case CL_INVALID_ARG_VALUE: return "CL_INVALID_ARG_VALUE";
			case CL_INVALID_ARG_SIZE: return "CL_INVALID_ARG_SIZE";
			case CL_INVALID_KERNEL_ARGS: return "CL_INVALID_KERNEL_ARGS";
			case CL_INVALID_WORK_DIMENSION: return "CL_INVALID_WORK_DIMENSION";
			case CL_INVALID_WORK_GROUP_SIZE: return "CL_INVALID_WORK_GROUP_SIZE";
			case CL_INVALID_WORK_ITEM_SIZE: return "CL_INVALID_WORK_ITEM_SIZE";
			case CL_INVALID_GLOBAL_OFFSET: return "CL_INVALID_GLOBAL_OFFSET";

			case CL_INVALID_EVENT_WAIT_LIST: return "CL_INVALID_EVENT_WAIT_LIST";
			case CL_INVALID_EVENT: return "CL_INVALID_EVENT";
			case CL_INVALID_OPERATION: return "CL_INVALID_OPERATION";
			case CL_INVALID_GL_OBJECT: return "CL_INVALID_GL_OBJECT";
			case CL_INVALID_BUFFER_SIZE: return "CL_INVALID_BUFFER_SIZE";
			case CL_INVALID_MIP_LEVEL: return "CL_INVALID_MIP_LEVEL";
			case CL_INVALID_GLOBAL_WORK_SIZE: return "CL_INVALID_GLOBAL_WORK_SIZE";
			case CL_INVALID_PROPERTY: return "CL_INVALID_PROPERTY";
			default: break;
		}

		return "CL_UNKOWN_ERROR";
	}

//
//
//
	XdevLComputeKernelCL::XdevLComputeKernelCL(cl_kernel kernel) :
		m_kernel(kernel) {
		XDEVL_MODULEX_INFO(XdevLComputeKernelCL, "XdevLComputeKernelCL()\n");
	}

	XdevLComputeKernelCL::~XdevLComputeKernelCL() {
		XDEVL_MODULEX_INFO(XdevLComputeKernelCL, "~XdevLComputeKernelCL()\n");
		if(nullptr != m_kernel) {
			clReleaseKernel(m_kernel);
		}
	}

	xdl_int XdevLComputeKernelCL::setArgument(xdl_int argumentID, XdevLComputeDeviceBuffer* argument) {
		XdevLComputeDeviceBufferCL* buffer = static_cast<XdevLComputeDeviceBufferCL*>(argument);

		cl_int ret = clSetKernelArg(m_kernel, argumentID, sizeof(cl_mem), &buffer->getMemory());
		if(CL_SUCCESS != ret) {
			XDEVL_MODULEX_ERROR(XdevLComputeProgramCL, "clEnqueueTask failed: " << clErrorAsString(ret) << std::endl);
			return ERR_ERROR;
		}
		return ERR_OK;
	}

//
//
//

	XdevLComputeDeviceBufferCL::XdevLComputeDeviceBufferCL(cl_mem mem) :
		m_memory(mem) {
	}

	XdevLComputeDeviceBufferCL::~XdevLComputeDeviceBufferCL() {
		if(nullptr != m_memory) {
			clReleaseMemObject(m_memory);
		}
	}


	xdl_int XdevLComputeDeviceBufferCL::upload(XdevLComputeDeviceQueue* queue, xdl_uint size, xdl_uint8* data) {
		auto queueCL = static_cast<XdevLComputeDeviceQueueCL*>(queue);

		cl_int ret = clEnqueueWriteBuffer(queueCL->getCommandQueue(), m_memory, CL_TRUE, 0, size, data, 0, nullptr, nullptr);
		if(CL_SUCCESS != ret) {
			XDEVL_MODULEX_ERROR(XdevLComputeDeviceBufferCL,"clEnqueueWriteBuffer failed: " << clErrorAsString(ret) << std::endl);
			return ERR_ERROR;
		}

		return ERR_OK;
	}

	xdl_int XdevLComputeDeviceBufferCL::download(XdevLComputeDeviceQueue* queue, xdl_uint size, xdl_uint8* data) {
		auto queueCL = static_cast<XdevLComputeDeviceQueueCL*>(queue);

		cl_int ret = clEnqueueReadBuffer(queueCL->getCommandQueue(), m_memory, CL_TRUE, 0, size, data, 0, nullptr, nullptr);
		if(CL_SUCCESS != ret) {
			XDEVL_MODULEX_ERROR(XdevLComputeDeviceBufferCL,"clEnqueueWriteBuffer failed: " << clErrorAsString(ret) << std::endl);
			return ERR_ERROR;
		}

		return ERR_OK;
	}

//
//
//

	XdevLComputeProgramCL::XdevLComputeProgramCL(cl_device_id deviceid, cl_context context) :
		m_deviceId(deviceid),
		m_context(context),
		m_program(nullptr) {
		XDEVL_MODULEX_INFO(XdevLComputeProgramCL, "XdevLComputeProgramCL()\n");
	}

	XdevLComputeProgramCL::~XdevLComputeProgramCL() {
		XDEVL_MODULEX_INFO(XdevLComputeProgramCL, "~XdevLComputeProgramCL()\n");
		if(nullptr != m_program) {
			clReleaseProgram(m_program);
		}
	}

	std::shared_ptr<XdevLComputeKernel> XdevLComputeProgramCL::compileFromFile(const XdevLFileName& filename, const XdevLString& kernelName) {
		std::fstream file(filename.toString().c_str());
		if(!file.is_open()) {
			XDEVL_MODULEX_ERROR(XdevLComputeProgramCL,"file.is_open failed: File not found: " << filename << std::endl);
			return nullptr;
		}

		std::stringstream kernel_source;
		kernel_source << file.rdbuf();
		file.close();

		std::vector<std::string> source;
		source.push_back(kernel_source.str());

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
			XDEVL_MODULEX_ERROR(XdevLComputeProgramCL,"clCreateProgramWithSource failed: " << clErrorAsString(ret) << std::endl);
			return nullptr;
		}

		ret = clBuildProgram(m_program, 1, &m_deviceId, nullptr, nullptr, nullptr);
		if(CL_SUCCESS != ret) {
			XDEVL_MODULEX_ERROR(XdevLComputeProgramCL, "clBuildProgram failed: " << clErrorAsString(ret) << std::endl);
			return nullptr;
		}

		cl_kernel tmp = clCreateKernel(m_program, kernelName.toString().c_str(), &ret);
		if(CL_SUCCESS != ret) {
			XDEVL_MODULEX_ERROR(XdevLComputeProgramCL, "clCreateKernel failed: " << clErrorAsString(ret) << std::endl);
			return nullptr;
		}

		auto kernel = std::make_shared<XdevLComputeKernelCL>(tmp);

		return kernel;
	}

	xdl_int XdevLComputeProgramCL::execute(XdevLComputeDeviceQueue* queue, XdevLComputeKernel* kernel) {
		auto queueCL = static_cast<XdevLComputeDeviceQueueCL*>(queue);
		auto kernelCL = static_cast<XdevLComputeKernelCL*>(kernel);

		cl_int ret = clEnqueueTask(queueCL->getCommandQueue(), kernelCL->getKernel(), 0, nullptr, nullptr);
		if(CL_SUCCESS != ret) {
			XDEVL_MODULEX_ERROR(XdevLComputeProgramCL, "clEnqueueTask failed: " << clErrorAsString(ret) << std::endl);
			return ERR_ERROR;
		}

//		size_t workgroup_size;
//		cl_int ret = clGetKernelWorkGroupInfo(m_kernel, m_deviceId, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &workgroup_size, nullptr);
//		if(CL_SUCCESS != ret) {
//			XDEVL_MODULEX_ERROR(XdevLComputeProgramCL, "clGetKernelWorkGroupInfo failed: " << clErrorAsString(ret) << std::endl);
//			return ERR_ERROR;
//		}
//
//		size_t global[1] = {32};
//		size_t local[1]  = {32};
//
//		ret = clEnqueueNDRangeKernel(tmp->getCommandQueue(), m_kernel, 1, nullptr, global, local, 0, nullptr, nullptr);

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

	std::shared_ptr<XdevLComputeDeviceQueue> XdevLComputeDeviceContextCL::createCommandQueue() {
		cl_int ret;
		cl_command_queue commandQueue = clCreateCommandQueue(m_context, m_deviceID, 0, &ret);
		auto tmp = std::make_shared<XdevLComputeDeviceQueueCL>(commandQueue);
		return tmp;
	}

	std::shared_ptr<XdevLComputeProgram> XdevLComputeDeviceContextCL::createProgram() {
		auto tmp = std::make_shared<XdevLComputeProgramCL>(m_deviceID, m_context);
		return tmp;
	}

	XdevLComputeDeviceBuffer* XdevLComputeDeviceContextCL::createBuffer(const XdevLComputeDeviceBufferAccessType& access, xdl_uint64 size) {
		cl_int acc = CL_MEM_READ_ONLY;
		switch(access) {
			case XDEVL_COMPUTE_BUFFER_READ_ONLY: acc = CL_MEM_READ_ONLY; break;
			case XDEVL_COMPUTE_BUFFER_WRITE_ONLY: acc = CL_MEM_WRITE_ONLY; break;
			case XDEVL_COMPUTE_BUFFER_READ_WRITE: acc = CL_MEM_READ_WRITE; break;
		}

		cl_int ret;
		cl_mem mem = clCreateBuffer(m_context, acc, size, nullptr, &ret);
		if(CL_SUCCESS != ret) {
			XDEVL_MODULEX_ERROR(XdevLComputeDeviceBuffer, "clCreateBuffer failed: " << clErrorAsString(ret) << std::endl);
			return nullptr;
		}

		XdevLComputeDeviceBufferCL* tmp = new XdevLComputeDeviceBufferCL(mem);
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
		XDEVL_MODULEX_INFO(XdevLComputeDeviceQueueCL, "XdevLComputeDeviceQueueCL()\n");
	}

	XdevLComputeDeviceQueueCL::~XdevLComputeDeviceQueueCL() {
		XDEVL_MODULEX_INFO(XdevLComputeDeviceQueueCL, "~XdevLComputeDeviceQueueCL()\n");
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
		XDEVL_MODULEX_INFO(XdevLComputeDeviceCL, "XdevLComputeDeviceCL()\n");
	}

	XdevLComputeDeviceCL::~XdevLComputeDeviceCL() {
		XDEVL_MODULEX_INFO(XdevLComputeDeviceCL, "~XdevLComputeDeviceCL()\n");
	}

	int XdevLComputeDeviceCL::init() {
		cl_int ret;

		cl_uint numPlatforms;
		ret = clGetPlatformIDs(1, &m_platformID, &numPlatforms);
		if(CL_SUCCESS != ret) {
			XDEVL_MODULEX_ERROR(XdevLComputeDeviceCL, "clGetPlatformIDs failed: " << clErrorAsString(ret) << std::endl);
			return -1;
		}

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
				XDEVL_MODULEX_ERROR(XdevLComputeDeviceCL, "clGetPlatformInfo failed: " << clErrorAsString(ret) << std::endl);
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
			if(CL_SUCCESS != ret) {
				XDEVL_MODULEX_ERROR(XdevLComputeDeviceCL, "clGetDeviceIDs failed: " << clErrorAsString(ret) << std::endl);
				return -1;
			}
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
