#include "XdevLWindowX11Keyboard.h"


#include <X11/keysym.h>
#include <X11/XKBlib.h>

#include "scancodes_xfree86.h"
#include "scancodes_darwin.h"
#include "scancodes_linux.h"


extern "C" {
#include "imKStoUCS.h"
}


namespace xdl {

	static const struct {
		KeySym keysym;
		XdevLScanCode scancode;
	} KeySymToXdevLScancode[] = {
		{ XK_Return, XDEVL_SCANCODE_RETURN },
		{ XK_Escape, XDEVL_SCANCODE_ESCAPE },
		{ XK_BackSpace, XDEVL_SCANCODE_BACKSPACE },
		{ XK_Tab, XDEVL_SCANCODE_TAB },
		{ XK_Caps_Lock, XDEVL_SCANCODE_CAPSLOCK },
		{ XK_F1, XDEVL_SCANCODE_F1 },
		{ XK_F2, XDEVL_SCANCODE_F2 },
		{ XK_F3, XDEVL_SCANCODE_F3 },
		{ XK_F4, XDEVL_SCANCODE_F4 },
		{ XK_F5, XDEVL_SCANCODE_F5 },
		{ XK_F6, XDEVL_SCANCODE_F6 },
		{ XK_F7, XDEVL_SCANCODE_F7 },
		{ XK_F8, XDEVL_SCANCODE_F8 },
		{ XK_F9, XDEVL_SCANCODE_F9 },
		{ XK_F10, XDEVL_SCANCODE_F10 },
		{ XK_F11, XDEVL_SCANCODE_F11 },
		{ XK_F12, XDEVL_SCANCODE_F12 },
		{ XK_Print, XDEVL_SCANCODE_PRINTSCREEN },
		{ XK_Scroll_Lock, XDEVL_SCANCODE_SCROLLLOCK },
		{ XK_Pause, XDEVL_SCANCODE_PAUSE },
		{ XK_Insert, XDEVL_SCANCODE_INSERT },
		{ XK_Home, XDEVL_SCANCODE_HOME },
		{ XK_Prior, XDEVL_SCANCODE_PAGEUP },
		{ XK_Delete, XDEVL_SCANCODE_DELETE },
		{ XK_End, XDEVL_SCANCODE_END },
		{ XK_Next, XDEVL_SCANCODE_PAGEDOWN },
		{ XK_Right, XDEVL_SCANCODE_RIGHT },
		{ XK_Left, XDEVL_SCANCODE_LEFT },
		{ XK_Down, XDEVL_SCANCODE_DOWN },
		{ XK_Up, XDEVL_SCANCODE_UP },
		{ XK_Num_Lock, XDEVL_SCANCODE_NUMLOCKCLEAR },
		{ XK_KP_Divide, XDEVL_SCANCODE_KP_DIVIDE },
		{ XK_KP_Multiply, XDEVL_SCANCODE_KP_MULTIPLY },
		{ XK_KP_Subtract, XDEVL_SCANCODE_KP_MINUS },
		{ XK_KP_Add, XDEVL_SCANCODE_KP_PLUS },
		{ XK_KP_Enter, XDEVL_SCANCODE_KP_ENTER },
		{ XK_KP_Delete, XDEVL_SCANCODE_KP_PERIOD },
		{ XK_KP_End, XDEVL_SCANCODE_KP_1 },
		{ XK_KP_Down, XDEVL_SCANCODE_KP_2 },
		{ XK_KP_Next, XDEVL_SCANCODE_KP_3 },
		{ XK_KP_Left, XDEVL_SCANCODE_KP_4 },
		{ XK_KP_Begin, XDEVL_SCANCODE_KP_5 },
		{ XK_KP_Right, XDEVL_SCANCODE_KP_6 },
		{ XK_KP_Home, XDEVL_SCANCODE_KP_7 },
		{ XK_KP_Up, XDEVL_SCANCODE_KP_8 },
		{ XK_KP_Prior, XDEVL_SCANCODE_KP_9 },
		{ XK_KP_Insert, XDEVL_SCANCODE_KP_0 },
		{ XK_KP_Decimal, XDEVL_SCANCODE_KP_PERIOD },
		{ XK_KP_1, XDEVL_SCANCODE_KP_1 },
		{ XK_KP_2, XDEVL_SCANCODE_KP_2 },
		{ XK_KP_3, XDEVL_SCANCODE_KP_3 },
		{ XK_KP_4, XDEVL_SCANCODE_KP_4 },
		{ XK_KP_5, XDEVL_SCANCODE_KP_5 },
		{ XK_KP_6, XDEVL_SCANCODE_KP_6 },
		{ XK_KP_7, XDEVL_SCANCODE_KP_7 },
		{ XK_KP_8, XDEVL_SCANCODE_KP_8 },
		{ XK_KP_9, XDEVL_SCANCODE_KP_9 },
		{ XK_KP_0, XDEVL_SCANCODE_KP_0 },
		{ XK_KP_Decimal, XDEVL_SCANCODE_KP_PERIOD },
		{ XK_Hyper_R, XDEVL_SCANCODE_APPLICATION },
		{ XK_KP_Equal, XDEVL_SCANCODE_KP_EQUALS },
		{ XK_F13, XDEVL_SCANCODE_F13 },
		{ XK_F14, XDEVL_SCANCODE_F14 },
		{ XK_F15, XDEVL_SCANCODE_F15 },
		{ XK_F16, XDEVL_SCANCODE_F16 },
		{ XK_F17, XDEVL_SCANCODE_F17 },
		{ XK_F18, XDEVL_SCANCODE_F18 },
		{ XK_F19, XDEVL_SCANCODE_F19 },
		{ XK_F20, XDEVL_SCANCODE_F20 },
		{ XK_F21, XDEVL_SCANCODE_F21 },
		{ XK_F22, XDEVL_SCANCODE_F22 },
		{ XK_F23, XDEVL_SCANCODE_F23 },
		{ XK_F24, XDEVL_SCANCODE_F24 },
		{ XK_Execute, XDEVL_SCANCODE_EXECUTE },
		{ XK_Help, XDEVL_SCANCODE_HELP },
		{ XK_Menu, XDEVL_SCANCODE_MENU },
		{ XK_Select, XDEVL_SCANCODE_SELECT },
		{ XK_Cancel, XDEVL_SCANCODE_STOP },
		{ XK_Redo, XDEVL_SCANCODE_AGAIN },
		{ XK_Undo, XDEVL_SCANCODE_UNDO },
		{ XK_Find, XDEVL_SCANCODE_FIND },
		{ XK_KP_Separator, XDEVL_SCANCODE_KP_COMMA },
		{ XK_Sys_Req, XDEVL_SCANCODE_SYSREQ },
		{ XK_Control_L, XDEVL_SCANCODE_LCTRL },
		{ XK_Shift_L, XDEVL_SCANCODE_LSHIFT },
		{ XK_Alt_L, XDEVL_SCANCODE_LALT },
		{ XK_Meta_L, XDEVL_SCANCODE_LGUI },
		{ XK_Super_L, XDEVL_SCANCODE_LGUI },
		{ XK_Control_R, XDEVL_SCANCODE_RCTRL },
		{ XK_Shift_R, XDEVL_SCANCODE_RSHIFT },
		{ XK_Alt_R, XDEVL_SCANCODE_RALT },
		{ XK_Meta_R, XDEVL_SCANCODE_RGUI },
		{ XK_Super_R, XDEVL_SCANCODE_RGUI },
		{ XK_Mode_switch, XDEVL_SCANCODE_MODE },
	};

	static const struct {
		KeySym keysym;
		XdevLKeyCode scancode;
	} KeySymToXdevLKeyCode[] = {
		{ XK_Return, KEY_RETURN },
		{ XK_Escape, KEY_ESCAPE},
		{ XK_BackSpace, KEY_BACKSPACE},
		{ XK_Tab, KEY_TAB },
		{ XK_Caps_Lock, KEY_CAPSLOCK},
		{ XK_F1, KEY_F1},
		{ XK_F2, KEY_F2 },
		{ XK_F3, KEY_F3 },
		{ XK_F4, KEY_F4 },
		{ XK_F5, KEY_F5 },
		{ XK_F6, KEY_F6 },
		{ XK_F7, KEY_F7 },
		{ XK_F8, KEY_F8 },
		{ XK_F9, KEY_F9 },
		{ XK_F10, KEY_F10 },
		{ XK_F11, KEY_F11 },
		{ XK_F12, KEY_F12 },
		{ XK_Print, KEY_PRINTSCREEN },
		{ XK_Scroll_Lock, KEY_SCROLLLOCK },
		{ XK_Pause, KEY_PAUSE },
		{ XK_Insert, KEY_INSERT },
		{ XK_Home, KEY_HOME },
		{ XK_Prior, KEY_PRIOR },
		{ XK_Delete, KEY_DELETE },
		{ XK_End, KEY_END },
		{ XK_Next, KEY_PAGEDOWN},
		{ XK_Right, KEY_RIGHT },
		{ XK_Left, KEY_LEFT },
		{ XK_Down, KEY_DOWN },
		{ XK_Up, KEY_UP },
		{ XK_Num_Lock, KEY_NUMLOCKCLEAR },
		{ XK_KP_Divide, KEY_KP_DIVIDE },
		{ XK_KP_Multiply, KEY_KP_MULTIPLY },
		{ XK_KP_Subtract, KEY_KP_MINUS },
		{ XK_KP_Add, KEY_KP_PLUS },
		{ XK_KP_Enter, KEY_KP_ENTER },
		{ XK_KP_Delete, KEY_KP_PERIOD },
		{ XK_KP_End, KEY_KP_1 },
		{ XK_KP_Down, KEY_KP_2 },
		{ XK_KP_Next, KEY_KP_3 },
		{ XK_KP_Left, KEY_KP_4 },
		{ XK_KP_Begin, KEY_KP_5 },
		{ XK_KP_Right, KEY_KP_6 },
		{ XK_KP_Home, KEY_KP_7 },
		{ XK_KP_Up, KEY_KP_8 },
		{ XK_KP_Prior, KEY_KP_9 },
		{ XK_KP_Insert, KEY_KP_0 },
		{ XK_KP_Decimal, KEY_KP_PERIOD },
		{ XK_KP_1, KEY_KP_1 },
		{ XK_KP_2, KEY_KP_2 },
		{ XK_KP_3, KEY_KP_3 },
		{ XK_KP_4, KEY_KP_4 },
		{ XK_KP_5, KEY_KP_5 },
		{ XK_KP_6, KEY_KP_6 },
		{ XK_KP_7, KEY_KP_7 },
		{ XK_KP_8, KEY_KP_8 },
		{ XK_KP_9, KEY_KP_9 },
		{ XK_KP_0, KEY_KP_0 },
		{ XK_KP_Decimal, KEY_KP_PERIOD },
		{ XK_Hyper_R, KEY_APPLICATION },
		{ XK_KP_Equal, KEY_KP_EQUALS },
		{ XK_F13, KEY_F13 },
		{ XK_F14, KEY_F14 },
		{ XK_F15, KEY_F15 },
		{ XK_F16, KEY_F16 },
		{ XK_F17, KEY_F17 },
		{ XK_F18, KEY_F18 },
		{ XK_F19, KEY_F19 },
		{ XK_F20, KEY_F20 },
		{ XK_F21, KEY_F21 },
		{ XK_F22, KEY_F22 },
		{ XK_F23, KEY_F23 },
		{ XK_F24, KEY_F24 },
		{ XK_Execute, KEY_EXEC },
		{ XK_Help, KEY_HELP },
		{ XK_Menu, KEY_MENU },
		{ XK_Select, KEY_SELECT },
		{ XK_Cancel, KEY_STOP },
		{ XK_Redo, KEY_AGAIN },
		{ XK_Undo, KEY_UNDO },
		{ XK_Find, KEY_FIND },
		{ XK_KP_Separator, KEY_KP_COMMA },
		{ XK_Sys_Req, KEY_SYSREQ },
		{ XK_Control_L, KEY_LCTRL },
		{ XK_Shift_L, KEY_LSHIFT },
		{ XK_Alt_L, KEY_LALT },
		{ XK_Meta_L, KEY_LGUI },
		{ XK_Super_L, KEY_LGUI },
		{ XK_Control_R, KEY_RCTRL },
		{ XK_Shift_R, KEY_RSHIFT },
		{ XK_Alt_R, KEY_RALT },
		{ XK_Meta_R, KEY_RGUI },
		{ XK_Super_R, KEY_RGUI },
		{ XK_Mode_switch, KEY_MODE }
	};

	static const struct {
		int const *table;
		int table_size;
	} scancode_set[] = {
		{ darwin_scancode_table, sizeof(darwin_scancode_table) },
		{ xfree86_scancode_table, sizeof(xfree86_scancode_table) },
		{ xfree86_scancode_table2, sizeof(xfree86_scancode_table2) },
	};




	XdevLWindowX11Keyboard::XdevLWindowX11Keyboard(Display* display, XdevLCoreMediator* core) :
		m_display(display),
		m_core(core) {
	}

	XdevLWindowX11Keyboard::~XdevLWindowX11Keyboard() {
	}

	void XdevLWindowX11Keyboard::initKeyboardKeyCodes() {
		struct {
			XdevLScanCode scancode;
			KeySym keysym;
			int value;
		} fingerprint[] = {
			{ XDEVL_SCANCODE_HOME, XK_Home, 0 },
			{ XDEVL_SCANCODE_PAGEUP, XK_Prior, 0 },
			{ XDEVL_SCANCODE_UP, XK_Up, 0 },
			{ XDEVL_SCANCODE_LEFT, XK_Left, 0 },
			{ XDEVL_SCANCODE_DELETE, XK_Delete, 0 },
			{ XDEVL_SCANCODE_KP_ENTER, XK_KP_Enter, 0 },
		};
		int best_distance;
		int best_index;
		int distance;

		int min_keycode, max_keycode;
		XAutoRepeatOn(m_display);

		int sizeOfFingerPrintArray = (sizeof(fingerprint)/sizeof(fingerprint[0]));

		best_distance =	sizeOfFingerPrintArray + 1;
		best_index = -1;

		XDisplayKeycodes(m_display, &min_keycode, &max_keycode);

		for(int i = 0; i < sizeOfFingerPrintArray; ++i) {
			fingerprint[i].value = XKeysymToKeycode(m_display, fingerprint[i].keysym) - min_keycode;
		}

		int sizeOfScanCodeArray = (sizeof(scancode_set)/sizeof(scancode_set[0]));
		for(int i = 0; i < sizeOfScanCodeArray; ++i) {

			if((max_keycode - min_keycode + 1) <= scancode_set[i].table_size) {
				continue;
			}

			distance = 0;


			for(int j = 0; j <	sizeOfFingerPrintArray; ++j) {
				if(fingerprint[j].value < 0 || fingerprint[j].value >= scancode_set[i].table_size) {
					distance += 1;
				} else if(scancode_set[i].table[fingerprint[j].value] != fingerprint[j].scancode) {
					distance += 1;
				}
			}
			if(distance < best_distance) {
				best_distance = distance;
				best_index = i;
			}
		}

		if(best_index >= 0 && best_distance <= 2) {
			memcpy(&m_scanCodeLayout[min_keycode], scancode_set[best_index].table, sizeof(XdevLScanCode) * scancode_set[best_index].table_size);
		} else {
			XdevLKeyCode keymap[XDEVL_NUM_SCANCODES];

			printf("Keyboard layout unknown, please send the following to the SDL mailing list (sdl@libsdl.org):\n");

			for(int a = 0; a < XDEVL_NUM_SCANCODES; ++a) {
				keymap[a] = m_keyCodeLayout[a];
			}

			for(int i = min_keycode; i <= max_keycode; ++i) {
				KeySym sym;

				sym = XkbKeycodeToKeysym(m_display, i, 0, 0);
				if(sym != NoSymbol) {
					XdevLScanCode scancode;
					printf("code = %d, sym = 0x%X (%s) ", i - min_keycode, (unsigned int) sym, XKeysymToString(sym));
					scancode = keyCodeToXdevLScancode(i);
					m_keyCodeLayout[i] = scancode;
					if(scancode == XDEVL_SCANCODE_UNKNOWN) {
						printf("scancode not found\n");
					} else {
						//printf("scancode = %d (%s)\n", scancode, SDL_GetScancodeName(scancode));
					}
				}
			}
		}

		updateKeyboardMap();
	}


	void XdevLWindowX11Keyboard::updateKeyboardMap() {
		XdevLScanCode scancode;

		for(int i = 0; i < (sizeof(m_scanCodeLayout)/sizeof(m_scanCodeLayout[0])); i++) {
			uint32_t key;

			/* Make sure this is a valid scancode */
			scancode = m_scanCodeLayout[i];
			if(scancode == XDEVL_SCANCODE_UNKNOWN) {
				continue;
			}

			/* See if there is a UCS keycode for this scancode */
			key = keyCodeToUcs4((KeyCode)i);
			if(key) {
				m_keyCodeLayout[scancode] = key;
			}
		}
	}

	void XdevLWindowX11Keyboard::sendKeyboardEvent(xdl_int state, KeyCode keycode, xdl_uint windowID) {
		
		XdevLKeyCode code = keyCodeToXdevLKeyCode(keycode);
		if(code == KEY_UNKNOWN) {
			// TODO Do we have to inform someone?
			return;
		}

		xdl_int mod =0;
		xdl_uint16 modstate;
		xdl_uint32 type;
		xdl_uint8 repeat;

		// Pressed
		if(state == 1) {
			switch(code) {
				case KEY_NUMLOCKCLEAR:
					mod ^= KEY_MOD_NUM;
					break;
				case KEY_CAPSLOCK:
					mod ^= KEY_MOD_CAPS;
					break;
				case KEY_LCTRL:
					mod |= KEY_MOD_LCTRL;
					break;
				case KEY_RCTRL:
					mod |= KEY_MOD_RCTRL;
					break;
				case KEY_LSHIFT:
					mod |= KEY_MOD_LSHIFT;
					break;
				case KEY_RSHIFT:
					mod |= KEY_MOD_RSHIFT;
					break;
				case KEY_LALT:
					mod |= KEY_MOD_LALT;
					break;
				case KEY_RALT:
					mod |= KEY_MOD_RALT;
					break;
				case KEY_LGUI:
					mod |= KEY_MOD_LGUI;
					break;
				case KEY_RGUI:
					mod |= KEY_MOD_RGUI;
					break;
				case KEY_MODE:
					mod |= KEY_MOD_MODE;
					break;
				default:
					break;
			}
		} else {
			switch(code) {
				case XDEVL_SCANCODE_NUMLOCKCLEAR:
				case XDEVL_SCANCODE_CAPSLOCK:
					break;
				case KEY_LCTRL:
					mod &= ~KEY_MOD_LCTRL;
					break;
				case KEY_RCTRL:
					mod &= ~KEY_MOD_RCTRL;
					break;
				case KEY_LSHIFT:
					mod &= ~KEY_MOD_LSHIFT;
					break;
				case KEY_RSHIFT:
					mod &= ~KEY_MOD_RSHIFT;
					break;
				case KEY_LALT:
					mod &= ~KEY_MOD_LALT;
					break;
				case KEY_RALT:
					mod &= ~KEY_MOD_RALT;
					break;
				case KEY_LGUI:
					mod &= ~KEY_MOD_LGUI;
					break;
				case KEY_RGUI:
					mod &= ~KEY_MOD_RGUI;
					break;
				case KEY_MODE:
					mod &= ~KEY_MOD_MODE;
					break;
				default:
					break;
			}
			modstate = mod;
		}

		/* Figure out what type of event this is */
		switch(state) {
			case 1:
				type = XDEVL_KEY_PRESSED;
				break;
			case 0:
				type = XDEVL_KEY_RELEASED;
				break;
			default:
				/* Invalid state -- bail */
				return;
		}

		/* Update internal keyboard state */
		m_keyCodeLayout[code] = state;


		XdevLEvent ev;
		ev.common.timestamp 				= m_core->getTimer().getTime64();
		ev.type 						= type;

		ev.window.windowid	= windowID;
		ev.key.repeat 			= repeat;
		ev.key.scancode 		= code;
		ev.key.sym 					= code;
		ev.key.mod 					= modstate;

		m_core->fireEvent(ev);
	}

	XdevLScanCode XdevLWindowX11Keyboard::keyCodeToXdevLScancode(KeyCode keycode) {

		// Convert the Keycode into Symbol expression.
		KeySym keysym = XkbKeycodeToKeysym(m_display, keycode, 0, 0);
		if(keysym == NoSymbol) {

			// No Symbol expression for that keycode found.
			return XDEVL_SCANCODE_UNKNOWN;
		}

		// Are the symbolic keys letters between A - Z?
		if(keysym >= XK_A && keysym <= XK_Z) {
			return XdevLScanCode(XDEVL_SCANCODE_A + (keysym - XK_A));
		}

		// Are the symbolic keys number from 0 - 9?
		if(keysym >= XK_0 && keysym <= XK_9) {
			return XdevLScanCode(XDEVL_SCANCODE_0 + (keysym - XK_0));
		}

		// If none of the above let's check our list.
		for(int i = 0; i < (sizeof(KeySymToXdevLScancode)/sizeof(KeySymToXdevLScancode[0])); ++i) {
			if(keysym == KeySymToXdevLScancode[i].keysym) {
				return KeySymToXdevLScancode[i].scancode;
			}
		}
		return XDEVL_SCANCODE_UNKNOWN;
	}

	XdevLKeyCode XdevLWindowX11Keyboard::keyCodeToXdevLKeyCode(KeyCode keycode) {

		// Convert the Keycode into Symbol expression.
		KeySym keysym = XkbKeycodeToKeysym(m_display, keycode, 0, 0);
		if(keysym == NoSymbol) {

			// No Symbol expression for that keycode found.
			return XDEVL_SCANCODE_UNKNOWN;
		}

		// Are the symbolic keys letters between A - Z?
		if(keysym >= XK_A && keysym <= XK_Z) {
			return XdevLScanCode(XDEVL_SCANCODE_A + (keysym - XK_A));
		}

		// Are the symbolic keys number from 0 - 9?
		if(keysym >= XK_0 && keysym <= XK_9) {
			return XdevLScanCode(XDEVL_SCANCODE_0 + (keysym - XK_0));
		}

		// If none of the above let's check our list.
		for(int i = 0; i < (sizeof(KeySymToXdevLKeyCode)/sizeof(KeySymToXdevLKeyCode[0])); ++i) {
			if(keysym == KeySymToXdevLKeyCode[i].keysym) {
				return KeySymToXdevLKeyCode[i].scancode;
			}
		}
		return KEY_UNKNOWN;
	}


	xdl_uint32 XdevLWindowX11Keyboard::keyCodeToUcs4(KeyCode keycode) {
		KeySym keysym;
		keysym = XkbKeycodeToKeysym(m_display, keycode, 0, 0);
		if(keysym == NoSymbol) {
			return 0;
		}

		return KeySymToUcs4(keysym);
	}

}
