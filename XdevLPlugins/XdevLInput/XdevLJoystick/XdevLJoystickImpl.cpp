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

XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(m_joystickPluginDescriptor);

XDEVL_PLUGIN_CREATE_MODULE {
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLJoystickImpl, joystickModuleDesc)
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}

XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DEFINITION(XdevLJoystick, xdl::XdevLJoystickImpl, joystickModuleDesc)

namespace xdl {

	XdevLJoystickImpl::XdevLJoystickImpl(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		XdevLJoystickBase<XdevLJoystick>(parameter, descriptor) {
	}

	xdl_int XdevLJoystickImpl::init() {
		return XdevLJoystickBase<XdevLJoystick>::init();
	}

	xdl_int XdevLJoystickImpl::create(IPXdevLJoystickServer joystickServer, const XdevLJoystickId& joystickId) {
		return XdevLJoystickBase<XdevLJoystick>::create(joystickServer, joystickId);
	}
	
	xdl_int XdevLJoystickImpl::useJoystick(const XdevLJoystickId& joystickId) {
		return XdevLJoystickBase<XdevLJoystick>::useJoystick(joystickId);
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
		*pov = nullptr;
		return ERR_OK;
	}


	xdl_bool XdevLJoystickImpl::getPressed(const xdl_uint key) {
		return m_Buttons[key]->getPressed();
	}

	xdl_bool XdevLJoystickImpl::getClicked(const xdl_uint key) {
		return m_Buttons[key]->getClicked();
	}

	void XdevLJoystickImpl::setClickResponseTimeForAll(xdl_double crt) {
		for(xdl_uint a = 0; a < m_Buttons.size(); ++a)
			m_Buttons[a]->setClickResponseTime(crt);
	}

	void XdevLJoystickImpl::setClickResponseTime(const xdl_uint key, xdl_double crt) {
		m_Buttons[key]->setClickResponseTime(crt);
	}

	xdl_double XdevLJoystickImpl::getClickResponseTime(const xdl_uint key) {
		return m_Buttons[key]->getClickResponseTime();
	}

	xdl_float XdevLJoystickImpl::getValue(const xdl_uint axis) {
		return m_Axes[axis]->getValue();
	}

	void XdevLJoystickImpl::setAxisRangeMinMax(const xdl_uint axis, xdl_float min, xdl_float max) {
		m_Axes[axis]->setMinMax(min, max);
	}

	void XdevLJoystickImpl::setAxisRangeMin(const xdl_uint axis, xdl_float min) {
		m_Axes[axis]->setMin(min);
	}

	void XdevLJoystickImpl::seAxisRangeMax(const xdl_uint axis, xdl_float max) {
		m_Axes[axis]->setMax(max);
	}

	void XdevLJoystickImpl::getAxisRangeMinMax(const xdl_uint axis, xdl_float* min, xdl_float* max) {
		m_Axes[axis]->getMinMax(min, max);
	}

	xdl_float XdevLJoystickImpl::getAxisRangeMin(const xdl_uint axis) const {
		return m_Axes[axis]->getMin();
	}

	xdl_float XdevLJoystickImpl::getAxisRangeMax(const xdl_uint axis) const {
		return m_Axes[axis]->getMax();
	}


}
