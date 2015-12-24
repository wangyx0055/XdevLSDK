
#include <iostream>
#include <string>
#include <XdevLJoystick/XdevLJoystick.h>
#include "XdevLJoystickServerLinux.h"
#include <XdevLPlatform.h>

#include <linux/joystick.h>


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
		m_device("/dev/input/js0"),
		m_fd(-1),
		m_name(""),
		m_numberOfAxes(0),
		m_numberOfButtons(0) {

	}

	XdevLJoystickServerLinux::~XdevLJoystickServerLinux() {
	}


	xdl_int XdevLJoystickServerLinux::init() {

		TiXmlDocument xmlDocument;
		if(getMediator()->getXmlFilename()) {
			if(!xmlDocument.LoadFile(getMediator()->getXmlFilename())) {
				XDEVL_MODULE_WARNING("Could not parse xml file: " << getMediator()->getXmlFilename() << "\n" );
			}

			if(readJoystickInfo(xmlDocument) != ERR_OK) {
				XDEVL_MODULE_WARNING("Some issues happend when parsing the XML file.\n" );
			}
		}

		return ERR_OK;
	}

	xdl_int XdevLJoystickServerLinux::shutdown() {
		if(close(m_fd) != -1) {
			m_fd = -1;
			m_name = XdevLString(""),
			m_numberOfAxes = 0,
			m_numberOfButtons = 0;
			return ERR_OK;
		}
		return ERR_ERROR;
	}


	xdl_int XdevLJoystickServerLinux::create() {
		return create(m_device);
	}

	xdl_int XdevLJoystickServerLinux::create(const XdevLString& deviceName) {

		m_fd = open(m_device.toString().c_str(), O_RDONLY | O_NONBLOCK);
		if(m_fd == -1) {
			XDEVL_MODULE_INFO("Error occured: " << strerror(errno) << std::endl);
			return ERR_ERROR;
		}

		char name[128];
		if (ioctl(m_fd, JSIOCGNAME(sizeof(name)), name) < 0) {
			XDEVL_MODULE_INFO("Error occured: " << strerror(errno) << std::endl);
			return ERR_ERROR;
		}
		m_name = XdevLString(name);

		XDEVL_MODULE_INFO("Name             : " << name << "\n");

		if(ioctl(m_fd, JSIOCGAXES, &m_numberOfAxes) < 0) {
			XDEVL_MODULE_INFO("Error occured: " << strerror(errno) << std::endl);
			return ERR_ERROR;
		}
		XDEVL_MODULE_INFO("Number of axes   : " << (xdl_int)m_numberOfAxes << "\n");

		if(ioctl(m_fd, JSIOCGBUTTONS, &m_numberOfButtons) < 0) {
			XDEVL_MODULE_INFO("Error occured: " << strerror(errno) << std::endl);
			return ERR_ERROR;
		}
		XDEVL_MODULE_INFO("Number of buttons: " << (xdl_int)m_numberOfButtons << "\n");

		Start();
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

	xdl_int XdevLJoystickServerLinux::readJoystickInfo(TiXmlDocument& document) {
		TiXmlHandle docHandle(&document);
		TiXmlElement* root = docHandle.FirstChild(XdevLCorePropertiesName.c_str()).FirstChildElement("XdevLWindow").ToElement();
		if(!root) {
			XDEVL_MODULE_INFO("<XdevLWindow> section not found. Using default values for the device.\n");
			return ERR_OK;
		}

		while(root != NULL) {

			// Does the user specified the id of the module?
			if(root->Attribute("id")) {
				XdevLID id(root->Attribute("id"));

				// Do only if we have the same ID.
				// TODO Maybe change comparison into string comparision.
				if(getID() == id) {
					TiXmlElement* child = nullptr;
					for(child = root->FirstChildElement(); child; child = child->NextSiblingElement()) {
						if(root->Attribute("Device"))
							m_device = XdevLString(root->Attribute("Device"));
					}
				}
			}
			root = root->NextSiblingElement();
		}
		return ERR_OK;
	}
}
