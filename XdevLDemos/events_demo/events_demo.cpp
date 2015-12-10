/**
	@brief      : Demonstrates how to handle events.
	@author     : Cengiz Terzibas

	At the moment there are to ways to receive events.

	1. C type Callback Function.
	2. Object Oriented C++ type using the XdevLListener interface.

	#C Type callback functions.
		- Create a function in the following form: void myfunc(xdl::XdevLEvent& event).
		- Register that function in the core with the

*/

#include <XdevL.h>
#include <XdevLLog.h>
#include <XdevLThread.h>
#include <XdevLWindow/XdevLWindow.h>
#include <map>

xdl::IPXdevLCore core = nullptr;
xdl::IPXdevLWindow window = nullptr;
xdl::IPXdevLWindow window2 = nullptr;
xdl::xdl_bool run = xdl::xdl_true;

std::map<xdl::xdl_uint64, xdl::IPXdevLWindow> windowsMap;

static const xdl::XdevLID ButtonPressed("XDEVL_BUTTON_PRESSED");
static const xdl::XdevLID ButtonReleased("XDEVL_BUTTON_RELEASED");
static const xdl::XdevLID MouseButtonPressed("XDEVL_MOUSE_BUTTON_PRESSED");
static const xdl::XdevLID MouseButtonReleased("XDEVL_MOUSE_BUTTON_RELEASED");
static const xdl::XdevLID MouseMouseMotion("XDEVL_MOUSE_MOTION");

struct Cursor {
	Cursor() : pressed(xdl::xdl_false), released(xdl::xdl_true), x(0), y(0) {}
	xdl::xdl_bool pressed;
	xdl::xdl_bool released;
	xdl::xdl_int32 x;
	xdl::xdl_int32 y;
};

Cursor currentCursorPosition;

void eventCallbackFunction(xdl::XdevLEvent& event) {


	if (event.type == ButtonPressed.getHashCode()) {
		printf(("WindowID: %ld -> XDEVL_KEY_PRESSED: %s\n"), event.key.windowid, xdl::xdevLButtonIdToString((xdl::XdevLButtonId)event.key.keycode).toString().c_str());
		if (event.key.keycode == xdl::KEY_ESCAPE) {
			run = xdl::xdl_false;
		}
		
	}
	else if (event.type == ButtonReleased.getHashCode()) {
		printf(("WindowID: %ld -> XDEVL_KEY_RELEASED: %s\n"), event.key.windowid, xdl::xdevLButtonIdToString((xdl::XdevLButtonId)event.key.keycode).toString().c_str());
		if (event.key.keycode == xdl::KEY_ESCAPE) {
			run = xdl::xdl_false;
		}
	}
	else if (event.type == MouseButtonPressed.getHashCode()) {
		printf(("WindowID: %ld -> XDEVL_MOUSE_BUTTON_PRESSED (%d, %d)\n"), event.button.windowid, event.button.x, event.button.y);
		currentCursorPosition.pressed = true;
		currentCursorPosition.released = false;
		currentCursorPosition.x = event.button.x;
		currentCursorPosition.y = event.button.y;
	}
	else if (event.type == MouseButtonReleased.getHashCode()) {
		printf(("WindowID: %ld -> XDEVL_MOUSE_BUTTON_RELEASED (%d, %d)\n"), event.button.windowid, event.button.x, event.button.y);
		currentCursorPosition.pressed = false;
		currentCursorPosition.released = true;
		currentCursorPosition.x = event.button.x;
		currentCursorPosition.y = event.button.y;
	}
	else if (event.type == MouseMouseMotion.getHashCode()) {
		std::cout << "WindowID: " << event.motion.windowid << " -> XDEVL_MOUSE_MOTION ( " << event.motion.x << "," << event.motion.y << "," << event.motion.xrel << "," << event.motion.yrel << std::endl;
	}


	switch(event.type) {
		case xdl::XDEVL_WINDOW_EVENT: {
			switch(event.window.event) {
				case xdl::XDEVL_WINDOW_CREATE: {
					printf(("WindowID: %ld -> XDEVL_WINDOW_CREATE\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_CLOSE: {
					printf(("WindowID: %ld -> XDEVL_WINDOW_CLOSE\n"), event.window.windowid);
					if(event.window.windowid == window->getWindowID()) {
						run = xdl::xdl_false;
					}
				}
				break;
				case xdl::XDEVL_WINDOW_EXPOSED: {
					printf(("WindowID: %ld -> XDEVL_WINDOW_EXPOSED\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_INPUT_FOCUS_GAINED: {
					printf(("WindowID: %ld -> XDEVL_WINDOW_INPUT_FOCUS_GAINED\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_INPUT_FOCUS_LOST: {
					printf(("WindowID: %ld -> XDEVL_WINDOW_INPUT_FOCUS_LOST\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_HIDDEN: {
					printf(("WindowID: %ld -> XDEVL_WINDOW_HIDDEN\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_LEAVE: {
					printf(("WindowID: %ld -> XDEVL_WINDOW_LEAVE\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_ENTER: {
					printf(("WindowID: %ld -> XDEVL_WINDOW_ENTER\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_MAXIMIZED: {
					printf(("WindowID: %ld -> XDEVL_WINDOW_MAXIMIZED\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_MINIMIZED: {
					printf(("WindowID: %ld -> XDEVL_WINDOW_MINIMIZED\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_MOVED: {
					printf(("WindowID: %ld -> XDEVL_WINDOW_MOVED (%d, %d)\n"),  event.window.windowid, event.window.x, event.window.y);
				}
				break;
				case xdl::XDEVL_WINDOW_RESIZED: {
					printf(("WindowID: %ld -> XDEVL_WINDOW_RESIZED (%d, %d)\n"),  event.window.windowid, event.window.width, event.window.height);
				}
				break;
				case xdl::XDEVL_WINDOW_RESTORED: {
					printf(("WindowID: %ld -> XDEVL_WINDOW_RESTORED\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_SHOWN: {
					printf(("WindowID: %ld -> XDEVL_WINDOW_SHOWN\n"), event.window.windowid);
				}
				break;
			}
		}
		break;
	}
}

class MyListenerClass : public xdl::XdevLListener {
	public:
		MyListenerClass() : id(xdl::XdevLID("MyListenerClass")) {}

		int notify(xdl::XdevLEvent& event) {
			//	std::cout << "MyListenerClass::notify: Message received.\n";
			switch(event.type) {
				case xdl::XDEVL_CORE_EVENT: {
					if(event.core.event == xdl::XDEVL_CORE_SHUTDOWN)
						run = xdl::xdl_false;
				}
				break;

			}
			return 0;
		}

		virtual const xdl::XdevLID& getID() const  {
			return id;
		}
	private:
		xdl::XdevLID id;
};

int main(int argc, char* argv[]) {

	// Create the core system.
	if(xdl::createCore(&core, argc, argv, xdl::XdevLFileName("events_demo.xml")) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	// Register a listener callback function to get events from the XdevLCore in C paradigm.
	core->registerListenerCallbackFunction(eventCallbackFunction);

	window = xdl::getModule<xdl::IPXdevLWindow>(core,  xdl::XdevLID("MyWindow1"));
	if(xdl::isModuleNotValid(window)) {
		return -1;
	}

	window->setType(xdl::WINDOW_NORMAL);
	windowsMap[window->getWindowID()] = window;

	xdl::IPXdevLWindow tooltip = xdl::createModule<xdl::IPXdevLWindow>(core, xdl::XdevLModuleName("XdevLWindow"), xdl::XdevLID("Tooltip"));
	if(xdl::isModuleNotValid(tooltip)) {
		return -1;
	}
	windowsMap[tooltip->getWindowID()] = tooltip;

	tooltip->setType(xdl::WINDOW_NORMAL);
	tooltip->setPosition(xdl::XdevLWindowPosition(600, 10));
	tooltip->setSize(xdl::XdevLWindowSize(320, 200));


	window->show();
	window->setInputFocus();

	tooltip->show();

	while(run) {

		core->update();

		//	xdl::sleep(0.1);
	}

	xdl::destroyCore(core);

	return 0;
}
