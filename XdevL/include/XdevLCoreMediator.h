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

#ifndef XDEVL_CORE_MEDIATOR_H
#define XDEVL_CORE_MEDIATOR_H

#include <XdevLEvent.h>
#include <XdevLVersion.h>
#include <string>

namespace xdl {

	class XdevLModule;
	class XdevLUserData;
	class XdevLListener;
	class XdevLTimer;
	class XdevLID;
	class XdevLLog;

	typedef void (*XdevLListenerCallbackFunc)(XdevLEvent& event);

	const std::string XdevLCorePropertiesName("XdevLCoreProperties");

	/**
		@enum XdevLCoreEventTypes
		@brief Supported XdevLCore message types.
	*/
	enum XdevLCoreEventTypes {
	  XDEVL_CORE_SHUTDOWN
	};

	/**
		@class XdevLCoreMediator
	 	@brief A communication interface between Core and modules.
	 	@author Cengiz Terzibas

	 	The mediator is the communication interface between modules and the core
	 	system. The user or the plugin developer can plug and unplug plugins and
	 	create and delete modules.
	*/
	class XdevLCoreMediator {
		public:

			virtual ~XdevLCoreMediator() {}
			
			/// Returns the logger.
			virtual XdevLLog* getLogger() = 0;
			
			/// Plugs a plugins into the core system.
			/**
				@param pluginName A null terminated string that indicates the file name of
				the plugins that should be loaded into the core system.
				@param version The version number of the plugin to plug. 
				@return Returns ERR_OK if the plugin process was successful. Otherwise it
				will return ERR_ERROR.
			*/
			virtual xdl_int plug(const XdevLPluginName& pluginName, const XdevLVersion& version = XdevLVersion::zero) = 0;

			/// Unplugs a plugin from the core system.
			/**
				@param pluginName A null terminated string that was used during the call of
				the plug() function.
				@return Returns ERR_OK if the unplug procedure was successful otherwise
				ERR_ERROR.
			*/
			virtual xdl_int unplug(const XdevLPluginName& pluginName) = 0;

			/// Creates a module from a plugin.
			/**
				This function create a modules which is in the specified plugin. Don't
				use the same id for different modules. You can use the getFreeModuleId()
				to get an available identification code from XdevLCore. The last parameter
				is for future development and has no function in the 0.1.0 version of XdevL.

				@param moduleName A NULL terminated string. This should be the modules name,
				in the plugin. This should be provided by the plugin and module manufacturer.
				@param id The identification code for the module which will be used in the
				system.
				@param pluginName The file name of the module without an extension like *.dll
				or *.so.				
				@param userParameter Not used in the XdevL version 0.1.0
				@return A valid pointer to the module if it was successful otherwise it
				will be NULL.
			*/
			virtual XdevLModule* createModule(const XdevLModuleName& moduleName,
			                                  const XdevLID& id,
			                                  const XdevLPluginName& pluginName = XdevLPluginName("Not Specified"),
			                                  XdevLUserData* userParameter = NULL) = 0;

			/// Removes a module from the core system.
			/**
				@param id The id of the module that should be deleted. You can get the XdevLModuleId
				for a module if you use the XdevLModule::getId() fuction.
				@return
				- @b ERR_MODULE_NOT_FOUND
				The specified module doesn't exists int the system.
				- @b ERR_OK
				If it was successful.
			*/
			virtual xdl_int deleteModule(const XdevLID& id) = 0;

			/// Returns the instance of a existing module.
			/**
				@param id The identification code for a module that should exist in the core
				system.
				@return
				- Returns a valid ponter to the module if it exists in the system.
				- @b NULL if the specified module doesn't exists in the system.
			*/
			virtual XdevLModule* getModule(const XdevLID& id) = 0;

			/// Returns the XML file name that was used during the creation of the core system.
			/**
				@return A valid file name if a XML file was used during the creation of the core system.
				If the core system was created in default mode (without XML file) it will return NULL.
			*/
			virtual const XdevLFileName& getXmlFilename() const = 0;

			/// Returns the current time of the XdevLCore system.
			/**
				@return The current core system time in seconds.
			*/
			virtual xdl_double getTime() = 0;

			/// Returns the elapsed time since the last update() call.
			/**
				@return Time in seconds.
			*/
			virtual xdl_double getDT() = 0;

			/// Returns the internal timer instance.
			virtual XdevLTimer& getTimer() = 0;

			/// Register a listener.
			/**
				Registers the listener to the core signal system. If events are send from the
				core system the listener will be informed about that. If you don't need this
				functionality don't register your module.

			 	@param listener A valid pointer to a listener object.
				@return
				- @b ERR_ERROR
				The specified module couldn't be registered (@e Every module is an XdevLListener).
				- @b ERR_OK
				If it was successful.
			*/
			virtual xdl_int registerListener(XdevLListener* listener) = 0;

			/// Send event.
			/**
				Sends a event directly to a listener. This message will not be queued.

				@param receiver The receiver's hash code. Can be retrieved with getID() method of modues.
				@param event A valid XdevLEvent object.
				@return
				- @b ERR_ERROR
				The specified event could not be send. This may have different causes.
					-# The specified receiver module doesn't exists in the system.
					-# The specified module caused an error.
				- @b ERR_OK
				If it was successful.
			*/
			virtual xdl_int sendEventTo(xdl_uint64 receiver, XdevLEvent& event) = 0;

			/// Fire an event.
			/**
				This message will be put into the event queue and all listeners will receive this event.
			*/
			virtual void fireEvent(XdevLEvent& event) = 0;

			/// Add a listener callback function. 
			virtual xdl_int registerListenerCallbackFunction(XdevLListenerCallbackFunc listenerCallbackFunc) = 0;
			
		protected:
			/// Returns the pointer to a internal object.
			/**
				This internal object can be platform specific like the 'Display' handle of
				the X-Window server. Check the plugin and module developers manuals for more
				information.

				@param id A NULL terminated string which is the identification of the internal value.
				@return
				- Valid pointer to the specified object.
				- @b NULL if the specified object doesn't exists
			*/
			virtual void* getInternal(const XdevLInternalName& id)= 0;

	};

}

#endif
