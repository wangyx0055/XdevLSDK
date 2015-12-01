/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2012 Cengiz Terzibas

	This library is free software; you can redistribute it and/or modify it under the
	terms of the GNU Lesser General Public License as published by the Free Software
	Foundation; either version 2.1 of the License, or (at your option) any later version.
	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License along with
	this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
	Suite 330, Boston, MA 02111-1307 USA

	I would appreciate if you report all bugs to: cengiz@terzibas.de
*/

#include <iostream>
#include <XdevLTypes.h>
#include <XdevLWindowX11.h>
#include <XdevLXstring.h>
#include <XdevLUtils.h>
#include <sstream>
#include <cstddef>
#include <climits>

class m_keylayout;

xdl::XdevLModuleDescriptor xdl::XdevLWindowLinux::m_windowX11ModuleDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[0],
	xdl::window_copyright,
	xdl::window_x11_description,
	xdl::XdevLWindowMajorVersion,
	xdl::XdevLWindowMinorVersion,
	xdl::XdevLWindowPatchVersion
};


xdl::XdevLPluginDescriptor m_windowX11PluginDescriptor {
	xdl::window_x11_pluginName,
	xdl::window_moduleNames,
	xdl::XdevLWindowPluginMajorVersion,
	xdl::XdevLWindowPluginMinorVersion,
	xdl::XdevLWindowPluginPatchVersion
};

static Display* m_display = nullptr;

static xdl::xdl_int reference_counter = 0;

static std::map<Window, xdl::XdevLWindow*> windowMap;

static Window m_clipWindow = None;

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter)  {

	if(reference_counter == 0) {

		// Start X-Server thread support.
		XInitThreads();

		// Open a display.
		if(m_display == nullptr) {
			m_display = XOpenDisplay(nullptr);
			if(m_display == nullptr) {
				return xdl::ERR_ERROR;
			}
			std::clog << "\n---------------------------- X11 Server Information ----------------------------\n";
			std::clog << "Vendor              : " << XServerVendor(m_display) << "\n";
			std::clog << "Release             : " << XVendorRelease(m_display)<< "\n";
			std::clog << "Number of Screens   : " << XScreenCount(m_display) 	<< std::endl;


			XSetWindowAttributes attrib;
			attrib.event_mask = FocusChangeMask | EnterWindowMask | LeaveWindowMask |
			                    ExposureMask | ButtonPressMask | ButtonReleaseMask |
			                    PointerMotionMask | KeyPressMask | KeyReleaseMask |
			                    PropertyChangeMask | StructureNotifyMask | SubstructureNotifyMask;

			m_clipWindow = XCreateWindow(m_display,
			                             RootWindow(m_display, DefaultScreen(m_display)),
			                             0,
			                             0,
			                             DisplayWidth(m_display, DefaultScreen(m_display)),
			                             DisplayHeight(m_display, DefaultScreen(m_display)),
			                             0,
			                             CopyFromParent,
			                             InputOnly,
			                             CopyFromParent,
			                             0,
			                             &attrib);

		}
	}

	if(xdl::XdevLWindowLinux::m_windowX11ModuleDesc.getName() == parameter->getModuleName()) {
		xdl::XdevLWindowLinux* window = new xdl::XdevLWindowLinux(parameter);
		parameter->setModuleInstance(window);
	} else if(xdl::XdevLWindowServerImpl::m_windowServerModuleDesc.getName() == parameter->getModuleName()) {
		xdl::XdevLWindowServerX11* windowServer = new xdl::XdevLWindowServerX11(parameter);
		parameter->setModuleInstance(windowServer);
	} else {
		return xdl::ERR_MODULE_NOT_FOUND;
	}

	reference_counter++;

	return xdl::ERR_OK;
}

extern "C" XDEVL_EXPORT void _delete(xdl::XdevLModule* obj) {
	if(obj)
		delete obj;

	reference_counter--;

	if(reference_counter == 0) {
		XDestroyWindow(m_display, m_clipWindow);

		// And now close the window.
		if(m_display) {
			XCloseDisplay(m_display);
			m_display = nullptr;
		}
	}
}

extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor()  {
	return &m_windowX11PluginDescriptor;
}


namespace xdl {


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



	xdl_int XdevLWindowLinux::RunThread(thread::ThreadArgument* argument) {
		XDEVL_MODULE_INFO("Starting Event Thread ...");

		XdevLWindowThreadArgument* arg = static_cast<XdevLWindowThreadArgument*>(argument);

		m_keyboard = new XdevLWindowX11Keyboard(m_display, arg->core);

		startEventThread();


		while(true) {
			// The thread got the stop event.
			if(isEventThreadRunning() == xdl_false) {
				break;
			}


			// Do this loop forever ... until :D.
			//	while(XPending(m_display) > 0) {
			XEvent event;

			// Get event and block if no events are available.
			XNextEvent(m_display, &event);


			// Get current XdevLWindow.
			XdevLWindow* window = windowMap[event.xany.window];


			// Check which event type we've got.
			switch(event.type) {

					//
					// Will be send if window was created.
					//
				case CreateNotify:
					if(window) {
						XdevLEvent ev;
						ev.common.timestamp = arg->core->getTimer().getTime64();
						ev.type				= XDEVL_WINDOW_EVENT;
						ev.window.event 	= XDEVL_WINDOW_CREATE;
						ev.window.windowid	= window->getWindowID();

						arg->core->fireEvent(ev);
					}
					break;

					//
					// Keyboard pressed.
					//
				case KeyPress: {
					KeySym keysym = NoSymbol;
					KeyCode keycode = event.xkey.keycode;
					keysym = XkbKeycodeToKeysym(m_display, keycode, 0, event.xkey.state & ShiftMask ? 1 : 0);
					XdevLScanCode idx = m_keyboard->keyCodeToXdevLScancode(keycode);

					//XLookupKeysym(&event.xkey, 0); // This gives the Symbol expression of the KeyCode.

					//std::cout << "X11 keycode: " << (int)keycode << ", X11 symcode: " << (int)keysym << ", XdevLScanCode: " << idx << std::endl;

					m_keyboard->sendKeyboardEvent(1, keycode, window->getWindowID());
				}
				break;

				//
				// Keyboard released.
				//
				case KeyRelease: {
					KeyCode keycode 	= event.xkey.keycode;
					m_keyboard->sendKeyboardEvent(0, keycode, window->getWindowID());
				}
				break;

				//
				// Mouse pointer moved.
				//
				case MotionNotify:
					if(window) {

						XdevLEvent ev;
						ev.common.timestamp 					= arg->core->getTimer().getTime64();
						ev.type 							= XDEVL_MOUSE_MOTION;

						ev.motion.x						= event.xmotion.x;
						ev.motion.y						= event.xmotion.y;
						//ev.motion.xrel				= event.motion.xrel;
						//ev.motion.yrel				= event.motion.yrel;
						ev.window.windowid		= window->getWindowID();

						if(event.xmotion.state & Button1Mask) {
							std::cout << "Button 1 was pressed during motion.\n";
						} else if(event.xmotion.state & Button2Mask) {
							std::cout << "Button 2 was pressed during motion.\n";
						}	else if(event.xmotion.state & Button3Mask) {
							std::cout << "Button 3 was pressed during motion.\n";
						}	else if(event.xmotion.state & Button4Mask) {
							std::cout << "Button 5 was pressed during motion.\n";
						}	else if(event.xmotion.state & Button5Mask) {
							std::cout << "Button 5 was pressed during motion.\n";
						}	else if(event.xmotion.state & ControlMask) {
							std::cout << "Control was pressed during motion.\n";
						}	else if(event.xmotion.state & Mod1Mask) {
							std::cout << "Mod1Mask was pressed during motion.\n";
						}	else if(event.xmotion.state & Mod2Mask) {
							std::cout << "Mod2Mask was pressed during motion.\n";
						}	else if(event.xmotion.state & Mod3Mask) {
							std::cout << "Mod3Mask was pressed during motion.\n";
						}	else if(event.xmotion.state & Mod4Mask) {
							std::cout << "Mod4Mask was pressed during motion.\n";
						}	else if(event.xmotion.state & Mod5Mask) {
							std::cout << "Mod5Mask was pressed during motion.\n";
						}

						ev.window.data1				= event.xmotion.state;

						arg->core->fireEvent(ev);
					}
					break;

					//
					// Mouse button pressed.
					//
				case ButtonPress:
					if(window) {
						XdevLEvent ev;
						ev.common.timestamp 					= arg->core->getTimer().getTime64();
						ev.type 							= XDEVL_MOUSE_BUTTON_PRESSED;

						ev.button.button			= event.xbutton.button;
						ev.button.x						= event.xbutton.x;
						ev.button.y						= event.xbutton.y;
						ev.window.windowid		= window->getWindowID();

						arg->core->fireEvent(ev);

					}
					break;

					//
					// Mouse button released.
					//
				case ButtonRelease :
					if(window) {
						XdevLEvent ev;
						ev.common.timestamp 					= arg->core->getTimer().getTime64();
						ev.type 							= XDEVL_MOUSE_BUTTON_RELEASED;

						ev.button.button			= event.xbutton.button;
						ev.button.x						= event.xbutton.x;
						ev.button.y						= event.xbutton.y;
						ev.window.windowid		= window->getWindowID();

						arg->core->fireEvent(ev);
					}
					break;

					//
					//
					//
				case PropertyNotify: {
					if(window) {
						//						char *name = XGetAtomName(m_display, event.xproperty.atom);
						//						std::cout << "PropertyNotify" << ": WindowID: " << window->getWindowID() << ": " << name << " -> " << ((event.xproperty.state == PropertyDelete) ? "deleted" : "changed") << std::endl;
						//						XFree(name);
						//						if(event.xproperty.atom == _NET_WM_STATE) {
						//							xdl_uint32 flags = getNetWMState();
						//							// TODO Do the WM state handling here.
						//							std::cout << "_NET_WM_STATE" << std::endl;
						//						}
					}
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
					if(XGetWindowProperty(m_display, m_rootWindow,
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
				case SelectionNotify:
					if(window) {

						Atom target = event.xselection.target;
						//	if (target == data->xdnd_req) {
						//}
					}
					break;
				default: {
					switch(event.type - m_event_basep) {
						case RRScreenChangeNotify: {
							// Show XRandR that we really care
							XRRUpdateConfiguration(&event);
						}
						break;
					}
				}
				break;

				//
				// The mouse entered the window area.
				//
				case EnterNotify:
					if(window) {
						XdevLEvent ev;
						ev.common.timestamp 				= arg->core->getTimer().getTime64();
						ev.type							= XDEVL_WINDOW_EVENT;
						ev.window.event 		= XDEVL_WINDOW_ENTER;
						ev.window.data1			= event.xcrossing.x;
						ev.window.data2			= event.xcrossing.y;
						ev.window.windowid	= window->getWindowID();

						arg->core->fireEvent(ev);
					}
					break;

					//
					// The mouse left the window area.
					//
				case LeaveNotify:
					if(window) {
						XdevLEvent ev;
						ev.common.timestamp 				= arg->core->getTimer().getTime64();
						ev.type							= XDEVL_WINDOW_EVENT;
						ev.window.event 		= XDEVL_WINDOW_LEAVE;
						ev.window.data1			= event.xcrossing.x;
						ev.window.data2			= event.xcrossing.y;
						ev.window.windowid	= window->getWindowID();

						arg->core->fireEvent(ev);
					}
					break;

					//
					// Input focus gained.
					//
				case FocusIn:
					if(window) {
						XdevLEvent ev;
						ev.common.timestamp 					= arg->core->getTimer().getTime64();
						ev.type								= XDEVL_WINDOW_EVENT;
						ev.window.event 			= XDEVL_WINDOW_INPUT_FOCUS_GAINED;
						ev.window.windowid		= window->getWindowID();

						getMediator()->fireEvent(ev);

					}
					break;

					//
					// Input focus lost focus.
					//
				case FocusOut:
					if(window) {
						XdevLEvent ev;
						ev.common.timestamp 					= arg->core->getTimer().getTime64();
						ev.type								= XDEVL_WINDOW_EVENT;
						ev.window.event 			= XDEVL_WINDOW_INPUT_FOCUS_LOST;
						ev.window.windowid		= window->getWindowID();

						arg->core->fireEvent(ev);
					}
					break;

					//
					// Window was hidden
					//
				case UnmapNotify:
					if(window) {
						XdevLEvent ev;
						ev.common.timestamp 				= arg->core->getTimer().getTime64();
						ev.type							= XDEVL_WINDOW_EVENT;
						ev.window.event 		= XDEVL_WINDOW_HIDDEN;
						ev.window.windowid	= window->getWindowID();

						arg->core->fireEvent(ev);
					}
					break;

					//
					// Window was shown.
					//
				case MapNotify:
					if(window) {
						XdevLEvent ev;
						ev.common.timestamp 				= arg->core->getTimer().getTime64();
						ev.type							= XDEVL_WINDOW_EVENT;
						ev.window.event 		= XDEVL_WINDOW_SHOWN;
						ev.window.windowid	= window->getWindowID();

						arg->core->fireEvent(ev);

					}
					break;

				case ClientMessage: {
					if((event.xclient.message_type == WM_PROTOCOLS) &&
					        (event.xclient.data.l[0] == WM_DELETE_WINDOW) &&
					        (event.xclient.format == 32)) {

						XdevLEvent ev;
						ev.common.timestamp 					= arg->core->getTimer().getTime64();
						ev.type								= XDEVL_WINDOW_EVENT;
						ev.window.event 			= XDEVL_WINDOW_CLOSE;
						ev.window.windowid		= window->getWindowID();

						arg->core->fireEvent(ev);

						// Make a core event.
						ev.type								= XDEVL_CORE_EVENT;
						ev.core.event 				= XDEVL_CORE_SHUTDOWN;

						arg->core->fireEvent(ev);

					}
					//
					// Desktop Systems. send a ping to check if the window is still active.
					// This must be handled otherwise the Desktop System will complain or it
					// will make the window dark or what ever.
					//
					else if((event.xclient.message_type == WM_PROTOCOLS) &&
					        (event.xclient.format == 32) &&
					        (event.xclient.data.l[0] == _NET_WM_PING)) {
						Window root = DefaultRootWindow(m_display);
						event.xclient.window = root;
						XSendEvent(m_display, root, False, SubstructureRedirectMask | SubstructureNotifyMask, &event);
					}
				}
				break;
				//
				//
				//
				case DestroyNotify: {
					if(event.xdestroywindow.window == m_window) {
						std::cout << "DestroyNotify" << ": WindowID: "
						          << windowMap[event.xexpose.window]->getWindowID()
						          << std::endl;
					}
				}
				break;

				//
				// Part of the window changed. Notify
				//
				case Expose:
					if(window) {
						XdevLEvent ev;
						ev.common.timestamp 				= arg->core->getTimer().getTime64();
						ev.type							= XDEVL_WINDOW_EVENT;
						ev.window.event 		= XDEVL_WINDOW_EXPOSED;
						ev.window.windowid	= windowMap[event.xexpose.window]->getWindowID();

						arg->core->fireEvent(ev);
					}
					break;

					//
					// Resized or moved
					//
				case ConfigureNotify:
					if(window) {
						if(event.xconfigure.x != m_prevConfigureEvent.x || event.xconfigure.y != m_prevConfigureEvent.y) {
							// Window moved.
							XdevLEvent ev;
							ev.common.timestamp 				= arg->core->getTimer().getTime64();
							ev.type							= XDEVL_WINDOW_EVENT;
							ev.window.event 		= XDEVL_WINDOW_MOVED;
							ev.window.x					= event.xconfigure.x;
							ev.window.y					= event.xconfigure.y;
							ev.window.width			= event.xconfigure.width;
							ev.window.height		= event.xconfigure.height;
							ev.window.windowid	= windowMap[event.xconfigure.window]->getWindowID();

							arg->core->fireEvent(ev);

						}
						if(event.xconfigure.width != m_prevConfigureEvent.width || event.xconfigure.height != m_prevConfigureEvent.height) {
							//
							// Window resized.
							//
							XdevLEvent ev;
							ev.common.timestamp 				= arg->core->getTimer().getTime64();
							ev.type							= XDEVL_WINDOW_EVENT;
							ev.window.event 		= XDEVL_WINDOW_RESIZED;
							ev.window.x					= event.xconfigure.x;
							ev.window.y					= event.xconfigure.y;
							ev.window.width			= event.xconfigure.width;
							ev.window.height		= event.xconfigure.height;
							ev.window.windowid	=  windowMap[event.xconfigure.window]->getWindowID();

							arg->core->fireEvent(ev);
						}
						m_prevConfigureEvent = event.xconfigure;

					}
					break;

					//
					//
					//
				case KeymapNotify: {
					//std::cout << "MappingNotify" << ": WindowID: "  << getWindowID() << std::endl;
					/* FIXME:
					   X11_SetKeyboardState(SDL_Display, xevent.xkeymap.key_vector);
					 */
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

		// Free the keyboard device.
		if(m_keyboard != nullptr) {
			delete m_keyboard;
			m_keyboard = nullptr;
		}

		XDEVL_MODULE_INFO("Stopping Event Thread.");

		return ERR_OK;
	}


	XdevLWindowLinux::XdevLWindowLinux(XdevLModuleCreateParameter* parameter) :
		XdevLWindowImpl(XdevLWindowImpl::getWindowsCounter(), parameter, m_windowX11ModuleDesc),
		m_rootWindow(0),
		m_window(0),
		m_screenNumber(0),
		m_screenWidth(0),
		m_screenHeight(0),
		m_best_fit_width(-1),
		m_best_fit_height(-1),
		m_best_fit_rate(-1),
		m_originalScreenConfig(nullptr) {}

	XdevLWindowLinux::~XdevLWindowLinux() {

	}

	xdl_int XdevLWindowLinux::init() {

		XdevLWindowImpl::init();

		if(create() != ERR_OK) {
			return ERR_ERROR;
		}

		return ERR_OK;
	}
	xdl_int XdevLWindowLinux::create(const XdevLWindowTitle& title,
	                                 const XdevLWindowPosition& position,
	                                 const XdevLWindowSize& size) {
		return ERR_ERROR;
	}

	int XdevLWindowLinux::create() {

		Visual* 							visual;
		XVisualInfo*					vinfo;

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
//		XParseColor(m_display, m_defaultColorMap, green, &color);
//		XAllocColor(m_display, m_defaultColorMap, &color);

//    windowAttributes.background_pixmap 	= None;
//    windowAttributes.border_pixel 			= 0;
//		windowAttributes.colormap 					= XCreateColormap(m_display, m_rootWindow, visual, AllocNone);

		// Check if the RandR extension is avaible.
		if(!XRRQueryVersion(m_display, &m_randrMajor, &m_randrMinor)) {
			XDEVL_MODULE_ERROR("RandR extension missing.\n");
			return ERR_ERROR;
		}

		Colormap colormap = m_defaultColorMap;


		XRRQueryExtension(m_display, &m_event_basep, &m_error_basep);

		if(m_rootTitle.toString().size() > 0) {
			XdevLWindow* rootWindow = static_cast<XdevLWindow*>(getMediator()->getModule(XdevLID(m_rootTitle.toString().c_str())));
			m_rootWindow = (Window)(rootWindow->getInternal(XdevLInternalName("X11_WINDOW")));
		}

		xdl_int borderwith = 0;

		XSetWindowAttributes WindowAttributes;
		WindowAttributes.override_redirect	= True;
		WindowAttributes.background_pixmap	= None;
		WindowAttributes.background_pixel 	= color.pixel;
		WindowAttributes.border_pixel				= 0;
		WindowAttributes.colormap						= CopyFromParent;

		m_window = XCreateWindow(m_display,
		                         m_rootWindow,
		                         m_position.x, m_position.y,
		                         m_size.width, m_size.height,
		                         borderwith,
		                         CopyFromParent,
		                         InputOutput,
		                         CopyFromParent,
		                         CWOverrideRedirect | CWColormap | CWBackPixmap | CWBackPixel,
		                         &WindowAttributes);


		xdl_int event_mask = FocusChangeMask | EnterWindowMask | LeaveWindowMask |
		                     ExposureMask | ButtonPressMask | ButtonReleaseMask |
		                     PointerMotionMask | KeyPressMask | KeyReleaseMask |
		                     PropertyChangeMask | StructureNotifyMask | SubstructureNotifyMask |
		                     KeymapStateMask;


		XSelectInput(m_display, m_window, event_mask);

		XSizeHints*  	size_hints 	= XAllocSizeHints();
		XWMHints*  		wm_hints 		= XAllocWMHints();
		XClassHint*  	class_hints = XAllocClassHint();
		XTextProperty windowName;
		XTextProperty iconName;

		char* window_name = (char*)getTitle().toString().c_str();
		char* icon_name 	= (char*)getTitle().toString().c_str();

		XStringListToTextProperty(&window_name, 1, &windowName);
		XStringListToTextProperty(&icon_name, 1, &iconName);

		size_hints->flags       = USPosition | USSize | PPosition | PSize | PMinSize;
		size_hints->x						= m_position.x;
		size_hints->y						= m_position.y;
		size_hints->width				= m_size.width;
		size_hints->height			= m_size.height;
		size_hints->min_width   = m_size.width;
		size_hints->min_height  = m_size.height;

		wm_hints->flags         = StateHint | InputHint;
		wm_hints->initial_state = NormalState;
		wm_hints->input         = True;

		class_hints->res_name   = (char*)getTitle().toString().c_str();
		class_hints->res_class  = (char*)getTitle().toString().c_str();

		XSetWMProperties(m_display, m_window, &windowName, &iconName, nullptr, 0, size_hints, wm_hints, class_hints);

		XFree(size_hints);
		XFree(wm_hints);
		XFree(class_hints);


		XClearWindow(m_display, m_window);


		// Get all protocols and assign for usage.
		setWMProtocols();

		// Check if the user specified fullscreen mode.
		if(getFullscreen()) {
			setFullscreenVideoMode();
		}

//		XChangeProperty(m_display, m_window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32, PropModeReplace, (unsigned char *)&_NET_WM_WINDOW_TYPE_NORMAL, 1);


		const long _NET_WM_BYPASS_COMPOSITOR_HINT_ON = 1;
		XChangeProperty(m_display, m_window, _NET_WM_BYPASS_COMPOSITOR, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&_NET_WM_BYPASS_COMPOSITOR_HINT_ON, 1);


		// Set the border property.
		setWindowBordered();

		//	showMousePointer(getHidePointer());

//		XMapWindow(m_display, m_window);

		XFlush(m_display);

		// Move window to the specified position. Normaly the window manager would put it
		// somewhere. This is just a cheap trick to position it to the right place.

		//	initKeyboardKeyCodes();

		//
		// Star the main event polling thread.
		//
		if(isEventThreadRunning() == xdl::xdl_false) {

			XdevLWindowThreadArgument* argument = new XdevLWindowThreadArgument();
			argument->core = getMediator();
			Start(argument);
		}

		windowMap[m_window] = this;

		return ERR_OK;
	}

	void* XdevLWindowLinux::getInternal(const char* id) {
		std::string data(id);
		if(data == "X11_DISPLAY")
			return m_display;
		if(data == "X11_WINDOW")
			return (void*)m_window;
		if(data == "X11X_SCREEN_NUMBER")
			return &m_screenNumber;
		return nullptr;
	}

	xdl_int XdevLWindowLinux::shutdown() {

		XDEVL_MODULE_INFO("Starting shutdown process.\n");

		stopEventThread();

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

		// Wait until thread finished it's job.
		Join();

		// Ok, check if the are in fullscreen mode.
		if(getFullscreen()) {
			restoreFullscreenVideoMode();
		}

		Atom atom;
		atom = XInternAtom(m_display, "_WIN_HINTS", True);
		if(atom != None) {
			XDeleteProperty(m_display, m_window, atom);
		}

		// We have to destroy the window. The X-Server can do it automatically but it's better
		// to do it manually.
		if(m_display && m_window && !m_rootWindow)
			XDestroyWindow(m_display, m_window);

		if(XdevLWindowImpl::shutdown() != ERR_OK) {
			return ERR_ERROR;
		}

		XDEVL_MODULE_SUCCESS("Shutdown process was successful.\n");

		return ERR_OK;
	}

	int XdevLWindowLinux::update() {

		return ERR_OK;
	}



	xdl_int XdevLWindowLinux::getGetClosestVideoMode() {
		int i, match, bestmatch;
		int sizecount, bestsize;
		int ratecount;
		short *ratelist;

		XRRScreenSize *sizelist;

		// Get current resolution and frequency.
		m_originalScreenConfig 	= XRRGetScreenInfo(m_display, m_rootWindow);
		m_originalScreenRate 	= XRRConfigCurrentRate(m_originalScreenConfig);
		m_originalSizeId		= XRRConfigCurrentConfiguration(m_originalScreenConfig, &m_originalRotation);

		// Get all supported screen sizes
		sizelist = XRRConfigSizes(m_originalScreenConfig, &sizecount);

		// Find the best matching mode
		bestsize  = -1;
		bestmatch = 999999;
		for(i = 0; i < sizecount; i++) {
			match = (getWidth() - sizelist[i].width) *
			        (getWidth() - sizelist[i].width) +
			        (getHeight() - sizelist[i].height) *
			        (getHeight() - sizelist[i].height);
			if(match < bestmatch) {
				bestmatch = match;
				bestsize  = i;
				m_sizeId = i;
			}
		}

		if(bestsize != -1) {
			// Report width & height of best matching mode
			m_best_fit_width = sizelist[bestsize].width;
			m_best_fit_height = sizelist[bestsize].height;
			if(m_originalScreenRate > 0) {
				ratelist = XRRConfigRates(m_originalScreenConfig, bestsize, &ratecount);

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

		if(bestsize != -1) {
			XDEVL_MODULE_INFO("Found matching screen size: " << sizelist[bestsize].width << "x" << sizelist[bestsize].height << " " << m_best_fit_rate << " Hz" << std::endl);
			return bestsize;
		}

		return 0;
	}

	void XdevLWindowLinux::setFullscreenVideoMode() {
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
		XRRSetScreenConfigAndRate(m_display,
		                          m_originalScreenConfig,
		                          m_rootWindow,
		                          m_sizeId,
		                          m_originalRotation,
		                          (short int)rate,
		                          CurrentTime);

		// Disable screensaver.
		XGetScreenSaver(m_display,
		                &timeout_return,
		                &interval_return,
		                &prefer_blanking_return,
		                &allow_exposures_return);
		// Disable screen saver
		XSetScreenSaver(m_display, 0, 0, DontPreferBlanking, DefaultExposures);
	}

	void XdevLWindowLinux::restoreFullscreenVideoMode() {
		// Change screen mode into it's original form.
		XRRSetScreenConfigAndRate(m_display,
		                          m_originalScreenConfig,
		                          m_rootWindow,
		                          m_originalSizeId,
		                          m_originalRotation,
		                          m_originalScreenRate,
		                          CurrentTime);
		// Enable disabled window manager decoration.
		enableDecoration();
		// Set the previous screen saver.
		XSetScreenSaver(m_display, timeout_return,
		                interval_return, prefer_blanking_return,
		                allow_exposures_return);

		// If we have changed the screen configuration, let's switch back.
		if(m_originalScreenConfig)
			XRRFreeScreenConfigInfo(m_originalScreenConfig);
	}


	xdl_int XdevLWindowLinux::disableDecoration() {
		xdl_bool decorations_removed = false;
		Atom hintAtom;
		// Ok, but now we have to inform the window manager about the fullscreen
		// request. EWMH and ICCCM specification.
		hintAtom = XInternAtom(m_display, "_MOTIF_WM_HINTS", True);
		if(hintAtom != None) {
			struct {
				unsigned long flags;
				unsigned long functions;
				unsigned long decorations;
				long input_mode;
				unsigned long status;
			} MWMHints = { MWM_HINTS_DECORATIONS, 0, 0, 0, 0 };

			XChangeProperty(m_display, m_window, hintAtom, hintAtom,
			                32, PropModeReplace, (unsigned char *)&MWMHints,
			                sizeof(MWMHints)/4);
			decorations_removed = xdl_true;
		}

		hintAtom = XInternAtom(m_display, "KWM_WIN_DECORATION", True);
		if(hintAtom != None) {
			long KWMHints = KDE_tinyDecoration;

			XChangeProperty(m_display,m_window, hintAtom, hintAtom,
			                32, PropModeReplace, (unsigned char *)&KWMHints,
			                sizeof(KWMHints)/4);
			decorations_removed = 1;
		}

		// Now try to set GNOME hints
		hintAtom = XInternAtom(m_display, "_WIN_HINTS", True);
		if(hintAtom != None) {
			long GNOMEHints = 0;

			XChangeProperty(m_display, m_window, hintAtom, hintAtom,
			                32, PropModeReplace, (unsigned char *)&GNOMEHints,
			                sizeof(GNOMEHints)/4);
			decorations_removed = 1;
		}

		// Now try to set KDE NET_WM hints
		hintAtom = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE", True);
		if(hintAtom != None) {
			Atom NET_WMHints[2];

			NET_WMHints[0] = XInternAtom(m_display, "_KDE_NET_WM_WINDOW_TYPE_OVERRIDE", True);
			// define a fallback...
			NET_WMHints[1] = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_NORMAL", True);

			XChangeProperty(m_display, m_window, hintAtom, XA_ATOM,
			                32, PropModeReplace, (unsigned char *)&NET_WMHints,
			                2);
			decorations_removed = 1;
		}

		hintAtom = XInternAtom(m_display, "_NET_WM_STATE", True);
		if(hintAtom != None) {
			Atom NET_WMHints[1];

			NET_WMHints[0] = XInternAtom(m_display, "_NET_WM_STATE_FULLSCREEN", True);

			XChangeProperty(m_display, m_window, hintAtom, XA_ATOM, 32, PropModeReplace, (unsigned char *)&NET_WMHints, 1);
			decorations_removed = xdl_true;
		}

		// Did we sucessfully remove the window decorations?
		if(decorations_removed) {
			// Finally set the transient hints
			XSetTransientForHint(m_display, m_window, m_rootWindow);
			XUnmapWindow(m_display, m_window);
			XMapWindow(m_display, m_window);
		} else {
			XSetWindowAttributes  attributes;
			// The Butcher way of removing window decorations
			attributes.override_redirect = True;
			XChangeWindowAttributes(m_display, m_window,
			                        CWOverrideRedirect, &attributes);
		}

		XRRSelectInput(m_display, m_window, RRScreenChangeNotifyMask);

		return ERR_OK;
	}

	xdl_int XdevLWindowLinux::enableDecoration() {
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

		// Finally unset the transient hints if necessary
		if(ActivatedDecorations) {
			// NOTE: Does this work?
			XSetTransientForHint(m_display, m_window, None);
			XUnmapWindow(m_display, m_window);
			XMapWindow(m_display, m_window);
		}
		return ERR_OK;
	}

	void XdevLWindowLinux::setFullscreen(xdl_bool state) {
		if(state) {
			setFullscreenVideoMode();
		} else {
			restoreFullscreenVideoMode();
		}
	}

	const XdevLWindowPosition& XdevLWindowLinux::getPosition() {
		XWindowAttributes wa;
		XGetWindowAttributes(m_display, m_window, &wa);
		m_position.x = wa.x;
		m_position.y = wa.y;
		return m_position;
	}

	const XdevLWindowSize& XdevLWindowLinux::getSize() {
		XWindowAttributes wa;
		XGetWindowAttributes(m_display, m_window, &wa);
		m_size.width 	= wa.width;
		m_size.height = wa.height;
		return m_size;
	}

	XdevLWindowSize::type  XdevLWindowLinux::getWidth() {
		XWindowAttributes wa;
		XGetWindowAttributes(m_display, m_window, &wa);
		m_size.width 	= wa.width;
		m_size.height = wa.height;
		return m_size.width;
	}

	XdevLWindowSize::type  XdevLWindowLinux::getHeight() {
		XWindowAttributes wa;
		XGetWindowAttributes(m_display, m_window, &wa);
		m_size.width 	= wa.width;
		m_size.height = wa.height;
		return m_size.height;
	}

	XdevLWindowPosition::type XdevLWindowLinux::getX() {
		XWindowAttributes wa;
		XGetWindowAttributes(m_display, m_window, &wa);
		m_position.x = wa.x;
		m_position.y = wa.y;
		return m_position.x;
	}

	XdevLWindowPosition::type XdevLWindowLinux::getY() {
		XWindowAttributes wa;
		XGetWindowAttributes(m_display, m_window, &wa);
		m_position.x = wa.x;
		m_position.y = wa.y;
		return m_position.y;
	}

	const XdevLWindowTitle& XdevLWindowLinux::getTitle() {
		return XdevLWindowImpl::getTitle();
	}

	xdl_bool XdevLWindowLinux::getFullscreen() {
		return XdevLWindowImpl::getFullscreen();
	}

	xdl_bool XdevLWindowLinux::getHidePointer() {
		return XdevLWindowImpl::getHidePointer();
	}

	xdl_int XdevLWindowLinux::getColorDepth() {
		return XdevLWindowImpl::getColorDepth();
	}

	void XdevLWindowLinux::setX(XdevLWindowPosition::type x) {
		XdevLWindowImpl::setX(x);
		XMoveWindow(m_display, m_window, m_position.x, m_position.y);
		XMapWindow(m_display, m_window);
	}

	void XdevLWindowLinux::setY(XdevLWindowPosition::type y) {
		XdevLWindowImpl::setY(y);
		XMoveWindow(m_display, m_window, m_position.x, m_position.y);
		XMapWindow(m_display, m_window);
	}

	void XdevLWindowLinux::setWidth(XdevLWindowSize::type width) {
		XdevLWindowImpl::setWidth(width);
		XResizeWindow(m_display, m_window, m_size.width, m_size.height);
		XMapWindow(m_display, m_window);
	}

	void XdevLWindowLinux::setHeight(XdevLWindowSize::type height) {
		XdevLWindowImpl::setHeight(height);
		XResizeWindow(m_display, m_window,  m_size.width, m_size.height);
		XMapWindow(m_display, m_window);
	}

	void XdevLWindowLinux::setSize(const XdevLWindowSize& size) {
		m_size = size;
		XResizeWindow(m_display, m_window, m_size.width, m_size.height);
		XMapWindow(m_display, m_window);
	}

	void XdevLWindowLinux::setPosition(const XdevLWindowPosition& position) {
		m_position = position;
		XMoveWindow(m_display, m_window, m_position.x, m_position.y);
		XMapWindow(m_display, m_window);
	}

	void XdevLWindowLinux::setTitle(const XdevLWindowTitle& title) {
		XdevLWindowImpl::setTitle(title);
	}

	void XdevLWindowLinux::showPointer() {
		showMousePointer(0);
	}

	void XdevLWindowLinux::hidePointer() {
		showMousePointer(1);
	}

	void XdevLWindowLinux::showMousePointer(xdl_bool state) {

		if(state) {
			Pixmap bm_no;
			Colormap cmap;
			Cursor no_ptr;
			XColor black, dummy;

			cmap 		= m_defaultColorMap;

			// Lets first create black color for the specific color map we use.
			XAllocNamedColor(m_display, cmap, "black", &black, &dummy);

			// Now we create an empty bitmap where we will fill nothing with the color.
			static char bm_no_data[] = {0, 0, 0, 0, 0, 0, 0, 0};
			bm_no 	= XCreateBitmapFromData(m_display, m_window, bm_no_data, 8, 8);

			// Now create the new cursor bitmap which is of course back, transparent.
			no_ptr 	= XCreatePixmapCursor(m_display, bm_no, bm_no, &black, &black, 0, 0);

			// Set the new shape of the cursor.
			XDefineCursor(m_display, m_window, no_ptr);

			// Free everything.
			XFreeCursor(m_display, no_ptr);

			if(bm_no != None) {
				XFreePixmap(m_display, bm_no);
			}

			XFreeColors(m_display, cmap, &black.pixel, 1, 0);
		} else
			XUndefineCursor(m_display, m_window);
	}

	void XdevLWindowLinux::show() {
		XMapWindow(m_display, m_window);

		//XEvent event;
		//XIfEvent(m_display, &event, &isMapNotify, (XPointer)&m_window);
		XFlush(m_display);
	}

	void XdevLWindowLinux::hide() {
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

	void XdevLWindowLinux::raise() {
		XRaiseWindow(m_display, m_window);
		//XFlush(m_display);
	}

	void XdevLWindowLinux::setPointerPosition(xdl_uint x, xdl_uint y) {
		XWindowAttributes wa;
		XGetWindowAttributes(m_display, m_window, &wa);
		// (display, src window, dst windiw, src x, src y, src width, src height, dst x, dst y)
		XWarpPointer(m_display, m_window, m_window, 0, 0, wa.width, wa.height, x, y);
	}

	void XdevLWindowLinux::clipPointerPosition(xdl_uint x, xdl_uint y, xdl_uint width, xdl_uint height) {
		//XReparentWindow(m_display, m_clipWindow, m_window, 0, 0);
		//XResizeWindow(m_display, m_clipWindow,  width, height);
		//XMoveWindow(m_display, m_clipWindow, x, y);

		XSizeHints*  	size_hints 	= XAllocSizeHints();
		XWMHints*  		wm_hints 		= XAllocWMHints();
		XClassHint*  	class_hints = XAllocClassHint();


		size_hints->flags       = USPosition | USSize | PPosition | PSize | PMinSize;
		size_hints->x						= x;
		size_hints->y						= y;
		size_hints->width				= width;
		size_hints->height			= height;
		size_hints->min_width   = width;
		size_hints->min_height  = height;

		wm_hints->flags         = StateHint | InputHint;
		wm_hints->initial_state = NormalState;
		wm_hints->input         = False;


		XSetWMProperties(m_display, m_clipWindow, nullptr, nullptr, nullptr, 0, size_hints, nullptr,nullptr);
		XMapWindow(m_display,m_clipWindow);
		XFlush(m_display);

		XFree(size_hints);
		XFree(wm_hints);
		XFree(class_hints);

//	Window window =	XCreateSimpleWindow(m_display,
//		                                m_rootWindow,
//		                                x, y,
//																		width, height,
//		                                0,
//		                                BlackPixel(m_display, m_screenNumber),
//																		BlackPixel(m_display, m_screenNumber));
//
//
//		xdl_int event_mask = FocusChangeMask | EnterWindowMask | LeaveWindowMask |
//		                     ExposureMask | ButtonPressMask | ButtonReleaseMask |
//		                     PointerMotionMask | KeyPressMask | KeyReleaseMask |
//		                     PropertyChangeMask | StructureNotifyMask | SubstructureNotifyMask |
//		                     KeymapStateMask;
//
//
//		XSelectInput(m_display, window, event_mask);
		//	XMapWindow(m_display,window);
		//	XFlush(m_display);
		for(;;) {
			static int counter = 0;

			int result = XGrabPointer(m_display, m_window, False, 0, GrabModeAsync, GrabModeAsync, m_clipWindow, None, CurrentTime);
			if(result == GrabSuccess) {
				std::cout << "result == GrabSuccess: " << counter++ << std::endl;
				break;
			}
		}

	}

	xdl_bool XdevLWindowLinux::isHidden() {
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

	xdl_uint32 XdevLWindowLinux::getNetWMState() {
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

		/* FIXME, check the size hints for resizable */
		/* flags |= SDL_WINDOW_RESIZABLE; */

		return flags;
	}

	void XdevLWindowLinux::setWindowBordered() {

		if(_MOTIF_WM_HINTS != None) {
			/* Hints used by Motif compliant window managers */
			struct {
				unsigned long flags;
				unsigned long functions;
				unsigned long decorations;
				long input_mode;
				unsigned long status;
			} MWMHints = {
				(1 << 1), 0, static_cast<unsigned long>((m_border == xdl_true) ? 1 : 0), 0, 0
			};

			XChangeProperty(m_display, m_window, _MOTIF_WM_HINTS, _MOTIF_WM_HINTS, 32,
			                PropModeReplace, (unsigned char *) &MWMHints,
			                sizeof(MWMHints) / 4);
		} else {  /* set the transient hints instead, if necessary */
			XSetTransientForHint(m_display, m_window, m_rootWindow);
		}
	}

	void XdevLWindowLinux::grabPointer() {
		for(;;) {
			static int counter = 0;
			int result = XGrabPointer(m_display, m_window, True, 0, GrabModeAsync, GrabModeAsync, m_window, None, CurrentTime);

			if(result == GrabSuccess) {
				std::cout << "result == GrabSuccess: Grab trials: " << counter++ << std::endl;
				counter = 0;
				break;
			}
		}
		XFlush(m_display);
	}

	void XdevLWindowLinux::ungrabPointer() {
		XUngrabPointer(m_display, CurrentTime);
	}

	void XdevLWindowLinux::grabKeyboard() {
		for(;;) {
			static int counter = 0;
			int result = XGrabKeyboard(m_display, m_window, True, GrabModeAsync, GrabModeAsync, CurrentTime);

			if(result == GrabSuccess) {
				std::cout << "result == GrabSuccess: Grab trials: " << counter++ << std::endl;
				counter = 0;
				break;
			}
		}
		XFlush(m_display);
	}

	void XdevLWindowLinux::ungrabKeyboard() {
		XUngrabKeyboard(m_display, CurrentTime);
	}

	void XdevLWindowLinux::setInputFocus() {
		XFlush(m_display);

//			XEvent event;
//			XIfEvent(m_display, &event, &isConfigureNotify, (XPointer)&m_window);
//


//		XSync(m_display, False);
//
		XWindowAttributes attribute;
		XGetWindowAttributes(m_display,m_window,&attribute);
		if(attribute.map_state == IsViewable) {
			XSetInputFocus(m_display, m_window,  RevertToParent, CurrentTime);
		}



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

	xdl_bool XdevLWindowLinux::hasFocus() {
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

	xdl_int XdevLWindowLinux::getInputFocus(XdevLWindow** window) {
		Window wnd;
		int revert_to;
		XGetInputFocus(m_display, &wnd, &revert_to);

		*window = windowMap[wnd];
		return ERR_OK;

		*window = nullptr;
		return ERR_ERROR;
	}

	void XdevLWindowLinux::setParent(XdevLWindow* window) {
		XdevLWindowImpl::setParent(window);
	}

	xdl_int XdevLWindowLinux::setWMProtocols() {

		_MOTIF_WM_HINTS 				= XInternAtom(m_display, "_MOTIF_WM_HINTS", False);
		_NET_WM_WINDOW_TYPE					= XInternAtom(m_display, "_NET_WM_WINDOW_TYPE", False);;
		_NET_WM_WINDOW_TYPE_DESKTOP			= XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_DESKTOP", False);;
		_NET_WM_WINDOW_TYPE_DOCK			= XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_DOCK", False);;
		_NET_WM_WINDOW_TYPE_TOOLBAR			= XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_TOOLBAR", False);;
		_NET_WM_WINDOW_TYPE_MENU			= XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_MENU", False);;
		_NET_WM_WINDOW_TYPE_UTILITY			= XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_UTILITY", False);;
		_NET_WM_WINDOW_TYPE_SPLASH			= XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_SPLASH", False);;
		_NET_WM_WINDOW_TYPE_DIALOG			= XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_DIALOG", False);;
		_NET_WM_WINDOW_TYPE_DROPDOWN_MENU	= XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU", False);
		_NET_WM_WINDOW_TYPE_POPUP_MENU		= XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_POPUP_MENU", False);
		_NET_WM_WINDOW_TYPE_TOOLTIP			= XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_TOOLTIP", False);
		_NET_WM_WINDOW_TYPE_NOTIFICATION	= XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_NOTIFICATION", False);
		_NET_WM_WINDOW_TYPE_COMBO			= XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_COMBO", False);
		_NET_WM_WINDOW_TYPE_DND				= XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_DND", False);
		_NET_WM_WINDOW_TYPE_NORMAL			= XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_NORMAL", False);
		_NET_WM_WINDOW_OPACITY 				= XInternAtom(m_display, "_NET_WM_WINDOW_OPACITY", False);
		_NET_WM_BYPASS_COMPOSITOR	= XInternAtom(m_display, "_NET_WM_BYPASS_COMPOSITOR", False);
		_NET_WM_ACTION_RESIZE			= XInternAtom(m_display, "_NET_WM_ACTION_RESIZE", False);
		_NET_WM_STATE					= XInternAtom(m_display, "_NET_WM_STATE", False);
		_NET_WM_STATE_MAXIMIZED_VERT	= XInternAtom(m_display, "_NET_WM_STATE_MAXIMIZED_VERT", False);
		_NET_WM_STATE_MAXIMIZED_HORZ	= XInternAtom(m_display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
		_NET_WM_STATE_FULLSCREEN		= XInternAtom(m_display, "_NET_WM_STATE_FULLSCREEN", False);
		_NET_WM_STATE_HIDDEN			= XInternAtom(m_display, "_NET_WM_STATE_HIDDEN", False);
		_NET_WM_STATE_FOCUSED			= XInternAtom(m_display, "_NET_WM_STATE_FOCUSED", False);
		_NET_WM_STATE_ADD				= XInternAtom(m_display, "_NET_WM_STATE_ADD", False);
		_NET_WM_STATE_REMOVE			= XInternAtom(m_display, "_NET_WM_STATE_REMOVE", False);
		_NET_WM_NAME					= XInternAtom(m_display, "_NET_WM_NAME", False);
		_NET_WM_ICON_NAME				= XInternAtom(m_display, "_NET_WM_ICON_NAME", False);
		_NET_WM_ICON					= XInternAtom(m_display, "_NET_WM_ICON", False);
		_NET_WM_PING					= XInternAtom(m_display, "_NET_WM_PING", False);

		WM_STATE						= XInternAtom(m_display, "WM_STATE", False);
		WM_NAME							= XInternAtom(m_display, "WM_NAME", False);
		WM_NORMAL_HINTS					= XInternAtom(m_display, "WM_NORMAL_HINTS", False);
		WM_HINTS						= XInternAtom(m_display, "WM_HINTS", False);
		WM_ICON_SIZE					= XInternAtom(m_display, "WM_ICON_SIZE", False);
		WM_DELETE_WINDOW				= XInternAtom(m_display, "WM_DELETE_WINDOW", False);
		WM_PROTOCOLS					= XInternAtom(m_display, "WM_PROTOCOLS", False);
		WM_TRANSIENT_FOR				= XInternAtom(m_display, "WM_TRANSIENT_FOR", False);
		WM_CLASS						= XInternAtom(m_display, "WM_CLASS", False);

		XdndEnter						= XInternAtom(m_display, "XdndEnter", False);
		XdndPosition					= XInternAtom(m_display, "XdndPosition", False);
		XdndStatus						= XInternAtom(m_display, "XdndStatus", False);
		XdndTypeList					= XInternAtom(m_display, "XdndTypeList", False);
		XdndActionCopy					= XInternAtom(m_display, "XdndActionCopy", False);
		XdndDrop						= XInternAtom(m_display, "XdndDrop", False);
		XdndFinished					= XInternAtom(m_display, "XdndFinished", False);
		XdndSelection					= XInternAtom(m_display, "XdndSelection", False);





		// Replace the WM_PROTOCOLS property on this window.

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



		XSetWMProtocols(m_display, m_window, protocols.data(), protocols.size());
		return ERR_OK;
	}

	void XdevLWindowLinux::SetType(XdevLWindowTypes type) {

		switch(type) {

			case WINDOW_NORMAL: {
				if(_NET_WM_WINDOW_TYPE_NORMAL != 0) {
					XChangeProperty(m_display, m_window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32,PropModeReplace, (unsigned char *)&_NET_WM_WINDOW_TYPE_NORMAL, 1);
				} else {
				}
			}
			break;
			case WINDOW_TOOLTIP: {
				if(_NET_WM_WINDOW_TYPE_TOOLTIP != 0) {
					XChangeProperty(m_display, m_window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32,PropModeReplace, (unsigned char *)&_NET_WM_WINDOW_TYPE_TOOLTIP, 1);
				} else {
				}
			}
			break;
			case WINDOW_DROPDOWN_MENU: {
				if(_NET_WM_WINDOW_TYPE_DROPDOWN_MENU != 0) {
					XChangeProperty(m_display, m_window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32,PropModeReplace, (unsigned char *)&_NET_WM_WINDOW_TYPE_DROPDOWN_MENU, 1);
				} else {
				}
			}
			break;
			case WINDOW_SPLASH: {
				if(_NET_WM_WINDOW_TYPE_SPLASH != 0) {
					XChangeProperty(m_display, m_window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32,PropModeReplace, (unsigned char *)&_NET_WM_WINDOW_TYPE_SPLASH, 1);
				} else {
				}
			}
			break;
			case WINDOW_UNKNOWN:
			default:
				break;
		}

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
	        const XdevLWindowSize& size) {

		*window = new XdevLWindowLinux(nullptr);

		return ERR_OK;
	}

}
