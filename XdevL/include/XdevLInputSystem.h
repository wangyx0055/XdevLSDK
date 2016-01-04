/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2015 Cengiz Terzibas

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

#ifndef XDEVL_EVENT_CATALYST_H
#define XDEVL_EVENT_CATALYST_H

#include <XdevLTypes.h>
#include <XdevLID.h>
#include <XdevLDelegate.h>

namespace xdl {

	enum XdevLButtonState {
	    BUTTON_PRESSED,
	    BUTTON_RELEASED
	};

	enum XdevLKeyMode {
	    KEY_MOD_NONE 		= 0x0000,
	    KEY_MOD_LSHIFT 		= 0x0001,
	    KEY_MOD_RSHIFT 		= 0x0002,
	    KEY_MOD_LCTRL 		= 0x0040,
	    KEY_MOD_RCTRL 		= 0x0080,
	    KEY_MOD_LALT 		= 0x0100,
	    KEY_MOD_RALT 		= 0x0200,
	    KEY_MOD_LGUI 		= 0x0400,
	    KEY_MOD_RGUI 		= 0x0800,
	    KEY_MOD_NUM 		= 0x1000,
	    KEY_MOD_CAPS 		= 0x2000,
	    KEY_MOD_MODE 		= 0x4000,
	    KEY_MOD_RESERVED 	= 0x8000
	};

	/**
		@enum XdevLButtonId
		@brief Mouse button identification codes.
		@author Cengiz Terzibas
	*/
	enum XdevLButtonId {
	    BUTTON_0 = 1,
	    BUTTON_LEFT = 1,
	    BUTTON_1 = 2,
	    BUTTON_MIDDLE = 2,
	    BUTTON_2 = 3,
	    BUTTON_RIGHT = 3,
	    BUTTON_3 = 4,
	    BUTTON_SIDE = 4,
	    BUTTON_4 = 5,
	    BUTTON_FORWARD = 5,
	    BUTTON_5 = 6,
	    BUTTON_BACK = 6,
	    BUTTON_6 = 7,
	    BUTTON_EXTRA = 7,
	    BUTTON_7 = 8,
	    BUTTON_TASK = 8,
	    BUTTON_8,
	    BUTTON_9,
	    BUTTON_10,
	    BUTTON_11,
	    BUTTON_12,
	    BUTTON_13,
	    BUTTON_14,
	    BUTTON_15,
	    BUTTON_16 = 17,
	    KEY_A = 17,
	    BUTTON_17 = 18,
	    KEY_B = 18,
	    BUTTON_18 = 19,
	    KEY_C = 19,
	    BUTTON_19 = 20,
	    KEY_D = 20,
	    BUTTON_20 = 21,
	    KEY_E = 21,
	    BUTTON_21 = 22,
	    KEY_F = 22,
	    BUTTON_22 = 23,
	    KEY_G = 23,
	    BUTTON_23 = 24,
	    KEY_H = 24,
	    BUTTON_24 = 25,
	    KEY_I = 25,
	    BUTTON_25 = 26,
	    KEY_J = 26,
	    BUTTON_26 = 27,
	    KEY_K = 27,
	    BUTTON_27 = 28,
	    KEY_L = 28,
	    BUTTON_28 = 29,
	    KEY_M = 29,
	    BUTTON_29 = 30,
	    KEY_N = 30,
	    BUTTON_30 = 31,
	    KEY_O = 31,
	    BUTTON_31 = 32,
	    KEY_P = 32,
	    BUTTON_32 = 33,
	    KEY_Q = 33,
	    BUTTON_33 = 34,
	    KEY_R = 34,
	    BUTTON_34 = 35,
	    KEY_S = 35,
	    BUTTON_35 = 36,
	    KEY_T = 36,
	    BUTTON_36 = 37,
	    KEY_U = 37,
	    BUTTON_37 = 38,
	    KEY_V = 38,
	    BUTTON_38 = 39,
	    KEY_W = 39,
	    BUTTON_39 = 40,
	    KEY_X = 40,
	    BUTTON_40 = 41,
	    KEY_Y = 41,
	    BUTTON_41 = 42,
	    KEY_Z = 42,
	    BUTTON_42 = 43,
	    KEY_0 = 43,
	    BUTTON_44 = 44,
	    KEY_1 = 44,
	    KEY_2,
	    KEY_3,
	    KEY_4,
	    KEY_5,
	    KEY_6,
	    KEY_7,
	    KEY_8,
	    KEY_9,

	    KEY_F1,
	    KEY_F2,
	    KEY_F3,
	    KEY_F4,
	    KEY_F5,
	    KEY_F6,
	    KEY_F7,
	    KEY_F8,
	    KEY_F9,
	    KEY_F10,
	    KEY_F11,
	    KEY_F12,
	    KEY_F13,
	    KEY_F14,
	    KEY_F15,
	    KEY_F16,
	    KEY_F17,
	    KEY_F18,
	    KEY_F19,
	    KEY_F20,
	    KEY_F21,
	    KEY_F22,
	    KEY_F23,
	    KEY_F24,

	    KEY_ESCAPE,
	    KEY_HOME,
	    KEY_END,
	    KEY_INSERT,
	    KEY_DELETE,
	    KEY_PAGEUP,
	    KEY_PAGEDOWN,
	    KEY_PLUS,
	    KEY_MINUS,
	    KEY_COMMA,
	    KEY_PERIOD,
	    KEY_SLASH,
	    KEY_BACKSLASH,
	    KEY_LEFTBRACKET,
	    KEY_RIGHTBRACKET,
	    KEY_SEMICOLON,
	    KEY_COLON,
	    KEY_UNDERSCORE,

	    KEY_SPACE,
	    KEY_BACKSPACE,
	    KEY_ENTER,
	    KEY_TAB,
	    KEY_LSUPER,
	    KEY_RSUPER,

	    KEY_UP,
	    KEY_DOWN,
	    KEY_LEFT,
	    KEY_RIGHT,

	    KEY_LALT,
	    KEY_LCTRL,
	    KEY_LSHIFT,
	    KEY_RALT,
	    KEY_RCTRL,
	    KEY_RSHIFT,

	    KEY_KP_0,
	    KEY_KP_1,
	    KEY_KP_2,
	    KEY_KP_3,
	    KEY_KP_4,
	    KEY_KP_5,
	    KEY_KP_6,
	    KEY_KP_7,
	    KEY_KP_8,
	    KEY_KP_9,

	    KEY_KP_NUM_LOCK,
	    KEY_KP_DIVIDE,
	    KEY_KP_MULTIPLY,
	    KEY_KP_MINUS,
	    KEY_KP_PLUS,
	    KEY_KP_ENTER,
	    KEY_KP_PERIOD,

	    KEY_NUMLOCK,
	    KEY_PRINTSCREEN,
	    KEY_SCROLLLOCK,
	    KEY_PAUSE,

	    KEY_CAPSLOCK,
	    KEY_RGUI,
	    KEY_LGUI,

	    KEY_EQUALS,
	    KEY_SYSREQ,
	    KEY_NONUSBACKSLASH,
	    KEY_KP_EQUALS,

	    KEY_APPLICATION,
	    KEY_CALCULATOR,
	    KEY_APP1,
	    KEY_APP2,
	    KEY_AC_BOOKMARKS,
	    KEY_AC_BACK,
	    KEY_AC_FORWARD,
	    KEY_EJECT,
	    KEY_AUDIONEXT,
	    KEY_AUDIOPLAY,
	    KEY_AUDIOPREV,
	    KEY_AUDIOSTOP,
	    KEY_AC_HOME,
	    KEY_AC_REFRESH,
	    KEY_AC_SEARCH,
	    KEY_DISPLAYSWITCH,
	    KEY_MEDIASELECT,
	    KEY_APOSTROPHE,
	    KEY_GRAVE,
	    KEY_MODE,
	    KEY_VOLUMEDOWN,
	    KEY_VOLUMEUP,
	    KEY_POWER,
	    KEY_CANCEL,
	    KEY_AGAIN,
	    KEY_UNDO,
	    KEY_COPY,
	    KEY_PASTE,
	    KEY_FIND,
	    KEY_CUT,
	    KEY_HELP,
	    KEY_WWW,
	    KEY_MAIL,
	    KEY_COMPUTER,
	    KEY_MUTE,
	    KEY_SLEEP,
	    KEY_BRIGHTNESSDOWN,
	    KEY_BRIGHTNESSUP,
	    KEY_KBDILLUMTOGGLE,
	    KEY_KBDILLUMDOWN,
	    KEY_KBDILLUMUP,
	    KEY_STOP,
	    BUTTON_UNKOWN,
	    KEY_UNKNOWN = BUTTON_UNKOWN,
	};

	inline XdevLString xdevLButtonIdToString(XdevLButtonId buttonId ) {
		switch(buttonId) {
			case KEY_0:
				return XdevLString("KEY_0");
			case KEY_1:
				return XdevLString("KEY_1");
			case KEY_2:
				return XdevLString("KEY_2");
			case KEY_3:
				return XdevLString("KEY_3");
			case KEY_4:
				return XdevLString("KEY_4");
			case KEY_5:
				return XdevLString("KEY_5");
			case KEY_6:
				return XdevLString("KEY_6");
			case KEY_7:
				return XdevLString("KEY_7");
			case KEY_8:
				return XdevLString("KEY_8");
			case KEY_9:
				return XdevLString("KEY_9");
			case KEY_A:
				return XdevLString("KEY_A");
			case KEY_B:
				return XdevLString("KEY_B");
			case KEY_C:
				return XdevLString("KEY_C");
			case KEY_D:
				return XdevLString("KEY_D");
			case KEY_E:
				return XdevLString("KEY_E");
			case KEY_F:
				return XdevLString("KEY_F");
			case KEY_G:
				return XdevLString("KEY_G");
			case KEY_H:
				return XdevLString("KEY_H");
			case KEY_I:
				return XdevLString("KEY_I");
			case KEY_J:
				return XdevLString("KEY_J");
			case KEY_K:
				return XdevLString("KEY_K");
			case KEY_L:
				return XdevLString("KEY_L");
			case KEY_M:
				return XdevLString("KEY_M");
			case KEY_N:
				return XdevLString("KEY_N");
			case KEY_O:
				return XdevLString("KEY_O");
			case KEY_P:
				return XdevLString("KEY_p");
			case KEY_Q:
				return XdevLString("KEY_Q");
			case KEY_R:
				return XdevLString("KEY_R");
			case KEY_S:
				return XdevLString("KEY_S");
			case KEY_T:
				return XdevLString("KEY_T");
			case KEY_U:
				return XdevLString("KEY_U");
			case KEY_V:
				return XdevLString("KEY_V");
			case KEY_W:
				return XdevLString("KEY_W");
			case KEY_X:
				return XdevLString("KEY_X");
			case KEY_Y:
				return XdevLString("KEY_Y");
			case KEY_Z:
				return XdevLString("KEY_Z");
			case KEY_F1:
				return XdevLString("KEY_F1");
			case KEY_F2:
				return XdevLString("KEY_F2");
			case KEY_F3:
				return XdevLString("KEY_F3");
			case KEY_F4:
				return XdevLString("KEY_F4");
			case KEY_F5:
				return XdevLString("KEY_F5");
			case KEY_F6:
				return XdevLString("KEY_F6");
			case KEY_F7:
				return XdevLString("KEY_F7");
			case KEY_F8:
				return XdevLString("KEY_F8");
			case KEY_F9:
				return XdevLString("KEY_F9");
			case KEY_F10:
				return XdevLString("KEY_F10");
			case KEY_F11:
				return XdevLString("KEY_F11");
			case KEY_F12:
				return XdevLString("KEY_F12");
			case KEY_F13:
				return XdevLString("KEY_F13");
			case KEY_F14:
				return XdevLString("KEY_F14");
			case KEY_F15:
				return XdevLString("KEY_F15");
			case KEY_F16:
				return XdevLString("KEY_F16");
			case KEY_F17:
				return XdevLString("KEY_F17");
			case KEY_F18:
				return XdevLString("KEY_F18");
			case KEY_F19:
				return XdevLString("KEY_F19");
			case KEY_F20:
				return XdevLString("KEY_F20");
			case KEY_F21:
				return XdevLString("KEY_F21");
			case KEY_F22:
				return XdevLString("KEY_F22");
			case KEY_F23:
				return XdevLString("KEY_F23");
			case KEY_F24:
				return XdevLString("KEY_F24");
			case KEY_ESCAPE:
				return XdevLString("KEY_ESCAPE");
			case KEY_LSHIFT:
				return XdevLString("KEY_LSHIFT");
			case KEY_LCTRL:
				return XdevLString("KEY_LCTRL");
			case KEY_LGUI:
				return XdevLString("KEY_LGUI");
			case KEY_LALT:
				return XdevLString("KEY_LALT");
			case KEY_RSHIFT:
				return XdevLString("KEY_RSHIFT");
			case KEY_RCTRL:
				return XdevLString("KEY_RCTRL");
			case KEY_RGUI:
				return XdevLString("KEY_RGUI");
			case KEY_RALT:
				return XdevLString("KEY_RALT");
			case KEY_SPACE:
				return XdevLString("KEY_SPACE");
			case KEY_ENTER:
				return XdevLString("KEY_ENTER");
			case KEY_BACKSPACE:
				return XdevLString("KEY_BACKSPACE");
			case KEY_TAB:
				return XdevLString("KEY_TAB");
			case KEY_CAPSLOCK:
				return XdevLString("KEY_CAPSLOCK");
			case KEY_UP:
				return XdevLString("KEY_UP");
			case KEY_DOWN:
				return XdevLString("KEY_DOWN");
			case KEY_LEFT:
				return XdevLString("KEY_LEFT");
			case KEY_RIGHT:
				return XdevLString("KEY_RIGHT");
			case KEY_PRINTSCREEN:
				return XdevLString("KEY_PRINTSCREEN");
			case KEY_SCROLLLOCK:
				return XdevLString("KEY_SCROLLLOCK");
			case KEY_PAUSE:
				return XdevLString("KEY_PAUSE");
			case KEY_INSERT:
				return XdevLString("KEY_INSERT");
			case KEY_HOME:
				return XdevLString("KEY_HOME");
			case KEY_PAGEUP:
				return XdevLString("KEY_PAGEUP");
			case KEY_PAGEDOWN:
				return XdevLString("KEY_PAGEDOWN");
			case KEY_DELETE:
				return XdevLString("KEY_DELETE");
			case KEY_END:
				return XdevLString("KEY_END");
			case KEY_KP_0:
				return XdevLString("KEY_KP_0");
			case KEY_KP_1:
				return XdevLString("KEY_KP_1");
			case KEY_KP_2:
				return XdevLString("KEY_KP_2");
			case KEY_KP_3:
				return XdevLString("KEY_KP_3");
			case KEY_KP_4:
				return XdevLString("KEY_KP_4");
			case KEY_KP_5:
				return XdevLString("KEY_KP_5");
			case KEY_KP_6:
				return XdevLString("KEY_KP_6");
			case KEY_KP_7:
				return XdevLString("KEY_KP_7");
			case KEY_KP_8:
				return XdevLString("KEY_KP_8");
			case KEY_KP_9:
				return XdevLString("KEY_KP_9");
			case KEY_KP_NUM_LOCK:
				return XdevLString("KEY_KP_NUM_LOCK");
			case KEY_KP_DIVIDE:
				return XdevLString("KEY_KP_DIVIDE");
			case KEY_KP_MULTIPLY:
				return XdevLString("KEY_KP_MULTIPLY");
			case KEY_KP_MINUS:
				return XdevLString("KEY_KP_MINUS");
			case KEY_KP_PLUS:
				return XdevLString("KEY_KP_PLUS");
			case KEY_KP_ENTER:
				return XdevLString("KEY_KP_ENTER");
			case KEY_KP_PERIOD:
				return XdevLString("KEY_KP_PERIOD");
			case KEY_MINUS:
				return XdevLString("KEY_MINUS");
			case KEY_EQUALS:
				return XdevLString("KEY_EQUALS");
			case KEY_LEFTBRACKET:
				return XdevLString("KEY_LEFTBRACKET");
			case KEY_RIGHTBRACKET:
				return XdevLString("KEY_RIGHTBRACKET");
			case KEY_COLON:
				return XdevLString("KEY_COLON");
			case KEY_SEMICOLON:
				return XdevLString("KEY_SEMICOLON");
			case KEY_APOSTROPHE:
				return XdevLString("KEY_APOSTROPHE");
			case KEY_UNDERSCORE:
				return XdevLString("KEY_UNDERSCORE");
			case KEY_GRAVE:
				return XdevLString("KEY_GRAVE");
			case KEY_COMMA:
				return XdevLString("KEY_COMMA");
			case KEY_SLASH:
				return XdevLString("KEY_SLASH");
			case KEY_BACKSLASH:
				return XdevLString("KEY_BACKSLASH");
			case KEY_PERIOD:
				return XdevLString("KEY_PERIOD");
			case KEY_NUMLOCK:
				return XdevLString("KEY_NUMLOCK");

			default:
				break;
		}
		return XdevLString("BUTTON_UNKNOWN");
	}

	/**
		@enum XdevLAxisId
		@brief Mouse axis identification codes.
		@author Cengiz Terzibas
	*/
	enum XdevLAxisId {
	    AXIS_0 = 0,
	    AXIS_X = 0,
	    AXIS_1 = 1,
	    AXIS_Y = 1,
	    AXIS_2 = 2,
			AXIS_Z = 2,
	    AXIS_3,
	    AXIS_4,
	    AXIS_5,
	    AXIS_6,
	    AXIS_7,
	    AXIS_8,
	    AXIS_9,
	    AXIS_10,
	    AXIS_11,
	    AXIS_12,
	    AXIS_13,
	    AXIS_14,
	    AXIS_15,
	    AXIS_UNKNOWN
	};


	/**
		@enum XdevLPOVId
		@brief Joystick POV identifications for example.
		@author Cengiz Terzibas
	*/
	enum XdevLPOVId {
	    POV_0,
	    POV_1,
	    POV_2,
	    POV_3
	};

	/**
		@enum XdevLPOVDirectionId
		@brief POV pointing direction identifications.
		@author Cengiz Terzibas
	*/
	enum XdevLPOVDirectionId {
	    POV_UP = 0,
	    POV_RIGHT_UP,
	    POV_RIGHT,
	    POV_RIGHT_DOWN,
	    POV_DOWN,
	    POV_LEFT_DOWN,
	    POV_LEFT,
	    POV_LEFT_UP,
	    POV_CENTERED
	};

	/**
		@class XdevLButton
		@brief Interface for buttons.
		@author Cengiz Terzibas

		This class supports buttons. The system doesn't distinguish between joystick or mouse buttons.

		@note The wheel mouse is represented as a button too. Scrolling up/down could mean: BUTTON4/BUTTON5

	*/
	class XdevLButton {
		public:
			virtual ~XdevLButton() {}
			/// Returns the state of a button.
			/**
				@return
				- @b 'true' as long as the specified button is pressed.
				- @b 'false' if the button is not pressed.
			*/

			virtual xdl_bool getPressed() = 0;
			/// Returns clicked state of the specified button.
			/**
				The default click response time (crt) is 300ms. To change the crt use the setClickResponseTime()
				function.

				@return
				- @b 'true' only once after the specified mouse button was pressed and released within the click
				response time (crt).
				- @b 'false' if the first condition not occurs.
			*/
			virtual xdl_bool getClicked() = 0;
			/// Sets the click response time
			/**
				@param crt The click response time in microsends.
			*/
			virtual void setClickResponseTime(xdl_uint64 crt) = 0;
			/// Returns the click response time
			/**
				@return Returns the click response time in microsends.
			*/
			virtual xdl_uint64 getClickResponseTime() = 0;
	};

	/**
		@class XdevLAxis
		@brief Interface for axis.
		@author Cengiz Terzibas

		An axis is a limited slider.
	*/
	class XdevLAxis {
		public:
			virtual ~XdevLAxis() {}
			/// Returns the current axis value.
			/**
				The value of the axis lies between [min, max] and is a float value. The resolution of the steps in this
				range is vendor specified and is not defined. Please check the technical specification of your device.
				Here a diagram.
				@code
					 min        max
						o----------]

				@endcode

				@return Returns the current axis position.
			*/
			virtual xdl_float getValue() = 0;

			virtual xdl_float getDeltaValue() = 0;

			/// Sets the minimum and maximum.
			/**
				@param min The minimum value.
				@param max The maximum value.
			*/
			virtual void setMinMax(xdl_float min, xdl_float max) = 0;

			/// Sets the minimum value of the axis.
			virtual void setMin(xdl_float min) = 0;

			/// Sets the maximum value of the axis.
			virtual void setMax(xdl_float max) = 0;

			/// Returns min and max value.
			/**
				@param min Should be a valid pointer to an xdl_float object. This variable will be set with the min value.
				@param max Should be a valid pointer to a xdl_float object. This variable will be set with the min value.
			*/
			virtual xdl_int getMinMax(xdl_float* min, xdl_float* max) = 0;

			/// Returns the minimum value.
			/**
				@return The current minimum value.
			*/
			virtual xdl_float getMin() const = 0;

			/// Returns the maximum value.
			/**
				@return The current maximum value.
			*/
			virtual xdl_float getMax() const = 0;

	};


	/**
		@class XdevLPOV
		@brief Interface for POV's control.
		@author Cengiz Terzibas

		POV is an abbreviation and means (Point of View) and is called sometimes
		Head switch. Most new modern joysticks supports this control. For example,
		many flight simulators use it to switch the player's views.
	*/
	class XdevLPOV {
		public:
			virtual ~XdevLPOV() {}
			/// Returns the state of an POV button element.
			/**
				@return The identification code for a POV button. Use one of these id's eXdevLPOVDirection.
			*/
			virtual xdl_uint getDirection() const = 0;
	};


	typedef XdevLDelegate<void, const XdevLButtonId&, const XdevLButtonState&> XdevLButtonDelegateType;
	typedef XdevLDelegate<void, const XdevLAxisId&, const xdl_float&> XdevLAxisDelegateType;
	typedef XdevLDelegate<void, const XdevLButtonState&> XdevLButtonIdDelegateType;
	typedef XdevLDelegate<void, const xdl_float&> XdevLAxisIdDelegateType;


	/**
		@class XdevLInputSystem
		@brief Interface for input devices like keyboard, mouse and joystick.
		@author Cengiz Terzibas

	*/
	class XdevLInputSystem {
		public:
			virtual ~XdevLInputSystem() {}

			/// Register one delegate for a specific button event.
			virtual xdl_int registerDelegate(const XdevLString& id, const XdevLButtonIdDelegateType& delegate) = 0;

			/// Register one delegate for a specific axis event.
			virtual xdl_int registerDelegate(const XdevLString& id, const XdevLAxisIdDelegateType& delegate) = 0;

			/// Register a delegate for button events.
			virtual xdl_int registerDelegate(const XdevLButtonDelegateType& delegate) = 0;

			/// Register a delegate for axis events.
			virtual xdl_int registerDelegate(const XdevLAxisDelegateType& delegate) = 0;

			/// Returns the specified button object of the device.
			/**
				@param idx The identification code of the button. For platform independence use the respective
				device depended id codes. More information about this id's look into the documentation
				of the devices itself. You can use also integer values which lies between [0, getNumButtons()]
				but without a guarantee for platform independence.
				@param button A valid pointer to an XdevLButton pointer. This pointer
				will hold a valid pointer to an XdevLButton object if it will be successful.
				If not the pointer will be NULL.
				@return
				- @b ERR_OK The button exists.
				- @b ERR_ERROR It seems like the specified button doesn't exist.
			*/
			virtual xdl_int getButton(const xdl_uint idx, XdevLButton** button) = 0;

			/// Returns the number of supported buttons.
			virtual xdl_uint getNumButtons() const = 0;

			/// Returns the specified axis object of the device.
			/**
				@param idx The identification code of the button. For platform independence use the respective
				device depended id codes. More information about this id's look into the documentation
				of the devices itself. You can use also integer values which lies between [0, getNumAxis()]
				but without a guarantee for platform independence.
				@param axis A valid pointer to an XdevLAxis pointer. This pointer
				will hold a valid pointer to an XdevLAxis object if it will be successful.
				If not the pointer will be NULL.
				@return
				- @b ERR_OK The axis exists.
				- @b ERR_ERROR It seems like the specified axis doesn't exist.
			*/
			/// Returns the specified axis object of the joystick device.
			virtual xdl_int getAxis(const xdl_uint idx, XdevLAxis** axis) const = 0;

			/// Returns the number of supported axis.
			virtual xdl_uint getNumAxis() const = 0;

			/// Returns a valid POV object.
			/**
				@param idx The identification code of the button. For platform independence use the respective
				device depended id codes. More information about this id's look into the documentation
				of the devices itself. You can use also integer values which lies between [0, getNumPOV()]
				but without a guarantee for platform independence. Most joysticks support only one POV.
				@param pov A valid pointer to an XdevLPOV pointer. This pointer
				will hold a valid pointer to an XdevLPOV object if it will be successful.
				If not the pointer will be NULL.
				- @b ERR_OK The POV exists.
				- @b ERR_ERROR It seems like the specified POV doesn't exist.
			*/
			virtual xdl_int getPOV(const xdl_uint idx, XdevLPOV** pov) const = 0;

			/// Returns the number of supported POV's.
			virtual xdl_uint getNumPOV() const = 0;

			/// Sets the click response time (crt) for all key/buttons
			/**
				A click is an event that starts with a button/key press and ends with a release. The time difference
				between press and release is called click response time.
				This method can change the default value which is 300ms.
				@param clickResponsteTime The click response time in seconds.
			*/
			virtual void setClickResponseTimeForAll(xdl_double clickResponsteTime) = 0;

			/// Sets the click response time for a specific button.
			/**
				A click is an event that starts with a button/key press and ends with a release. The time difference
				between press and release is called click response time.
				This method can change the default value which is 300ms.

				@param button The button identifier .
				@param clickResponsteTime The click response time in seconds.
			*/
			virtual void setClickResponseTime(const xdl_uint button, xdl_double clickResponsteTime) = 0;

			/// Returns the click response time for a specific button.
			/**
				@return Returns the click response time in seconds.
			*/
			virtual xdl_double getClickResponseTime(const xdl_uint button) = 0;

			/// Sets the axis min max range values.
			virtual void setAxisRangeMinMax(const xdl_uint axis, xdl_float min, xdl_float max) = 0;

			/// Sets the axis minimum range value.
			virtual void setAxisRangeMin(const xdl_uint axis, xdl_float min) = 0;

			/// Sets the axis maximum range value.
			virtual void seAxisRangeMax(const xdl_uint axis, xdl_float max) = 0;

			/// Return the axis minimum and maximum range values.
			virtual void getAxisRangeMinMax(const xdl_uint axis, xdl_float* min, xdl_float* max) = 0;

			/// Returns the axis minimum range value.
			virtual xdl_float getAxisRangeMin(const xdl_uint axis) const = 0;

			/// Returns the axis maximum range value.
			virtual xdl_float getAxisRangeMax(const xdl_uint axis) const = 0;
	};


	typedef XdevLButton*	IPXdevLButton;
	typedef XdevLButton		IXdevLButton;

	typedef XdevLAxis*		IPXdevLAxis;
	typedef XdevLAxis		IXdevLAxis;

	typedef XdevLPOV*		IPXdevLPOV;
	typedef XdevLPOV		IXdevLPOV;

}

#endif
