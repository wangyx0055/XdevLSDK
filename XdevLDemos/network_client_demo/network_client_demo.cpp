/*
	Application: network_client_demo.cpp
	Author     : Cengiz Terzibas 
	Brief      : Demonstrates how to use the socket plugin.
*/

#include <XdevL.h>

struct myArray {
	xdl::xdl_double	time;
};

int main(int argc, char* argv[]) {
	// To get the XdevL system ready we have to create the core system.
	xdl::XdevLCorePtr core = xdl::createCore(argc, argv,"network_client_demo.xml");

	// Get the instance to the socket module.
	xdl::XdevLUDPSocket* socket = xdl::getModule<xdl::XdevLUDPSocket*>(core, "MyUDPSocket");
	if(!socket){
		return xdl::ERR_ERROR;
	}

	socket->bind(4321);
	
	myArray data;
	
	// Just do that for 10 seconds.
	if(socket->receive((xdl::xdl_int8*)&data, sizeof(myArray)) != xdl::ERR_ERROR)
		std::cout << "Core server time: " << data.time << "\n\n";	
			
	socket->close();

	return 0;
}

