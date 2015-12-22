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

#include "XdevLCoreImpl.h"
#include <XdevLModule.h>
#include <XdevLCommandLine.h>
#include <XdevLListener.h>
#include <XdevLXstring.h>
#include <XdevLXfstring.h>
#include <XdevLTimer.h>
#include <XdevLUtils.h>
#include <XdevLError.h>

#include <fstream>
#include <iostream>
#include <algorithm>
#include <tinyxml.h>
#include <signal.h>

xdl::XdevLModuleDescriptor xdl::XdevLCoreImpl::m_coreDescriptor {
	vendor,
	author,
	moduleName[0],
	copyright,
	description,
	XdevLCoreMajorVersion,
	XdevLCoreMinorVersion,
	XdevLCorePatchVersion
};

xdl::XdevLPluginDescriptor m_corePluginDescriptor {
	xdl::pluginName,
	xdl::moduleName,
	xdl::XdevLCorePluginMajorVersion,
	xdl::XdevLCorePluginMinorVersion,
	xdl::XdevLCorePluginPatchVersion
};

extern "C" XDEVL_EXPORT int _createXdevLCore(xdl::XdevLCore** core, xdl::XdevLCommandLineParser* param) {
	// TODO Do we have to do something if *core==nullptr?
	*core = new xdl::XdevLCoreImpl(param);
	return xdl::ERR_OK;
}

extern "C" XDEVL_EXPORT void _deleteXdevLCore(xdl::XdevLCore* core) {
	if(core)
		delete core;
}

extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor() {
	return &m_corePluginDescriptor;
}

namespace xdl {

	XDEVL_DECLARE_LOG(XdevLCore);

	void logConsumerCallback(const XdevLLogItem& item) {
		std::cout << "Time [us]: " << item.timestamp << ", Module: " << item.additional << " : " << item.message << std::endl;
	}

	XdevLCoreImpl::XdevLCoreImpl(XdevLCommandLineParser* param) :
		m_initialized(xdl_false),
		m_id(XdevLID(getDescriptor().getName().toString().c_str())),
		m_commandLine(param) {

		// Print Core information and copyright.
		XdevLVersion moduleVersion = getDescriptor().getVersion();

		std::cout << "\n\n----------------------------------------------------------\n";
		std::cout << "---       Starting " <<
		          getID().getName() << " System Version "
		          << moduleVersion.geMajor() << "."
		          << moduleVersion.getMinor() << "."
		          << moduleVersion.getPatch() << "      ---\n";
		std::cout << "----------------------------------------------------------\n\n";

		std::cout << copyright << "\n\n";

	}

	XdevLCoreImpl::~XdevLCoreImpl() {
		if(coreInitialized()) {
			shutdown();
		}
	}

	XdevLModuleDescriptor& XdevLCoreImpl::getDescriptor() {
		return m_coreDescriptor;
	}

	xdl_bool  XdevLCoreImpl::coreInitialized() {
		return m_initialized;
	}

	XdevLLog* XdevLCoreImpl::getLogger() {
		return m_logger;
	}

	const XdevLID& XdevLCoreImpl::getID() const {
		return m_id;
	}

	xdl_double XdevLCoreImpl::getTime() {
		return m_timer.getTime();
	}

	xdl_double XdevLCoreImpl::getDT() {
		return m_timer.getDT();
	}

	XdevLTimer& XdevLCoreImpl::getTimer() {
		return m_timer;
	}

	const XdevLFileName& XdevLCoreImpl::getXmlFilename() const {
		return m_XmlFilename;
	}

	xdl_int XdevLCoreImpl::shutdown() {

		std::cout << "\n\n----------------------------------------------------------\n";
		std::cout << "---             Starting shutdown process              ---\n";
		std::cout << "----------------------------------------------------------\n\n";

		if(coreInitialized() == xdl_false) {
			return ERR_ERROR;
		}

		// Delete all modules.
		XDEVL_MODULE_INFO("Removing all modules.\n");

		//
		// First find all modules that depends on another module and put them in a list
		// so that they can be removed first. This version is rather a hack then a real solution.
		// TODO
		std::list<XdevLModule*> tmp;
		for(auto moduleIterator : m_modules) {

			XdevLModule* module = moduleIterator.second->getModuleCreateParameter()->getModuleInstance();
			auto& dependency = module->getDescriptor().getDependencies();

			for(auto& dependencyModule : dependency) {
				XDEVL_MODULE_INFO("Module: " << dependencyModule->getDescriptor().getName() << " depends on Module: " <<  module->getDescriptor().getName() << "\n");
				if(dependencyModule->getDescriptor().getState(XDEVL_MODULE_STATE_DISABLE_AUTO_DESTROY) == xdl_false) {
					auto it = std::find(tmp.begin(), tmp.end(), dependencyModule);
					if(it == tmp.end()) {
						tmp.push_back(dependencyModule);
					}
				}
			}
		}

		//
		// Now remove the modules that depends on other modules.
		//
		for(auto module : tmp) {
			XDEVL_MODULE_INFO("Removing first: " << module->getDescriptor().getName() << "\n");
			deleteModule(module->getID());
		}


		//
		// Now remove all other modules left.
		//
		moduleMap::const_iterator moduleIterator = m_modules.begin();
		while(moduleIterator != m_modules.end()) {
			XdevLModule* module = moduleIterator->second->getModuleCreateParameter()->getModuleInstance();
			auto& dependency = module->getDescriptor().getDependencies();

			if(module->getDescriptor().getState(XDEVL_MODULE_STATE_DISABLE_AUTO_DESTROY) == xdl_false) {
				_deleteModule(module->getID());
				moduleIterator = m_modules.erase(moduleIterator);
			} else {
				moduleIterator++;
			}
		}

		// Go through all plugins and delete them from the system.
		XDEVL_MODULE_INFO("Removing all plugins.\n");
		pluginMap::const_iterator pluginIterator = m_plugins.begin();
		while(pluginIterator != m_plugins.end()) {
			_unplug(pluginIterator->first);
			pluginIterator = m_plugins.erase(pluginIterator);
		}

		m_listener.clear();
		m_modules.clear();
		m_plugins.clear();

		m_initialized = false;

		std::cout << "\n\n----------------------------------------------------------\n";
		std::cout << "----              " << getID().getName() << " System stopped             ---\n";
		std::cout << "----------------------------------------------------------\n\n";


		return ERR_OK;
	}

	xdl_int XdevLCoreImpl::plug(const XdevLPluginName& pluginName, const XdevLVersion& version) {

		std::string path, name, ext;
		std::string pluginNameString = pluginName.toString();
		xstd::trim(pluginNameString);

		// Create the dynamic library loader object.
		XdevLSharedLibrary* modulesSharedLibrary = nullptr;
		try {
			modulesSharedLibrary = new XdevLSharedLibrary();
		} catch(std::bad_alloc& e) {
			XDEVL_MODULE_ERROR(e.what());
			return ERR_ERROR;
		}

		if(version == XdevLVersion::zero) {

			// Just get the path, name and extension of the plugin.
			std::string name_file {pluginNameString};

			// Because Windows can deal with both "/" and "\" folder separator we just convert into
			// the Unix type one.
			xstd::make_unix_file(name_file);

			// Get all file information, path, filename and extension.
			xstd::get_filename_info(name_file, path, name, ext);

			// Fix the extension because version numbers have "." in their names.
			if(ext != "so" && ext != "dll") {
				ext = "";
				name = xstd::get_pathless(name_file);
			}

			// Check if this plugin is already registered.
			if(m_plugins.find(XdevLPluginName(name)) != m_plugins.end()) {
				XDEVL_MODULE_ERROR("Plugin: '" << name << "' is registered already.\n");
				return ERR_ERROR;
			}

			if(ext.empty()) {
#ifdef XDEVL_DEBUG
				name += "d";
#endif
			}

			//
			// Check if the user gave a path information with the filename.
			//
			if(path != xstd::current_path_id) {

				// Yes he did, Ok then load that directly.
				auto tmp = path + "/" + name;

				if(modulesSharedLibrary->open(tmp.c_str()) != 0) {
					delete modulesSharedLibrary;
					XDEVL_MODULE_ERROR("Could not open plugin: '" << tmp << "'.\n");
					exit(-1);
				}

			} else {

				//
				// No the user didn't gave any path information. Ok Check first the local path.
				//
				auto tmp = name + XdevLSharedLibrary::extension.toString();

				std::ifstream fs;
				fs.open(tmp.c_str());
				if(fs.is_open()) {
					fs.close();
					// File found in the local path. Than lets load it.
					if(modulesSharedLibrary->open(tmp.c_str()) != 0) {
						XDEVL_MODULE_ERROR("Could not open plugin: '" << tmp << "'.\n");
						delete modulesSharedLibrary;
						exit(-1);
					}
				} else {

					//
					// Ok, it's not in the local path, so check the XDEVL_PLUGINS path.
					//

					XDEVL_MODULE_INFO("Searching in XDEVL_PLUGINS environment variable folder.\n");
					tmp = m_xdevlPluginPath + "/" + tmp;

					// Make platform dependent fileformat.
					xstd::make_path(tmp);

					fs.open(tmp.c_str());
					if(fs.is_open()) {
						fs.close();
						// File found in the local path. Than lets load it.
						if(modulesSharedLibrary->open(tmp.c_str()) != 0) {
							XDEVL_MODULE_ERROR("Could not open plugin: '" << tmp << "'.\n");
							delete modulesSharedLibrary;
							exit(-1);

						}
					} else {
						XDEVL_MODULE_ERROR("Could not open plugin: '" << tmp << "'.\n");
						delete modulesSharedLibrary	;
						exit(-1);

					} // if(fs.is_open())
				}
			}


		} else {
			//
			// No the user didn't gave any path information. Ok Check first the local path.
			//
			auto tmp = pluginNameString + "-" + version.toString();
#ifdef XDEVL_DEBUG
			tmp += "d";
#endif
			tmp += XdevLSharedLibrary::extension.toString();

			std::ifstream fs;
			fs.open(tmp.c_str());
			if(fs.is_open()) {
				fs.close();
				// File found in the local path. Than lets load it.
				if(modulesSharedLibrary->open(tmp.c_str()) != 0) {
					XDEVL_MODULE_ERROR("Could not open plugin: '" << tmp << "'.\n");
					delete modulesSharedLibrary;
					exit(-1);
				}
			} else {

				//
				// Ok, it's not in the local path, so check the XDEVL_PLUGINS path.
				//

				XDEVL_MODULE_INFO("Searching in XDEVL_PLUGINS environment variable folder.\n");
				tmp = m_xdevlPluginPath + "/" + tmp;

				// Make platform dependent fileformat.
				xstd::make_path(tmp);

				fs.open(tmp.c_str());
				if(fs.is_open()) {
					fs.close();
					// File found in the local path. Than lets load it.
					if(modulesSharedLibrary->open(tmp.c_str()) != 0) {
						XDEVL_MODULE_ERROR("Could not open plugin: '" << tmp << "'.\n");
						delete modulesSharedLibrary;
						exit(-1);

					}
				} else {
					XDEVL_MODULE_ERROR("Could not open plugin: '" << tmp << "'.\n");
					delete modulesSharedLibrary	;
					exit(-1);

				} // if(fs.is_open())
			}
		}

		//
		// Get the plugins basic create, delete and descriptor functions.
		//
		XdevLGetPluginDescriptorFunction 	plugin_descriptor	= (XdevLGetPluginDescriptorFunction)(modulesSharedLibrary->getFunctionAddress("_getDescriptor"));
		XdevLCreateModuleFunction create_module		= (XdevLCreateModuleFunction)(modulesSharedLibrary->getFunctionAddress("_create"));
		XdevLDeleteModuleFunction delete_module		= (XdevLDeleteModuleFunction)(modulesSharedLibrary->getFunctionAddress("_delete"));
		XdevLPluginInitFunction init_plugin = (XdevLPluginInitFunction)(modulesSharedLibrary->getFunctionAddress("_init_plugin"));
		XdevLPluginShutdownFunction shtudown_plugin = (XdevLPluginShutdownFunction)(modulesSharedLibrary->getFunctionAddress("_shutdown_plugin"));


		// Check if we have all necessary module functions from the dynamic library.
		if((plugin_descriptor == nullptr) || (create_module == nullptr) || (delete_module == nullptr)) {
			XDEVL_MODULE_ERROR("Unable to acquire necessary module function from the dynamic library.\n");
			XDEVL_ASSERT(0, "Plugin has not the right functions defined or another problem exists.");
		}

		auto plugininfo 	= new XdevLPluginInfo(init_plugin, shtudown_plugin, create_module, delete_module, plugin_descriptor, modulesSharedLibrary);
		m_plugins.insert(pluginMap::value_type(plugin_descriptor()->getName(), plugininfo));

		// Add all module names into a map. We need that later if the user only
		// specifies a module name to create a module. We use this sort of look up
		// map to get the correct plugin.
		for(int module = 0; module < plugin_descriptor()->getNumModules(); ++module) {
			m_modulesMap[plugin_descriptor()->getModuleName(module).toString()] = plugininfo;
			XDEVL_MODULE_INFO("Found Module: " << plugin_descriptor()->getModuleName(module).toString() << ".\n");
		}

		// Initialize the plugin if supported.
		if(plugininfo->isInitPluginValid()) {
			XDEVL_MODULE_INFO("Initializing plugin: " << plugin_descriptor()->getName() << "\n");
			XdevLPluginCreateParameter parameter(this);

			if(plugininfo->initPlugin(&parameter) != ERR_OK) {
				XDEVL_MODULE_WARNING("Pluing initialisation faild. " << ".\n");
				return shutdown();
			} else {
				XDEVL_MODULE_SUCCESS("Initializing plugin: " << plugin_descriptor()->getName() << " was succesful.\n");

			}
		}

		XdevLVersion pluginVersion = plugin_descriptor()->getVersion();
		XDEVL_MODULE_SUCCESS("Plugin: name='" << plugin_descriptor()->getName() << "' " << pluginVersion << " loaded.\n");
		return ERR_OK;
	}


	//
	// Unplug Plugin
	//
	xdl_int XdevLCoreImpl::unplug(const XdevLPluginName& pluginName) {

		auto i = _unplug(pluginName);
		if(i == m_plugins.end()) {
			return ERR_ERROR;
		}

		// Remove it from the plugin list.
		m_plugins.erase(i);

		return ERR_OK;
	}

	pluginMap::iterator XdevLCoreImpl::_unplug(const XdevLPluginName& pluginName) {
		XDEVL_MODULE_INFO("Unplug plugin: '" << pluginName << "'" << std::endl);

		auto name_file(pluginName.toString());
		xstd::make_unix_file(name_file);

		// Check if the specified plugin exists.
		auto i = m_plugins.find(XdevLPluginName(name_file));
		if(i == m_plugins.end()) {
			XDEVL_MODULE_ERROR("Plugin: '" << name_file << "' doesn't exist. Can't remove plugin.\n");
			return m_plugins.end();
		}


		//
		// Let's try first to shutdown the plugin. There might be some modules left.
		//
		if(i->second->isShutdownPluginValid()) {
			XDEVL_MODULE_INFO("Shutting down plugin: " << pluginName << "\n");
			if(i->second->shutdownPlugin() != ERR_OK) {
				XDEVL_MODULE_WARNING("Shutting down plugin failed.\n");
			} else {
				XDEVL_MODULE_SUCCESS("Shutting down plugin was successful.\n");
			}
		}

		// Ok, delete the dynlib.
		delete(i->second);

		return i;
	}

	XdevLModule* XdevLCoreImpl::createModule(const XdevLModuleName& moduleName,
	        const XdevLID& id,
	        const XdevLPluginName& pluginName,
	        XdevLUserData* userParameter) {

		auto parameter = new XdevLModuleCreateParameter();

		// Sets the plugins name.
		parameter->setPluginName(pluginName);
		// The requested module name
		parameter->setModuleName(moduleName);
		// The requested id
		parameter->setModuleId(id);
		// Of course the core mediator
		parameter->setMediator(this);
		// User parameter
		parameter->setUserParameter(userParameter);

		return createModule(parameter);
	}

	XdevLModule* XdevLCoreImpl::createModule(XdevLModuleCreateParameter* parameter) {

		XdevLPluginInfo* pluginInfo = nullptr;
		//
		// A plugin name was specified so use that to create the model.
		//
		XdevLPluginName pluginName {parameter->getPluginName()};

		if(pluginName != XdevLPluginName("Not Specified")) {
			auto i = m_plugins.find(parameter->getPluginName());
			if(i == m_plugins.end()) {
				// The specified plugin is not found.
				XDEVL_MODULE_ERROR("Creating Module '" << parameter->getModuleName() << "' failed. Could not find plugin: '" << parameter->getPluginName() << "' in the core system.\n");
				exit(-1);
			}
			// Assign value.
			pluginInfo = i->second;

		} else {

			//
			// The user did not specify a plugin name. We have to check if we have
			// that module already registered. Available modules are in the
			// m_modulesMap.
			//
			pluginInfo = m_modulesMap.find(parameter->getModuleName().toString().c_str())->second;

			if(pluginInfo == nullptr) {
				XDEVL_MODULE_ERROR("Could not find any plugins for the module: " << parameter->getModuleId().getName() << std::endl);
				return nullptr;
			}
		}

		// Do we have a module with the same id?
		moduleMap::iterator moduleIterator = m_modules.find(parameter->getModuleId().getName());
		if(m_modules.end() != moduleIterator) {
			XDEVL_MODULE_ERROR("Creating module failed. Module id: '" << parameter->getModuleId().getName() << "' exists. Please use another id.\n");
			exit(-1);
		}

		// Call the create function in the plugin and create the module.
		auto ret = pluginInfo->createModule(parameter);
		switch(ret) {
			case ERR_OK:
				break;
			case ERR_MODULE_NOT_FOUND: {
				XDEVL_MODULE_ERROR("Creating Module failed. Could not find module '" << parameter->getModuleName() << "' in plugin: '" <<  parameter->getPluginName() << "'" << std::endl);
				return nullptr;
			}
			default: {
				XDEVL_MODULE_ERROR("Creating Module failed. Could not allocate memory for module object: '" << parameter->getModuleName() << "'" << std::endl);
				return nullptr;
			}
		}

		// Get some information about the module and print that out.
		XdevLVersion moduleVersion = parameter->getModuleInstance()->getDescriptor().getVersion();
		XDEVL_MODULE_SUCCESS("Module: '" << parameter->getModuleName() << " "
		                     << moduleVersion.geMajor() 	<< "."
		                     << moduleVersion.getMinor()	<< "."
		                     << moduleVersion.getPatch() 	<< "', id = '" << parameter->getModuleId().getName() << "' created successfully." << std::endl);

		// Register this module as an listener. This is important to get messages from the core system.
		registerListener(parameter->getModuleInstance());

		XDEVL_MODULE_INFO("Initialize Module: " << parameter->getModuleId().getName() << "\n");

		// Ok now to initiate the module we have to send the "XDEVL_MODULE_INIT" message.
		XdevLEvent moduleInit;
		moduleInit.type = XDEVL_MODULE_EVENT;
		moduleInit.module.sender = getID().getHashCode();
		moduleInit.module.event = XDEVL_MODULE_INIT;

		if(sendEventTo(parameter->getModuleInstance()->getID().getHashCode(), moduleInit) != ERR_OK) {
			XDEVL_MODULE_ERROR("Initialize failed.\n");
			pluginInfo->deleteModule(parameter->getModuleInstance());
			delete parameter;
			return nullptr;
		}

		auto di = new XdevLModuleInfo(parameter, pluginInfo);
		m_modules.insert(moduleMap::value_type(parameter->getModuleId().getName(), di));
		XDEVL_MODULE_SUCCESS("Started properly:\n");

		return parameter->getModuleInstance();
	}


	xdl_int XdevLCoreImpl::deleteModule(const XdevLID& id) {

		auto moduleIterator = _deleteModule(id);
		if(moduleIterator == m_modules.end()) {
			return ERR_ERROR;
		}

		// Remove it from the module list.
		m_modules.erase(moduleIterator);

		return ERR_OK;
	}

	moduleMap::iterator XdevLCoreImpl::_deleteModule(const XdevLID& id) {
		XDEVL_MODULE_INFO("Removing Module " << id << std::endl);

		auto moduleIterator = m_modules.find(id.getName());
		if(m_modules.end() == moduleIterator) {
			XDEVL_MODULE_ERROR("Module id: '" << id << "' doesn't exist. Cannot remove module.\n");
			return m_modules.end();
		}

		//
		// Send the shutdown message to the module.
		//
		XdevLEvent moduleInit;
		moduleInit.type 			= XDEVL_MODULE_EVENT;
		moduleInit.module.sender 	= getID().getHashCode();
		moduleInit.module.event 	= XDEVL_MODULE_SHUTDOWN;

		if(sendEventTo(moduleIterator->second->getModuleCreateParameter()->getModuleInstance()->getID().getHashCode(), moduleInit) != ERR_OK) {
			XDEVL_MODULE_ERROR("Error occurred during the shutdown process of: '" << id.getName() << "'\n");
		}

		// Remove from the listener list.
		m_listener.erase(id.getHashCode());

		// Delete it from the shared memory.
		moduleIterator->second->getPluginInfo()->deleteModule(moduleIterator->second->getModuleCreateParameter()->getModuleInstance());

		// Delete the XdevLModuleInfo object.
		delete moduleIterator->second;

		return moduleIterator;
	}


	XdevLModule* XdevLCoreImpl::getModule(const XdevLID& id) {
		auto imi = m_modules.find(id.getName());
		if(m_modules.end() == imi) {
			XDEVL_MODULE_ERROR("Module with id: '" << id << "' doesn't exists.\n");
			return nullptr;
		}
		return imi->second->getModuleCreateParameter()->getModuleInstance();
	}

	void* XdevLCoreImpl::getInternal(const XdevLInternalName& id) {
		return nullptr;
	}

	xdl_int XdevLCoreImpl::init() {

		// Initialize with empty parameters.
		XdevLCoreInitParameters parameters;

		return init(parameters);
	}

	xdl_int XdevLCoreImpl::init(const XdevLCoreInitParameters& parameters) {

		m_xdevlPluginPath = parameters.pluginsPath;

		if(parameters.userDataList != nullptr) {
			// TODO Do something with the user data.

			for(xdl_uint a = 0; a < parameters.numberOfUserData; ++a) {
				m_userDataMap[parameters.userDataList[a]->id.getHashCode()] = parameters.userDataList[a];
			}
		}


		TiXmlDocument xmlDocument;
		// Parse xml file only if we have one.
		if(parameters.xmlFileName != "Not Specified") {

			m_XmlFilename = parameters.xmlFileName;
			if(!xmlDocument.LoadFile(m_XmlFilename)) {
				XDEVL_MODULE_ERROR("Could not parse xml file: " << m_XmlFilename << std::endl);
				return ERR_ERROR;
			}

		}

		// If the user has defined a xml file plug, create and initialize which is in the xml file.
		if(readCorePlugins(xmlDocument) != ERR_OK) {
			return ERR_ERROR;
		}

		if(readModuleInfo(xmlDocument) != ERR_OK) {
			return ERR_ERROR;
		}

		// Register the XdevLCore as a listener too.
		if(registerListener(this)!= ERR_OK) {
			XDEVL_MODULE_WARNING("Could not register as listener. Messages will be ignored.\n");
		}

		m_initialized = xdl_true;

		return ERR_OK;
	}

	int XdevLCoreImpl::readCorePlugins(TiXmlDocument& document) {

		// Read plugin information from the XML file.
		TiXmlHandle docHandle(&document);
		TiXmlElement* root = docHandle.FirstChild("XdevLCoreProperties").FirstChildElement("XdevLPlugins").ToElement();
		if(!root) {
			return ERR_OK;
		}

		XDEVL_MODULE_INFO("------------------------------------------------------------\n");
		XDEVL_MODULE_INFO("Begin plug procedure.\n");
		TiXmlElement* child = 0;
		for(child = root->FirstChildElement(); child; child = child->NextSiblingElement()) {
			if(child->ValueTStr() == "XdevLPlugin") {

				// We have to methods to load. If the user specified the filename
				// tag the string will be interpreted as filename.
				// If not the plugin name and version will be used to create the
				// filename of the plugin.
				XdevLPluginName pluginName;
				XdevLVersion version;
				XdevLString platform("");
				if(child->Attribute("filename")) {
					pluginName = child->Attribute("filename");
				} else {

					if(child->Attribute("name")) {
						pluginName = child->Attribute("name");
					}

					if(child->Attribute("version")) {
						XdevLString versionAsString(child->Attribute("version"));
						version.fromString(versionAsString);
					}

					if(child->Attribute("platform")) {
						platform = child->Attribute("platform");
					}
				}
				if(platform == XdevLString("")) {
					if(plug(pluginName, version) != ERR_OK) {
						XDEVL_MODULE_ERROR("Could not plug plugin: " << pluginName << "\n");
						return ERR_ERROR;
					}
				} else if(XDEVL_CURRENT_PLATFORM_AS_STRING == platform) {
					if(plug(pluginName, version) != ERR_OK) {
						XDEVL_MODULE_ERROR("Could not plug plugin: " << pluginName << "\n");
						return ERR_ERROR;
					}
				} else {
					// TODO Shall we put some info here? I think for now it is not necessary.
				}
			} else {
				XDEVL_MODULE_WARNING("No plugin tag found in the xml file.\n");
			}
		}
		XDEVL_MODULE_INFO("Exit plug procedure.\n");
		XDEVL_MODULE_INFO("------------------------------------------------------------\n\n");
		return ERR_OK;
	}

	int XdevLCoreImpl::readModuleInfo(TiXmlDocument& document) {

		// Read module information from the XML file.
		TiXmlHandle docHandle(&document);
		TiXmlElement* root = docHandle.FirstChild("XdevLCoreProperties").FirstChildElement("XdevLModules").ToElement();
		if(!root) {
			// No modules section defined. That is OK :).
			return ERR_OK;
		}

		XDEVL_MODULE_INFO("------------------------------------------------------------\n");
		XDEVL_MODULE_INFO("Begin module procedure.\n");

		TiXmlElement* plugins = 0;
		for(plugins = root->FirstChildElement(); plugins; plugins = plugins->NextSiblingElement()) {

			if(plugins->ValueTStr() == "XdevLModule") {
				// Make error if the user not specified the id of that module.
				if(!plugins->Attribute("id")) {
					XDEVL_MODULE_ERROR("Missing module 'id' attribute");
					return ERR_ERROR;
				}

				XdevLPluginName plugin_name;
				XdevLModuleName module_name;
				if(plugins->Attribute("plugin_name")) {
					plugin_name = plugins->Attribute("plugin_name");
				}
				if(plugins->Attribute("name")) {
					module_name = plugins->Attribute("name");
				} else {
					XDEVL_MODULE_ERROR("Could not create module. Missing the 'module_name' attribute.\n");
					continue; // TODO Maybe exit() here?
				}

				XdevLID id(plugins->Attribute("id"));
				if(createModule(module_name, id, plugin_name, nullptr) == nullptr) {
					continue; // Maybe exit() here?
				}
			}
		}
		XDEVL_MODULE_INFO("Exit module procedure.\n");
		XDEVL_MODULE_INFO("------------------------------------------------------------\n\n");
		return ERR_OK;
	}

	xdl_int XdevLCoreImpl::update() {

		// TODO This sections needs redesign. The update and listener methods
		// maybe block the whole system which is not so good. For the moment
		// we accept this crappy limitation.

		//
		// Call all update methods of registered modules.
		//
		for(auto& module : m_modules) {
			XdevLEvent event;
			event.type 			= XDEVL_MODULE_EVENT;
			event.module.event 	= XDEVL_MODULE_UPDATE;
			sendEventTo(module.second->getModuleCreateParameter()->getModuleId().getHashCode(), event);
		}

		//
		// Inform all listeners about events.
		//
		XdevLEvent event;
		while(m_event_queue.pull_front(event) != -1) {

			//
			// Send to XdevLListener objects.
			//
			for(auto listener : m_listener) {

				// Inform listener.
				listener.second->notify(event);

			}

			//
			// Send to listener using callback.
			//
			if(m_listenerCallbackFuncList.size() > 0) {
				for(auto callback : m_listenerCallbackFuncList) {
					callback(event);
				}
			}

		}

		return ERR_OK;
	}


	xdl_int XdevLCoreImpl::registerListener(XdevLListener* listener) {
		assert(listener && "XdevLCoreImpl::registerListener(listener = nullptr)");

		auto i = m_listener.find(listener->getID().getHashCode());
		if(i!=m_listener.end()) {
			XDEVL_MODULE_INFO("! -> Listener: '" << listener->getID().getName() << "' is registered already.\n");
			return ERR_ERROR;
		}

		m_listener.insert(listenerMap::value_type(listener->getID().getHashCode(), listener));

		return ERR_OK;
	}


	xdl_int XdevLCoreImpl::sendEventTo(xdl_uint64 receiver, XdevLEvent& ev) {

		auto i = m_listener.find(receiver);
		if(i==m_listener.end()) {
			XDEVL_MODULE_INFO("! -> Listener width HASH= '" << receiver << "' not found. Could not send event.\n");
			return ERR_ERROR;
		}

		return i->second->notify(ev);
	}

	void XdevLCoreImpl::fireEvent(XdevLEvent& event) {
		m_event_queue.put(event);
	}

	xdl_int XdevLCoreImpl::notify(XdevLEvent& event) {

		switch(event.type) {
			case XDEVL_MODULE_EVENT: {
				if(event.module.type == XDEVL_CORE_SHUTDOWN) {
					// Let's do something here.
				}
			} break;

		}
		return ERR_OK;

	}

	xdl_int XdevLCoreImpl::registerListenerCallbackFunction(XdevLListenerCallbackFunc listenerCallbackFunc) {
		m_listenerCallbackFuncList.push_back(listenerCallbackFunc);
		return ERR_OK; // TODO Can something happen here?
	}

}
