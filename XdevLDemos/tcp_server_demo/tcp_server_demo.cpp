/*
	Application: tcp_client_demo.cpp
	Author     : Cengiz Terzibas
	Brief      : Demonstrates how to use the socket plugin.

*/

#include <XdevL.h>
#include <XdevLArrayModifier.h>
#include <XdevLNetwork/XdevLSocket.h>
#include <signal.h>

xdl::IPXdevLCore core 				= nullptr;
xdl::IPXdevLTCPSocket tcp_socket 	= nullptr;

void exitHandle(int signal) {

	xdl::destroyCore(core);

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

	// Start socket as server wich will bind the socket to the host and port
	// and start listening.
	if(tcp_socket->startServer(xdl::XdevLHostName("localhost"), xdl::XdevLPort(5432)) != xdl::ERR_OK) {
		std::cerr << "Could not start server.\n";
		return xdl::ERR_ERROR;
	}

	// This function blocks because the socket is in Blocking-Mode.
	// Wait until a client will connect.
	if(tcp_socket->accept() != xdl::ERR_OK)
		return xdl::ERR_ERROR;

	xdl::xdl_uint8 data[64] {0};

	std::cout << "Press CTRL + C to stop application.\n";
	while(true) {
		xdl::XdevLArrayModifier buffer(data, 64);

		buffer.write<xdl::xdl_double>(core->getTime());

		// Sending will block too until a receive is called on the client side.
		if(tcp_socket->write(buffer.getData(), buffer.getSize()) == -1)
			break;
		else {
			buffer.reset();
			std::cout << "XdevLCore Time: " << buffer.read<xdl::xdl_double>() << "\r" << std::flush;
		}
		// Ok, we just wanna sent every 100ms an information. Not the whole time.
		// The client will be run with 100ms too if the client socket is in Blocking-Mode.
		xdl::sleep(0.1);
	}

	xdl::destroyCore(core);

	return 0;
}
