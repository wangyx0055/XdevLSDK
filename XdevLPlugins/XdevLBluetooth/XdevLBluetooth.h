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

#ifndef XDEVL_BLUETOOTH_H
#define XDEVL_BLUETOOTH_H

#include <XdevLStream.h>

namespace xdl {

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
	virtual xdl_int bind(const xdl_char* host = 0) = 0;
	/// Listen to the bluetooth socket.
	/**
		@return ERR_OK if it was succesful otherwise ERR_ERROR and errno
		is set appropriately.
	*/
	virtual xdl_int listen(xdl::xdl_int backlog) = 0;
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
	virtual xdl_int accept(XdevLBluetoothAddress** addr) = 0;
};

}

#endif
