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

#include <iostream>
#include <XdevLTypes.h>
#include <XdevLWindowX11.h>
#include <XdevLXstring.h>
#include <XdevLUtils.h>
#include <sstream>
#include <cstddef>
#include <climits>
#include <vector>

static xdl::xdl_int reference_counter = 0;
static std::shared_ptr<xdl::XdevLX11Display> globalDisplay;
static Display* display = nullptr;

static const xdl::XdevLString windowX11PluginName {
	"XdevLWindowX11"
};

//
// The XdevLWindow plugin descriptor.
//
xdl::XdevLPluginDescriptor windowX11PluginDescriptor {
	windowX11PluginName,
	xdl::window_moduleNames,
	XDEVLX11_MAJOR_VERSION,
	XDEVLX11_MINOR_VERSION,
	XDEVLX11_PATCH_VERSION
};

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

//
// The XdevLWindow module descriptor.
//
xdl::XdevLModuleDescriptor windowX11Desc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_WINDOW_MODULE_NAME],
	xdl::window_copyright,
	xdl::XdevLString("Support for X11 window creation."),
	XDEVLX11_MODULE_MAJOR_VERSION,
	XDEVLX11_MODULE_MINOR_VERSION,
	XDEVLX11_MODULE_PATCH_VERSION
};

//
// The XdevLWindowEventServer module descriptor.
//
xdl::XdevLModuleDescriptor windowServerX11Desc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_WINDOW_SERVER_MODULE_NAME],
	xdl::window_copyright,
	xdl::windowServerDescription,
	XDEVLX11_SERVER_MODULE_MAJOR_VERSION,
	XDEVLX11_SERVER_MODULE_MINOR_VERSION,
	XDEVLX11_SERVER_MODULE_PATCH_VERSION
};

//
// The XdevLWindowEventServer module descriptor.
//
xdl::XdevLModuleDescriptor windowEventServerX11Desc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_WINDOW_EVENT_SERVER_MODULE_NAME],
	xdl::window_copyright,
	xdl::windowServerDescription,
	XDEVLX11_EVENT_SERVER_MODULE_MAJOR_VERSION,
	XDEVLX11_EVENT_SERVER_MODULE_MINOR_VERSION,
	XDEVLX11_EVENT_SERVER_MODULE_PATCH_VERSION
};

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

	const XdevLID ButtonPressed("XDEVL_BUTTON_PRESSED");
	const XdevLID ButtonReleased("XDEVL_BUTTON_RELEASED");
	const XdevLID MouseButtonPressed("XDEVL_MOUSE_BUTTON_PRESSED");
	const XdevLID MouseButtonReleased("XDEVL_MOUSE_BUTTON_RELEASED");
	const XdevLID MouseMotion("XDEVL_MOUSE_MOTION");
	const XdevLID MouseMotionRelative("XDEVL_MOUSE_MOTION_RELATIVE");
	const XdevLID WindowEvent("XDEVL_WINDOW_EVENT");


#define _NET_WM_STATE_REMOVE    0l
#define _NET_WM_STATE_ADD       1l
#define _NET_WM_STATE_TOGGLE    2l
#define MWM_HINTS_DECORATIONS (1L << 1)

	enum {
	  KDE_noDecoration = 0,
	  KDE_normalDecoration = 1,
	  KDE_tinyDecoration = 2,
	  KDE_noFocus = 256,
	  KDE_standaloneMenuBar = 512,
	  KDE_desktopIcon = 1024 ,
	  KDE_staysOnTop = 2048
	};


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

//
//
//


	XdevLX11Display::XdevLX11Display(XdevLCoreMediator* core) : m_core(core) {
		std::cout << "XdevLX11Display::XdevLX11Display()\n";

		// Start X server with thread support.
		XInitThreads();

		// Connect to X server. TODO We are using the default display. Needs to be changed later to
		// make it more flexible.
		display = XOpenDisplay(nullptr);
		if(display == nullptr) {
			return;
		}

		// Print out some useless information :D
		std::clog << "\n---------------------------- X11 Server Information ----------------------------\n";
		std::clog << "Vendor              : " << XServerVendor(display) << "\n";
		std::clog << "Release             : " << XVendorRelease(display)<< "\n";
		std::clog << "Number of Screens   : " << XScreenCount(display) 	<< std::endl;

		XdevLModuleCreateParameter parameter;
		parameter.setModuleId(XdevLID("XdevLWindowEventServer"));
		windowEventServer = std::make_shared<XdevLWindowEventServerX11>(&parameter, windowEventServerX11Desc);

		parameter.setModuleId(XdevLID("XdevLCursor"));
		cursor = std::make_shared<XdevLCursorX11>(&parameter, cursorX11Desc);

		// Register within the Core if this is using one.
		if(m_core) {
			m_core->registerModule(windowEventServer);
			m_core->registerModule(cursor);
		}

	}


	XdevLX11Display::~XdevLX11Display() {
		std::cout << "XdevLX11Display::~XdevLX11Display()\n";
		if(display) {
			if(m_core) {
				m_core->deleteModule(windowEventServer->getID());
				m_core->deleteModule(cursor->getID());
			}
			XCloseDisplay(display);
			display = nullptr;
		}
	}

//
// XdevLWindowX11
//

	XdevLWindowX11::XdevLWindowX11(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor) :
		XdevLWindowImpl(XdevLWindowImpl::getWindowsCounter(), parameter, desriptor),
		m_display(nullptr),
		m_rootWindow(None),
		m_window(None),
		m_screenNumber(0),
		m_screenWidth(0),
		m_screenHeight(0),
		m_fullscreenModeActive(xdl_false) {
		XDEVL_MODULE_INFO("XdevLWindowX11()\n");

		//
		// Initialize connection to display server just once.
		//
		if(0 == reference_counter) {
			XdevLCoreMediator* mediator = nullptr;
			if(nullptr != parameter) {
				mediator = parameter->getMediator();
			}
			globalDisplay = std::make_shared<XdevLX11Display>(mediator);
		}
		reference_counter++;
	}

	XdevLWindowX11::~XdevLWindowX11() {
		XDEVL_MODULE_INFO("~XdevLWindowX11()\n");

		if(reference_counter == 1) {
			globalDisplay.reset();
		}
		reference_counter--;
	}

	xdl_int XdevLWindowX11::init() {
		return XdevLWindowImpl::init();
	}

	xdl_int XdevLWindowX11::create(const XdevLWindowAttribute& attribute) {
		XdevLWindowImpl::create(attribute);
		return create();
	}

	xdl_int XdevLWindowX11::create(const XdevLWindowTitle& title,
	                               const XdevLWindowPosition& position,
	                               const XdevLWindowSize& size,
	                               const XdevLWindowTypes& type) {
		m_attribute.title = title;
		m_attribute.position = position;
		m_attribute.size = size;
		m_attribute.type = type;

		return create();
	}

	int XdevLWindowX11::create() {

		m_display = display;

		// Get the default screen number.
		m_screenNumber = DefaultScreen(m_display);

		// Get the default screen with.
		m_screenWidth = DisplayWidth(m_display, m_screenNumber);

		// Get the default screen height.
		m_screenHeight = DisplayHeight(m_display,m_screenNumber);

		// Get the root window.
		m_rootWindow = RootWindow(m_display, m_screenNumber);

		// Get the default color map.
		m_defaultColorMap = DefaultColormap(m_display, m_screenNumber);

		XColor color;
		color.red 	= 0;
		color.green = 0;
		color.blue 	= 0;
		color.pixel	= ((xdl_int)m_backgroundColor[0] << 16) | ((xdl_int)m_backgroundColor[1] << 8) | (xdl_int)m_backgroundColor[2];

//		char green[] = "#00FF00";
//		XParseColor(globalDisplay, m_defaultColorMap, green, &color);
//		XAllocColor(globalDisplay, m_defaultColorMap, &color);

//    windowAttributes.background_pixmap 	= None;
//    windowAttributes.border_pixel 			= 0;
//		windowAttributes.colormap 					= XCreateColormap(globalDisplay, m_rootWindow, visual, AllocNone);

		if(m_rootTitle.toString().size() > 0) {
			XdevLWindow* rootWindow = static_cast<XdevLWindow*>(getMediator()->getModule(XdevLID(m_rootTitle.toString().c_str())));
			m_rootWindow = (Window)(rootWindow->getInternal(XdevLInternalName("X11_WINDOW")));
		}

		xdl_int borderwith = 0;
		XSetWindowAttributes WindowAttributes;
		WindowAttributes.override_redirect	= False;

		//
		// What shall we do with the override_redirect flag? Shall we use it for popups, tooltips etc.
		// If we use it then the Window Manager has no influence on this window which can cause issues.
		if((m_attribute.type == XDEVL_WINDOW_TYPE_TOOLTIP) ||
		    (m_attribute.type == XDEVL_WINDOW_TYPE_POPUP) ||
		    (m_attribute.type == XDEVL_WINDOW_TYPE_SPLASH) ||
		    (m_attribute.type == XDEVL_WINDOW_TYPE_NOTIFICATION)) {
//			WindowAttributes.override_redirect	= True;
		} else {
//			WindowAttributes.override_redirect	= False;
		}

		WindowAttributes.background_pixmap = None;
		WindowAttributes.background_pixel = color.pixel;
		WindowAttributes.border_pixel = 0;
		WindowAttributes.colormap = CopyFromParent;

		m_window = XCreateWindow(m_display,
		                         m_rootWindow,
		                         m_attribute.position.x,
		                         m_screenHeight - (m_attribute.size.height + m_attribute.position.y),
		                         m_attribute.size.width,
		                         m_attribute.size.height,
		                         borderwith,
		                         CopyFromParent,
		                         InputOutput,
		                         CopyFromParent,
		                         CWOverrideRedirect | CWColormap | CWBackPixmap | CWBackPixel,
		                         &WindowAttributes);

		if(None == m_window) {
			XDEVL_MODULE_ERROR("Couldn't create X11 window.\n");
			return ERR_ERROR;
		}
		// Initialize all EWMH to tell the Window Manager which protocols we are going to handle.
		initializeEWMH();

		// Set the event mask to tell the X server on which events we are interessted in for this window.
		xdl_int event_mask = FocusChangeMask |        // Keyboard focus in/out events.
		                     EnterWindowMask |        // Events that tells us if the mouse pointer entered the window.
		                     LeaveWindowMask |        // Events that tells us if the mouse pointer left the window.
		                     ExposureMask |
		                     ButtonPressMask |        // Events when the user pressed one of the mouse buttons on the window.
		                     ButtonReleaseMask |      // Events when the user released one of the mouse buttons on the window.
		                     PointerMotionMask |      // Events when the user move the mouse pointer while it is over the window.
		                     KeyPressMask |           // Events when the user pressed a key.
		                     KeyReleaseMask |         // Events when the user released a key.
		                     StructureNotifyMask |    // Events when the window got Mapped, Unmapped, Destroyed, Reparented or Configured like, position, size.
		                     SubstructureNotifyMask |
		                     KeymapStateMask |
		                     GenericEvent;


		XSelectInput(m_display, m_window, event_mask);

		XClearWindow(m_display, m_window);

		//
		// Set the WM hints
		//
		XWMHints* wmHints 	= XAllocWMHints();
		wmHints->flags = 	StateHint | // We want to set the initial state of this window (NormalState)
		                  InputHint;  // We want to set the input focus model.
		wmHints->input = True;          // Does this application rely on the window manager to get keyboard input?
		wmHints->initial_state = NormalState; // Most applications start this way. WithdrawnState would hide the window and IconicState would start as icon.
		XSetWMHints(m_display, m_window, wmHints);
		XFree(wmHints);

		// Set title of the window.
		setTitle(m_attribute.title);

		// Set type of the window.
		setType(m_attribute.type);

		// Check if the user specified fullscreen mode.
		if(getFullscreen()) {
			enableFullscreenMode();
		}

		m_id = m_window;

		globalDisplay->getWindowEventServer()->registerWindowForEvents(this);

		XMapRaised(m_display, m_window);
		XSync(m_display, False);

		return ERR_OK;
	}

	void* XdevLWindowX11::getInternal(const XdevLInternalName& id) {
		std::string data(id);
		if(data == "X11_DISPLAY")
			return m_display;
		if(data == "X11_WINDOW")
			return (void*)m_window;
		if(data == "X11_SCREEN_NUMBER")
			return &m_screenNumber;
		return nullptr;
	}

	xdl_int XdevLWindowX11::shutdown() {

		// Send a more or less random value to unblock the
		// XNextEvent functions in the thread.

		XEvent ev;
		memset(&ev, 0, sizeof(ev));

		ev.xclient.type = ClientMessage;
		ev.xclient.window = m_window;
		ev.xclient.message_type = XInternAtom(m_display, "WM_PROTOCOLS", true);
		ev.xclient.format = 32;
		ev.xclient.data.l[0] = XInternAtom(m_display, "WM_DELETE_WINDOW", false);
		ev.xclient.data.l[1] = CurrentTime;
		XSendEvent(m_display, m_window, False, NoEventMask, &ev);

		// Ok, check if the are in fullscreen mode.
		if(m_fullscreenModeActive)  {
			disableFullscreenMode();
		}

//		for(auto atom : m_atoms) {
//			XDeleteProperty(m_display, m_window, atom);
//		}

		// We have to destroy the window. The X-Server can do it automatically but it's better
		// to do it manually.
		if(m_display && m_window && !m_rootWindow) {
			XDestroyWindow(m_display, m_window);
		}

		XFlush(m_display);

		if(XdevLWindowImpl::shutdown() != ERR_OK) {
			return ERR_ERROR;
		}

		return ERR_OK;
	}

	int XdevLWindowX11::update() {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");
		return ERR_OK;
	}

	void XdevLWindowX11::enableFullscreenMode() {
		// Don't do this if we already in fullscreen mode.
		if(xdl_false != m_fullscreenModeActive) {
			return;
		}

		//
		// Store Screen saver settings and disable it.
		//
		XGetScreenSaver(m_display, &timeout_return, &interval_return, &prefer_blanking_return, &allow_exposures_return);
		XSetScreenSaver(m_display, 0, 0, DontPreferBlanking, DefaultExposures);

		XSync(m_display, False);

		//
		// We use the _NET_WM_STATE_FULLSCREEN to tell the WM we want to have a fullscreen window.
		//
		XEvent msg;
		memset(&msg, 0, sizeof(XEvent));
		msg.xclient.type = ClientMessage;
		msg.xclient.message_type = _NET_WM_STATE;
		msg.xclient.window 			= m_window;
		msg.xclient.format 			= 32;
		msg.xclient.data.l[0] 	= _NET_WM_STATE_ADD;
		msg.xclient.data.l[1] 	= _NET_WM_STATE_FULLSCREEN;
		msg.xclient.data.l[2] 	= 0;
		msg.xclient.data.l[3] 	= 1; // Normal application

		if(XSendEvent(m_display, m_rootWindow, False, SubstructureNotifyMask | SubstructureRedirectMask, &msg) == 0) {
			XDEVL_MODULE_WARNING("Could not add the _NET_WM_STATE_FULLSCREEN state to the current Window Manger.\n");
		}

		//
		// Let's tell the compositor not to composite this window in fullscreen mode.
		//
		// A value of 0 indicates no preference.
		// A value of 1 hints the compositor to disabling compositing of this window.
		// A value of 2 hints the compositor to not disabling compositing of this window.

		// Change the _NET_WM_BYPASS_COMPOSITOR property of this window.
		const long _NET_WM_BYPASS_COMPOSITOR_HINT_ON = 1;
		XChangeProperty(m_display, m_window, _NET_WM_BYPASS_COMPOSITOR, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&_NET_WM_BYPASS_COMPOSITOR_HINT_ON, 1);

		XFlush(m_display);

		m_fullscreenModeActive = xdl_true;
	}

	void XdevLWindowX11::disableFullscreenMode() {

		// Only to this if we are in fullscreen mode.
		if(xdl_false == m_fullscreenModeActive) {
			return;
		}

		//
		// We use the _NET_WM_STATE_FULLSCREEN to tell the WM we want to disable the fullscreen mode.
		//
		XEvent msg;
		memset(&msg, 0, sizeof(XEvent));
		msg.xclient.type = ClientMessage;
		msg.xclient.message_type = _NET_WM_STATE;
		msg.xclient.window 			= m_window;
		msg.xclient.format 			= 32;
		msg.xclient.data.l[0] 	= _NET_WM_STATE_REMOVE;
		msg.xclient.data.l[1] 	= _NET_WM_STATE_FULLSCREEN;
		msg.xclient.data.l[2] 	= 0;
		msg.xclient.data.l[3] 	= 1; // Normal application

		if(XSendEvent(m_display, m_rootWindow,  False, SubstructureNotifyMask | SubstructureRedirectMask, &msg) == 0) {
			XDEVL_MODULE_WARNING("Could not remove the _NET_WM_STATE_FULLSCREEN state from the current Window Manger.\n");
		}

		//
		// Let's tell the compositor not to composite this window in fullscreen mode.
		//
		// A value of 0 indicates no preference.
		// A value of 1 hints the compositor to disabling compositing of this window.
		// A value of 2 hints the compositor to not disabling compositing of this window.

		// Change the _NET_WM_BYPASS_COMPOSITOR property of this window.
		const xdl_uint32 _NET_WM_BYPASS_COMPOSITOR_HINT_ON = 0;
		XChangeProperty(m_display, m_window, _NET_WM_BYPASS_COMPOSITOR, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&_NET_WM_BYPASS_COMPOSITOR_HINT_ON, 1);

		//
		// Set the previous screen saver settings.
		//
		XSetScreenSaver(m_display, timeout_return, interval_return, prefer_blanking_return, allow_exposures_return);

		XSync(m_display, False);

		m_fullscreenModeActive = xdl_false;
	}


	xdl_int XdevLWindowX11::disableDecoration() {

		xdl_bool decorations_removed = xdl_false;
		Atom hintAtom;

		// Ok, but now we have to inform the window manager about the fullscreen
		// request. EWMH and ICCCM specification.
		if(_MOTIF_WM_HINTS != None) {
			struct {
				unsigned long flags;
				unsigned long functions;
				unsigned long decorations;
				long input_mode;
				unsigned long status;
			} MWMHints = { MWM_HINTS_DECORATIONS, 0, 0, 0, 0 };

			XChangeProperty(m_display, m_window, _MOTIF_WM_HINTS, _MOTIF_WM_HINTS, 32, PropModeReplace, (unsigned char *)&MWMHints, sizeof(MWMHints)/4);
			decorations_removed = xdl_true;
		}

		hintAtom = XInternAtom(m_display, "KWM_WIN_DECORATION", True);
		if(hintAtom != None) {
			long KWMHints = KDE_tinyDecoration;

			XChangeProperty(m_display,m_window, hintAtom, hintAtom, 32, PropModeReplace, (unsigned char *)&KWMHints,  sizeof(KWMHints)/4);
			decorations_removed = xdl_true;
		}

		// Now try to set GNOME hints
		hintAtom = XInternAtom(m_display, "_WIN_HINTS", True);
		if(hintAtom != None) {
			long GNOMEHints = 0;

			XChangeProperty(m_display, m_window, hintAtom, hintAtom, 32, PropModeReplace, (unsigned char *)&GNOMEHints, sizeof(GNOMEHints)/4);
			decorations_removed = xdl_true;
		}

		// Now try to set KDE NET_WM hints
		if(_NET_WM_WINDOW_TYPE != None) {
			Atom NET_WMHints[2] = {_KDE_NET_WM_WINDOW_TYPE_OVERRIDE, _NET_WM_WINDOW_TYPE_NORMAL};

			XChangeProperty(m_display, m_window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32, PropModeReplace, (unsigned char *)&NET_WMHints, 2);
			decorations_removed = 1;
		}

		if(_NET_WM_STATE != None) {

			XEvent msg;
			memset(&msg, 0, sizeof(XEvent));
			msg.xclient.type			= ClientMessage;
			msg.xclient.message_type 	= _NET_WM_STATE;
			msg.xclient.window 			= m_window;
			msg.xclient.format 			= 32;
			msg.xclient.data.l[0] 		= _NET_WM_STATE_ADD;
			msg.xclient.data.l[1] 		= _NET_WM_STATE_FULLSCREEN;
			msg.xclient.data.l[2] 		= 0;
			msg.xclient.data.l[3] 		= 1; // Normal application

			if(XSendEvent(m_display, DefaultRootWindow(m_display),  False, SubstructureNotifyMask | SubstructureRedirectMask, &msg) == 0) {
				XDEVL_MODULE_WARNING("Could not add _NET_WM_STATE_FULLSCREEN state.\n");
			}
		}

		// Did we sucessfully remove the window decorations?
		if(decorations_removed) {
			// Finally set the transient hints
			XSetTransientForHint(m_display, m_window, m_rootWindow);
			XRaiseWindow(m_display, m_window);
		} else {
			// Seems like we couldn't remove the decoration using EWMH hints. Let's try to change they window into a override_redirect window
			// which means the Window Manager is supposed to ignore this window totally.
			XSetWindowAttributes  attributes;
			attributes.override_redirect = True;
			XChangeWindowAttributes(m_display, m_window, CWOverrideRedirect, &attributes);
		}
		XRRSelectInput(m_display, m_window, RRScreenChangeNotifyMask);
		XFlush(m_display);
		return ERR_OK;
	}

	xdl_int XdevLWindowX11::enableDecoration() {
		int                   ActivatedDecorations;
		Atom                  HintAtom;

		ActivatedDecorations = 0;

		// Unset MWM hints
		HintAtom = XInternAtom(m_display, "_MOTIF_WM_HINTS", True);
		if(HintAtom != None) {
			XDeleteProperty(m_display, m_window, HintAtom);
			ActivatedDecorations = 1;
		}

		// Unset KWM hints
		HintAtom = XInternAtom(m_display, "KWM_WIN_DECORATION", True);
		if(HintAtom != None) {
			XDeleteProperty(m_display, m_window, HintAtom);
			ActivatedDecorations = 1;
		}

		// Unset GNOME hints
		HintAtom = XInternAtom(m_display, "_WIN_HINTS", True);
		if(HintAtom != None) {
			XDeleteProperty(m_display, m_window, HintAtom);
			ActivatedDecorations = 1;
		}

		// Unset NET_WM hints
		HintAtom = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE", True);
		if(HintAtom != None) {
			Atom NET_WMHints = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_NORMAL", True);
			if(NET_WMHints != None) {
				XChangeProperty(m_display, m_window,
				                HintAtom, XA_ATOM, 32, PropModeReplace,
				                (unsigned char *)&NET_WMHints, 1);
				ActivatedDecorations = 1;
			}
		}


		if(_NET_WM_STATE != None) {

			XEvent msg;
			memset(&msg, 0, sizeof(XEvent));
			msg.xclient.type			= ClientMessage;
			msg.xclient.message_type 	= _NET_WM_STATE;
			msg.xclient.window 			= m_window;
			msg.xclient.format 			= 32;
			msg.xclient.data.l[0] 		= _NET_WM_STATE_REMOVE;
			msg.xclient.data.l[1] 		= _NET_WM_STATE_FULLSCREEN;
			msg.xclient.data.l[2] 		= 0;
			msg.xclient.data.l[3] 		= 1; // Normal application

			if(XSendEvent(m_display, DefaultRootWindow(m_display),  False, SubstructureNotifyMask | SubstructureRedirectMask, &msg) == 0) {
				XDEVL_MODULE_WARNING("Could not remove _NET_WM_STATE_FULLSCREEN state.\n");
			}

		}

		// Finally unset the transient hints if necessary
		if(ActivatedDecorations) {
			// NOTE: Does this work?
			XSetTransientForHint(m_display, m_window, None);
			XUnmapWindow(m_display, m_window);
			XMapWindow(m_display, m_window);
		}
		return ERR_OK;
	}

	void XdevLWindowX11::setFullscreen(xdl_bool state) {
		if(state) {
			enableFullscreenMode();
		} else {
			disableFullscreenMode();
		}
	}

	const XdevLWindowPosition& XdevLWindowX11::getPosition() {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

//		XWindowAttributes wa;
//		XGetWindowAttributes(m_display, m_window, &wa);
//		m_attribute.position.x = wa.x;
//		m_attribute.position.y = wa.y;
		return m_attribute.position;
	}

	const XdevLWindowSize& XdevLWindowX11::getSize() {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

//		XWindowAttributes wa;
//		XGetWindowAttributes(m_display, m_window, &wa);
//		m_attribute.size.width 	= wa.width;
//		m_attribute.size.height = wa.height;
		return m_attribute.size;
	}

	XdevLWindowSize::type  XdevLWindowX11::getWidth() {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");
//		XWindowAttributes wa;
//		XGetWindowAttributes(m_display, m_window, &wa);
//		m_attribute.size.width 	= wa.width;
//		m_attribute.size.height = wa.height;
		return m_attribute.size.width;
	}

	XdevLWindowSize::type  XdevLWindowX11::getHeight() {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");
//		XWindowAttributes wa;
//		XGetWindowAttributes(m_display, m_window, &wa);
//		m_attribute.size.width 	= wa.width;
//		m_attribute.size.height = wa.height;
		return m_attribute.size.height;
	}

	XdevLWindowPosition::type XdevLWindowX11::getX() {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");
//		XWindowAttributes wa;
//		XGetWindowAttributes(m_display, m_window, &wa);
//		m_attribute.position.x = wa.x;
//		m_attribute.position.y = wa.y;
		return m_attribute.position.x;
	}

	XdevLWindowPosition::type XdevLWindowX11::getY() {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");
//		XWindowAttributes wa;
//		XGetWindowAttributes(m_display, m_window, &wa);
//		m_attribute.position.x = wa.x;
//		m_attribute.position.y = wa.y;
		return m_attribute.position.y;
	}

	const XdevLWindowTitle& XdevLWindowX11::getTitle() {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

		return XdevLWindowImpl::getTitle();
	}

	xdl_bool XdevLWindowX11::getFullscreen() {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

		return XdevLWindowImpl::getFullscreen();
	}

	xdl_bool XdevLWindowX11::getHidePointer() {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

		return XdevLWindowImpl::getHidePointer();
	}

	void XdevLWindowX11::setX(XdevLWindowPosition::type x) {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

		XdevLWindowImpl::setX(x);
		XMoveWindow(m_display, m_window, m_attribute.position.x, m_attribute.position.y);
		XMapWindow(m_display, m_window);
	}

	void XdevLWindowX11::setY(XdevLWindowPosition::type y) {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

		XdevLWindowImpl::setY(m_screenHeight - (m_attribute.size.height + y));
		XMoveWindow(m_display, m_window, m_attribute.position.x, m_attribute.position.y);
		XMapWindow(m_display, m_window);
	}

	void XdevLWindowX11::setWidth(XdevLWindowSize::type width) {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

		XdevLWindowImpl::setWidth(width);
		XResizeWindow(m_display, m_window, m_attribute.size.width, m_attribute.size.height);
		XMapWindow(m_display, m_window);
	}

	void XdevLWindowX11::setHeight(XdevLWindowSize::type height) {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

		XdevLWindowImpl::setHeight(height);
		XResizeWindow(m_display, m_window,  m_attribute.size.width, m_attribute.size.height);
		XMapWindow(m_display, m_window);
	}

	void XdevLWindowX11::setSize(const XdevLWindowSize& size) {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

		m_attribute.size = size;

		XSizeHints *sizehints = XAllocSizeHints();
		long userhints;

		XGetWMNormalHints(m_display, m_window, sizehints, &userhints);

		sizehints->min_width = sizehints->max_width = m_attribute.size.width;
		sizehints->min_height = sizehints->max_height = m_attribute.size.height;
		sizehints->flags |= PMinSize | PMaxSize;

		XSetWMNormalHints(m_display, m_window, sizehints);

		XFree(sizehints);

		// OK What Moving the window for a resize? Well some Window Manager
		// resize only after either the user move/resize the window or within the code
		// or first unmapping then mapping.

		XResizeWindow(m_display, m_window, m_attribute.size.width, m_attribute.size.height);
		XMoveWindow(m_display, m_window, m_attribute.position.x, m_attribute.position.y);
		XRaiseWindow(m_display, m_window);
		XFlush(m_display);
	}

	void XdevLWindowX11::setPosition(const XdevLWindowPosition& position) {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

		m_attribute.position.x = position.x;
		m_attribute.position.y = m_screenHeight - (m_attribute.size.height + position.y);
		XMoveWindow(m_display, m_window, m_attribute.position.x, m_attribute.position.y);
		XMapWindow(m_display, m_window);
	}

	void XdevLWindowX11::setResizeable(xdl_bool state) {
		XSizeHints* sizeHints 	= XAllocSizeHints();
		sizeHints->flags		= USPosition | USSize | PPosition | PSize | PMinSize;
		sizeHints->x			= m_attribute.position.x;
		sizeHints->y			= m_attribute.position.y;
		sizeHints->width		= m_attribute.size.width;
		sizeHints->height		= m_attribute.size.height;
		sizeHints->min_width	= m_attribute.size.width;
		sizeHints->min_height	= m_attribute.size.height;
		XSetWMNormalHints(m_display, m_window, sizeHints);
		XFree(sizeHints);
	}

	void XdevLWindowX11::setTitle(const XdevLWindowTitle& title) {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

		XdevLWindowImpl::setTitle(title);

		XClassHint* class_hints = XAllocClassHint();
		XTextProperty windowName;
		XTextProperty iconName;

		char* window_name = (char*)getTitle().toString().c_str();
		char* icon_name = (char*)getTitle().toString().c_str();

		XStringListToTextProperty(&window_name, 1, &windowName);
		XStringListToTextProperty(&icon_name, 1, &iconName);

		class_hints->res_name   = (char*)getTitle().toString().c_str();
		class_hints->res_class  = (char*)getTitle().toString().c_str();

		XSetWMProperties(m_display, m_window, &windowName, &iconName, nullptr, 0, nullptr, nullptr, class_hints);
		XFree(class_hints);
		XFree(windowName.value);
		XFree(iconName.value);
	}


	void XdevLWindowX11::show() {
		XMapWindow(m_display, m_window);

		//XEvent event;
		//XIfEvent(m_display, &event, &isMapNotify, (XPointer)&m_window);
		XFlush(m_display);
	}

	void XdevLWindowX11::hide() {
		XUnmapEvent event;
		event.type = UnmapNotify;
		event.window = m_window;
		event.from_configure = False;
		XSendEvent(m_display, m_rootWindow, False, StructureNotifyMask | SubstructureNotifyMask, (XEvent*)&event);

		XUnmapWindow(m_display, m_window);

		//XEvent event;
		//XIfEvent(m_display, &event, &isUnmapNotify, (XPointer)&m_window);
		XFlush(m_display);
	}

	void XdevLWindowX11::raise() {
		XRaiseWindow(m_display, m_window);
		//XFlush(m_display);
	}

	xdl_bool XdevLWindowX11::isHidden() {
		Atom actualType;
		int actualFormat;
		unsigned long i, numItems, bytesAfter;
		unsigned char *propertyValue = nullptr;
		long maxLength = 1024;
		if(XGetWindowProperty(m_display, m_window, _NET_WM_STATE,
		                      0l, maxLength, False, XA_ATOM, &actualType,
		                      &actualFormat, &numItems, &bytesAfter,
		                      &propertyValue) == Success) {

			Atom *atoms = (Atom *) propertyValue;
			for(i = 0; i < numItems; ++i) {
				if(atoms[i] == _NET_WM_STATE_HIDDEN) {
					return xdl_true;
				}
			}
		}
		return xdl_false; // TODO This may cause problems.
	}

	xdl_uint32 XdevLWindowX11::getNetWMState() {
		Atom actualType;
		int actualFormat;
		unsigned long i, numItems, bytesAfter;
		unsigned char *propertyValue = nullptr;
		long maxLength = 1024;
		xdl_uint32 flags = 0;

		if(XGetWindowProperty(m_display, m_window, _NET_WM_STATE,
		                      0l, maxLength, False, XA_ATOM, &actualType,
		                      &actualFormat, &numItems, &bytesAfter,
		                      &propertyValue) == Success) {
			Atom *atoms = (Atom *) propertyValue;
			int maximized = 0;
			int fullscreen = 0;

			for(i = 0; i < numItems; ++i) {
				if(atoms[i] == _NET_WM_STATE_HIDDEN) {
					flags |= XDEVL_WINDOW_HIDDEN;
				} else if(atoms[i] == _NET_WM_STATE_FOCUSED) {
					flags |= XDEVL_WINDOW_INPUT_FOCUS_GAINED;
				} else if(atoms[i] == _NET_WM_STATE_MAXIMIZED_VERT) {
					maximized |= 1;
				} else if(atoms[i] == _NET_WM_STATE_MAXIMIZED_HORZ) {
					maximized |= 2;
				} else if(atoms[i] == _NET_WM_STATE_FULLSCREEN) {
					fullscreen = 1;
				}
			}
			if(maximized == 3) {
				flags |= XDEVL_WINDOW_MAXIMIZED;
			}  else if(fullscreen == 1) {
				flags |= XDEVL_WINDOW_FULLSCREEN;
			}
			XFree(propertyValue);
		}

		return flags;
	}

	void XdevLWindowX11::setInputFocus() {

		XWindowAttributes attribute;
		XGetWindowAttributes(m_display,m_window,&attribute);
		if(attribute.map_state == IsViewable) {
			XSetInputFocus(m_display, m_window,  RevertToParent, CurrentTime);
		}
		XFlush(m_display);


		//XGrabKeyboard(m_display, m_window, False, GrabModeAsync, GrabModeAsync, CurrentTime);

//		Atom atom = _NET_ACTIVE_WINDOW;
//		XChangeProperty(m_display, m_window, _NET_WM_STATE, XA_ATOM, 32, PropModeReplace, (unsigned char *)&atom, 1);

//		XEvent event;
//		event.xany.type 						= ClientMessage;
//		event.xclient.message_type 	= _NET_ACTIVE_WINDOW;
//		event.xclient.format 				= 32;
//		event.xclient.window 				= m_window;
//		event.xclient.data.l[0] 		= 1;  /* source indication. 1 = application */
//		event.xclient.data.l[1] 		= CurrentTime;
//		event.xclient.data.l[2] 		= m_window;
//
//		XSendEvent(m_display, m_rootWindow, 0, SubstructureNotifyMask | SubstructureRedirectMask, &event);
//
	}

	xdl_bool XdevLWindowX11::hasFocus() {
		Atom actualType;
		int actualFormat;
		unsigned long i, numItems, bytesAfter;
		unsigned char *propertyValue = nullptr;
		long maxLength = 1024;
		if(XGetWindowProperty(m_display, m_window, _NET_WM_STATE,
		                      0l, maxLength, False, XA_ATOM, &actualType,
		                      &actualFormat, &numItems, &bytesAfter,
		                      &propertyValue) == Success) {

			Atom *atoms = (Atom *) propertyValue;
			for(i = 0; i < numItems; ++i) {
				if(atoms[i] == _NET_WM_STATE_FOCUSED) {
					return xdl_true;
				}
			}
		}
		return xdl_false; // TODO This may cause problems.
	}

	void XdevLWindowX11::setParent(XdevLWindow* window) {
		XdevLWindowImpl::setParent(window);
	}

	xdl_int XdevLWindowX11::initializeEWMH() {

		_MOTIF_WM_HINTS = XInternAtom(m_display, "_MOTIF_WM_HINTS", False);
		if(None == _MOTIF_WM_HINTS) {
			XDEVL_MODULE_ERROR("Returning atom identifier _MOTIF_WM_HINTS failed.\n");
		}

		_NET_WM_WINDOW_TYPE = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE", False);
		if(None == _NET_WM_WINDOW_TYPE) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_WINDOW_TYPE failed.\n");
		}

		_NET_WM_WINDOW_TYPE_DESKTOP = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
		if(None == _NET_WM_WINDOW_TYPE_DESKTOP) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_WINDOW_TYPE_DESKTOP failed.\n");
		}

		_NET_WM_WINDOW_TYPE_DOCK = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_DOCK", False);
		if(None == _NET_WM_WINDOW_TYPE_DOCK) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_WINDOW_TYPE_DOCK failed.\n");
		}

		_NET_WM_WINDOW_TYPE_TOOLBAR = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_TOOLBAR", False);
		if(None == _NET_WM_WINDOW_TYPE_TOOLBAR) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_WINDOW_TYPE_TOOLBAR failed.\n");
		}

		_NET_WM_WINDOW_TYPE_MENU = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_MENU", False);
		if(None == _NET_WM_WINDOW_TYPE_MENU) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_WINDOW_TYPE_MENU failed.\n");
		}

		_NET_WM_WINDOW_TYPE_UTILITY = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_UTILITY", False);
		if(None == _NET_WM_WINDOW_TYPE_UTILITY) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_WINDOW_TYPE_UTILITY failed.\n");
		}

		_NET_WM_WINDOW_TYPE_SPLASH = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_SPLASH", False);
		if(None == _NET_WM_WINDOW_TYPE_SPLASH) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_WINDOW_TYPE_SPLASH failed.\n");
		}

		_NET_WM_WINDOW_TYPE_DIALOG = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_DIALOG", False);
		if(None == _NET_WM_WINDOW_TYPE_DIALOG) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_WINDOW_TYPE_DIALOG failed.\n");
		}

		_NET_WM_WINDOW_TYPE_DROPDOWN_MENU = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU", False);
		if(None == _NET_WM_WINDOW_TYPE_DROPDOWN_MENU) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_WINDOW_TYPE_DROPDOWN_MENU failed.\n");
		}

		_NET_WM_WINDOW_TYPE_POPUP_MENU = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_POPUP_MENU", False);
		if(None == _NET_WM_WINDOW_TYPE_POPUP_MENU) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_WINDOW_TYPE_POPUP_MENU failed.\n");
		}

		_NET_WM_WINDOW_TYPE_TOOLTIP = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_TOOLTIP", False);
		if(None == _NET_WM_WINDOW_TYPE_TOOLTIP) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_WINDOW_TYPE_TOOLTIP failed.\n");
		}

		_NET_WM_WINDOW_TYPE_NOTIFICATION = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_NOTIFICATION", False);
		if(None == _NET_WM_WINDOW_TYPE_NOTIFICATION) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_WINDOW_TYPE_NOTIFICATION failed.\n");
		}

		_NET_WM_WINDOW_TYPE_COMBO = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_COMBO", False);
		if(None == _NET_WM_WINDOW_TYPE_COMBO) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_WINDOW_TYPE_COMBO failed.\n");
		}

		_NET_WM_WINDOW_TYPE_DND = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_DND", False);
		if(None == _NET_WM_WINDOW_TYPE_DND) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_WINDOW_TYPE_DND failed.\n");
		}

		_NET_WM_WINDOW_TYPE_NORMAL = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_NORMAL", False);
		if(None == _NET_WM_WINDOW_TYPE_NORMAL) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_WINDOW_TYPE_NORMAL failed.\n");
		}

		_NET_WM_WINDOW_OPACITY = XInternAtom(m_display, "_NET_WM_WINDOW_OPACITY", False);
		if(None == _NET_WM_WINDOW_OPACITY) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_WINDOW_OPACITY failed.\n");
		}

		_NET_WM_BYPASS_COMPOSITOR = XInternAtom(m_display, "_NET_WM_BYPASS_COMPOSITOR", False);
		if(None == _NET_WM_BYPASS_COMPOSITOR) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_BYPASS_COMPOSITOR failed.\n");
		}

		_NET_WM_ACTION_RESIZE = XInternAtom(m_display, "_NET_WM_ACTION_RESIZE", False);
		if(None == _NET_WM_ACTION_RESIZE) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_ACTION_RESIZE failed.\n");
		}

		_NET_WM_STATE = XInternAtom(m_display, "_NET_WM_STATE", False);
		if(None == _NET_WM_STATE) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_STATE failed.\n");
		}

		_NET_WM_STATE_MAXIMIZED_VERT = XInternAtom(m_display, "_NET_WM_STATE_MAXIMIZED_VERT", False);
		if(None == _NET_WM_STATE_MAXIMIZED_VERT) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_STATE_MAXIMIZED_VERT failed.\n");
		}

		_NET_WM_STATE_MAXIMIZED_HORZ = XInternAtom(m_display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
		if(None == _NET_WM_STATE_MAXIMIZED_HORZ) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_STATE_MAXIMIZED_HORZ failed.\n");
		}

		_NET_WM_STATE_FULLSCREEN = XInternAtom(m_display, "_NET_WM_STATE_FULLSCREEN", False);
		if(None == _NET_WM_STATE_FULLSCREEN) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_STATE_FULLSCREEN failed.\n");
		}

		_NET_WM_STATE_HIDDEN = XInternAtom(m_display, "_NET_WM_STATE_HIDDEN", False);
		if(None == _NET_WM_STATE_HIDDEN) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_STATE_HIDDEN failed.\n");
		}

		_NET_WM_STATE_FOCUSED = XInternAtom(m_display, "_NET_WM_STATE_FOCUSED", False);
		if(None == _NET_WM_STATE_FOCUSED) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_STATE_FOCUSED failed.\n");
		}

		_NET_WM_NAME = XInternAtom(m_display, "_NET_WM_NAME", False);
		if(None == _NET_WM_NAME) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_NAME failed.\n");
		}

		_NET_WM_ICON_NAME = XInternAtom(m_display, "_NET_WM_ICON_NAME", False);
		if(None == _NET_WM_ICON_NAME) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_ICON_NAME failed.\n");
		}

		_NET_WM_ICON = XInternAtom(m_display, "_NET_WM_ICON", False);
		if(None == _NET_WM_ICON) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_ICON failed.\n");
		}

		_NET_WM_PING = XInternAtom(m_display, "_NET_WM_PING", False);
		if(None == _NET_WM_PING) {
			XDEVL_MODULE_ERROR("Returning atom identifier _NET_WM_PING failed.\n");
		}

		WM_STATE = XInternAtom(m_display, "WM_STATE", False);
		if(None == WM_STATE) {
			XDEVL_MODULE_ERROR("Returning atom identifier WM_STATE failed.\n");
		}

		WM_NAME = XInternAtom(m_display, "WM_NAME", False);
		if(None == WM_NAME) {
			XDEVL_MODULE_ERROR("Returning atom identifier WM_NAME failed.\n");
		}

		WM_NORMAL_HINTS = XInternAtom(m_display, "WM_NORMAL_HINTS", False);
		if(None == WM_NORMAL_HINTS) {
			XDEVL_MODULE_ERROR("Returning atom identifier WM_NORMAL_HINTS failed.\n");
		}

		WM_HINTS = XInternAtom(m_display, "WM_HINTS", False);
		if(None == WM_HINTS) {
			XDEVL_MODULE_ERROR("Returning atom identifier WM_HINTS failed.\n");
		}

		WM_ICON_SIZE = XInternAtom(m_display, "WM_ICON_SIZE", False);
		if(None == WM_ICON_SIZE) {
			XDEVL_MODULE_ERROR("Returning atom identifier WM_ICON_SIZE failed.\n");
		}

		WM_DELETE_WINDOW = XInternAtom(m_display, "WM_DELETE_WINDOW", False);
		if(None == WM_DELETE_WINDOW) {
			XDEVL_MODULE_ERROR("Returning atom identifier WM_DELETE_WINDOW failed.\n");
		}

		WM_PROTOCOLS = XInternAtom(m_display, "WM_PROTOCOLS", False);
		if(None == WM_PROTOCOLS) {
			XDEVL_MODULE_ERROR("Returning atom identifier WM_PROTOCOLS failed.\n");
		}

		WM_TRANSIENT_FOR = XInternAtom(m_display, "WM_TRANSIENT_FOR", False);
		if(None == WM_TRANSIENT_FOR) {
			XDEVL_MODULE_ERROR("Returning atom identifier WM_TRANSIENT_FOR failed.\n");
		}

		WM_CLASS = XInternAtom(m_display, "WM_CLASS", False);
		if(None == WM_CLASS) {
			XDEVL_MODULE_ERROR("Returning atom identifier WM_CLASS failed.\n");
		}

		XdndEnter = XInternAtom(m_display, "XdndEnter", False);
		if(None == XdndEnter) {
			XDEVL_MODULE_ERROR("Returning atom identifier XdndEnter failed.\n");
		}

		XdndPosition = XInternAtom(m_display, "XdndPosition", False);
		if(None == XdndPosition) {
			XDEVL_MODULE_ERROR("Returning atom identifier XdndPosition failed.\n");
		}

		XdndStatus = XInternAtom(m_display, "XdndStatus", False);
		if(None == XdndStatus) {
			XDEVL_MODULE_ERROR("Returning atom identifier XdndStatus failed.\n");
		}

		XdndTypeList = XInternAtom(m_display, "XdndTypeList", False);
		if(None == XdndTypeList) {
			XDEVL_MODULE_ERROR("Returning atom identifier XdndTypeList failed.\n");
		}

		XdndActionCopy = XInternAtom(m_display, "XdndActionCopy", False);
		if(None == XdndActionCopy) {
			XDEVL_MODULE_ERROR("Returning atom identifier XdndActionCopy failed.\n");
		}

		XdndDrop = XInternAtom(m_display, "XdndDrop", False);
		if(None == XdndDrop) {
			XDEVL_MODULE_ERROR("Returning atom identifier XdndDrop failed.\n");
		}

		XdndFinished = XInternAtom(m_display, "XdndFinished", False);
		if(None == XdndFinished) {
			XDEVL_MODULE_ERROR("Returning atom identifier XdndFinished failed.\n");
		}

		XdndSelection = XInternAtom(m_display, "XdndSelection", False);
		if(None == XdndSelection) {
			XDEVL_MODULE_ERROR("Returning atom identifier XdndSelection failed.\n");
		}

		_KDE_NET_WM_WINDOW_TYPE_OVERRIDE	= XInternAtom(m_display, "_KDE_NET_WM_WINDOW_TYPE_OVERRIDE", False);
		if(None == _KDE_NET_WM_WINDOW_TYPE_OVERRIDE) {
			XDEVL_MODULE_ERROR("Returning atom identifier _KDE_NET_WM_WINDOW_TYPE_OVERRIDE failed.\n");
		}

		// Now we are going to tell the WM in which protocols we are interested are or which one we support.

		m_atoms.push_back(_NET_WM_WINDOW_TYPE_DESKTOP);
		m_atoms.push_back(_NET_WM_ACTION_RESIZE);
		m_atoms.push_back(_NET_WM_WINDOW_TYPE);
		m_atoms.push_back(_NET_WM_WINDOW_TYPE_UTILITY);
		m_atoms.push_back(_NET_WM_WINDOW_OPACITY);
		m_atoms.push_back(_NET_WM_STATE);
		m_atoms.push_back(_NET_WM_STATE_MAXIMIZED_VERT);
		m_atoms.push_back(_NET_WM_STATE_MAXIMIZED_HORZ);
		m_atoms.push_back(_NET_WM_STATE_FULLSCREEN);
		m_atoms.push_back(_NET_WM_STATE_HIDDEN);
		m_atoms.push_back(_NET_WM_STATE_FOCUSED);
		m_atoms.push_back(_NET_WM_STATE_ADD);
		m_atoms.push_back(_NET_WM_STATE_REMOVE);
		m_atoms.push_back(_NET_WM_NAME);
		m_atoms.push_back(_NET_WM_ICON_NAME);
		m_atoms.push_back(_NET_WM_ICON);
		m_atoms.push_back(WM_STATE);
		m_atoms.push_back(WM_ICON_SIZE);
		m_atoms.push_back(WM_DELETE_WINDOW);
		m_atoms.push_back(WM_PROTOCOLS);
		m_atoms.push_back(_MOTIF_WM_HINTS);

		m_atoms.push_back(XdndEnter);
		m_atoms.push_back(XdndPosition);
		m_atoms.push_back(XdndStatus);
		m_atoms.push_back(XdndTypeList);
		m_atoms.push_back(XdndActionCopy);
		m_atoms.push_back(XdndDrop);
		m_atoms.push_back(XdndFinished);
		m_atoms.push_back(XdndSelection);

		// Set the supported protocols for this window.
		XSetWMProtocols(m_display, m_window, m_atoms.data(), m_atoms.size());

		return ERR_OK;
	}

	void XdevLWindowX11::setType(XdevLWindowTypes type) {

		switch(type) {

			case XDEVL_WINDOW_TYPE_NORMAL: {
				if(_NET_WM_WINDOW_TYPE_NORMAL != 0) {
					XChangeProperty(m_display, m_window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32,PropModeReplace, (unsigned char *)&_NET_WM_WINDOW_TYPE_NORMAL, 1);
				} else {
				}
			}
			break;
			case XDEVL_WINDOW_TYPE_TOOLTIP: {
				if(_NET_WM_WINDOW_TYPE_TOOLTIP != 0) {
					XChangeProperty(m_display, m_window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32,PropModeReplace, (unsigned char *)&_NET_WM_WINDOW_TYPE_TOOLTIP, 1);
				} else {
				}
			}
			break;
			case XDEVL_WINDOW_TYPE_DROPDOWN_MENU: {
				if(_NET_WM_WINDOW_TYPE_DROPDOWN_MENU != 0) {
					XChangeProperty(m_display, m_window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32,PropModeReplace, (unsigned char *)&_NET_WM_WINDOW_TYPE_DROPDOWN_MENU, 1);
				} else {
				}
			}
			break;
			case XDEVL_WINDOW_TYPE_SPLASH: {
				if(_NET_WM_WINDOW_TYPE_SPLASH != 0) {
					XChangeProperty(m_display, m_window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32,PropModeReplace, (unsigned char *)&_NET_WM_WINDOW_TYPE_SPLASH, 1);
				} else {
				}
			}
			break;
			case XDEVL_WINDOW_TYPE_UNKNOWN:
			default:
				break;
		}
		XFlush(m_display);
	}

	Display* XdevLWindowX11::getNativeDisplay() {
		return m_display;
	}

	Window XdevLWindowX11::getNativeWindow() {
		return m_window;
	}

	Window XdevLWindowX11::getNativeRootWindow() {
		return m_rootWindow;
	}

//
// -------------------------------------------------------------------------
//

	XdevLWindowServerX11::XdevLWindowServerX11(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor) :
		XdevLWindowServerImpl(parameter, desriptor) {

	}

	XdevLWindowServerX11::~XdevLWindowServerX11() {

	}

	xdl_int XdevLWindowServerX11::createWindow(XdevLWindow** window,
	    const XdevLWindowTitle& title,
	    const XdevLWindowPosition& position,
	    const XdevLWindowSize& size,
	    const XdevLWindowTypes& type) {

		XdevLWindowX11* wnd = new XdevLWindowX11(nullptr, getDescriptor());
		wnd->create(title, position, size, type);
		*window = wnd;
		m_windowList[wnd->getWindowID()] = wnd;
		return ERR_OK;
	}



//
// -----------------------------------------------------------------------------
//

	XdevLWindowEventServerX11::XdevLWindowEventServerX11(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor) :
		XdevLWindowEventServerImpl(parameter, desriptor),
		m_focusWindow(nullptr),
		m_keyboard(nullptr) {
	}

	XdevLWindowEventServerX11::~XdevLWindowEventServerX11() {

	}


	xdl_int XdevLWindowEventServerX11::registerWindowForEvents(XdevLWindow* window) {
		return XdevLWindowEventServerImpl::registerWindowForEvents(window);
	}

	xdl_int XdevLWindowEventServerX11::unregisterWindowFromEvents(XdevLWindow* window) {
		return XdevLWindowEventServerImpl::unregisterWindowFromEvents(window);
	}

	xdl_int XdevLWindowEventServerX11::init() {

		m_keyboard = new XdevLWindowX11Keyboard(display, getMediator());

		WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", False);
		WM_PROTOCOLS = XInternAtom(display, "WM_PROTOCOLS", False);
		_NET_WM_PING = XInternAtom(display, "_NET_WM_PING", False);

		return ERR_OK;
	}

	void* XdevLWindowEventServerX11::getInternal(const XdevLInternalName& id) {

		return nullptr;
	}

	xdl_int XdevLWindowEventServerX11::shutdown() {

		// Free the keyboard device.
		if(m_keyboard != nullptr) {
			delete m_keyboard;
			m_keyboard = nullptr;
		}
		return ERR_OK;
	}

	xdl_int XdevLWindowEventServerX11::update() {
		return pollEvents();
	}

	xdl_int XdevLWindowEventServerX11::pollEvents() {

		// Only run this while loop when we have events in the queue.
		while(XPending(display) > 0) {
			XEvent event;

			// Get next event.
			XNextEvent(display, &event);

			//
			// Handle generic events like xinput2, xfixes etc.
			//
			if(event.type == GenericEvent) {
				XGenericEventCookie* cookie = &event.xcookie;
				XGetEventData(display, cookie);

				if(globalDisplay->getCursor()) {
					// TODO Don't cast here. Find another way.
					globalDisplay->getCursor()->onHandleXinputEvent(cookie, m_focusWindow);
				}

				XFreeEventData(display, cookie);
				continue;
			}

			// Handle window events.
			XdevLWindowX11* window = static_cast<XdevLWindowX11*>(getWindow(event.xany.window));
			if(window == nullptr) {
				// We didn't find the window in the list. That means we are not managing it so
				// we skip this event.
				continue;
			}

			XdevLEvent ev;
			ev.common.timestamp = getMediator()->getTimer().getTime64();

			// Check which event type we've got.
			switch(event.type) {

					//
					// Will be send if window was created.
					//
				case CreateNotify: {

					ev.type				= XDEVL_WINDOW_EVENT;
					ev.window.event 	= XDEVL_WINDOW_CREATE;
					ev.window.windowid	= window->getWindowID();

					getMediator()->fireEvent(ev);
				}
				break;

				//
				// Keyboard pressed.
				//
				case KeyPress: {
					xdl_uint8 repeat = 0;
					m_keyboard->sendKeyboardEvent(ButtonPressed.getHashCode(), event.xkey.keycode, repeat, window->getWindowID());
				}
				break;

				//
				// Keyboard released.
				//
				case KeyRelease: {

					//
					// Check if the auto repeat is on.
					//
					xdl_uint8 repeat = 0;
					if(XPending(window->getNativeDisplay())) {
						XEvent nev;
						XPeekEvent(window->getNativeDisplay(), &nev);

						if(nev.type == KeyPress && nev.xkey.time == event.xkey.time && nev.xkey.keycode == event.xkey.keycode) {
							repeat = 1;
						}
					}
					m_keyboard->sendKeyboardEvent(ButtonReleased.getHashCode(), event.xkey.keycode, repeat, window->getWindowID());
				}
				break;

				//
				// Mouse pointer moved.
				//
				case MotionNotify: {
					if(globalDisplay->getCursor()->isRelativeMotionEnabled() == xdl_false) {
						ev.type 						= MouseMotion.getHashCode();
						ev.motion.x					= (2.0 / window->getWidth()*event.xmotion.x - 1.0f) * 32768.0f;
						ev.motion.y					= -(2.0 / window->getHeight() * event.xmotion.y - 1.0f) * 32768.0f;
						ev.motion.windowid 	= window->getWindowID();
						getMediator()->fireEvent(ev);
					}
				}
				break;

				//
				// Mouse button pressed.
				//
				case ButtonPress: {

					ev.type 				= MouseButtonPressed.getHashCode();
					ev.button.button		= event.xbutton.button;
					ev.button.x				= event.xbutton.x;
					ev.button.y				= event.xbutton.y;
					ev.window.windowid		= window->getWindowID();

					getMediator()->fireEvent(ev);

				}
				break;

				//
				// Mouse button released.
				//
				case ButtonRelease : {

					ev.type 				= MouseButtonReleased.getHashCode();
					ev.button.button		= event.xbutton.button;
					ev.button.x				= event.xbutton.x;
					ev.button.y				= event.xbutton.y;
					ev.window.windowid		= window->getWindowID();

					getMediator()->fireEvent(ev);
				}
				break;

				//
				//
				//
				case SelectionRequest: {

					int seln_format;
					unsigned long nbytes;
					unsigned long overflow;
					unsigned char *seln_data;
					XEvent sevent;
					sevent.xany.type = SelectionNotify;
					sevent.xselection.selection = event.xselectionrequest.selection;
					sevent.xselection.target = None;
					sevent.xselection.property = None;
					sevent.xselection.requestor = event.xselectionrequest.requestor;
					sevent.xselection.time = event.xselectionrequest.time;

					// Check if  we have a cut buffer.
					if(XGetWindowProperty(display, window->getNativeRootWindow(),
					                      XA_CUT_BUFFER0, 0, INT_MAX/4, False, event.xselectionrequest.target,
					                      &sevent.xselection.target, &seln_format, &nbytes,
					                      &overflow, &seln_data) == Success) {

						Atom XA_TARGETS = XInternAtom(display, "TARGETS", 0);
						if(sevent.xselection.target == event.xselectionrequest.target) {
							XChangeProperty(display, event.xselectionrequest.requestor, event.xselectionrequest.property,
							                sevent.xselection.target, seln_format, PropModeReplace,
							                seln_data, nbytes);
							sevent.xselection.property = event.xselectionrequest.property;
						} else if(XA_TARGETS == event.xselectionrequest.target) {
							Atom SupportedFormats[] = { sevent.xselection.target, XA_TARGETS };
							XChangeProperty(display, event.xselectionrequest.requestor, event.xselectionrequest.property,
							                XA_ATOM, 32, PropModeReplace,
							                (unsigned char*)SupportedFormats,
							                sizeof(SupportedFormats)/sizeof(*SupportedFormats));
							sevent.xselection.property = event.xselectionrequest.property;
						}
						XFree(seln_data);
					}
					XSendEvent(display, event.xselectionrequest.requestor, False, 0, &sevent);
					XSync(display, False);

				}
				break;

				//
				//
				//
				case SelectionNotify: {
//					if(window) {
//
//						Atom target = event.xselection.target;
//						//	if (target == data->xdnd_req) {
//						//}
//					}
//					break;
//				default: {
//					switch(event.type - m_event_basep) {
//						case RRScreenChangeNotify: {
//							// Show XRandR that we really care
//							XRRUpdateConfiguration(&event);
//						}
//						break;
//					}
				} break;

				//
				// The mouse entered the window area.
				//
				case EnterNotify: {

					ev.type					= XDEVL_WINDOW_EVENT;
					ev.window.event 		= XDEVL_WINDOW_ENTER;
					ev.window.data1			= event.xcrossing.x;
					ev.window.data2			= event.xcrossing.y;
					ev.window.windowid		= window->getWindowID();

					getMediator()->fireEvent(ev);
				}
				break;

				//
				// The mouse left the window area.
				//
				case LeaveNotify: {

					ev.type					= XDEVL_WINDOW_EVENT;
					ev.window.event 		= XDEVL_WINDOW_LEAVE;
					ev.window.data1			= event.xcrossing.x;
					ev.window.data2			= event.xcrossing.y;
					ev.window.windowid		= window->getWindowID();

					getMediator()->fireEvent(ev);
				}
				break;

				//
				// Input focus gained.
				//
				case FocusIn: {
					m_focusWindow = window;

					ev.type					= XDEVL_WINDOW_EVENT;
					ev.window.event 		= XDEVL_WINDOW_INPUT_FOCUS_GAINED;
					ev.window.windowid		= window->getWindowID();

					focusGained(window);

					getMediator()->fireEvent(ev);

				}
				break;

				//
				// Input focus lost focus.
				//
				case FocusOut: {
					m_focusWindow = window;

					ev.type					= XDEVL_WINDOW_EVENT;
					ev.window.event 		= XDEVL_WINDOW_INPUT_FOCUS_LOST;
					ev.window.windowid		= window->getWindowID();

					getMediator()->fireEvent(ev);
				}
				break;

				//
				// Window was hidden
				//
				case UnmapNotify: {

					ev.type					= XDEVL_WINDOW_EVENT;
					ev.window.event 		= XDEVL_WINDOW_HIDDEN;
					ev.window.windowid		= window->getWindowID();

					getMediator()->fireEvent(ev);
				}
				break;

				//
				// Window was shown.
				//
				case MapNotify: {

					ev.type					= XDEVL_WINDOW_EVENT;
					ev.window.event 		= XDEVL_WINDOW_SHOWN;
					ev.window.windowid		= window->getWindowID();

					getMediator()->fireEvent(ev);

				}
				break;

				case ClientMessage: {
					if((event.xclient.message_type == WM_PROTOCOLS) &&
					    ((Atom)event.xclient.data.l[0] == WM_DELETE_WINDOW) &&
					    (event.xclient.format == 32)) {

						ev.type					= XDEVL_WINDOW_EVENT;
						ev.window.event 		= XDEVL_WINDOW_CLOSE;
						ev.window.windowid		= window->getWindowID();

						getMediator()->fireEvent(ev);

						// Make a core event.
						ev.type					= XDEVL_CORE_EVENT;
						ev.core.event 			= XDEVL_CORE_SHUTDOWN;

						getMediator()->fireEvent(ev);

					}
					//
					// Desktop Systems. send a ping to check if the window is still active.
					// This must be handled otherwise the Desktop System will complain or it
					// will make the window dark or what ever.
					//
					else if((event.xclient.message_type == WM_PROTOCOLS) &&
					        (event.xclient.format == 32) &&
					        ((Atom)event.xclient.data.l[0] == _NET_WM_PING)) {
						Window root = DefaultRootWindow(display);
						event.xclient.window = root;
						XSendEvent(display, root, False, SubstructureRedirectMask | SubstructureNotifyMask, &event);
					}
				}
				break;

				//
				// Window got destroyed.
				//
				case DestroyNotify: {
					// TODO Do we have to do something here?
				}
				break;

				//
				// Part of the window changed. Notify
				//
				case Expose: {
					ev.type					= XDEVL_WINDOW_EVENT;
					ev.window.event 		= XDEVL_WINDOW_EXPOSED;
					ev.button.windowid		= window->getWindowID();

					getMediator()->fireEvent(ev);
				}
				break;

				//
				// Resized or moved
				//
				case ConfigureNotify: {
					if((event.xconfigure.x != window->getX()) || (event.xconfigure.y != window->getY())) {

						ev.type					= XDEVL_WINDOW_EVENT;
						ev.window.event 		= XDEVL_WINDOW_MOVED;
						ev.window.x				= event.xconfigure.x;
						ev.window.y				= event.xconfigure.y;
						ev.window.width			= event.xconfigure.width;
						ev.window.height		= event.xconfigure.height;
						ev.button.windowid		= window->getWindowID();

						getMediator()->fireEvent(ev);
					}
					if((event.xconfigure.width != window->getWidth()) || (event.xconfigure.height != window->getHeight())) {
						ev.type					= XDEVL_WINDOW_EVENT;
						ev.window.event 		= XDEVL_WINDOW_RESIZED;
						ev.window.x				= event.xconfigure.x;
						ev.window.y				= event.xconfigure.y;
						ev.window.width			= event.xconfigure.width;
						ev.window.height		= event.xconfigure.height;
						ev.window.windowid		= window->getWindowID();

						getMediator()->fireEvent(ev);

					}
				}
				break;

				//
				//
				//
				case KeymapNotify: {
					// TODO Something to do here?
				}
				break;

				//
				// Keyboard mapping changed.
				//
				case MappingNotify: {
					//std::cout << "MappingNotify" << std::endl;
					//	m_keyboard->updateKeyboardMap();
				}
				break;

			}
			//	}

		}

		return ERR_OK;
	}

	void XdevLWindowEventServerX11::flush() {
		pollEvents();
	}

//
// -----------------------------------------------------------------------------
//

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
