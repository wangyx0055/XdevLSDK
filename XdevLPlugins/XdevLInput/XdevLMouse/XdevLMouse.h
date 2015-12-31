/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2012 Cengiz Terzibas

	This library is free software; you can redistribute it and/or modify it under the
	terms of the GNU Lesser General Public License as published by the Free Software
	Foundation; either version 2.1 of the License, or (at your option) any later version.
	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License along with
	this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
	Suite 330, Boston, MA 02111-1307 USA

	I would appreciate if you report all bugs to: cengiz@terzibas.de
*/

#ifndef XDEVL_MOUSE_H
#define XDEVL_MOUSE_H

#include <XdevLModule.h>
#include <XdevLInputSystem.h>

namespace xdl {

	class XdevLWindow;

	/**
		@class XdevLMouse
		@brief Interface class for handling mouse events.


		@section xdevl_mouse_xml Plugin & Module information

		Here the feature list of this module:

		- Gives easy access to mouse button pressed/released or clicked events.
		- Assign a delegate that handles all mouse press/release events.
		- Assign a delegate that handles a specific button press/release event.
		- Assign a delegate that handles all mouse motion events.
		- Assign a delegate that handles one specific axis motion event.
		- Change click response time (crt) for click events.
		- Gives easy access to mouse axis values.
		- Change axis range values.

		It is expected that the necessary plugin is already plugged into the core system to do all the steps in the 
		following sections. Let's start with creating the module.

		@section xdevl_mouse_xml_create Creating the module manually

		Here I show you how you can create a module using this plugin. Use the xdl::createModule()
		function to create the module using the following information:

		- Module Name = @b "XdevLMouse"

		Keep in mind that the plugin name has no relation to the filename of that plugin. It's an internal
		literal identification code. Here an example how we create a valid XdevLMouse object:

		@code
		// Create the mouse module to get access to the mouse device.
		xdl::IPXdevLMouse mouse = xdl::createModule<xdl::IPXdevLMouse>(core, xdl::XdevLModuleName("XdevLMouse"), xdl::XdevLID("MyMouse"));
		if(nullptr == mouse) {
			return xdl::ERR_ERROR
		}
		@endcode

		The last parameter @e "MyMouse" is the identification code for the module that will be managed withing the core. You can not 
		use the same identification name for a another module. Internally the system will change this literal into an unique
		integer value that corresponds to the string.

		@section xdevl_mouse_xml_create_via_xml Creating the module via XML file

		Here is an alternative method to create the module. You can instruct XdevLCore to create the module during start up time
		using the main XML file. 
		To do that add the "<XdevLModules>" section (if not added already) into the main core XML file. To instruct XdevLCore 
		to create a valid object of that module add the "<XdevLModule>" element into the "<XdevLModules>" section. The "<Modules>" 
		section of the XML file could look like this:

		@code
		<!-- Create necessary modules -->
		<XdevLModules>
			<XdevLModule id="MyMouse"		name="XdevLMouse"/>
		</XdevLModules>
		@endcode

		Now you can use the getModule() function with the specified 'id' parameter in your application to get the
		object of the created module.

		@code
			// Get the instance to the mouse module.
			xdl::IPXdevLMouse mouse = xdl::getModule<xdl::IPXdevLMouse>(core, xdl::XdevLID("MyMouse"));
			if(nullptr == mouse) {
				return xdl::ERR_ERROR;
			}
		@endcode

		@section xdevl_mouse_usage Usage
		
		@subsection xdevl_mouse_usage_easy Easy access.
		The easiest way to check if a button is press/released/clicked use the getPressed() and getClicked() method.
		To get easy access to the values of the mouse axis use the getValue() method.
		
		@code
			if(mouse->getPressed(xdl::BUTTON_0) == xdl::xdl_true) {
				// Mouse button 0 is pressed.
			}
			
			if(mouse->getClicked(xdl::BUTTON_0) == xdl::xdl_true) {
				// Mouse button 0 is clicked.
			}
			
			xdl_float value = mouse->getValue(xdl::AXIS_0);
		@endcode

		@subsection xdevl_mouse_usage_object_oriented Using the XdevLButton and/or XdevLAxis interface.
		Another way got get press/click/motion events is to assign buttons and axis to the XdevLButton and XdevLAxis objects. 
		That gives you more flexibility how ot interpret button press/click etc. In the @ref xdevl_keyboard_usage you can assign 
		keyboard key to a XdevLButton object. Doing that it does not matter anymore if a press/click is done by a button/key on a
		mouse, keyboard or joystick etc.
		
		Here an example:
		
		@code
			// Get the left mouse button and assign it to the fireFireworks object;
			xdl::IPXdevLButton fireFireworks = nullptr;
			mouse->getButton(xdl::BUTTON_LEFT, &fireFireworks);
			
			
			if(fireFireworks->getPressed) == xdl::xdl_true) {
				// Mouse BUTTON_LEFT is pressed.
			}
			
			if(fireFireworks->getClicked) == xdl::xdl_true) {
				// Mouse BUTTON_LEFT is clicked.
			}
			
		@endcode

		As you can see, it is not important which button is assigned to an event that shall for example fire
		an firework. We could for example assign a keyboard or a joystick button to the fireFireworks object.
		This way we are more flexible which device like mouse, keyboard or joystick should cause to fire the
		fireworks :D.

		@subsection xdevl_mouse_usage_delegates Using delegates
		
		Another cool way of handling events is using delegates. We can assign a member function of a class that must have
		a defined return value and parameters value to a delegate, register it to the module and every time an event occurs 
		the delegate will be used to trigger the specified member function. Here we go:

		@code
		// This is one of our "Game" class.
		class MySuperActor {
			void fireFireworksHandler(const XdevLButtonState& state) {
				if(state == xdl::BUTTON_PRESSED) {
					// Fire the fireworks.
				} else {
					// Well, do something else :D.
				}
			}
		};
		
		MySuperActor mySuperActorInstance;
		
		// First create XdevLButtonIdDelegate object and assign the member function. 
		xdl::XdevLButtonIdDelegateType fireFireworks = xdl::XdevLButtonIdDelegateType::Create<MySuperActor, &MySuperActor::fireFireworksHandler>(&mySuperActorInstance);
		
		// And now we assign a specific button that shall trigger our member function when the button is pressed/release/clicked.
		mouse->registerDelegate(xdl::XdevLString("BUTTON_0"), fireFireworks);
		@endcode

		Now everytime the mouse button xdl::BUTTON_0 is pressed/released the delegate will be used to call the MySuperActor member function fireFireworksHandler.
		I think from here you can imagine where this goes ;).
		The above example was just showing how to assign a specific button to a delegate. 

		If we want handle all events from all buttons using delegates we do this.
		
		@code
		class MySuperActor {
			void fireFireworksHandler(const xdl::XdevLButtonId& button, const xdl::XdevLButtonState& state) {
				switch(button) {
					case xdl::BUTTON_0: {
						if(state == xdl::BUTTON_PRESSED) {
							// Fire the fireworks.
						} else {
							// Well, do something else :D.
						}
					}break;
					case xdl::BUTTON_1: {
					}break;
				}
			}
		};
		
		MySuperActor mySuperActorInstance;
		
		xdl::XdevLButtonDelegateType fireFireworks = xdl::XdevLButtonDelegateType::Create<MySuperActor, &MySuperActor::fireFireworksHandler>(&mySuperActorInstance);
		
		// And now we register the delegate to the module.
		mouse->registerDelegate(fireFireworks);

		@endcode

		Now we will receive all buttons events in one member function.
		

		@section xdevl_mouse_xml_element XML Format.
		You can change some properties of the mouse device via the XML file too. In this section the elements and
		attributes are described which can be changed.
		@subsection xdevl_mouse_xml_mouse_device "<XdevLMouse>" Block
			- Attributes:
				- id = identification code
			-	Elements:
				- "<Range>"
				- "<ClickResponseTime>"
		The @b id attribute is important because only the properties of the module with the valid id
		will be changed. For example in the case above we used @b "MyMouse" as the identification code.
		If we write this into the @b id attribute we will change the properties of that module. If you
		use a different id which doesn't exist nothing will be changed.

		@subsection xdevl_mouse_xml_mouse_device_range "<Range>" Block
			- Attributes:
				- axis : Specifies the axis.
				- min  : The minimum value for the axis.
				- max  : The maximum value for the axis.
			- Elements:
				- none

		The attribute @e axis specifies the axis number which should be set.

		@subsection xdevl_mouse_xml_mouse_device_crt "<ClickResponseTime>" Block
			- Attributes:
				- button : Specifies the button.
				- value  : The crt value for the specified button in seconds.

		This is the clicked response time block. The @b value attribute has the unit @e seconds.

		Here an example which changes the range and click response time of the mouse module "MyMouse":

		@code
	  <XdevLMouse id="MyMouse">
			<Range axis="AXIS_0" min="-1.0" max="1.0"></Range>
	    <ClickResponseTime button="BUTTON_0" value="0.1"></ClickResponseTime>
	  </XdevLMouse>
		@endcode

		@author Cengiz Terzibas
	*/
	class XdevLMouse : public XdevLInputSystem, public XdevLModule {
		public:
			virtual ~XdevLMouse() {};
			
			/// Attach the mouse device to a window.
			/**
				This module must be attached to a window otherwise it will assert.

				@param window A valid IPXdevLWindow object.
				@return Returns ERR_OK if the attachment was successful otherwise ERR_ERROR.
			*/

			virtual xdl_int attach(XdevLWindow* window) = 0;

			/// Returns the state of a button/key.
			/**
				@return Returns 
				- @b xdl_true As long as the specified button/key is pressed.
				- @b xdl_false Else.
			*/
			virtual xdl_bool getPressed(const xdl_uint buttonToCheck) = 0;

			/// Returns clicked state of the specified button.
			/**
				The default click response time (crt) is 300ms. To change the crt use the setClickResponseTime()
				function.

				@return Returns 
				@b xdl_true Only after the specified button/key was pressed and released within the click response time (crt).
				@b xdl_false Else.
			*/
			virtual xdl_bool getClicked(const xdl_uint buttonToCheck) = 0;

			/// Returns the current axis value.
			/**
				The value of the axis lies between [min, max] and is a float value. The resolution of the steps in this
				range is vendor specified and is not defined. Please check the technical specification of your device.
				Here a diagram.
				@code
					 min        max
						o----------]
				@endcode

				@return Returns the current axis position.
			*/
			virtual xdl_float getValue(const xdl_uint axis) = 0;


			/// Sets the relative mode.
			/**
				This mode will move the mouse pointer always to the center of the window
				if one of the window's border are reached. Use this to get a infinite
				mouse delta movement.
			*/
			virtual void setRelativeMode(xdl_bool state) = 0;
	};


	typedef XdevLMouse	IXdevLMouse;
	typedef XdevLMouse*	IPXdevLMouse;
}

#endif
