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
#include <XdevLCore.h>
#include <XdevLKeyboard/XdevLKeyboard.h>
#include <XdevLMouse/XdevLMouse.h>
#include <XdevLJoystick/XdevLJoystick.h>
#include "XdevLWindowSDL.h"
#include <XdevLXstring.h>
#include <XdevLUtils.h>
#include <sstream>
#include <cstddef>
#include <map>


xdl::XdevLModuleDescriptor xdl::XdevLWindowSDL::m_windowSDLModuleDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[0],
	xdl::window_copyright,
	xdl::windowDescription,
	xdl::XdevLWindowMajorVersion,
	xdl::XdevLWindowMinorVersion,
	xdl::XdevLWindowPatchVersion
};

xdl::XdevLPluginDescriptor m_windowSDLPluginDescriptor {
	xdl::windowPluginName,
	xdl::window_moduleNames,
	xdl::XdevLWindowPluginMajorVersion,
	xdl::XdevLWindowPluginMinorVersion,
	xdl::XdevLWindowPluginPatchVersion
};


static xdl::xdl_int reference_counter = 0;
static std::map<xdl::xdl_uint32, xdl::XdevLWindow*> windowMap;

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	// Only init SDL if this is the first module.
	if(reference_counter == 0) {
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
	}

	if(xdl::XdevLWindowSDL::m_windowSDLModuleDesc.getName() == parameter->getModuleName()) {
		xdl::XdevLWindowSDL* window = new xdl::XdevLWindowSDL(parameter);
		parameter->setModuleInstance(window);

	} else if(xdl::XdevLWindowServerImpl::m_windowServerModuleDesc.getName() == parameter->getModuleName()) {
		xdl::XdevLWindowServerSDL* windowServer = new xdl::XdevLWindowServerSDL(parameter);
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

	// Only Quit SDL if this is the last module.
	if(reference_counter == 0) {
		SDL_Quit();
	}
}

extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor()  {
	return &m_windowSDLPluginDescriptor;
}

namespace xdl {


	XdevLWindowSDL::XdevLWindowSDL(XdevLModuleCreateParameter* parameter) :
		XdevLWindowImpl(XdevLWindowImpl::getWindowsCounter(), parameter, m_windowSDLModuleDesc), m_joy(NULL) {}

	XdevLWindowSDL::~XdevLWindowSDL() {

	}


	xdl_int XdevLWindowSDL::init() {

		XdevLWindowImpl::init();

		if(create() != ERR_OK) {
			XDEVL_MODULE_ERROR("Could not create SDL window: " << SDL_GetError() << std::endl);

			return ERR_ERROR;
		}
		XDEVL_MODULE_SUCCESS("SDL window created successfully: " << m_window << std::endl);

		return ERR_OK;
	}

	xdl_int XdevLWindowSDL::create() {

		xdl_uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN;
		if(!m_border) {
			flags |= SDL_WINDOW_BORDERLESS;
		}

		auto title = XdevLWindowImpl::getTitle();

		m_window = SDL_CreateWindow(title.toString().c_str(),
		                            XdevLWindowImpl::getX(),
		                            XdevLWindowImpl::getY(),
		                            XdevLWindowImpl::getWidth(),
		                            XdevLWindowImpl::getHeight(), flags);

		if(m_window == nullptr) {
			return ERR_ERROR;
		}

		SDL_Renderer* renderer = SDL_CreateRenderer(m_window, -1, 0);
		SDL_SetRenderDrawColor(renderer, m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
		SDL_DestroyRenderer(renderer);

		setHidePointer(XdevLWindowImpl::getHidePointer());
		setFullscreen(XdevLWindowImpl::getFullscreen());


		SDL_VERSION(&m_wmInfo.version);
		SDL_GetWindowWMInfo(m_window,&m_wmInfo);
		SDL_UpdateWindowSurface(m_window);


		m_numberOfJoystickDevices = SDL_NumJoysticks();
		if(m_numberOfJoystickDevices < 1) {
			XDEVL_MODULE_WARNING("No joystick detected.\n");
		} else {
			m_joy = SDL_JoystickOpen(0);
			if(m_joy == nullptr) {
				//	XDEVL_MODULE_WARNING("No joystick detected." << std::endl);
			} else {
				m_numberOfJoystickButtons = SDL_JoystickNumButtons(m_joy);
				m_numberOfJoystickAxis = SDL_JoystickNumAxes(m_joy);
				SDL_JoystickEventState(SDL_ENABLE);
			}
		}

		windowMap[SDL_GetWindowID(m_window)] = this;

		//
		// Star the main event polling thread.
		//
		if(isEventThreadRunning() == xdl::xdl_false) {

			XdevLWindowThreadArgument* argument = new XdevLWindowThreadArgument();
			argument->core = getMediator();
			Start(argument);
		}

		return ERR_OK;
	}
	void XdevLWindowSDL::setParent(XdevLWindow* window) {

		XdevLWindowImpl::setParent(window);
	}

	void* XdevLWindowSDL::getInternal(const XdevLInternalName& id) {

		// Return the SDL_window
		if(id.toString() == "SDL_Window") {
			return (void*)m_window;
		}

#ifdef __LINUX__
		if(id.toString() == "UNIX_DISPLAY") {
			return m_wmInfo.info.x11.display;
		}
		if(id.toString() == "UNIX_WINDOW") {
			return &m_wmInfo.info.x11.window;
		}
#endif

#ifdef WIN32
		if(id.toString() == "UNIX_DISPLAY") {
			return m_wmInfo.info.win.window;
		}
#endif

#ifdef __MACOSX__
		if(id.toString() == "MACOSX_WINDOW") {
			return m_wmInfo.info.cocoa.window;
		}
#endif

		return nullptr;
	}

	xdl_int XdevLWindowSDL::shutdown() {

		XDEVL_MODULE_INFO("Starting shutdown process.\n");

		stopEventThread();

		Join();

		if(m_joy != nullptr) {
			SDL_JoystickClose(m_joy);
		}
		SDL_DestroyWindow(m_window);

		XdevLWindowImpl::shutdown();

		XDEVL_MODULE_SUCCESS("Shutdown process was successful.\n");

		return ERR_OK;
	}

	xdl_int XdevLWindowSDL::update() {
		return pollEvents();
	}

	xdl_int XdevLWindowSDL::pollEvents() {

		SDL_Event event;
		while(SDL_PollEvent(&event)) {

			switch(event.type) {
				case SDL_KEYUP: {

					// Register button up event in the Core.
					XdevLEvent ev;
					ev.type 			= XDEVL_KEY_RELEASED;
					ev.common.timestamp	= getMediator()->getTimer().getTime64();

					ev.key.windowid		=  windowMap[event.key.windowID]->getWindowID();
					ev.key.repeat 		= event.key.repeat;
					ev.key.scancode 	= event.key.keysym.scancode;
					ev.key.sym 			= event.key.keysym.sym;
					ev.key.mod 			= event.key.keysym.mod;

					getMediator()->fireEvent(ev);

				}
				break;
				case SDL_KEYDOWN: {

					// Register button up event in the Core.
					XdevLEvent ev;
					ev.type 			= XDEVL_KEY_PRESSED;
					ev.common.timestamp	= getMediator()->getTimer().getTime64();


					ev.key.windowid		= windowMap[event.key.windowID]->getWindowID();
					ev.key.repeat 		= event.key.repeat;
					ev.key.scancode 	= event.key.keysym.scancode;
					ev.key.sym 			= event.key.keysym.sym;
					ev.key.mod 			= event.key.keysym.mod;

					getMediator()->fireEvent(ev);


				}
				break;
				case SDL_MOUSEBUTTONDOWN: {
					// Register button up event in the Core.
					XdevLEvent ev;
					ev.common.timestamp 	= getMediator()->getTimer().getTime64();
					ev.type 				= XDEVL_MOUSE_BUTTON_PRESSED;

					ev.button.windowid		=  windowMap[event.key.windowID]->getWindowID();
					ev.button.button		= event.button.button;
					ev.button.x				= event.button.x;
					ev.button.y				= event.button.y;

					getMediator()->fireEvent(ev);

				}
				break;
				case SDL_MOUSEBUTTONUP: {
					// Register button up event in the Core.
					XdevLEvent ev;
					ev.common.timestamp 	= getMediator()->getTimer().getTime64();
					ev.type 				= XDEVL_MOUSE_BUTTON_RELEASED;

					ev.button.windowid		=  windowMap[event.key.windowID]->getWindowID();
					ev.button.button		= event.button.button;
					ev.button.x				= event.button.x;
					ev.button.y				= event.button.y;

					getMediator()->fireEvent(ev);

				}
				break;
				case SDL_MOUSEMOTION: {
					// Register button up event in the Core.
					XdevLEvent ev;
					ev.common.timestamp 	= getMediator()->getTimer().getTime64();
					ev.type 				= XDEVL_MOUSE_MOTION;

					ev.motion.windowid		=  windowMap[event.key.windowID]->getWindowID();
					ev.motion.x				= event.motion.x;
					ev.motion.y				= event.motion.y;
					ev.motion.xrel			= event.motion.xrel;
					ev.motion.yrel			= event.motion.yrel;

					getMediator()->fireEvent(ev);
				}
				break;
				case SDL_JOYAXISMOTION: {
					// Register button up event in the Core.
					XdevLEvent ev;
					ev.common.timestamp 	= getMediator()->getTimer().getTime64();
					ev.type 				= XDEVL_JOYSTICK_MOTION;
					ev.jaxis.axis			= event.jaxis.axis;
					ev.jaxis.value			= event.jaxis.value;

					getMediator()->fireEvent(ev);

				}
				break;
				case SDL_JOYBUTTONDOWN: {
					// Register button up event in the Core.
					XdevLEvent ev;
					ev.common.timestamp 	= getMediator()->getTimer().getTime64();
					ev.type 				= XDEVL_JOYSTICK_BUTTON_PRESSED;
					ev.button.button		= event.jbutton.button;

					getMediator()->fireEvent(ev);

				}
				break;
				case SDL_JOYBUTTONUP: {
					// Register button up event in the Core.
					XdevLEvent ev;
					ev.common.timestamp 	= getMediator()->getTimer().getTime64();
					ev.type 				= XDEVL_JOYSTICK_BUTTON_RELEASED;
					ev.button.button		= event.jbutton.button;

					getMediator()->fireEvent(ev);

				}
				break;
				case SDL_JOYHATMOTION: {
					// Register button up event in the Core.
					XdevLEvent ev;
					ev.common.timestamp 	= getMediator()->getTimer().getTime64();
					ev.type 				= XDEVL_JOYSTICK_POV;

					switch(event.jhat.value) {
						case SDL_HAT_UP:
							ev.jpov.direction = POV_UP;
							break;
						case SDL_HAT_RIGHTUP:
							ev.jpov.direction = POV_RIGHT_UP;
							break;
						case SDL_HAT_RIGHT:
							ev.jpov.direction = POV_RIGHT;
							break;
						case SDL_HAT_RIGHTDOWN:
							ev.jpov.direction = POV_RIGHT_DOWN;
							break;
						case SDL_HAT_DOWN:
							ev.jpov.direction = POV_DOWN;
							break;
						case SDL_HAT_LEFTDOWN:
							ev.jpov.direction = POV_LEFT_DOWN;
							break;
						case SDL_HAT_LEFT:
							ev.jpov.direction = POV_LEFT;
							break;
						case SDL_HAT_LEFTUP:
							ev.jpov.direction = POV_LEFT_UP;
							break;
						case SDL_HAT_CENTERED:
							ev.jpov.direction = POV_CENTERED;
							break;
						default:
							break;
					}

					getMediator()->fireEvent(ev);

				}
				break;
				// Here we do some window event handling.
				case SDL_WINDOWEVENT: {
					switch(event.window.event) {
						case SDL_WINDOWEVENT_SHOWN: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_SHOWN;
							ev.window.windowid	=  windowMap[event.key.windowID]->getWindowID();

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_HIDDEN: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_HIDDEN;
							ev.window.windowid	=  windowMap[event.key.windowID]->getWindowID();

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_EXPOSED: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_EXPOSED;
							ev.window.windowid	=  windowMap[event.key.windowID]->getWindowID();

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_MOVED: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_MOVED;
							ev.window.x			= event.window.data1;
							ev.window.y			= event.window.data2;
							ev.window.windowid	=  windowMap[event.key.windowID]->getWindowID();

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_RESIZED: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_RESIZED;
							ev.window.width		= event.window.data1;
							ev.window.height	= event.window.data2;
							ev.window.windowid	=  windowMap[event.key.windowID]->getWindowID();

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_MINIMIZED: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_MINIMIZED;
							ev.window.windowid	=  windowMap[event.key.windowID]->getWindowID();

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_MAXIMIZED: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_MAXIMIZED;
							ev.window.windowid	=  windowMap[event.key.windowID]->getWindowID();

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_RESTORED: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_RESTORED;
							ev.window.windowid	=  windowMap[event.key.windowID]->getWindowID();

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_ENTER: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_ENTER;
							ev.window.data1		= event.window.data1;
							ev.window.data2		= event.window.data2;
							ev.window.windowid	=  windowMap[event.key.windowID]->getWindowID();

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_LEAVE: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_LEAVE;
							ev.window.data1		= event.window.data1;
							ev.window.data2		= event.window.data2;
							ev.window.windowid	=  windowMap[event.key.windowID]->getWindowID();

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_FOCUS_GAINED: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_INPUT_FOCUS_GAINED;
							ev.window.windowid	=  windowMap[event.key.windowID]->getWindowID();

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_FOCUS_LOST: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_INPUT_FOCUS_LOST;
							ev.window.windowid	=  windowMap[event.key.windowID]->getWindowID();

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_CLOSE: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_CLOSE;
							ev.window.windowid	=  windowMap[event.key.windowID]->getWindowID();

							getMediator()->fireEvent(ev);

							// Make a core event.
							ev.type				= XDEVL_CORE_EVENT;
							ev.core.event 		= XDEVL_CORE_SHUTDOWN;

							getMediator()->fireEvent(ev);

							printf("SDL_WINDOWEVENT_CLOSE\n");
						}
						break;
						default:
							break;
					}

				}
				break;
				default:
					//	XDEVL_MODULE_ERROR("Unhandled event: " << event.type << std::endl);
					break;
			}
		}

		return ERR_OK;
	}

	void XdevLWindowSDL::setSize(const XdevLWindowSize& size) {
		SDL_SetWindowSize(m_window, size.width, size.height);
	}

	void XdevLWindowSDL::setPosition(const XdevLWindowPosition& position) {
		SDL_SetWindowPosition(m_window, position.x, position.y);
	}

	void XdevLWindowSDL::setTitle(const XdevLWindowTitle& title) {
		XdevLWindowImpl::setTitle(title);

		SDL_SetWindowTitle(m_window, title.toString().c_str());
	}

	void XdevLWindowSDL::setFullscreen(xdl_bool state) {

		int flags {0};
		if(state) {
			flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		SDL_SetWindowFullscreen(m_window, flags);
		SDL_GetWindowSize(m_window, &m_size.width, &m_size.height);
	}

	void XdevLWindowSDL::SetType(XdevLWindowTypes type) {

	}

	void XdevLWindowSDL::showPointer() {
		SDL_ShowCursor(1);
	}

	void XdevLWindowSDL::hidePointer() {
		SDL_ShowCursor(0);
	}

	void XdevLWindowSDL::setX(XdevLWindowPosition::type x) {
		xdl_int tmp, y;
		SDL_GetWindowPosition(m_window, &tmp, &y);
		SDL_SetWindowPosition(m_window, x, y);
	}

	void XdevLWindowSDL::setY(XdevLWindowPosition::type y) {
		xdl_int x, tmp;
		SDL_GetWindowPosition(m_window, &x, &tmp);
		SDL_SetWindowPosition(m_window, x, y);
	}

	void XdevLWindowSDL::setWidth(XdevLWindowSize::type width) {
		xdl_int tmp, height;
		SDL_GetWindowSize(m_window, &tmp, &height);
		SDL_SetWindowSize(m_window, width, height);
	}

	void XdevLWindowSDL::setHeight(XdevLWindowSize::type height) {
		xdl_int width, tmp;
		SDL_GetWindowSize(m_window, &width, &tmp);
		SDL_SetWindowSize(m_window, width, height);
	}

	XdevLWindowPosition::type XdevLWindowSDL::getX() {
		SDL_GetWindowPosition(m_window, &m_position.x, &m_position.y);
		return m_position.x;
	}

	XdevLWindowPosition::type XdevLWindowSDL::getY() {
		SDL_GetWindowPosition(m_window, &m_position.x, &m_position.y);
		return m_position.y;
	}

	XdevLWindowSize::type XdevLWindowSDL::getWidth() {
		SDL_GetWindowSize(m_window, &m_size.width, &m_size.height);
		return m_size.width;
	}

	XdevLWindowSize::type XdevLWindowSDL::getHeight() {
		SDL_GetWindowSize(m_window, &m_size.width, &m_size.height);
		return m_size.height;
	}

	const XdevLWindowSize& XdevLWindowSDL::getSize() {
		SDL_GetWindowSize(m_window, &m_size.width, &m_size.height);
		return m_size;
	}

	const XdevLWindowPosition& XdevLWindowSDL::getPosition() {
		SDL_GetWindowPosition(m_window, &m_position.x, &m_position.y);
		return m_position;
	}

	const XdevLWindowTitle& XdevLWindowSDL::getTitle() {
		m_title = XdevLWindowTitle(SDL_GetWindowTitle(m_window));
		return m_title;
	}

	xdl_bool  XdevLWindowSDL::getFullscreen() {
		return XdevLWindowImpl::getFullscreen();
	}

	xdl_int  XdevLWindowSDL::getColorDepth() {
		return XdevLWindowImpl::getColorDepth();
	}

	xdl_bool  XdevLWindowSDL::getHidePointer() {
		return XdevLWindowImpl::getHidePointer();
	}

	void  XdevLWindowSDL::show() {
		SDL_ShowWindow(m_window);
	}

	void  XdevLWindowSDL::hide() {
		SDL_HideWindow(m_window);
	}

	xdl_bool XdevLWindowSDL::isHidden() {
		return xdl_false; // TODO This may cause problems.
	}

	void XdevLWindowSDL::raise() {
		SDL_RaiseWindow(m_window);
	}

	void  XdevLWindowSDL::setPointerPosition(xdl_uint x, xdl_uint y) {
		SDL_WarpMouseInWindow(m_window, x,y);
	}

	void XdevLWindowSDL::clipPointerPosition(xdl_uint x, xdl_uint y, xdl_uint width, xdl_uint height) {

	}

	void XdevLWindowSDL::grabPointer() {

	}

	void XdevLWindowSDL::ungrabPointer() {

	}

	void XdevLWindowSDL::grabKeyboard() {

	}

	void XdevLWindowSDL::ungrabKeyboard() {

	}

	void XdevLWindowSDL::setInputFocus() {

	}

	xdl_bool XdevLWindowSDL::hasFocus() {

		return xdl_false;
	}

	xdl_int XdevLWindowSDL::getInputFocus(XdevLWindow** window) {
		*window = nullptr;
		return ERR_ERROR;
	}


	xdl_int XdevLWindowSDL::RunThread(thread::ThreadArgument* argument) {
		return ERR_OK;
	}


	//
	// -------------------------------------------------------------------------
	//

	XdevLWindowServerSDL::XdevLWindowServerSDL(XdevLModuleCreateParameter* parameter) :
		XdevLWindowServerImpl(parameter) {

	}

	XdevLWindowServerSDL::~XdevLWindowServerSDL() {

	}

	xdl_int XdevLWindowServerSDL::createWindow(XdevLWindow** window,
	        const XdevLWindowTitle& title,
	        const XdevLWindowPosition& position,
	        const XdevLWindowSize& size) {

		XdevLWindowSDL* sdlWindow = new XdevLWindowSDL(nullptr);
		sdlWindow->setTitle(title);
		sdlWindow->setPosition(position);
		sdlWindow->setSize(size);
		sdlWindow->create();
		*window = sdlWindow;
		m_windowList[sdlWindow->getWindowID()] = sdlWindow;
		return ERR_OK;
	}

}
