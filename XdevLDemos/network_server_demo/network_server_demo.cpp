/*
	Application: network_server_demo.cpp
	Author     : Cengiz Terzibas 
	Brief      : Demonstrates how to use the socket plugin.
	
*/

#include <XdevL.h>

struct myArray {
	xdl::xdl_double time;
};

int main(int argc, char* argv[]) {

	// To get the XdevL system ready we have to create the core system.
	xdl::XdevLCorePtr core = xdl::createCore(argc, argv,"network_server_demo.xml");

	xdl::XdevLUDPSocket* socket = xdl::getModule<xdl::XdevLUDPSocket*>(core, "MyUDPSocket");
	if(!socket){
		return xdl::ERR_ERROR;
	}

	// Connect socket.
	socket->connect(4321,"127.0.0.1");

	myArray data;
	
	// Just do that for 10 seconds.
	while(core->getTime() < 10.0){
		data.time = core->getTime();
	
		socket->send((xdl::xdl_int8*)(&data), sizeof(myArray));
		
		core->update();
	}	

	socket->close();
	
	return 0;
}
