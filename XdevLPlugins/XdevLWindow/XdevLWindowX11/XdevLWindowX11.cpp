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

#define _NET_WM_STATE_REMOVE    0l
#define _NET_WM_STATE_ADD       1l
#define _NET_WM_STATE_TOGGLE    2l

//
// The XdevLWindow plugin descriptor.
//
xdl::XdevLPluginDescriptor windowX11PluginDescriptor {
	xdl::windowX11PluginName,
	xdl::window_moduleNames,
	xdl::XdevLWindowPluginMajorVersion,
	xdl::XdevLWindowPluginMinorVersion,
	xdl::XdevLWindowPluginPatchVersion
};

//
// The XdevLWindow module descriptor.
//
xdl::XdevLModuleDescriptor windowX11ModuleDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_WINDOW_MODULE_NAME],
	xdl::window_copyright,
	xdl::window_x11_description,
	xdl::XdevLWindowMajorVersion,
	xdl::XdevLWindowMinorVersion,
	xdl::XdevLWindowPatchVersion
};

//
// The XdevLWindowEventServer module descriptor.
//
xdl::XdevLModuleDescriptor windowEventServerModuleDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_WINDOW_EVENT_SERVER_MODULE_NAME],
	xdl::window_copyright,
	xdl::windowServerDescription,
	xdl::XdevLWindowEventServerMajorVersion,
	xdl::XdevLWindowEventServerMinorVersion,
	xdl::XdevLWindowEventServerPatchVersion
};

//
// The XdevLCursor module descriptor.
//
xdl::XdevLModuleDescriptor cursorModuleDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_CURSOR_MODULE_NAME],
	xdl::window_copyright,
	xdl::windowServerDescription,
	xdl::XdevLWindowEventServerMajorVersion,
	xdl::XdevLWindowEventServerMinorVersion,
	xdl::XdevLWindowEventServerPatchVersion
};

static xdl::XdevLCursorX11* x11cursor = nullptr;
static Display* globalDisplay = nullptr;
static Colormap defaultColorMap;


extern "C" XDEVL_EXPORT xdl::xdl_int _init_plugin(xdl::XdevLPluginCreateParameter* parameter) {

	// Start X server with thread support.
	XInitThreads();

	// Connect to X server.
	if(globalDisplay == nullptr) {
		globalDisplay = XOpenDisplay(nullptr);
		if(globalDisplay == nullptr) {
			return xdl::ERR_ERROR;
		}

		// Print out some useless information :D
		std::clog << "\n---------------------------- X11 Server Information ----------------------------\n";
		std::clog << "Vendor              : " << XServerVendor(globalDisplay) << "\n";
		std::clog << "Release             : " << XVendorRelease(globalDisplay)<< "\n";
		std::clog << "Number of Screens   : " << XScreenCount(globalDisplay) 	<< std::endl;

		// If there is not event server first create one.
		if(xdl::windowEventServer == nullptr) {
			// If there is no even server active, create and activate it.
			xdl::windowEventServer = static_cast<xdl::XdevLWindowX11EventServer*>(parameter->getMediator()->createModule(xdl::XdevLModuleName("XdevLWindowEventServer"), xdl::XdevLID("XdevLWindowEventServer")));
		}

		if(xdl::cursor == nullptr) {
			xdl::cursor = static_cast<xdl::XdevLCursor*>(parameter->getMediator()->createModule(xdl::XdevLModuleName("XdevLCursor"), xdl::XdevLID("XdevLCursor")));
		}

	}
	return xdl::ERR_OK;
}

extern "C" XDEVL_EXPORT xdl::xdl_int _shutdown_plugin() {

	// And now close the window.
	if(globalDisplay) {
		XCloseDisplay(globalDisplay);
		globalDisplay = nullptr;
	}

	return xdl::ERR_OK;
}


extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter)  {

	//
	// Create XdevLWindow instance.
	//
	if(windowX11ModuleDesc.getName() == parameter->getModuleName()) {

		xdl::XdevLWindowX11* window = new xdl::XdevLWindowX11(parameter);
		parameter->setModuleInstance(window);
	}

	//
	// Create XdevLWindowServer instance.
	//
	else if(xdl::XdevLWindowServerImpl::m_windowServerModuleDesc.getName() == parameter->getModuleName()) {

		xdl::XdevLWindowServerX11* windowServer = new xdl::XdevLWindowServerX11(parameter);
		parameter->setModuleInstance(windowServer);
	}

	//
	// Create XdevLEventServer instance.
	//
	else if(windowEventServerModuleDesc.getName() == parameter->getModuleName()) {
		if(xdl::windowEventServer == nullptr) {
			xdl::windowEventServer = new xdl::XdevLWindowX11EventServer(parameter);
			xdl::XdevLWindowEventServerParameter = parameter;
		}
		parameter->setModuleInstance(xdl::windowEventServer);
	}

	//
	// Create XdevLCursor instance.
	//
	else if(cursorModuleDesc.getName() == parameter->getModuleName()) {
		x11cursor =  new xdl::XdevLCursorX11(parameter);
		xdl::cursor = x11cursor;
		parameter->setModuleInstance(xdl::cursor);

	} else {
		return xdl::ERR_MODULE_NOT_FOUND;
	}

	return xdl::ERR_OK;
}

extern "C" XDEVL_EXPORT void _delete(xdl::XdevLModule* obj) {
	if(obj)
		delete obj;
}

extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor()  {
	return &windowX11PluginDescriptor;
}


namespace xdl {

	const XdevLID ButtonPressed("XDEVL_BUTTON_PRESSED");
	const XdevLID ButtonReleased("XDEVL_BUTTON_RELEASED");
	const XdevLID MouseButtonPressed("XDEVL_MOUSE_BUTTON_PRESSED");
	const XdevLID MouseButtonReleased("XDEVL_MOUSE_BUTTON_RELEASED");
	const XdevLID MouseMotion("XDEVL_MOUSE_MOTION");
	const XdevLID MouseMotionRelative("XDEVL_MOUSE_MOTION_RELATIVE");
	const XdevLID WindowEvent("XDEVL_WINDOW_EVENT");


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


	XdevLWindowX11::XdevLWindowX11(XdevLModuleCreateParameter* parameter) :
		XdevLWindowImpl(XdevLWindowImpl::getWindowsCounter(), parameter, windowX11ModuleDesc),
		m_display(nullptr),
		m_rootWindow(0),
		m_window(0),
		m_screenNumber(0),
		m_screenWidth(0),
		m_screenHeight(0),
		m_best_fit_width(-1),
		m_best_fit_height(-1),
		m_best_fit_rate(-1),
		m_fullscreenModeActive(xdl_false),
		m_originalScreenConfig(nullptr) {

	}

	XdevLWindowX11::~XdevLWindowX11() {

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

		Visual* 						visual;
		XVisualInfo*					vinfo;

		m_display = globalDisplay;

		// Get the default screen number.
		m_screenNumber = DefaultScreen(globalDisplay);

		// Get the default screen with.
		m_screenWidth = DisplayWidth(globalDisplay, m_screenNumber);

		// Get the default screen height.
		m_screenHeight = DisplayHeight(globalDisplay,m_screenNumber);

		// Get the root window.
		m_rootWindow = RootWindow(globalDisplay, m_screenNumber);

		// Get the default color map.
		m_defaultColorMap = DefaultColormap(globalDisplay, m_screenNumber);

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

		// Check if the RandR extension is avaible.
		if(!XRRQueryVersion(globalDisplay, &m_randrMajor, &m_randrMinor)) {
			XDEVL_MODULE_ERROR("RandR extension missing.\n");
			return ERR_ERROR;
		}

		Colormap colormap = m_defaultColorMap;

		XRRQueryExtension(globalDisplay, &m_event_basep, &m_error_basep);

		if(m_rootTitle.toString().size() > 0) {
			XdevLWindow* rootWindow = static_cast<XdevLWindow*>(getMediator()->getModule(XdevLID(m_rootTitle.toString().c_str())));
			m_rootWindow = (Window)(rootWindow->getInternal(XdevLInternalName("X11_WINDOW")));
		}

		xdl_int borderwith = 0;
		xdl_int hasNotDecoration = xdl_false;
		XSetWindowAttributes WindowAttributes;
		WindowAttributes.override_redirect	= False;

		//
		// What shall we do with the override_redirect flag? Shall we use it for popups, tooltips etc.
		// If we use it then the Window Manager has no influence on this window which can cause issues.
		if(	(m_attribute.type == XDEVL_WINDOW_TYPE_TOOLTIP) ||
		    (m_attribute.type == XDEVL_WINDOW_TYPE_POPUP) ||
		    (m_attribute.type == XDEVL_WINDOW_TYPE_SPLASH) ||
		    (m_attribute.type == XDEVL_WINDOW_TYPE_NOTIFICATION)) {
			hasNotDecoration = xdl_true;
//			WindowAttributes.override_redirect	= True;
		} else {
//			WindowAttributes.override_redirect	= False;
		}

		WindowAttributes.background_pixmap = None;
		WindowAttributes.background_pixel = color.pixel;
		WindowAttributes.border_pixel = 0;
		WindowAttributes.colormap = CopyFromParent;

		m_window = XCreateWindow(globalDisplay,
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


		XSelectInput(globalDisplay, m_window, event_mask);

		XClearWindow(globalDisplay, m_window);

		//
		// Set the WM hints
		//
		XWMHints* wmHints 	= XAllocWMHints();
		wmHints->flags = 	StateHint | // We want to set the initial state of this window (NormalState)
		                  InputHint;  // We want to set the input focus model.
		wmHints->input = True;          // Does this application rely on the window manager to get keyboard input?
		wmHints->initial_state = NormalState; // Most applications start this way. WithdrawnState would hide the window and IconicState would start as icon.
		XSetWMHints(globalDisplay, m_window, wmHints);
		XFree(wmHints);

		// Set title of the window.
		setTitle(m_attribute.title);

		// Set type of the window.
		setType(m_attribute.type);

		// Check if the user specified fullscreen mode.
		if(getFullscreen()) {
			setFullscreenVideoMode();
		}

		const long _NET_WM_BYPASS_COMPOSITOR_HINT_ON = 1;
		XChangeProperty(globalDisplay, m_window, _NET_WM_BYPASS_COMPOSITOR, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&_NET_WM_BYPASS_COMPOSITOR_HINT_ON, 1);

		// Tell the Window Manager to show or hide the decorations.
		if(hasNotDecoration) {
			disableDecoration();
		}

		XFlush(globalDisplay);

		//	initKeyboardKeyCodes();

		m_id = m_window;

		windowEventServer->registerWindowForEvents(this);

		XMapRaised(globalDisplay, m_window);
		XSync(globalDisplay, False);

		return ERR_OK;
	}

	void* XdevLWindowX11::getInternal(const XdevLInternalName& id) {
		std::string data(id);
		if(data == "X11_DISPLAY")
			return globalDisplay;
		if(data == "X11_WINDOW")
			return (void*)m_window;
		if(data == "X11_SCREEN_NUMBER")
			return &m_screenNumber;
		return nullptr;
	}

	xdl_int XdevLWindowX11::shutdown() {

		XDEVL_MODULE_INFO("Starting shutdown process.\n");

		// Send a more or less random value to unblock the
		// XNextEvent functions in the thread.

		XEvent ev;
		memset(&ev, 0, sizeof(ev));

		ev.xclient.type = ClientMessage;
		ev.xclient.window = m_window;
		ev.xclient.message_type = XInternAtom(globalDisplay, "WM_PROTOCOLS", true);
		ev.xclient.format = 32;
		ev.xclient.data.l[0] = XInternAtom(globalDisplay, "WM_DELETE_WINDOW", false);
		ev.xclient.data.l[1] = CurrentTime;
		XSendEvent(globalDisplay, m_window, False, NoEventMask, &ev);

		// Ok, check if the are in fullscreen mode.
		if(m_fullscreenModeActive)  {
			restoreFullscreenVideoMode();
		}

		Atom atom;
		atom = XInternAtom(globalDisplay, "_WIN_HINTS", True);
		if(atom != None) {
			XDeleteProperty(globalDisplay, m_window, atom);
		}

		// We have to destroy the window. The X-Server can do it automatically but it's better
		// to do it manually.
		if(globalDisplay && m_window && !m_rootWindow)
			XDestroyWindow(globalDisplay, m_window);

		if(XdevLWindowImpl::shutdown() != ERR_OK) {
			return ERR_ERROR;
		}

		XDEVL_MODULE_SUCCESS("Shutdown process was successful.\n");

		return ERR_OK;
	}

	int XdevLWindowX11::update() {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");
		return ERR_OK;
	}



	xdl_int XdevLWindowX11::getGetClosestVideoMode() {
		int i, match;
		int ratecount;

		// Get current resolution and frequency.
		m_originalScreenConfig 	= XRRGetScreenInfo(globalDisplay, m_rootWindow);
		m_originalScreenRate 	= XRRConfigCurrentRate(m_originalScreenConfig);
		m_originalSizeId		= XRRConfigCurrentConfiguration(m_originalScreenConfig, &m_originalRotation);

		// Get all supported screen sizes
		xdl_int numberOfScreenSizes;
		XRRScreenSize* sizelist = XRRConfigSizes(m_originalScreenConfig, &numberOfScreenSizes);

		// Find the best matching mode
		m_bestSizeId  = -1;
		xdl_int bestmatch = 999999;
		for(int i = 0; i < numberOfScreenSizes; i++) {
			match = (getWidth() - sizelist[i].width) * (getWidth() - sizelist[i].width) + (getHeight() - sizelist[i].height) * (getHeight() - sizelist[i].height);
			if(match < bestmatch) {
				bestmatch = match;
				m_bestSizeId = i;
			}
		}

		if(m_bestSizeId != -1) {
			// Report width & height of best matching mode
			m_best_fit_width = sizelist[m_bestSizeId].width;
			m_best_fit_height = sizelist[m_bestSizeId].height;
			if(m_originalScreenRate > 0) {
				short* ratelist = XRRConfigRates(m_originalScreenConfig, m_bestSizeId, &ratecount);

				m_best_fit_rate = -1;
				bestmatch = 999999;
				for(i = 0; i < ratecount; i++) {
					match = abs(ratelist[i] - m_originalScreenRate);
					if(match < bestmatch) {
						bestmatch = match;
						m_best_fit_rate = ratelist[i];
					}
				}
			}
		}

		if(m_bestSizeId != -1) {
			XDEVL_MODULE_INFO("Found matching screen size: " << sizelist[m_bestSizeId].width << "x" << sizelist[m_bestSizeId].height << " " << m_best_fit_rate << " Hz" << std::endl);
			return m_bestSizeId;
		}

		return 0;
	}

	void XdevLWindowX11::displayFromNormalToFullscreen() {
		// Disable window manager decoration.
		disableDecoration();

		// Check for the closest video mode.
		getGetClosestVideoMode();

		// Set the video mode.
		xdl_int rate;
		if(m_best_fit_rate == -1)
			rate = m_originalScreenRate;
		else
			rate = m_best_fit_rate;

		// Change screen mode.
		XRRSetScreenConfigAndRate(globalDisplay,
		                          m_originalScreenConfig,
		                          m_rootWindow,
		                          m_bestSizeId,
		                          m_originalRotation,
		                          (short int)rate,
		                          CurrentTime);

		//
		// Store old screen saver settings and disable it.
		//
		XGetScreenSaver(globalDisplay, &timeout_return, &interval_return, &prefer_blanking_return, &allow_exposures_return);
		XSetScreenSaver(globalDisplay, 0, 0, DontPreferBlanking, DefaultExposures);

		XSync(globalDisplay, False);
	}

	void XdevLWindowX11::displayFromFullscreenToNormal() {

		// Change screen mode into it's original form.
		XRRSetScreenConfigAndRate(globalDisplay,
		                          m_originalScreenConfig,
		                          m_rootWindow,
		                          m_originalSizeId,
		                          m_originalRotation,
		                          m_originalScreenRate,
		                          CurrentTime);

		// Set the previous screen saver settings.
		XSetScreenSaver(globalDisplay, timeout_return, interval_return, prefer_blanking_return, allow_exposures_return);

		// If we have changed the screen configuration, let's switch back.
		if(m_originalScreenConfig) {
			XRRFreeScreenConfigInfo(m_originalScreenConfig);
		}

		// Enable disabled window manager decoration.
		enableDecoration();

		XSync(globalDisplay, False);
	}

	void XdevLWindowX11::setFullscreenVideoMode() {
		// Don't do this if we already in fullscreen mode.
		if(xdl_false != m_fullscreenModeActive) {
			return;
		}

		displayFromNormalToFullscreen();

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

		if(XSendEvent(globalDisplay, m_rootWindow,  False, SubstructureNotifyMask | SubstructureRedirectMask, &msg) == 0) {

		}
		XFlush(globalDisplay);

		m_fullscreenModeActive = xdl_true;
	}

	void XdevLWindowX11::restoreFullscreenVideoMode() {
		// Only to this if we are in fullscreen mode.
		if(xdl_false == m_fullscreenModeActive) {
			return;
		}

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

		if(XSendEvent(globalDisplay, m_rootWindow,  False, SubstructureNotifyMask | SubstructureRedirectMask, &msg) == 0) {

		}
		XFlush(globalDisplay);
		
		displayFromFullscreenToNormal();
		
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

			XChangeProperty(globalDisplay, m_window, _MOTIF_WM_HINTS, _MOTIF_WM_HINTS, 32, PropModeReplace, (unsigned char *)&MWMHints, sizeof(MWMHints)/4);
			decorations_removed = xdl_true;
		}

		hintAtom = XInternAtom(globalDisplay, "KWM_WIN_DECORATION", True);
		if(hintAtom != None) {
			long KWMHints = KDE_tinyDecoration;

			XChangeProperty(globalDisplay,m_window, hintAtom, hintAtom, 32, PropModeReplace, (unsigned char *)&KWMHints,  sizeof(KWMHints)/4);
			decorations_removed = xdl_true;
		}

		// Now try to set GNOME hints
		hintAtom = XInternAtom(globalDisplay, "_WIN_HINTS", True);
		if(hintAtom != None) {
			long GNOMEHints = 0;

			XChangeProperty(globalDisplay, m_window, hintAtom, hintAtom, 32, PropModeReplace, (unsigned char *)&GNOMEHints, sizeof(GNOMEHints)/4);
			decorations_removed = xdl_true;
		}

		// Now try to set KDE NET_WM hints
		if(_NET_WM_WINDOW_TYPE != None) {
			Atom NET_WMHints[2] = {_KDE_NET_WM_WINDOW_TYPE_OVERRIDE, _NET_WM_WINDOW_TYPE_NORMAL};

			XChangeProperty(globalDisplay, m_window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32, PropModeReplace, (unsigned char *)&NET_WMHints, 2);
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

			if(XSendEvent(globalDisplay, DefaultRootWindow(globalDisplay),  False, SubstructureNotifyMask | SubstructureRedirectMask, &msg) == 0) {
				XDEVL_MODULE_WARNING("Could not add _NET_WM_STATE_FULLSCREEN state.\n");
			}
		}

		// Did we sucessfully remove the window decorations?
		if(decorations_removed) {
			// Finally set the transient hints
			XSetTransientForHint(globalDisplay, m_window, m_rootWindow);
			XRaiseWindow(globalDisplay, m_window);
		} else {
			// Seems like we couldn't remove the decoration using EWMH hints. Let's try to change they window into a override_redirect window
			// which means the Window Manager is supposed to ignore this window totally.
			XSetWindowAttributes  attributes;
			attributes.override_redirect = True;
			XChangeWindowAttributes(globalDisplay, m_window, CWOverrideRedirect, &attributes);
		}
		XFlush(globalDisplay);

		XRRSelectInput(globalDisplay, m_window, RRScreenChangeNotifyMask);

		return ERR_OK;
	}

	xdl_int XdevLWindowX11::enableDecoration() {
		int                   ActivatedDecorations;
		Atom                  HintAtom;

		ActivatedDecorations = 0;

		// Unset MWM hints
		HintAtom = XInternAtom(globalDisplay, "_MOTIF_WM_HINTS", True);
		if(HintAtom != None) {
			XDeleteProperty(globalDisplay, m_window, HintAtom);
			ActivatedDecorations = 1;
		}

		// Unset KWM hints
		HintAtom = XInternAtom(globalDisplay, "KWM_WIN_DECORATION", True);
		if(HintAtom != None) {
			XDeleteProperty(globalDisplay, m_window, HintAtom);
			ActivatedDecorations = 1;
		}

		// Unset GNOME hints
		HintAtom = XInternAtom(globalDisplay, "_WIN_HINTS", True);
		if(HintAtom != None) {
			XDeleteProperty(globalDisplay, m_window, HintAtom);
			ActivatedDecorations = 1;
		}

		// Unset NET_WM hints
		HintAtom = XInternAtom(globalDisplay, "_NET_WM_WINDOW_TYPE", True);
		if(HintAtom != None) {
			Atom NET_WMHints = XInternAtom(globalDisplay, "_NET_WM_WINDOW_TYPE_NORMAL", True);
			if(NET_WMHints != None) {
				XChangeProperty(globalDisplay, m_window,
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

			if(XSendEvent(globalDisplay, DefaultRootWindow(globalDisplay),  False, SubstructureNotifyMask | SubstructureRedirectMask, &msg) == 0) {
				XDEVL_MODULE_WARNING("Could not remove _NET_WM_STATE_FULLSCREEN state.\n");
			}

		}

		// Finally unset the transient hints if necessary
		if(ActivatedDecorations) {
			// NOTE: Does this work?
			XSetTransientForHint(globalDisplay, m_window, None);
			XUnmapWindow(globalDisplay, m_window);
			XMapWindow(globalDisplay, m_window);
		}
		return ERR_OK;
	}

	void XdevLWindowX11::setFullscreen(xdl_bool state) {
		if(state) {
			setFullscreenVideoMode();
		} else {
			restoreFullscreenVideoMode();
		}
	}

	const XdevLWindowPosition& XdevLWindowX11::getPosition() {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

//		XWindowAttributes wa;
//		XGetWindowAttributes(globalDisplay, m_window, &wa);
//		m_attribute.position.x = wa.x;
//		m_attribute.position.y = wa.y;
		return m_attribute.position;
	}

	const XdevLWindowSize& XdevLWindowX11::getSize() {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

//		XWindowAttributes wa;
//		XGetWindowAttributes(globalDisplay, m_window, &wa);
//		m_attribute.size.width 	= wa.width;
//		m_attribute.size.height = wa.height;
		return m_attribute.size;
	}

	XdevLWindowSize::type  XdevLWindowX11::getWidth() {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");
//		XWindowAttributes wa;
//		XGetWindowAttributes(globalDisplay, m_window, &wa);
//		m_attribute.size.width 	= wa.width;
//		m_attribute.size.height = wa.height;
		return m_attribute.size.width;
	}

	XdevLWindowSize::type  XdevLWindowX11::getHeight() {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");
//		XWindowAttributes wa;
//		XGetWindowAttributes(globalDisplay, m_window, &wa);
//		m_attribute.size.width 	= wa.width;
//		m_attribute.size.height = wa.height;
		return m_attribute.size.height;
	}

	XdevLWindowPosition::type XdevLWindowX11::getX() {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");
//		XWindowAttributes wa;
//		XGetWindowAttributes(globalDisplay, m_window, &wa);
//		m_attribute.position.x = wa.x;
//		m_attribute.position.y = wa.y;
		return m_attribute.position.x;
	}

	XdevLWindowPosition::type XdevLWindowX11::getY() {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");
//		XWindowAttributes wa;
//		XGetWindowAttributes(globalDisplay, m_window, &wa);
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
		XMoveWindow(globalDisplay, m_window, m_attribute.position.x, m_attribute.position.y);
		XMapWindow(globalDisplay, m_window);
	}

	void XdevLWindowX11::setY(XdevLWindowPosition::type y) {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

		XdevLWindowImpl::setY(m_screenHeight - (m_attribute.size.height + y));
		XMoveWindow(globalDisplay, m_window, m_attribute.position.x, m_attribute.position.y);
		XMapWindow(globalDisplay, m_window);
	}

	void XdevLWindowX11::setWidth(XdevLWindowSize::type width) {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

		XdevLWindowImpl::setWidth(width);
		XResizeWindow(globalDisplay, m_window, m_attribute.size.width, m_attribute.size.height);
		XMapWindow(globalDisplay, m_window);
	}

	void XdevLWindowX11::setHeight(XdevLWindowSize::type height) {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

		XdevLWindowImpl::setHeight(height);
		XResizeWindow(globalDisplay, m_window,  m_attribute.size.width, m_attribute.size.height);
		XMapWindow(globalDisplay, m_window);
	}

	void XdevLWindowX11::setSize(const XdevLWindowSize& size) {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

		m_attribute.size = size;

		XSizeHints *sizehints = XAllocSizeHints();
		long userhints;

		XGetWMNormalHints(globalDisplay, m_window, sizehints, &userhints);

		sizehints->min_width = sizehints->max_width = m_attribute.size.width;
		sizehints->min_height = sizehints->max_height = m_attribute.size.height;
		sizehints->flags |= PMinSize | PMaxSize;

		XSetWMNormalHints(globalDisplay, m_window, sizehints);

		XFree(sizehints);

		// OK What Moving the window for a resize? Well some Window Manager
		// resize only after either the user move/resize the window or within the code
		// or first unmapping then mapping.

		XResizeWindow(globalDisplay, m_window, m_attribute.size.width, m_attribute.size.height);
		XMoveWindow(globalDisplay, m_window, m_attribute.position.x, m_attribute.position.y);
		XRaiseWindow(globalDisplay, m_window);
		XFlush(globalDisplay);
	}

	void XdevLWindowX11::setPosition(const XdevLWindowPosition& position) {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

		m_attribute.position.x = position.x;
		m_attribute.position.y = m_screenHeight - (m_attribute.size.height + position.y);
		XMoveWindow(globalDisplay, m_window, m_attribute.position.x, m_attribute.position.y);
		XMapWindow(globalDisplay, m_window);
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
		XSetWMNormalHints(globalDisplay, m_window, sizeHints);
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

		XSetWMProperties(globalDisplay, m_window, &windowName, &iconName, nullptr, 0, nullptr, nullptr, class_hints);
		XFree(class_hints);
	}


	void XdevLWindowX11::show() {
		XMapWindow(globalDisplay, m_window);

		//XEvent event;
		//XIfEvent(globalDisplay, &event, &isMapNotify, (XPointer)&m_window);
		XFlush(globalDisplay);
	}

	void XdevLWindowX11::hide() {
		XUnmapEvent event;
		event.type = UnmapNotify;
		event.window = m_window;
		event.from_configure = False;
		XSendEvent(globalDisplay, m_rootWindow, False, StructureNotifyMask | SubstructureNotifyMask, (XEvent*)&event);

		XUnmapWindow(globalDisplay, m_window);

		//XEvent event;
		//XIfEvent(globalDisplay, &event, &isUnmapNotify, (XPointer)&m_window);
		XFlush(globalDisplay);
	}

	void XdevLWindowX11::raise() {
		XRaiseWindow(globalDisplay, m_window);
		//XFlush(globalDisplay);
	}

	void XdevLWindowX11::setPointerPosition(xdl_uint x, xdl_uint y) {

		XWindowAttributes wa;
		XGetWindowAttributes(globalDisplay, m_window, &wa);
		// (display, src window, dst windiw, src x, src y, src width, src height, dst x, dst y)
		XWarpPointer(globalDisplay, m_window, m_window, 0, 0, wa.width, wa.height, x, y);
	}

	xdl_bool XdevLWindowX11::isHidden() {
		Atom actualType;
		int actualFormat;
		unsigned long i, numItems, bytesAfter;
		unsigned char *propertyValue = nullptr;
		long maxLength = 1024;
		if(XGetWindowProperty(globalDisplay, m_window, _NET_WM_STATE,
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

		if(XGetWindowProperty(globalDisplay, m_window, _NET_WM_STATE,
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

	void XdevLWindowX11::setWindowBordered(xdl_bool state) {

		if(_MOTIF_WM_HINTS != None) {
			/* Hints used by Motif compliant window managers */
			struct {
				unsigned long flags;
				unsigned long functions;
				unsigned long decorations;
				long input_mode;
				unsigned long status;
			} MWMHints = {
				(1 << 1), 0, static_cast<unsigned long>((state == xdl_true) ? 1 : 0), 0, 0
			};

			XChangeProperty(globalDisplay, m_window, _MOTIF_WM_HINTS, _MOTIF_WM_HINTS, 32,
			                PropModeReplace, (unsigned char *) &MWMHints,
			                sizeof(MWMHints) / 4);
		} else {  /* set the transient hints instead, if necessary */
			XSetTransientForHint(globalDisplay, m_window, m_rootWindow);
		}
	}

	void XdevLWindowX11::grabPointer() {
//		for(;;) {
//			xdl_int result = XGrabPointer(globalDisplay, m_window, True, 0, GrabModeAsync, GrabModeAsync, m_window, None, CurrentTime);
//			if(result == GrabSuccess) {
//				break;
//			}
//		}
//		XFlush(globalDisplay);
	}

	void XdevLWindowX11::ungrabPointer() {
//		XUngrabPointer(globalDisplay, CurrentTime);
	}

	void XdevLWindowX11::setInputFocus() {

		XWindowAttributes attribute;
		XGetWindowAttributes(globalDisplay,m_window,&attribute);
		if(attribute.map_state == IsViewable) {
			XSetInputFocus(globalDisplay, m_window,  RevertToParent, CurrentTime);
		}
		XFlush(globalDisplay);


		//XGrabKeyboard(globalDisplay, m_window, False, GrabModeAsync, GrabModeAsync, CurrentTime);

//		Atom atom = _NET_ACTIVE_WINDOW;
//		XChangeProperty(globalDisplay, m_window, _NET_WM_STATE, XA_ATOM, 32, PropModeReplace, (unsigned char *)&atom, 1);

//		XEvent event;
//		event.xany.type 						= ClientMessage;
//		event.xclient.message_type 	= _NET_ACTIVE_WINDOW;
//		event.xclient.format 				= 32;
//		event.xclient.window 				= m_window;
//		event.xclient.data.l[0] 		= 1;  /* source indication. 1 = application */
//		event.xclient.data.l[1] 		= CurrentTime;
//		event.xclient.data.l[2] 		= m_window;
//
//		XSendEvent(globalDisplay, m_rootWindow, 0, SubstructureNotifyMask | SubstructureRedirectMask, &event);
//
	}

	xdl_bool XdevLWindowX11::hasFocus() {
		Atom actualType;
		int actualFormat;
		unsigned long i, numItems, bytesAfter;
		unsigned char *propertyValue = nullptr;
		long maxLength = 1024;
		if(XGetWindowProperty(globalDisplay, m_window, _NET_WM_STATE,
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

		_MOTIF_WM_HINTS 					= XInternAtom(globalDisplay, "_MOTIF_WM_HINTS", False);
		_NET_WM_WINDOW_TYPE					= XInternAtom(globalDisplay, "_NET_WM_WINDOW_TYPE", False);
		_NET_WM_WINDOW_TYPE_DESKTOP			= XInternAtom(globalDisplay, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
		_NET_WM_WINDOW_TYPE_DOCK			= XInternAtom(globalDisplay, "_NET_WM_WINDOW_TYPE_DOCK", False);
		_NET_WM_WINDOW_TYPE_TOOLBAR			= XInternAtom(globalDisplay, "_NET_WM_WINDOW_TYPE_TOOLBAR", False);
		_NET_WM_WINDOW_TYPE_MENU			= XInternAtom(globalDisplay, "_NET_WM_WINDOW_TYPE_MENU", False);
		_NET_WM_WINDOW_TYPE_UTILITY			= XInternAtom(globalDisplay, "_NET_WM_WINDOW_TYPE_UTILITY", False);
		_NET_WM_WINDOW_TYPE_SPLASH			= XInternAtom(globalDisplay, "_NET_WM_WINDOW_TYPE_SPLASH", False);
		_NET_WM_WINDOW_TYPE_DIALOG			= XInternAtom(globalDisplay, "_NET_WM_WINDOW_TYPE_DIALOG", False);
		_NET_WM_WINDOW_TYPE_DROPDOWN_MENU	= XInternAtom(globalDisplay, "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU", False);
		_NET_WM_WINDOW_TYPE_POPUP_MENU		= XInternAtom(globalDisplay, "_NET_WM_WINDOW_TYPE_POPUP_MENU", False);
		_NET_WM_WINDOW_TYPE_TOOLTIP			= XInternAtom(globalDisplay, "_NET_WM_WINDOW_TYPE_TOOLTIP", False);
		_NET_WM_WINDOW_TYPE_NOTIFICATION	= XInternAtom(globalDisplay, "_NET_WM_WINDOW_TYPE_NOTIFICATION", False);
		_NET_WM_WINDOW_TYPE_COMBO			= XInternAtom(globalDisplay, "_NET_WM_WINDOW_TYPE_COMBO", False);
		_NET_WM_WINDOW_TYPE_DND				= XInternAtom(globalDisplay, "_NET_WM_WINDOW_TYPE_DND", False);
		_NET_WM_WINDOW_TYPE_NORMAL			= XInternAtom(globalDisplay, "_NET_WM_WINDOW_TYPE_NORMAL", False);
		_NET_WM_WINDOW_OPACITY 				= XInternAtom(globalDisplay, "_NET_WM_WINDOW_OPACITY", False);
		_NET_WM_BYPASS_COMPOSITOR			= XInternAtom(globalDisplay, "_NET_WM_BYPASS_COMPOSITOR", False);
		_NET_WM_ACTION_RESIZE				= XInternAtom(globalDisplay, "_NET_WM_ACTION_RESIZE", False);
		_NET_WM_STATE						= XInternAtom(globalDisplay, "_NET_WM_STATE", False);
		_NET_WM_STATE_MAXIMIZED_VERT		= XInternAtom(globalDisplay, "_NET_WM_STATE_MAXIMIZED_VERT", False);
		_NET_WM_STATE_MAXIMIZED_HORZ		= XInternAtom(globalDisplay, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
		_NET_WM_STATE_FULLSCREEN			= XInternAtom(globalDisplay, "_NET_WM_STATE_FULLSCREEN", False);
		_NET_WM_STATE_HIDDEN				= XInternAtom(globalDisplay, "_NET_WM_STATE_HIDDEN", False);
		_NET_WM_STATE_FOCUSED				= XInternAtom(globalDisplay, "_NET_WM_STATE_FOCUSED", False);

		_NET_WM_NAME						= XInternAtom(globalDisplay, "_NET_WM_NAME", False);
		_NET_WM_ICON_NAME					= XInternAtom(globalDisplay, "_NET_WM_ICON_NAME", False);
		_NET_WM_ICON						= XInternAtom(globalDisplay, "_NET_WM_ICON", False);
		_NET_WM_PING						= XInternAtom(globalDisplay, "_NET_WM_PING", False);

		WM_STATE							= XInternAtom(globalDisplay, "WM_STATE", False);
		WM_NAME								= XInternAtom(globalDisplay, "WM_NAME", False);
		WM_NORMAL_HINTS						= XInternAtom(globalDisplay, "WM_NORMAL_HINTS", False);
		WM_HINTS							= XInternAtom(globalDisplay, "WM_HINTS", False);
		WM_ICON_SIZE						= XInternAtom(globalDisplay, "WM_ICON_SIZE", False);
		WM_DELETE_WINDOW					= XInternAtom(globalDisplay, "WM_DELETE_WINDOW", False);
		WM_PROTOCOLS						= XInternAtom(globalDisplay, "WM_PROTOCOLS", False);
		WM_TRANSIENT_FOR					= XInternAtom(globalDisplay, "WM_TRANSIENT_FOR", False);
		WM_CLASS							= XInternAtom(globalDisplay, "WM_CLASS", False);

		XdndEnter							= XInternAtom(globalDisplay, "XdndEnter", False);
		XdndPosition						= XInternAtom(globalDisplay, "XdndPosition", False);
		XdndStatus							= XInternAtom(globalDisplay, "XdndStatus", False);
		XdndTypeList						= XInternAtom(globalDisplay, "XdndTypeList", False);
		XdndActionCopy						= XInternAtom(globalDisplay, "XdndActionCopy", False);
		XdndDrop							= XInternAtom(globalDisplay, "XdndDrop", False);
		XdndFinished						= XInternAtom(globalDisplay, "XdndFinished", False);
		XdndSelection						= XInternAtom(globalDisplay, "XdndSelection", False);


		_KDE_NET_WM_WINDOW_TYPE_OVERRIDE	= XInternAtom(globalDisplay, "_KDE_NET_WM_WINDOW_TYPE_OVERRIDE", False);


		// Now we are going to tell the WM in which protocols we are interested are or which one we support.

		std::vector<Atom> protocols;


		protocols.push_back(_NET_WM_WINDOW_TYPE_DESKTOP);
		protocols.push_back(_NET_WM_ACTION_RESIZE);
		protocols.push_back(_NET_WM_WINDOW_TYPE);
		protocols.push_back(_NET_WM_WINDOW_TYPE_UTILITY);
		protocols.push_back(_NET_WM_WINDOW_OPACITY);
		protocols.push_back(_NET_WM_STATE);
		protocols.push_back(_NET_WM_STATE_MAXIMIZED_VERT);
		protocols.push_back(_NET_WM_STATE_MAXIMIZED_HORZ);
		protocols.push_back(_NET_WM_STATE_FULLSCREEN);
		protocols.push_back(_NET_WM_STATE_HIDDEN);
		protocols.push_back(_NET_WM_STATE_FOCUSED);
		protocols.push_back(_NET_WM_STATE_ADD);
		protocols.push_back(_NET_WM_STATE_REMOVE);
		protocols.push_back(_NET_WM_NAME);
		protocols.push_back(_NET_WM_ICON_NAME);
		protocols.push_back(_NET_WM_ICON);
		protocols.push_back(WM_STATE);
		protocols.push_back(WM_ICON_SIZE);
		protocols.push_back(WM_DELETE_WINDOW);
		protocols.push_back(WM_PROTOCOLS);
		protocols.push_back(_MOTIF_WM_HINTS);

		protocols.push_back(XdndEnter);
		protocols.push_back(XdndPosition);
		protocols.push_back(XdndStatus);
		protocols.push_back(XdndTypeList);
		protocols.push_back(XdndActionCopy);
		protocols.push_back(XdndDrop);
		protocols.push_back(XdndFinished);
		protocols.push_back(XdndSelection);



		XSetWMProtocols(globalDisplay, m_window, protocols.data(), protocols.size());
		return ERR_OK;
	}

	void XdevLWindowX11::setType(XdevLWindowTypes type) {

		switch(type) {

			case XDEVL_WINDOW_TYPE_NORMAL: {
				if(_NET_WM_WINDOW_TYPE_NORMAL != 0) {
					XChangeProperty(globalDisplay, m_window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32,PropModeReplace, (unsigned char *)&_NET_WM_WINDOW_TYPE_NORMAL, 1);
				} else {
				}
			}
			break;
			case XDEVL_WINDOW_TYPE_TOOLTIP: {
				if(_NET_WM_WINDOW_TYPE_TOOLTIP != 0) {
					XChangeProperty(globalDisplay, m_window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32,PropModeReplace, (unsigned char *)&_NET_WM_WINDOW_TYPE_TOOLTIP, 1);
				} else {
				}
			}
			break;
			case XDEVL_WINDOW_TYPE_DROPDOWN_MENU: {
				if(_NET_WM_WINDOW_TYPE_DROPDOWN_MENU != 0) {
					XChangeProperty(globalDisplay, m_window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32,PropModeReplace, (unsigned char *)&_NET_WM_WINDOW_TYPE_DROPDOWN_MENU, 1);
				} else {
				}
			}
			break;
			case XDEVL_WINDOW_TYPE_SPLASH: {
				if(_NET_WM_WINDOW_TYPE_SPLASH != 0) {
					XChangeProperty(globalDisplay, m_window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32,PropModeReplace, (unsigned char *)&_NET_WM_WINDOW_TYPE_SPLASH, 1);
				} else {
				}
			}
			break;
			case XDEVL_WINDOW_TYPE_UNKNOWN:
			default:
				break;
		}
		XFlush(globalDisplay);
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

	XdevLWindowServerX11::XdevLWindowServerX11(XdevLModuleCreateParameter* parameter) :
		XdevLWindowServerImpl(parameter) {

	}

	XdevLWindowServerX11::~XdevLWindowServerX11() {

	}

	xdl_int XdevLWindowServerX11::createWindow(XdevLWindow** window,
	    const XdevLWindowTitle& title,
	    const XdevLWindowPosition& position,
	    const XdevLWindowSize& size,
	    const XdevLWindowTypes& type) {

		XdevLWindowX11* wnd = new XdevLWindowX11(nullptr);
		wnd->create(title, position, size, type);
		*window = wnd;
		m_windowList[wnd->getWindowID()] = wnd;
		return ERR_OK;
	}



//
// -----------------------------------------------------------------------------
//

	XdevLWindowX11EventServer::XdevLWindowX11EventServer(XdevLModuleCreateParameter* parameter) :
		XdevLWindowEventServerImpl(parameter, windowEventServerModuleDesc),
		m_focusWindow(nullptr),
		m_keyboard(nullptr) {
		m_keyboard = new XdevLWindowX11Keyboard(globalDisplay, getMediator());

		WM_DELETE_WINDOW					= XInternAtom(globalDisplay, "WM_DELETE_WINDOW", False);
		WM_PROTOCOLS						= XInternAtom(globalDisplay, "WM_PROTOCOLS", False);
		_NET_WM_PING						= XInternAtom(globalDisplay, "_NET_WM_PING", False);
	}

	XdevLWindowX11EventServer::~XdevLWindowX11EventServer() {

	}


	xdl_int XdevLWindowX11EventServer::registerWindowForEvents(XdevLWindow* window) {
		return XdevLWindowEventServerImpl::registerWindowForEvents(window);
	}

	xdl_int XdevLWindowX11EventServer::unregisterWindowFromEvents(XdevLWindow* window) {
		return XdevLWindowEventServerImpl::unregisterWindowFromEvents(window);
	}

	xdl_int XdevLWindowX11EventServer::init() {
		XDEVL_MODULE_SUCCESS("Created successfully" << std::endl);
		return ERR_OK;
	}

	void* XdevLWindowX11EventServer::getInternal(const XdevLInternalName& id) {

		return nullptr;
	}

	xdl_int XdevLWindowX11EventServer::shutdown() {

		// Free the keyboard device.
		if(m_keyboard != nullptr) {
			delete m_keyboard;
			m_keyboard = nullptr;
		}

		XDEVL_MODULE_SUCCESS("Shutdown process was successful.\n");
		return ERR_OK;
	}

	xdl_int XdevLWindowX11EventServer::update() {
		return pollEvents();
	}

	xdl_int XdevLWindowX11EventServer::pollEvents() {

		// Only run this while loop when we have events in the queue.
		while(XPending(globalDisplay) > 0) {
			XEvent event;

			// Get next event.
			XNextEvent(globalDisplay, &event);

			//
			// Handle generic events like xinput2, xfixes etc.
			//
			if(event.type == GenericEvent) {
				XGenericEventCookie* cookie = &event.xcookie;
				XGetEventData(globalDisplay, cookie);

				if(x11cursor) {
					x11cursor->onHandleXinputEvent(cookie, m_focusWindow);
				}

				XFreeEventData(globalDisplay, cookie);
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
					if (XPending(window->getNativeDisplay())) {
						XEvent nev;
						XPeekEvent(window->getNativeDisplay(), &nev);

						if (nev.type == KeyPress && nev.xkey.time == event.xkey.time && nev.xkey.keycode == event.xkey.keycode) {
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
					if(x11cursor->isRelativeMotionEnabled() == xdl_false) {
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
					if(XGetWindowProperty(globalDisplay, window->getNativeRootWindow(),
					                      XA_CUT_BUFFER0, 0, INT_MAX/4, False, event.xselectionrequest.target,
					                      &sevent.xselection.target, &seln_format, &nbytes,
					                      &overflow, &seln_data) == Success) {

						Atom XA_TARGETS = XInternAtom(globalDisplay, "TARGETS", 0);
						if(sevent.xselection.target == event.xselectionrequest.target) {
							XChangeProperty(globalDisplay, event.xselectionrequest.requestor, event.xselectionrequest.property,
							                sevent.xselection.target, seln_format, PropModeReplace,
							                seln_data, nbytes);
							sevent.xselection.property = event.xselectionrequest.property;
						} else if(XA_TARGETS == event.xselectionrequest.target) {
							Atom SupportedFormats[] = { sevent.xselection.target, XA_TARGETS };
							XChangeProperty(globalDisplay, event.xselectionrequest.requestor, event.xselectionrequest.property,
							                XA_ATOM, 32, PropModeReplace,
							                (unsigned char*)SupportedFormats,
							                sizeof(SupportedFormats)/sizeof(*SupportedFormats));
							sevent.xselection.property = event.xselectionrequest.property;
						}
						XFree(seln_data);
					}
					XSendEvent(globalDisplay, event.xselectionrequest.requestor, False, 0, &sevent);
					XSync(globalDisplay, False);

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
					    (event.xclient.data.l[0] == WM_DELETE_WINDOW) &&
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
					        (event.xclient.data.l[0] == _NET_WM_PING)) {
						Window root = DefaultRootWindow(globalDisplay);
						event.xclient.window = root;
						XSendEvent(globalDisplay, root, False, SubstructureRedirectMask | SubstructureNotifyMask, &event);
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
					if( (event.xconfigure.x != window->getX()) || (event.xconfigure.y != window->getY())) {

						ev.type					= XDEVL_WINDOW_EVENT;
						ev.window.event 		= XDEVL_WINDOW_MOVED;
						ev.window.x				= event.xconfigure.x;
						ev.window.y				= event.xconfigure.y;
						ev.window.width			= event.xconfigure.width;
						ev.window.height		= event.xconfigure.height;
						ev.button.windowid		= window->getWindowID();

						getMediator()->fireEvent(ev);
					}
					if( (event.xconfigure.width != window->getWidth()) || (event.xconfigure.height != window->getHeight())) {
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

	void XdevLWindowX11EventServer::flush() {
		pollEvents();
	}

//
// -----------------------------------------------------------------------------
//

	XdevLCursorX11::XdevLCursorX11(XdevLModuleCreateParameter* parameter) :
		XdevLModuleImpl<XdevLCursor>(parameter, cursorModuleDesc),
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
				ev.type 				= MouseMotion.getHashCode();
				ev.motion.timestamp		= getMediator()->getTimer().getTime64();
				ev.motion.x				= x_window;
				ev.motion.y				= y_window;
				ev.motion.xrel			= relative_cords[0];
				ev.motion.yrel			= relative_cords[1];
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
