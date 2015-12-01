
#include <iostream>
#include <string>
#include <XdevLMouse/XdevLMouse.h>
#include "XdevLMouseLinux.h"
#include <XdevLPlatform.h>
#include <linux/input.h>


xdl::XdevLModuleDescriptor xdl::XdevLMouseLinux::m_moduleDescriptor {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[0],
	xdl::copyright,
	xdl::description,
	xdl::XdevLMouseServerMajorVersion,
	xdl::XdevLMouseServerMinorVersion,
	xdl::XdevLMouseServerPatchVersion
};

xdl::XdevLPluginDescriptor pluginDescriptor {
	xdl::pluginName,
	xdl::moduleNames,
	xdl::XdevLMouseServerPluginMajorVersion,
	xdl::XdevLMouseServerPluginMinorVersion,
	xdl::XdevLMouseServerPluginPatchVersion
};


extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	if(xdl::XdevLMouseLinux::m_moduleDescriptor.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLMouseLinux(parameter);
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

	XdevLMouseLinux::XdevLMouseLinux(XdevLModuleCreateParameter* parameter) :
		XdevLModuleAutoImpl<XdevLMouseServer>(parameter, m_moduleDescriptor),
		m_fd(-1),
		m_button_left(0),
		m_button_right(0),
		m_button_middle(0) {

	}

	XdevLMouseLinux::~XdevLMouseLinux() {
	}


	xdl_int XdevLMouseLinux::init() {

		// Let's open the first mouse device.
		char devname [] = "/dev/input/event2";
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

	xdl_int XdevLMouseLinux::shutdown() {
		if(close(m_fd) != -1)
			return ERR_OK;

		return ERR_ERROR;
	}


	xdl_int xdl::XdevLMouseLinux::notify(XdevLEvent& event) {

		return XdevLModuleImpl<XdevLMouseServer>::notify(event);

	}

	xdl_int XdevLMouseLinux::reset() {
		return ERR_ERROR;
	}

	xdl_int XdevLMouseLinux::update() {
		input_event event;
		int size = read(m_fd, &event, sizeof(input_event));

		m_mutex.Lock();

		switch(event.type) {
			case EV_KEY: {
				switch(event.code) {
					case BTN_LEFT: {
						sendButtonPressEvent(BUTTON_LEFT);
						m_button_left = 1;
					}
					break;
					case BTN_RIGHT: {
						XdevLEvent ev;
						sendButtonPressEvent(BUTTON_RIGHT);
					}
					break;
					case BTN_MIDDLE: {
						sendButtonPressEvent(BUTTON_MIDDLE);
					}
					break;
					case BTN_SIDE: {
						sendButtonPressEvent(BUTTON_SIDE);
					}
					break;
					case BTN_EXTRA: {
						sendButtonPressEvent(BUTTON_EXTRA);
					}
					break;
					case BTN_FORWARD: {
						sendButtonPressEvent(BUTTON_FORWARD);
					}
					break;
					case BTN_BACK: {
						sendButtonPressEvent(BUTTON_BACK);
					}
					break;
					case BTN_TASK: {
						sendButtonPressEvent(BUTTON_TASK);
					}
					break;
				}
			}
			break;
			case EV_REL: {
				switch(event.code) {
					case REL_X: {
//					printf("REL_X: %i\n", event.value);
					} break;
					case REL_Y: {
//					printf("REL_Y: %i\n", event.value);
					} break;
					case REL_WHEEL: {
						//				printf("REL_WHEEL: %i\n", event.value);
					} break;
				}
			}
			break;

			case EV_ABS: {
				switch(event.code) {
					case ABS_X: {
						//				printf("ABS_X: %i\n", event.value);
					} break;
					case ABS_Y: {
						//				printf("ABS_Y: %i\n", event.value);
					} break;
				}
			}
			break;
		}
		m_mutex.Unlock();
//		printf("type: %d, code: %d, value: %d\n", event.type, event.code,  event.value);

	}

	xdl_int XdevLMouseLinux::RunThread(thread::ThreadArgument*) {
		XDEVL_MODULE_INFO("Starting threading mode.\n");
		while(true) {
			update();
//			sleep(0.001);
		}
		XDEVL_MODULE_INFO("Stopping threading mode.\n");
		return 0;
	}

	void XdevLMouseLinux::sendButtonPressEvent(xdl_int buttonID) {
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
