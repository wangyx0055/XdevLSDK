/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2012 Cengiz Terzibas

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
