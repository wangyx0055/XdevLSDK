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
#include "XdevLDisplayX11.h"

//
// The XdevLDisplay module descriptor.
//
xdl::XdevLModuleDescriptor windowX11DisplayDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_WINDOW_DISPLAY_MODULE_NAME],
	xdl::window_copyright,
	STRING("Display module for the X11 system."),
	XDEVLX11_DISPLAY_MODULE_MAJOR_VERSION,
	XDEVLX11_DISPLAY_MODULE_MINOR_VERSION,
	XDEVLX11_DISPLAY_MODULE_PATCH_VERSION
};

namespace xdl {

	XdevLDisplayX11::XdevLDisplayX11(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor) :
		XdevLModuleImpl<XdevLDisplay>(parameter, desriptor),
		m_display(nullptr),
		m_rootWindow(None) {

	}

	XdevLDisplayX11::~XdevLDisplayX11() {

	}

	xdl_int XdevLDisplayX11::init() {

		// Start X server with thread support.
		XInitThreads();

		// Connect to X server. TODO We are using the default display. Needs to be changed later to
		// make it more flexible.
		m_display = XOpenDisplay(nullptr);
		if(m_display == nullptr) {
			return ERR_ERROR;;
		}

		// Get the root window of the X11 display.
		m_rootWindow = RootWindow(m_display, DefaultScreen(m_display));

		if(!XRRQueryExtension(m_display, &m_event_basep, &m_error_basep)) {
			XDEVL_MODULEX_ERROR(XdevLDisplayX11, "RandR extension missing.\n");
			return ERR_ERROR;
		}

		// Check if the RandR extension is avaible.
		if(!XRRQueryVersion(m_display, &m_randrMajor, &m_randrMinor)) {
			XDEVL_MODULEX_ERROR(XdevLDisplayX11, "RandR extension missing.\n");
			return ERR_ERROR;
		}

		m_screenResources = XRRGetScreenResources(m_display, m_rootWindow);
		m_primaryOutput = XRRGetOutputPrimary(m_display, m_rootWindow);

		for(xdl_int i = 0;  i < m_screenResources->ncrtc;  i++) {
			XRRCrtcInfo* crtcInfo = XRRGetCrtcInfo(m_display, m_screenResources, m_screenResources->crtcs[i]);

			for (xdl_int j = 0;  j < crtcInfo->noutput;  j++) {
				XRROutputInfo* outputInfo = XRRGetOutputInfo(m_display, m_screenResources, crtcInfo->outputs[j]);

				if (outputInfo->connection != RR_Connected) {
					XRRFreeOutputInfo(outputInfo);
					continue;
				}

				XdevLMonitor monitor;

				// Is the monitor content rotated?
				if (crtcInfo->rotation == RR_Rotate_90 || crtcInfo->rotation == RR_Rotate_270) {
					monitor.m_widthMM  = outputInfo->mm_height;
					monitor.m_heightMM = outputInfo->mm_width;
				} else {
					monitor.m_widthMM  = outputInfo->mm_width;
					monitor.m_heightMM = outputInfo->mm_height;
				}

				monitor.m_output = crtcInfo->outputs[j];
				monitor.m_crtc   = outputInfo->crtc;

				m_monitors.push_back(monitor);

				XRRFreeOutputInfo(outputInfo);
			}

			XRRFreeCrtcInfo(crtcInfo);
		}

		// Get the current screen info so we can revert back how the user had it.
		m_originalDisplayMode.screenConfig = XRRGetScreenInfo(m_display, m_rootWindow);
		if(nullptr == m_originalDisplayMode.screenConfig) {
			XDEVL_MODULEX_ERROR(XdevLDisplayX11, "XRRGetScreenInfo failed.\n");
			return ERR_ERROR;
		}

		// Get the current screen rate. (Hz)
		m_originalDisplayMode.rate = XRRConfigCurrentRate(m_originalDisplayMode.screenConfig);

		// Get the current screen resolution and it's rotation info.
		m_originalDisplayMode.sizeId = XRRConfigCurrentConfiguration(m_originalDisplayMode.screenConfig, &m_originalDisplayMode.rotation);

		xdl_int numSizes;
		XRRScreenSize* screenSizes = XRRSizes(m_display, 0, &numSizes);
		m_originalDisplayMode.size.width = screenSizes[m_originalDisplayMode.sizeId].width;
		m_originalDisplayMode.size.height = screenSizes[m_originalDisplayMode.sizeId].height;

		//
		// Now let's get all supported screen sizes and store it.
		//
		xdl_int numberOfScreenSizes;
		XRRScreenSize* sizelist = XRRConfigSizes(m_originalDisplayMode.screenConfig, &numberOfScreenSizes);
		if (sizelist && (numberOfScreenSizes > 0)) {
			for(xdl_int i = 0; i < numberOfScreenSizes; i++) {
				XdevLDisplayModeX11 mode;
				mode.sizeId = i;
				mode.size.width = sizelist[i].width;
				mode.size.height = sizelist[i].height;
				mode.screenConfig = XRRGetScreenInfo(m_display, m_rootWindow);
				XRRConfigCurrentConfiguration(m_originalDisplayMode.screenConfig, &mode.rotation);

				xdl_int ratecount;
				short* ratelist = XRRConfigRates(m_originalDisplayMode.screenConfig, mode.sizeId, &ratecount);
				for(xdl_int i = 0; i < ratecount; i++) {
					mode.rate = ratelist[i];
					m_displayModes.push_back(mode);
				}
			}
		}

		// We tell the X11 display server that we are interessted in display change events.
		XRRSelectInput(m_display, m_rootWindow, RROutputChangeNotifyMask);

		return ERR_OK;
	}

	xdl_int XdevLDisplayX11::shutdown() {

		// Let's free resources we allocated.
		XRRFreeScreenConfigInfo(m_originalDisplayMode.screenConfig);

		if(nullptr != m_display) {
			XCloseDisplay(m_display);
			m_display = nullptr;
		}
		return ERR_OK;
	}

	XdevLDisplayModes XdevLDisplayX11::getDisplayModes() {
		XdevLDisplayModes displayModes;

		for(auto displayMode : m_displayModes) {
			XdevLDisplayMode mode;
			mode.size = displayMode.size;
			mode.rate = displayMode.rate;
			displayModes.push_back(mode);
		}

		return displayModes;
	}

	XdevLDisplayModeId XdevLDisplayX11::getDisplayModeId(const XdevLDisplayMode& mode) {
		auto find = std::find(m_displayModes.begin(), m_displayModes.end(), mode);
		if(find == m_displayModes.end()) {
			return -1;
		}
		return std::distance(m_displayModes.begin(), find);
	}

	xdl_int XdevLDisplayX11::setDisplayMode(const XdevLDisplayModeId& mode) {
		if(mode == -1) {
			return ERR_ERROR;
		}

		m_currentDisplayMode = m_displayModes[mode];

		// Change screen mode.
		if(XRRSetScreenConfigAndRate(m_display,
		                             m_currentDisplayMode.screenConfig,
		                             m_rootWindow,
		                             m_displayModes[mode].sizeId,
		                             m_displayModes[mode].rotation,
		                             m_displayModes[mode].rate,
		                             CurrentTime) != RRSetConfigSuccess) {
			XDEVL_MODULEX_ERROR(XdevLDisplayX11, "XRRSetScreenConfigAndRate failed. That means the specified size, rotation or rate couldn't be changed.\n");
			return ERR_ERROR;
		}

		return ERR_OK;
	}

	XdevLDisplayModeId XdevLDisplayX11::getClosestDisplayModeId(const XdevLDisplayMode& mode) {

		xdl_int match;
		xdl_int bestmatch = 999999;
		std::vector<XdevLDisplayModeX11> closestDisplayModes;
		for(auto tmp : m_displayModes) {
			match = (mode.size.width - tmp.size.width) * (mode.size.width - tmp.size.width) +
			        (mode.size.height - tmp.size.height) * (mode.size.height - tmp.size.height);
			if(match < bestmatch) {
				bestmatch = match;
				closestDisplayModes.push_back(tmp);
			}
		}

		bestmatch = 999999;
		XdevLDisplayModeX11 closestDisplayMode;
		for(auto tmp : closestDisplayModes) {
			match = abs(tmp.rate - m_originalDisplayMode.rate);
			if(match < bestmatch) {
				bestmatch = match;
				closestDisplayMode = tmp;
			}
		}

		XDEVL_MODULEX_INFO(XdevLDisplayX11, "Found matching screen size: " << closestDisplayMode.size.width
		                   << "x" << closestDisplayMode.size.height
		                   << " " << closestDisplayMode.rate
		                   << " Hz" << std::endl);

		auto find = std::find(m_displayModes.begin(), m_displayModes.end(), closestDisplayMode);
		if(find == m_displayModes.end()) {
			return -1;
		}
		return std::distance(m_displayModes.begin(), find);
	}

	xdl_int XdevLDisplayX11::restore() {
		// Change screen mode into it's original form.
		if(XRRSetScreenConfigAndRate(m_display,
		                             m_originalDisplayMode.screenConfig,
		                             m_rootWindow,
		                             m_originalDisplayMode.sizeId,
		                             m_originalDisplayMode.rotation,
		                             m_originalDisplayMode.rate,
		                             CurrentTime) != RRSetConfigSuccess) {
			XDEVL_MODULEX_ERROR(XdevLDisplayX11, "XRRSetScreenConfigAndRate failed. Couldn't restore the previous display settings.\n");
			return ERR_ERROR;
		}
		return ERR_OK;
	}

	xdl_int XdevLDisplayX11::attach(XdevLWindowX11* window) {
		XRRSelectInput(window->getNativeDisplay(), window->getNativeWindow(), RRScreenChangeNotifyMask);

		return ERR_OK;
	}

	xdl_bool XdevLDisplayX11::onHandleDisplayEvent(XEvent& event) {

		// Check if this is a RRNotify event.
		if(event.type != RRNotify + m_event_basep) {
			return xdl_false;
		}

		XRRUpdateConfiguration(&event);

		switch(event.type - m_event_basep) {
			case RRScreenChangeNotify: {
				std::cout << "RRScreenChangeNotify" << std::endl;
			} break;
		}
		return xdl_true;
	}

	void XdevLDisplayX11::update(XEvent& event) {
		XRRUpdateConfiguration(&event);
	}

}
