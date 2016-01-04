#include <XdevL.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLInput/XdevLMouse/XdevLMouse.h>

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

int main(int argc, char **argv) {

	//
	// Create the core
	//
	xdl::IPXdevLCore core = nullptr;
	xdl::createCore(&core, argc, argv, xdl::XdevLFileName("devices_demo.xml"));


	//
	// Create the window module instance.
	//
	xdl::IPXdevLWindow window = xdl::createModule<xdl::IPXdevLWindow>(core, xdl::XdevLModuleName("XdevLWindow"), xdl::XdevLID("MyWindow"));
	if(xdl::isModuleNotValid(window)) {
		xdl::destroyCore(core);
		return -1;
	}

	window->create();
	
	
	window->show();

	//
	// Create the joystick module instance.
	//
	xdl::IPXdevLMouse mouse = xdl::createModule<xdl::IPXdevLMouse>(core, xdl::XdevLModuleName("XdevLMouse"), xdl::XdevLID("MyMouse"));
	if(xdl::isModuleNotValid(mouse)) {
		xdl::destroyCore(core);
		return -1;
	}

	mouse->attach(window);

	//
	// Now we use delegates that will help us to managed events. When a specific button or axis is used
	// the delegate will call a function/member function that we can use to do some stuff.
	//
	xdl::XdevLButtonIdDelegateType button0Delegate = xdl::XdevLButtonIdDelegateType::Create<&callbackButton0>();
	xdl::XdevLButtonIdDelegateType button1Delegate = xdl::XdevLButtonIdDelegateType::Create<&callbackButton1>();
	xdl::XdevLButtonIdDelegateType button2Delegate = xdl::XdevLButtonIdDelegateType::Create<&callbackButton2>();

	xdl::XdevLAxisIdDelegateType axis0Delegate = xdl::XdevLAxisIdDelegateType::Create<&callbackAxis0>();
	xdl::XdevLAxisIdDelegateType axis1Delegate = xdl::XdevLAxisIdDelegateType::Create<&callbackAxis1>();

//	mouse->registerDelegate(xdl::XdevLString("BUTTON_0"), button0Delegate);
//	mouse->registerDelegate(xdl::XdevLString("BUTTON_1"), button1Delegate);
//	mouse->registerDelegate(xdl::XdevLString("BUTTON_2"), button2Delegate);
//	mouse->registerDelegate(xdl::XdevLString("AXIS_0"), axis0Delegate);
//	mouse->registerDelegate(xdl::XdevLString("AXIS_1"), axis1Delegate);

	xdl::xdl_float prev_axis_0 = mouse->getDeltaValue(xdl::AXIS_0);
	xdl::xdl_float prev_axis_1 = mouse->getDeltaValue(xdl::AXIS_1);
	
	// Start the main loop.
	for(;;) {
		core->update();
		if(prev_axis_0 != mouse->getDeltaValue(xdl::AXIS_0) || prev_axis_1 != mouse->getDeltaValue(xdl::AXIS_1)) {
			std::cout << mouse->getDeltaValue(xdl::AXIS_0) << " : " << mouse->getDeltaValue(xdl::AXIS_1) << std::endl;
		}
		xdl::sleep(0.001);
	}

	xdl::destroyCore(core);

	return 0;
}
