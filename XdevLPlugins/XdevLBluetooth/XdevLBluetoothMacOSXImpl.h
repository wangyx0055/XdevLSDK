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

#ifndef XDEVL_BLUETOOTH_MACOSXIMPL_H
#define XDEVL_BLUETOOTH_MACOSXIMPL_H

#include <XdevLBluetooth.h>
#include <XdevLPluginImpl.h>
#include <XdevLUtils.h>
#include <IOBluetooth/IOBluetooth.h>
#include <IOBluetooth/IOBluetoothUserLib.h>
#include <IOBluetooth/IOBluetoothUtilities.h>
#include <IOBluetoothUI/IOBluetoothUI.h>
#include <IOBluetoothUI/IOBluetoothUIUserLib.h>

namespace xdl {

static const XdevLString	vendor 				{"www.codeposer.net"};
static const XdevLString	author				{"Cengiz Terzibas"};
static const XdevLString	copyright			{"(c) 2005 - 2012 Cengiz Terzibas."};
static const XdevLString	pluginName 		{"XdevLBluetooth"};
static const XdevLString description 	{"A bluetooth device communicaton module."};

static const std::vector<XdevLModuleName>	moduleNames	{ 
																													XdevLModuleName("XdevLBluetooth")
																												};

class XdevLBluetoothAddressImpl : public XdevLBluetoothAddress {
public:
	XdevLBluetoothAddressImpl() {}
	virtual~ XdevLBluetoothAddressImpl() {}
	
protected:

};

/**
	@class XdevLBluetoothMacOSXImpl
	@brief Reference implementation for the MacOSX.
	@author Cengiz Terzibas
*/

class XdevLBluetoothMacOSXImpl : public XdevLModuleImpl<XdevLBluetooth> {
public:
	XdevLBluetoothMacOSXImpl(XdevLModuleCreateParameter* parameter);
	virtual ~XdevLBluetoothMacOSXImpl() {}

	static XdevLModuleDescriptor m_bluetoothModuleDesc;
	
	virtual xdl_int open();
	virtual xdl_int open(const XdevLFileName& host);
	virtual xdl_int close();
	virtual xdl_int flush();
	virtual xdl_int bind(const xdl_char* host = 0);
	virtual xdl_int write(xdl_uint8* src, xdl_int size);
	virtual xdl_int read(xdl_uint8* dst, xdl_int size);
	virtual xdl_int listen(xdl::xdl_int backlog);
	virtual xdl_int accept(XdevLBluetoothAddress** addr);
protected:
	virtual xdl_int init();
	virtual xdl_int shutdown();


	xdl_int readInfoFromXMLFile();
private:
	std::vector<XdevLBluetoothAddressImpl*> m_acceptedSockets;
	XdevLFileName m_deviceName;
	
	IOBluetoothRFCOMMChannelRef	m_RFCOMMChannelRef;
};

}

#endif
