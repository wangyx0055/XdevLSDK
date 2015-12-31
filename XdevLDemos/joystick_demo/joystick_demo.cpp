#include <XdevL.h>
#include <XdevLJoystick/XdevLJoystick.h>

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

int main(int argc, char **argv) {
	xdl::IPXdevLCore core = nullptr;
	xdl::createCore(&core, argc, argv);

	xdl::plug(core, xdl::XdevLPluginName("XdevLJoystickServerLinux"), xdl::XdevLVersion(0, 1, 0));
	xdl::plug(core, xdl::XdevLPluginName("XdevLJoystick"), xdl::XdevLVersion(1, 0, 0));

	xdl::IPXdevLJoystick joystick = xdl::createModule<xdl::IPXdevLJoystick>(core, xdl::XdevLModuleName("XdevLJoystick"), xdl::XdevLID("MyJoystick"));
	if(xdl::isModuleNotValid(joystick)) {
		xdl::destroyCore(core);
		return -1;
	}

	xdl::XdevLButtonIdDelegateType button0Delegate = xdl::XdevLButtonIdDelegateType::Create<&callbackButton0>();
	xdl::XdevLButtonIdDelegateType button1Delegate = xdl::XdevLButtonIdDelegateType::Create<&callbackButton1>();

	xdl::XdevLAxisIdDelegateType axis0Delegate = xdl::XdevLAxisIdDelegateType::Create<&callbackAxis0>();
	xdl::XdevLAxisIdDelegateType axis1Delegate = xdl::XdevLAxisIdDelegateType::Create<&callbackAxis1>();

	joystick->registerDelegate(xdl::XdevLString("BUTTON_0"), button0Delegate);
	joystick->registerDelegate(xdl::XdevLString("BUTTON_1"), button1Delegate);
	joystick->registerDelegate(xdl::XdevLString("AXIS_0"), axis0Delegate);
	joystick->registerDelegate(xdl::XdevLString("AXIS_1"), axis1Delegate);

	for(;;) {
		core->update();
		xdl::sleep(0.001);
	}

	xdl::destroyCore(core);

	return 0;
}
