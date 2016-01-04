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
