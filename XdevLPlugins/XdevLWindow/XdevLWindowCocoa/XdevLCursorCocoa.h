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

#ifndef XDEVL_CURSOR_COCOA_H
#define XDEVL_CURSOR_COCOA_H

// Import stuff for the cocoa window.
#import <Cocoa/Cocoa.h>

// Import stuff for the cursor.
#import <IOKit/hid/IOHIDKeys.h>
#import <IOKit/hidsystem/IOHIDShared.h>

#include <XdevLWindow/XdevLWindowImpl.h>

namespace xdl {

	class XdevLWindowCocoa;

	class XdevLCursorCocoa : public XdevLModuleImpl<XdevLCursor>  {
		public:
			XdevLCursorCocoa(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor);

			virtual ~XdevLCursorCocoa() {}

			xdl_int init() override;
			xdl_int shutdown() override;
			void* getInternal(const XdevLInternalName& id) override;

			xdl_int attach(XdevLWindow* window) override;
			void show() override;
			void hide() override;
			void setPosition(xdl_uint x, xdl_uint y) override;
			xdl_int clip(xdl_uint x, xdl_uint y, xdl_uint width, xdl_uint height);
			void releaseClip() override;
			xdl_int enableRelativeMotion() override;
			void disableRelativeMotion() override;
			xdl_bool isRelativeMotionEnabled() override;
		private:
			NSCursor* m_cursor;
			XdevLWindowCocoa* m_window;
			io_object_t m_hidInterface;
			xdl_double m_acceleration;
	};
}

#endif
