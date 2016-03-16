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

#include "XdevLWindowX11.h"
#include "XdevLWindowServerX11.h"

//
// The XdevLWindowServer module descriptor.
//
xdl::XdevLModuleDescriptor windowServerX11Desc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_WINDOW_SERVER_MODULE_NAME],
	xdl::window_copyright,
	xdl::windowServerDescription,
	XDEVLX11_SERVER_MODULE_MAJOR_VERSION,
	XDEVLX11_SERVER_MODULE_MINOR_VERSION,
	XDEVLX11_SERVER_MODULE_PATCH_VERSION
};

namespace xdl {

	XdevLWindowServerX11::XdevLWindowServerX11(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor) :
		XdevLWindowServerImpl(parameter, desriptor) {

	}

	XdevLWindowServerX11::~XdevLWindowServerX11() {

	}

	xdl_int XdevLWindowServerX11::create(XdevLWindow** window, const XdevLWindowAttribute& attribute) {

		XdevLWindowX11* wnd = new XdevLWindowX11(nullptr, getDescriptor());
		wnd->create(attribute);
		*window = wnd;
		m_windowList[wnd->getWindowID()] = wnd;
		return ERR_OK;
	}
}