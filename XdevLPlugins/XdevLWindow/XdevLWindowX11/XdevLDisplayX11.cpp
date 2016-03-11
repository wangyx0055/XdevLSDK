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
	xdl::XdevLString("Display module for the X11 system."),
	XDEVLX11_DISPLAY_MODULE_MAJOR_VERSION,
	XDEVLX11_DISPLAY_MODULE_MINOR_VERSION,
	XDEVLX11_DISPLAY_MODULE_PATCH_VERSION
};

namespace xdl {

	XdevLDisplayX11::XdevLDisplayX11(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor) :
		XdevLModuleImpl<XdevLDisplay>(parameter, desriptor),
		m_display(nullptr),
		m_rootWindow(None),
		m_bestFitWidth(-1),
		m_bestFitHeight(-1),
		m_bestFitRate(-1) {

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

		m_rootWindow = RootWindow(m_display, DefaultScreen(m_display));

		if(!XRRQueryExtension(m_display, &m_event_basep, &m_error_basep)) {
			XDEVL_MODULE_ERROR("RandR extension missing.\n");
			return ERR_ERROR;
		}

		// Check if the RandR extension is avaible.
		if(!XRRQueryVersion(m_display, &m_randrMajor, &m_randrMinor)) {
			XDEVL_MODULE_ERROR("RandR extension missing.\n");
			return ERR_ERROR;
		}

		m_originalScreenConfig = XRRGetScreenInfo(m_display, m_rootWindow);
		if(nullptr == m_originalScreenConfig) {
			XDEVL_MODULE_ERROR("XRRGetScreenInfo failed.\n");
			return ERR_ERROR;
		}

		m_originalScreenRate = XRRConfigCurrentRate(m_originalScreenConfig);
		m_originalSizeId = XRRConfigCurrentConfiguration(m_originalScreenConfig, &m_originalRotation);

		// Get all supported screen sizes
		xdl_int numberOfScreenSizes;
		XRRScreenSize* sizelist = XRRConfigSizes(m_originalScreenConfig, &numberOfScreenSizes);
		if (sizelist && (numberOfScreenSizes > 0)) {
			for(xdl_int i = 0; i < numberOfScreenSizes; i++) {
				XdevLDisplayModeX11 mode;
				mode.sizeId = i;
				mode.size.width = sizelist[i].width;
				mode.size.height = sizelist[i].height;
				mode.screenConfig = XRRGetScreenInfo(m_display, m_rootWindow);
				XRRConfigCurrentConfiguration(m_originalScreenConfig, &mode.rotation);

				xdl_int ratecount;
				short* ratelist = XRRConfigRates(m_originalScreenConfig, mode.sizeId, &ratecount);
				for(xdl_int i = 0; i < ratecount; i++) {
					mode.rate = ratelist[i];
					m_displayModes.push_back(mode);
				}
			}
		}

		return ERR_OK;
	}

	xdl_int XdevLDisplayX11::shutdown() {
		XRRFreeScreenConfigInfo(m_originalScreenConfig);

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

		// Change screen mode.
		if(XRRSetScreenConfigAndRate(m_display,
		                             m_originalScreenConfig,
		                             m_rootWindow,
		                             m_displayModes[mode].sizeId,
		                             m_displayModes[mode].rotation,
		                             m_displayModes[mode].rate,
		                             CurrentTime) != Success) {
			XDEVL_MODULE_ERROR("XRRSetScreenConfigAndRate failed. That means the specified size, rotation or rate couldn't be changed.\n");
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
			match = abs(tmp.rate - m_originalScreenRate);
			if(match < bestmatch) {
				bestmatch = match;
				closestDisplayMode = tmp;
			}
		}

		XDEVL_MODULE_INFO("Found matching screen size: " << closestDisplayMode.size.width
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
		XRRSetScreenConfigAndRate(m_display,
		                          m_originalScreenConfig,
		                          m_rootWindow,
		                          m_originalSizeId,
		                          m_originalRotation,
		                          m_originalScreenRate,
		                          CurrentTime);
		return ERR_OK;
	}

	xdl_int XdevLDisplayX11::attach(XdevLWindowX11* window) {
		XRRSelectInput(window->getNativeDisplay(), window->getNativeWindow(), RRScreenChangeNotifyMask);
		
		return ERR_OK;
	}
}
