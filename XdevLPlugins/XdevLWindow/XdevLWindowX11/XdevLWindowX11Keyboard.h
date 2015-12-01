#ifndef XDEVL_WINDOW_X11_KEYBOARD_H
#define XDEVL_WINDOW_X11_KEYBOARD_H

#include <XdevL.h>
#include <XdevLKeyboard/XdevLKeyboard.h>

#include <X11/Xlib.h>

namespace xdl {

	class XdevLWindowLinux;

	class XdevLWindowX11Keyboard {
		public:

			XdevLWindowX11Keyboard(Display* display, XdevLCoreMediator* core);

			~XdevLWindowX11Keyboard();

			/// Initialize the keyboard device.
			void initKeyboardKeyCodes();

			/// Update the keyboard map.
			void updateKeyboardMap();

			/// Cast key event.
			void sendKeyboardEvent(xdl_int state, KeyCode keycode, xdl_uint windowID);

			/// Convert keycode to Unicode.
			xdl_uint32 keyCodeToUcs4(KeyCode keycode);

			/// Convert keycode to XdevLScanCode.
			XdevLScanCode keyCodeToXdevLScancode(KeyCode keycode);			
			
			/// Convertes X11 KeyCode to XdevL KeyCode.
			XdevLKeyCode keyCodeToXdevLKeyCode(KeyCode keycode);
			
		private:
			XdevLCoreMediator*		m_core;
			Display*							m_display;
			XdevLScanCode 				m_scanCodeLayout[256];
			XdevLKeyCode 					m_keyCodeLayout[XDEVL_NUM_SCANCODES];
	};

}

#endif
