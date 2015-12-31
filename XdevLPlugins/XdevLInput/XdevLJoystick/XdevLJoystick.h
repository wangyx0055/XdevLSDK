/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2014 Cengiz Terzibas

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

#ifndef XDEVL_JOYSTICK_H
#define XDEVL_JOYSTICK_H

#include <XdevLModule.h>
#include <XdevLInputSystem.h>

namespace xdl {

	class XdevLWindow;

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

	/**
		@class XdevLJoystickDeviceInfo
		@brief Structure that holds joystick information for the joystick server.
		@author Cengiz Terzibas
	*/
	struct XdevLJoystickDeviceInfo {
		XdevLJoystickDeviceInfo() :
			name(""),
			joystickid(0),
			numberOfAxes(0),
			numberOfButtons(0) {}

		/// Name of the joystick.
		XdevLString name;

		/// Identification code for the joystick that is used when send joystick events.
		xdl_uint16 joystickid;

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
			virtual XdevLJoystickDeviceInfo getJoystickInfo(xdl_uint16 joystickid) = 0;
	};


	typedef XdevLJoystick	IXdevLJoystick;
	typedef XdevLJoystick*	IPXdevLJoystick;
	typedef XdevLJoystickDeviceInfo	IXdevLJoystickDeviceInfo;
	typedef XdevLJoystickDeviceInfo*	IPXdevLJoystickDeviceInfo;
	typedef XdevLJoystickServer	IXdevLJoystickServer;
	typedef XdevLJoystickServer*	IPXdevLJoystickServer;



}


#endif
