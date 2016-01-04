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

#ifndef XDEVL_WINDOW_X11_KEYBOARD_H
#define XDEVL_WINDOW_X11_KEYBOARD_H

#include <XdevL.h>
#include <XdevLInput/XdevLKeyboard/XdevLKeyboard.h>

#include <X11/Xlib.h>

namespace xdl {

	class XdevLWindowLinux;

	extern const XdevLID ButtonPressed;
	extern const XdevLID ButtonReleased;

	class XdevLWindowX11Keyboard {
		public:

			XdevLWindowX11Keyboard(Display* display, XdevLCoreMediator* core);

			~XdevLWindowX11Keyboard();

			/// Cast key event.
			void sendKeyboardEvent(xdl_uint64 state, KeyCode keycode, xdl_uint8 repeat, xdl_uint windowID);

			/// Convert keycode to Unicode.
			xdl_uint32 keyCodeToUcs4(KeyCode keycode);

			/// Convertes X11 KeyCode to XdevL KeyCode.
			XdevLButtonId x11KeyCodeToXdevLButtonId(KeyCode keycode);

		private:
			XdevLCoreMediator*		m_core;
			Display*				m_display;
	};

}

#endif
