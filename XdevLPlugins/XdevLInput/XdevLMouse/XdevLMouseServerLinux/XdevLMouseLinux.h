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
#include <vector>

namespace xdl {

	class XdevLMouse;

	// Holds the plugin major version.
	const xdl_uint XdevLMouseServerPluginMajorVersion	= XDEVLMOUSE_SERVER_MAJOR_VERSION;

	// Holds the plugin minor version.
	const xdl_uint XdevLMouseServerPluginMinorVersion	= XDEVLMOUSE_SERVER_MINOR_VERSION;

	// Holds the plugin patch version.
	const xdl_uint XdevLMouseServerPluginPatchVersion	= XDEVLMOUSE_SERVER_PATCH_VERSION;



	// Holds the Major version number.
	const xdl_uint XdevLMouseServerMajorVersion		= XDEVLMOUSE_SERVER_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLMouseServerMinorVersion		= XDEVLMOUSE_SERVER_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLMouseServerPatchVersion		= XDEVLMOUSE_SERVER_MODULE_PATCH_VERSION;


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
		"XdevLMouseServer"
	};
	static const XdevLString description {
		"Gives you access to mouse events."
	};
	static const std::vector<XdevLModuleName> moduleNames {
		XdevLModuleName("XdevLMouseServer")
	};


	class XdevLMouseServer : public XdevLModule {
		public:
			virtual ~XdevLMouseServer() {}
	};


	/**
		@class XdevLMouseLinux
		@brief Class to support mouse devices
		@author Cengiz Terzibas

	*/
	class XdevLMouseLinux : public XdevLModuleAutoImpl<XdevLMouseServer>, public thread::Thread {
		public:
			XdevLMouseLinux(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLMouseLinux();

			static XdevLModuleDescriptor m_moduleDescriptor;

			virtual xdl_int init();
			virtual xdl_int shutdown();



			virtual xdl_int notify(XdevLEvent& event);
			virtual void* getInternal(const xdl_char* id);
			xdl_int RunThread(thread::ThreadArgument* p_arg);
		private:
			xdl_int update();
			xdl_int reset();
			void sendButtonPressEvent(xdl_int buttonID);
		private:
			int m_fd;
			xdl::xdl_int m_button_left;
			xdl::xdl_int m_button_right;
			xdl::xdl_int m_button_middle;
			thread::Mutex					m_mutex;
	};


}

#endif
