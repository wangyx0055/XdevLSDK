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

xdl::XdevLModuleDescriptor moduleMouseDescriptor {
	xdl::mouse_vendor,
	xdl::mouse_author,
	xdl::mouse_moduleNames[0],
	xdl::mouse_copyright,
	xdl::mouseDescription,
	xdl::XdevLMouseMajorVersion,
	xdl::XdevLMouseMinorVersion,
	xdl::XdevLMousePatchVersion
};

xdl::XdevLPluginDescriptor mousePluginDescriptor {
	xdl::mouse_pluginName,
	xdl::mouse_moduleNames,
	xdl::XdevLMousePluginMajorVersion,
	xdl::XdevLMousePluginMinorVersion,
	xdl::XdevLMousePluginPatchVersion
};

XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(mousePluginDescriptor)

XDEVL_PLUGIN_CREATE_MODULE {
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLMouseImpl, moduleMouseDescriptor)
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}

XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DEFINITION(XdevLMouse, xdl::XdevLMouseImpl, moduleMouseDescriptor)

namespace xdl {


	XdevLMouseImpl::XdevLMouseImpl(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		XdevLMouseBase<XdevLMouse>(parameter, descriptor) {
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

	xdl_int XdevLMouseImpl::update() {
		return XdevLMouseBase<XdevLMouse>::update();
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

	xdl_float XdevLMouseImpl::getDeltaValue(const xdl_uint axis) {
		XDEVL_ASSERT(m_attached, "Mouse Device not attached");
		return m_Axes[axis]->getDeltaValue();
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

}
