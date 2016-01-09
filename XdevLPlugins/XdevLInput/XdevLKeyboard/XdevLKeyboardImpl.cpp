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

xdl::XdevLPluginDescriptor keyboardPluginDescriptor {
	xdl::keyboard_pluginName,
	xdl::keyboard_moduleNames,
	xdl::XdevLKeyboardPluginMajorVersion,
	xdl::XdevLKeyboardPluginMinorVersion,
	xdl::XdevLKeyboardPluginPatchVersion
};

XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(keyboardPluginDescriptor);

XDEVL_PLUGIN_CREATE_MODULE {

	if(xdl::XdevLKeyboardImpl::m_keyboardModuleDesc.getName() == XDEVL_MODULE_PARAMETER_NAME ) {

		xdl::IPXdevLModule module = XDEVL_NEW_MODULE(xdl::XdevLKeyboardImpl,  XDEVL_MODULE_PARAMETER);
		XDEVL_MODULE_SET_MODULE_INSTACE(module);		

		return xdl::ERR_OK;
	}

	return xdl::ERR_MODULE_NOT_FOUND;
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
