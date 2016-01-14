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

#ifndef XDEVL_CORE_IMPL_H
#define XDEVL_CORE_IMPL_H

#include <map>
#include <list>
#include <vector>
#include <stack>
#include <tinyxml.h>
#include <XdevLTypes.h>
#include <XdevLModule.h>
#include <XdevLPlugin.h>
#include <XdevLCore.h>
#include <XdevLUtils.h>
#include <XdevLDynLib.h>
#include <XdevLRingBuffer.h>
#include <XdevLTimer.h>
#include <XdevLPluginImpl.h>

// TODO Destroying of modules that depend on each other causes issues when destroying. Plugins and Modules must have a dependency list that can be check
// for destroy order.

namespace xdl {

	// Holds the plugin major version.
	const xdl_uint XdevLCorePluginMajorVersion = XDEVLCORE_MAJOR_VERSION;

	// Holds the plugin minor version.
	const xdl_uint XdevLCorePluginMinorVersion = XDEVLCORE_MINOR_VERSION;

	// Holds the plugin patch version.
	const xdl_uint XdevLCorePluginPatchVersion = XDEVLCORE_PATCH_VERSION;


	// Holds the Major version number.
	const xdl_uint XdevLCoreMajorVersion = XDEVLCORE_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLCoreMinorVersion = XDEVLCORE_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLCorePatchVersion = XDEVLCORE_MODULE_PATCH_VERSION;


	static const XdevLString vendor {
		"www.codeposer.net"
	};
	static const XdevLString author {
		"Cengiz Terzibas"
	};
	static const XdevLString copyright {
		"(c) 2005 - 2016 Cengiz Terzibas."
	};
	static const XdevLString pluginName {
		"XdevLCore"
	};
	static const XdevLString description {
		"The XdevL Core System."
	};
	static const std::vector<XdevLModuleName> moduleName {
		XdevLModuleName("XdevLCore")
	};

	/**
		@class XdevLModuleInfo
		@brief Holds XdevLModule information.
	 */
	class XdevLModuleInfo {
		public:
			XdevLModuleInfo(XdevLModuleCreateParameter* param, XdevLPluginInfo* pluginInfo) : m_parameter(param), m_pluginInfo(pluginInfo) {}

			virtual ~XdevLModuleInfo() {
				delete m_parameter;
			}

			XdevLPluginInfo* getPluginInfo() {
				return m_pluginInfo;
			}

			XdevLModuleCreateParameter*	 getModuleCreateParameter() {
				return m_parameter;
			}

		private:
			XdevLModuleCreateParameter*	m_parameter;
			XdevLPluginInfo*			m_pluginInfo;
	};

	typedef std::map<XdevLPluginName, XdevLPluginInfo*> pluginMap;

	typedef std::map<XdevLString, XdevLModuleInfo*>	moduleMap;

	typedef std::map<xdl_uint64, XdevLListener*>	listenerMap;

	typedef std::vector<XdevLListenerCallbackFunc> callbackListenerType;

	/**
		@class XdevLCoreImpl
		@brief Implementation of XdevLCore
		@author Cengiz Terzibas
	*/
	class XdevLCoreImpl : public XdevLCore {
		public:
			XdevLCoreImpl(XdevLCommandLineParser* param);
			virtual ~XdevLCoreImpl();

			static XdevLModuleDescriptor m_coreDescriptor;

			virtual XdevLModuleDescriptor& getDescriptor();
			virtual void* getInternal(const XdevLInternalName& id);
			virtual xdl_int update();
			virtual xdl_int shutdown();
			virtual XdevLLog* getLogger();
			virtual XdevLModule* getModule(const XdevLID& id);
			virtual XdevLModule* createModule(const XdevLModuleName& moduleName,
			                                  const XdevLID& id,
			                                  const XdevLPluginName& pluginName,
			                                  XdevLUserData* userParameter);
			virtual XdevLModule* createModule(XdevLModuleCreateParameter* parameter);
			virtual xdl_int deleteModule(const XdevLID& id);
			virtual xdl_int plug(const XdevLPluginName& pluginName, const XdevLVersion& version = XdevLVersion::zero, const XdevLPlatformName& platform = XDEVL_CURRENT_PLATFORM_AS_STRING);
			virtual xdl_int unplug(const XdevLPluginName& pluginName);
			virtual const XdevLFileName& getXmlFilename() const;
			virtual xdl_int init();
			virtual xdl_int init(const XdevLCoreInitParameters& parameters);
			virtual xdl_int registerListener(XdevLListener* listener);
			virtual xdl_int sendEventTo(xdl_uint64 receiver, XdevLEvent& ev);
			virtual void fireEvent(XdevLEvent& event);
			virtual xdl_int notify(XdevLEvent& event);
			virtual const XdevLID& getID() const;
			virtual xdl_double getTime();
			virtual xdl_double getDT();
			virtual XdevLTimer& getTimer();
			virtual xdl_int registerListenerCallbackFunction(XdevLListenerCallbackFunc listenerCallbackFunc);
			xdl_bool coreInitialized();

		protected:
			pluginMap::iterator _unplug(const XdevLPluginName& pluginName);
			moduleMap::iterator _deleteModule(const XdevLID& id);
		protected:

			// Holds the initialized state.
			xdl_bool m_initialized;

			// Holds the id.
			XdevLID		m_id;

			// Holds the logger.
			XdevLLog*		m_logger;

			// Holds listener.
			listenerMap m_listener;

			// Devices which should be created automatically.
			std::vector<std::string> m_autoDevices;

			// Reads all plugins specified in the XML document.
			int readCorePlugins(TiXmlDocument& document);

			// Reads all modules specified in the XML document.
			int readModuleInfo(TiXmlDocument& document);

			// Holds the main xml filename.
			XdevLFileName m_XmlFilename;

			// Holds the timer.
			XdevLTimer 	m_timer;

			// Holds the plugin path.
			std::string m_xdevlPluginPath;

			// Holds the command line information.
			XdevLCommandLineParser* m_commandLine;

			// TODO This is the event queue. Maybe another approach would be better.
			XdevLRingBuffer<XdevLEvent, 500> m_event_queue;

			std::map<xdl_uint64, XdevLUserData*> m_userDataMap;
			std::map<std::string, XdevLPluginInfo*> m_modulesMap;
			moduleMap		m_modules;
			pluginMap		m_plugins;
			callbackListenerType m_listenerCallbackFuncList;
	};

}

#endif
