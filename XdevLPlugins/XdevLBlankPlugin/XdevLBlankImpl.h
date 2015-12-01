/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2008 Cengiz Terzibas

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

#ifndef XDEVL_BLANK_PLUGIN_H
#define XDEVL_BLANK_PLUGIN_H

#include <XdevLBlank.h>
#include <XdevLPluginImpl.h>

namespace xdl {


	// Holds the plugin major version.
	const xdl_uint XdevLBlankPluginMajorVersion = XDEVLBLANK_MAJOR_VERSION;

	// Holds the plugin minor version.
	const xdl_uint XdevLBlankPluginMinorVersion = XDEVLBLANK_MINOR_VERSION;

	// Holds the plugin patch version.
	const xdl_uint XdevLBlankPluginPatchVersion = XDEVLBLANK_PATCH_VERSION;


	// Holds the Major version number.
	const xdl_uint XdevLBlankMajorVersion = XDEVLBLANK_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLBlankMinorVersion = XDEVLBLANK_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLBlankPatchVersion = XDEVLBLANK_MODULE_PATCH_VERSION;


	static const XdevLString vendor {
		"www.codeposer.net"
	};
	static const XdevLString author {
		"Cengiz Terzibas"
	};
	static const XdevLString copyright {
		"(c) 2005 - 2015 Cengiz Terzibas."
	};
	static const XdevLString pluginName {
		"XdevLBlank"
	};
	static const XdevLString moduleDescription {
		"This is an empty module"
	};

	static std::vector<XdevLModuleName>	moduleNames {
		XdevLModuleName("XdevLBlank"),
	};


	class XdevLBlankModuleImpl : public XdevLModuleImpl<XdevLBlankModule> {
		public:
			XdevLBlankModuleImpl(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLBlankModuleImpl();


	};



}

#endif
