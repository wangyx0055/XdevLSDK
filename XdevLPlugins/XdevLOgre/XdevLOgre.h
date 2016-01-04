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

#ifndef XDEVL_OGRE_H
#define XDEVL_OGRE_H

#include <OGRE/Ogre.h>
#include <XdevLModule.h>

namespace xdl {

	class XdevLWindow;

	/**
		@class XdevLOgre
		@brief Core class for OGRE support.
		@author Cengiz Terzibas

		This module integrates OGRE context onto the XdevLWindowDevice. Everything rendered from
		OGRE will be displayed in XdevLWindowDevice. This module is a basic module to support
		OGRE. Next releases will support more functionality.
		It is really important to attach this module to a XdevLWindowDevice. Otherwise it will not
		work correctly or will crash. Furthermore this module provides a basic resource parse management.
		Check @ref xdevl_xdevlogre_xml_format_resources section how you can add resource information to the
		OGRE resource manager.
		@note This module writes Ogre log information into a file. The filename depends on the id you used to
		create this module. Let's say you used "MyOgre" for the id. The OGRE log filename whould be then
		MyOgre.log. It's an ASCII file. If you have problems check this file to find out whats going on in OGRE.
		@section xdevl_xdevlogre_xml_create Creating the module

		- plugin_name = @b "XdevLOgre"
		- module_name = @b "XdevLOgre"

		@section xdevl_xdevlogre_xml_format Xml Format.

		@subsection xdevl_xdevlogre_xml_format_element "<XdevLOgre>" element
			- Attributes:
				- id
					- desc				: Identification code of the module.
					- value type	: string
				- vsync
					- desc				: Switch VSYNC on/off.
					- value type  : int
					- value range : 0 or 1
					- default     : 0
				- fsaa
					- desc        : Switch fullscreen antialiasing on/off.
					- value type  : int
					- value range : 0 or 1
					- default     : 0
				- gamma
					- desc        : Gamma correction on/off.
					- value type  : int
					- value range : 0 or 1
					- default     : 1
				- color_depth
					- desc        : Depth of the color buffer.
					- value type  : int
					- value range : 16, 24 or 32
					- default     : 32
				- use_nv_perf_hud
					- desc        : Only supported for dx9. Switch perf_hud on/off.
					- value type  : int
					- value range : 0 or 1
					- default     : 0
				- display_frequency
					- desc        : Sets the display frequence.
					- value type  : int
					- value range : [0, inf] but it makes no sense using display frequencies above the supported frequence of your monitor.
					- default     : Default value from the system.

			-	Elements:
				- Plugins
					- desc        : This section holds which OGRE plug-ins should be loaded. You need at least one rendering plug-ins to
					start this Plug-In correctly.
				- Resources
					- desc        : This sections holds folders and files of resources for the OGRE resource manager.
			- Textfield
				- none

		@subsection xdevl_xdevlogre_xml_format_plugins "<Plugins>" element
			- Attributes:
				-none
			- Elements:
				- Plugin
					- desc        : Holds one specific OGRE plugin name. Will be used to load the specified plugin.
			- Textfield:
				- none

		@subsection xdevl_xdevlogre_xml_format_plugin "<Plugin>" element
			- Attributes:
				- none
			- Elements:
				- none
			- Textfield:
				- desc          : A strings which represents the OGRE plugin filename.

		@subsection xdevl_xdevlogre_xml_format_resources "<Resources>" element
			- Attributes:
				-none
			- Elements:
				- Location
					- desc        : Represents the location of an resource
			- Textfield:
				- none

		@subsection xdevl_xdevlogre_xml_format_location "<Location>" element
		You can add more than just one element into the "<Location>" element.
			- Attributes:
				- group         : The name of the group.
			- Elements:
				- Filesystem
					- desc        : Represents a folder.
				- Zip
					- desc        : Represents a zip file.
			- Textfield:
				- none

		@subsection xdevl_xdevlogre_xml_format_filesystem "<Filesystem>" element
			- Attributes:
				- none
			- Elements:
				- none
			- Textfield:
				- Name of the folder.

		@subsection xdevl_xdevlogre_xml_format_zip "<Zip>" element
			- Attributes:
				- none
			- Elements:
				- none
			- Textfield:
				- Name of the zip file.

	*/
	class XdevLOgre : public XdevLModule {
		public:
			virtual ~XdevLOgre() {};
			/// Returns a pointer to the OGRE root object.
			/**
				@return
					- A valid pointer to the Ogre::Root object.
					- NULL if it fails.
			*/
			virtual Ogre::Root* getRoot() = 0;
			/// Returns a pointer to the OGRE render window.
			/**
				@return
					- Returns A valid pointer to the Ogre::RenderWindow object.
					- NULL if it fails.
			*/
			virtual Ogre::RenderWindow* getWindow() = 0;
			/// Attach the OGRE context to a specific window.
			/**
				You must attach this module to a XdevLWindowDevice. Otherwise it will not work
				correctly.
				@param window A valid pointer to an XdevLWindowDevice.
				@return
					- @b ERR_OK if the attachement was successful.
					- @b ERR_ERROR if it fails.
			*/
			virtual xdl_int attach(XdevLWindow* window) = 0;
			/// Updates all rendering and event states in OGRE.
			/**
				It is recommend to use OGRE API to render a frame, for example using
				renderOneFrame().
				@return
					- @b ERR_OK if the update was successful.
					- @b ERR_ERROR if it fails.
			*/
			virtual xdl_int update() = 0;
	};


	typedef XdevLOgre 	IXdevLOgre;
	typedef XdevLOgre* 	IPXdevLOgre;


}

#endif
