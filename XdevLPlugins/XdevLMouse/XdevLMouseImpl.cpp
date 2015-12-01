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

#include <iostream>
#include <string>
#include <algorithm>

#include <XdevLPlatform.h>
#include <XdevLPluginImpl.h>
#include <XdevLCoreMediator.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLXstring.h>
#include <XdevLUtils.h>
#include "XdevLMouseImpl.h"

xdl::XdevLModuleDescriptor xdl::XdevLMouseImpl::m_moduleDescriptor {
	xdl::mouse_vendor,
	xdl::mouse_author,
	xdl::mouse_moduleNames[0],
	xdl::mouse_copyright,
	xdl::mouseDescription,
	xdl::XdevLMouseMajorVersion,
	xdl::XdevLMouseMinorVersion,
	xdl::XdevLMousePatchVersion
};

xdl::XdevLPluginDescriptor m_mousePluginDescriptor {
	xdl::mouse_pluginName,
	xdl::mouse_moduleNames,
	xdl::XdevLMousePluginMajorVersion,
	xdl::XdevLMousePluginMinorVersion,
	xdl::XdevLMousePluginPatchVersion
};

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	if(xdl::XdevLMouseImpl::m_moduleDescriptor.getName() == parameter->getModuleName()) {
		xdl::XdevLMouseImpl*obj = new xdl::XdevLMouseImpl(parameter);
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
	return &m_mousePluginDescriptor;
}


namespace xdl {


	XdevLMouseImpl::XdevLMouseImpl(XdevLModuleCreateParameter* parameter) :
		XdevLMouseBase<XdevLMouse>(parameter, m_moduleDescriptor) {
	}
	
	xdl_int XdevLMouseImpl::registerDelegate(const XdevLString& id, const XdevLButtonIdDelegateType& delegate) {
	  return XdevLMouseBase<XdevLMouse>::registerDelegate(id, delegate);
	}

	xdl_int XdevLMouseImpl::registerDelegate(const XdevLString& id, const XdevLAxisIdDelegateType& delegate) {
		return XdevLMouseBase<XdevLMouse>::registerDelegate(id, delegate);
	}
	
	xdl_int XdevLMouseImpl::registerDelegate(const XdevLButtonDelegateType& delegate) {
		return XdevLMouseBase<XdevLMouse>::registerDelegate(delegate);
	}
	
	xdl_int XdevLMouseImpl::registerDelegate(const XdevLAxisDelegateType& delegate) {
		return XdevLMouseBase<XdevLMouse>::registerDelegate(delegate);
	}

	xdl_int XdevLMouseImpl::init() {
		return XdevLMouseBase<XdevLMouse>::init();
	}

	xdl_int XdevLMouseImpl::shutdown() {
		return XdevLMouseBase<XdevLMouse>::shutdown();
	}


	xdl_int XdevLMouseImpl::attach(XdevLWindow* window) {
		std::string moduleName = this->getDescriptor().getName().toString();
		return XdevLMouseBase<XdevLMouse>::attach(window, moduleName.c_str());
	}

	xdl_uint XdevLMouseImpl::getNumButtons() const {
		return static_cast<xdl_int>(m_Buttons.size());
	}

	xdl_int XdevLMouseImpl::getButton(const xdl_uint idx, XdevLButton** button) {
		XDEVL_ASSERT(m_attached, "Mouse Device not attached");

		if(m_Buttons.size() == 0) {
			*button = NULL;
			return ERR_ERROR;
		}
		if((idx >= m_Buttons.size())) {
			*button = NULL;
			return ERR_ERROR;
		}
		*button = m_Buttons[idx];
		return ERR_OK;
	}

	xdl_uint XdevLMouseImpl::getNumAxis() const {
		XDEVL_ASSERT(m_attached, "Mouse Device not attached");
		return static_cast<xdl_int>(m_Axes.size());
	}

	xdl_int XdevLMouseImpl::getAxis(const xdl_uint idx, XdevLAxis** axis) const {
		XDEVL_ASSERT(m_attached, "Mouse Device not attached");

		if(m_Axes.size() == 0) {
			*axis = NULL;
			return ERR_ERROR;
		}
		if((idx >= m_Axes.size())) {
			*axis = NULL;
			return ERR_ERROR;
		}
		*axis = m_Axes[idx];
		return ERR_OK;
	}

	xdl_uint XdevLMouseImpl::getNumPOV() const {
		XDEVL_ASSERT(m_attached, "Mouse Device not attached");
		return 0;
	}

	xdl_int XdevLMouseImpl::getPOV(const xdl_uint, XdevLPOV** pov) const {
		XDEVL_ASSERT(m_attached, "Mouse Device not attached");
		*pov = NULL;
		return ERR_ERROR;
	}

	xdl_bool XdevLMouseImpl::getPressed(const xdl_uint key) {
		XDEVL_ASSERT(m_attached, "Mouse Device not attached");
		return m_Buttons[key]->getPressed();
	}

	xdl_bool XdevLMouseImpl::getClicked(const xdl_uint key) {
		XDEVL_ASSERT(m_attached, "Mouse Device not attached");
		return m_Buttons[key]->getClicked();
	}

	void XdevLMouseImpl::setClickResponseTimeForAll(xdl_double crt) {
		XDEVL_ASSERT(m_attached, "Mouse Device not attached");
		for(xdl_uint a = 0; a < m_Buttons.size(); ++a)
			m_Buttons[a]->setClickResponseTime(crt);
	}

	void XdevLMouseImpl::setClickResponseTime(const xdl_uint key, xdl_double crt) {
		XDEVL_ASSERT(m_attached, "Mouse Device not attached");
		m_Buttons[key]->setClickResponseTime(crt);
	}

	xdl_double XdevLMouseImpl::getClickResponseTime(const xdl_uint key) {
		XDEVL_ASSERT(m_attached, "Mouse Device not attached");
		return m_Buttons[key]->getClickResponseTime();
	}

	xdl_float XdevLMouseImpl::getValue(const xdl_uint axis) {
		XDEVL_ASSERT(m_attached, "Mouse Device not attached");
		return m_Axes[axis]->getValue();
	}

	void XdevLMouseImpl::setAxisRangeMinMax(const xdl_uint axis, xdl_float min, xdl_float max) {
		XDEVL_ASSERT(m_attached, "Mouse Device not attached");
		m_Axes[axis]->setMinMax(min, max);
	}

	void XdevLMouseImpl::setAxisRangeMin(const xdl_uint axis, xdl_float min) {
		XDEVL_ASSERT(m_attached, "Mouse Device not attached");
		m_Axes[axis]->setMin(min);
	}

	void XdevLMouseImpl::seAxisRangeMax(const xdl_uint axis, xdl_float max) {
		XDEVL_ASSERT(m_attached, "Mouse Device not attached");
		m_Axes[axis]->setMax(max);
	}

	void XdevLMouseImpl::getAxisRangeMinMax(const xdl_uint axis, xdl_float* min, xdl_float* max) {
		XDEVL_ASSERT(m_attached, "Mouse Device not attached");
		m_Axes[axis]->getMinMax(min, max);
	}

	xdl_float XdevLMouseImpl::getAxisRangeMin(const xdl_uint axis) const {
		XDEVL_ASSERT(m_attached, "Mouse Device not attached");
		return m_Axes[axis]->getMin();
	}

	xdl_float XdevLMouseImpl::getAxisRangeMax(const xdl_uint axis) const {
		XDEVL_ASSERT(m_attached, "Mouse Device not attached");
		return m_Axes[axis]->getMax();
	}

	void  XdevLMouseImpl::setRelativeMode(xdl_bool state) {
		m_relativeMode = state;
		if(state)
			m_window->hidePointer();
		else
			m_window->showPointer();
	}


}
