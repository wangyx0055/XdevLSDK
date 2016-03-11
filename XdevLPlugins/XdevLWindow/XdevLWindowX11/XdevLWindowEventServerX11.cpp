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

#include <climits>

#include "XdevLWindowEventServerX11.h"
#include "XdevLWindowX11.h"

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

namespace xdl {

	std::shared_ptr<XdevLX11Display> globalDisplay;

	const XdevLID ButtonPressed("XDEVL_BUTTON_PRESSED");
	const XdevLID ButtonReleased("XDEVL_BUTTON_RELEASED");
	const XdevLID MouseButtonPressed("XDEVL_MOUSE_BUTTON_PRESSED");
	const XdevLID MouseButtonReleased("XDEVL_MOUSE_BUTTON_RELEASED");
	const XdevLID MouseMotion("XDEVL_MOUSE_MOTION");
	const XdevLID WindowEvent("XDEVL_WINDOW_EVENT");

	XdevLWindowEventServerX11::XdevLWindowEventServerX11(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor) :
		XdevLWindowEventServerImpl(parameter, desriptor),
		m_display(nullptr),
		m_rootWindow(None),
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
		// Start X server with thread support.
		XInitThreads();

		// Connect to X server. TODO We are using the default display. Needs to be changed later to
		// make it more flexible.
		m_display = XOpenDisplay(nullptr);
		if(m_display == nullptr) {
			return ERR_ERROR;;
		}

		m_rootWindow = RootWindow(m_display, DefaultScreen(m_display));

		// Print out some useless information :D
		std::clog << "\n---------------------------- X11 Server Information ----------------------------\n";
		std::clog << "Vendor              : " << XServerVendor(m_display) << "\n";
		std::clog << "Release             : " << XVendorRelease(m_display)<< "\n";
		std::clog << "Number of Screens   : " << XScreenCount(m_display) 	<< std::endl;

		m_keyboard = new XdevLWindowX11Keyboard(m_display, getMediator());

		WM_DELETE_WINDOW = XInternAtom(m_display, "WM_DELETE_WINDOW", False);
		WM_PROTOCOLS = XInternAtom(m_display, "WM_PROTOCOLS", False);
		_NET_WM_PING = XInternAtom(m_display, "_NET_WM_PING", False);

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

		if(nullptr != m_display) {
			XCloseDisplay(m_display);
			m_display = nullptr;
		}

		return ERR_OK;
	}

	xdl_int XdevLWindowEventServerX11::update() {
		return pollEvents();
	}

	xdl_int XdevLWindowEventServerX11::pollEvents() {

		// Only run this while loop when we have events in the queue.
		while(XPending(m_display) > 0) {
			XEvent event;

			// Get next event.
			XNextEvent(m_display, &event);

			//
			// Handle generic events like xinput2, xfixes etc.
			//
			if(event.type == GenericEvent) {
				XGenericEventCookie* cookie = &event.xcookie;
				XGetEventData(m_display, cookie);

				if(globalDisplay->getCursor()) {
					// TODO Don't cast here. Find another way.
					globalDisplay->getCursor()->onHandleXinputEvent(cookie, m_focusWindow);
				}

				XFreeEventData(m_display, cookie);
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
					if(XGetWindowProperty(m_display, window->getNativeRootWindow(),
					                      XA_CUT_BUFFER0, 0, INT_MAX/4, False, event.xselectionrequest.target,
					                      &sevent.xselection.target, &seln_format, &nbytes,
					                      &overflow, &seln_data) == Success) {

						Atom XA_TARGETS = XInternAtom(m_display, "TARGETS", 0);
						if(sevent.xselection.target == event.xselectionrequest.target) {
							XChangeProperty(m_display, event.xselectionrequest.requestor, event.xselectionrequest.property,
							                sevent.xselection.target, seln_format, PropModeReplace,
							                seln_data, nbytes);
							sevent.xselection.property = event.xselectionrequest.property;
						} else if(XA_TARGETS == event.xselectionrequest.target) {
							Atom SupportedFormats[] = { sevent.xselection.target, XA_TARGETS };
							XChangeProperty(m_display, event.xselectionrequest.requestor, event.xselectionrequest.property,
							                XA_ATOM, 32, PropModeReplace,
							                (unsigned char*)SupportedFormats,
							                sizeof(SupportedFormats)/sizeof(*SupportedFormats));
							sevent.xselection.property = event.xselectionrequest.property;
						}
						XFree(seln_data);
					}
					XSendEvent(m_display, event.xselectionrequest.requestor, False, 0, &sevent);
					XSync(m_display, False);

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
						Window root = DefaultRootWindow(m_display);
						event.xclient.window = root;
						XSendEvent(m_display, root, False, SubstructureRedirectMask | SubstructureNotifyMask, &event);
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

	Display* XdevLWindowEventServerX11::getNativeDisplay() {
		return m_display;
	}

	Window XdevLWindowEventServerX11::getNativeRootWindow() {
		return m_rootWindow;
	}

}
