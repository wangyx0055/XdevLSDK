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

/* Mac virtual key code to SDL scancode mapping table
   Sources:
   - Inside Macintosh: Text <http://developer.apple.com/documentation/mac/Text/Text-571.html>
   - Apple USB keyboard driver source <http://darwinsource.opendarwin.org/10.4.6.ppc/IOHIDFamily-172.8/IOHIDFamily/Cosmo_USB2ADB.c>
   - experimentation on various ADB and USB ISO keyboards and one ADB ANSI keyboard
*/
/* *INDENT-OFF* */
namespace xdl {


	static const int darwin_scancode_table[] = {
		/*   0 */   XDEVL_SCANCODE_A,
		/*   1 */   XDEVL_SCANCODE_S,
		/*   2 */   XDEVL_SCANCODE_D,
		/*   3 */   XDEVL_SCANCODE_F,
		/*   4 */   XDEVL_SCANCODE_H,
		/*   5 */   XDEVL_SCANCODE_G,
		/*   6 */   XDEVL_SCANCODE_Z,
		/*   7 */   XDEVL_SCANCODE_X,
		/*   8 */   XDEVL_SCANCODE_C,
		/*   9 */   XDEVL_SCANCODE_V,
		/*  10 */   XDEVL_SCANCODE_NONUSBACKSLASH, /* XDEVL_SCANCODE_NONUSBACKSLASH on ANSI and JIS keyboards (if this key would exist there), XDEVL_SCANCODE_GRAVE on ISO. (The USB keyboard driver actually translates these usage codes to different virtual key codes depending on whether the keyboard is ISO/ANSI/JIS. That's why you have to help it identify the keyboard type when you plug in a PC USB keyboard. It's a historical thing - ADB keyboards are wired this way.) */
		/*  11 */   XDEVL_SCANCODE_B,
		/*  12 */   XDEVL_SCANCODE_Q,
		/*  13 */   XDEVL_SCANCODE_W,
		/*  14 */   XDEVL_SCANCODE_E,
		/*  15 */   XDEVL_SCANCODE_R,
		/*  16 */   XDEVL_SCANCODE_Y,
		/*  17 */   XDEVL_SCANCODE_T,
		/*  18 */   XDEVL_SCANCODE_1,
		/*  19 */   XDEVL_SCANCODE_2,
		/*  20 */   XDEVL_SCANCODE_3,
		/*  21 */   XDEVL_SCANCODE_4,
		/*  22 */   XDEVL_SCANCODE_6,
		/*  23 */   XDEVL_SCANCODE_5,
		/*  24 */   XDEVL_SCANCODE_EQUALS,
		/*  25 */   XDEVL_SCANCODE_9,
		/*  26 */   XDEVL_SCANCODE_7,
		/*  27 */   XDEVL_SCANCODE_MINUS,
		/*  28 */   XDEVL_SCANCODE_8,
		/*  29 */   XDEVL_SCANCODE_0,
		/*  30 */   XDEVL_SCANCODE_RIGHTBRACKET,
		/*  31 */   XDEVL_SCANCODE_O,
		/*  32 */   XDEVL_SCANCODE_U,
		/*  33 */   XDEVL_SCANCODE_LEFTBRACKET,
		/*  34 */   XDEVL_SCANCODE_I,
		/*  35 */   XDEVL_SCANCODE_P,
		/*  36 */   XDEVL_SCANCODE_RETURN,
		/*  37 */   XDEVL_SCANCODE_L,
		/*  38 */   XDEVL_SCANCODE_J,
		/*  39 */   XDEVL_SCANCODE_APOSTROPHE,
		/*  40 */   XDEVL_SCANCODE_K,
		/*  41 */   XDEVL_SCANCODE_SEMICOLON,
		/*  42 */   XDEVL_SCANCODE_BACKSLASH,
		/*  43 */   XDEVL_SCANCODE_COMMA,
		/*  44 */   XDEVL_SCANCODE_SLASH,
		/*  45 */   XDEVL_SCANCODE_N,
		/*  46 */   XDEVL_SCANCODE_M,
		/*  47 */   XDEVL_SCANCODE_PERIOD,
		/*  48 */   XDEVL_SCANCODE_TAB,
		/*  49 */   XDEVL_SCANCODE_SPACE,
		/*  50 */   XDEVL_SCANCODE_GRAVE, /* XDEVL_SCANCODE_GRAVE on ANSI and JIS keyboards, XDEVL_SCANCODE_NONUSBACKSLASH on ISO (see comment about virtual key code 10 above) */
		/*  51 */   XDEVL_SCANCODE_BACKSPACE,
		/*  52 */   XDEVL_SCANCODE_KP_ENTER, /* keyboard enter on portables */
		/*  53 */   XDEVL_SCANCODE_ESCAPE,
		/*  54 */   XDEVL_SCANCODE_RGUI,
		/*  55 */   XDEVL_SCANCODE_LGUI,
		/*  56 */   XDEVL_SCANCODE_LSHIFT,
		/*  57 */   XDEVL_SCANCODE_CAPSLOCK,
		/*  58 */   XDEVL_SCANCODE_LALT,
		/*  59 */   XDEVL_SCANCODE_LCTRL,
		/*  60 */   XDEVL_SCANCODE_RSHIFT,
		/*  61 */   XDEVL_SCANCODE_RALT,
		/*  62 */   XDEVL_SCANCODE_RCTRL,
		/*  63 */   XDEVL_SCANCODE_RGUI, /* fn on portables, acts as a hardware-level modifier already, so we don't generate events for it, also XK_Meta_R */
		/*  64 */   XDEVL_SCANCODE_F17,
		/*  65 */   XDEVL_SCANCODE_KP_PERIOD,
		/*  66 */   XDEVL_SCANCODE_UNKNOWN, /* unknown (unused?) */
		/*  67 */   XDEVL_SCANCODE_KP_MULTIPLY,
		/*  68 */   XDEVL_SCANCODE_UNKNOWN, /* unknown (unused?) */
		/*  69 */   XDEVL_SCANCODE_KP_PLUS,
		/*  70 */   XDEVL_SCANCODE_UNKNOWN, /* unknown (unused?) */
		/*  71 */   XDEVL_SCANCODE_NUMLOCKCLEAR,
		/*  72 */   XDEVL_SCANCODE_VOLUMEUP,
		/*  73 */   XDEVL_SCANCODE_VOLUMEDOWN,
		/*  74 */   XDEVL_SCANCODE_MUTE,
		/*  75 */   XDEVL_SCANCODE_KP_DIVIDE,
		/*  76 */   XDEVL_SCANCODE_KP_ENTER, /* keypad enter on external keyboards, fn-return on portables */
		/*  77 */   XDEVL_SCANCODE_UNKNOWN, /* unknown (unused?) */
		/*  78 */   XDEVL_SCANCODE_KP_MINUS,
		/*  79 */   XDEVL_SCANCODE_F18,
		/*  80 */   XDEVL_SCANCODE_F19,
		/*  81 */   XDEVL_SCANCODE_KP_EQUALS,
		/*  82 */   XDEVL_SCANCODE_KP_0,
		/*  83 */   XDEVL_SCANCODE_KP_1,
		/*  84 */   XDEVL_SCANCODE_KP_2,
		/*  85 */   XDEVL_SCANCODE_KP_3,
		/*  86 */   XDEVL_SCANCODE_KP_4,
		/*  87 */   XDEVL_SCANCODE_KP_5,
		/*  88 */   XDEVL_SCANCODE_KP_6,
		/*  89 */   XDEVL_SCANCODE_KP_7,
		/*  90 */   XDEVL_SCANCODE_UNKNOWN, /* unknown (unused?) */
		/*  91 */   XDEVL_SCANCODE_KP_8,
		/*  92 */   XDEVL_SCANCODE_KP_9,
		/*  93 */   XDEVL_SCANCODE_INTERNATIONAL3, /* Cosmo_USB2ADB.c says "Yen (JIS)" */
		/*  94 */   XDEVL_SCANCODE_INTERNATIONAL1, /* Cosmo_USB2ADB.c says "Ro (JIS)" */
		/*  95 */   XDEVL_SCANCODE_KP_COMMA, /* Cosmo_USB2ADB.c says ", JIS only" */
		/*  96 */   XDEVL_SCANCODE_F5,
		/*  97 */   XDEVL_SCANCODE_F6,
		/*  98 */   XDEVL_SCANCODE_F7,
		/*  99 */   XDEVL_SCANCODE_F3,
		/* 100 */   XDEVL_SCANCODE_F8,
		/* 101 */   XDEVL_SCANCODE_F9,
		/* 102 */   XDEVL_SCANCODE_LANG2, /* Cosmo_USB2ADB.c says "Eisu" */
		/* 103 */   XDEVL_SCANCODE_F11,
		/* 104 */   XDEVL_SCANCODE_LANG1, /* Cosmo_USB2ADB.c says "Kana" */
		/* 105 */   XDEVL_SCANCODE_PRINTSCREEN, /* On ADB keyboards, this key is labeled "F13/print screen". Problem: USB has different usage codes for these two functions. On Apple USB keyboards, the key is labeled "F13" and sends the F13 usage code (XDEVL_SCANCODE_F13). I decided to use XDEVL_SCANCODE_PRINTSCREEN here nevertheless since SDL applications are more likely to assume the presence of a print screen key than an F13 key. */
		/* 106 */   XDEVL_SCANCODE_F16,
		/* 107 */   XDEVL_SCANCODE_SCROLLLOCK, /* F14/scroll lock, see comment about F13/print screen above */
		/* 108 */   XDEVL_SCANCODE_UNKNOWN, /* unknown (unused?) */
		/* 109 */   XDEVL_SCANCODE_F10,
		/* 110 */   XDEVL_SCANCODE_APPLICATION, /* windows contextual menu key, fn-enter on portables */
		/* 111 */   XDEVL_SCANCODE_F12,
		/* 112 */   XDEVL_SCANCODE_UNKNOWN, /* unknown (unused?) */
		/* 113 */   XDEVL_SCANCODE_PAUSE, /* F15/pause, see comment about F13/print screen above */
		/* 114 */   XDEVL_SCANCODE_INSERT, /* the key is actually labeled "help" on Apple keyboards, and works as such in Mac OS, but it sends the "insert" usage code even on Apple USB keyboards */
		/* 115 */   XDEVL_SCANCODE_HOME,
		/* 116 */   XDEVL_SCANCODE_PAGEUP,
		/* 117 */   XDEVL_SCANCODE_DELETE,
		/* 118 */   XDEVL_SCANCODE_F4,
		/* 119 */   XDEVL_SCANCODE_END,
		/* 120 */   XDEVL_SCANCODE_F2,
		/* 121 */   XDEVL_SCANCODE_PAGEDOWN,
		/* 122 */   XDEVL_SCANCODE_F1,
		/* 123 */   XDEVL_SCANCODE_LEFT,
		/* 124 */   XDEVL_SCANCODE_RIGHT,
		/* 125 */   XDEVL_SCANCODE_DOWN,
		/* 126 */   XDEVL_SCANCODE_UP,
		/* 127 */   XDEVL_SCANCODE_POWER
	};
	/* *INDENT-ON* */
}
