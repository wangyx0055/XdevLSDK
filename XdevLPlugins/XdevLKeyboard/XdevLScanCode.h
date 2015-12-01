/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2014 Cengiz Terzibas

	This library is free software; you can redistribute it and/or modify it under the
	terms of the GNU Lesser General Public License as published by the Free Software
	Foundation; either version 2.1 of the License, or (at your option) any later version.
	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License along with
	this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
	Suite 330, Boston, MA 02111-1307 USA

	I would appreciate if you report all bugs to: cengiz@terzibas.de
*/

#ifndef XDEVL_SCAN_CODE_H
#define XDEVL_SCAN_CODE_H

namespace xdl {

	/**
		@enum XdevLScanCode
	*/
  enum XdevLScanCode
  {
      XDEVL_SCANCODE_UNKNOWN = 0,

      XDEVL_SCANCODE_A = 4,
      XDEVL_SCANCODE_B = 5,
      XDEVL_SCANCODE_C = 6,
      XDEVL_SCANCODE_D = 7,
      XDEVL_SCANCODE_E = 8,
      XDEVL_SCANCODE_F = 9,
      XDEVL_SCANCODE_G = 10,
      XDEVL_SCANCODE_H = 11,
      XDEVL_SCANCODE_I = 12,
      XDEVL_SCANCODE_J = 13,
      XDEVL_SCANCODE_K = 14,
      XDEVL_SCANCODE_L = 15,
      XDEVL_SCANCODE_M = 16,
      XDEVL_SCANCODE_N = 17,
      XDEVL_SCANCODE_O = 18,
      XDEVL_SCANCODE_P = 19,
      XDEVL_SCANCODE_Q = 20,
      XDEVL_SCANCODE_R = 21,
      XDEVL_SCANCODE_S = 22,
      XDEVL_SCANCODE_T = 23,
      XDEVL_SCANCODE_U = 24,
      XDEVL_SCANCODE_V = 25,
      XDEVL_SCANCODE_W = 26,
      XDEVL_SCANCODE_X = 27,
      XDEVL_SCANCODE_Y = 28,
      XDEVL_SCANCODE_Z = 29,

      XDEVL_SCANCODE_1 = 30,
      XDEVL_SCANCODE_2 = 31,
      XDEVL_SCANCODE_3 = 32,
      XDEVL_SCANCODE_4 = 33,
      XDEVL_SCANCODE_5 = 34,
      XDEVL_SCANCODE_6 = 35,
      XDEVL_SCANCODE_7 = 36,
      XDEVL_SCANCODE_8 = 37,
      XDEVL_SCANCODE_9 = 38,
      XDEVL_SCANCODE_0 = 39,

      XDEVL_SCANCODE_RETURN = 40,
      XDEVL_SCANCODE_ESCAPE = 41,
      XDEVL_SCANCODE_BACKSPACE = 42,
      XDEVL_SCANCODE_TAB = 43,
      XDEVL_SCANCODE_SPACE = 44,

      XDEVL_SCANCODE_MINUS = 45,
      XDEVL_SCANCODE_EQUALS = 46,
      XDEVL_SCANCODE_LEFTBRACKET = 47,
      XDEVL_SCANCODE_RIGHTBRACKET = 48,
      XDEVL_SCANCODE_BACKSLASH = 49, /**< Located at the lower left of the return
                                    *   key on ISO keyboards and at the right end
                                    *   of the QWERTY row on ANSI keyboards.
                                    *   Produces REVERSE SOLIDUS (backslash) and
                                    *   VERTICAL LINE in a US layout, REVERSE
                                    *   SOLIDUS and VERTICAL LINE in a UK Mac
                                    *   layout, NUMBER SIGN and TILDE in a UK
                                    *   Windows layout, DOLLAR SIGN and POUND SIGN
                                    *   in a Swiss German layout, NUMBER SIGN and
                                    *   APOSTROPHE in a German layout, GRAVE
                                    *   ACCENT and POUND SIGN in a French Mac
                                    *   layout, and ASTERISK and MICRO SIGN in a
                                    *   French Windows layout.
                                    */
      XDEVL_SCANCODE_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                    *   instead of 49 for the same key, but all
                                    *   OSes I've seen treat the two codes
                                    *   identically. So, as an implementor, unless
                                    *   your keyboard generates both of those
                                    *   codes and your OS treats them differently,
                                    *   you should generate XDEVL_SCANCODE_BACKSLASH
                                    *   instead of this code. As a user, you
                                    *   should not rely on this code because SDL
                                    *   will never generate it with most (all?)
                                    *   keyboards.
                                    */
      XDEVL_SCANCODE_SEMICOLON = 51,
      XDEVL_SCANCODE_APOSTROPHE = 52,
      XDEVL_SCANCODE_GRAVE = 53, /**< Located in the top left corner (on both ANSI
                                *   and ISO keyboards). Produces GRAVE ACCENT and
                                *   TILDE in a US Windows layout and in US and UK
                                *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                                *   and NOT SIGN in a UK Windows layout, SECTION
                                *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                                *   layouts on ISO keyboards, SECTION SIGN and
                                *   DEGREE SIGN in a Swiss German layout (Mac:
                                *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                                *   DEGREE SIGN in a German layout (Mac: only on
                                *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                                *   French Windows layout, COMMERCIAL AT and
                                *   NUMBER SIGN in a French Mac layout on ISO
                                *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                                *   SIGN in a Swiss German, German, or French Mac
                                *   layout on ANSI keyboards.
                                */
      XDEVL_SCANCODE_COMMA = 54,
      XDEVL_SCANCODE_PERIOD = 55,
      XDEVL_SCANCODE_SLASH = 56,

      XDEVL_SCANCODE_CAPSLOCK = 57,

      XDEVL_SCANCODE_F1 = 58,
      XDEVL_SCANCODE_F2 = 59,
      XDEVL_SCANCODE_F3 = 60,
      XDEVL_SCANCODE_F4 = 61,
      XDEVL_SCANCODE_F5 = 62,
      XDEVL_SCANCODE_F6 = 63,
      XDEVL_SCANCODE_F7 = 64,
      XDEVL_SCANCODE_F8 = 65,
      XDEVL_SCANCODE_F9 = 66,
      XDEVL_SCANCODE_F10 = 67,
      XDEVL_SCANCODE_F11 = 68,
      XDEVL_SCANCODE_F12 = 69,

      XDEVL_SCANCODE_PRINTSCREEN = 70,
      XDEVL_SCANCODE_SCROLLLOCK = 71,
      XDEVL_SCANCODE_PAUSE = 72,
      XDEVL_SCANCODE_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                     does send code 73, not 117) */
      XDEVL_SCANCODE_HOME = 74,
      XDEVL_SCANCODE_PAGEUP = 75,
      XDEVL_SCANCODE_DELETE = 76,
      XDEVL_SCANCODE_END = 77,
      XDEVL_SCANCODE_PAGEDOWN = 78,
      XDEVL_SCANCODE_RIGHT = 79,
      XDEVL_SCANCODE_LEFT = 80,
      XDEVL_SCANCODE_DOWN = 81,
      XDEVL_SCANCODE_UP = 82,

      XDEVL_SCANCODE_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                       */
      XDEVL_SCANCODE_KP_DIVIDE = 84,
      XDEVL_SCANCODE_KP_MULTIPLY = 85,
      XDEVL_SCANCODE_KP_MINUS = 86,
      XDEVL_SCANCODE_KP_PLUS = 87,
      XDEVL_SCANCODE_KP_ENTER = 88,
      XDEVL_SCANCODE_KP_1 = 89,
      XDEVL_SCANCODE_KP_2 = 90,
      XDEVL_SCANCODE_KP_3 = 91,
      XDEVL_SCANCODE_KP_4 = 92,
      XDEVL_SCANCODE_KP_5 = 93,
      XDEVL_SCANCODE_KP_6 = 94,
      XDEVL_SCANCODE_KP_7 = 95,
      XDEVL_SCANCODE_KP_8 = 96,
      XDEVL_SCANCODE_KP_9 = 97,
      XDEVL_SCANCODE_KP_0 = 98,
      XDEVL_SCANCODE_KP_PERIOD = 99,

      XDEVL_SCANCODE_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                          *   keyboards have over ANSI ones,
                                          *   located between left shift and Y.
                                          *   Produces GRAVE ACCENT and TILDE in a
                                          *   US or UK Mac layout, REVERSE SOLIDUS
                                          *   (backslash) and VERTICAL LINE in a
                                          *   US or UK Windows layout, and
                                          *   LESS-THAN SIGN and GREATER-THAN SIGN
                                          *   in a Swiss German, German, or French
                                          *   layout. */
      XDEVL_SCANCODE_APPLICATION = 101, /**< windows contextual menu, compose */
      XDEVL_SCANCODE_POWER = 102, /**< The USB document says this is a status flag,
                                 *   not a physical key - but some Mac keyboards
                                 *   do have a power key. */
      XDEVL_SCANCODE_KP_EQUALS = 103,
      XDEVL_SCANCODE_F13 = 104,
      XDEVL_SCANCODE_F14 = 105,
      XDEVL_SCANCODE_F15 = 106,
      XDEVL_SCANCODE_F16 = 107,
      XDEVL_SCANCODE_F17 = 108,
      XDEVL_SCANCODE_F18 = 109,
      XDEVL_SCANCODE_F19 = 110,
      XDEVL_SCANCODE_F20 = 111,
      XDEVL_SCANCODE_F21 = 112,
      XDEVL_SCANCODE_F22 = 113,
      XDEVL_SCANCODE_F23 = 114,
      XDEVL_SCANCODE_F24 = 115,
      XDEVL_SCANCODE_EXECUTE = 116,
      XDEVL_SCANCODE_HELP = 117,
      XDEVL_SCANCODE_MENU = 118,
      XDEVL_SCANCODE_SELECT = 119,
      XDEVL_SCANCODE_STOP = 120,
      XDEVL_SCANCODE_AGAIN = 121,   /**< redo */
      XDEVL_SCANCODE_UNDO = 122,
      XDEVL_SCANCODE_CUT = 123,
      XDEVL_SCANCODE_COPY = 124,
      XDEVL_SCANCODE_PASTE = 125,
      XDEVL_SCANCODE_FIND = 126,
      XDEVL_SCANCODE_MUTE = 127,
      XDEVL_SCANCODE_VOLUMEUP = 128,
      XDEVL_SCANCODE_VOLUMEDOWN = 129,
  /* not sure whether there's a reason to enable these */
  /*     XDEVL_SCANCODE_LOCKINGCAPSLOCK = 130,  */
  /*     XDEVL_SCANCODE_LOCKINGNUMLOCK = 131, */
  /*     XDEVL_SCANCODE_LOCKINGSCROLLLOCK = 132, */
      XDEVL_SCANCODE_KP_COMMA = 133,
      XDEVL_SCANCODE_KP_EQUALSAS400 = 134,

      XDEVL_SCANCODE_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                              footnotes in USB doc */
      XDEVL_SCANCODE_INTERNATIONAL2 = 136,
      XDEVL_SCANCODE_INTERNATIONAL3 = 137, /**< Yen */
      XDEVL_SCANCODE_INTERNATIONAL4 = 138,
      XDEVL_SCANCODE_INTERNATIONAL5 = 139,
      XDEVL_SCANCODE_INTERNATIONAL6 = 140,
      XDEVL_SCANCODE_INTERNATIONAL7 = 141,
      XDEVL_SCANCODE_INTERNATIONAL8 = 142,
      XDEVL_SCANCODE_INTERNATIONAL9 = 143,
      XDEVL_SCANCODE_LANG1 = 144, /**< Hangul/English toggle */
      XDEVL_SCANCODE_LANG2 = 145, /**< Hanja conversion */
      XDEVL_SCANCODE_LANG3 = 146, /**< Katakana */
      XDEVL_SCANCODE_LANG4 = 147, /**< Hiragana */
      XDEVL_SCANCODE_LANG5 = 148, /**< Zenkaku/Hankaku */
      XDEVL_SCANCODE_LANG6 = 149, /**< reserved */
      XDEVL_SCANCODE_LANG7 = 150, /**< reserved */
      XDEVL_SCANCODE_LANG8 = 151, /**< reserved */
      XDEVL_SCANCODE_LANG9 = 152, /**< reserved */

      XDEVL_SCANCODE_ALTERASE = 153, /**< Erase-Eaze */
      XDEVL_SCANCODE_SYSREQ = 154,
      XDEVL_SCANCODE_CANCEL = 155,
      XDEVL_SCANCODE_CLEAR = 156,
      XDEVL_SCANCODE_PRIOR = 157,
      XDEVL_SCANCODE_RETURN2 = 158,
      XDEVL_SCANCODE_SEPARATOR = 159,
      XDEVL_SCANCODE_OUT = 160,
      XDEVL_SCANCODE_OPER = 161,
      XDEVL_SCANCODE_CLEARAGAIN = 162,
      XDEVL_SCANCODE_CRSEL = 163,
      XDEVL_SCANCODE_EXSEL = 164,

      XDEVL_SCANCODE_KP_00 = 176,
      XDEVL_SCANCODE_KP_000 = 177,
      XDEVL_SCANCODE_THOUSANDSSEPARATOR = 178,
      XDEVL_SCANCODE_DECIMALSEPARATOR = 179,
      XDEVL_SCANCODE_CURRENCYUNIT = 180,
      XDEVL_SCANCODE_CURRENCYSUBUNIT = 181,
      XDEVL_SCANCODE_KP_LEFTPAREN = 182,
      XDEVL_SCANCODE_KP_RIGHTPAREN = 183,
      XDEVL_SCANCODE_KP_LEFTBRACE = 184,
      XDEVL_SCANCODE_KP_RIGHTBRACE = 185,
      XDEVL_SCANCODE_KP_TAB = 186,
      XDEVL_SCANCODE_KP_BACKSPACE = 187,
      XDEVL_SCANCODE_KP_A = 188,
      XDEVL_SCANCODE_KP_B = 189,
      XDEVL_SCANCODE_KP_C = 190,
      XDEVL_SCANCODE_KP_D = 191,
      XDEVL_SCANCODE_KP_E = 192,
      XDEVL_SCANCODE_KP_F = 193,
      XDEVL_SCANCODE_KP_XOR = 194,
      XDEVL_SCANCODE_KP_POWER = 195,
      XDEVL_SCANCODE_KP_PERCENT = 196,
      XDEVL_SCANCODE_KP_LESS = 197,
      XDEVL_SCANCODE_KP_GREATER = 198,
      XDEVL_SCANCODE_KP_AMPERSAND = 199,
      XDEVL_SCANCODE_KP_DBLAMPERSAND = 200,
      XDEVL_SCANCODE_KP_VERTICALBAR = 201,
      XDEVL_SCANCODE_KP_DBLVERTICALBAR = 202,
      XDEVL_SCANCODE_KP_COLON = 203,
      XDEVL_SCANCODE_KP_HASH = 204,
      XDEVL_SCANCODE_KP_SPACE = 205,
      XDEVL_SCANCODE_KP_AT = 206,
      XDEVL_SCANCODE_KP_EXCLAM = 207,
      XDEVL_SCANCODE_KP_MEMSTORE = 208,
      XDEVL_SCANCODE_KP_MEMRECALL = 209,
      XDEVL_SCANCODE_KP_MEMCLEAR = 210,
      XDEVL_SCANCODE_KP_MEMADD = 211,
      XDEVL_SCANCODE_KP_MEMSUBTRACT = 212,
      XDEVL_SCANCODE_KP_MEMMULTIPLY = 213,
      XDEVL_SCANCODE_KP_MEMDIVIDE = 214,
      XDEVL_SCANCODE_KP_PLUSMINUS = 215,
      XDEVL_SCANCODE_KP_CLEAR = 216,
      XDEVL_SCANCODE_KP_CLEARENTRY = 217,
      XDEVL_SCANCODE_KP_BINARY = 218,
      XDEVL_SCANCODE_KP_OCTAL = 219,
      XDEVL_SCANCODE_KP_DECIMAL = 220,
      XDEVL_SCANCODE_KP_HEXADECIMAL = 221,

      XDEVL_SCANCODE_LCTRL = 224,
      XDEVL_SCANCODE_LSHIFT = 225,
      XDEVL_SCANCODE_LALT = 226, /**< alt, option */
      XDEVL_SCANCODE_LGUI = 227, /**< windows, command (apple), meta */
      XDEVL_SCANCODE_RCTRL = 228,
      XDEVL_SCANCODE_RSHIFT = 229,
      XDEVL_SCANCODE_RALT = 230, /**< alt gr, option */
      XDEVL_SCANCODE_RGUI = 231, /**< windows, command (apple), meta */

      XDEVL_SCANCODE_MODE = 257,    /**< I'm not sure if this is really not covered
                                   *   by any of the above, but since there's a
                                   *   special KMOD_MODE for it I'm adding it here
                                   */

      /* @} *//* Usage page 0x07 */

      /**
       *  \name Usage page 0x0C
       *
       *  These values are mapped from usage page 0x0C (USB consumer page).
       */
      /* @{ */

      XDEVL_SCANCODE_AUDIONEXT = 258,
      XDEVL_SCANCODE_AUDIOPREV = 259,
      XDEVL_SCANCODE_AUDIOSTOP = 260,
      XDEVL_SCANCODE_AUDIOPLAY = 261,
      XDEVL_SCANCODE_AUDIOMUTE = 262,
      XDEVL_SCANCODE_MEDIASELECT = 263,
      XDEVL_SCANCODE_WWW = 264,
      XDEVL_SCANCODE_MAIL = 265,
      XDEVL_SCANCODE_CALCULATOR = 266,
      XDEVL_SCANCODE_COMPUTER = 267,
      XDEVL_SCANCODE_AC_SEARCH = 268,
      XDEVL_SCANCODE_AC_HOME = 269,
      XDEVL_SCANCODE_AC_BACK = 270,
      XDEVL_SCANCODE_AC_FORWARD = 271,
      XDEVL_SCANCODE_AC_STOP = 272,
      XDEVL_SCANCODE_AC_REFRESH = 273,
      XDEVL_SCANCODE_AC_BOOKMARKS = 274,

      XDEVL_SCANCODE_BRIGHTNESSDOWN = 275,
      XDEVL_SCANCODE_BRIGHTNESSUP = 276,
      XDEVL_SCANCODE_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                             switch, video mode switch */
      XDEVL_SCANCODE_KBDILLUMTOGGLE = 278,
      XDEVL_SCANCODE_KBDILLUMDOWN = 279,
      XDEVL_SCANCODE_KBDILLUMUP = 280,
      XDEVL_SCANCODE_EJECT = 281,
      XDEVL_SCANCODE_SLEEP = 282,

      XDEVL_SCANCODE_APP1 = 283,
      XDEVL_SCANCODE_APP2 = 284,

      XDEVL_NUM_SCANCODES = 512 /**< not a key, just marks the number of scancodes
                                   for array bounds */
  };

}

#endif
