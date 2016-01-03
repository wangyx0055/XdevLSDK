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

#include <XdevLCoreMediator.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLXstring.h>
#include <XdevLUtils.h>
#include "XdevLJoystickImpl.h"
#include <iostream>
#include <tinyxml.h>

xdl::XdevLModuleDescriptor joystickModuleDesc {
	xdl::joystick_vendor,
	xdl::joystick_author,
	xdl::joystick_moduleNames[0],
	xdl::joystick_copyright,
	xdl::joystick_description,
	xdl::XdevLJoystickMajorVersion,
	xdl::XdevLJoystickMinorVersion,
	xdl::XdevLJoystickPatchVersion
};

xdl::XdevLPluginDescriptor m_joystickPluginDescriptor {
	xdl::joystick_pluginName,
	xdl::joystick_moduleNames,
	xdl::XdevLJoystickPluginMajorVersion,
	xdl::XdevLJoystickPluginMinorVersion,
	xdl::XdevLJoystickPluginPatchVersion
};

//
// Initialize plugin function.
//
extern "C" XDEVL_EXPORT xdl::xdl_int _init_plugin(xdl::XdevLPluginCreateParameter* parameter) {
	return xdl::ERR_OK;
}

//
// Shutdown plugin.
//
extern "C" XDEVL_EXPORT xdl::xdl_int _shutdown_plugin() {
	
	return xdl::ERR_OK;
}

//
// Create Module function without XdevLCore.
//
extern "C" XDEVL_EXPORT xdl::XdevLModule* _createModule(const xdl::XdevLPluginDescriptor& pluginDescriptor, const xdl::XdevLModuleDescriptor& moduleDescriptor) {

	if(joystickModuleDesc.getName()  == moduleDescriptor.getName()) {
		xdl::XdevLJoystickImpl*obj = new xdl::XdevLJoystickImpl(nullptr);
		if(!obj) {
			return nullptr;
		}

		return obj;
	}

	return nullptr;
}

//
// Create module function using XdevLCore.
//
extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	if(joystickModuleDesc.getName() == parameter->getModuleName()) {
		xdl::XdevLJoystickImpl*obj = new xdl::XdevLJoystickImpl(parameter);
		if(!obj)
			return xdl::ERR_ERROR;
		parameter->setModuleInstance(obj);
		return xdl::ERR_OK;
	}
	return xdl::ERR_MODULE_NOT_FOUND;
}

//
// Delete module function.
//
extern "C" XDEVL_EXPORT void _delete(xdl::XdevLModule* obj) {
	if(obj)
		delete obj;
}

//
// Return plugin descriptor function.
//
extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor()  {
	return &m_joystickPluginDescriptor;
}


namespace xdl {

	XdevLJoystickImpl::XdevLJoystickImpl(XdevLModuleCreateParameter* parameter) :
		XdevLJoystickBase<XdevLJoystick>(parameter, joystickModuleDesc) {
	}

	xdl_int XdevLJoystickImpl::init() {
		return XdevLJoystickBase<XdevLJoystick>::init();
	}

	xdl_int XdevLJoystickImpl::create(const XdevLJoystickDeviceInfo& joystickDeviceInfo) {
		return XdevLJoystickBase<XdevLJoystick>::create(joystickDeviceInfo);
	}

	xdl_int XdevLJoystickImpl::registerDelegate(const XdevLString& id, const XdevLButtonIdDelegateType& delegate) {
		return XdevLJoystickBase<XdevLJoystick>::registerDelegate(id, delegate);
	}
	
	xdl_int XdevLJoystickImpl::registerDelegate(const XdevLString& id, const XdevLAxisIdDelegateType& delegate) {
		return XdevLJoystickBase<XdevLJoystick>::registerDelegate(id, delegate);
	}
	
	xdl_int XdevLJoystickImpl::registerDelegate(const XdevLButtonDelegateType& delegate) {
		return XdevLJoystickBase<XdevLJoystick>::registerDelegate(delegate);
	}
	
	xdl_int XdevLJoystickImpl::registerDelegate(const XdevLAxisDelegateType& delegate) {
		return XdevLJoystickBase<XdevLJoystick>::registerDelegate(delegate);
	}

	xdl_uint XdevLJoystickImpl::getNumButtons() const {
		return static_cast<xdl_uint>(m_Buttons.size());
	}

	xdl_uint XdevLJoystickImpl::getNumAxis() const {
		return static_cast<xdl_uint>(m_Axes.size());
	}

	xdl_uint XdevLJoystickImpl::getNumPOV() const {
		return static_cast<xdl_uint>(1);
	}

	xdl_int XdevLJoystickImpl::getButton(const xdl_uint idx, XdevLButton** button) {
		XDEVL_ASSERT(m_attached, "Joystick device not attached!");

		if(getNumButtons() == 0) {
			*button = NULL;
			return ERR_ERROR;
		}
		if((idx >= getNumButtons())) {
			*button = NULL;
			return ERR_ERROR;
		}
		*button = m_Buttons[idx];
		return ERR_OK;
	}

	xdl_int XdevLJoystickImpl::getAxis(const xdl_uint idx, XdevLAxis** axis) const {
		XDEVL_ASSERT(m_attached, "Joystick device not attached!");
		if(getNumAxis() == 0) {
			*axis = NULL;
			return ERR_ERROR;
		}
		if((idx >= getNumAxis())) {
			*axis = NULL;
			return ERR_ERROR;
		}
		*axis = m_Axes[idx];
		return ERR_OK;
	}

	xdl_int XdevLJoystickImpl::getPOV(const xdl_uint idx, XdevLPOV** pov) const {
		if(getNumAxis() == 0) {
			*pov = NULL;
			return ERR_ERROR;
		}
		if((idx >= getNumAxis())) {
			*pov = NULL;
			return ERR_ERROR;
		}
		*pov = m_POV;
		return ERR_OK;
	}


	xdl_bool XdevLJoystickImpl::getPressed(const xdl_uint key) {
		XDEVL_ASSERT(m_attached, "Joystick device not attached!");
		return m_Buttons[key]->getPressed();
	}

	xdl_bool XdevLJoystickImpl::getClicked(const xdl_uint key) {
		XDEVL_ASSERT(m_attached, "Joystick device not attached!");
		return m_Buttons[key]->getClicked();
	}

	void XdevLJoystickImpl::setClickResponseTimeForAll(xdl_double crt) {
		XDEVL_ASSERT(m_attached, "Joystick device not attached!");
		for(xdl_uint a = 0; a < m_Buttons.size(); ++a)
			m_Buttons[a]->setClickResponseTime(crt);
	}

	void XdevLJoystickImpl::setClickResponseTime(const xdl_uint key, xdl_double crt) {
		XDEVL_ASSERT(m_attached, "Joystick device not attached!");
		m_Buttons[key]->setClickResponseTime(crt);
	}

	xdl_double XdevLJoystickImpl::getClickResponseTime(const xdl_uint key) {
		XDEVL_ASSERT(m_attached, "Joystick device not attached!");
		return m_Buttons[key]->getClickResponseTime();
	}

	xdl_float XdevLJoystickImpl::getValue(const xdl_uint axis) {
		XDEVL_ASSERT(m_attached, "Joystick device not attached!");
		return m_Axes[axis]->getValue();
	}

	void XdevLJoystickImpl::setAxisRangeMinMax(const xdl_uint axis, xdl_float min, xdl_float max) {
		XDEVL_ASSERT(m_attached, "Joystick device not attached!");
		m_Axes[axis]->setMinMax(min, max);
	}

	void XdevLJoystickImpl::setAxisRangeMin(const xdl_uint axis, xdl_float min) {
		XDEVL_ASSERT(m_attached, "Joystick device not attached!");
		m_Axes[axis]->setMin(min);
	}

	void XdevLJoystickImpl::seAxisRangeMax(const xdl_uint axis, xdl_float max) {
		XDEVL_ASSERT(m_attached, "Joystick device not attached!");
		m_Axes[axis]->setMax(max);
	}

	void XdevLJoystickImpl::getAxisRangeMinMax(const xdl_uint axis, xdl_float* min, xdl_float* max) {
		XDEVL_ASSERT(m_attached, "Joystick device not attached!");
		m_Axes[axis]->getMinMax(min, max);
	}

	xdl_float XdevLJoystickImpl::getAxisRangeMin(const xdl_uint axis) const {
		XDEVL_ASSERT(m_attached, "Joystick device not attached!");
		return m_Axes[axis]->getMin();
	}

	xdl_float XdevLJoystickImpl::getAxisRangeMax(const xdl_uint axis) const {
		XDEVL_ASSERT(m_attached, "Joystick device not attached!");
		return m_Axes[axis]->getMax();
	}


}
