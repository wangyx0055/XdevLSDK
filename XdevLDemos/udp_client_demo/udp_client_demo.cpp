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
#include <XdevLNetwork/XdevLSocket.h>
#include <signal.h>

xdl::IPXdevLCore core				= nullptr;
xdl::IPXdevLUDPSocket2 udp_socket	= nullptr;
xdl::xdl_bool run					= true;


enum Requests {
    // Do nothing request.
    REQ_NOTHING,
    // Get XdevLCore time request.
    REQ_GETTIME,
    // Stop server.
    REQ_STOP_SERVER
};

struct myNetworkData {
	myNetworkData() : request(REQ_NOTHING), time(0.0) {}
	// Holds the request information.
	xdl::xdl_int8		request;
	// Holds the time information.
	xdl::xdl_double time;
};


void exitHandle(int signal) {

	// Close socket.
	udp_socket->close();

	// Destroy the Core.
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
	if(xdl::createCore(&core, argc, argv, xdl::XdevLFileName("udp_client_demo.xml")) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	// Get the instance to the socket module.
	udp_socket = xdl::getModule<xdl::XdevLUDPSocket2*>(core, xdl::XdevLID("MyUDPSocket"));
	if(!udp_socket) {
		return xdl::ERR_ERROR;
	}

	if(udp_socket->connect(xdl::XdevLHostName("localhost"), xdl::XdevLPort(4321)) != xdl::ERR_OK)
		return xdl::ERR_ERROR;

	myNetworkData 					data;
	while(run) {
		data.request = REQ_GETTIME;

		if(udp_socket->write((xdl::xdl_uint8*)&data, sizeof(myNetworkData)) != sizeof(myNetworkData)) {
			std::cerr << "Couldn't send request: " << udp_socket->getError() << std::endl;
		}

		if(udp_socket->read((xdl::xdl_uint8*)&data, sizeof(myNetworkData)) == sizeof(myNetworkData)) {
			std::cout << "XdevLCore server time: " << data.time << "\r" << std::flush;
		}

	}

	// Close socket.
	udp_socket->close();

	// Destroy the Core.
	xdl::destroyCore(core);

	return 0;
}
