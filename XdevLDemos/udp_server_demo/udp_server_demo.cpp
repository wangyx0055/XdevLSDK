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

enum Requests {
    // Do nothing request.
    REQ_NOTHING,
    // Get XdevLCore time request.
    REQ_GETTIME,
    // Stop server.
    REQ_STOP_SERVER,
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
	if(xdl::createCore(&core, argc, argv, xdl::XdevLFileName("udp_server_demo.xml")) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	// Get the UDP socket from the core system.
	udp_socket = xdl::getModule<xdl::IPXdevLUDPSocket2>(core, xdl::XdevLID("MyUDPSocket"));
	if(!udp_socket) {
		return xdl::ERR_ERROR;
	}

	// Bind the socket to a port.
	if(udp_socket->bind(xdl::XdevLPort(4321)) != xdl::ERR_OK) {
		std::cerr << "Bind failed.\n";
		return xdl::ERR_ERROR;
	}

	xdl::xdl_bool run = true;
	while(run) {
		core->update();

		myNetworkData data;
		if(udp_socket->read((xdl::xdl_uint8*)&data, sizeof(myNetworkData)) != sizeof(myNetworkData))
			std::cerr << "Couldn't reveive request properly.\n";

		switch(data.request) {
			case REQ_GETTIME: {
				data.time			= core->getTime();
				data.request	= REQ_GETTIME;
				if(udp_socket->write((xdl::xdl_uint8*)&data, sizeof(myNetworkData)) != sizeof(myNetworkData))
					std::cerr << "Couldn't send requested data: " << udp_socket->getError() << std::endl;
			}
			break;
			break;
			case REQ_STOP_SERVER: {
				std::clog << "REQ_STOP_SERVER received. Stopping server.\n";
				data.time			= core->getTime();
				data.request	= REQ_STOP_SERVER;
				if(udp_socket->write((xdl::xdl_uint8*)&data, sizeof(myNetworkData)) != sizeof(myNetworkData))
					std::cerr << "Couldn't send requested data.\n";
				run = false;
				continue;
			}
			case REQ_NOTHING:
			default:
				break;
		}
		std::cout << "XdevLCore time: " << data.time << "\r" << std::flush;

		// Update the system every 100ms if the client runs faster.
		xdl::sleep(0.1);

	}
	// Close socket.
	udp_socket->close();

	// Destroy the Core.
	xdl::destroyCore(core);
	return 0;
}
