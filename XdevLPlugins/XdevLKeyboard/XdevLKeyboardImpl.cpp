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

// TODO Key values can get high. Array of button must be changed to std::map.

#include <iostream>
#include <map>

#include <XdevLTypes.h>
#include <XdevLEvent.h>
#include <XdevLCoreMediator.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLXstring.h>
#include <XdevLUtils.h>
#include "XdevLKeyboardImpl.h"


xdl::XdevLModuleDescriptor xdl::XdevLKeyboardImpl::m_keyboardModuleDesc {
	xdl::keyboard_vendor,
	xdl::keyboard_author,
	xdl::keyboard_moduleNames[0],
	xdl::keyboard_copyright,
	xdl::keyboardDescription,
	xdl::XdevLKeyboardMajorVersion,
	xdl::XdevLKeyboardMinorVersion,
	xdl::XdevLKeyboardPatchVersion
};

xdl::XdevLPluginDescriptor m_keyboardPluginDescriptor {
	xdl::keyboard_pluginName,
	xdl::keyboard_moduleNames,
	xdl::XdevLKeyboardPluginMajorVersion,
	xdl::XdevLKeyboardPluginMinorVersion,
	xdl::XdevLKeyboardPluginPatchVersion
};

extern "C" XDEVL_EXPORT xdl::xdl_int _init_plugin(xdl::XdevLPluginCreateParameter* parameter) {
	return xdl::ERR_OK;
}

extern "C" XDEVL_EXPORT xdl::xdl_int _shutdown_plugin() {
	return xdl::ERR_OK;
}

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter)  {
	// Is it the first module
	if(xdl::XdevLKeyboardImpl::m_keyboardModuleDesc.getName() == parameter->getModuleName()) {
		xdl::XdevLKeyboardImpl* obj = new xdl::XdevLKeyboardImpl(parameter);
		if(!obj)
			return xdl::ERR_ERROR;

		parameter->setModuleInstance(obj);
		return xdl::ERR_OK;
	}

	return xdl::ERR_MODULE_NOT_FOUND;
}

extern "C" XDEVL_EXPORT void _delete(xdl::XdevLModule* obj) {
	if(obj)
		delete obj;
}

extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor() {
	return &m_keyboardPluginDescriptor;
}


namespace xdl {

	XdevLKeyboardImpl::XdevLKeyboardImpl(XdevLModuleCreateParameter* parameter) : XdevlKeyboardBase<XdevLKeyboard>(parameter, m_keyboardModuleDesc) {
	}

	XdevLKeyboardImpl::~XdevLKeyboardImpl() {
	}

	xdl_int XdevLKeyboardImpl::init() {
		return XdevlKeyboardBase<XdevLKeyboard>::init();
	}

	xdl_int XdevLKeyboardImpl::shutdown() {
		return XdevlKeyboardBase<XdevLKeyboard>::shutdown();
	}

	xdl_int XdevLKeyboardImpl::registerDelegate(const XdevLString& id, const XdevLButtonIdDelegateType& delegate) {
		return XdevlKeyboardBase<XdevLKeyboard>::registerDelegate(id, delegate);
	}
	
	xdl_int XdevLKeyboardImpl::registerDelegate(const XdevLString& id, const XdevLAxisIdDelegateType& delegate) {
		return ERR_ERROR;
	}
	
	xdl_int XdevLKeyboardImpl::registerDelegate(const XdevLButtonDelegateType& delegate) {
		return XdevlKeyboardBase<XdevLKeyboard>::registerDelegate(delegate);
	}
	
	xdl_int XdevLKeyboardImpl::registerDelegate(const XdevLAxisDelegateType& delegate) {
		return ERR_ERROR;
	}

	int XdevLKeyboardImpl::attach(XdevLWindow* window) {
		return XdevlKeyboardBase<XdevLKeyboard>::attach(window, this->getDescriptor().getName().toString().c_str());
	}

	xdl_uint XdevLKeyboardImpl::getNumButtons() const {
		return 255;
	}

	xdl_uint XdevLKeyboardImpl::getNumAxis() const {
		return 0;
	}

	xdl_uint XdevLKeyboardImpl::getNumPOV() const {
		return 0;
	}

	xdl_int XdevLKeyboardImpl::getAxis(const xdl_uint, XdevLAxis** axis) const {
		*axis = nullptr;
		return ERR_ERROR;
	}
	xdl_int XdevLKeyboardImpl::getPOV(const xdl_uint, XdevLPOV** pov) const {
		*pov = nullptr;
		return ERR_ERROR;
	}

	xdl_int XdevLKeyboardImpl::getButton(const xdl_uint idx, XdevLButton** button)  {
		XDEVL_ASSERT(m_attached, "Keyboard device not attached!");

		// If the key mapping does not exists, just create one.
		XdevLButtonImpl* tmp = m_Buttons[idx];
		if(tmp == nullptr) {
			m_Buttons[idx] =  new XdevLButtonImpl(&m_mutex);
			tmp = m_Buttons[idx];
		}
		*button = tmp;
		return ERR_OK;
	}

	xdl_bool XdevLKeyboardImpl::getPressed(const xdl_uint key) {
		XDEVL_ASSERT(m_attached, "Keyboard device not attached!");
		return m_Buttons[key]->getPressed();
	}

	xdl_bool XdevLKeyboardImpl::getClicked(const xdl_uint key) {
		XDEVL_ASSERT(m_attached, "Keyboard device not attached!");
		return m_Buttons[key]->getClicked();
	}

	void XdevLKeyboardImpl::setClickResponseTimeForAll(xdl_double crt) {
		XDEVL_ASSERT(m_attached, "Keyboard device not attached!");
		for(auto a = 0; a < m_Buttons.size(); ++a) {
			m_Buttons[a]->setClickResponseTime(crt);
		}
	}

	void XdevLKeyboardImpl::setClickResponseTime(const xdl_uint key, xdl_double crt) {
		XDEVL_ASSERT(m_attached, "Keyboard device not attached!");
		m_Buttons[key]->setClickResponseTime(crt);
	}

	xdl_double XdevLKeyboardImpl::getClickResponseTime(const xdl_uint key) {
		XDEVL_ASSERT(m_attached, "Keyboard device not attached!");
		return m_Buttons[key]->getClickResponseTime();
	}

	void XdevLKeyboardImpl::setAxisRangeMinMax(const xdl_uint axis, xdl_float min, xdl_float max) {
		XDEVL_ASSERT(m_attached, "Keyboard device not attached!");
	}

	void XdevLKeyboardImpl::setAxisRangeMin(const xdl_uint axis, xdl_float min) {
		XDEVL_ASSERT(m_attached, "Keyboard device not attached!");
	}

	void XdevLKeyboardImpl::seAxisRangeMax(const xdl_uint axis, xdl_float max) {
		XDEVL_ASSERT(m_attached, "Keyboard device not attached!");
	}

	void XdevLKeyboardImpl::getAxisRangeMinMax(const xdl_uint axis, xdl_float* min, xdl_float* max) {
		XDEVL_ASSERT(m_attached, "Keyboard device not attached!");
	}

	xdl_float XdevLKeyboardImpl::getAxisRangeMin(const xdl_uint axis) const {
		XDEVL_ASSERT(m_attached, "Keyboard device not attached!");
		return 0.0f;
	}

	xdl_float XdevLKeyboardImpl::getAxisRangeMax(const xdl_uint axis) const {
		XDEVL_ASSERT(m_attached, "Keyboard device not attached!");
		return 0.0f;
	}
}
