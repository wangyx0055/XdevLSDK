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

			void setID(const XdevLID& id) {
				m_id = id;
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
			XdevLListenerImpl(XdevLModuleCreateParameter* parameter) :	XdevLClassifierImpl<T>(parameter), m_core(nullptr) {
				if(parameter != nullptr) {
					m_core = parameter->getMediator();
				}
			}
			virtual ~XdevLListenerImpl() {};
			// --------------------------------------------------------------------------
			// XdevLListener functions
			//
			/// Returns the pointer to the mediator
			XdevLCoreMediator* getMediator() const {
				return this->m_core;
			}

			void setMediator(XdevLCoreMediator* core) {
				m_core = core;
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
				m_descriptor(descriptor) {

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

				return XdevLListenerImpl<T>::notify(event);
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
