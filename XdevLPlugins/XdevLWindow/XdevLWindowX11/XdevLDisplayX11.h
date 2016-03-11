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

#ifndef XDEVL_DISPLAY_X11_H
#define XDEVL_DISPLAY_X11_H

#include <XdevLWindow/XdevLWindowImpl.h>

#include <X11/extensions/Xrandr.h>
#include <X11/Xlib.h>

namespace xdl {

	class XdevLWindowX11;

	class XdevLDisplayModeX11 : public XdevLDisplayMode {
			SizeID getSizeId() {
				return sizeId;
			}
			friend bool operator==(const XdevLDisplayModeX11& m1, const XdevLDisplayMode& m2) {
				return ((m1.size == m2.size) and (m1.rate == m2.rate));
			}
		public:
			XRRScreenConfiguration* screenConfig;
			Rotation rotation;
			SizeID sizeId;
	};

	class XdevLDisplayX11 : public XdevLModuleImpl<XdevLDisplay> {
		public:
			XdevLDisplayX11(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor);
			virtual ~XdevLDisplayX11();

			//
			// Contructor for the XdevL plugin system.
			//
			xdl_int init() override;
			xdl_int shutdown() override;

			//
			// XdevLCursor interface methods.
			//
			XdevLDisplayModes getDisplayModes() override;
			XdevLDisplayModeId getDisplayModeId(const XdevLDisplayMode& mode) override;
			XdevLDisplayModeId getClosestDisplayModeId(const XdevLDisplayMode& mode) override;
			xdl_int setDisplayMode(const XdevLDisplayModeId& mode) override;
			xdl_int restore() override;

			//
			// XdevLCursorX11 related methods.
			//
			xdl_int attach(XdevLWindowX11* window);

			xdl_bool onHandleDisplayEvent(XEvent& event);

			void update(XEvent& event);

		private:

			Display* m_display;
			Window m_rootWindow;

			xdl_int m_randrMajor;
			xdl_int m_randrMinor;

			xdl_int m_event_basep;
			xdl_int m_error_basep;
			xdl_int m_bestFitWidth;
			xdl_int m_bestFitHeight;
			xdl_int m_bestFitRate;
			xdl_int m_screenWidth;
			xdl_int m_screenHeight;

			SizeID m_bestSizeId;
			SizeID m_originalSizeId;
			XRRScreenConfiguration* m_originalScreenConfig;
			short int m_originalScreenRate;
			Rotation m_originalRotation;

			std::vector<XdevLDisplayModeX11> m_displayModes;
	};

}

#endif
