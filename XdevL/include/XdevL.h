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

#include <map>

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
		- nullptr if an error occurs.
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
	               XdevLUserData* data = nullptr)
		@brief Creates a module which is in a plugin.
		@param core A valid pointer to the XdevLCore object.
		@param moduleName A null terminated string which represents the module name.
		@param id A null terminated string which will be used to identify the module in the XdevL core system.
		@param pluginName A null terminated string which is the name of the plugin.
		@param data Not used at the moment.
		@return
		- Returns a valid pointer for the interface T.
		- nullptr if an error occurs.

	*/
	template<typename T>
	T createModule(XdevLCore* core,
	               const XdevLModuleName& moduleName,
	               const XdevLID& id,
	               const XdevLPluginName& pluginName = XdevLPluginName("Not Specified"),
	               XdevLUserData* data = nullptr) {
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
	                   xdl_char* argv[] = nullptr,
	                   const XdevLFileName& xmlFilename = XdevLFileName(),
	                   XdevLUserData* userDataList[] = nullptr,
	                   xdl_uint numberOfUserData = 0)
		@brief Creates the main XdevL core system.

		@param core A valid pointer to the XdevLCore object.
		@param argc The C/C++ main argc parameter. Use 0 if you don't pass arguments.
		@param argv The C/C++ the main argv parameter. Use nullptr if you don't pass arguments.
		@param xmlFilename A null terminated string which points to the main xml file.
		@param userDataList Is not used at the moment.
		@param numberOfUserData Number of Userdata in the list.
		@return
			- ERR_OK if it was successful.
			- or xdl::eXdevLErrorCodes if a failure occured.
	*/
	xdl_int createCore(XdevLCore** core,
	                   xdl_int argc = 0,
	                   xdl_char* argv[] = nullptr,
	                   const XdevLFileName& xmlFilename = XdevLFileName(),
	                   XdevLUserData* userDataList[] = nullptr,
	                   xdl_uint numberOfUserData = 0);

	XdevLCore* createCore(xdl_int argc = 0,
	                      xdl_char* argv[] = nullptr,
	                      const XdevLFileName& xmlFilename = XdevLFileName(),
	                      XdevLUserData* userDataList[] = nullptr,
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

#if 1
#define createModuleText(CORE, INTERFACE, ID, MODULE) xdl::createModule<xdl::INTERFACE*>(CORE, xdl::XdevLModuleName(#INTERFACE), xdl::XdevLID(ID))
#else
#define createModuleText(CORE, INTERFACE, ID, MODULE) new xdl::INTERFACE##MODULE
#endif

	typedef xdl::xdl_int(*XdevLCreateModuleFunctionType)(xdl::XdevLModuleCreateParameter* parameter, std::shared_ptr<xdl::XdevLModule>& module);
	typedef xdl::xdl_int(*XdevLDeleteModuleFunctionType)();

	class XdevLModuleNode {
		public:
			XdevLModuleNode() :
				create(nullptr),
				destroy(nullptr) {
			}

			XdevLModuleNode(XdevLCreateModuleFunctionType createFunction) :
				create(createFunction) {
			}

			XdevLModuleNode(XdevLCreateModuleFunctionType createFunction, XdevLDeleteModuleFunctionType destroyFunction) :
				create(createFunction),
				destroy(destroyFunction) {
			}

			XdevLCreateModuleFunctionType create;
			XdevLDeleteModuleFunctionType destroy;
	};

	class XdevLCoreSystem {
		public:
			template<typename T>
			void plug(XdevLCreateModuleFunctionType synapseCreate) {
				m_moduleNodes[typeid(T).hash_code()] = XdevLModuleNode(synapseCreate);
			}

			template<typename T>
			std::shared_ptr<T> create(const xdl::XdevLID& id, std::shared_ptr<xdl::XdevLCore> core = nullptr) {
				auto node = m_moduleNodes.find(typeid(T).hash_code());
				if(node == m_moduleNodes.end()) {
					return nullptr;
				}

				xdl::XdevLModuleCreateParameter parameter;
				parameter.setModuleId(id);
				if(core != nullptr) {
					parameter.setModuleMediator(core.get());
				}

				std::shared_ptr<xdl::XdevLModule> tmp;
				if(node->second.create(&parameter, tmp) != xdl::ERR_OK) {
					return nullptr;
				}

				return std::dynamic_pointer_cast<T>(tmp);
			}
		private:
			std::map<std::size_t, XdevLModuleNode> m_moduleNodes;
	};

}


#endif
