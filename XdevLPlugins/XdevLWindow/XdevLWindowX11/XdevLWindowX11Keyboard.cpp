#include "XdevLWindowX11Keyboard.h"


#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <map>

extern "C" {
#include "imKStoUCS.h"
}


namespace xdl {


	std::map<KeySym, XdevLButtonId> X11KeySymToXdevLButtonIdMap = {
		{ XK_a, KEY_A},
		{ XK_b, KEY_B},
		{ XK_c, KEY_C},
		{ XK_d, KEY_D},
		{ XK_e, KEY_E},
		{ XK_f, KEY_F},
		{ XK_g, KEY_G},
		{ XK_h, KEY_H},
		{ XK_i, KEY_I},
		{ XK_j, KEY_J},
		{ XK_k, KEY_K},
		{ XK_l, KEY_L},
		{ XK_m, KEY_M},
		{ XK_n, KEY_N},
		{ XK_o, KEY_O},
		{ XK_p, KEY_P},
		{ XK_q, KEY_Q},
		{ XK_r, KEY_R},
		{ XK_s, KEY_S},
		{ XK_t, KEY_T},
		{ XK_u, KEY_U},
		{ XK_v, KEY_V},
		{ XK_w, KEY_W},
		{ XK_x, KEY_X},
		{ XK_y, KEY_Y},
		{ XK_z, KEY_Z},

		{ XK_0, KEY_0},
		{ XK_1, KEY_1},
		{ XK_2, KEY_2},
		{ XK_3, KEY_3},
		{ XK_4, KEY_4},
		{ XK_5, KEY_5},
		{ XK_6, KEY_6},
		{ XK_7, KEY_7},
		{ XK_8, KEY_8},
		{ XK_9, KEY_9},


		{ XK_F1, KEY_F1},
		{ XK_F2, KEY_F2},
		{ XK_F3, KEY_F3},
		{ XK_F4, KEY_F4},
		{ XK_F5, KEY_F5},
		{ XK_F6, KEY_F6},
		{ XK_F7, KEY_F7},
		{ XK_F8, KEY_F8},
		{ XK_F9, KEY_F9},
		{ XK_F10, KEY_F10},
		{ XK_F11, KEY_F11},
		{ XK_F12, KEY_F12},
		{ XK_F13, KEY_F13},
		{ XK_F14, KEY_F14},
		{ XK_F15, KEY_F15},
		{ XK_F16, KEY_F16},
		{ XK_F17, KEY_F17},
		{ XK_F18, KEY_F18},
		{ XK_F19, KEY_F19},
		{ XK_F20, KEY_F20},
		{ XK_F21, KEY_F21},
		{ XK_F22, KEY_F22},
		{ XK_F23, KEY_F23},
		{ XK_F24, KEY_F24},

		{ XK_Escape, KEY_ESCAPE},
		{ XK_Home, KEY_HOME },
		{ XK_End, KEY_END },
		{ XK_Insert, KEY_INSERT },
		{ XK_Delete, KEY_DELETE },
		{ XK_Next, KEY_PAGEDOWN},
		{ XK_Prior, KEY_PAGEUP },

		{ XK_space, KEY_SPACE},
		{ XK_BackSpace, KEY_BACKSPACE},
		{ XK_Return, KEY_ENTER },
		{ XK_Tab, KEY_TAB },
		{ XK_Caps_Lock, KEY_CAPSLOCK},
		{ XK_Super_R, KEY_RGUI },
		{ XK_Super_L, KEY_LGUI },
		{ XK_plus, KEY_PLUS },
		{ XK_minus, KEY_MINUS },
		{ XK_backslash, KEY_BACKSLASH },
		{ XK_slash, KEY_SLASH },
		{ XK_period, KEY_PERIOD },
		{ XK_comma, KEY_COMMA },
		{ XK_colon, KEY_COLON },
		{ XK_semicolon, KEY_SEMICOLON },

		{ XK_bracketleft, KEY_LEFTBRACKET },
		{ XK_bracketright, KEY_RIGHTBRACKET },
		{ XK_grave, KEY_GRAVE },
		{ XK_underscore, KEY_UNDERSCORE},


		{ XK_Up, KEY_UP },
		{ XK_Down, KEY_DOWN },
		{ XK_Left, KEY_LEFT },
		{ XK_Right, KEY_RIGHT },

		{ XK_Alt_L, KEY_LALT },
		{ XK_Control_L, KEY_LCTRL },
		{ XK_Shift_L, KEY_LSHIFT },
		{ XK_Alt_R, KEY_RALT },
		{ XK_Control_R, KEY_RCTRL },
		{ XK_Shift_R, KEY_RSHIFT },


		{ XK_KP_0, KEY_KP_0 },
		{ XK_KP_1, KEY_KP_1 },
		{ XK_KP_2, KEY_KP_2 },
		{ XK_KP_3, KEY_KP_3 },
		{ XK_KP_4, KEY_KP_4 },
		{ XK_KP_5, KEY_KP_5 },
		{ XK_KP_6, KEY_KP_6 },
		{ XK_KP_7, KEY_KP_7 },
		{ XK_KP_8, KEY_KP_8 },
		{ XK_KP_9, KEY_KP_9 },


		{ XK_Num_Lock, KEY_NUMLOCK },
		{ XK_KP_Divide, KEY_KP_DIVIDE },
		{ XK_KP_Multiply, KEY_KP_MULTIPLY },
		{ XK_KP_Subtract, KEY_KP_MINUS },
		{ XK_KP_Add, KEY_KP_PLUS },
		{ XK_KP_Enter, KEY_KP_ENTER },
		{ XK_KP_Delete, KEY_KP_PERIOD },

		{ XK_Print, KEY_PRINTSCREEN },
		{ XK_Scroll_Lock, KEY_SCROLLLOCK },
		{ XK_Pause, KEY_PAUSE },

		{ XK_Caps_Lock, KEY_CAPSLOCK },
		

	};

	XdevLWindowX11Keyboard::XdevLWindowX11Keyboard(Display* display, XdevLCoreMediator* core) :
		m_display(display),
		m_core(core) {
	}

	XdevLWindowX11Keyboard::~XdevLWindowX11Keyboard() {
	}

	void XdevLWindowX11Keyboard::sendKeyboardEvent(xdl_uint64 state, KeyCode keycode, xdl_uint windowID) {
		printf("***************: X11: %d\n", keycode);
		XdevLButtonId code = x11KeyCodeToXdevLButtonId(keycode);
		if(keycode == KEY_UNKNOWN) {
			// TODO Do we have to inform someone?
			return;
		}

		xdl_int mod =0;
		xdl_uint16 modstate = 0;
		xdl_uint8 repeat = 0;

		if(state == ButtonPressed.getHashCode()) {
			switch(code) {
				case KEY_NUMLOCK:
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
				default:
					break;
			}
		} else if( state == ButtonReleased.getHashCode()) {
			switch(code) {
				case KEY_NUMLOCK:
				case KEY_CAPSLOCK:
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
				default:
					break;
			}
			modstate = mod;
		}

		XdevLEvent ev;
		ev.common.timestamp 	= m_core->getTimer().getTime64();
		ev.type 				= state;							// Is is BUTTON_PRESSED or BUTTON_RELEASED

		ev.window.windowid		= windowID;
		ev.key.repeat 			= repeat;							// Is repeat on or off?
		ev.key.keycode			= code;								// Which key button state has changed.
		ev.key.mod 				= modstate;							// Save the modifier keys.

		m_core->fireEvent(ev);
	}

	XdevLButtonId XdevLWindowX11Keyboard::x11KeyCodeToXdevLButtonId(KeyCode keycode) {

		// Convert the Keycode into Symbol expression.
		KeySym keysym = XkbKeycodeToKeysym(m_display, keycode, 0, 0);
		if(keysym == NoSymbol) {
			// No Symbol expression for that keycode found.
			return KEY_UNKNOWN;
		}

		auto it = X11KeySymToXdevLButtonIdMap.find(keysym);
		if(it != X11KeySymToXdevLButtonIdMap.end()) {
			XdevLButtonId tmp = X11KeySymToXdevLButtonIdMap[keysym];
			int b = 0;
		}


		return it->second;
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
