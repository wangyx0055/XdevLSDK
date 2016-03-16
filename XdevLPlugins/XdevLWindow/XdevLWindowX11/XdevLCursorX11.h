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

#ifndef XDEVL_CURSOR_X11_H
#define XDEVL_CURSOR_X11_H

#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>
#include <X11/extensions/Xfixes.h>

#include <XdevLWindow/XdevLWindowImpl.h>

namespace xdl {

	class XdevLCursorX11 : public XdevLModuleImpl<XdevLCursor>  {
		public:
			virtual ~XdevLCursorX11() {}

			//
			// Contructor for the XdevL plugin system.
			//
			XdevLCursorX11(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor);

			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;
			virtual void* getInternal(const XdevLInternalName& id) override;

			//
			// XdevLCursor interface methods.
			//
			virtual xdl_int attach(XdevLWindow* window) override;
			virtual void show() override;
			virtual void hide() override;
			virtual void setPosition(xdl_uint x, xdl_uint y) override;
			virtual xdl_int clip(xdl_uint x1, xdl_uint y1, xdl_uint x2, xdl_uint y2) override;
			virtual void releaseClip() override;
			virtual xdl_int enableRelativeMotion() override;
			virtual void disableRelativeMotion() override;
			virtual xdl_bool isRelativeMotionEnabled() override;

			//
			// XdevLCursorX11 related methods.
			//
			void onHandleXinputEvent(XGenericEventCookie* cookie, XdevLWindow* window) ;
			void parseValuators(const double *input_values,unsigned char *mask,int mask_len, double *output_values,int output_values_len);

		private:

			XdevLWindowX11* m_window;
			Cursor		m_invisibleCursor;
			Pixmap		m_invisibleCursorPixmap;
			XColor		m_black;
			XColor		m_dummy;
			Colormap	m_defaultColorMap;
			xdl_int		m_screenNumber;

			//
			// Pointer barrier stuff.
			//
			xdl_bool	m_barriersSupported;
			xdl_int		m_fixes_opcode;
			xdl_int		m_fixes_event;
			xdl_int		m_fixes_error;
			PointerBarrier m_barriers[4];

			xdl_bool	m_xinput2Supported;
			xdl_int		m_xinput2_opcode;
			xdl_int		m_xinput2_event;
			xdl_int		m_xinput2_error;

			xdl_int m_screenWidth;
			xdl_int m_screenHeight;

			xdl_bool m_reltaiveModeEnabled;
	};

}

#endif
