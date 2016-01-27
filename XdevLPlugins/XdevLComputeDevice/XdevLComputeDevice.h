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

#ifndef XDEVL_COMPUTE_DEVICE_MODULE_H
#define XDEVL_COMPUTE_DEVICE_MODULE_H

#include <XdevLModule.h>

namespace xdl {

	enum XdevLComputePlatformId {
	  XDEVL_COMPUTE_PLATFORM_DEFAULT = 0,
	  XDEVL_COMPUTE_PLATFORM_0 = 0,
	  XDEVL_COMPUTE_PLATFORM_1,
	  XDEVL_COMPUTE_PLATFORM_2,
	  XDEVL_COMPUTE_PLATFORM_3,
	};

	enum XdevLComputeDeviceId {
	  XDEVL_COMPUTE_DEVICE_DEFAULT = 0,
	  XDEVL_COMPUTE_DEVICE_0 = 0,
	  XDEVL_COMPUTE_DEVICE_1,
	  XDEVL_COMPUTE_DEVICE_2,
	  XDEVL_COMPUTE_DEVICE_3,
	};

	enum XdevLComputeDeviceBufferAccessType {
	  XDEVL_COMPUTE_BUFFER_READ_ONLY,
	  XDEVL_COMPUTE_BUFFER_WRITE_ONLY,
	  XDEVL_COMPUTE_BUFFER_READ_WRITE,
	};

	class XdevLComputeDeviceQueue {
		public:
			virtual ~XdevLComputeDeviceQueue() {}
	};

	class XdevLComputeDeviceBuffer {
		public:
			virtual ~XdevLComputeDeviceBuffer() {}
			virtual xdl_int upload(XdevLComputeDeviceQueue* queue, xdl_uint size, xdl_uint8* data) = 0;
			virtual xdl_int download(XdevLComputeDeviceQueue* queue, xdl_uint size, xdl_uint8* data) = 0;
	};

	class XdevLComputeKernel {
		public:
			virtual ~XdevLComputeKernel() {}
			virtual xdl_int setArgument(xdl_int argumentID, XdevLComputeDeviceBuffer* argument) = 0;
	};

	class XdevLComputeExecuteParameter {
		public:
			XdevLComputeExecuteParameter(XdevLComputeDeviceQueue* q, XdevLComputeKernel* k, std::vector<std::size_t> g, std::vector<std::size_t> l) :
				queue(q),
				kernel(k),
				global(std::move(g)),
				local(std::move(l)) {}
			XdevLComputeDeviceQueue* queue;
			XdevLComputeKernel* kernel;
			std::vector<std::size_t> global;
			std::vector<std::size_t> local;
	};

	class XdevLComputeProgram {
		public:
			virtual ~XdevLComputeProgram() {}

			virtual std::shared_ptr<XdevLComputeKernel> compileFromFile(const XdevLFileName& filename, const XdevLString& kernelName) = 0;
			virtual xdl_int execute(const XdevLComputeExecuteParameter& parameter) = 0;
	};

	class XdevLComputeDeviceContext {
		public:
			virtual ~XdevLComputeDeviceContext() {}

			virtual std::shared_ptr<XdevLComputeDeviceQueue> createCommandQueue() = 0;
			virtual std::shared_ptr<XdevLComputeProgram> createProgram() = 0;
			virtual XdevLComputeDeviceBuffer* createBuffer(const XdevLComputeDeviceBufferAccessType& access, xdl_uint64 size) = 0;
	};


	/**
		@class XdevLComputeDevice
	*/
	class XdevLComputeDevice : public XdevLModule {
		public:
			virtual ~XdevLComputeDevice() {};
			virtual XdevLComputeDeviceContext* createContext(const XdevLComputePlatformId platformId = XDEVL_COMPUTE_PLATFORM_DEFAULT, const XdevLComputeDeviceId& deviceId = XDEVL_COMPUTE_DEVICE_DEFAULT) = 0;
	};

	typedef XdevLComputeDevice	IXdevLComputeDevice;
	typedef XdevLComputeDevice*	IPXdevLComputeDevice;

	XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DECLARATION(XdevLComputeDevice)
}

#endif
