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

#ifndef XDEVL_KEYBOARD_H
#define XDEVL_KEYBOARD_H

#include <XdevLModule.h>
#include <XdevLInputSystem.h>

namespace xdl {

	class XdevLWindow;

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
