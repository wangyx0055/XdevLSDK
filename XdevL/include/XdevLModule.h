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

#ifndef XDEVL_MODULE_H
#define XDEVL_MODULE_H

#include <XdevLVersion.h>
#include <XdevLListener.h>
#include <XdevLLog.h>
#include <XdevLUserData.h>
#include <algorithm>
#include <vector>
#include <memory>

namespace xdl {

	class XdevLModuleCreateParameter;
	class XdevLCoreMediator;
	class XdevLCommandLineParser;
	class XdevLLog;

	typedef XdevLModule* IPXdevLModule;

//#define XDEVL_NEW_MODULE(CLASS_NAME, PARAMETER) std::shared_ptr<CLASS_NAME>(new CLASS_NAME(PARAMETER))
//#define XDEVL_NEW_MODULE_DESCRIPTOR(CLASS_NAME, PARAMETER, DESCRIPTOR) std::shared_ptr<CLASS_NAME>(new CLASS_NAME(PARAMETER, DESCRIPTOR))

#define XDEVL_USE_MODULE(CLASS_NAME, MODULE) (MODULE)
#define XDEVL_NEW_MODULE(CLASS_NAME, PARAMETER) new CLASS_NAME(PARAMETER)
#define XDEVL_NEW_MODULE_DESCRIPTOR(CLASS_NAME, PARAMETER, DESCRIPTOR) new CLASS_NAME(PARAMETER, DESCRIPTOR)

#define XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DECLARATION(MODULENAME) extern "C" XDEVL_EXPORT xdl::xdl_int create##MODULENAME(xdl::XdevLModuleCreateParameter* parameter, std::shared_ptr<xdl::XdevLModule>& module);
#define XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DEFINITION(MODULENAME, MODULECLASS, DESCRIPTOR) extern "C" XDEVL_EXPORT xdl::xdl_int create##MODULENAME(xdl::XdevLModuleCreateParameter* parameter, std::shared_ptr<xdl::XdevLModule>& module) {\
		module = std::make_shared<MODULECLASS>(parameter, DESCRIPTOR);\
		return xdl::ERR_OK;\
	}

#define XDEVL_EXPORT_PLUGIN_INIT_FUNCTION_DECLARATION(PLUGINNAME) extern "C" XDEVL_EXPORT xdl::xdl_int init##PLUGINNAME();
#define XDEVL_EXPORT_PLUGIN_SHUTDOWN_FUNCTION_DECLARATION(PLUGINNAME) extern "C" XDEVL_EXPORT xdl::xdl_int shutdown##PLUGINNAME();

#define XDEVL_EXPORT_PLUGIN_INIT_FUNCTION_DEFINITION(PLUGINNAME) extern "C" XDEVL_EXPORT xdl::xdl_int init##PLUGINNAME()
#define XDEVL_EXPORT_PLUGIN_INIT_FUNCTION_DEFINITION_DEFAULT(PLUGINNAME) extern "C" XDEVL_EXPORT xdl::xdl_int init##PLUGINNAME(){return xdl::ERR_OK;}

#define XDEVL_EXPORT_PLUGIN_SHUTDOWN_FUNCTION_DEFINITION(PLUGINNAME) extern "C" XDEVL_EXPORT xdl::xdl_int shutdown##PLUGINNAME()
#define XDEVL_EXPORT_PLUGIN_SHUTDOWN_FUNCTION_DEFINITION_DEFAULT(PLUGINNAME) extern "C" XDEVL_EXPORT xdl::xdl_int shutdown##PLUGINNAME(){return xdl::ERR_OK;}



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

	enum XdevLModuleStates {
	  XDEVL_MODULE_STATE_NONE = 1,
	  XDEVL_MODULE_STATE_DISABLE_AUTO_DESTROY = 2
	};

	/**
		@class XdevLModuleDescriptor
		@brief This class holds informations about a module.
	 */
	class XdevLModuleDescriptor {
		public:
			XdevLModuleDescriptor(const XdevLModuleName& moduleName, const XdevLVersion& version):
				m_moduleName(moduleName),
				m_version(version),
				m_states(XDEVL_MODULE_STATE_NONE) {
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
				m_version(major, minor, patch),
				m_states(XDEVL_MODULE_STATE_NONE) {

			}

			XdevLModuleDescriptor(const XdevLString& vendor,
			                      const XdevLString& author,
			                      const XdevLModuleName& moduleName,
			                      const XdevLString& copyright,
			                      const XdevLString& description,
			                      xdl_uint major,
			                      xdl_uint minor,
			                      xdl_uint patch,
			                      xdl_uint32 states) :
				m_vendor(vendor),
				m_author(author),
				m_moduleName(moduleName),
				m_copyright(copyright),
				m_description(description),
				m_version(major, minor, patch),
				m_states(states) {

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

			/// Return the state of the module.
			virtual xdl_bool getState(XdevLModuleStates state) const {
				return (m_states & state);
			}

			/// Add module that depends on this module.
			virtual void registerDependency(XdevLModule* module) {
				m_dependency.push_back(module);
			}

			/// Return the list of dependency modules.
			const std::vector<XdevLModule*>& getDependencies() const {
				return m_dependency;
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

			xdl_uint32			m_states;

			std::vector<IPXdevLModule> m_dependency;
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
			virtual XdevLModuleDescriptor& getDescriptor() = 0;
			
			/// Sets the mediator of this module.
			virtual void setMediator(XdevLCoreMediator* core) = 0;
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

			XdevLModuleCreateParameter(XdevLCoreMediator* mediator) :
				m_moduleInstance(nullptr),
				m_logger(nullptr),
				m_mediator(mediator),
				m_userData(nullptr) {}

			virtual ~XdevLModuleCreateParameter() {}

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
			virtual void setModuleInstance(IPXdevLModule obj) {
				m_moduleInstance = obj;
			}

			/// Returns the module instance.
			virtual IPXdevLModule getModuleInstance() {
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
			
			/// Sets the modules mediator.
			virtual void setModuleMediator(XdevLCoreMediator* mediator) {
				m_mediator = mediator;
			}

		private:
			XdevLID 						m_id;
			IPXdevLModule 			m_moduleInstance;
			XdevLLog*						m_logger;
			XdevLCoreMediator*	m_mediator;
			XdevLModuleName			m_moduleName;
			XdevLPluginName			m_pluginsName;
			XdevLUserData* 			m_userData;

	};

}

#endif
