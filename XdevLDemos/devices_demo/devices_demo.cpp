/*
	Application: devices_demo.cpp
	Author     : Cengiz Terzibas
	Brief      : Demonstrates how to use XdevLKeyboard, XdevLMouse and XdevLJoystick.
*/

#include <XdevL.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLKeyboard/XdevLKeyboard.h>
#include <XdevLInput/XdevLMouse/XdevLMouse.h>
#include <XdevLInput/XdevLJoystick/XdevLJoystick.h>


//#define USE_DELEGATES

// Some keyboard mappings.
xdl::IPXdevLButton escape	= nullptr;
xdl::IPXdevLButton m		= nullptr;
xdl::IPXdevLButton p		= nullptr;
xdl::IPXdevLButton v		= nullptr;

// Print mouse x,y coordinates on the screen.
xdl::xdl_bool verbose = xdl::xdl_false;

// Flag to run/stop the application.
xdl::xdl_bool run = xdl::xdl_true;

//
// This callback function we use to exit from the application.
//
void eventCallbackFunction(xdl::XdevLEvent& event) {
	switch(event.type) {
		case xdl::XDEVL_CORE_EVENT: {
			if(event.core.event == xdl::XDEVL_CORE_SHUTDOWN)
				run = xdl::xdl_false;
		}
		break;
		case xdl::XDEVL_KEY_PRESSED: {

		} break;
	}
}

//
// This class shows how to use the delegate oriented input handling.
//
class MyClassThatHandlesInput {
	public:
		MyClassThatHandlesInput(xdl::IPXdevLKeyboard keyboard, xdl::IPXdevLMouse mouse, xdl::IPXdevLJoystick joystick) {


			//
			// Uncomment this part to use delegates that handles all input events.
			//
			
//			buttonDelegate = xdl::XdevLButtonDelegateType::Create<MyClassThatHandlesInput, &MyClassThatHandlesInput::buttonDelegateHandler>(this);
//			axisDelegate = xdl::XdevLAxisDelegateType::Create<MyClassThatHandlesInput, &MyClassThatHandlesInput::axisDelegateHandler>(this);
//			
//			mouse->registerDelegate(buttonDelegate);
//			mouse->registerDelegate(axisDelegate);
//			joystick->registerDelegate(buttonDelegate);
//			joystick->registerDelegate(axisDelegate);


			// Create delegates.
			buttonIdDelegate = xdl::XdevLButtonIdDelegateType::Create<MyClassThatHandlesInput, &MyClassThatHandlesInput::button_0_handler>(this);
			buttonIdDelegate2 = xdl::XdevLButtonIdDelegateType::Create<MyClassThatHandlesInput, &MyClassThatHandlesInput::button_f10_handler>(this);

			// Assign to the mouse button 0 a delegate to receive state changes.
			mouse->registerDelegate(STRING("BUTTON_0"), buttonIdDelegate);

			// Assign to the keyboard key F10 a delegate to receive state changes.
			keyboard->registerDelegate(STRING("KEY_F10"), buttonIdDelegate2);



		}
//
//		void buttonDelegateHandler(const xdl::XdevLButtonId& button, const xdl::XdevLButtonState& eventType) {
//			std::cout << "Button: " << button << " was " << (eventType == xdl::BUTTON_PRESSED ? "pressed" : "released") << std::endl;
//		}
//
//		void axisDelegateHandler(const xdl::XdevLAxisId& axis, const xdl::xdl_float& value) {
//			std::cout << "Axis: " << axis << " value: " << value << std::endl;
//		}

		void button_0_handler(const xdl::XdevLButtonState& state) {
			std::cout << "BUTTON_0 was " << (state == xdl::BUTTON_PRESSED ? "pressed" : "released") << std::endl;
		}

		void button_f10_handler(const xdl::XdevLButtonState& state) {
			std::cout << "F10 was " << (state == xdl::BUTTON_PRESSED ? "pressed" : "released") << std::endl;
		}
		
	private:
		xdl::XdevLButtonDelegateType buttonDelegate;
		xdl::XdevLAxisDelegateType axisDelegate;
		xdl::XdevLButtonIdDelegateType buttonIdDelegate;
		xdl::XdevLButtonIdDelegateType buttonIdDelegate2;
};


int main(int argc, char* argv[]) {

	xdl::IPXdevLCore core = nullptr;

	// Create the core system.
	if(xdl::createCore(&core, argc, argv, xdl::XdevLFileName("devices_demo.xml")) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	// Register a listener callback function to get events. In this case to stop the application.
	core->registerListenerCallbackFunction(eventCallbackFunction);

	// Create a window because we can use keyboard/mouse only if we have one (hopefully will be fixed in later versions of XdevL).
	xdl::IPXdevLWindow window = xdl::createModule<xdl::IPXdevLWindow>(core, xdl::XdevLModuleName("XdevLWindow"), xdl::XdevLID("MyWindow"));
	if(nullptr == window) {
		return xdl::ERR_ERROR;
	}
	
	window->create();
	
	// Create the keyboard module to get access to the keyboard device.
	xdl::IPXdevLKeyboard keyboard = xdl::createModule<xdl::IPXdevLKeyboard>(core, xdl::XdevLModuleName("XdevLKeyboard"), xdl::XdevLID("MyKeyboard"));
	if(nullptr == keyboard)
		return xdl::ERR_ERROR;

	// Create the mouse module to get access to the mouse device.
	xdl::IPXdevLMouse mouse = xdl::createModule<xdl::IPXdevLMouse>(core, xdl::XdevLModuleName("XdevLMouse"), xdl::XdevLID("MyMouse"));
	if(nullptr == mouse)
		return xdl::ERR_ERROR;

	// Create the joystick module to get access to the joystick device.
	xdl::IPXdevLJoystick joystick = xdl::createModule<xdl::IPXdevLJoystick>(core, xdl::XdevLModuleName("XdevLJoystick"), xdl::XdevLID("MyJoystick"));
	if(nullptr == joystick)
		return xdl::ERR_ERROR;

	//
	// Attach all input modules to the window.
	//
	if(keyboard->attach(window) == xdl::ERR_ERROR) {
		return xdl::ERR_ERROR;
	}

	if(mouse->attach(window)== xdl::ERR_ERROR) {
		return xdl::ERR_ERROR;
	}

#ifdef USE_DELEGATES

	MyClassThatHandlesInput instanceOfMyClass(keyboard, mouse, joystick);

#else

	if(keyboard->getButton(xdl::KEY_M, &m) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}
	if(keyboard->getButton(xdl::KEY_P, &p) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}
	if(keyboard->getButton(xdl::KEY_V, &v) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	//
	// Assign mouse buttons clicks to the XdevLButton objects.
	//
	std::vector<xdl::IPXdevLButton> m_buttons;
	m_buttons.reserve(joystick->getNumButtons() + mouse->getNumButtons());
	m_buttons.resize(joystick->getNumButtons() + mouse->getNumButtons());

	xdl::xdl_int idx = 0;
	for(; idx < mouse->getNumButtons(); idx++) {
		if(mouse->getButton(idx, &m_buttons[idx]) == xdl::ERR_ERROR) {
			return -1;
		}
	}
	for(xdl::xdl_int idx2 = 0; idx2 < joystick->getNumButtons(); idx2++) {
		if(joystick->getButton(idx2, &m_buttons[mouse->getNumButtons() + idx2]) == xdl::ERR_ERROR) {
			return -1;
		}
	}

	//
	// Assign mouse movement to the XdevLAxis objects.
	//
	xdl::XdevLAxis* mouse_x = nullptr;
	mouse->getAxis(xdl::AXIS_X, &mouse_x);

	xdl::XdevLAxis* mouse_y = nullptr;
	mouse->getAxis(xdl::AXIS_Y, &mouse_y);

	xdl::XdevLAxis* joystick_x = nullptr;
	joystick->getAxis(xdl::AXIS_0, &joystick_x);

	xdl::XdevLAxis* joystick_y = nullptr;
	joystick->getAxis(xdl::AXIS_1, &joystick_y);

	//
	// Assign pov from the joystick to XdevLPOV
	//
	xdl::XdevLPOV* joystick_pov = nullptr;
	joystick->getPOV(xdl::POV_0, &joystick_pov);

	xdl::xdl_bool mode 	= xdl::xdl_true;
	xdl::xdl_bool pov 	= xdl::xdl_false;
#endif

	//
	// Assign key to the XdevLButton objects.
	//
	if(keyboard->getButton(xdl::KEY_ESCAPE, &escape) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}
	
	window->show();

	//
	// Start main loop
	//
	while(run) {

		core->update();

		if(escape->getClicked())
			break;

#ifndef USE_DELEGATES

		xdl::xdl_int idx = 0;
		for(auto& button : m_buttons) {
			if(button->getClicked()) {
				std::cout << "BUTTON_" << idx << " clicked" << std::endl;
			}
			idx++;
		}

		if(m->getClicked()) {
			mode = !mode;
			std::cout << "Switched mode.\n";
		}
		if(v->getClicked()) {
			verbose = !verbose;
		}
		if(p->getClicked()) {
			pov = !pov;
		}

		if(verbose) {
			if(mode) {
				std::cout << "x:  " << mouse_x->getValue()
				          << ", y:  " << mouse_y->getValue()
				          << "\n";
			} else {
				std::cout << "jx: " << joystick_x->getValue()
				          << ", jy: " << joystick_y->getValue()
				          << "\n";
			}
		}

		if(pov) {
			std::cout << "direction: " << joystick_pov->getDirection()
			          << "\n";
		}
#endif
		xdl::sleep(0.001);
	}

	xdl::destroyCore(core);

	return 0;
}
