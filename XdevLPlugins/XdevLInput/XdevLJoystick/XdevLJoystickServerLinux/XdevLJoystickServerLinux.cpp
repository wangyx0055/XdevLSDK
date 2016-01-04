
#include <iostream>
#include <string>
#include <XdevLInput/XdevLJoystick/XdevLJoystick.h>
#include "XdevLJoystickServerLinux.h"
#include <XdevLPlatform.h>
#include <XdevLMutex.h>

#include <linux/joystick.h>

xdl::XdevLPluginDescriptor pluginDescriptor {
	xdl::pluginName,
	xdl::moduleNames,
	XDEVLJOYSTICK_SERVER_MAJOR_VERSION,
	XDEVLJOYSTICK_SERVER_MINOR_VERSION,
	XDEVLJOYSTICK_SERVER_PATCH_VERSION
};

xdl::XdevLModuleDescriptor moduleDescriptor {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[0],
	xdl::copyright,
	xdl::description,
	XDEVLJOYSTICK_SERVER_MODULE_MAJOR_VERSION,
	XDEVLJOYSTICK_SERVER_MODULE_MINOR_VERSION,
	XDEVLJOYSTICK_SERVER_MODULE_PATCH_VERSION
};

#if XDEVL_USE_UDEV
static xdl::XdevLJoystickServerLinuxUDev joystickudev;
#endif

xdl::XdevLJoystickServerLinux* joystickServerLinux = nullptr;

extern "C" XDEVL_EXPORT xdl::xdl_int _init_plugin(xdl::XdevLPluginCreateParameter* parameter) {
#if XDEVL_USE_UDEV
	return joystickudev.init();
#else
	return xdl::ERR_OK;
#endif
}

extern "C" XDEVL_EXPORT xdl::xdl_int _shutdown_plugin() {
#if XDEVL_USE_UDEV
	return joystickudev.shutdown();
#else
	return xdl::ERR_OK;
#endif
}


extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {

	if(nullptr == joystickServerLinux) {
		if(moduleDescriptor.getName() == parameter->getModuleName()) {
			joystickServerLinux  = new xdl::XdevLJoystickServerLinux(parameter, moduleDescriptor);
			if(!joystickServerLinux)
				return xdl::ERR_ERROR;
			parameter->setModuleInstance(joystickServerLinux);
			return xdl::ERR_OK;
		}
	} else {
		return xdl::ERR_MODULE_EXIST_ALREADY;
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
	static xdl_uint16 joystickID = 0;

	xdl_uint16 wrapJoystickIdToInteger(const XdevLJoystickId& id) {
		switch(id) {
			case XdevLJoystickId::JOYSTICK_0: return 0;
			case XdevLJoystickId::JOYSTICK_1: return 1;
			case XdevLJoystickId::JOYSTICK_2: return 2;
			case XdevLJoystickId::JOYSTICK_3: return 3;
			case XdevLJoystickId::JOYSTICK_4: return 4;
			case XdevLJoystickId::JOYSTICK_5: return 5;
			case XdevLJoystickId::JOYSTICK_6: return 6;
			case XdevLJoystickId::JOYSTICK_7: return 7;
			case XdevLJoystickId::JOYSTICK_8: return 8;
			case XdevLJoystickId::JOYSTICK_9: return 9;
			case XdevLJoystickId::JOYSTICK_10: return 10;
			case XdevLJoystickId::JOYSTICK_11: return 11;
			case XdevLJoystickId::JOYSTICK_12: return 12;
			case XdevLJoystickId::JOYSTICK_13: return 13;
			case XdevLJoystickId::JOYSTICK_14: return 14;
			case XdevLJoystickId::JOYSTICK_15: return 15;
			default:
			break;
		}
		return 0;
	}

	XdevLJoystickId wrapIntegerToJoystickId(xdl_uint16 id) {
		switch(id) {
			case 0: return XdevLJoystickId::JOYSTICK_0;
			case 1: return XdevLJoystickId::JOYSTICK_1;
			case 2: return XdevLJoystickId::JOYSTICK_2;
			case 3: return XdevLJoystickId::JOYSTICK_3;
			case 4: return XdevLJoystickId::JOYSTICK_4;
			case 5: return XdevLJoystickId::JOYSTICK_5;
			case 6: return XdevLJoystickId::JOYSTICK_6;
			case 7: return XdevLJoystickId::JOYSTICK_7;
			case 8: return XdevLJoystickId::JOYSTICK_8;
			case 9: return XdevLJoystickId::JOYSTICK_9;
			case 10: return XdevLJoystickId::JOYSTICK_10;
			case 11: return XdevLJoystickId::JOYSTICK_11;
			case 12: return XdevLJoystickId::JOYSTICK_12;
			case 13: return XdevLJoystickId::JOYSTICK_13;
			case 14: return XdevLJoystickId::JOYSTICK_14;
			case 15: return XdevLJoystickId::JOYSTICK_15;
			default:
			break;
		}
		return XdevLJoystickId::JOYSTICK_UNKNOWN;
	}

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

	XdevLJoystickServerLinux::XdevLJoystickServerLinux(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		XdevLModuleAutoImpl<XdevLJoystickServer>(parameter, descriptor) {

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
				XDEVL_MODULE_WARNING("Some issues happened when parsing the XML file.\n" );
			}
		}

		//
		// Add all connected joysticks.
		// TODO This is a hacky way and should be changed later.
		//
		xdl_uint id = 0;
		while(addJoystick(id) == ERR_OK) {
			id++;
		}
		return ERR_OK;
	}

	xdl_int XdevLJoystickServerLinux::shutdown() {
		if(xdl_true == m_running) {
			m_mutex.Lock();
			m_running = xdl_false;
			m_mutex.Unlock();

			for(auto& device : m_joystickDevices) {
				if(close(device.second->fd) == -1) {
					XDEVL_MODULE_WARNING("Closing Joystick: " << device.second->name << " descriptor failed.\n");
				} else {
					delete device.second;
				}
			}
			m_joystickDevices.clear();
		}

		return ERR_ERROR;
	}


	xdl_int XdevLJoystickServerLinux::openUsingId(xdl_uint id) {
		xdl_char device[128] {0};
		std::sprintf(device, "/dev/input/js%i", id);
		return open(device, O_RDONLY | O_NONBLOCK);
	}


	xdl_int XdevLJoystickServerLinux::openUsingPath(const std::string& path) {
		return open(path.c_str(), O_RDONLY | O_NONBLOCK);
	}

	xdl_int XdevLJoystickServerLinux::addJoystick(xdl_uint id) {
		std::stringstream path;
		path << "/dev/input/js" << id;
		return addJoystick(path.str());
	}

	xdl_int XdevLJoystickServerLinux::addJoystick(const std::string& path) {
		thread::XdevLScopeLock lock(m_mutex);

		auto it = m_joystickDevices.find(path);
		if(it != m_joystickDevices.end()) {
			return ERR_OK;
		}

		xdl_int fd = openUsingPath(path);
		if(fd== -1) {
			XDEVL_MODULE_INFO("Error occured: " << strerror(errno) << std::endl);
			return ERR_ERROR;
		}

		XdevLJoystickDeviceInfoLinux* joystickInfo = getJoystickInfo(fd, path);
		if(nullptr == joystickInfo) {
			return ERR_ERROR;
		} 
		m_joystickDevices[joystickInfo->device] = joystickInfo;

		return ERR_OK;
	}

	XdevLJoystickDeviceInfoLinux* XdevLJoystickServerLinux::getJoystickInfo(xdl_int fd, const std::string& path) {
		
		//
		// Get information of the joystick device.
		//
		char name[128];
		if (ioctl(fd, JSIOCGNAME(sizeof(name)), name) < 0) {
			XDEVL_MODULE_INFO("Error occured: " << strerror(errno) << std::endl);
			return nullptr;
		}
		XDEVL_MODULE_INFO("Name             : " << name << "\n");

		xdl_uint numberOfAxes = 0;
		if(ioctl(fd, JSIOCGAXES, &numberOfAxes) < 0) {
			XDEVL_MODULE_INFO("Error occured: " << strerror(errno) << std::endl);
			return nullptr;
		}
		XDEVL_MODULE_INFO("Number of axes   : " << numberOfAxes << "\n");

		xdl_uint numberOfButtons = 0;
		if(ioctl(fd, JSIOCGBUTTONS, &numberOfButtons) < 0) {
			XDEVL_MODULE_INFO("Error occured: " << strerror(errno) << std::endl);
			return nullptr;
		}
		XDEVL_MODULE_INFO("Number of buttons: " << numberOfButtons << "\n");

		//
		// Extract the number as identification code.
		//
		size_t pos = path.find_first_of("js");
		if(pos == std::string::npos) {
			return nullptr;
		}
		std::stringstream ss(path.substr(pos + 2, std::string::npos));
		xdl_uint joystickid;
		ss >> joystickid;

		//
		// Create the info structure.
		//
		XdevLJoystickDeviceInfoLinux* devInfo = new XdevLJoystickDeviceInfoLinux();
		devInfo->joystickid = wrapIntegerToJoystickId(joystickid);
		devInfo->fd = fd;
		devInfo->device = path;
		devInfo->name = XdevLString(name);
		devInfo->numberOfAxes = numberOfAxes;
		devInfo->numberOfButtons = numberOfButtons;

		return devInfo;
	}

	xdl_int xdl::XdevLJoystickServerLinux::notify(XdevLEvent& event) {
		return XdevLModuleAutoImpl<XdevLJoystickServer>::notify(event);
	}

	xdl_int XdevLJoystickServerLinux::reset() {
		return ERR_ERROR;
	}

	void* XdevLJoystickServerLinux::getInternal(const XdevLInternalName& id) {
		return nullptr;
	}

	xdl_int XdevLJoystickServerLinux::update() {
		return pollEvents();
	}

	xdl_int XdevLJoystickServerLinux::pollEvents() {
		thread::XdevLScopeLock lock(m_mutex);

		for(auto& device : m_joystickDevices) {
			js_event event;
			ssize_t size = read(device.second->fd, &event, sizeof(js_event));
			if(-1 == size) {

			}

			switch(event.type) {
				case JS_EVENT_INIT: {
					XDEVL_MODULE_INFO("JS_EVENT_INIT\n");
				}
				break;
				case JS_EVENT_AXIS: {
					sendAxisEvent(device.second->joystickid, wrapLinuxAxisToXdevLAxis(event.number), event.value);
				}
				break;
				case JS_EVENT_BUTTON: {
					sendButtonEvent(device.second->joystickid, wrapLinuxButtonToXdevLButton(event.number), (event.value > 0) ? xdl_true : xdl_false);
				}
				break;

			}
		}
		return ERR_OK;
	}

	xdl_int XdevLJoystickServerLinux::RunThread(thread::ThreadArgument*) {
		XDEVL_MODULE_INFO("Starting threading mode.\n");
		m_running = xdl_true;
		for(;;) {
			{
				m_mutex.Lock();
				xdl_bool tmp = m_running;
				m_mutex.Unlock();
				if(tmp == xdl_false) {
					break;
				}
			}
			pollEvents();
		}
		XDEVL_MODULE_INFO("Stopping threading mode.\n");
		return 0;
	}

	void XdevLJoystickServerLinux::sendButtonEvent(const XdevLJoystickId& joystickid, xdl_int buttonID, xdl_bool pressed) {
		XdevLEvent ev;
		ev.common.timestamp		= getMediator()->getTimer().getTime64();
		ev.type 							= pressed ? JoystickButtonPressed.getHashCode() : JoystickButtonReleased.getHashCode();
		ev.jbutton.joystickid	= wrapJoystickIdToInteger(joystickid);
		ev.jbutton.buttonid		= buttonID;
		getMediator()->fireEvent(ev);
	}

	void XdevLJoystickServerLinux::sendAxisEvent(const XdevLJoystickId& joystickid, xdl_uint8 axisID, xdl::xdl_int16 value) {
		XdevLEvent ev;
		ev.common.timestamp	= getMediator()->getTimer().getTime64();
		ev.type 							= JoystickMotion.getHashCode();
		ev.jbutton.joystickid	= wrapJoystickIdToInteger(joystickid);
		ev.jaxis.axisid				= axisID;
		ev.jaxis.value				= (xdl_int32)value;
		getMediator()->fireEvent(ev);
	}

	xdl_int XdevLJoystickServerLinux::readJoystickInfo(TiXmlDocument& document) {
		TiXmlHandle docHandle(&document);
		TiXmlElement* root = docHandle.FirstChild(XdevLCorePropertiesName.c_str()).FirstChildElement("XdevLWindow").ToElement();
		if(!root) {
			XDEVL_MODULE_INFO("<XdevLWindow> section not found. Using default values for the device.\n");
			return ERR_OK;
		}

		while(root != nullptr) {

			// Does the user specified the id of the module?
			if(root->Attribute("id")) {
				XdevLID id(root->Attribute("id"));

				// Do only if we have the same ID.
				// TODO Maybe change comparison into string comparison.
				if(getID() == id) {
					TiXmlElement* child = nullptr;
					for(child = root->FirstChildElement(); child; child = child->NextSiblingElement()) {
//						if(root->Attribute("Device"))
//							m_device = XdevLString(root->Attribute("Device"));
					}
				}
			}
			root = root->NextSiblingElement();
		}
		return ERR_OK;
	}

	xdl_uint XdevLJoystickServerLinux::getNumJoysticks() {
		return m_joystickDevices.size();
	}

	xdl_int XdevLJoystickServerLinux::getJoystickInfo(const XdevLJoystickId& joystickid, XdevLJoystickDeviceInfo& joystickDeviceInfo) {

		std::stringstream tmp;
		tmp << "/dev/input/js" << (xdl_int)joystickid;

		auto it = m_joystickDevices.find(tmp.str());
		if(it == m_joystickDevices.end()) {
			return ERR_ERROR;
		}

		//
		// Only fill out info it the id does not exceed the real number of joysticks
		// connected.
		//
		auto deviceInfo = m_joystickDevices.at(tmp.str());

		XdevLJoystickDeviceInfo info;
		info.name = deviceInfo->name;
		info.joystickid = deviceInfo->joystickid;
		info.numberOfAxes = deviceInfo->numberOfAxes;
		info.numberOfButtons = deviceInfo->numberOfButtons;
		joystickDeviceInfo = info;
		return ERR_OK;
	}

	void XdevLJoystickServerLinux::removeJoystick(const std::string& path) {
		thread::XdevLScopeLock lock(m_mutex);

		auto it = m_joystickDevices.find(path);
		if(it != m_joystickDevices.end()) {
			m_joystickDevices.erase(it);
		}
	}

//
// UDEV stuff to detected plug/unlug of joysticks.
//

#if XDEVL_USE_UDEV
	xdl_int XdevLJoystickServerLinuxUDev::init() {

		udev = udev_new();
		if (!udev) {
			std::cerr << "Can't create udev" << std::endl;
			return xdl::ERR_ERROR;
		}

		//
		// Add listener for plug/unplug
		//
		mon = udev_monitor_new_from_netlink(udev, "udev");
		udev_monitor_filter_add_match_subsystem_devtype(mon, "input", nullptr);
		udev_monitor_enable_receiving(mon);
		fd = udev_monitor_get_fd(mon);
//
//		//
//		// Lets enumerate through all inputs
//		//
//		enumerate = udev_enumerate_new(udev);
//		udev_enumerate_add_match_subsystem(enumerate, "hidraw");
//		udev_enumerate_scan_devices(enumerate);
//		devices = udev_enumerate_get_list_entry(enumerate);
//		udev_list_entry_foreach(dev_list_entry, devices) {
//			const char *path;
//			path = udev_list_entry_get_name(dev_list_entry);
//			dev = udev_device_new_from_syspath(udev, path);
//			dev = udev_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device");
//			if (!dev) {
//				printf("Unable to find parent usb device.");
//				return xdl::ERR_OK;
//			}
//			printf("  VID/PID: %s %s\n", udev_device_get_sysattr_value(dev,"idVendor"), udev_device_get_sysattr_value(dev, "idProduct"));
//			printf("  %s\n  %s\n", udev_device_get_sysattr_value(dev,"manufacturer"), udev_device_get_sysattr_value(dev,"product"));
//			printf("  serial: %s\n", udev_device_get_sysattr_value(dev, "serial"));
//			printf(" SysName: %s\n", udev_device_get_devnode(dev));
//		}

		Start();

		return ERR_OK;
	}

	xdl_int XdevLJoystickServerLinuxUDev::shutdown() {
		udev_enumerate_unref(enumerate);
		udev_device_unref(dev);
		udev_unref(udev);
		return ERR_OK;
	}


	xdl_int XdevLJoystickServerLinuxUDev::RunThread(thread::ThreadArgument*) {
		for(;;) {
			fd_set fds;
			struct timeval tv;
			int ret;

			FD_ZERO(&fds);
			FD_SET(fd, &fds);
			tv.tv_sec = 0;
			tv.tv_usec = 0;

			ret = select(fd+1, &fds, nullptr, nullptr, &tv);
			if (ret > 0 && FD_ISSET(fd, &fds)) {
				dev = udev_monitor_receive_device(mon);
				if (dev) {

					std::string node;
					std::string action;
					const xdl_char* nodeptr = udev_device_get_devnode(dev);
					if(nodeptr != nullptr) {
						node = std::string(udev_device_get_devnode(dev));
					}

					const xdl_char* actionptr = udev_device_get_action(dev);
					if(nullptr != actionptr) {
						action = std::string(udev_device_get_action(dev));
					}

					std::size_t pos = node.find("js");
					if(pos != std::string::npos) {
						std::cout << "XdevLJoystickServerLinuxUDev:: Joystick device = " << node.substr(pos + 2, std::string::npos) << " got";
						if(action == "add") {
							std::cout << " added.";
						} else if(action == "remove") {
							std::cout << " removed.";
						}
						std::cout << std::endl;
						
						if(nullptr != joystickServerLinux) {
							if(action == "add") {
								joystickServerLinux->addJoystick(node);
							} else if( action == "remove") {
								joystickServerLinux->removeJoystick(node);
							}
						}
					}
					udev_device_unref(dev);
				} else {
					printf("No Device from receive_device(). An error occured.\n");
				}
			}
			usleep(250*1000);
		}
		return 0;
	}
#endif

}
