/*
	Application: udp_client_demo.cpp
	Author     : Cengiz Terzibas
	Brief      : Demonstrates how to use the udp2 socket plugin.
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
