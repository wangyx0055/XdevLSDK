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

#include "XdevLWindowCocoa.h"
#include "XdevLCursorCocoa.h"

static const xdl::XdevLString cursorDescription {
	"Support for handling the cursor using Cocoa."
};

xdl::XdevLModuleDescriptor cursorCocoaDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_CURSOR_MODULE_NAME],
	xdl::window_copyright,
	xdl::windowServerDescription,
	XDEVLCOCOA_CURSOR_MODULE_MAJOR_VERSION,
	XDEVLCOCOA_CURSOR_MODULE_MINOR_VERSION,
	XDEVLCOCOA_CURSOR_MODULE_PATCH_VERSION
};

namespace xdl {

	XdevLCursorCocoa::XdevLCursorCocoa(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor) :
		XdevLModuleImpl<XdevLCursor>(parameter, desriptor),
		m_cursor(nullptr),
		m_window(nullptr),
		m_hidInterface(0),
		m_acceleration(0.0) {

	}

	xdl_int XdevLCursorCocoa::init() {
		mach_port_t MasterPort;
		kern_return_t KernResult = IOMasterPort(MACH_PORT_NULL, &MasterPort);
		if(KERN_SUCCESS != KernResult) {
			XDEVL_MODULE_ERROR("IOMasterPort failed.\n");
			return ERR_ERROR;
		}
		CFMutableDictionaryRef ClassesToMatch = IOServiceMatching("IOHIDSystem");
		if(nullptr == ClassesToMatch) {
			XDEVL_MODULE_ERROR("IOServiceMatching failed.\n");
			return ERR_ERROR;
		}

		io_iterator_t MatchingServices;
		KernResult = IOServiceGetMatchingServices(MasterPort, ClassesToMatch, &MatchingServices);
		if(KERN_SUCCESS != KernResult) {
			XDEVL_MODULE_ERROR("IOServiceGetMatchingServices failed.\n");
			return ERR_ERROR;
		}

		io_object_t IntfService = IOIteratorNext(MatchingServices);
		if(!IntfService) {
			XDEVL_MODULE_ERROR("IOIteratorNext failed.\n");
			return ERR_ERROR;
		}

		KernResult = IOServiceOpen(IntfService, mach_task_self(), kIOHIDParamConnectType, &m_hidInterface);
		if(KERN_SUCCESS != KernResult) {
			XDEVL_MODULE_ERROR("IOServiceOpen failed.\n");
			return ERR_ERROR;
		}
		KernResult = IOHIDGetAccelerationWithKey(m_hidInterface, CFSTR(kIOHIDMouseAccelerationType), &m_acceleration);

		return ERR_OK;
	}

	xdl_int XdevLCursorCocoa::shutdown() {
		return ERR_OK;
	}

	void* XdevLCursorCocoa::getInternal(const XdevLInternalName& id) {
		return nullptr;
	}

	xdl_int XdevLCursorCocoa::attach(XdevLWindow* window) {
		m_window = static_cast<XdevLWindowCocoa*>(window);
		return ERR_OK;
	}

	void XdevLCursorCocoa::show() {
		if([NSApp isActive]) {
			if(0 != m_hidInterface) {
				IOHIDSetAccelerationWithKey(m_hidInterface, CFSTR(kIOHIDMouseAccelerationType), m_acceleration);
			}
			//CGDisplayShowCursor(kCGDirectMainDisplay);
			[NSCursor unhide];
		}
	}

	void XdevLCursorCocoa::hide() {
		if([NSApp isActive]) {
			if(0 != m_hidInterface) {
				IOHIDSetAccelerationWithKey(m_hidInterface, CFSTR(kIOHIDMouseAccelerationType), -1);
			}
			[NSCursor hide];
			//CGDisplayHideCursor(kCGDirectMainDisplay);
		}
	}

	void XdevLCursorCocoa::setPosition(xdl_uint x, xdl_uint y) {
		CGPoint point;
		point.x = (xdl_float)x;
		point.y = (xdl_float)y;

		if(CGWarpMouseCursorPosition(point) != kCGErrorSuccess) {
			XDEVL_MODULE_ERROR("CGWarpMouseCursorPosition failed.\n");
		}
	}

	xdl_int XdevLCursorCocoa::clip(xdl_uint x, xdl_uint y, xdl_uint width, xdl_uint height) {
		return ERR_ERROR;
	}

	void XdevLCursorCocoa::releaseClip() {
	}

	xdl_int XdevLCursorCocoa::enableRelativeMotion() {
		return ERR_ERROR;
	}

	void XdevLCursorCocoa::disableRelativeMotion() {

	}

	xdl_bool XdevLCursorCocoa::isRelativeMotionEnabled() {
		return xdl_false;
	}
}
