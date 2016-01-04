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

#ifndef XDEVL_BLUETOOTH_WINIMPL_H
#define XDEVL_BLUETOOTH_WINIMPL_H

#include <XdevLBluetooth.h>
#include <XdevLPluginImpl.h>
#include <XdevLUtils.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/l2cap.h>

namespace xdl {

	static const XdevLString	vendor			{"www.codeposer.net"};
	static const XdevLString	author			{"Cengiz Terzibas"};
	static const XdevLString	copyright		{"(c) 2005 - 2014 Cengiz Terzibas."};
	static const XdevLString	pluginName	{"XdevLBluetooth"};
	static const XdevLString 	description	{"Creates a Bluetooth connection."};

	static const std::vector<XdevLModuleName>	moduleNames	{
																													XdevLModuleName("XdevLBluetooth")
																												};
	
	class XdevLBluetoothAddressImpl : public XdevLBluetoothAddress {
		public:
			XdevLBluetoothAddressImpl(xdl::xdl_int socket, sockaddr_rc addr) : m_socket(socket), m_addr(addr) {}
			virtual~ XdevLBluetoothAddressImpl() {}
			sockaddr_rc get() {
				return m_addr;
			}
		protected:
			xdl::xdl_int 	m_socket;
			sockaddr_rc 	m_addr;
	};

	/**
		@class XdevLBluetoothLinuxImpl
		@brief Core class to support Bluetooth.
		@author Cengiz Terzibas
	*/

	class XdevLBluetoothLinuxImpl : public XdevLModuleImpl<XdevLBluetooth> {
		public:
			XdevLBluetoothLinuxImpl(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLBluetoothLinuxImpl() {}

			static XdevLModuleDescriptor m_bluetoothModuleDescriptor;
			
			virtual xdl_int scan(const xdl_char* device);

			virtual xdl_int open(const XdevLFileName& host);
			virtual xdl_int close();
			virtual xdl_int bind(const xdl_char* host = 0);
			virtual xdl_int write(xdl_uint8* src, xdl_int size);
			virtual xdl_int read(xdl_uint8* dst, xdl_int size);
			virtual xdl_int flush();
			virtual xdl_int listen(xdl::xdl_int backlog);
			virtual xdl_int accept(XdevLBluetoothAddress** addr);
		protected:
			virtual xdl_int init();
			virtual xdl_int shutdown();


		private:
			virtual xdl_int open();

			xdl_int readInfoFromXMLFile();
		private:
			xdl::xdl_int m_socket;
			std::vector<XdevLBluetoothAddressImpl*> m_acceptedSockets;
			std::string m_scanedDevices;
			XdevLFileName m_deviceName;
	};

}

#endif
