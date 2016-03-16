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

#ifndef XDEVL_X11_ENVIRONMENT_H
#define XDEVL_X11_ENVIRONMENT_H

#include "XdevLDisplayX11.h"
#include "XdevLCursorX11.h"
#include "XdevLWindowEventServerX11.h"

namespace xdl {

	class XdevLX11Environment {
		public:
			XdevLX11Environment(XdevLCoreMediator* core);
			~XdevLX11Environment();

			XdevLDisplayX11* getDisplay() {
				return display.get();
			}

			XdevLWindowEventServerX11* getWindowEventServer() {
				return windowEventServer.get();
			}

			XdevLCursorX11* getCursor() {
				return cursor.get();
			}

		public:
			XdevLCoreMediator* m_core;
			std::shared_ptr<XdevLDisplayX11> display;
			std::shared_ptr<XdevLWindowEventServerX11> windowEventServer;
			std::shared_ptr<XdevLCursorX11> cursor;
	};

	extern std::shared_ptr<XdevLX11Environment> globalX11Display;
}

#endif
