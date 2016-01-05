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
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLInput/XdevLMouse/XdevLMouse.h>

#include <csignal>

xdl::IPXdevLCore core = nullptr;

void callbackButton0(const xdl::XdevLButtonState& id) {
	std::cout << "BUTTON_0: " << ((id == xdl::BUTTON_PRESSED) ? "pressed" : "released") << std::endl;
}

void callbackButton1(const xdl::XdevLButtonState& id) {
	std::cout << "BUTTON_1: " << ((id == xdl::BUTTON_PRESSED) ? "pressed" : "released") << std::endl;
}

void callbackButton2(const xdl::XdevLButtonState& id) {
	std::cout << "BUTTON_2: " << ((id == xdl::BUTTON_PRESSED) ? "pressed" : "released") << std::endl;
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
	if(xdl::createCore(&core, argc, argv, xdl::XdevLFileName("mouse_demo.xml")) != xdl::ERR_OK) {
		return -1;
	}

	//
	// Create the window module instance.
	//
	xdl::IPXdevLWindow window = xdl::createModule<xdl::IPXdevLWindow>(core, xdl::XdevLModuleName("XdevLWindow"), xdl::XdevLID("MyWindow"));
	if(xdl::isModuleNotValid(window)) {
		xdl::destroyCore(core);
		return -1;
	}

	// This will create the window using default parameters or using the attributes provided in the XML file.
	window->create();

	// Maps the window to the display so we can see it.
	window->show();

	//
	// Create the joystick module instance.
	//
	xdl::IPXdevLMouse mouse = xdl::createModule<xdl::IPXdevLMouse>(core, xdl::XdevLModuleName("XdevLMouse"), xdl::XdevLID("MyMouse"));
	if(xdl::isModuleNotValid(mouse)) {
		xdl::destroyCore(core);
		return -1;
	}

	// Attach this mouse to the window. We have to do this otherwise the XdevLMouse module will not work.
	mouse->attach(window);

	//
	// Now we use delegates that will help us to managed events. When a specific button or axis is used
	// the delegate will call a function/member that we can use to do some stuff.
	//
	xdl::XdevLButtonIdDelegateType button0Delegate = xdl::XdevLButtonIdDelegateType::Create<&callbackButton0>();
	xdl::XdevLButtonIdDelegateType button1Delegate = xdl::XdevLButtonIdDelegateType::Create<&callbackButton1>();
	xdl::XdevLButtonIdDelegateType button2Delegate = xdl::XdevLButtonIdDelegateType::Create<&callbackButton2>();

	xdl::XdevLAxisIdDelegateType axis0Delegate = xdl::XdevLAxisIdDelegateType::Create<&callbackAxis0>();
	xdl::XdevLAxisIdDelegateType axis1Delegate = xdl::XdevLAxisIdDelegateType::Create<&callbackAxis1>();

	mouse->registerDelegate(xdl::XdevLString("BUTTON_0"), button0Delegate);
	mouse->registerDelegate(xdl::XdevLString("BUTTON_1"), button1Delegate);
	mouse->registerDelegate(xdl::XdevLString("BUTTON_2"), button2Delegate);
	mouse->registerDelegate(xdl::XdevLString("AXIS_0"), axis0Delegate);
	mouse->registerDelegate(xdl::XdevLString("AXIS_1"), axis1Delegate);

	xdl::xdl_float prev_axis_0 = mouse->getDeltaValue(xdl::AXIS_0);
	xdl::xdl_float prev_axis_1 = mouse->getDeltaValue(xdl::AXIS_1);

	// Start the main loop.
	for(;;) {

		// We need to call the update of the core manually.
		core->update();

		// Let's not use too much CPU power.
		xdl::sleep(0.001);
	}

	xdl::destroyCore(core);

	return 0;
}
