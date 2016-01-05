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

#include <XdevL.h>
#include <XdevLArrayModifier.h>
#include <XdevLNetwork/XdevLSocket.h>
#include <signal.h>
#include <array>

xdl::IPXdevLCore core				= nullptr;
xdl::IPXdevLUDPSocket2 udp_socket	= nullptr;

void exitHandle(int signal) {

	// Close socket.
	udp_socket->close();

	// Destroy Core.
	xdl::destroyCore(core);

	exit(0);
}

int main(int argc, char* argv[]) {

	// Let's register the CTRL + c signal handler.
	if(signal(SIGINT, exitHandle) == SIG_ERR) {
		std::cerr <<  "Failed to set SIGINT handler." << std::endl;
		return -1;
	}

	// Register termination handler.
	if(signal(SIGTERM, exitHandle) == SIG_ERR) {
		std::cerr <<  "Failed to set SIGTERM handler." << std::endl;
		return -1;
	}

	// Create the core system.
	if(xdl::createCore(&core) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	// Plug the XdevLNetwork plugins into the core.
	if(xdl::plug(core, xdl::XdevLPluginName("XdevLNetwork"), xdl::XdevLVersion(1, 0, 0)) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	// Create the UDP socket object.
	udp_socket = xdl::createModule<xdl::XdevLUDPSocket2*>(core, xdl::XdevLModuleName("XdevLUDPSocket2"), xdl::XdevLID("MyUDPSocket"));
	if(!udp_socket) {
		return xdl::ERR_ERROR;
	}

	if(udp_socket->bind(xdl::XdevLPort(4321)) != xdl::ERR_OK) {
		std::cout << "Can't bind socket.\n";
		return xdl::ERR_ERROR;
	}

	// We need an array declaration because this will be send with the data. Using
	// this we will know how the send structure will look like.
	xdl::XdevLArrayDeclaration arrayDecl;

	std::array<xdl::xdl_uint8, 64> dataReceived {{0}};

	xdl::XdevLTimer timer;

	while(true) {
		timer.setT1();

		auto ret = 0;
		// Read incomming data.
		if((ret = udp_socket->receiveArray((xdl::xdl_uint8*)dataReceived.data(), arrayDecl)) != -1) {

			// This class helps us to read values from and array.
			xdl::XdevLArrayModifier buffer(dataReceived.data(), ret);

			// Iterate through all elements.
			for(auto& declaration : arrayDecl) {

				switch(declaration.element) {
					case xdl::ARRAY_DOUBLE_ELEMENT: {
						std::cout << "xdl_double: " << buffer.read<xdl::xdl_double>() << std::endl;
					}
					break;
					case xdl::ARRAY_FLOAT_ELEMENT: {
						std::cout << "xdl_float: " << buffer.read<xdl::xdl_float>() << std::endl;
					}
					break;
					case xdl::ARRAY_INT32_ELEMENT: {
						std::cout << "xdl_int32: " << buffer.read<xdl::xdl_int32>() << std::endl;
					}
					break;
					case xdl::ARRAY_UINT32_ELEMENT: {
						std::cout << "xdl_uint32: " << buffer.read<xdl::xdl_uint32>() << std::endl;
					}
					break;
					case xdl::ARRAY_INT16_ELEMENT: {
						std::cout << "xdl_int16: " << buffer.read<xdl::xdl_int16>() << std::endl;
					}
					break;
					case xdl::ARRAY_UINT16_ELEMENT: {
						std::cout << "xdl_uint16: " << buffer.read<xdl::xdl_uint16>() << std::endl;
					}
					break;
					case xdl::ARRAY_INT8_ELEMENT: {
						std::cout << "xdl_int8: " << (xdl::xdl_int)buffer.read<xdl::xdl_int8>() << std::endl;
					}
					break;
					case xdl::ARRAY_UINT8_ELEMENT: {
						std::cout << "xdl_uint8: " << (xdl::xdl_int)buffer.read<xdl::xdl_uint8>() << std::endl;
					}
					break;
				}
				std::cout << std::endl;
			}

		}
		timer.setT2();

		std::cout << "Time: " << (timer.getT2T164()) << " us" << std::endl;
	}

	udp_socket->close();

	// Destroy Core.
	xdl::destroyCore(core);

	return 0;
}
