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

#ifndef XDEVL_WINDOW_EVENT_SERVER_X11_H
#define XDEVL_WINDOW_EVENT_SERVER_X11_H

#include <XdevLWindow/XdevLWindowImpl.h>

#include "XdevLWindowX11Keyboard.h"

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/extensions/Xrandr.h>

namespace xdl {

	class XdevLWindowEventServerX11 : public XdevLWindowEventServerImpl {
		public:
			XdevLWindowEventServerX11(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor);
			virtual ~XdevLWindowEventServerX11();
			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;
			virtual void* getInternal(const XdevLInternalName& id) override;
			virtual xdl_int update() override;

			virtual xdl_int registerWindowForEvents(XdevLWindow* window) override;
			virtual xdl_int unregisterWindowFromEvents(XdevLWindow* window) override;
			void flush() override;

			Display* getNativeDisplay();
			Window getNativeRootWindow();

		private:
			Display* m_display;
			Window m_rootWindow;

			int pollEvents();
			XdevLWindow* m_focusWindow;
			XdevLWindowX11Keyboard* m_keyboard;
			xdl_int m_event_basep;
			xdl_int m_error_basep;

			Atom WM_PROTOCOLS;
			Atom WM_DELETE_WINDOW;
			Atom _NET_WM_PING;

			XConfigureEvent m_prevConfigureEvent;
	};
}

#endif
