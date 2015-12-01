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

#ifndef XDEVL_BLUETOOTH_H
#define XDEVL_BLUETOOTH_H

#include <XdevLStream.h>

namespace xdl {

// Holds the major version number of the plugin.
const xdl_uint XdevLBluetoothPluginMajorVersion = 0;
// Holds the Minor version number of the plugin.
const xdl_uint XdevLBluetoothPluginMinorVersion = 1;
// Holds the Patch version number of the plugin.
const xdl_uint XdevLBluetoothPluginPatchVersion = 0;


// Holds the Major version number.
const xdl_uint XdevLBluetoothMajorVersion = 0;
// Holds the Minor version number.
const xdl_uint XdevLBluetoothMinorVersion = 1;
// Holds the Patch version number.
const xdl_uint XdevLBluetoothPatchVersion = 0;

/**
	@class XdevLBluetoothAddress
	@brief Bluetooth address
	@author Cengiz Terzibas
*/
class XdevLBluetoothAddress {
public:
	XdevLBluetoothAddress() {}
	virtual~ XdevLBluetoothAddress() {}
};

/**
	@class XdevLBluetooth
	@brief Core class to support Bluetooth sockets.
	@author Cengiz Terzibas
*/
class XdevLBluetooth : public XdevLStream {
public:
	virtual ~XdevLBluetooth() {};
	/// Bind the bluetooth socket.
	/**
		@return ERR_OK if it was succesful otherwise ERR_ERROR and errno
		is set appropriately.
	*/
	virtual xdl_int bind(const xdl_char* host = 0) 				= 0;
	/// Listen to the bluetooth socket.
	/**
		@return ERR_OK if it was succesful otherwise ERR_ERROR and errno
		is set appropriately.
	*/
	virtual xdl_int listen(xdl::xdl_int backlog)					= 0;
	/// Accept client connections.
	/**
		To use this you create a valid XdevLBluetoothAddress pointer object and pass the pointer
		as an argument to the accept() function.
		@code
		xdl::XdevLBluetoothAddress* addr = NULL;
		if(bluetooth->accept(&addr) == ERR_OK){
			// Ok, Here we accepted a client. You should store addr somewhere if you need to.
		}
		@endcode

		@return ERR_OK if it was succesful otherwise ERR_ERROR and errno
		is set appropriately.
	*/
	virtual xdl_int accept(XdevLBluetoothAddress** addr)	= 0;
};

}

#endif
