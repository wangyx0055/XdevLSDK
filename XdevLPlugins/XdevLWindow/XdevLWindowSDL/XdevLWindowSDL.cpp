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
#include <XdevLCore.h>
#include <XdevLInputSystem.h>
#include <XdevLInput/XdevLKeyboard/XdevLKeyboard.h>
#include <XdevLInput/XdevLMouse/XdevLMouse.h>
#include <XdevLInput/XdevLJoystick/XdevLJoystick.h>
#include "XdevLWindowSDL.h"
#include <XdevLXstring.h>
#include <XdevLUtils.h>
#include <sstream>
#include <cstddef>
#include <map>


xdl::XdevLModuleDescriptor windowSDLModuleDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[0],
	xdl::window_copyright,
	xdl::windowDescription,
	xdl::XdevLWindowMajorVersion,
	xdl::XdevLWindowMinorVersion,
	xdl::XdevLWindowPatchVersion
};

xdl::XdevLModuleDescriptor windowEventServerModuleDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[2],
	xdl::window_copyright,
	xdl::windowServerDescription,
	xdl::XdevLWindowEventServerMajorVersion,
	xdl::XdevLWindowEventServerMinorVersion,
	xdl::XdevLWindowEventServerPatchVersion,
	xdl::XDEVL_MODULE_STATE_DISABLE_AUTO_DESTROY
};

xdl::XdevLModuleDescriptor cursorModuleDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[3],
	xdl::window_copyright,
	xdl::windowServerDescription,
	xdl::XdevLWindowEventServerMajorVersion,
	xdl::XdevLWindowEventServerMinorVersion,
	xdl::XdevLWindowEventServerPatchVersion
};


xdl::XdevLPluginDescriptor m_windowSDLPluginDescriptor {
	xdl::windowPluginName,
	xdl::window_moduleNames,
	xdl::XdevLWindowPluginMajorVersion,
	xdl::XdevLWindowPluginMinorVersion,
	xdl::XdevLWindowPluginPatchVersion
};

static std::map<xdl::xdl_uint32, xdl::XdevLWindow*> windowMap;


// TODO This might be changed in future releases
static xdl::xdl_uint numberOfJoystickDevices = 0;

struct XdevLJoysticks {
	SDL_Joystick* instance;
	xdl::xdl_uint numberOfJoystickButtons;
	xdl::xdl_uint numberOfJoystickAxis;
};

static std::vector<XdevLJoysticks> joysticks;


extern "C" XDEVL_EXPORT xdl::xdl_int _init_plugin(xdl::XdevLPluginCreateParameter* parameter) {

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);

	numberOfJoystickDevices = SDL_NumJoysticks();
	if(numberOfJoystickDevices > 0) {
		for(xdl::xdl_uint idx = 0; idx < numberOfJoystickDevices; idx++) {
			SDL_Joystick* js = SDL_JoystickOpen(idx);
			if(js != nullptr) {
				XdevLJoysticks jse;
				jse.instance = js;
				jse.numberOfJoystickButtons = SDL_JoystickNumButtons(js);
				jse.numberOfJoystickAxis = SDL_JoystickNumAxes(js);
				joysticks.push_back(jse);
			} else {
				numberOfJoystickDevices--;
			}
		}
		SDL_JoystickEventState(SDL_ENABLE);
	}

	// If there is not event server first create one.
	if(xdl::windowEventServer == nullptr) {
		// If there is no even server active, create and activate it.
		xdl::windowEventServer = static_cast<xdl::XdevLWindowSDLEventServer*>(parameter->getMediator()->createModule(xdl::XdevLModuleName("XdevLWindowEventServer"), xdl::XdevLID("XdevLWindowEventServer"), xdl::XdevLPluginName("XdevLWindowSDL")));
	}

	if(xdl::cursor == nullptr) {
		xdl::cursor = static_cast<xdl::XdevLCursor*>(parameter->getMediator()->createModule(xdl::XdevLModuleName("XdevLCursor"), xdl::XdevLID("XdevLCursor")));
	}

	return xdl::ERR_OK;
}

extern "C" XDEVL_EXPORT xdl::xdl_int _shutdown_plugin() {

	// If the last window was destroy make sure to destroy the event server too.
	if(xdl::windowEventServer != nullptr) {
		xdl::XdevLWindowEventServerParameter->getMediator()->deleteModule(xdl::windowEventServer->getID());
		xdl::windowEventServer = nullptr;
	}

	if(joysticks.size() > 0) {
		for(auto& js : joysticks) {
			SDL_JoystickClose(js.instance);
		}
	}

	SDL_Quit();

	return xdl::ERR_OK;
}


extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {


	//
	// Create XdevLWindow instance.
	//
	if(windowSDLModuleDesc.getName() == parameter->getModuleName()) {

		xdl::XdevLWindowSDL* window = new xdl::XdevLWindowSDL(parameter);
		parameter->setModuleInstance(window);
	}

	//
	// Create XdevLWindowServer instance.
	//
	else if(xdl::XdevLWindowServerImpl::m_windowServerModuleDesc.getName() == parameter->getModuleName()) {

		xdl::XdevLWindowServerSDL* windowServer = new xdl::XdevLWindowServerSDL(parameter);
		parameter->setModuleInstance(windowServer);
	}

	//
	// Create XdevLEventServer instance.
	//
	else if(windowEventServerModuleDesc.getName() == parameter->getModuleName()) {
		xdl::windowEventServer = new xdl::XdevLWindowSDLEventServer(parameter);
		parameter->setModuleInstance(xdl::windowEventServer);
		xdl::XdevLWindowEventServerParameter = parameter;
	}

	//
	// Create XdevLCursor instance.
	//
	else if(cursorModuleDesc.getName() == parameter->getModuleName()) {

		xdl::XdevLCursorSDL* cursor = new xdl::XdevLCursorSDL(parameter);
		parameter->setModuleInstance(cursor);

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
	return &m_windowSDLPluginDescriptor;
}

namespace xdl {

	static const XdevLID ButtonPressed("XDEVL_BUTTON_PRESSED");
	static const XdevLID ButtonReleased("XDEVL_BUTTON_RELEASED");
	static const XdevLID MouseButtonPressed("XDEVL_MOUSE_BUTTON_PRESSED");
	static const XdevLID MouseButtonReleased("XDEVL_MOUSE_BUTTON_RELEASED");
	static const XdevLID MouseMotion("XDEVL_MOUSE_MOTION");
	static const XdevLID WindowEvent("XDEVL_WINDOW_EVENT");

	std::map<int, XdevLButtonId> KeySymToXdevLKeyCode = {
		{ SDLK_a, KEY_A},
		{ SDLK_b, KEY_B},
		{ SDLK_c, KEY_C},
		{ SDLK_d, KEY_D},
		{ SDLK_e, KEY_E},
		{ SDLK_f, KEY_F},
		{ SDLK_g, KEY_G},
		{ SDLK_h, KEY_H},
		{ SDLK_i, KEY_I},
		{ SDLK_j, KEY_J},
		{ SDLK_k, KEY_K},
		{ SDLK_l, KEY_L},
		{ SDLK_m, KEY_M},
		{ SDLK_n, KEY_N},
		{ SDLK_o, KEY_O},
		{ SDLK_p, KEY_P},
		{ SDLK_q, KEY_Q},
		{ SDLK_r, KEY_R},
		{ SDLK_s, KEY_S},
		{ SDLK_t, KEY_T},
		{ SDLK_u, KEY_U},
		{ SDLK_v, KEY_V},
		{ SDLK_w, KEY_W},
		{ SDLK_x, KEY_X},
		{ SDLK_y, KEY_Y},
		{ SDLK_z, KEY_Z},

		{ SDLK_0, KEY_0},
		{ SDLK_1, KEY_1},
		{ SDLK_2, KEY_2},
		{ SDLK_3, KEY_3},
		{ SDLK_4, KEY_4},
		{ SDLK_5, KEY_5},
		{ SDLK_6, KEY_6},
		{ SDLK_7, KEY_7},
		{ SDLK_8, KEY_8},
		{ SDLK_9, KEY_9},


		{ SDLK_F1, KEY_F1},
		{ SDLK_F2, KEY_F2},
		{ SDLK_F3, KEY_F3},
		{ SDLK_F4, KEY_F4},
		{ SDLK_F5, KEY_F5},
		{ SDLK_F6, KEY_F6},
		{ SDLK_F7, KEY_F7},
		{ SDLK_F8, KEY_F8},
		{ SDLK_F9, KEY_F9},
		{ SDLK_F10, KEY_F10},
		{ SDLK_F11, KEY_F11},
		{ SDLK_F12, KEY_F12},
		{ SDLK_F13, KEY_F13},
		{ SDLK_F14, KEY_F14},
		{ SDLK_F15, KEY_F15},
		{ SDLK_F16, KEY_F16},
		{ SDLK_F17, KEY_F17},
		{ SDLK_F18, KEY_F18},
		{ SDLK_F19, KEY_F19},
		{ SDLK_F20, KEY_F20},
		{ SDLK_F21, KEY_F21},
		{ SDLK_F22, KEY_F22},
		{ SDLK_F23, KEY_F23},
		{ SDLK_F24, KEY_F24},

		{ SDLK_ESCAPE, KEY_ESCAPE},
		{ SDLK_HOME, KEY_HOME },
		{ SDLK_END, KEY_END },
		{ SDLK_INSERT, KEY_INSERT },
		{ SDLK_DELETE, KEY_DELETE },
		{ SDLK_PAGEDOWN, KEY_PAGEDOWN},
		{ SDLK_PAGEUP, KEY_PAGEUP },

		{ SDLK_SPACE, KEY_SPACE},
		{ SDLK_BACKSPACE, KEY_BACKSPACE},
		{ SDLK_RETURN, KEY_ENTER },
		{ SDLK_TAB , KEY_TAB },
		{ SDLK_CAPSLOCK, KEY_CAPSLOCK},
		{ SDLK_PLUS, KEY_PLUS },
		{ SDLK_MINUS, KEY_MINUS },

		{ SDLK_UP, KEY_UP },
		{ SDLK_DOWN, KEY_DOWN },
		{ SDLK_LEFT, KEY_LEFT },
		{ SDLK_RIGHT, KEY_RIGHT },

		{ SDLK_LALT, KEY_LALT },
		{ SDLK_LCTRL, KEY_LCTRL },
		{ SDLK_LSHIFT, KEY_LSHIFT },
		{ SDLK_RALT, KEY_RALT },
		{ SDLK_RCTRL, KEY_RCTRL },
		{ SDLK_RSHIFT, KEY_RSHIFT },
		{ SDLK_LGUI, KEY_LGUI },
		{ SDLK_RGUI, KEY_RGUI },

		{ SDLK_KP_0, KEY_KP_0 },
		{ SDLK_KP_1, KEY_KP_1 },
		{ SDLK_KP_2, KEY_KP_2 },
		{ SDLK_KP_3, KEY_KP_3 },
		{ SDLK_KP_4, KEY_KP_4 },
		{ SDLK_KP_5, KEY_KP_5 },
		{ SDLK_KP_6, KEY_KP_6 },
		{ SDLK_KP_7, KEY_KP_7 },
		{ SDLK_KP_8, KEY_KP_8 },
		{ SDLK_KP_9, KEY_KP_9 },


		{ SDLK_NUMLOCKCLEAR, KEY_NUMLOCK },
		{ SDLK_KP_DIVIDE, KEY_KP_DIVIDE },
		{ SDLK_KP_MULTIPLY, KEY_KP_MULTIPLY },
		{ SDLK_KP_MINUS, KEY_KP_MINUS },
		{ SDLK_KP_PLUS, KEY_KP_PLUS },
		{ SDLK_KP_ENTER, KEY_KP_ENTER },

		{ SDLK_PRINTSCREEN, KEY_PRINTSCREEN },
		{ SDLK_SCROLLLOCK, KEY_SCROLLLOCK },
		{ SDLK_PAUSE, KEY_PAUSE },

	};


	XdevLWindowSDL::XdevLWindowSDL(XdevLModuleCreateParameter* parameter) :
		XdevLWindowImpl(XdevLWindowImpl::getWindowsCounter(), parameter, windowSDLModuleDesc),
		m_window(nullptr) {
	}

	XdevLWindowSDL::~XdevLWindowSDL() {

		// Window got destroyed, did someone shut it down?
		if(m_window != nullptr) {
			shutdown();
		}
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

		xdl_uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE;
		if(	(m_attribute.type == XDEVL_WINDOW_TYPE_TOOLTIP) ||
		    (m_attribute.type == XDEVL_WINDOW_TYPE_POPUP) ||
		    (m_attribute.type == XDEVL_WINDOW_TYPE_SPLASH) ||
		    (m_attribute.type == XDEVL_WINDOW_TYPE_NOTIFICATION)) {
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

		SDL_SetRenderDrawColor(renderer,255,0, 0, 255);
		SDL_RenderDrawLine(renderer, 0, 0, 500, 500);

		SDL_RenderPresent(renderer);
		SDL_DestroyRenderer(renderer);

		setHidePointer(XdevLWindowImpl::getHidePointer());
		setFullscreen(XdevLWindowImpl::getFullscreen());


		SDL_VERSION(&m_wmInfo.version);
		SDL_GetWindowWMInfo(m_window,&m_wmInfo);
		SDL_UpdateWindowSurface(m_window);


		//
		// Star the main event polling thread.
		//
		if(isEventThreadRunning() == xdl::xdl_false) {

			XdevLWindowThreadArgument* argument = new XdevLWindowThreadArgument();
			argument->core = getMediator();
			Start(argument);
		}

		m_id = SDL_GetWindowID(m_window);
		windowEventServer->registerWindowForEvents(this);

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
		if(id.toString() == "X11_DISPLAY") {
			return m_wmInfo.info.x11.display;
		}
		if(id.toString() == "X11_WINDOW") {
			return (void*)m_wmInfo.info.x11.window;
		}
#endif

#ifdef WIN32
		if(id.toString() == "WINDOWS_DISPLAY") {
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

		// We have to call this before SDL_DestroyWindow to get all pending events.
		XdevLWindowImpl::shutdown();

		if(nullptr != m_window) {
			SDL_DestroyWindow(m_window);
			m_window = nullptr;
		}

		XDEVL_MODULE_SUCCESS("Shutdown process was successful.\n");

		return ERR_OK;
	}

	xdl_int XdevLWindowSDL::update() {
		return ERR_OK;
	}

	void XdevLWindowSDL::setSize(const XdevLWindowSize& size) {
		XdevLWindowImpl::setSize(size);

		if(m_window) {
			SDL_SetWindowSize(m_window, size.width, size.height);
		}
	}

	void XdevLWindowSDL::setPosition(const XdevLWindowPosition& position) {
		XdevLWindowImpl::setPosition(position);

		if(m_window) {
			SDL_SetWindowPosition(m_window, position.x, position.y);
		}
	}

	void XdevLWindowSDL::setTitle(const XdevLWindowTitle& title) {
		XdevLWindowImpl::setTitle(title);

		if(m_window) {
			SDL_SetWindowTitle(m_window, title.toString().c_str());
		}
	}

	void XdevLWindowSDL::setFullscreen(xdl_bool state) {

		int flags {0};
		if(state) {
			flags = SDL_WINDOW_FULLSCREEN;
		}

		if(m_window) {
			SDL_SetWindowFullscreen(m_window, flags);
		}

		XdevLWindowSize size;
		SDL_GetWindowSize(m_window, &size.width, &size.height);
		setSize(size);
	}

	void XdevLWindowSDL::setType(XdevLWindowTypes type) {
		XDEVL_MODULE_WARNING("SDL2 does not support this function.\n");
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
		SDL_GetWindowPosition(m_window, &m_attribute.position.x, &m_attribute.position.y);
		return m_attribute.position.x;
	}

	XdevLWindowPosition::type XdevLWindowSDL::getY() {
		SDL_GetWindowPosition(m_window, &m_attribute.position.x, &m_attribute.position.y);
		return m_attribute.position.y;
	}

	XdevLWindowSize::type XdevLWindowSDL::getWidth() {
		SDL_GetWindowSize(m_window, &m_attribute.size.width, &m_attribute.size.height);
		return m_attribute.size.width;
	}

	XdevLWindowSize::type XdevLWindowSDL::getHeight() {
		SDL_GetWindowSize(m_window, &m_attribute.size.width, &m_attribute.size.height);
		return m_attribute.size.height;
	}

	const XdevLWindowSize& XdevLWindowSDL::getSize() {
		SDL_GetWindowSize(m_window, &m_attribute.size.width, &m_attribute.size.height);
		return m_attribute.size;
	}

	const XdevLWindowPosition& XdevLWindowSDL::getPosition() {
		SDL_GetWindowPosition(m_window, &m_attribute.position.x, &m_attribute.position.y);
		return m_attribute.position;
	}

	const XdevLWindowTitle& XdevLWindowSDL::getTitle() {
		return getTitle();
	}

	xdl_bool  XdevLWindowSDL::getFullscreen() {
		return XdevLWindowImpl::getFullscreen();
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

	void XdevLWindowSDL::grabPointer() {

	}

	void XdevLWindowSDL::ungrabPointer() {

	}

	void XdevLWindowSDL::setInputFocus() {

	}

	xdl_bool XdevLWindowSDL::hasFocus() {

		return xdl_false;
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
	        const XdevLWindowSize& size,
	        const XdevLWindowTypes& type) {

		XdevLWindowSDL* sdlWindow = new XdevLWindowSDL(nullptr);
		sdlWindow->setTitle(title);
		sdlWindow->setPosition(position);
		sdlWindow->setSize(size);
		sdlWindow->setWindowDecoration(xdl_false);
		sdlWindow->create();
		*window = sdlWindow;
		m_windowList[sdlWindow->getWindowID()] = sdlWindow;
		return ERR_OK;
	}





	XdevLWindowSDLEventServer::XdevLWindowSDLEventServer(XdevLModuleCreateParameter* parameter) :
		XdevLWindowEventServerImpl(parameter, windowEventServerModuleDesc)
	{}


	xdl_int XdevLWindowSDLEventServer::registerWindowForEvents(XdevLWindow* window) {
		return XdevLWindowEventServerImpl::registerWindowForEvents(window);
	}

	xdl_int XdevLWindowSDLEventServer::unregisterWindowFromEvents(XdevLWindow* window) {
		return XdevLWindowEventServerImpl::unregisterWindowFromEvents(window);
	}

	xdl_int XdevLWindowSDLEventServer::init() {
		XDEVL_MODULE_SUCCESS("Initialized successfully" << std::endl);
		return ERR_OK;
	}

	void* XdevLWindowSDLEventServer::getInternal(const XdevLInternalName& id) {

		return nullptr;
	}

	xdl_int XdevLWindowSDLEventServer::shutdown() {
		XDEVL_MODULE_SUCCESS("Shutdown process was successful.\n");
		return ERR_OK;
	}

	xdl_int XdevLWindowSDLEventServer::update() {
		return pollEvents();
	}

	xdl_int XdevLWindowSDLEventServer::pollEvents() {

		SDL_Event event;
		while(SDL_PollEvent(&event)) {

			XdevLWindow* window = getWindow(event.key.windowID);
			if(window == nullptr) {
				continue;
			}

			switch(event.type) {
				case SDL_KEYDOWN: {

					// Register button up event in the Core.
					XdevLEvent ev;
					ev.type 			= ButtonPressed.getHashCode();
					ev.key.timestamp	= getMediator()->getTimer().getTime64();


					ev.key.windowid		= window->getWindowID();
					ev.key.repeat 		= event.key.repeat;
					ev.key.keycode		= KeySymToXdevLKeyCode[event.key.keysym.sym];
					ev.key.mod 			= event.key.keysym.mod;

					getMediator()->fireEvent(ev);

					std::cout << ev.key.keycode << std::endl;
				}
				break;
				case SDL_KEYUP: {

					// Register button up event in the Core.
					XdevLEvent ev;
					ev.type 			= ButtonReleased.getHashCode();
					ev.common.timestamp	= getMediator()->getTimer().getTime64();

					ev.key.windowid		= window->getWindowID();
					ev.key.repeat 		= event.key.repeat;
					ev.key.keycode		= KeySymToXdevLKeyCode[event.key.keysym.sym];
					ev.key.mod 			= event.key.keysym.mod;

					getMediator()->fireEvent(ev);

				}
				break;
				case SDL_MOUSEBUTTONDOWN: {
					// Register button up event in the Core.
					XdevLEvent ev;
					ev.common.timestamp 	= getMediator()->getTimer().getTime64();
					ev.type 				= MouseButtonPressed.getHashCode();

					ev.button.windowid		= window->getWindowID();
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
					ev.type 				= MouseButtonReleased.getHashCode();

					ev.button.windowid		= window->getWindowID();
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
					ev.type 				= MouseMotion.getHashCode();

					ev.motion.windowid		= window->getWindowID();
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
					ev.jaxis.axisid			= event.jaxis.axis;
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
							ev.window.windowid	= window->getWindowID();

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_HIDDEN: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_HIDDEN;
							ev.window.windowid	= window->getWindowID();

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_EXPOSED: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_EXPOSED;
							ev.window.windowid	= window->getWindowID();

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
							ev.window.windowid	= window->getWindowID();

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
							ev.window.windowid	= window->getWindowID();

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_MINIMIZED: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_MINIMIZED;
							ev.window.windowid	= window->getWindowID();

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_MAXIMIZED: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_MAXIMIZED;
							ev.window.windowid	= window->getWindowID();

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_RESTORED: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_RESTORED;
							ev.window.windowid	= window->getWindowID();

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
							ev.window.windowid	= window->getWindowID();

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
							ev.window.windowid	= window->getWindowID();

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_FOCUS_GAINED: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_INPUT_FOCUS_GAINED;
							ev.window.windowid	= window->getWindowID();

							focusGained(window);

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_FOCUS_LOST: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_INPUT_FOCUS_LOST;
							ev.window.windowid	= window->getWindowID();

							getMediator()->fireEvent(ev);

						}
						break;
						case SDL_WINDOWEVENT_CLOSE: {
							XdevLEvent ev;
							ev.common.timestamp = getMediator()->getTimer().getTime64();
							ev.type				= XDEVL_WINDOW_EVENT;
							ev.window.event 	= XDEVL_WINDOW_CLOSE;
							ev.window.windowid	= window->getWindowID();

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

	xdl_int XdevLWindowSDLEventServer::notify(XdevLEvent& event) {

		switch(event.type) {
			case XDEVL_JOYSTICK_REQ_DEVICES_INFO: {
				XdevLEvent event;
				event.type = XDEVL_JOYSTICK_RPLY_DEVICES_INFO;
				event.jdeviceinfo.sender = this->getID().getHashCode();
				event.jdeviceinfo.timestamp = this->getMediator()->getTimer().getTime64();
				event.jdeviceinfo.number_devices = joysticks.size();

				xdl_int i = 0;
				for(auto& joy : joysticks) {
					event.jdeviceinfo.number_buttons[i] = joy.numberOfJoystickButtons;
					event.jdeviceinfo.number_axis[i] = joy.numberOfJoystickAxis;
					i++;
					if(i == 4) {
						break;
					}
				}
				getMediator()->fireEvent(event);
			}
			break;
		}

		return 	XdevLModuleAutoImpl::notify(event);;
	}

	void XdevLWindowSDLEventServer::flush() {
		pollEvents();
	}

//
// -----------------------------------------------------------------------------
//

	XdevLCursorSDL::XdevLCursorSDL(XdevLModuleCreateParameter* parameter) :
		XdevLModuleImpl<XdevLCursor>(parameter, cursorModuleDesc),
		m_window(nullptr) {

	}

	xdl_int XdevLCursorSDL::init() {
		return ERR_OK;
	}

	xdl_int XdevLCursorSDL::shutdown() {
		return ERR_OK;
	}

	void* XdevLCursorSDL::getInternal(const XdevLInternalName& id) {
		return nullptr;
	}

	xdl_int XdevLCursorSDL::attach(XdevLWindow* window) {
		XDEVL_ASSERT(window, "Invalid window parameter");

		m_window = static_cast<XdevLWindowSDL*>(window);
		return ERR_OK;
	}

	void XdevLCursorSDL::show() {
		SDL_ShowCursor(SDL_TRUE);
	}

	void XdevLCursorSDL::hide() {
		SDL_ShowCursor(SDL_FALSE);
	}

	void XdevLCursorSDL::setPosition(xdl_uint x, xdl_uint y) {
		XDEVL_ASSERT(m_window, "Cursor not attached to a window.");
		SDL_WarpMouseInWindow(m_window->getNativeWindow(), x, m_window->getHeight() - y);
	}

	xdl_int XdevLCursorSDL::clip(xdl_uint x, xdl_uint y, xdl_uint width, xdl_uint height) {
		XDEVL_ASSERT(nullptr, "Not supported by SDL2");
		return ERR_ERROR;
	}

	void XdevLCursorSDL::releaseClip() {
		XDEVL_ASSERT(nullptr, "Not supported by SDL2");
	}

	xdl_int XdevLCursorSDL::enableRelativeMotion() {
		SDL_SetRelativeMouseMode(SDL_TRUE);
		return ERR_OK;
	}
	void XdevLCursorSDL::disableRelativeMotion() {
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}

}
