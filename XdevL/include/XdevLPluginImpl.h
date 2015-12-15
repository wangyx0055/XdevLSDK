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

#ifndef XDEVL_PLUGIN_IMPL_H
#define XDEVL_PLUGIN_IMPL_H

#include <map>

#include <XdevLPlatform.h>
#include <XdevLPlugin.h>
#include <XdevLModule.h>
#include <XdevLUtils.h>
#include <XdevLCoreMediator.h>
#include <XdevLError.h>
#include <stdlib.h>

namespace xdl {

	class XdevLModuleCreateParameter;

	/**
		@class XdevLClasssifierImpl
		@brief Implementation of the XdevLModule interface

	*/
	template<typename T>
	class XdevLClassifierImpl: public T {
		public:
			XdevLClassifierImpl(XdevLModuleCreateParameter* parameter) {
				if(parameter != nullptr) {
					m_id = parameter->getModuleId();
				}
			}
			virtual ~XdevLClassifierImpl() {};

			// --------------------------------------------------------------------------
			// XdevLClassifier function
			//
			virtual const XdevLID& getID() const {
				return this->m_id;
			}

		private:
			// Holds the module id.
			XdevLID	m_id;
	};

	/**
		@class XdevLListenerImpl
		@brief Implementation of the XdevLModule interface

	*/
	template<typename T>
	class XdevLListenerImpl: public XdevLClassifierImpl<T> {
		public:
			XdevLListenerImpl(XdevLModuleCreateParameter* parameter) :	XdevLClassifierImpl<T>(parameter) {
				if(parameter != nullptr) {
					m_core = parameter->getMediator();
				}
			}
			virtual ~XdevLListenerImpl() {};
			// --------------------------------------------------------------------------
			// XdevLListener functions
			//

			virtual xdl_int notify(XdevLEvent& event) {
				return ERR_OK;
			}

			/// Returns the pointer to the mediator
			XdevLCoreMediator* getMediator() const {
				return this->m_core;
			}

		private:
			// Holds the core mediator.
			XdevLCoreMediator* m_core;
	};

	/**
		@class XdevLModuleImpl
		@brief Implementation of the XdevLModule interface witch supports XDEVL_MESSAGE_INIT and XDEVL_MESSAGE_SHUTDOWN message
	*/
	template<typename T>
	class XdevLModuleImpl: public XdevLListenerImpl<T> {
		public:
			XdevLModuleImpl(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :	XdevLListenerImpl<T>(parameter),
                                                      m_descriptor(descriptor){

			}
			virtual ~XdevLModuleImpl() {};
      
			// --------------------------------------------------------------------------
			// XdevLModule function
			//
			virtual xdl_int init() {
				return ERR_OK;
			}

			virtual xdl_int shutdown() {
				return ERR_OK;
			}

			virtual void* getInternal(const XdevLInternalName& id) {
				return nullptr;
			}
      
      virtual XdevLModuleDescriptor& getDescriptor() {
        return m_descriptor;
      }
      
			// --------------------------------------------------------------------------
			// XdevLListener functions
			//

			virtual xdl_int notify(XdevLEvent& event) {

				/// Not an module event? Skip it.
				if(event.type != XDEVL_MODULE_EVENT) {
					return ERR_OK;
				}

				switch(event.module.event) {
					case XDEVL_MODULE_INIT:
						return init();
					case XDEVL_MODULE_SHUTDOWN:
						return shutdown();
				}

				return ERR_OK;
			}
  protected:
      XdevLModuleDescriptor m_descriptor;
	};

	/**
		@class XdevLModuleAutoImpl
		@brief Implementation of XdevLModule witch supports XDEVL_MESSAGE_UPDATE message
	*/
	template<typename T>
	class XdevLModuleAutoImpl: public XdevLModuleImpl<T> {
		public:
			XdevLModuleAutoImpl(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :	XdevLModuleImpl<T>(parameter, descriptor) {
			}
			virtual ~XdevLModuleAutoImpl() {};

			virtual xdl_int update() {
				return ERR_OK;
			}

			// --------------------------------------------------------------------------
			// XdevLListener functions
			//
			virtual xdl_int notify(XdevLEvent& event) {

				/// Not an module event? Skip it.
				if(event.type != XDEVL_MODULE_EVENT) {
					return ERR_OK;
				}

				if(XdevLModuleImpl<T>::notify(event) != ERR_OK) {
					return ERR_ERROR;
				}

				switch(event.module.event) {
					case XDEVL_MODULE_UPDATE:
						return update();
				}

				return ERR_OK;
			}
	};

}
#endif
