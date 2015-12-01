/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2014 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include <XdevLKeyboard/XdevLScanCode.h>

/* Linux virtual key code to XDEVL_Keycode mapping table
   Sources:
   - Linux kernel source input.h
*/
/* *INDENT-OFF* */
namespace xdl {


	static const int linux_scancode_table[] = {
		/*  0 */    XDEVL_SCANCODE_UNKNOWN,
		/*  1 */    XDEVL_SCANCODE_ESCAPE,
		/*  2 */    XDEVL_SCANCODE_1,
		/*  3 */    XDEVL_SCANCODE_2,
		/*  4 */    XDEVL_SCANCODE_3,
		/*  5 */    XDEVL_SCANCODE_4,
		/*  6 */    XDEVL_SCANCODE_5,
		/*  7 */    XDEVL_SCANCODE_6,
		/*  8 */    XDEVL_SCANCODE_7,
		/*  9 */    XDEVL_SCANCODE_8,
		/*  10 */    XDEVL_SCANCODE_9,
		/*  11 */    XDEVL_SCANCODE_0,
		/*  12 */    XDEVL_SCANCODE_MINUS,
		/*  13 */    XDEVL_SCANCODE_EQUALS,
		/*  14 */    XDEVL_SCANCODE_BACKSPACE,
		/*  15 */    XDEVL_SCANCODE_TAB,
		/*  16 */    XDEVL_SCANCODE_Q,
		/*  17 */    XDEVL_SCANCODE_W,
		/*  18 */    XDEVL_SCANCODE_E,
		/*  19 */    XDEVL_SCANCODE_R,
		/*  20 */    XDEVL_SCANCODE_T,
		/*  21 */    XDEVL_SCANCODE_Y,
		/*  22 */    XDEVL_SCANCODE_U,
		/*  23 */    XDEVL_SCANCODE_I,
		/*  24 */    XDEVL_SCANCODE_O,
		/*  25 */    XDEVL_SCANCODE_P,
		/*  26 */    XDEVL_SCANCODE_LEFTBRACKET,
		/*  27 */    XDEVL_SCANCODE_RIGHTBRACKET,
		/*  28 */    XDEVL_SCANCODE_RETURN,
		/*  29 */    XDEVL_SCANCODE_LCTRL,
		/*  30 */    XDEVL_SCANCODE_A,
		/*  31 */    XDEVL_SCANCODE_S,
		/*  32 */    XDEVL_SCANCODE_D,
		/*  33 */    XDEVL_SCANCODE_F,
		/*  34 */    XDEVL_SCANCODE_G,
		/*  35 */    XDEVL_SCANCODE_H,
		/*  36 */    XDEVL_SCANCODE_J,
		/*  37 */    XDEVL_SCANCODE_K,
		/*  38 */    XDEVL_SCANCODE_L,
		/*  39 */    XDEVL_SCANCODE_SEMICOLON,
		/*  40 */    XDEVL_SCANCODE_APOSTROPHE,
		/*  41 */    XDEVL_SCANCODE_GRAVE,
		/*  42 */    XDEVL_SCANCODE_LSHIFT,
		/*  43 */    XDEVL_SCANCODE_BACKSLASH,
		/*  44 */    XDEVL_SCANCODE_Z,
		/*  45 */    XDEVL_SCANCODE_X,
		/*  46 */    XDEVL_SCANCODE_C,
		/*  47 */    XDEVL_SCANCODE_V,
		/*  48 */    XDEVL_SCANCODE_B,
		/*  49 */    XDEVL_SCANCODE_N,
		/*  50 */    XDEVL_SCANCODE_M,
		/*  51 */    XDEVL_SCANCODE_COMMA,
		/*  52 */    XDEVL_SCANCODE_PERIOD,
		/*  53 */    XDEVL_SCANCODE_SLASH,
		/*  54 */    XDEVL_SCANCODE_RSHIFT,
		/*  55 */    XDEVL_SCANCODE_KP_MULTIPLY,
		/*  56 */    XDEVL_SCANCODE_LALT,
		/*  57 */    XDEVL_SCANCODE_SPACE,
		/*  58 */    XDEVL_SCANCODE_CAPSLOCK,
		/*  59 */    XDEVL_SCANCODE_F1,
		/*  60 */    XDEVL_SCANCODE_F2,
		/*  61 */    XDEVL_SCANCODE_F3,
		/*  62 */    XDEVL_SCANCODE_F4,
		/*  63 */    XDEVL_SCANCODE_F5,
		/*  64 */    XDEVL_SCANCODE_F6,
		/*  65 */    XDEVL_SCANCODE_F7,
		/*  66 */    XDEVL_SCANCODE_F8,
		/*  67 */    XDEVL_SCANCODE_F9,
		/*  68 */    XDEVL_SCANCODE_F10,
		/*  69 */    XDEVL_SCANCODE_NUMLOCKCLEAR,
		/*  70 */    XDEVL_SCANCODE_SCROLLLOCK,
		/*  71 */    XDEVL_SCANCODE_KP_7,
		/*  72 */    XDEVL_SCANCODE_KP_8,
		/*  73 */    XDEVL_SCANCODE_KP_9,
		/*  74 */    XDEVL_SCANCODE_KP_MINUS,
		/*  75 */    XDEVL_SCANCODE_KP_4,
		/*  76 */    XDEVL_SCANCODE_KP_5,
		/*  77 */    XDEVL_SCANCODE_KP_6,
		/*  78 */    XDEVL_SCANCODE_KP_PLUS,
		/*  79 */    XDEVL_SCANCODE_KP_1,
		/*  80 */    XDEVL_SCANCODE_KP_2,
		/*  81 */    XDEVL_SCANCODE_KP_3,
		/*  82 */    XDEVL_SCANCODE_KP_0,
		/*  83 */    XDEVL_SCANCODE_KP_PERIOD,
		0,
		/*  85 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_ZENKAKUHANKAKU */
		/*  86 */    XDEVL_SCANCODE_NONUSBACKSLASH, /* KEY_102ND */
		/*  87 */    XDEVL_SCANCODE_F11,
		/*  88 */    XDEVL_SCANCODE_F12,
		/*  89 */    XDEVL_SCANCODE_INTERNATIONAL1, /* KEY_RO */
		/*  90 */    XDEVL_SCANCODE_LANG3, /* KEY_KATAKANA */
		/*  91 */    XDEVL_SCANCODE_LANG4, /* KEY_HIRAGANA */
		/*  92 */    XDEVL_SCANCODE_INTERNATIONAL4, /* KEY_HENKAN */
		/*  93 */    XDEVL_SCANCODE_INTERNATIONAL2, /* KEY_KATAKANAHIRAGANA */
		/*  94 */    XDEVL_SCANCODE_INTERNATIONAL5, /* KEY_MUHENKAN */
		/*  95 */    XDEVL_SCANCODE_INTERNATIONAL5, /* KEY_KPJPCOMMA */
		/*  96 */    XDEVL_SCANCODE_KP_ENTER,
		/*  97 */    XDEVL_SCANCODE_RCTRL,
		/*  98 */    XDEVL_SCANCODE_KP_DIVIDE,
		/*  99 */    XDEVL_SCANCODE_SYSREQ,
		/*  100 */    XDEVL_SCANCODE_RALT,
		/*  101 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_LINEFEED */
		/*  102 */    XDEVL_SCANCODE_HOME,
		/*  103 */    XDEVL_SCANCODE_UP,
		/*  104 */    XDEVL_SCANCODE_PAGEUP,
		/*  105 */    XDEVL_SCANCODE_LEFT,
		/*  106 */    XDEVL_SCANCODE_RIGHT,
		/*  107 */    XDEVL_SCANCODE_END,
		/*  108 */    XDEVL_SCANCODE_DOWN,
		/*  109 */    XDEVL_SCANCODE_PAGEDOWN,
		/*  110 */    XDEVL_SCANCODE_INSERT,
		/*  111 */    XDEVL_SCANCODE_DELETE,
		/*  112 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_MACRO */
		/*  113 */    XDEVL_SCANCODE_MUTE,
		/*  114 */    XDEVL_SCANCODE_VOLUMEDOWN,
		/*  115 */    XDEVL_SCANCODE_VOLUMEUP,
		/*  116 */    XDEVL_SCANCODE_POWER,
		/*  117 */    XDEVL_SCANCODE_KP_EQUALS,
		/*  118 */    XDEVL_SCANCODE_KP_PLUSMINUS,
		/*  119 */    XDEVL_SCANCODE_PAUSE,
		0,
		/*  121 */    XDEVL_SCANCODE_KP_COMMA,
		/*  122 */    XDEVL_SCANCODE_LANG1, /* KEY_HANGUEL */
		/*  123 */    XDEVL_SCANCODE_LANG2, /* KEY_HANJA */
		/*  124 */    XDEVL_SCANCODE_INTERNATIONAL3, /* KEY_YEN */
		/*  125 */    XDEVL_SCANCODE_LGUI,
		/*  126 */    XDEVL_SCANCODE_RGUI,
		/*  127 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_COMPOSE */
		/*  128 */    XDEVL_SCANCODE_STOP,
		/*  129 */    XDEVL_SCANCODE_AGAIN,
		/*  130 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_PROPS */
		/*  131 */    XDEVL_SCANCODE_UNDO,
		/*  132 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_FRONT */
		/*  133 */    XDEVL_SCANCODE_COPY,
		/*  134 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_OPEN */
		/*  135 */    XDEVL_SCANCODE_PASTE,
		/*  136 */    XDEVL_SCANCODE_FIND,
		/*  137 */    XDEVL_SCANCODE_CUT,
		/*  138 */    XDEVL_SCANCODE_HELP,
		/*  139 */    XDEVL_SCANCODE_MENU,
		/*  140 */    XDEVL_SCANCODE_CALCULATOR,
		/*  141 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_SETUP */
		/*  142 */    XDEVL_SCANCODE_SLEEP,
		/*  143 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_WAKEUP */
		/*  144 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_FILE */
		/*  145 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_SENDFILE */
		/*  146 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_DELETEFILE */
		/*  147 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_XFER */
		/*  148 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_PROG1 */
		/*  149 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_PROG2 */
		/*  150 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_WWW */
		/*  151 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_MSDOS */
		/*  152 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_COFFEE */
		/*  153 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_DIRECTION */
		/*  154 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_CYCLEWINDOWS */
		/*  155 */    XDEVL_SCANCODE_MAIL,
		/*  156 */    XDEVL_SCANCODE_AC_BOOKMARKS,
		/*  157 */    XDEVL_SCANCODE_COMPUTER,
		/*  158 */    XDEVL_SCANCODE_AC_BACK,
		/*  159 */    XDEVL_SCANCODE_AC_FORWARD,
		/*  160 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_CLOSECD */
		/*  161 */    XDEVL_SCANCODE_EJECT, /* KEY_EJECTCD */
		/*  162 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_EJECTCLOSECD */
		/*  163 */    XDEVL_SCANCODE_AUDIONEXT, /* KEY_NEXTSONG */
		/*  164 */    XDEVL_SCANCODE_AUDIOPLAY, /* KEY_PLAYPAUSE */
		/*  165 */    XDEVL_SCANCODE_AUDIOPREV, /* KEY_PREVIOUSSONG */
		/*  166 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_STOPCD */
		/*  167 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_RECORD */
		/*  168 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_REWIND */
		/*  169 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_PHONE */
		/*  170 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_ISO */
		/*  171 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_CONFIG */
		/*  172 */    XDEVL_SCANCODE_AC_HOME,
		/*  173 */    XDEVL_SCANCODE_AC_REFRESH,
		/*  174 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_EXIT */
		/*  175 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_MOVE */
		/*  176 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_EDIT */
		/*  177 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_SCROLLUP */
		/*  178 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_SCROLLDOWN */
		/*  179 */    XDEVL_SCANCODE_KP_LEFTPAREN,
		/*  180 */    XDEVL_SCANCODE_KP_RIGHTPAREN,
		/*  181 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_NEW */
		/*  182 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_REDO */
		/*  183 */    XDEVL_SCANCODE_F13,
		/*  184 */    XDEVL_SCANCODE_F14,
		/*  185 */    XDEVL_SCANCODE_F15,
		/*  186 */    XDEVL_SCANCODE_F16,
		/*  187 */    XDEVL_SCANCODE_F17,
		/*  188 */    XDEVL_SCANCODE_F18,
		/*  189 */    XDEVL_SCANCODE_F19,
		/*  190 */    XDEVL_SCANCODE_F20,
		/*  191 */    XDEVL_SCANCODE_F21,
		/*  192 */    XDEVL_SCANCODE_F22,
		/*  193 */    XDEVL_SCANCODE_F23,
		/*  194 */    XDEVL_SCANCODE_F24,
		0, 0, 0, 0,
		/*  200 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_PLAYCD */
		/*  201 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_PAUSECD */
		/*  202 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_PROG3 */
		/*  203 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_PROG4 */
		0,
		/*  205 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_SUSPEND */
		/*  206 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_CLOSE */
		/*  207 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_PLAY */
		/*  208 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_FASTFORWARD */
		/*  209 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_BASSBOOST */
		/*  210 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_PRINT */
		/*  211 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_HP */
		/*  212 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_CAMERA */
		/*  213 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_SOUND */
		/*  214 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_QUESTION */
		/*  215 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_EMAIL */
		/*  216 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_CHAT */
		/*  217 */    XDEVL_SCANCODE_AC_SEARCH,
		/*  218 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_CONNECT */
		/*  219 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_FINANCE */
		/*  220 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_SPORT */
		/*  221 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_SHOP */
		/*  222 */    XDEVL_SCANCODE_ALTERASE,
		/*  223 */    XDEVL_SCANCODE_CANCEL,
		/*  224 */    XDEVL_SCANCODE_BRIGHTNESSDOWN,
		/*  225 */    XDEVL_SCANCODE_BRIGHTNESSUP,
		/*  226 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_MEDIA */
		/*  227 */    XDEVL_SCANCODE_DISPLAYSWITCH, /* KEY_SWITCHVIDEOMODE */
		/*  228 */    XDEVL_SCANCODE_KBDILLUMTOGGLE,
		/*  229 */    XDEVL_SCANCODE_KBDILLUMDOWN,
		/*  230 */    XDEVL_SCANCODE_KBDILLUMUP,
		/*  231 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_SEND */
		/*  232 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_REPLY */
		/*  233 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_FORWARDMAIL */
		/*  234 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_SAVE */
		/*  235 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_DOCUMENTS */
		/*  236 */    XDEVL_SCANCODE_UNKNOWN, /* KEY_BATTERY */
	};
	/* *INDENT-ON* */
}
