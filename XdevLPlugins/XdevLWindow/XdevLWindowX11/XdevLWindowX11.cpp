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

extern xdl::XdevLModuleDescriptor windowX11DisplayDesc;
extern xdl::XdevLModuleDescriptor cursorX11Desc;
extern xdl::XdevLModuleDescriptor windowEventServerX11Desc;

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

namespace xdl {

	xdl_int reference_counter = 0;
	extern std::shared_ptr<XdevLX11Environment> globalX11Display;


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


	XdevLX11Environment::XdevLX11Environment(XdevLCoreMediator* core) : m_core(core) {
		std::cout << "XdevLX11Environment::XdevLX11Environment()\n";

		XdevLModuleCreateParameter parameter;

		parameter.setModuleId(XdevLID("XdevLDisplay"));
		display = std::make_shared<XdevLDisplayX11>(&parameter, windowX11DisplayDesc);

		parameter.setModuleId(XdevLID("XdevLWindowEventServer"));
		windowEventServer = std::make_shared<XdevLWindowEventServerX11>(&parameter, windowEventServerX11Desc);

		parameter.setModuleId(XdevLID("XdevLCursor"));
		cursor = std::make_shared<XdevLCursorX11>(&parameter, cursorX11Desc);

		// Register within the Core if this is using one.
		if(m_core) {
			m_core->registerModule(display);
			m_core->registerModule(windowEventServer);
			m_core->registerModule(cursor);
		}
	}


	XdevLX11Environment::~XdevLX11Environment() {
		std::cout << "XdevLX11Environment::~XdevLX11Environment()\n";
		if(m_core) {
			m_core->deleteModule(windowEventServer->getID());
			m_core->deleteModule(cursor->getID());
			m_core->deleteModule(display->getID());
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
			globalX11Display = std::make_shared<XdevLX11Environment>(mediator);
		}
		reference_counter++;
	}

	XdevLWindowX11::~XdevLWindowX11() {

		XDEVL_MODULE_INFO("~XdevLWindowX11()\n");

		if(reference_counter == 1) {
			globalX11Display.reset();
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

		// Get the connection to the display server.
		m_display = globalX11Display->getWindowEventServer()->getNativeDisplay();

		// Get the root window.
		m_rootWindow = globalX11Display->getWindowEventServer()->getNativeRootWindow();

		// Get the default screen number.
		m_screenNumber = DefaultScreen(m_display);

		// Get the default color map.
		m_defaultColorMap = DefaultColormap(m_display, m_screenNumber);

		XColor color;
		color.red = 0;
		color.green = 0;
		color.blue = 0;
		color.pixel = ((xdl_int)m_backgroundColor[0] << 16) | ((xdl_int)m_backgroundColor[1] << 8) | (xdl_int)m_backgroundColor[2];

//		char green[] = "#00FF00";
//		XParseColor(globalX11Display, m_defaultColorMap, green, &color);
//		XAllocColor(globalX11Display, m_defaultColorMap, &color);

//    windowAttributes.background_pixmap 	= None;
//    windowAttributes.border_pixel 			= 0;
//		windowAttributes.colormap 					= XCreateColormap(globalX11Display, m_rootWindow, visual, AllocNone);

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
		                         m_attribute.position.y,
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
		if(isFullscreen()) {
			enableFullscreenMode();
		}

		m_id = m_window;

		// This we might want to change later
		globalX11Display->getWindowEventServer()->registerWindowForEvents(this);

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

	xdl_bool XdevLWindowX11::isFullscreen() {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

		return XdevLWindowImpl::isFullscreen();
	}

	xdl_bool XdevLWindowX11::isPointerHidden() {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

		return XdevLWindowImpl::isPointerHidden();
	}

	void XdevLWindowX11::setX(XdevLWindowPosition::type x) {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

		XdevLWindowImpl::setX(x);
		XMoveWindow(m_display, m_window, m_attribute.position.x, m_attribute.position.y);
		XMapWindow(m_display, m_window);
	}

	void XdevLWindowX11::setY(XdevLWindowPosition::type y) {
		XDEVL_ASSERT(None != m_window, "XdevLWindowX11 not created.");

		XdevLWindowImpl::setY(y);
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
		m_attribute.position.y = position.y;
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

	xdl_int XdevLWindowX11::getNativeScreenNumber() {
		return m_screenNumber;
	}

	Colormap XdevLWindowX11::getNativeDefaultColorMap() {
		return m_defaultColorMap;
	}


}
