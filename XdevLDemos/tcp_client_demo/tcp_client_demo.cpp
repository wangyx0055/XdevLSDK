/*
	Application: tcp_client_demo.cpp
	Author     : Cengiz Terzibas
	Brief      : Demonstrates how to use the socket plugin.

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
