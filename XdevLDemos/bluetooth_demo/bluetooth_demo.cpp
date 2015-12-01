/*
	Application: bluetooth_demo.cpp
	Author     : Cengiz Terzibas 
	Brief      : Demonstrates how to use the bluetooth plugin.
*/

#include <XdevL.h>
#include <XdevLBluetooth/XdevLBluetooth.h>
#include <XdevLSerial/XdevLSerial.h>
#include <XdevLThread.h>
#include <XdevLMutex.h>

#include <signal.h>
#include <cmath>

xdl::XdevLBluetooth* bluetooth = NULL;

void exitHandle(int signal){

	// Close connection.
	bluetooth->close();	
	
	exit(0);
}

int main(int argc, char* argv[]) {
	
	//Handle the CTRL + C event.
	if(signal(SIGINT, exitHandle) == SIG_ERR){
		std::cerr << "Could not connect signal.\n";
	}
	
	// Create the core system.
	xdl::XdevLCorePtr core;
	if(xdl::createCore(argc, argv, "bluetooth_demo.xml", core) != xdl::ERR_OK){
		return xdl::ERR_ERROR;
	}

	// Get the instance to the socket module.
	bluetooth = xdl::getModule<xdl::XdevLBluetooth*>(core, "MyBluetooth");
	if(!bluetooth){
		return xdl::ERR_ERROR;
	}
	
	// This number is just an example. You have to use the one of your device.
	if(bluetooth->open("(00:0B:CE:05:F2:CF)") == xdl::ERR_ERROR){
		return xdl::ERR_ERROR;
	}
	
//	const int numberToRead = 16;
//	// Start the main loop.
//	while(true){
//		// Initialize the buffer with zeros.
//		xdl::xdl_uint8 buffer[numberToRead] = {0};
//		
//		// Get data from the serial port buffer.
//		xdl::xdl_int size = bluetooth->read(buffer, numberToRead);
//		if(size == -1) {
//			std::cerr << "Error: " << strerror(errno) << std::endl;
//		}else {
//			std::cout << std::hex;
//			// Print out how some bytes in hex format.
//			for(int a = 0; a < numberToRead; a++)
//				std::cout << (xdl::xdl_int)(buffer[a]&0xff) << " ";
//			std::cout << std::endl;
//		}
//		// Just wait a bit.
//		xdl::sleep(0.004);
//	}
	return 0;
}

