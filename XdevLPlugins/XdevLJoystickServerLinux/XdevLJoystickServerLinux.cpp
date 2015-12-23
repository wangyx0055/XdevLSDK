
#include <iostream>
#include <string>
#include <XdevLJoystick/XdevLJoystick.h>
#include "XdevLJoystickServerLinux.h"
#include <XdevLPlatform.h>
#include <linux/input.h>


xdl::XdevLPluginDescriptor pluginDescriptor {
	xdl::pluginName,
	xdl::moduleNames,
	XDEVLJOYSTICK_SERVER_MAJOR_VERSION,
	XDEVLJOYSTICK_SERVER_MINOR_VERSION,
	XDEVLJOYSTICK_SERVER_PATCH_VERSION
};

xdl::XdevLModuleDescriptor xdl::XdevLJoystickServerLinux::m_moduleDescriptor {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[0],
	xdl::copyright,
	xdl::description,
	XDEVLJOYSTICK_SERVER_MODULE_MAJOR_VERSION,
	XDEVLJOYSTICK_SERVER_MODULE_MINOR_VERSION,
	XDEVLJOYSTICK_SERVER_MODULE_PATCH_VERSION
};


extern "C" XDEVL_EXPORT xdl::xdl_int _init_plugin(xdl::XdevLPluginCreateParameter* parameter) {
	return xdl::ERR_OK;
}

extern "C" XDEVL_EXPORT xdl::xdl_int _shutdown_plugin() {
	return xdl::ERR_OK;
}


extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	if(xdl::XdevLJoystickServerLinux::m_moduleDescriptor.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLJoystickServerLinux(parameter);
		if(!obj)
			return xdl::ERR_ERROR;
		parameter->setModuleInstance(obj);
		return xdl::ERR_OK;
	}

	return xdl::ERR_MODULE_NOT_FOUND;
}
extern "C" XDEVL_EXPORT void _delete(xdl::XdevLModule* obj) {
	if(obj)
		delete obj;
}

extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor() {
	return &pluginDescriptor;
}


namespace xdl {

	#define JS_EVENT_BUTTON         0x01    /* button pressed/released */
	#define JS_EVENT_AXIS           0x02    /* joystick moved */
	#define JS_EVENT_INIT           0x80    /* initial state of device */

	struct js_event {
		unsigned int time;      /* event timestamp in milliseconds */
		short value;   /* value */
		unsigned char type;     /* event type */
		unsigned char number;   /* axis/button number */
	};

	struct wwvi_js_event {
		int button[11];
		int stick_x;
		int stick_y;
	};

	XdevLJoystickServerLinux::XdevLJoystickServerLinux(XdevLModuleCreateParameter* parameter) :
		XdevLModuleAutoImpl<XdevLJoystickServer>(parameter, m_moduleDescriptor),
		m_fd(-1),
		m_button_left(0),
		m_button_right(0),
		m_button_middle(0) {

	}

	XdevLJoystickServerLinux::~XdevLJoystickServerLinux() {
	}


	xdl_int XdevLJoystickServerLinux::init() {

		// Let's open the first mouse device.
		char devname [] = "/dev/input/js0";
		m_fd = open(devname, O_RDWR | O_NOCTTY);
		if(m_fd == -1) {
			XDEVL_MODULE_INFO("Error occured: " << strerror(errno) << std::endl);
			return ERR_ERROR;
		}

		input_id id;
		if(ioctl(m_fd, EVIOCGID, &id)) {
			XDEVL_MODULE_INFO("Error occured: " << strerror(errno) << std::endl);
			return ERR_ERROR;
		}

		char name[256] = {0};
		if(ioctl(m_fd, EVIOCGNAME(sizeof(name)), name) < 0) {
			XDEVL_MODULE_INFO("Error occured: " << strerror(errno) << std::endl);
		}
		XDEVL_MODULE_INFO("Device: " << devname << ", Name: " << name << std::endl);

		Start();

		return ERR_OK;
	}

	xdl_int XdevLJoystickServerLinux::shutdown() {
		if(close(m_fd) != -1)
			return ERR_OK;

		return ERR_ERROR;
	}


	xdl_int xdl::XdevLJoystickServerLinux::notify(XdevLEvent& event) {

		return XdevLModuleImpl<XdevLJoystickServer>::notify(event);

	}

	xdl_int XdevLJoystickServerLinux::reset() {
		return ERR_ERROR;
	}

	xdl_int XdevLJoystickServerLinux::update() {
		js_event event;
		int size = read(m_fd, &event, sizeof(input_event));

		m_mutex.Lock();

		switch(event.type) {
			case JS_EVENT_INIT: {
				std::cout << "joystick init\n";
			}break;
			case JS_EVENT_AXIS: {
				switch(event.number) {
					case 0 : {
						std::cout << event.value << std::endl;
					} break;
				}
			}
			break;
			case JS_EVENT_BUTTON: {
				
			}break;
			
		}
		m_mutex.Unlock();

	}

	xdl_int XdevLJoystickServerLinux::RunThread(thread::ThreadArgument*) {
		XDEVL_MODULE_INFO("Starting threading mode.\n");
		while(true) {
			update();
			sleep(0.001);
		}
		XDEVL_MODULE_INFO("Stopping threading mode.\n");
		return 0;
	}

	void XdevLJoystickServerLinux::sendButtonPressEvent(xdl_int buttonID) {
		XdevLEvent ev;
		ev.common.timestamp	= getMediator()->getTimer().getTime64();
		ev.type 			= XDEVL_MOUSE_BUTTON_PRESSED;
		ev.window.windowid	= 0;
		ev.button.button	= buttonID;
		ev.button.x			= 0;
		ev.button.y			= 0;
		getMediator()->fireEvent(ev);
	}
}
