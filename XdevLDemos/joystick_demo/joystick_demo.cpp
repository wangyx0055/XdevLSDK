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
#include <XdevLInput/XdevLJoystick/XdevLJoystick.h>

#include <csignal>

xdl::IPXdevLCore core = nullptr;

void callbackButton0(const xdl::XdevLButtonState& id) {
	std::cout << "BUTTON_0: " << ((id == xdl::BUTTON_PRESSED) ? "pressed" : "released") << std::endl;
}

void callbackButton1(const xdl::XdevLButtonState& id) {
	std::cout << "BUTTON_1: " << ((id == xdl::BUTTON_PRESSED) ? "pressed" : "released") << std::endl;
}

void callbackAxis0(const xdl::xdl_float& value) {
	std::cout << "AXIS_0: " << value << std::endl;
}

void callbackAxis1(const xdl::xdl_float& value) {
	std::cout << "AXIS_1: " << value << std::endl;
}

void exitHandle(int signal) {

	// Destroy the Core.
	xdl::destroyCore(core);

	exit(0);
}

int main(int argc, char **argv) {

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

	//
	// Create the core
	//
	if(xdl::createCore(&core, argc, argv) != xdl::ERR_OK) {
		return -1;
	}

	//
	// Plug the necessary plugins.
	//
	xdl::plug(core, xdl::XdevLPluginName("XdevLJoystickServerLinux"), xdl::XdevLVersion(0, 1, 0));
	xdl::plug(core, xdl::XdevLPluginName("XdevLJoystick"), xdl::XdevLVersion(1, 0, 0));

	//
	// Get the joystick module instance.
	//
	xdl::IPXdevLJoystick joystick = xdl::createModule<xdl::IPXdevLJoystick>(core, xdl::XdevLModuleName("XdevLJoystick"), xdl::XdevLID("MyJoystick"));
	if(xdl::isModuleNotValid(joystick)) {
		xdl::destroyCore(core);
		return -1;
	}

	//
	// We need info which joysticks are available.
	//
	xdl::IPXdevLJoystickServer joystickServer = xdl::createModule<xdl::IPXdevLJoystickServer>(core, xdl::XdevLModuleName("XdevLJoystickServer"), xdl::XdevLID("MyJoystickServer"));
	if(nullptr == joystickServer) {
		xdl::destroyCore(core);
		return -1;
	}

	// And use that info to connect to the joystick device we want.
	//
	if(joystick->create(joystickServer, xdl::XdevLJoystickId::JOYSTICK_DEFAULT) != xdl::ERR_OK) {
		xdl::destroyCore(core);
		return -1;
	}

	//
	// Now we use delegates that will help us to managed events. When a specific button or axis is used
	// the delegate will call a function/member function that we can use to do some stuff.
	//
	xdl::XdevLButtonIdDelegateType button0Delegate = xdl::XdevLButtonIdDelegateType::Create<&callbackButton0>();
	xdl::XdevLButtonIdDelegateType button1Delegate = xdl::XdevLButtonIdDelegateType::Create<&callbackButton1>();

	xdl::XdevLAxisIdDelegateType axis0Delegate = xdl::XdevLAxisIdDelegateType::Create<&callbackAxis0>();
	xdl::XdevLAxisIdDelegateType axis1Delegate = xdl::XdevLAxisIdDelegateType::Create<&callbackAxis1>();

	joystick->registerDelegate(xdl::XdevLString("BUTTON_0"), button0Delegate);
	joystick->registerDelegate(xdl::XdevLString("BUTTON_1"), button1Delegate);
	joystick->registerDelegate(xdl::XdevLString("AXIS_0"), axis0Delegate);
	joystick->registerDelegate(xdl::XdevLString("AXIS_1"), axis1Delegate);

	// Start the main loop.
	for(;;) {

		// We need to call the update of the core manually.
		core->update();

		// Let's not use too much CPU power.
		xdl::sleep(0.001);
	}

	// We, this might be never called in this demo but I like to show
	// that the core has to be destroyed at the end. In this example
	// we quite using CTRL + C or terminating see above. :D.
	xdl::destroyCore(core);

	return 0;
}
