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

#ifndef XDEVL_MODULE_H
#define XDEVL_MODULE_H

#include <XdevLVersion.h>
#include <XdevLListener.h>
#include <XdevLLog.h>
#include <XdevLUserData.h>
#include <algorithm>
#include <vector>

namespace xdl {

	class XdevLCoreMediator;
	class XdevLCommandLineParser;
	class XdevLLog;

	/**
		@enum XdevLModuleEvents
		@brief Supported Module messages.
	*/
	enum XdevLModuleEvents {
	    XDEVL_MODULE_INIT,
	    XDEVL_MODULE_SHUTDOWN,
	    XDEVL_MODULE_UPDATE,
	    XDEVL_MODULE_RESTART,
	    XDEVL_MODULE_RESET
	};

	/**
		@class XdevLModuleDescriptor
		@brief This class holds informations about a module.
	 */
	class XdevLModuleDescriptor {
		public:
			XdevLModuleDescriptor(const XdevLModuleName& moduleName, const XdevLVersion& version):
				m_moduleName(moduleName),
				m_version(version) {
			}

			XdevLModuleDescriptor(const XdevLString& vendor,
			                      const XdevLString& author,
			                      const XdevLModuleName& moduleName,
			                      const XdevLString& copyright,
			                      const XdevLString& description,
			                      xdl_uint major,
			                      xdl_uint minor,
			                      xdl_uint patch) :
				m_vendor(vendor),
				m_author(author),
				m_moduleName(moduleName),
				m_copyright(copyright),
				m_description(description),
				m_version(major, minor, patch) {

			}

			virtual ~XdevLModuleDescriptor() {}

			/// Return the vendor of the module.
			virtual const XdevLString& getVendor() const {
				return m_vendor;
			}

			/// Return the author of the module.
			virtual const XdevLString& getAuthor() const {
				return m_author;
			}

			/// Return the name of the module.
			virtual const XdevLModuleName& getName() const 	{
				return m_moduleName;
			}

			/// Return the copyright.
			virtual const XdevLString& getCopyright() const {
				return m_copyright;
			}

			/// Return the description.
			virtual const XdevLString& getDescription() const {
				return m_description;
			}

			/// Return the version of the module.
			virtual const XdevLVersion& getVersion() const {
				return m_version;
			}

		private:
			// Holds the name of the vendor.
			XdevLString 		m_vendor;

			// Holds the name of the author.
			XdevLString 		m_author;

			// Holds the name of the module.
			XdevLModuleName m_moduleName;

			// Holds the the copyright.
			XdevLString 		m_copyright;

			// Holds the description.
			XdevLString 		m_description;

			// Holds the version of the module.
			XdevLVersion 		m_version;
	};


	/**
		@class XdevLModule
		@brief Interface class for all modules in plugins
		@author Cengiz Terzibas

		This class is the base class for all modules in the XdevL Core System. If you
		are a plugin developer you should read the @b XdevLSDKManual which is in the @b XdevLSDK/XdevL/doc
		folder.
		@note Normal users shouldn't use this functions.
	*/
	class XdevLModule : public XdevLListener {
		public:
			virtual ~XdevLModule() {}

			/// Returns the pointer to a internal object.
			/**
				This internal object can be platform specific like the 'Display' handle of
				the X-Window server. You have to check the vendor manuals for more information
				about that. Avoid using this function in your applications. For plugins
				programmer this function could be essential. Check the provided manual of the
				plugins for the supported id's.

				@param id The identification string that describes the object.
			*/
			virtual void* getInternal(const XdevLInternalName& id) = 0;

			/// Returns the XdevLModuleDescriptor of the module
			/**
				Every module has a descriptor which gives you detailed information like, version, author etc.
				@return A valid pointer to the descriptor.
			*/
			virtual const XdevLModuleDescriptor& getDescriptor() const = 0;
	};

	/**
		@class XdevLModuleCreateParameter
		@brief Parameter interface to create modules.
		@author Cengiz Terzibas

		This interface is used to pass parameters to the module during creation time.
	*/
	class XdevLModuleCreateParameter {
		public:
			XdevLModuleCreateParameter() :
				m_moduleInstance(nullptr),
				m_logger(nullptr),
				m_mediator(nullptr),
				m_userData(nullptr) {}

			virtual ~XdevLModuleCreateParameter() {}

			/// Sets the mediator of the core system.
			/**
				The current XdevLCore module will set this to provide a valid mediator interface for the module.
				@param med A valid pointer to the mediator.
			*/
			virtual void setMediator(XdevLCoreMediator* med) {
				m_mediator = med;
			}

			/// Returns the mediator of the core system.
			/**
				A valid pointer to the current mediator. This is a part of the XdevLCore module.
			*/
			virtual XdevLCoreMediator* getMediator() {
				return m_mediator;
			}

			/// Sets the logger.
			virtual void setLogger(XdevLLog* logger) {
				m_logger = logger;
			}

			/// Returns the logger.
			virtual XdevLLog* getLog() {
				return m_logger;
			}

			/// Sets the module instance.
			/**
				This function will be used in the plugin. If the user request was creating a valid instance
				of a module this function will provide it.
				@param obj A valid module instance.
			*/
			virtual void setModuleInstance(XdevLModule* obj) {
				m_moduleInstance = obj;
			}

			/// Returns the module instance.
			virtual XdevLModule* getModuleInstance() {
				return m_moduleInstance;
			}

			/// Sets the module id. Don't get confused with the moduleClassId.
			virtual void setModuleId(XdevLID id) {
				m_id = id;
			}

			/// Returns the module id. Don't get confused with the moduleClassId.
			virtual XdevLID getModuleId() {
				return m_id;
			}

			/// Sets the module name.
			virtual void setModuleName(const XdevLModuleName& name) {
				m_moduleName = name;
			}

			/// Returns the module name.
			virtual const XdevLModuleName& getModuleName() const {
				return m_moduleName;
			}

			/// Sets a user defines data.
			virtual void setUserParameter(XdevLUserData* userData) {
				m_userData = userData;
			}

			/// Returns the user defines data.
			virtual XdevLUserData* getUserDefinedData() {
				return m_userData;
			}

			/// Returns the plugins name.
			virtual const XdevLPluginName& getPluginName() const {
				return m_pluginsName;
			}

			/// Sets the plugins name.
			virtual void setPluginName(const XdevLPluginName& name) {
				m_pluginsName = name;
			}

		private:
			XdevLID 								m_id;
			XdevLModule* 						m_moduleInstance;
			XdevLLog*								m_logger;
			XdevLCoreMediator*	 		m_mediator;
			XdevLModuleName					m_moduleName;
			XdevLPluginName					m_pluginsName;
			XdevLUserData* 					m_userData;

	};

	/**
		@class XdevLModuleDeleteParameter
		@brief Interface class to communicate with plugins. This interface is used during deletion
		proccess of modules.
		@author Cengiz Terzibas

		If you instruct XdevLCore to delete a module or device he communicates via this
		interface with the plugins that holds module information's. This class isn't important
		for normal users. If you are a plugin developer you should read the @b XdevLSDKManual which
		is in the @b XdevLSDK/XdevL/doc folder.
	*/
	class XdevLModuleDeleteParameter {
		public:

			virtual ~XdevLModuleDeleteParameter() {}

			/// Returns the module instance.
			virtual XdevLModule* getModuleInstance() = 0;

	};

}

#endif
