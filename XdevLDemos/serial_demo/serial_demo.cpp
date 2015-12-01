/*
	Application: serial_demo.cpp
	Author     : Cengiz Terzibas 
	Brief      : Demonstrates how to use the serial port plugin.
*/

#include <XdevL.h>
#include <XdevLSerial/XdevLSerial.h>
#include <csignal>
#include <iomanip>

// Serial Port Object Pointer.
xdl::IPXdevLSerial serial = nullptr;

void exitHandle(int signal) {
	
	std::cout << "CTRL + C / Termination captured." << std::endl;
	
	// Close serial port.
	serial->close();
	
	// Exit program.
	exit(0);
}

int main(int argc, char* argv[]) {

	// Let's register a CTRL + c signal handler.
	if(signal(SIGINT, exitHandle) == SIG_ERR) {
		std::cerr <<  "Failed to set SIGINT handler." << std::endl;
		return -1;
	}
	
	// Register a termination handler.
	if(signal(SIGTERM, exitHandle) == SIG_ERR) {
		std::cerr <<  "Failed to set SIGTERM handler." << std::endl;
		return -1;
	}
	
	// Create the core system using a XML file.
	xdl::IPXdevLCore core = nullptr;
	if(xdl::createCore(&core, argc, argv, xdl::XdevLFileName("serial_demo.xml")) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}
	
	// Get the serial port module.
	serial = xdl::getModule<xdl::IPXdevLSerial>(core, xdl::XdevLID("MySerial"));
	if(serial == NULL)
		return xdl::ERR_ERROR;
	
	// Connect the serial port module to a port.
	if(serial->open() != xdl::ERR_OK){
		// Could not open the serial port.
		std::cerr << "Could not open a connection to serial device.\n";
		return xdl::ERR_ERROR;
	}

	// Switch output into decimal mode.
	std::cout << std::dec;
	
	// Start the main loop.
	while(true) {
		
		xdl::xdl_uint8 sync;
		
		// Keep reading until we get a specific value. 
		// Note: This is for my serial device. It sends a sync byte = 255 that tells me when a new package
		// will be send. So don't use this part for your device. :D
		do {
			xdl::xdl_int size = serial->read((xdl::xdl_uint8*)&sync, 1);
		}while(sync != 255);
		
		// Read the counter value of my serial device.
		xdl::xdl_uint8 counter;
		serial->read((xdl::xdl_uint8*)&counter, 1);
		
		std::cout << (unsigned int)counter << std::endl;
	}
	
	return 0;
}

