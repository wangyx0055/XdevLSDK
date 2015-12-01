/*
	Application: udp_client_array_demo.cpp
	Author     : Cengiz Terzibas
	Brief      : Demonstrates how to use the udp2 array member functions.

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


	if(udp_socket->connect(xdl::XdevLHostName("localhost"), xdl::XdevLPort(4321)) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	// Create an array declaration. It describes the structure of the array.
	xdl::XdevLArrayDeclaration arrayDecl;
	arrayDecl.add(xdl::ARRAY_FLOAT_ELEMENT);
	arrayDecl.add(xdl::ARRAY_UINT16_ELEMENT);
	arrayDecl.add(xdl::ARRAY_DOUBLE_ELEMENT);
	arrayDecl.add(xdl::ARRAY_UINT8_ELEMENT);
	arrayDecl.add(xdl::ARRAY_UINT16_ELEMENT);

	// Create an array to store the data.
	std::array<xdl::xdl_uint8, 64> dataToTransfer {{0}};

	while(true) {

		// This class helps us to add values into the array.
		xdl::XdevLArrayModifier arrayModifier(dataToTransfer.data(), 64);

		// Put some values into the array.
		arrayModifier.write<xdl::xdl_float>(10.123f);
		arrayModifier.write<xdl::xdl_uint16>(20);
		arrayModifier.write<xdl::xdl_double>(30.9394);
		arrayModifier.write<xdl::xdl_uint8>(1);
		arrayModifier.write<xdl::xdl_uint16>(40);

		// Send the stuff.
		udp_socket->sendArray((xdl::xdl_uint8*)dataToTransfer.data(), arrayDecl);

		// Wait a bit
		//	xdl::sleep(0.01);

	}

	udp_socket->close();

	// Destroy Core.
	xdl::destroyCore(core);

	return 0;
}
