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

	class XdevLComputeDeviceQueueCL : public XdevLComputeDeviceQueue {
		public:
			XdevLComputeDeviceQueueCL(cl_command_queue queue) : m_CommandQueue(queue) {}
		private:
			cl_command_queue m_CommandQueue;
	};

//
//
//

	class XdevLComputeDeviceContextCL : public XdevLComputeDeviceContext {
		public:
			XdevLComputeDeviceContextCL(cl_device_id id, cl_context ctx);

			virtual ~XdevLComputeDeviceContextCL();

			XdevLComputeDeviceQueue* createCommandBuffer();

			cl_device_id getDeviceId();
			cl_context getContext();

		private:
			cl_device_id m_deviceID;
			cl_context m_context;
	};



//
//
//

	class XdevLComputeDeviceCL : public XdevLModuleImpl<XdevLComputeDeviceCL> {
		public:
			XdevLComputeDeviceCL(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor);
			virtual ~XdevLComputeDeviceCL();
			virtual int init();
			virtual int shutdown();

			virtual XdevLComputeDeviceContext* createContext(const XdevLComputePlatformId platformId = XDEVL_COMPUTE_PLATFORM_DEFAULT, const XdevLComputeDeviceId& deviceId = XDEVL_COMPUTE_DEVICE_DEFAULT);

		private:
			cl_platform_id m_platformID;
			cl_device_id m_deviceID;
			std::vector<XdevLPlatformInfo> m_platforms;
			std::vector<XdevLDeviceInfo> m_devices;
	};

	XdevLComputeDeviceCL::XdevLComputeDeviceCL() :
		m_platformID(nullptr),
		m_deviceID(nullptr) {

	}

	XdevLComputeDeviceCL::~XdevLComputeDeviceCL() {

	}

}

#endif
