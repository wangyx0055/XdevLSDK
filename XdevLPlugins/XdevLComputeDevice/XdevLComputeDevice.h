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


	class XdevLComputeDeviceQueue {
		public:
			virtual ~XdevLComputeDeviceQueue() {}
	};


	class XdevLComputeDeviceContext {
		public:
			virtual ~XdevLComputeDeviceContext() {}

			virtual XdevLComputeDeviceQueue* createCommandBuffer() = 0;
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
