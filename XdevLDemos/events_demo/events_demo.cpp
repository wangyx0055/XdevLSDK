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
#include <XdevLKeyboard/XdevLKeyCode.h>
#include <map>

xdl::IPXdevLCore core = nullptr;
xdl::IPXdevLWindow window = nullptr;
xdl::IPXdevLWindow window2 = nullptr;
xdl::xdl_bool run = xdl::xdl_true;

std::map<int, xdl::IPXdevLWindow> windowsMap;

struct Cursor {
	Cursor() : pressed(xdl::xdl_false), released(xdl::xdl_true), x(0), y(0) {}
	xdl::xdl_bool pressed;
	xdl::xdl_bool released;
	xdl::xdl_int32 x;
	xdl::xdl_int32 y;
};

Cursor currentCursorPosition;

void eventCallbackFunction(xdl::XdevLEvent& event) {

	switch(event.type) {

		case xdl::XDEVL_KEY_PRESSED: {
			printf(("WindowID: %d -> XDEVL_KEY_PRESSED\n"), event.window.windowid);
			if(event.key.sym == xdl::KEY_ESCAPE) {
				run = xdl::xdl_false;
			}
		}
		break;
		case xdl::XDEVL_KEY_RELEASED: {
			printf(("WindowID: %d -> XDEVL_KEY_RELEASED\n"), event.window.windowid);
			if(event.key.sym == xdl::KEY_ESCAPE) {
				run = xdl::xdl_false;
			}
		}
		break;
		case xdl::XDEVL_MOUSE_MOTION: {
			printf(("WindowID: %d -> XDEVL_MOUSE_MOTION (%d, %d)\n"),  event.window.windowid, event.motion.x, event.motion.y);
			if(currentCursorPosition.pressed) {
				xdl::IPXdevLWindow tmp = windowsMap[event.window.windowid];
				if(tmp != nullptr) {
					tmp->setPosition(xdl::XdevLWindowPosition(currentCursorPosition.x + event.motion.x, currentCursorPosition.y + event.motion.y));
				}
			}
		}
		break;
		case xdl::XDEVL_MOUSE_BUTTON_PRESSED: {
			printf(("WindowID: %d -> XDEVL_MOUSE_BUTTON_PRESSED (%d, %d)\n"),  event.window.windowid, event.button.x, event.button.y);
			currentCursorPosition.pressed = true;
			currentCursorPosition.released = false;
			currentCursorPosition.x = event.button.x;
			currentCursorPosition.y = event.button.y;
		}
		break;
		case xdl::XDEVL_MOUSE_BUTTON_RELEASED: {
			printf(("WindowID: %d -> XDEVL_MOUSE_BUTTON_RELEASED (%d, %d)\n"),  event.window.windowid, event.button.x, event.button.y);
			currentCursorPosition.pressed = false;
			currentCursorPosition.released = true;
			currentCursorPosition.x = event.button.x;
			currentCursorPosition.y = event.button.y;
		}
		break;
		case xdl::XDEVL_WINDOW_EVENT: {
			switch(event.window.event) {
				case xdl::XDEVL_WINDOW_CREATE: {
					printf(("WindowID: %d -> XDEVL_WINDOW_CREATE\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_CLOSE: {
					printf(("WindowID: %d -> XDEVL_WINDOW_CLOSE\n"), event.window.windowid);
					if(event.window.windowid == window->getWindowID()) {
						run = xdl::xdl_false;
					}
				}
				break;
				case xdl::XDEVL_WINDOW_EXPOSED: {
					printf(("WindowID: %d -> XDEVL_WINDOW_EXPOSED\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_INPUT_FOCUS_GAINED: {
					printf(("WindowID: %d -> XDEVL_WINDOW_INPUT_FOCUS_GAINED\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_INPUT_FOCUS_LOST: {
					printf(("WindowID: %d -> XDEVL_WINDOW_INPUT_FOCUS_LOST\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_HIDDEN: {
					printf(("WindowID: %d -> XDEVL_WINDOW_HIDDEN\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_LEAVE: {
					printf(("WindowID: %d -> XDEVL_WINDOW_LEAVE\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_ENTER: {
					printf(("WindowID: %d -> XDEVL_WINDOW_ENTER\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_MAXIMIZED: {
					printf(("WindowID: %d -> XDEVL_WINDOW_MAXIMIZED\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_MINIMIZED: {
					printf(("WindowID: %d -> XDEVL_WINDOW_MINIMIZED\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_MOVED: {
					printf(("WindowID: %d -> XDEVL_WINDOW_MOVED (%d, %d)\n"),  event.window.windowid, event.window.x, event.window.y);
				}
				break;
				case xdl::XDEVL_WINDOW_RESIZED: {
					printf(("WindowID: %d -> XDEVL_WINDOW_RESIZED (%d, %d)\n"),  event.window.windowid, event.window.width, event.window.height);
				}
				break;
				case xdl::XDEVL_WINDOW_RESTORED: {
					printf(("WindowID: %d -> XDEVL_WINDOW_RESTORED\n"), event.window.windowid);
				}
				break;
				case xdl::XDEVL_WINDOW_SHOWN: {
					printf(("WindowID: %d -> XDEVL_WINDOW_SHOWN\n"), event.window.windowid);
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


void logConsumerCallback(const xdl::XdevLLogItem& item) {
	std::cout << "Time [us]: " << item.timestamp << " Message: " << item.message << std::endl;
}


int main(int argc, char* argv[]) {

	// Create the core system.
	if(xdl::createCore(&core, argc, argv, xdl::XdevLFileName("events_demo.xml")) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	// Register a listener callback function to get events from the XdevLCore in C paradigm.
	core->registerListenerCallbackFunction(eventCallbackFunction);

	xdl::IPXdevLWindowServer windowServer = xdl::createModule<xdl::IPXdevLWindowServer>(core, xdl::XdevLModuleName("XdevLWindowServer"), xdl::XdevLID("MyWindowServer"));
	if(nullptr == windowServer) {
		return xdl::ERR_ERROR;
	}

	window = xdl::getModule<xdl::IPXdevLWindow>(core,  xdl::XdevLID("MyWindow1"));
	windowsMap[window->getWindowID()] = window;

	xdl::IPXdevLWindow tooltip = xdl::createModule<xdl::IPXdevLWindow>(core, xdl::XdevLModuleName("XdevLWindow"), xdl::XdevLID("Tooltip"));
	windowsMap[tooltip->getWindowID()] = tooltip;

	tooltip->setPosition(xdl::XdevLWindowPosition(600, 10));
	tooltip->setSize(xdl::XdevLWindowSize(100, 32));



	// Get a window device.

	window->SetType(xdl::WINDOW_NORMAL);
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
