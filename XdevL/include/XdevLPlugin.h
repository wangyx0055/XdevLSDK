/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2015 Cengiz Terzibas

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

#ifndef XDEVL_PLUGIN_H
#define XDEVL_PLUGIN_H

#include <XdevLTypes.h>
#include <XdevLVersion.h>
#include <XdevLDynLib.h>
#include <string>
#include <vector>
#include <cassert>

namespace xdl {


	class XdevLPluginCreateParameter;
	class XdevLModuleCreateParameter;
	class XdevLModule;
	class XdevLCoreMediator;
	class XdevLPluginDescriptor;

	//
	// Function pointer typedefs for creating/deleting modules.
	//
	typedef int(*XdevLCreateModuleFunction)(XdevLModuleCreateParameter* parameter);
	typedef void(*XdevLDeleteModuleFunction)(XdevLModule* obj);
	typedef XdevLPluginDescriptor* (*XdevLGetPluginDescriptorFunction)();
	typedef xdl_int(*XdevLPluginInitFunction)(XdevLPluginCreateParameter* parameter);
	typedef xdl_int(*XdevLPluginShutdownFunction)();


	class XdevLPluginCreateParameter {
		public:
			XdevLPluginCreateParameter(XdevLCoreMediator* mediator) : m_mediator(mediator) {
				
			}
			/// Returns the mediator of the core system.
			/**
				A valid pointer to the current mediator. This is a part of the XdevLCore module.
			*/
			virtual XdevLCoreMediator* getMediator() {
				return m_mediator;
			}
		private:
			XdevLCoreMediator*	 		m_mediator;
	};

	/**
		@class XdevLPluginDescriptor
		@brief This class holds informations about a plugin.
	*/
	class XdevLPluginDescriptor {
		public:
			XdevLPluginDescriptor(const XdevLPluginName& name, const XdevLVersion& version) :
				m_name(name),
				m_version(version) {}

			XdevLPluginDescriptor(const XdevLString& name,
			                      const std::vector<XdevLModuleName>& moduleNames,
			                      xdl_uint major,
			                      xdl_uint minor,
			                      xdl_uint patch) :
				m_name(name.toString()),
				m_version(major, minor, patch) {
				for(auto& moduleName : moduleNames) {
					m_moduleNames.push_back(moduleName);
				}
			}

			virtual ~XdevLPluginDescriptor() {}

			/// Returns the name of the plugin.
			/**
				@return Returns a valid pointer to a string.
			*/
			virtual const XdevLPluginName& getName() const {
				return m_name;
			}

			/// Returns the number of modules in the plugin.
			/**
				@return Returns the number of supported modules in the plugin.
			*/
			virtual xdl_int getNumModules() const {
				return m_moduleNames.size();
			}

			/// Returns the name of the specified module.
			/**
				@param idx Index of the module. The index idx < getNumModules().
				@return Returns a valid pointer to a string which is the name of the module.
			*/
			virtual const XdevLModuleName& getModuleName(const xdl_int& idx) const {
				return m_moduleNames[idx];
			}

			/// Returns the version of the plugins.
			/**
				@return Return the version of this plugin.
			*/
			virtual const XdevLVersion& getVersion() const {
				return m_version;
			}

		private:
			// Holds the name of the plugins.
			XdevLPluginName	m_name;

			// Holds the name of the modules in this plugin.
			std::vector<XdevLModuleName> m_moduleNames;

			// Holds the version of this plugin.
			XdevLVersion m_version;
	};


	/**
		@class XdevLPluginInfo
		@brief Class that creates/deletes modules.
	*/
	class XdevLPluginInfo {
		public:

			XdevLPluginInfo(XdevLCreateModuleFunction c,
			                XdevLDeleteModuleFunction d,
			                XdevLGetPluginDescriptorFunction desc,
			                XdevLSharedLibrary* dynlib) :
				m_initPlugin(nullptr),
				m_shutdownPlugin(nullptr),
				m_createModule(c),
				m_deleteModule(d),
				m_getPluginDescriptor(desc),
				m_version(desc()->getVersion()),
				m_sharedLibraryLoader(dynlib) {
			}

			XdevLPluginInfo(XdevLPluginInitFunction pif,
			                XdevLPluginShutdownFunction psf,
			                XdevLCreateModuleFunction c,
			                XdevLDeleteModuleFunction d,
			                XdevLGetPluginDescriptorFunction desc,
			                XdevLSharedLibrary* dynlib) :
				m_initPlugin(pif),
				m_shutdownPlugin(psf),
				m_createModule(c),
				m_deleteModule(d),
				m_getPluginDescriptor(desc),
				m_version(desc()->getVersion()),
				m_sharedLibraryLoader(dynlib) {
			}

			~XdevLPluginInfo() {
				delete m_sharedLibraryLoader;
			}

			/// Initialize a plugin.
			xdl_int initPlugin(XdevLPluginCreateParameter* parameter) {
				return m_initPlugin(parameter);
			}

			/// Shut down a plugin.
			xdl_int shutdownPlugin() {
				return m_shutdownPlugin();
			}

			/// Creates a module.
			xdl_int createModule(XdevLModuleCreateParameter* parameter) {
				return m_createModule(parameter);
			}

			/// Deletes a module.
			void deleteModule(XdevLModule* obj) {
				m_deleteModule(obj);
			}

			/// Returns the plugins descriptor.
			XdevLPluginDescriptor* getPluginDescriptor() {
				return m_getPluginDescriptor();
			}

			/// Returns the associated shared library loader object.
			XdevLSharedLibrary* getSharedLibraryLoader() {
				return m_sharedLibraryLoader;
			}

			/// Returns the version.
			const XdevLVersion& getVersion() const  {
				return m_version;
			}

			xdl_bool isInitPluginValid() {
				return (m_initPlugin != nullptr);
			}

			xdl_bool isShutdownPluginValid() {
				return (m_shutdownPlugin != nullptr);
			}

		private:

			// Hodls the function pointer to initialize a plugin.
			XdevLPluginInitFunction m_initPlugin;

			// Holds the function pointer to shutdown a plugin.
			XdevLPluginShutdownFunction m_shutdownPlugin;

			// Holds the functon pointer to create a module.
			XdevLCreateModuleFunction 			m_createModule;

			// Holds the function pointer to delete a module.
			XdevLDeleteModuleFunction 			m_deleteModule;

			// Holds the function pointer to get the plugins descriptor.
			XdevLGetPluginDescriptorFunction 		m_getPluginDescriptor;

			// Holds the version of the plugin.
			XdevLVersion 							m_version;

			// Holds the shared library that contains the modules.
			XdevLSharedLibrary* 					m_sharedLibraryLoader;

	};

}

#endif
