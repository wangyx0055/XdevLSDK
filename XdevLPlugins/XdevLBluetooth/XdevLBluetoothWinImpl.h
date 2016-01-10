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
#include <WinBluetoothWrapper.h>

#include <WinSock2.h>

namespace xdl {


	static const std::vector<XdevLModuleName>	moduleNames	{
		XdevLModuleName("XdevLBluetooth")
	};

	class XdevLBluetoothAddressImpl : public XdevLBluetoothAddress {
		public:
			XdevLBluetoothAddressImpl(SOCKET socket, SOCKADDR_BTH addr) : m_socket(socket), m_addr(addr) {}
			virtual~ XdevLBluetoothAddressImpl() {}
			SOCKADDR_BTH get() {
				return m_addr;
			}
		protected:
			SOCKET 				m_socket;
			SOCKADDR_BTH 	m_addr;
	};



	/**
		@class XdevLBluetoothWinImpl
		@brief Core class to support Bluetooth.
		@author Cengiz Terzibas
	*/

	class XdevLBluetoothWinImpl : public XdevLModuleImpl<XdevLBluetooth> {
		public:
			XdevLBluetoothWinImpl(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
				XdevLModuleImpl<XdevLBluetooth>(parameter, descriptor),
				m_handle(NULL),
				m_wVersionRequested(0x202) {};
			virtual ~XdevLBluetoothWinImpl() {}

			virtual xdl_int scan(const xdl_char* device);
			virtual xdl_int open();
			virtual xdl_int open(const XdevLFileName& name);
			virtual xdl_int open(const xdl_char* name);
			virtual xdl_int close();
			virtual xdl_int flush();
			virtual xdl_int bind(const xdl_char* host = 0);
			virtual xdl_int write(xdl_uint8* src, xdl_int size);
			virtual xdl_int read(xdl_uint8* dst, xdl_int size);
			virtual xdl_int listen(xdl_int backlog);
			virtual xdl_int accept(XdevLBluetoothAddress** addr);
		protected:
			virtual xdl_int init();
			virtual xdl_int shutdown();



			xdl_int readInfoFromXMLFile();
		private:
			HANDLE						m_handle;
			WORD 							m_wVersionRequested;
			WSADATA 					m_wsaData;
			SOCKET 						m_socket;
			WSAPROTOCOL_INFO 	m_protocolInfo;
			SOCKADDR_BTH 			m_connection;
			std::vector<XdevLBluetoothAddressImpl*> m_acceptedSockets;

			std::string m_deviceName;
			xdl_int			m_port;
	};

}

#endif
