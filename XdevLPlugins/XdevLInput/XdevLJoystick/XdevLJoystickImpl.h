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

#ifndef XDEVL_JOYSTICK_IMPL_H
#define XDEVL_JOYSTICK_IMPL_H

#include <XdevLPluginImpl.h>
#include <XdevLListener.h>
#include <XdevLMutex.h>
#include <XdevLConditionalVariable.h>
#include <XdevLInputSystemUtils.h>
#include <XdevLJoystick.h>

#include <tinyxml.h>

namespace xdl {

	class XdevLCoreMediator;
	class XdevLModuleCreateParameter;


	// Holds the plugin major version.
	const xdl_uint XdevLJoystickPluginMajorVersion = XDEVLJOYSTICK_MAJOR_VERSION;

	// Holds the plugin minor version.
	const xdl_uint XdevLJoystickPluginMinorVersion = XDEVLJOYSTICK_MINOR_VERSION;

	// Holds the plugin patch version.
	const xdl_uint XdevLJoystickPluginPatchVersion = XDEVLJOYSTICK_PATCH_VERSION;


	// Holds the Major version number.
	const xdl_uint XdevLJoystickMajorVersion = XDEVLJOYSTICK_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLJoystickMinorVersion = XDEVLJOYSTICK_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLJoystickPatchVersion = XDEVLJOYSTICK_MODULE_PATCH_VERSION;


	static const XdevLString joystick_vendor {
		"www.codeposer.net"
	};
	static const XdevLString joystick_author {
		"Cengiz Terzibas"
	};
	static const XdevLString joystick_copyright {
		"(c) 2005 - 2016 Cengiz Terzibas."
	};
	static const XdevLString joystick_pluginName {
		"XdevLJoystick"
	};
	static const XdevLString joystick_description {
		"Class that helps you to handle Controller events with ease."
	};
	static const std::vector<XdevLModuleName> joystick_moduleNames {
		XdevLModuleName("XdevLJoystick")
	};

	typedef XdevLButtonImpl XdevLJoystickButtonImpl;
	typedef XdevLAxisImpl XdevLJoystickAxisImpl;
	typedef XdevLPOVImpl XdevLJoystickPOVImpl;


	const XdevLID JoystickButtonPressed("XDEVL_JOYSTICK_BUTTON_PRESSED");
	const XdevLID JoystickButtonReleased("XDEVL_JOYSTICK_BUTTON_RELEASED");
	const XdevLID JoystickMotion("XDEVL_JOYSTICK_MOTION");


	template<typename T>
	class XdevLJoystickBase : public XdevLModuleImpl<T> {
		public:
			XdevLJoystickBase(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
				XdevLModuleImpl<T>(parameter, descriptor),
				m_joystickServer(nullptr),
				m_joy_button_down(false),
				m_joy_moved(false),
				m_joy_curr_x(0),
				m_joy_curr_y(0),
				m_joy_old_x(0),
				m_joy_old_y(0),
				m_relativeMode(xdl_false) {
			}

			virtual ~XdevLJoystickBase() {}

		protected:
			xdl_int readJoystickXmlInfoPre(TiXmlDocument& document, const xdl_char* moduleName);
			xdl_int readJoystickXmlInfo(TiXmlDocument& document, const xdl_char* moduleName);
			xdl_int notify(XdevLEvent& event);
			xdl_int update();
			xdl_int init() override;
			xdl_int shutdown() override;
			xdl_bool getAttached();
			void setAttached(xdl_bool state);
			xdl_int reset();
			void initializeButtonsAndAxisArrays();

			xdl_int registerDelegate(const XdevLString& id, const XdevLButtonIdDelegateType& delegate);
			xdl_int registerDelegate(const XdevLString& id, const XdevLAxisIdDelegateType& delegate);
			xdl_int registerDelegate(const XdevLButtonDelegateType& delegate);
			xdl_int registerDelegate(const XdevLAxisDelegateType& delegate);

			xdl_int create(IPXdevLJoystickServer joystickServer, const XdevLJoystickId& joystickId);
			xdl_int useJoystick(const XdevLJoystickId& joystickId);

		protected:
			IPXdevLJoystickServer m_joystickServer;
			XdevLJoystickDeviceInfo m_joystickDeviceInfo;

			xdl_bool				m_joy_button_down;
			xdl_bool				m_joy_moved;
			xdl_int					m_joy_curr_x;
			xdl_int					m_joy_curr_y;
			xdl_int					m_joy_old_x;
			xdl_int					m_joy_old_y;
			xdl_bool				m_relativeMode;
			thread::Mutex		m_mutex;
			std::vector<XdevLJoystickButtonImpl*> m_Buttons;
			std::vector<XdevLJoystickAxisImpl*> m_Axes;
			std::vector<XdevLButtonDelegateType>										m_buttonDelegates;
			std::vector<XdevLAxisDelegateType>											m_axisDelegates;
			std::multimap<XdevLButtonId, XdevLButtonIdDelegateType> m_buttonIdDelegates;
			std::multimap<XdevLAxisId, XdevLAxisIdDelegateType>			m_axisIdDelegates;
	};

	template<typename T>
	xdl_int  XdevLJoystickBase<T>::registerDelegate(const XdevLString& id, const XdevLAxisIdDelegateType& delegate) {
		XdevLAxisId idType = getAxisId(id);
		m_axisIdDelegates.insert(std::pair<const XdevLAxisId, const XdevLAxisIdDelegateType>(idType, delegate));
		return ERR_OK;
	}

	template<typename T>
	xdl_int  XdevLJoystickBase<T>::registerDelegate(const XdevLString& id, const XdevLButtonIdDelegateType& delegate) {
		m_buttonIdDelegates.insert(std::pair<const XdevLButtonId, const XdevLButtonIdDelegateType>(getButtonId(id), delegate));
		return ERR_OK;
	}

	template<typename T>
	xdl_int  XdevLJoystickBase<T>::registerDelegate(const XdevLButtonDelegateType& delegate) {
		m_buttonDelegates.push_back(delegate);
		return ERR_OK;
	}

	template<typename T>
	xdl_int  XdevLJoystickBase<T>::registerDelegate(const XdevLAxisDelegateType& delegate) {
		m_axisDelegates.push_back(delegate);
		return ERR_OK;
	}

	template<typename T>
	void XdevLJoystickBase<T>::initializeButtonsAndAxisArrays() {

		if(m_joystickDeviceInfo.numberOfAxes > 0) {
			// Delete previous one.
			for(auto& axis : m_Axes) {
				delete axis;
			}

			// Reset and reserver new array of axis.
			m_Axes.clear();
			m_Axes.reserve(m_joystickDeviceInfo.numberOfAxes);
			m_Axes.resize(m_joystickDeviceInfo.numberOfAxes);
			for(size_t a = 0; a < m_Axes.size(); ++a) {
				m_Axes[a] = new XdevLJoystickAxisImpl(&m_mutex);
				m_Axes[a]->setMinMax(-1.0f, 1.0f);
			}
		}


		if(m_joystickDeviceInfo.numberOfButtons > 0) {
			// Delete previous one.
			for(auto& button : m_Buttons) {
				delete button;
			}

			// Reset and reserver new array of buttons.
			m_Buttons.clear();
			m_Buttons.reserve(m_joystickDeviceInfo.numberOfButtons);
			m_Buttons.resize(m_joystickDeviceInfo.numberOfButtons);
			for(size_t a = 0; a < m_Buttons.size(); ++a) {
				m_Buttons[a] = new XdevLJoystickButtonImpl(&m_mutex);
				m_Buttons[a]->setState(false);
			}
		}
	}

	template<typename T>
	xdl_int XdevLJoystickBase<T>::create(IPXdevLJoystickServer joystickServer, const XdevLJoystickId& joystickId) {

		if(nullptr == joystickServer) {
			return ERR_ERROR;
		}

		m_joystickServer = joystickServer;

		return useJoystick(joystickId);

	}

	template<typename T>
	xdl_int XdevLJoystickBase<T>::useJoystick(const XdevLJoystickId& joystickId) {
		XDEVL_ASSERT(m_joystickServer, "Joystick not created. Use the create method first.");

		if(m_joystickServer->getJoystickInfo(joystickId, m_joystickDeviceInfo) == ERR_ERROR) {
			return ERR_ERROR;
		}

		initializeButtonsAndAxisArrays();

		return ERR_OK;
	}


	template<typename T>
	xdl_int XdevLJoystickBase<T>::init() {

		if(this->getMediator()->getXmlFilename()) {
			TiXmlDocument xmlDocument;
			if(!xmlDocument.LoadFile(this->getMediator()->getXmlFilename())) {
				XDEVL_MODULE_WARNING("Could not parse xml file: " << this->getMediator()->getXmlFilename() << "\n");
			}
		}

		return ERR_OK;
	}

	template<typename T>
	xdl_int XdevLJoystickBase<T>::shutdown() {

		for(size_t a = 0; a < m_Buttons.size(); ++a)
			delete m_Buttons[a];
		for(size_t a = 0; a < m_Axes.size(); ++a)
			delete m_Axes[a];

		m_Buttons.clear();
		m_Axes.clear();

		return ERR_OK;
	}

	template<typename T>
	xdl_int XdevLJoystickBase<T>::reset() {
		for(size_t a = 0; a < m_Buttons.size(); ++a)
			m_Buttons[a]->setState(false);
		for(size_t a = 0; a < m_Axes.size(); ++a)
			m_Axes[a]->setValue(0.5f);

		return ERR_OK;
	}

	template<typename T>
	xdl_int XdevLJoystickBase<T>::notify(XdevLEvent& event) {

		if(event.type == JoystickButtonPressed.getHashCode()) {
			xdl_int idx = event.jbutton.buttonid;
			if(idx > m_Buttons.size() - 1) {
				// Not supported button.
				return ERR_OK;
			}
			if(!m_joy_button_down) {
				m_Buttons[idx]->capturePressTime(event.common.timestamp);
			}
			m_joy_button_down = xdl_true;
			m_Buttons[idx]->setState(xdl_true);

			//
			// Handle delegates that sends a button id's and the state of the button.
			//
			for(auto& delegate : m_buttonDelegates) {
				delegate(covertIdxToXdevLButton(idx), BUTTON_PRESSED);
			}

			//
			// Handle delegates that registered only for one specific button id.
			//
			auto pp = m_buttonIdDelegates.equal_range((XdevLButtonId)idx);
			for(auto it = pp.first; it != pp.second; ++it) {
				auto delegate = it->second;
				delegate(BUTTON_PRESSED);
			}

		} else if(event.type ==  JoystickButtonReleased.getHashCode()) {
			xdl_int idx = event.jbutton.buttonid;
			if(idx > m_Buttons.size() - 1) {
				// Not supported button.
				return ERR_OK;
			}
			if(m_joy_button_down)
				m_Buttons[idx]->captureReleaseTime(event.common.timestamp);

			m_joy_button_down = xdl_false;
			m_Buttons[idx]->setState(xdl_false);

			//
			// Handle delegates that sends a button id's and the state of the button.
			//
			for(auto& delegate : m_buttonDelegates) {
				delegate(covertIdxToXdevLButton(idx), BUTTON_RELEASED);
			}

			//
			// Handle delegates that registered only for one specific button id.
			//
			auto pp = m_buttonIdDelegates.equal_range((XdevLButtonId)idx);
			for(auto it = pp.first; it != pp.second; ++it) {
				auto delegate = it->second;
				delegate(BUTTON_RELEASED);
			}

		} else if(event.type == JoystickMotion.getHashCode()) {
			xdl_float value = ((xdl_float)event.jaxis.value)/32768.0f;

			m_joy_old_x = m_joy_curr_x;
			m_joy_curr_x = value;
			m_Axes[event.jaxis.axisid]->setValue(value);
			m_joy_moved = true;

			//
			// Handle delegates that sends a axis id's and the value of the axis.
			//
			for(auto& delegate : m_axisDelegates) {
				delegate((XdevLAxisId)event.jaxis.axisid, m_Axes[event.jaxis.axisid]->getValue());
			}

			//
			// Handle delegates that registered only for one specific axis id.
			//
			auto pp = m_axisIdDelegates.equal_range((XdevLAxisId)event.jaxis.axisid);
			for(auto it = pp.first; it != pp.second; ++it) {
				auto delegate = it->second;
				delegate(m_Axes[event.jaxis.axisid]->getValue());
			}

		}

		return XdevLModuleImpl<T>::notify(event);
	}


	template<typename T>
	xdl_int XdevLJoystickBase<T>::update() {
		m_mutex.Lock();
		m_joy_moved		= false;

		m_mutex.Unlock();

		return ERR_OK;
	}

	template<typename T>
	xdl_int XdevLJoystickBase<T>::readJoystickXmlInfoPre(TiXmlDocument& document, const xdl_char* moduleName) {
		TiXmlHandle docHandle(&document);
		TiXmlElement* root = docHandle.FirstChild(XdevLCorePropertiesName.c_str()).FirstChildElement(moduleName).ToElement();
		if(!root) {
			XDEVL_MODULE_WARNING("<XdevLJoystick> section not found. Using default values for the device.\n");
			return ERR_OK;
		}
		if(root->Attribute("id")) {
			XdevLID id(root->Attribute("id"));
			if(this->getID() != id) {
				return ERR_OK;
			}
		} else {
			XDEVL_MODULE_ERROR("No 'id' attribute specified.");
			return ERR_ERROR;
		}

		return ERR_OK;
	}

	template<typename T>
	xdl_int XdevLJoystickBase<T>::readJoystickXmlInfo(TiXmlDocument& document, const xdl_char* moduleName) {
		TiXmlHandle docHandle(&document);
		TiXmlElement* root = docHandle.FirstChild(XdevLCorePropertiesName.c_str()).FirstChildElement(moduleName).ToElement();
		if(!root) {
			XDEVL_MODULE_WARNING("<XdevLJoystick> section not found. Using default values for the device.\n");
			return ERR_OK;
		}
		if(root->Attribute("id")) {
			XdevLID id(root->Attribute("id"));
			if(this->getID() != id) {
				return ERR_OK;
			}
		} else {
			XDEVL_MODULE_ERROR("No 'id' attribute specified.");
			return ERR_ERROR;
		}

		// If there is a crt attribute, set this value for all buttons.
		if(root->Attribute("crt")) {
			xdl_double crt = xstd::from_string<xdl_int>(root->Attribute("crt"));
			for(size_t a = 0; a < m_Buttons.size(); ++a)
				m_Buttons[a]->setClickResponseTime(crt);
		}

		// If there is a min attribute, set this value for all axes.
		if(root->Attribute("min")) {
			xdl_float min = xstd::from_string<xdl_float>(root->Attribute("min"));
			for(size_t a = 0; a < m_Axes.size(); ++a)
				m_Axes[a]->setMin(min);
		}

		// If there is a max attribute, set this value for all axes.
		if(root->Attribute("max")) {
			xdl_float max = xstd::from_string<xdl_float>(root->Attribute("max"));
			for(size_t a = 0; a < m_Axes.size(); ++a)
				m_Axes[a]->setMax(max);
		}

		TiXmlElement* child = 0;
		for(child = root->FirstChildElement(); child; child = child->NextSiblingElement()) {
			if(child->ValueTStr() == "Range") {
				xdl_int axis = 0;
				if(child->Attribute("axis")) {
					axis = getAxisId(STRING(child->Attribute("axis")));
					float value = 0.0f;
					if(child->Attribute("min")) {
						value = xstd::from_string<float>(child->Attribute("min"));
						if(NULL != m_Axes[axis])
							m_Axes[axis]->setMin(value);
						else {
							XDEVL_MODULE_WARNING("Axis: " << axis << " 'min' value can not be set.\n");
						}
					}
					if(child->Attribute("max")) {
						value = xstd::from_string<float>(child->Attribute("max"));
						if(NULL != m_Axes[axis])
							m_Axes[axis]->setMax(value);
						else {
							XDEVL_MODULE_WARNING("Axis: " << axis << " 'max' value can not be set.\n");
						}

					}
				} else {
					XDEVL_MODULE_ERROR("No axis attribute in <Range> element found. Please specify an axis id.\n");
					return ERR_ERROR;
				}
			}

			if(child->ValueTStr() == "ClickResponseTime") {
				xdl_int button = 0;
				if(child->Attribute("button")) {
					button = getButtonId(STRING(child->Attribute("button")));
					if(child->Attribute("value"))
						m_Buttons[button]->setClickResponseTime(xstd::from_string<float>(child->Attribute("value")));
				} else {
					XDEVL_MODULE_ERROR("No button attribute in <ClickResponseTime> element found. Please specify a button id.\n");
					return ERR_ERROR;
				}
			}
		}

		return ERR_OK;
	}


	/**
		@class XdevLJoystickImpl
		@brief Implementation class for the XXdevLJoystick interface
		@author Cengiz Terzibas
	*/
	class XdevLJoystickImpl : public XdevLJoystickBase<XdevLJoystick> {
		public:
			XdevLJoystickImpl(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor);
			virtual ~XdevLJoystickImpl() {}

			virtual xdl_int init() override;
			virtual xdl_int create(IPXdevLJoystickServer joystickServer, const XdevLJoystickId& joystickId = XdevLJoystickId::JOYSTICK_DEFAULT) override;
			virtual xdl_int useJoystick(const XdevLJoystickId& joystickId) override;

			virtual xdl_int registerDelegate(const XdevLString& id, const XdevLButtonIdDelegateType& delegate);
			virtual xdl_int registerDelegate(const XdevLString& id, const XdevLAxisIdDelegateType& delegate);
			virtual xdl_int registerDelegate(const XdevLButtonDelegateType& delegate);
			virtual xdl_int registerDelegate(const XdevLAxisDelegateType& delegate);

			// --------------------------------------------------------------------------
			// XdevLEventCatalystFactory functions
			//
			virtual xdl_uint getNumButtons() const;
			virtual xdl_int getButton(const xdl_uint idx, XdevLButton** button);
			virtual xdl_uint getNumAxis() const;
			virtual xdl_int getAxis(const xdl_uint idx, XdevLAxis** axis) const;
			virtual xdl_uint getNumPOV() const;
			virtual xdl_int getPOV(const xdl_uint idx, XdevLPOV** pov) const;

			virtual xdl_bool getPressed(const xdl_uint key);
			virtual xdl_bool getClicked(const xdl_uint key);
			virtual void setClickResponseTimeForAll(xdl_double crt);
			virtual void setClickResponseTime(const xdl_uint key, xdl_double crt);
			virtual xdl_double getClickResponseTime(const xdl_uint key);

			virtual xdl_float getValue(const xdl_uint button);

			virtual void setAxisRangeMinMax(const xdl_uint axis, xdl_float min, xdl_float max);
			virtual void setAxisRangeMin(const xdl_uint axis, xdl_float min);
			virtual void seAxisRangeMax(const xdl_uint axis, xdl_float max);
			virtual void getAxisRangeMinMax(const xdl_uint axis, xdl_float* min, xdl_float* max);
			virtual xdl_float getAxisRangeMin(const xdl_uint axis) const;
			virtual xdl_float getAxisRangeMax(const xdl_uint axis) const;
	};
}


#endif
