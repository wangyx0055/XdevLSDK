#ifndef XDEVL_WINDOW_X11_KEYBOARD_H
#define XDEVL_WINDOW_X11_KEYBOARD_H

#include <XdevL.h>
#include <XdevLKeyboard/XdevLKeyboard.h>

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
			void sendKeyboardEvent(xdl_uint64 state, KeyCode keycode, xdl_uint windowID);

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
