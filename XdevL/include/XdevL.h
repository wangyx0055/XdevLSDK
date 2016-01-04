/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2016 Cengiz Terzibas

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

#ifndef XDEVL_H
#define XDEVL_H

/**
	@file XdevL.h
	@brief This file contains the main core create functions.
*/

#include <XdevLTypes.h>
#include <XdevLCore.h>
#include <XdevLListener.h>
#include <XdevLModule.h>
#include <XdevLPlugin.h>
#include <XdevLUtils.h>
#include <XdevLInputSystem.h>
#include <XdevLTimer.h>
#include <XdevLXstring.h>
#include <XdevLXfstring.h>
#include <XdevLError.h>

namespace xdl {

	class XdevLUserData;

	extern XdevLModule* createModule(const XdevLPluginDescriptor& pluginDescriptor, const xdl::XdevLModuleDescriptor& moduleDescriptor);

	/// Plugs a new Plugin to the specified core.
	inline xdl_int plug(XdevLCore* core, const XdevLPluginName& pluginName, const XdevLVersion& version = XdevLVersion()) {
		return core->plug(pluginName, version);
	}
	
	/// Unplug a plugin from the specified core.
	inline xdl_int unplug(XdevLCore* core, const XdevLPluginName& pluginName) {
		return core->unplug(pluginName);
	}

	/// Check if the specified module is valid.
	inline xdl_bool isModuleValid(xdl::XdevLModule* module) {
		return (module != nullptr) ? xdl_true : xdl_false;
	}

	/// Check if the specified module is valid.
	inline xdl_bool isModuleNotValid(xdl::XdevLModule* module) {
		return (module == nullptr) ? xdl_true : xdl_false;
	}

	/**
		@fn template<typename T> T getModule(XdevLCore* core, const XdevLID& id)
		@brief Returns a existing module from the XdevL core system.

		@param core A valid pointer to the XdevLCore object.
		@param id A null terminated string which identifies the created module.
		@return
		- Returns a valid pointer for the interface T.
		- NULL if an error occurs.
	*/
	template<typename T>
	T getModule(XdevLCore* core, const XdevLID& id) {
		return static_cast<T>(core->getModule(id));
	}

	/**
		@fn template<typename T> T createModule(XdevLCore* core,
	               const XdevLModuleName& moduleName,
	               const XdevLID& id,
	               const XdevLPluginName& pluginName = XdevLPluginName("Not Specified"),
	               XdevLUserData* data = NULL)
		@brief Creates a module which is in a plugin.
		@param core A valid pointer to the XdevLCore object.
		@param moduleName A null terminated string which represents the module name.
		@param id A null terminated string which will be used to identify the module in the XdevL core system.
		@param pluginName A null terminated string which is the name of the plugin.
		@param data Not used at the moment.
		@return
		- Returns a valid pointer for the interface T.
		- NULL if an error occurs.

	*/
	template<typename T>
	T createModule(XdevLCore* core,
	               const XdevLModuleName& moduleName,
	               const XdevLID& id,
	               const XdevLPluginName& pluginName = XdevLPluginName("Not Specified"),
	               XdevLUserData* data = NULL) {
		return static_cast<T>(core->createModule(moduleName, id, pluginName, data));
	}

	/**
	 * @brief Create a module without the XdevLCore system.
	 * @param pluginDescriptor The descriptor of the plugin.
	 * @param moduleDescriptor The descriptor of the module./
	 * @return A valid instance of the module if successful, nullptr if it fails.
	 */
	template<typename T>
	T createModule(const XdevLPluginDescriptor& pluginDescriptor, const xdl::XdevLModuleDescriptor& moduleDescriptor) {
		return static_cast<T>(createModule(pluginDescriptor, moduleDescriptor));
	}

	/// Removes a module from the core system.
	/**
		@fn template<typename T> void destroyModule(XdevLCore* core, T* module)
		@brief Deletes a module from the Core system.

		@param core A valid pointer to the XdevLCore object.
		@param id The id of the module that should be deleted. You can get the XdevLModuleId
		for a module if you use the XdevLModule::getId() fuction.
		@return
		- @b ERR_MODULE_NOT_FOUND
		The specified module doesn't exists int the system.
		- @b ERR_OK
		If it was successful.
	*/
	template<typename T>
	xdl_int destroyModule(XdevLCore* core, T* module) {
		return core->deleteModule(module->getID());
	}


	/**
		@fn xdl_int createCore(XdevLCore** core,
	                   xdl_int argc = 0,
	                   xdl_char* argv[] = NULL,
	                   const XdevLFileName& xmlFilename = XdevLFileName(),
	                   XdevLUserData* userDataList[] = NULL,
	                   xdl_uint numberOfUserData = 0)
		@brief Creates the main XdevL core system.

		@param core A valid pointer to the XdevLCore object.
		@param argc The C/C++ main argc parameter. Use 0 if you don't pass arguments.
		@param argv The C/C++ the main argv parameter. Use NULL if you don't pass arguments.
		@param xmlFilename A null terminated string which points to the main xml file.
		@param userDataList Is not used at the moment.
		@param numberOfUserData Number of Userdata in the list.
		@return
			- ERR_OK if it was successful.
			- or xdl::eXdevLErrorCodes if a failure occured.
	*/
	xdl_int createCore(XdevLCore** core,
	                   xdl_int argc = 0,
	                   xdl_char* argv[] = NULL,
	                   const XdevLFileName& xmlFilename = XdevLFileName(),
	                   XdevLUserData* userDataList[] = NULL,
	                   xdl_uint numberOfUserData = 0);


	/**
		@fn xdl_int destroyCore(XdevLCore* core)
		@brief Destroys the XdevLCore system.

		@param core A valid pointer to the XdevLCore object.
		@return
			- ERR_OK if it was successful.
			- or xdl::eXdevLErrorCodes if a failure occurred.
	*/
	xdl_int destroyCore(XdevLCore* core);


}


#endif
