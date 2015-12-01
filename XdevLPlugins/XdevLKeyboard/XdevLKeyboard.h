/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2012 Cengiz Terzibas

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

#ifndef XDEVL_KEYBOARD_H
#define XDEVL_KEYBOARD_H

#include <XdevLModule.h>
#include <XdevLInputSystem.h>
#include <XdevLKeyboard/XdevLScanCode.h>
#include <XdevLKeyboard/XdevLKeyCode.h>

namespace xdl {

	class XdevLWindow;

	enum XdevLKeyStates {
	    XDEVL_KEY_UP,
	    XDEVL_KEY_DOWN
	};

	/**
		@class XdevLKeyboard
		@brief Interface class for keyboards.

		@section xdevl_keyboard_usage Usage

		@author Cengiz Terzibas
	 */

	class XdevLKeyboard : public XdevLInputSystem, public XdevLModule {
		public:
			virtual ~XdevLKeyboard() {};
			/// Attach this keyboard to a window.
			/**
				It is important that this device is attached to a window otherwise it will not work.
				After you have created this device you should directly attach it to a window. If
				your application doesn't need a window you can't use this device. Future development
				will eliminate this restriction.
				@note The attachment ensures that only key events from that window will be captured and
				interpreted as events. Otherwise every key strokes from other applications whould cause
				an key event. Mostly this is not what you want.

				@param window A valid pointer to an XdevLWindowDevice.
				@return Returns @e ERR_OK if the attachment was successful otherwise @e ERR_ERROR.
			*/
			virtual xdl_int attach(XdevLWindow* window) = 0;
			/// Returns the state of a button.
			/**
				@return Returns @e 'true' as long as the specified button is pressed. Otherwise it
				will return @e 'false'.
			*/
			virtual xdl_bool getPressed(const xdl_uint key) = 0;
			/// Returns clicked state of the specified button.
			/**
				The default click response time (crt) is 300ms. To change the crt use the setClickResponseTime()
				function.

				@return Returns @e 'true' only once after the specified mouse button was pressed and released
				within the click response time (crt).
			*/
			virtual xdl_bool getClicked(const xdl_uint key) = 0;
	};


	typedef XdevLKeyboard	IXdevLKeyboard;
	typedef XdevLKeyboard*	IPXdevLKeyboard;
}

#endif
