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

#ifndef XDEVL_COMPUTE_DEVICE_CL_MODULE_IMPL_H
#define XDEVL_COMPUTE_DEVICE_CL_MODULE_IMPL_H


#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <iostream>
#include <vector>
#include <map>
#include <memory>

#include <XdevLComputeDevice/XdevLComputeDevice.h>
#include <XdevLPluginImpl.h>

namespace xdl {



	struct XdevLPlatformInfo {
		cl_platform_id id;
		char profile[128];
		char version[128];
		char name[128];
		char vendor[128];
	};

	struct XdevLDeviceInfo {
		cl_device_id id;
		char profile[128];
		char version[128];
		char name[128];
		char vendor[128];
	};

//
//
//

	class XdevLComputeDeviceBufferCL : public XdevLComputeDeviceBuffer {
		public:
			XdevLComputeDeviceBufferCL(cl_mem mem);

			virtual ~XdevLComputeDeviceBufferCL();

			xdl_int upload(XdevLComputeDeviceQueue* queue, xdl_uint size, xdl_uint8* data) override;
			xdl_int download(XdevLComputeDeviceQueue* queue, xdl_uint size, xdl_uint8* data) override;

			cl_mem& getMemory() {
				return m_memory;
			}

		private:

			cl_mem m_memory;
	};


	class XdevLComputeKernelCL : public XdevLComputeKernel {
		public:

			XdevLComputeKernelCL(cl_kernel kernel);

			virtual ~XdevLComputeKernelCL();

			xdl_int setArgumentBuffer(xdl_int argumentID, XdevLComputeDeviceBuffer* argument) override;
			xdl_int setArgumentUInt(xdl_int argumentID, xdl_uint value) override;
			xdl_int setArgumentInt(xdl_int argumentID, xdl_int value) override;
			xdl_int setArgumentFloat(xdl_int argumentID, xdl_float value) override;
			xdl_int setArgumentDouble(xdl_int argumentID, xdl_double value) override;

			cl_kernel getKernel() {
				return m_kernel;
			}

		private:

			cl_kernel m_kernel;
	};

//
//
//

	class XdevLComputeProgramCL : public XdevLComputeProgram {
		public:
			XdevLComputeProgramCL(cl_device_id deviceid, cl_context context);
			virtual ~XdevLComputeProgramCL();

			std::shared_ptr<XdevLComputeKernel> compileFromFile(const XdevLFileName& filename, const XdevLString& kernelName) override;
			xdl_int execute(const XdevLComputeExecuteParameter& parameter) override;

		private:
			cl_device_id m_deviceId;
			cl_context m_context;
			cl_program m_program;
	};

//
//
//

	class XdevLComputeDeviceQueueCL : public XdevLComputeDeviceQueue {
		public:
			XdevLComputeDeviceQueueCL(cl_command_queue queue);
			virtual ~XdevLComputeDeviceQueueCL();
			cl_command_queue getCommandQueue();
		private:
			cl_command_queue m_commandQueue;
	};

//
//
//

	class XdevLComputeDeviceContextCL : public XdevLComputeDeviceContext {
		public:
			XdevLComputeDeviceContextCL(cl_device_id id, cl_context ctx);

			virtual ~XdevLComputeDeviceContextCL();

			std::shared_ptr<XdevLComputeDeviceQueue> createCommandQueue() override;
			std::shared_ptr<XdevLComputeProgram> createProgram() override;
			XdevLComputeDeviceBuffer* createBuffer(const XdevLComputeDeviceBufferAccessType& access, xdl_uint64 size) override;

			cl_device_id getDeviceId();
			cl_context getContext();

		private:
			cl_device_id m_deviceID;
			cl_context m_context;
	};

//
//
//

	class XdevLComputeDeviceCL : public XdevLModuleImpl<XdevLComputeDevice> {
		public:
			XdevLComputeDeviceCL(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor);
			virtual ~XdevLComputeDeviceCL();
			virtual int init();
			virtual int shutdown();

			virtual XdevLComputeDeviceContext* createContext(const XdevLComputePlatformId platformId = XDEVL_COMPUTE_PLATFORM_DEFAULT, const XdevLComputeDeviceId& deviceId = XDEVL_COMPUTE_DEVICE_DEFAULT) override;

		private:
			cl_platform_id m_platformID;
			cl_device_id m_deviceID;
			std::vector<XdevLPlatformInfo> m_platforms;
			std::vector<XdevLDeviceInfo> m_devices;
	};

}

#endif
