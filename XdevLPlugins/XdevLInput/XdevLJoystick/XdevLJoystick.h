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

#ifndef XDEVL_JOYSTICK_H
#define XDEVL_JOYSTICK_H

#include <XdevLModule.h>
#include <XdevLInputSystem.h>

namespace xdl {

	class XdevLJoystick;
	class XdevLJoystickDeviceInfo;
	class XdevLJoystickServer;

	typedef XdevLJoystick	IXdevLJoystick;
	typedef XdevLJoystick*	IPXdevLJoystick;

	typedef XdevLJoystickDeviceInfo	IXdevLJoystickDeviceInfo;
	typedef XdevLJoystickDeviceInfo*	IPXdevLJoystickDeviceInfo;

	typedef XdevLJoystickServer	IXdevLJoystickServer;
	typedef XdevLJoystickServer*	IPXdevLJoystickServer;

	/**
		@enum XdevLJoystickId
		@brief Joystick device IDs
		@author Cengiz Terzibas
	*/
	enum class XdevLJoystickId {
	  JOYSTICK_0 = 0,
	  JOYSTICK_DEFAULT = 0,
	  JOYSTICK_1,
	  JOYSTICK_2,
	  JOYSTICK_3,
	  JOYSTICK_4,
	  JOYSTICK_5,
	  JOYSTICK_6,
	  JOYSTICK_7,
	  JOYSTICK_8,
	  JOYSTICK_9,
	  JOYSTICK_10,
	  JOYSTICK_11,
	  JOYSTICK_12,
	  JOYSTICK_13,
	  JOYSTICK_14,
	  JOYSTICK_15,
	  JOYSTICK_UNKNOWN
	};

	/**
		@enum XdevLGameControllerId
		@brief Game Controller Id's
		@author Cengiz Terzibas
	*/
	enum class XdevLGameControllerId {
	  CONTROLLER_X,
	  CONTROLLER_Y,
	  CONTROLLER_A,
	  CONTROLLER_B,
	  CONTROLLER_START,
	  CONTROLLER_BACK,
	  CONTROLLER_DPAD_UP,
	  CONTROLLER_DPAD_DOWN,
	  CONTROLLER_DPAD_LEFT,
	  CONTROLLER_DPAD_RIGHT,
	  CONTROLLER_LEFT_SHOULDER,
	  CONTROLLER_RIGHT_SHOULDER,
	  CONTROLLER_LEFT_STICK,
	  CONTROLLER_RIGHT_STICK
	};

	/**
		@class XdevLJoystickDeviceInfo
		@brief Structure that holds joystick information for the joystick server.
		@author Cengiz Terzibas
	*/
	class XdevLJoystickDeviceInfo {
		public:
			XdevLJoystickDeviceInfo() :
				name(""),
				joystickid(XdevLJoystickId::JOYSTICK_DEFAULT),
				numberOfAxes(0),
				numberOfButtons(0) {}

			const XdevLString& getName() const {
				return name;
			}

			XdevLJoystickId getJoystickId() const {
				return joystickid;
			}

			xdl_uint8 getNumberOfAxes() const {
				return numberOfAxes;
			}

			xdl_uint8 getNumberOfButtons() const {
				return numberOfButtons;
			}

			/// Name of the joystick.
			XdevLString name;

			/// Identification code for the joystick that is used when send joystick events.
			XdevLJoystickId joystickid;

			/// Number of axes supported by this joystick device.
			xdl_uint8 numberOfAxes;

			/// Number of buttons supported by this joystick device.
			xdl_uint8 numberOfButtons;
	};



	/**
		@class XdevLJoystickServer
		@brief Interface that manages joysticks
		@author Cengiz Terzibas
	*/
	class XdevLJoystickServer : public XdevLModule {
		public:
			virtual ~XdevLJoystickServer() {}

			/// Returns the number of joystick devices.
			virtual xdl_uint getNumJoysticks() = 0;

			/**
			 * @brief Returns information of the specified joystick device.
			 * @param joystickid
			 * @return If successful it will return a XdevLJoystickDeviceInfo filled with a valid name.
			 * Otherwise the name field will be empty.
			 */
			virtual xdl_int getJoystickInfo(const XdevLJoystickId& joystickid, XdevLJoystickDeviceInfo& joystickDeviceInfo) = 0;
	};


	/**
		@class XdevLJoystick
		@brief Interface class for joystick devices.
		@author Cengiz Terzibas

		@section xdevl_joystick_xml Plugin & Module information

		The plugin for this module is a built in plugin in @b XdevL @b 0.1 and that means that you don't have to
		plug this plugin manually because it's pluged already :).

		@section xdevl_joystick_xml_create Creating the module

		Here I show you how you can create a Module (object from that pluged plugin). Use the xdl::createModule()
		function in your application and use the following information:

		- plugin_name = @b "XdevLJoystick"
		- module_name = @b "XdevLJoystick"

		to create the module. The plugin name has no relation to the file name of the plugin. It's an internal
		literal identification code.
		Here an example how we create an valid XdevLJoystick object:

		@code
		XdevLJoystick* joystick = xdl::createModule<xdl::XdevLJoystick*>(	core,
																			plugin_name,
																			module_name,
																			"MyJoystick");
		@endcode

		The last parameter @e "MyJoystick" is the identification code for that created object. You could not use the
		same name for a different module. Internaly the system will change this literal into an unique integer value.
		This value will be used for identifying messages and requests to that module.

		Here another method to create a valid XdevLJoystick object. You can instruct XdevLCore to create
		the module via the XML file. Add the "<Modules>" section (if not added already) into the main core XML
		file. To instruct that XdevLCore should create a valid object of that module add the "<Module>" element.
		The "<Modules>" section of the XML file should look like this:

		@code
		<Modules>
	    <Module id="MyJoystick" plugin_name="XdevLJoystick" module_name="XdevLJoystick"> </Module>
	  </Modules>
		@endcode

		In your application use the getModule() function with the specified 'id' parameter in your applicaton
		to get the created object.

		@code
		XdevLJoystick* joystick = xdl::getModule<xdl::XdevLJoystick*>(core, "MyJoystick");
		@endcode

		@section xdevl_joystick_xml_element Xml Format.

		@subsection xdevl_xml_joystick_device "<XdevLJoystick>" Block
			- Attributes:
				- id = identification code
			-	Elements:
				- "<Range>"
				- "<ClickResponseTime>"

		@subsection xdevl_xml_joystick_device_range "<Range>" Block
			- Attributes:
				- axis : Specifies the axis.
				- min  : The minimum value for the axis.
				- max  : The maximum value for the axis.
			- Elements:
				- none

		The attribute @e axis specifies the axis number which should be set.
		@subsection xdevl_xml_joystick_device_crt "<ClickResponseTime>" Block
			- Attributes:
				- value

		This is the clicked response time (crt) block. The @b value attribute has the unit @e seconds.
		You can set the @e crt manually with the setClickResponseTime() function.

		Here an example:
		@code
	  <XdevLJoystick id="MyJoystick">
	    <ClickResponseTime value="0.5"></ClickResponseTime>
	  </XdevLJoystick>
		@endcode
		We change the click response time (crt) for the joystick device '0' to 500 @e ms.

		@section xdevl_joystick_button_axis_id Supported identification codes
		- xdl::eXdevLJoyButton joystick button id's.
		- xdl::eXdevLJoyAxis joystick axis id's.
		- xdl::eXdevLJoyPOV joystick POV id's.

	*/
	class XdevLJoystick : public XdevLInputSystem, public XdevLModule {
		public:
			virtual ~XdevLJoystick() {};

			/// Create a connection to a specific joystick device.
			virtual xdl_int create(IPXdevLJoystickServer joystickServer, const XdevLJoystickId& joystickId = XdevLJoystickId::JOYSTICK_DEFAULT) = 0;

			/// Change joystick device.
			virtual xdl_int useJoystick(const XdevLJoystickId& joystickId) = 0;

			/// Returns the state of a button.
			/**
				@return Returns @e 'true' as long as the specified button is pressed. Otherwise it
				will return @e 'false'.
			*/
			virtual xdl_bool getPressed(const xdl_uint button) = 0;
			/// Returns clicked state of the specified button.
			/**
				The default click response time (crt) is 300ms. To change the crt use the setClickResponseTime()
				function.

				@return Returns @e 'true' only once after the specified mouse button was pressed and released
				within the click response time (crt).
			*/
			virtual xdl_bool getClicked(const xdl_uint button) = 0;

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
	};

	typedef XdevLJoystick	IXdevLJoystick;
	typedef XdevLJoystick* IPXdevLJoystick;

	XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DECLARATION(XdevLJoystick)
}


#endif
