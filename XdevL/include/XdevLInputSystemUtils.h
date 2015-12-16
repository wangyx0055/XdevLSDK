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

#ifndef XDEVL_EVENT_CATALYST_IMPL_H
#define XDEVL_EVENT_CATALYST_IMPL_H

#include <XdevLTimer.h>
#include <XdevLInputSystem.h>
#include <XdevLCoreMediator.h>
#include <XdevLMutex.h>
#include <XdevLError.h>
#include <cmath>

namespace xdl {

	XdevLButtonId getButtonId(const XdevLString& str) {
		if(str == STRING("BUTTON_0") || str == STRING("BUTTON_LEFT"))
			return BUTTON_0;
		if(str == STRING("BUTTON_1") || str == STRING("BUTTON_RIGHT"))
			return BUTTON_1;
		if(str == STRING("BUTTON_2") || str == STRING("BUTTON_MIDDLE"))
			return BUTTON_2;
		if(str == STRING("BUTTON_3"))
			return BUTTON_3;
		if(str == STRING("BUTTON_4"))
			return BUTTON_4;
		if(str == STRING("BUTTON_5"))
			return BUTTON_5;
		if(str == STRING("BUTTON_6"))
			return BUTTON_6;
		if(str == STRING("BUTTON_7"))
			return BUTTON_7;
		if(str == STRING("BUTTON_8"))
			return BUTTON_8;
		if(str == STRING("BUTTON_9"))
			return BUTTON_9;
		if(str == STRING("BUTTON_10"))
			return BUTTON_10;
		if(str == STRING("BUTTON_11"))
			return BUTTON_11;
		if(str == STRING("BUTTON_12"))
			return BUTTON_12;
		if(str == STRING("BUTTON_13"))
			return BUTTON_13;
		if(str == STRING("BUTTON_14"))
			return BUTTON_14;
		if(str == STRING("BUTTON_15"))
			return BUTTON_15;

		return BUTTON_UNKOWN;
	}

	// TODO Change this into a usual function. Do not use inline.
	inline XdevLButtonId covertIdxToXdevLButton(xdl_uint idx) {
		switch(idx) {
			case 1:
				return BUTTON_0;
			case 2:
				return BUTTON_1;
			case 3:
				return BUTTON_2;
			case 4:
				return BUTTON_3;
			case 5:
				return BUTTON_4;
			case 6:
				return BUTTON_5;
			case 7:
				return BUTTON_6;
			case 8:
				return BUTTON_7;
			case 9:
				return BUTTON_8;
			case 10:
				return BUTTON_9;
			case 11:
				return BUTTON_10;
			case 12:
				return BUTTON_11;
			case 13:
				return BUTTON_12;
			case 14:
				return BUTTON_13;
			case 15:
				return BUTTON_14;
			case 16:
				return BUTTON_15;
			default:
				return BUTTON_UNKOWN;
		}
	}

	XdevLAxisId getAxisId(const XdevLString& str) {
		if(str == STRING("AXIS_0"))
			return AXIS_0;
		if(str == STRING("AXIS_1"))
			return AXIS_1;
		if(str == STRING("AXIS_2"))
			return AXIS_2;
		if(str == STRING("AXIS_3"))
			return AXIS_3;
		if(str == STRING("AXIS_4"))
			return AXIS_4;
		if(str == STRING("AXIS_5"))
			return AXIS_5;
		if(str == STRING("AXIS_6"))
			return AXIS_6;
		if(str == STRING("AXIS_7"))
			return AXIS_7;
		if(str == STRING("AXIS_8"))
			return AXIS_8;
		if(str == STRING("AXIS_9"))
			return AXIS_9;
		if(str == STRING("AXIS_10"))
			return AXIS_10;
		if(str == STRING("AXIS_11"))
			return AXIS_11;
		if(str == STRING("AXIS_12"))
			return AXIS_12;
		if(str == STRING("AXIS_13"))
			return AXIS_13;
		if(str == STRING("AXIS_14"))
			return AXIS_14;
		if(str == STRING("AXIS_15"))
			return AXIS_15;
		return AXIS_0;
	}

	class XdevLButtonImpl : public XdevLButton {
		public:
			XdevLButtonImpl(thread::Mutex* mutex) :	m_pressTime(0),
				m_releaseTime(0),
				m_crt(300000),
				m_press(false),
				m_pressOld(false),
				m_mutex(mutex) {
			}

			virtual ~XdevLButtonImpl() {}
			virtual xdl_bool getPressed() {
				xdl_bool tmp;
				m_mutex->Lock();
				tmp = m_press;
				m_mutex->Unlock();
				return tmp;
			}
			virtual xdl_bool getClicked() {
				xdl_bool tmp = false;
				m_mutex->Lock();
				if((m_press == false) && (m_pressOld == true)) {
					m_pressOld = false;
					if((m_releaseTime - m_pressTime) <= m_crt) {
						m_pressTime = 0;
						m_releaseTime = 1000000;
						tmp = true;
					}
				}
				m_mutex->Unlock();
				return tmp;
			}
			virtual void setClickResponseTime(xdl_uint64 crt) {
				m_mutex->Lock();
				m_crt = crt;
				m_mutex->Unlock();
			}
			virtual xdl_uint64 getClickResponseTime() {
				xdl_uint64 tmp;
				m_mutex->Lock();
				tmp = m_crt;
				m_mutex->Unlock();
				return tmp;
			}
			// --------------------------------------------------------------------------
			// Invisible for the user.
			//
			void setState(xdl_bool state) {
				m_pressOld = m_press;
				m_press = state;
			}
			void capturePressTime(xdl_uint64 time) {
				m_pressTime = time;
			}
			void captureReleaseTime(xdl_uint64 time) {
				m_releaseTime = time;
			}
		protected:
			xdl_uint64		m_pressTime;
			xdl_uint64		m_releaseTime;
			xdl_uint64		m_crt;
			xdl_bool		m_press;
			xdl_bool		m_pressOld;
			thread::Mutex*	m_mutex;
	};

	class XdevLAxisImpl: public XdevLAxis {
		public:
			XdevLAxisImpl(thread::Mutex* mutex) :
				m_value(0.0f),
				m_valueOld(0.0f),
				m_min(-1.0f),
				m_max(1.0f),
				m_mutex(mutex) {
			}

			virtual ~XdevLAxisImpl()  {}
			void setMinMax(xdl_float lmin, xdl_float lmax) {
				m_mutex->Lock();
				m_min = lmin;
				m_max = lmax;
				m_mutex->Unlock();
			}
			void setMin(xdl_float lmin) {
				m_mutex->Lock();
				m_min = lmin;
				m_mutex->Unlock();
			}
			void setMax(xdl_float lmax) {
				m_mutex->Lock();
				m_max = lmax;
				m_mutex->Unlock();
			}

			xdl_int getMinMax(xdl_float* lmin, xdl_float* lmax) {
				m_mutex->Lock();
				*lmin = m_min;
				*lmax = m_max;
				m_mutex->Unlock();
				return ERR_OK;
			}
			virtual xdl_float getValue() {
				xdl_float tmp;
				m_mutex->Lock();
				tmp = ((m_min*(1.0f-m_value) + m_max*m_value));
				m_mutex->Unlock();
				return tmp;
			}
			virtual xdl_float getDeltaValue() {
				xdl_float tmp;
				m_mutex->Lock();
				tmp = m_deltaValue;
				m_mutex->Unlock();
				return tmp;
			}
			virtual xdl_float getMin() const {
				xdl_float tmp;
				m_mutex->Lock();
				tmp =  m_min;
				m_mutex->Unlock();
				return tmp;
			}
			virtual xdl_float getMax() const {
				xdl_float tmp;
				m_mutex->Lock();
				tmp = m_max;
				m_mutex->Unlock();
				return tmp;
			}

			// --------------------------------------------------------------------------
			// Invisible for the user.
			//
			void setValue(xdl_float value) {
//				m_deltaValue = value - m_value;
				m_valueOld = m_value;
				m_value = value;
			}
			
			void setDeltaValue(xdl_float value) {
				m_deltaValue = value;
			}

			void addValue(xdl_float value) {
				m_valueOld = m_value;
				m_value += value;
			}
		protected:
			xdl_float		m_value;
			xdl_float		m_deltaValue;
			xdl_float		m_valueOld;
			xdl_float		m_min;
			xdl_float 		m_max;
			thread::Mutex* 	m_mutex;
	};

	class XdevLPOVImpl : public XdevLPOV {
		public:
			XdevLPOVImpl(thread::Mutex* mutex) : m_direction(0), m_mutex(mutex) {}
			virtual ~XdevLPOVImpl() {
			}
			virtual xdl_uint getDirection() const {
				xdl_uint val;
				m_mutex->Lock();
				val = m_direction;
				m_mutex->Unlock();
				return val;
			}
			// --------------------------------------------------------------------------
			// Invisible for the user.
			//
			virtual void setDirection(xdl_uint dir) {
				m_direction = dir;
			}
		protected:
			xdl_uint		m_direction;
			thread::Mutex* 	m_mutex;
	};

	template <typename T>
	class XdevLInputSystemImpl : XdevLModuleImpl<XdevLInputSystem> {

			std::vector<XdevLButtonImpl*>	m_Buttons;
			std::vector<XdevLAxisImpl*>		m_Axes;
			std::vector<XdevLPOVImpl*>		m_POVs;
			thread::Mutex					m_mutex;
			std::multimap<XdevLButtonId, XdevLButtonIdDelegateType> m_buttonIdDelegates;
			std::multimap<XdevLAxisId, XdevLAxisIdDelegateType> 	m_axisIdDelegates;
			std::vector<XdevLButtonDelegateType> 					m_buttonDelegates;
			std::vector<XdevLAxisDelegateType> 						m_axisDelegates;
			xdl_bool	m_threaded;
			xdl_double	m_sleep;
			xdl_bool	m_relativeMode;

		public:

			xdl_int registerDelegate(const XdevLString& id, const XdevLButtonIdDelegateType& delegate) override {
				XdevLButtonId idType = getButtonId(id);
				m_buttonIdDelegates.insert(std::pair<const XdevLButtonId, const XdevLButtonIdDelegateType>(idType, delegate));
				for(auto d : m_buttonIdDelegates) {
					std::cout << "Register Button: " << d.first << std::endl;
				}
				return ERR_OK;
			}

			xdl_int registerDelegate(const XdevLString& id, const XdevLAxisIdDelegateType& delegate) override {
				XdevLAxisId idType = getAxisId(id);
				m_axisIdDelegates.insert(std::pair<const XdevLAxisId, const XdevLAxisIdDelegateType>(idType, delegate));
				for(auto d : m_axisIdDelegates) {
					std::cout << "Register Axis: " << d.first << std::endl;
				}
				return ERR_OK;
			}

			xdl_int registerDelegate(const XdevLButtonDelegateType& delegate) override {
				m_buttonDelegates.push_back(delegate);
				return ERR_OK;
			}

			xdl_int registerDelegate(const XdevLAxisDelegateType& delegate) override {
				m_axisDelegates.push_back(delegate);
				return ERR_OK;
			}

			xdl_uint getNumButtons() const override {
				return static_cast<xdl_int>(m_Buttons.size());
			}

			xdl_int getButton(const xdl_uint idx, XdevLButton** button) override {

				if(m_Buttons.size() == 0) {
					*button = nullptr;
					return ERR_ERROR;
				}
				if((idx >= m_Buttons.size())) {
					*button = nullptr;
					return ERR_ERROR;
				}
				*button = m_Buttons[idx];
				return ERR_OK;
			}

			xdl_uint getNumAxis() const override {

				return static_cast<xdl_int>(m_Axes.size());
			}

			xdl_int getAxis(const xdl_uint idx, XdevLAxis** axis) const override {

				if(m_Axes.size() == 0) {
					*axis = nullptr;
					return ERR_ERROR;
				}
				if((idx >= m_Axes.size())) {
					*axis = nullptr;
					return ERR_ERROR;
				}
				*axis = m_Axes[idx];
				return ERR_OK;
			}

			xdl_uint getNumPOV() const {
				return static_cast<xdl_int>(m_POVs.size());
			}

			xdl_int getPOV(const xdl_uint idx, XdevLPOV** pov) const {
				if(m_POVs.size() == 0) {
					*pov = nullptr;
					return ERR_ERROR;
				}
				if((idx >= m_POVs.size())) {
					*pov = nullptr;
					return ERR_ERROR;
				}
				*pov = m_POVs[idx];
				return ERR_OK;
			}

			xdl_int readMouseInfo(TiXmlDocument& document, const xdl_char* modulename) {
				TiXmlHandle docHandle(&document);
				TiXmlElement* root = docHandle.FirstChild(XdevLCorePropertiesName.c_str()).FirstChildElement(modulename).ToElement();
				if(!root) {
					return ERR_OK;
				}

				while(root != NULL) {
					if(root->Attribute("id")) {
						XdevLID id(root->Attribute("id"));
						if(XdevLModuleImpl<T>::getID() == id) {
							// If there is a crt attribute, set this value for all buttons.
							if(root->Attribute("relative")) {
								m_relativeMode = xstd::from_string<xdl_bool>(root->Attribute("relative"));
							}

							// If there is a crt attribute, set this value for all buttons.
							if(root->Attribute("crt")) {
								xdl_double crt = xstd::from_string<xdl_int>(root->Attribute("crt"));
								for(size_t a = 0; a < m_Buttons.size(); ++a)
									m_Buttons[a]->setClickResponseTime(crt);
							}

							// If there is a min attribute, set this value for all axes.
							if(root->Attribute("min")) {
								xdl_float lmin = xstd::from_string<xdl_float>(root->Attribute("min"));
								for(size_t a = 0; a < m_Axes.size(); ++a)
									m_Axes[a]->setMin(lmin);
							}

							// If there is a max attribute, set this value for all axes.
							if(root->Attribute("max")) {
								xdl_float lmax = xstd::from_string<xdl_float>(root->Attribute("max"));
								for(size_t a = 0; a < m_Axes.size(); ++a)
									m_Axes[a]->setMax(lmax);
							}

							// Is there threading request?
							if(root->Attribute("thread")) {
								m_threaded = xstd::from_string<xdl_bool>(root->Attribute("thread"));
							}
							// Any sleep time information?
							if(root->Attribute("sleep")) {
								m_sleep = xstd::from_string<xdl_double>(root->Attribute("sleep"));
							}

							//FIXME Da die achsen erst spaeter erzeugt werden koennen die funktionen der achse nicht verwendet werden. Daher wird in m_axisMin und m_axisMax zwischengespeichert. das bitte aendern.
							TiXmlElement* child = 0;
							for(child = root->FirstChildElement(); child; child = child->NextSiblingElement()) {
								if(child->ValueTStr() == "Range") {
									int axis = 0;
									if(child->Attribute("axis")) {
										axis = getAxisId(STRING(child->Attribute("axis")));
										float value = 0.0f;
										if(child->Attribute("min")) {
											value = xstd::from_string<float>(child->Attribute("min"));
											m_Axes[axis]->setMin(value);
										}
										if(child->Attribute("max")) {
											value = xstd::from_string<float>(child->Attribute("max"));
											m_Axes[axis]->setMax(value);
										}
									} else {
										return ERR_ERROR;
									}
								}

								if(child->ValueTStr() == "ClickResponseTime") {
									int button = 0;
									if(child->Attribute("button")) {
										button = getButtonId(STRING(child->Attribute("button")));
										if(child->Attribute("value"))
											m_Buttons[button]->setClickResponseTime(xstd::from_string<float>(child->Attribute("value")));
									} else {
										return ERR_ERROR;
									}
								}
							}
						}
					}

					root = root->NextSiblingElement();
				}


				return ERR_OK;
			}

		public:

			void initAxis(xdl_int number) {

				m_Axes.reserve(number);
				m_Axes.resize(number);
				for(xdl_int a = 0; a < number; ++a) {
					m_Axes[a] = new XdevLAxisImpl(&m_mutex);
				}
			}
			void initButtons(xdl_int number) {
				m_Buttons.reserve(number);
				m_Buttons.resize(number);
				for(xdl_int a = 0; a < number; ++a) {
					m_Buttons[a] = new XdevLButtonImpl(&m_mutex);
				}
			}

			void initPOVs(xdl_int number) {
				m_POVs.reserve(number);
				m_POVs.resize(number);
				for(xdl_int a = 0; a < number; ++a) {
					m_POVs[a] = new XdevLPOVImpl(&m_mutex);
				}
			}

			XdevLButtonImpl* getButton(const xdl_uint id) const {
				return m_Buttons[id];
			}

			XdevLAxisImpl* getAxis(const xdl_uint id) const {
				return m_Axes[id];
			}

			void reset() {
				for(size_t a = 0; a < m_Buttons.size(); ++a)
					m_Buttons[a]->setState(xdl_false);
				for(size_t a = 0; a < m_Axes.size(); ++a)
					m_Axes[a]->setValue(xdl_false);
			}

			xdl_int shutdown() {
				for(size_t a = 0; a < m_Buttons.size(); ++a)
					delete m_Buttons[a];
				for(size_t a = 0; a < m_Axes.size(); ++a)
					delete m_Axes[a];
				for(size_t a = 0; a < m_POVs.size(); ++a)
					delete m_POVs[a];
				m_Buttons.clear();
				m_Axes.clear();
				m_POVs.clear();
			}
			std::vector<XdevLButtonDelegateType>& getButtonDelegates() {
				return m_buttonDelegates;
			}

			std::vector<XdevLAxisDelegateType>& getAxisDelegates() {
				return m_axisDelegates;
			}

			std::multimap<XdevLButtonId, XdevLButtonIdDelegateType>& getButtonIdDelegates() {
				return m_buttonIdDelegates;
			}

			thread::Mutex& getMutex() {
				return m_mutex;
			}

	};
}

#endif
