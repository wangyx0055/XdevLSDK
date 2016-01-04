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

#ifndef XDEVL_MOUSE_IMPL_H
#define XDEVL_MOUSE_IMPL_H

#include <XdevLPlatform.h>
#include <XdevLPluginImpl.h>
#include <XdevLMouse.h>
#include <XdevLThread.h>
#include <XdevLMutex.h>
#include "XdevLInputSystemUtils.h"
#include <tinyxml.h>

namespace xdl {

	// Holds the plugin major version.
	const xdl_uint XdevLMousePluginMajorVersion	= XDEVLMOUSE_MAJOR_VERSION;

	// Holds the plugin minor version.
	const xdl_uint XdevLMousePluginMinorVersion	= XDEVLMOUSE_MINOR_VERSION;

	// Holds the plugin patch version.
	const xdl_uint XdevLMousePluginPatchVersion	= XDEVLMOUSE_PATCH_VERSION;



	// Holds the Major version number.
	const xdl_uint XdevLMouseMajorVersion		= XDEVLMOUSE_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLMouseMinorVersion		= XDEVLMOUSE_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLMousePatchVersion		= XDEVLMOUSE_MODULE_PATCH_VERSION;


	static const XdevLString mouse_vendor {
		"www.codeposer.net"
	};
	static const XdevLString mouse_author {
		"Cengiz Terzibas"
	};
	static const XdevLString mouse_copyright {
		"(c) 2005 - 2015 Cengiz Terzibas."
	};
	static const XdevLString mouse_pluginName {
		"XdevLMouse"
	};
	static const XdevLString mouseDescription {
		"Gives you access to mouse events."
	};
	static const std::vector<XdevLModuleName> mouse_moduleNames {
		XdevLModuleName("XdevLMouse")
	};


	const xdl_uint MOUSE_MAX_AXES = 3;
	const xdl_uint MOUSE_MAX_BUTTONS = 16;

	static const XdevLID ButtonPressed("XDEVL_MOUSE_BUTTON_PRESSED");
	static const XdevLID ButtonReleased("XDEVL_MOUSE_BUTTON_RELEASED");
	static const XdevLID MouseMotion("XDEVL_MOUSE_MOTION");


	template<typename T>
	class XdevLMouseBase : public XdevLModuleAutoImpl<T>, public thread::Thread {
		public:
			XdevLMouseBase(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor)	:
				XdevLModuleAutoImpl<T>(parameter, descriptor),
				m_window(NULL),
				m_windowWidth(640),
				m_windowHeight(480),
				m_mouse_curr_x(0),
				m_mouse_curr_y(0),
				m_mouse_old_x(0),
				m_mouse_old_y(0),
				m_mouse_button_down(xdl_false),
				m_mouse_moved(xdl_false),
				m_mouse_moved_old(xdl_false),
				m_relativeMode(xdl_false),
				m_simulate_wheel_buttons(xdl_false),
				m_attached(xdl_false),
				m_skip(0),
				m_threaded(xdl_false),
				m_sleep(0.001) {
			}

			virtual ~XdevLMouseBase() {
			}

		protected:

			// Returns the button id as string.
			XdevLString getButtonIdAsString(const XdevLButtonId& id);

			// Reads the mouse device information from an xml file.
			xdl_int readMouseInfo(TiXmlDocument& document, const xdl_char* modulename);

			xdl_int update();

			xdl_int RunThread(thread::ThreadArgument* p_arg);

			xdl_int attach(XdevLWindow* window, const xdl_char* modulename);

			xdl_int notify(XdevLEvent& event) override;
			xdl_int init();
			xdl_int shutdown();

			xdl_int reset();

			xdl_bool getAttached();

			void setAttached(xdl_bool state);

			xdl_int registerDelegate(const XdevLString& id, const XdevLButtonIdDelegateType& delegate);
			xdl_int registerDelegate(const XdevLString& id, const XdevLAxisIdDelegateType& delegate);
			xdl_int registerDelegate(const XdevLButtonDelegateType& delegate);
			xdl_int registerDelegate(const XdevLAxisDelegateType& delegate);

		protected:
			XdevLWindow*	m_window;
			xdl_int			m_windowWidth;
			xdl_int			m_windowHeight;
			xdl_int			m_mouse_curr_x;
			xdl_int			m_mouse_curr_y;
			xdl_int			m_mouse_old_x;
			xdl_int			m_mouse_old_y;
			xdl_bool		m_mouse_button_down;
			xdl_bool		m_mouse_moved;
			xdl_bool		m_mouse_moved_old;
			xdl_bool		m_relativeMode;
			xdl_bool		m_simulate_wheel_buttons;
			xdl_bool		m_attached;
			xdl_int			m_skip;
			xdl_bool		m_threaded;
			xdl_double		m_sleep;
			thread::Mutex	m_mutex;
			std::vector<XdevLButtonImpl*>	m_Buttons;
			std::vector<XdevLAxisImpl*>		m_Axes;
			std::multimap<XdevLButtonId, XdevLButtonIdDelegateType> m_buttonIdDelegates;
			std::multimap<XdevLAxisId, XdevLAxisIdDelegateType> m_axisIdDelegates;
			std::vector<XdevLButtonDelegateType> m_buttonDelegates;
			std::vector<XdevLAxisDelegateType> m_axisDelegates;

	};

	template<typename T>
	xdl_int  XdevLMouseBase<T>::registerDelegate(const XdevLString& id, const XdevLButtonIdDelegateType& delegate) {
		XdevLButtonId idType = getButtonId(id);
		m_buttonIdDelegates.insert(std::pair<const XdevLButtonId, const XdevLButtonIdDelegateType>(idType, delegate));
		return ERR_OK;
	}

	template<typename T>
	xdl_int  XdevLMouseBase<T>::registerDelegate(const XdevLString& id, const XdevLAxisIdDelegateType& delegate) {
		XdevLAxisId idType = getAxisId(id);
		m_axisIdDelegates.insert(std::pair<const XdevLAxisId, const XdevLAxisIdDelegateType>(idType, delegate));
		return ERR_OK;
	}


	template<typename T>
	xdl_int  XdevLMouseBase<T>::registerDelegate(const XdevLButtonDelegateType& delegate) {
		m_buttonDelegates.push_back(delegate);
		return ERR_OK;
	}

	template<typename T>
	xdl_int  XdevLMouseBase<T>::registerDelegate(const XdevLAxisDelegateType& delegate) {
		m_axisDelegates.push_back(delegate);
		return ERR_OK;
	}

	template<typename T>
	xdl_bool XdevLMouseBase<T>::getAttached() {
		xdl_bool tmp;
		m_mutex.Lock();
		tmp = m_attached;
		m_mutex.Unlock();
		return tmp;
	}

	template<typename T>
	void XdevLMouseBase<T>::setAttached(xdl_bool state) {
		m_mutex.Lock();
		m_attached = state;
		m_mutex.Unlock();
	}

	template<typename T>
	xdl_int XdevLMouseBase<T>::init() {

		if(MOUSE_MAX_AXES > 0) {
			m_Axes.reserve(MOUSE_MAX_AXES);
			m_Axes.resize(MOUSE_MAX_AXES);
			for(xdl_int a = 0; a < MOUSE_MAX_AXES; ++a) {
				m_Axes[a] = new XdevLAxisImpl(&m_mutex);
			}
		}

		if(MOUSE_MAX_BUTTONS > 0) {
			m_simulate_wheel_buttons = xdl_true;
			m_Buttons.reserve(MOUSE_MAX_BUTTONS);
			m_Buttons.resize(MOUSE_MAX_BUTTONS);
			for(xdl_int a = 0; a < MOUSE_MAX_BUTTONS; ++a) {
				m_Buttons[a] = new XdevLButtonImpl(&m_mutex);
			}
		}

		reset();

		return ERR_OK;
	}

	template<typename T>
	xdl_int XdevLMouseBase<T>::shutdown() {
		XDEVL_MODULE_INFO("Starting shutdown process.\n");

		for(size_t a = 0; a < m_Buttons.size(); ++a)
			delete m_Buttons[a];
		for(size_t a = 0; a < m_Axes.size(); ++a)
			delete m_Axes[a];
		m_Buttons.clear();
		m_Axes.clear();


		setAttached(xdl_false);

		// Wait unit the thread has finished his job.
		if(m_threaded)
			Join();

		XDEVL_MODULE_SUCCESS("Shutdown process was successful.\n");
		return ERR_OK;
	}

	template<typename T>
	xdl_int XdevLMouseBase<T>::reset() {
		for(auto& button : m_Buttons)
			button->setState(xdl_false);
		for(size_t a = 0; a < m_Axes.size(); ++a)
			m_Axes[a]->setValue(0.0f);
		return ERR_OK;
	}

	template<typename T>
	xdl_int XdevLMouseBase<T>::RunThread(thread::ThreadArgument*) {
		XDEVL_MODULE_INFO("Starting threading mode.\n");
		while(getAttached()) {
			update();
			sleep(m_sleep);
		}
		return 0;
	}

	template<typename T>
	xdl_int XdevLMouseBase<T>::update() {

		m_mutex.Lock();

		m_mouse_moved_old = m_mouse_moved;
		m_mouse_moved = xdl_false;

		m_Axes[AXIS_0]->setDeltaValue(0.0f);
		m_Axes[AXIS_1]->setDeltaValue(0.0f);

		m_mutex.Unlock();

		return ERR_OK;
	}

	template<typename T>
	xdl_int XdevLMouseBase<T>::notify(XdevLEvent& event) {

		//
		// Do this only if the user attached a window.
		//
		if(nullptr != m_window) {

			if(event.type == XDEVL_WINDOW_EVENT) {
				if(event.window.event == XDEVL_WINDOW_RESIZED) {
					if(m_window->getWindowID() == event.window.windowid) {
						m_windowWidth = event.window.width;
						m_windowHeight = event.window.height;
					}
				}
			} else if( event.type == ButtonPressed.getHashCode()) {
				if(m_Buttons.size() == 0)
					return xdl_false;

				xdl_int idx = event.button.button;
				if(!m_mouse_button_down)
					m_Buttons[idx]->capturePressTime(event.common.timestamp);

				m_mouse_button_down = xdl_true;
				m_Buttons[idx]->setState(xdl_true);

				for(auto& delegate : m_buttonDelegates) {
					delegate(covertIdxToXdevLButton(idx), BUTTON_PRESSED);
				}



				XdevLButtonId id = covertIdxToXdevLButton(idx);
				auto pp = m_buttonIdDelegates.equal_range(id);
				for (auto it = pp.first; it != pp.second; ++it) {
					auto delegate = it->second;
					delegate(BUTTON_PRESSED);
				}

			} else if( event.type == ButtonReleased.getHashCode()) {
				if(m_Buttons.size() == 0)
					return xdl_false;

				xdl_int idx = event.button.button;

				if(m_mouse_button_down)
					m_Buttons[idx]->captureReleaseTime(event.common.timestamp);

				m_mouse_button_down = xdl_false;
				m_Buttons[idx]->setState(xdl_false);

				for(auto& delegate : m_buttonDelegates) {
					delegate(covertIdxToXdevLButton(idx), BUTTON_RELEASED);
				}

				XdevLButtonId id = covertIdxToXdevLButton(idx);
				auto pp = m_buttonIdDelegates.equal_range(id);
				for (auto it = pp.first; it != pp.second; ++it) {
					auto delegate = it->second;
					delegate(BUTTON_RELEASED);
				}

			} else if( event.type == MouseMotion.getHashCode()) {

				xdl_uint32 x = event.motion.x;
				xdl_uint32 y = event.motion.y;

				m_mouse_old_x = m_mouse_curr_x;
				m_mouse_curr_x = x;
				m_mouse_old_y = m_mouse_curr_y;
				m_mouse_curr_y = y;
				m_mouse_moved = true;

				if(m_window && m_relativeMode) {
					m_window->grabPointer();

					if( (x <= 0) || (x >= m_windowWidth - 1) || (y <= 0) || (y >= m_windowHeight - 1))   {
						m_window->setPointerPosition(m_windowWidth/2, m_windowHeight/2);
						m_mouse_old_x 	= m_windowWidth/2;
						m_mouse_old_y 	= m_windowHeight/2;
						m_mouse_curr_x 	= m_windowWidth/2;
						m_mouse_curr_y 	= m_windowHeight/2;
						return ERR_OK;
					}

				}

				//
				// Why do I do x + 1? Because: 0 <= x < width. The same goes for y.
				//

				if(x >= m_windowWidth - 1) {
					x = m_windowWidth;
				}

				if(y >= m_windowHeight - 1) {
					y = m_windowHeight;
				}

				if(m_Axes.size() > 0) {
					m_Axes[AXIS_0]->setValue(((xdl_float)(m_mouse_curr_x) / (xdl_float)m_windowWidth));
					m_Axes[AXIS_1]->setValue(((xdl_float)(m_mouse_curr_y) / (xdl_float)m_windowHeight));
					m_Axes[AXIS_0]->setDeltaValue((xdl_float)(m_mouse_curr_x - m_mouse_old_x));
					m_Axes[AXIS_1]->setDeltaValue((xdl_float)(m_mouse_curr_y - m_mouse_old_y));

					for(auto& delegate : m_axisDelegates) {
						delegate(AXIS_0, m_Axes[AXIS_0]->getValue());
						delegate(AXIS_1, m_Axes[AXIS_1]->getValue());
					}
				}
				
//				printf("abs. (%f %f), rel. (%f %f)\n", m_Axes[AXIS_0]->getValue(), 
//														m_Axes[AXIS_1]->getValue(), 
//														m_Axes[AXIS_0]->getDeltaValue(), 
//														m_Axes[AXIS_0]->getDeltaValue());


			}

		}

		return XdevLModuleAutoImpl<T>::notify(event);
	}

	template<typename T>
	xdl_int XdevLMouseBase<T>::attach(XdevLWindow* window, const xdl_char* modulename) {
		if(!window) {
			XDEVL_MODULE_ERROR("Could not attach device to window.\n");
			return ERR_ERROR;
		}
		m_window = window;

		if(getAttached()) {
			setAttached(false);
			Join();
		}

		const XdevLWindowSize& size = window->getSize();

		m_windowWidth	= size.width;
		m_windowHeight	= size.height;


		if(this->getMediator()->getXmlFilename()) {
			TiXmlDocument xmlDocument;
			if(!xmlDocument.LoadFile(this->getMediator()->getXmlFilename())) {
				XDEVL_MODULE_ERROR("Could not parse xml file: " << this->getMediator()->getXmlFilename() << std::endl);
				return ERR_ERROR;
			}

			if(readMouseInfo(xmlDocument, modulename) != ERR_OK)
				return ERR_ERROR;
		}

		// If threading is activated it will start only if this device is attached.
		m_attached = true;

		return ERR_OK;
	}

	template<typename T>
	xdl_int XdevLMouseBase<T>::readMouseInfo(TiXmlDocument& document, const xdl_char* modulename) {
		TiXmlHandle docHandle(&document);
		TiXmlElement* root = docHandle.FirstChild(XdevLCorePropertiesName.c_str()).FirstChildElement(modulename).ToElement();
		if(!root) {
			XDEVL_MODULE_WARNING("<XdevLMouse> section not found. Using default values for the device.\n");
			return ERR_OK;
		}

		while(root != NULL) {
			if(root->Attribute("id")) {
				XdevLID id(root->Attribute("id"));
				if(this->getID() == id) {
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
								XDEVL_MODULE_ERROR("No axis attribute in <Range> element found. Please specify an axis id.\n");
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
								XDEVL_MODULE_ERROR("No button attribute in <ClickResponseTime> element found. Please specify a button id.\n");
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



	template<typename T>
	XdevLString XdevLMouseBase<T>::getButtonIdAsString(const XdevLButtonId& id) {
		switch(id) {
			case BUTTON_0:
				return XdevLString("BUTTON_0");
			default:
				return XdevLString();
		}
	}


	/**
		@class XdevLMouseImpl
		@brief Implementation class for the XdevLMouseDevice interface
		@author Cengiz Terzibas
	*/
	class XdevLMouseImpl : public XdevLMouseBase<XdevLMouse> {
		public:
			XdevLMouseImpl(XdevLModuleCreateParameter* parameter);

			static XdevLModuleDescriptor m_moduleDescriptor;

			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;
			virtual xdl_int update() override;

			virtual xdl_int registerDelegate(const XdevLString& id, const XdevLButtonIdDelegateType& delegate);
			virtual xdl_int registerDelegate(const XdevLString& id, const XdevLAxisIdDelegateType& delegate);
			virtual xdl_int registerDelegate(const XdevLButtonDelegateType& delegate);
			virtual xdl_int registerDelegate(const XdevLAxisDelegateType& delegate);

			virtual xdl_int attach(XdevLWindow* window);

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
			virtual xdl_double getClickResponseTime(const xdl_uint key) ;

			virtual xdl_float getValue(const xdl_uint button);

			virtual void setAxisRangeMinMax(const xdl_uint axis, xdl_float min, xdl_float max);
			virtual void setAxisRangeMin(const xdl_uint axis, xdl_float min);
			virtual void seAxisRangeMax(const xdl_uint axis, xdl_float max);
			virtual void getAxisRangeMinMax(const xdl_uint axis, xdl_float* min, xdl_float* max);
			virtual xdl_float getAxisRangeMin(const xdl_uint axis) const;
			virtual xdl_float getAxisRangeMax(const xdl_uint axis) const;


			virtual void setRelativeMode(xdl_bool state);
	};

}

#endif
