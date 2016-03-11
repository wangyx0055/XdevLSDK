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

#include "XdevLWindowX11.h"
#include "XdevLCursorX11.h"

//
// The XdevLCursor module descriptor.
//
xdl::XdevLModuleDescriptor cursorX11Desc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_CURSOR_MODULE_NAME],
	xdl::window_copyright,
	xdl::windowServerDescription,
	XDEVLX11CURSOR_MODULE_MAJOR_VERSION,
	XDEVLX11CURSOR_MODULE_MINOR_VERSION,
	XDEVLX11CURSOR_MODULE_PATCH_VERSION
};


namespace xdl {

	const XdevLID MouseMotionRelative("XDEVL_MOUSE_MOTION_RELATIVE");

	XdevLCursorX11::XdevLCursorX11(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor) :
		XdevLModuleImpl<XdevLCursor>(parameter, desriptor),
		m_window(nullptr),
		m_barriersSupported(xdl_false),
		m_xinput2Supported(xdl_false),
		m_reltaiveModeEnabled(xdl_false) {
		memset(m_barriers, 0, sizeof(PointerBarrier)*4);
	}

	xdl_int XdevLCursorX11::init() {
		return ERR_OK;
	}

	xdl_int XdevLCursorX11::attach(XdevLWindow* window) {
		XDEVL_ASSERT(window, "Invalid window used.");

		m_window = static_cast<XdevLWindowX11*>(window);

		m_screenNumber 		= DefaultScreen(m_window->getNativeDisplay());
		m_defaultColorMap 	= DefaultColormap(m_window->getNativeDisplay(), DefaultScreen(m_window->getNativeDisplay()));
		m_screenWidth 		= DisplayWidth(m_window->getNativeDisplay(), DefaultScreen(m_window->getNativeDisplay()));
		m_screenHeight 		= DisplayHeight(m_window->getNativeDisplay(),DefaultScreen(m_window->getNativeDisplay()));

		// Lets first create black color for the specific color map we use.
		XAllocNamedColor(m_window->getNativeDisplay(), m_defaultColorMap, "black", &m_black, &m_dummy);


		// Check for pointer barrier support.
		if(XQueryExtension(m_window->getNativeDisplay(), "XFIXES", &m_fixes_opcode, &m_fixes_event, &m_fixes_error)) {
			m_barriersSupported = xdl_true;
		}

		// Check for xinput2 support.
		if(XQueryExtension(m_window->getNativeDisplay(), "XInputExtension", &m_xinput2_opcode, &m_xinput2_event, &m_xinput2_error)) {
			m_xinput2Supported = xdl_true;
		}

		// Now we create an empty bitmap where we will fill nothing with the color.
		static char bm_no_data[] = {0, 0, 0, 0, 0, 0, 0, 0};
		m_invisibleCursorPixmap = XCreateBitmapFromData(m_window->getNativeDisplay(), m_window->getNativeWindow(), bm_no_data, 8, 8);

		// Now create the new cursor bitmap which is of course back, transparent.
		m_invisibleCursor = XCreatePixmapCursor(m_window->getNativeDisplay(), m_invisibleCursorPixmap, m_invisibleCursorPixmap, &m_black, &m_black, 0, 0);
		XFlush(m_window->getNativeDisplay());

		return ERR_OK;
	}

	xdl_int XdevLCursorX11::shutdown() {
		if(m_window == nullptr) {
			return ERR_OK;
		}

		XFreeColors(m_window->getNativeDisplay(), m_defaultColorMap, &m_black.pixel, 1, 0);
		XFreeCursor(m_window->getNativeDisplay(), m_invisibleCursor);
		XFreePixmap(m_window->getNativeDisplay(), m_invisibleCursorPixmap);

		return ERR_OK;
	}

	void* XdevLCursorX11::getInternal(const XdevLInternalName& id) {
		return nullptr;
	}

	void XdevLCursorX11::show() {
		XUndefineCursor(m_window->getNativeDisplay(), m_window->getNativeWindow());
		XFlush(m_window->getNativeDisplay());
	}

	void XdevLCursorX11::hide() {
		XDefineCursor(m_window->getNativeDisplay(), m_window->getNativeWindow(), m_invisibleCursor);
		XFlush(m_window->getNativeDisplay());
	}

	void XdevLCursorX11::setPosition(xdl_uint x, xdl_uint y) {
		XWarpPointer(m_window->getNativeDisplay(), None, m_window->getNativeWindow(), 0, 0, 0, 0, x, y);
		XFlush(m_window->getNativeDisplay());
	}

	xdl_int XdevLCursorX11::clip(xdl_uint x1, xdl_uint y1, xdl_uint x2, xdl_uint y2) {
		if(xdl_false == m_barriersSupported) {
			return ERR_ERROR;
		}

		// Release previous barriers.
		releaseClip();

		/** Create the left barrier */
		m_barriers[0] = XFixesCreatePointerBarrier(m_window->getNativeDisplay(), DefaultRootWindow(m_window->getNativeDisplay()),
		                x1, 0,
		                x1, m_screenHeight,
		                BarrierPositiveX,
		                0, NULL);
		/** Create the right barrier */
		m_barriers[1] = XFixesCreatePointerBarrier(m_window->getNativeDisplay(), DefaultRootWindow(m_window->getNativeDisplay()),
		                x2, 0,
		                x2, m_screenHeight,
		                BarrierNegativeX,
		                0, NULL);
		/** Create the top barrier */
		m_barriers[2] = XFixesCreatePointerBarrier(m_window->getNativeDisplay(), DefaultRootWindow(m_window->getNativeDisplay()),
		                0, y1,
		                m_screenWidth, y1,
		                BarrierPositiveY,
		                0, NULL);
		/** Create the bottom barrier */
		m_barriers[3] = XFixesCreatePointerBarrier(m_window->getNativeDisplay(), DefaultRootWindow(m_window->getNativeDisplay()),
		                0, y2,
		                m_screenWidth, y2,
		                BarrierNegativeY,
		                0, NULL);
		return ERR_OK;


	}

	void XdevLCursorX11::releaseClip() {
		if((m_barriers[0] > 0) || (m_barriers[1] > 0) || (m_barriers[2] > 0) || (m_barriers[3] > 0)) {
			for(xdl_int i = 0; i < 4; i++) {
				XFixesDestroyPointerBarrier(m_window->getNativeDisplay(), m_barriers[i]);
				m_barriers[i] = 0;
			}
		}
	}

	xdl_int XdevLCursorX11::enableRelativeMotion() {
		if(xdl_false == m_xinput2Supported) {
			return ERR_ERROR;
		}

		XIEventMask eventmask;
		unsigned char mask[5] = { 0,0,0,0,0 };

		eventmask.deviceid = XIAllMasterDevices;
		eventmask.mask_len = sizeof(mask);
		eventmask.mask = mask;

		XISetMask(mask, XI_RawMotion);
		XISetMask(mask, XI_RawButtonPress);
		XISetMask(mask, XI_RawButtonRelease);

		if(XISelectEvents(m_window->getNativeDisplay(), DefaultRootWindow(m_window->getNativeDisplay()), &eventmask,1) != Success) {
			m_xinput2Supported = xdl_false;
			return ERR_ERROR;
		}
		m_reltaiveModeEnabled = xdl_true;
		return ERR_OK;
	}
	void XdevLCursorX11::disableRelativeMotion() {
		m_reltaiveModeEnabled = xdl_false;
	}

	xdl_bool XdevLCursorX11::isRelativeMotionEnabled() {
		return m_reltaiveModeEnabled;
	}

	void XdevLCursorX11::onHandleXinputEvent(XGenericEventCookie* cookie, XdevLWindow* window) {

		// Only handle xinput2 events.
		if(cookie->extension != m_xinput2_opcode) {
			return;
		}

		switch(cookie->evtype) {
			case XI_RawMotion: {
				const XIRawEvent *rawev = (const XIRawEvent*)cookie->data;
				if(rawev == nullptr) {
					return;
				}

				double relative_cords[2];
				parseValuators(rawev->raw_values,rawev->valuators.mask, rawev->valuators.mask_len,relative_cords,2);


				Window rootWindow = None;
				Window childWindow = None;
				xdl_int x_root, y_root;
				xdl_int x_window, y_window;
				xdl_uint modifier_mask;
				XQueryPointer(m_window->getNativeDisplay(), m_window->getNativeWindow(), &rootWindow, &childWindow, &x_root, &y_root, &x_window, &y_window, &modifier_mask);

				XdevLEvent ev;
				ev.type 							= MouseMotionRelative.getHashCode();
				ev.motion.timestamp		= getMediator()->getTimer().getTime64();
				ev.motion.x						= x_window;
				ev.motion.y						= y_window;
				ev.motion.xrel				= relative_cords[0];
				ev.motion.yrel				= relative_cords[1];
				ev.window.windowid		= window->getWindowID();
				getMediator()->fireEvent(ev);

				return;
			}
			break;

			case XI_RawButtonPress:
			case XI_RawButtonRelease:
				break;
		}
	}

	void XdevLCursorX11::parseValuators(const double *input_values,unsigned char *mask,int mask_len, double *output_values,int output_values_len) {
		const int MAX_AXIS = 16;

		int i = 0,z = 0;
		int top = mask_len * 8;
		if(top > MAX_AXIS)
			top = MAX_AXIS;

		memset(output_values, 0, output_values_len * sizeof(double));
		for(; i < top && z < output_values_len; i++) {
			if(XIMaskIsSet(mask, i)) {
				const int value = (int) *input_values;
				output_values[z] = value;
				input_values++;
			}
			z++;
		}
	}

}
