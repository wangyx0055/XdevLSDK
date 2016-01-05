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
#include <XdevLUtils.h>
#include <signal.h>

xdl::IPXdevLCore core 				= nullptr;
xdl::IPXdevLTCPSocket tcp_socket 	= nullptr;

void exitHandle(int signal) {
	// Close socket.
	tcp_socket->close();

	exit(0);
}

int main(int argc, char** argv) {

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
	if(xdl::createCore(&core, argc, argv) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	// Plug the XdevLNetwork plugin.
	if(core->plug(xdl::XdevLPluginName("XdevLNetwork-1.0.0")) == xdl::ERR_ERROR) {
		return xdl::ERR_ERROR;
	}

	// Create TCP socket.
	tcp_socket = xdl::createModule<xdl::XdevLTCPSocket*>(core, xdl::XdevLModuleName("XdevLTCPSocket"), xdl::XdevLID("MyTCPSocket"));
	if(tcp_socket == NULL) {
		return xdl::ERR_ERROR;
	}

	// Connect socket to server.
	if(tcp_socket->connect(xdl::XdevLHostName("localhost"), xdl::XdevLPort(5432)) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	std::cout << "Press CTRL + C to stop application.\n";
	while(true) {

		xdl::xdl_uint8 data[64] {0};

		xdl::xdl_int ret = 0;
		// Receiving will block until a send is called on the server side.
		if( (ret = tcp_socket->read(data, 64)) != -1) {
			xdl::XdevLArrayModifier buffer(data, ret);

			std::cout << "Server XdevLCore Time: " << buffer.read<xdl::xdl_double>() << "\r" << std::flush;
		} else {
			std::cout << "Buffer problem.\n";
		}
	}

	return 0;
}
