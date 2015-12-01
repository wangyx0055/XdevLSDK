/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2014 Cengiz Terzibas

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

#ifndef XDEVL_JOYSTICK_IMPL_H
#define XDEVL_JOYSTICK_IMPL_H

#include <XdevLPluginImpl.h>
#include <XdevLJoystick.h>
#include <XdevLListener.h>
#include <XdevLThread.h>
#include <XdevLMutex.h>
#include <XdevLConditionalVariable.h>
#include <XdevLInputSystemUtils.h>
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
		"(c) 2005 - 2014 Cengiz Terzibas."
	};
	static const XdevLString joystick_pluginName {
		"XdevLJoystick"
	};
	static const XdevLString joystick_description {
		"Gives you access to joystick devices elements."
	};
	static const std::vector<XdevLModuleName> joystick_moduleNames {
		XdevLModuleName("XdevLJoystick")
	};

	const xdl_uint  JOYSTICK_MAX_AXES			= 15;
	const xdl_uint	JOY_MAX_POV					= 4;
	const xdl_float JOYSTICK_MINMAX_AXIS_VALUE	= 32768.0f;
	const xdl_float JOYSTICK_MINMAX_DOUBLE_AXIS_VALUE = JOYSTICK_MINMAX_AXIS_VALUE*2.0f;

	typedef XdevLButtonImpl XdevLJoystickButtonImpl;
	typedef XdevLAxisImpl XdevLJoystickAxisImpl;
	typedef XdevLPOVImpl XdevLJoystickPOVImpl;






	template<typename T>
	class XdevLJoystickBase : public XdevLModuleImpl<T>, public thread::Thread {
		public:
			XdevLJoystickBase(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
				XdevLModuleImpl<T>(parameter, descriptor),
				m_POV(nullptr),
				m_joy_button_down(false),
				m_joy_moved(false),
				m_joy_curr_x(0),
				m_joy_curr_y(0),
				m_joy_old_x(0),
				m_joy_old_y(0),
				m_relativeMode(xdl_false),
				m_vendor(""),
				m_numAxis(4),		// TODO This value is a hack.
				m_numButtons(16),	// TODO This value is a hack.
				m_attached(false),
				m_threaded(false),
				m_sleep(0.001),
				m_windowWidth(0),
				m_windowHeight(0) {


				for(xdl_uint a = 0; a != JOYSTICK_MAX_AXES ; ++a) {
					m_center[a] = 0.0f;
				}

			}

			virtual ~XdevLJoystickBase() {}

		protected:
            using XdevLModuleImpl<T>::attach;

			xdl_int RunThread(thread::ThreadArgument* p_arg);
			xdl_int readJoystickXmlInfoPre(TiXmlDocument& document, const xdl_char* moduleName);
			xdl_int readJoystickXmlInfo(TiXmlDocument& document, const xdl_char* moduleName);
			xdl_int notify(XdevLEvent& event);
			xdl_int update();
			xdl_int init() override;
			xdl_int shutdown() override;
			xdl_int attach(XdevLWindow* window, const xdl_char* moduleName);
			xdl_bool getAttached();
			void setAttached(xdl_bool state);
			xdl_int reset();
			void initializeButtonsAndAxisArrays();

			xdl_int registerDelegate(const XdevLString& id, const XdevLButtonIdDelegateType& delegate);
			xdl_int registerDelegate(const XdevLString& id, const XdevLAxisIdDelegateType& delegate);
			xdl_int registerDelegate(const XdevLButtonDelegateType& delegate);
			xdl_int registerDelegate(const XdevLAxisDelegateType& delegate);

		protected:
			XdevLJoystickPOVImpl*	m_POV;
			xdl_bool				m_joy_button_down;
			xdl_bool				m_joy_moved;
			xdl_int					m_joy_curr_x;
			xdl_int					m_joy_curr_y;
			xdl_int					m_joy_old_x;
			xdl_int					m_joy_old_y;
			xdl_bool				m_relativeMode;
			std::string				m_vendor;
			xdl_uint 				m_numAxis;
			xdl_uint				m_numButtons;
			xdl_bool				m_attached;
			xdl_bool				m_threaded;
			xdl_double				m_sleep;
			thread::Mutex			m_mutex;
			xdl_uint				m_windowWidth;
			xdl_uint				m_windowHeight;
			xdl_float								m_center[JOYSTICK_MAX_AXES];
			std::vector<XdevLJoystickButtonImpl*> 	m_Buttons;
			std::vector<XdevLJoystickAxisImpl*>		m_Axes;
			thread::Mutex				m_replyMutex;
			thread::ConditionalVariable	m_replyCondVariable;
			std::vector<XdevLButtonDelegateType> m_buttonDelegates;
			std::vector<XdevLAxisDelegateType> m_axisDelegates;
			std::multimap<XdevLButtonId, XdevLButtonIdDelegateType> m_buttonIdDelegates;
			std::multimap<XdevLAxisId, XdevLAxisIdDelegateType> m_axisIdDelegates;
	};

	template<typename T>
	xdl_int  XdevLJoystickBase<T>::registerDelegate(const XdevLString& id, const XdevLAxisIdDelegateType& delegate) {
		XdevLAxisId idType = getAxisId(id);
		m_axisIdDelegates.insert(std::pair<const XdevLAxisId, const XdevLAxisIdDelegateType>(idType, delegate));
		for(auto d : m_axisIdDelegates) {
			std::cout << "Register Axis: " << d.first << std::endl;
		}
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
		if(m_numAxis > 0) {
			m_Axes.clear();
			m_Axes.reserve(m_numAxis);
			m_Axes.resize(m_numAxis);
			for(size_t a = 0; a < m_Axes.size(); ++a) {
				m_Axes[a] = new XdevLJoystickAxisImpl(&m_mutex);
				// FIXME: This is the middle position of the joystick. Cengiz find another solution for this really ugly hack.
				m_Axes[a]->setValue(0.5f);
			}
		}

		if(m_numButtons > 0) {
			m_Buttons.clear();
			m_Buttons.reserve(m_numButtons);
			m_Buttons.resize(m_numButtons);
			for(size_t a = 0; a < m_numButtons; ++a) {
				m_Buttons[a] = new XdevLJoystickButtonImpl(&m_mutex);
				m_Buttons[a]->setState(false);
			}
		}
	}
	template<typename T>
	xdl_bool XdevLJoystickBase<T>::getAttached() {
		xdl_bool tmp;
		m_mutex.Lock();
		tmp = m_attached;
		m_mutex.Unlock();
		return tmp;
	}

	template<typename T>
	void XdevLJoystickBase<T>::setAttached(xdl_bool state) {
		m_mutex.Lock();
		m_attached = state;
		m_mutex.Unlock();
	}

	template<typename T>
	xdl_int XdevLJoystickBase<T>::init() {

		return ERR_OK;
	}

	template<typename T>
	xdl_int XdevLJoystickBase<T>::shutdown() {
		XDEVL_MODULE_INFO("Starting shutdown process.\n");

		for(size_t a = 0; a < m_Buttons.size(); ++a)
			delete m_Buttons[a];
		for(size_t a = 0; a < m_Axes.size(); ++a)
			delete m_Axes[a];
		delete m_POV;

		m_Buttons.clear();
		m_Axes.clear();

		setAttached(false);

		// Wait unit the thread has finished his job.
		if(m_threaded)
			Join();
		XDEVL_MODULE_SUCCESS("Shutdown process was successful.\n");
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

		switch(event.type) {
			case XDEVL_JOYSTICK_RPLY_DEVICES_INFO: {
				std::cout <<
				          "Devices: " << event.jdeviceinfo.number_devices << std::endl <<
				          "Buttons: " << event.jdeviceinfo.number_buttons << std::endl <<
				          "Axis: " << event.jdeviceinfo.number_axis << std::endl;

				// TODO To do this here will lead to corrupt pointers aquired by getButton, getAxis...
				//  m_numAxis = event.jdeviceinfo.number_axis;
				//  m_numButtons = event.jdeviceinfo.number_buttons;
				//  initializeButtonsAndAxisArrays();

				m_replyCondVariable.signal();
			}
			break;
			case XDEVL_JOYSTICK_BUTTON_PRESSED: {
				xdl_int idx = event.button.button;
				if(idx > m_Buttons.size() - 1) {
					// Not supported button.
					break;
				}
				if(!m_joy_button_down) {
					m_Buttons[idx]->capturePressTime(event.common.timestamp);
				}
				m_joy_button_down = xdl_true;
				m_Buttons[idx]->setState(xdl_true);
				
				for(auto& delegate : m_buttonDelegates) {
					delegate(covertIdxToXdevLButton(idx), BUTTON_PRESSED);
				}
			}
			break;
			case XDEVL_JOYSTICK_BUTTON_RELEASED: {
				xdl_int idx = event.button.button;
				if(idx > m_Buttons.size() - 1) {
					// Not supported button.
					break;
				}
				if(m_joy_button_down)
					m_Buttons[idx]->captureReleaseTime(event.common.timestamp);

				m_joy_button_down = xdl_false;
				m_Buttons[idx]->setState(xdl_false);
				
				for(auto& delegate : m_buttonDelegates) {
					delegate(covertIdxToXdevLButton(idx), BUTTON_RELEASED);
				}
			}
			break;
			case XDEVL_JOYSTICK_MOTION: {
				if(event.jaxis.axis == 0) {
					m_joy_old_x = m_joy_curr_x;
					m_joy_curr_x = event.jaxis.value;
					m_Axes[AXIS_0]->setValue(((xdl_float)event.jaxis.value));
					m_joy_moved = true;
					for(auto& delegate : m_axisDelegates) {
						delegate(AXIS_0, m_Axes[AXIS_0]->getValue());
					}
				}

				if(event.jaxis.axis == 1) {
					m_joy_old_y = m_joy_curr_y;
					m_joy_curr_y = event.jaxis.value;
					m_Axes[AXIS_1]->setValue(((xdl_float)event.jaxis.value));
					m_joy_moved = true;
					for(auto& delegate : m_axisDelegates) {
						delegate(AXIS_1, m_Axes[AXIS_1]->getValue());
					}
				}

				if(event.jaxis.axis == 2) {
					m_Axes[AXIS_2]->setValue(((xdl_float)event.jaxis.value));
					m_joy_moved = true;
					for(auto& delegate : m_axisDelegates) {
						delegate(AXIS_2, m_Axes[AXIS_2]->getValue());
					}
				}

				if(event.jaxis.axis == 3) {
					m_Axes[AXIS_3]->setValue(((xdl_float)event.jaxis.value));
					m_joy_moved = true;
					for(auto& delegate : m_axisDelegates) {
						delegate(AXIS_3, m_Axes[AXIS_3]->getValue());
					}
				}
				m_Axes[AXIS_0]->setDeltaValue(0.0f);
				m_Axes[AXIS_1]->setDeltaValue(0.0f);
			}
			break;
			case XDEVL_JOYSTICK_POV: {
				m_POV->setDirection(event.jpov.direction);
			}
			break;
			default: {


			}
			break;
		}

		return ERR_OK;
	}


	template<typename T>
	xdl_int XdevLJoystickBase<T>::update() {
		m_mutex.Lock();
		m_joy_moved		= false;

		m_mutex.Unlock();

		return ERR_OK;
	}

	template<typename T>
	xdl_int XdevLJoystickBase<T>::RunThread(thread::ThreadArgument*) {
		XDEVL_MODULE_INFO("Starting threading mode.\n");
		while(getAttached()) {
			update();
			sleep(m_sleep);
		}
		return 0;
	}

	template<typename T>
	xdl_int XdevLJoystickBase<T>::attach(XdevLWindow* window, const xdl_char* moduleName) {
		if(!window) {
			std::cerr << "# -> Could not attach device to window. No vaild XdevLWindowDevice specified.\n";
			return ERR_ERROR;
		}

		if(getAttached()) {
			setAttached(false);
			Join();

		}



		if(this->getMediator()->getXmlFilename()) {
			TiXmlDocument xmlDocument;
			if(!xmlDocument.LoadFile(this->getMediator()->getXmlFilename())) {
				std::cerr << "Could not parse xml file: " << this->getMediator()->getXmlFilename() << std::endl;
				return ERR_ERROR;
			}

			if(readJoystickXmlInfoPre(xmlDocument, moduleName) != ERR_OK)
				return ERR_ERROR;
		}

		XdevLEvent event;
		event.jdeviceinfo.type = XDEVL_JOYSTICK_REQ_DEVICES_INFO;
		event.jdeviceinfo.sender = this->getID().getHashCode();
		event.jdeviceinfo.timestamp = this->getMediator()->getTime();
		this->getMediator()->fireEvent(event);
//
//		m_replyCondVariable.wait(m_replyMutex);
//		std::cout << "**********************" << std::endl;

		const XdevLWindowSize& size = window->getSize();

		m_windowWidth		= size.width;
		m_windowHeight	= size.height;

		initializeButtonsAndAxisArrays();

		m_POV = new XdevLJoystickPOVImpl(&m_mutex);

		if(this->getMediator()->getXmlFilename()) {
			TiXmlDocument xmlDocument;
			if(!xmlDocument.LoadFile(this->getMediator()->getXmlFilename())) {
				std::cerr << "Could not parse xml file: " << this->getMediator()->getXmlFilename() << std::endl;
				return ERR_ERROR;
			}

			if(readJoystickXmlInfo(xmlDocument, moduleName) != ERR_OK)
				return ERR_ERROR;
		}

		setAttached(true);

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

		if(root->Attribute("vendor")) {
			m_vendor = root->Attribute("vendor");
			XDEVL_MODULE_INFO("Request for Joystick Vendor: " << m_vendor << "\n");
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

		// Is there threading request?
		if(root->Attribute("thread")) {
			m_threaded = xstd::from_string<xdl_bool>(root->Attribute("thread"));
		}
		// Any sleep time information?
		if(root->Attribute("sleep")) {
			m_sleep = xstd::from_string<xdl_double>(root->Attribute("sleep"));
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
			XdevLJoystickImpl(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLJoystickImpl() {}

			static XdevLModuleDescriptor m_joystickModuleDesc;
			
			virtual xdl_int registerDelegate(const XdevLString& id, const XdevLButtonIdDelegateType& delegate);
			virtual xdl_int registerDelegate(const XdevLString& id, const XdevLAxisIdDelegateType& delegate);
			virtual xdl_int registerDelegate(const XdevLButtonDelegateType& delegate);
			virtual xdl_int registerDelegate(const XdevLAxisDelegateType& delegate);

			// --------------------------------------------------------------------------
			// XdevLKeyboard functions
			//
			virtual xdl_int attach(XdevLWindow* window);
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
