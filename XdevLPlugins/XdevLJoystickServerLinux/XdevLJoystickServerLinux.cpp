
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

	const XdevLID JoystickButtonPressed("XDEVL_JOYSTICK_BUTTON_PRESSED");
	const XdevLID JoystickButtonReleased("XDEVL_JOYSTICK_BUTTON_RELEASED");
	const XdevLID JoystickMotion("XDEVL_JOYSTICK_MOTION");

#define JS_EVENT_BUTTON         0x01    /* button pressed/released */
#define JS_EVENT_AXIS           0x02    /* joystick moved */
#define JS_EVENT_INIT           0x80    /* initial state of device */

	struct js_event {
		unsigned int time;      /* event timestamp in milliseconds */
		xdl::xdl_int16 value;   /* value */
		xdl::xdl_uint8 type;     /* event type */
		xdl::xdl_uint8 number;   /* axis/button number */
	};

	struct wwvi_js_event {
		int button[11];
		int stick_x;
		int stick_y;
	};

	XdevLButtonId wrapLinuxButtonToXdevLButton(xdl_uint8 button) {
		switch(button) {
			case 0:
				return BUTTON_0;
			case 1:
				return BUTTON_1;
			case 2:
				return BUTTON_2;
			case 3:
				return BUTTON_3;
			case 4:
				return BUTTON_4;
			case 5:
				return BUTTON_5;
			case 6:
				return BUTTON_6;
			case 7:
				return BUTTON_7;
			case 8:
				return BUTTON_8;
			case 9:
				return BUTTON_9;
			case 10:
				return BUTTON_10;
			case 11:
				return BUTTON_11;
			case 12:
				return BUTTON_12;
			case 13:
				return BUTTON_13;
			case 14:
				return BUTTON_14;
			case 15:
				return BUTTON_15;
			default:
				break;
		}
		return BUTTON_UNKOWN;
	}

	XdevLAxisId wrapLinuxAxisToXdevLAxis(xdl_uint8 axis) {
		switch(axis) {
			case 0:
				return AXIS_0;
			case 1:
				return AXIS_1;
			case 2:
				return AXIS_2;
			case 3:
				return AXIS_3;
			case 4:
				return AXIS_4;
			case 5:
				return AXIS_5;
			case 6:
				return AXIS_6;
			case 7:
				return AXIS_7;
			case 8:
				return AXIS_8;
			case 9:
				return AXIS_9;
			case 10:
				return AXIS_10;
			case 11:
				return AXIS_11;
			case 12:
				return AXIS_12;
			case 13:
				return AXIS_13;
			case 14:
				return AXIS_14;
			case 15:
				return AXIS_15;
			default:
				break;
		}
		return AXIS_UNKNOWN;
	}

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
		m_fd = open(devname, O_RDONLY | O_NONBLOCK);
		if(m_fd == -1) {
			XDEVL_MODULE_INFO("Error occured: " << strerror(errno) << std::endl);
			return ERR_ERROR;
		}

//		input_id id;
//		if(ioctl(m_fd, EVIOCGID, &id)) {
//			XDEVL_MODULE_INFO("Error occured: " << strerror(errno) << std::endl);
//			return ERR_ERROR;
//		}
//
//		char name[256] = {0};
//		if(ioctl(m_fd, EVIOCGNAME(sizeof(name)), name) < 0) {
//			XDEVL_MODULE_INFO("Error occured: " << strerror(errno) << std::endl);
//		}
//		XDEVL_MODULE_INFO("Device: " << devname << ", Name: " << name << std::endl);

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

	void* XdevLJoystickServerLinux::getInternal(const XdevLInternalName& id) {
		return nullptr;
	}

	xdl_int XdevLJoystickServerLinux::pollEvents() {
		js_event event;
		int size = read(m_fd, &event, sizeof(js_event));

		m_mutex.Lock();

		switch(event.type) {
			case JS_EVENT_INIT: {
				std::cout << "joystick init\n";
			}
			break;
			case JS_EVENT_AXIS: {
				sendAxisEvent(wrapLinuxAxisToXdevLAxis(event.number), event.value);
			}
			break;
			case JS_EVENT_BUTTON: {
				sendButtonEvent(wrapLinuxButtonToXdevLButton(event.number), (event.value > 0) ? xdl_true : xdl_false);
			}
			break;

		}
		m_mutex.Unlock();
	}

	xdl_int XdevLJoystickServerLinux::RunThread(thread::ThreadArgument*) {
		XDEVL_MODULE_INFO("Starting threading mode.\n");
		while(true) {
			pollEvents();
			sleep(0.001);
		}
		XDEVL_MODULE_INFO("Stopping threading mode.\n");
		return 0;
	}

	void XdevLJoystickServerLinux::sendButtonEvent(xdl_int buttonID, xdl_bool pressed) {
		XdevLEvent ev;
		ev.common.timestamp	= getMediator()->getTimer().getTime64();
		ev.type 						= pressed ? JoystickButtonPressed.getHashCode() : JoystickButtonReleased.getHashCode();
		ev.jbutton.buttonid	= buttonID;
		getMediator()->fireEvent(ev);
	}

	void XdevLJoystickServerLinux::sendAxisEvent(xdl_uint8 axisID, xdl::xdl_int16 value) {
		XdevLEvent ev;
		ev.common.timestamp	= getMediator()->getTimer().getTime64();
		ev.type 				= JoystickMotion.getHashCode();
		ev.jaxis.axis		= axisID;
		ev.jaxis.value	= (xdl_int32)value;
		getMediator()->fireEvent(ev);
	}
}
