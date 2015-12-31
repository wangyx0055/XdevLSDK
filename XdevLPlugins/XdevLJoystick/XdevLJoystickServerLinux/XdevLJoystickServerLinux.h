/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2008 Cengiz Terzibas

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

#ifndef XDEVL_MOUSE_LINUX_H
#define XDEVL_MOUSE_LINUX_H

#include <XdevLPluginImpl.h>
#include <XdevLUtils.h>
#include <XdevLThread.h>
#include <XdevLMutex.h>
#include <map>



#define XDEVL_USE_UDEV 1

#if XDEVL_USE_UDEV
#include <libudev.h>
#endif


namespace xdl {

	static const XdevLString vendor {
		"www.codeposer.net"
	};
	static const XdevLString author {
		"Cengiz Terzibas"
	};
	static const XdevLString copyright {
		"(c) 2005 - 2015 Cengiz Terzibas."
	};
	static const XdevLString pluginName {
		"XdevLJoystickServer"
	};
	static const XdevLString description {
		"Gives you access to joystick events."
	};
	static const std::vector<XdevLModuleName> moduleNames {
		XdevLModuleName("XdevLJoystickServer")
	};


	class XdevLJoystickDeviceInfoLinux : public XdevLJoystickDeviceInfo {
		public:
			XdevLJoystickDeviceInfoLinux() :
				fd(-1) {}
			xdl_int fd;
			std::string device;
	};

	/**
		@class XdevLJoystickServerLinux
		@brief Class to support mouse devices
		@author Cengiz Terzibas

	*/
	class XdevLJoystickServerLinux : public XdevLModuleAutoImpl<XdevLJoystickServer>, public thread::Thread {
		public:
			XdevLJoystickServerLinux(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor);
			virtual ~XdevLJoystickServerLinux();

			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;
			virtual xdl_int update() override;


			virtual xdl_uint getNumJoysticks();
			virtual XdevLJoystickDeviceInfo getJoystickInfo(xdl_uint16 joystickid);

			virtual xdl_int notify(XdevLEvent& event);
			virtual void* getInternal(const XdevLInternalName& id);
			xdl_int RunThread(thread::ThreadArgument* p_arg);

			xdl_int openUsingId(xdl_uint id);
			xdl_int openUsingPath(const std::string& path);
			xdl_int addJoystick(xdl_uint id);
			xdl_int addJoystick(const std::string& path);
			void removeJoystick(const std::string& path);
			XdevLJoystickDeviceInfoLinux* getJoystickInfo(xdl_int fd, const std::string& path);
		private:


			xdl_int pollEvents();
			xdl_int reset();
			void sendButtonEvent(xdl_uint16 joystickid, xdl_int buttonID, xdl_bool pressed);
			void sendAxisEvent(xdl_uint16 joystickid, xdl_uint8 axisID, xdl::xdl_int16 value);
			xdl_int readJoystickInfo(TiXmlDocument& document);
		private:
			thread::Mutex m_mutex;
			xdl_bool m_running;
			std::map<std::string, XdevLJoystickDeviceInfoLinux*> m_joystickDevices;
	};

#if XDEVL_USE_UDEV
	class XdevLJoystickServerLinuxUDev : public thread::Thread  {
		public:
			xdl_int init();
			xdl_int shutdown();
			xdl_int RunThread(thread::ThreadArgument* p_arg);
		private:
			struct udev *udev;
			struct udev_enumerate *enumerate;
			struct udev_list_entry *devices, *dev_list_entry;
			struct udev_device *dev;
			struct udev_monitor *mon;
			xdl_int fd;
	};
#endif

}

#endif
