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
		"(c) 2005 - 2015 Cengiz Terzibas."
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

			virtual const XdevLModuleDescriptor& getDescriptor() const;
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
			virtual xdl_int plug(const XdevLPluginName& pluginName, const XdevLVersion& version = XdevLVersion::zero);
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
