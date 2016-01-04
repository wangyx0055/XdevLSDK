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
			virtual xdl_int getJoystickInfo(const XdevLJoystickId& joystickid, XdevLJoystickDeviceInfo& joystickDeviceInfo);

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
			void sendButtonEvent(const XdevLJoystickId& joystickid, xdl_int buttonID, xdl_bool pressed);
			void sendAxisEvent(const XdevLJoystickId& joystickid, xdl_uint8 axisID, xdl::xdl_int16 value);
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
