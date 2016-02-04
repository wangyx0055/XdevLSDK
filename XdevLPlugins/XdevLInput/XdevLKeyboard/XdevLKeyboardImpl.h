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

#ifndef XDEVL_KEYBOARD_DEVICE_IMPL_H
#define XDEVL_KEYBOARD_DEVICE_IMPL_H

#include <XdevLPlatform.h>
#include <XdevLPluginImpl.h>
#include <XdevLInput/XdevLKeyboard/XdevLKeyboard.h>
#include <XdevLXstring.h>
#include <XdevLThread.h>
#include <XdevLMutex.h>
#include <XdevLInputSystem.h>
#include "XdevLInputSystemUtils.h"
#include <tinyxml.h>
#include <map>

namespace xdl {

	class XdevLCoreMediator;
	class XdevLCoreImpl;
	class XdevLButtonImpl;
	class XdevLWindow;


	// Holds the plugin major version.
	const xdl_uint XdevLKeyboardPluginMajorVersion = XDEVLKEYBOARD_MAJOR_VERSION;

	// Holds the plugin minor version.
	const xdl_uint XdevLKeyboardPluginMinorVersion = XDEVLKEYBOARD_MINOR_VERSION;

	// Holds the plugin patch version.
	const xdl_uint XdevLKeyboardPluginPatchVersion = XDEVLKEYBOARD_PATCH_VERSION;



	// Holds the Major version number.
	const xdl_uint XdevLKeyboardMajorVersion = XDEVLKEYBOARD_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLKeyboardMinorVersion = XDEVLKEYBOARD_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLKeyboardPatchVersion = XDEVLKEYBOARD_MODULE_PATCH_VERSION;



	static const XdevLString keyboard_vendor {
		"www.codeposer.net"
	};
	static const XdevLString keyboard_author {
		"Cengiz Terzibas"
	};
	static const XdevLString keyboard_copyright {
		"(c) 2005 - 2015 Cengiz Terzibas."
	};
	static const XdevLString keyboard_pluginName {
		"XdevLKeyboard"
	};
	static const XdevLString keyboardDescription {
		"Gives you access to keyboard devices elements."
	};

	static const std::vector<XdevLModuleName> keyboard_moduleNames {
		XdevLModuleName("XdevLKeyboard")
	};

	static const XdevLID ButtonPressed("XDEVL_BUTTON_PRESSED");
	static const XdevLID ButtonReleased("XDEVL_BUTTON_RELEASED");

	template<typename T>
	class XdevlKeyboardBase :  public XdevLModuleImpl<T>, public thread::Thread {
		public:
			XdevlKeyboardBase(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor):
				XdevLModuleImpl<T>(parameter, descriptor),
				m_attached(xdl_false) ,
				m_key_down(xdl_false),
				m_threaded(xdl_false),
				m_initialized(xdl_false),
				m_sleep(0.001) {

			}
			virtual	~XdevlKeyboardBase() {
				for(auto keybutton : m_Buttons) {
					delete keybutton.second;
				}
				keyButtonId.clear();
			}
			typedef std::map<const XdevLString, XdevLButtonId> KeyMapType;
		protected:
			// Is this device attached to a window?
			xdl_bool		m_attached;

			// Key map.
			KeyMapType keyButtonId;
			xdl_bool m_key_down;
			xdl_bool m_threaded;
			xdl_bool m_initialized;
			xdl_double		m_sleep;
			thread::Mutex	m_mutex;
			std::vector<XdevLButtonDelegateType> m_buttonDelegates;
			std::multimap<XdevLButtonId, XdevLButtonIdDelegateType> m_buttonIdDelegates;
			std::map<xdl_uint, XdevLButtonImpl*> m_Buttons;
		protected:
			using XdevLModuleImpl<T>::attach;

			xdl_bool getAttached();
			void setAttached(xdl_bool state);

			XdevLButtonId getButtonId(const XdevLString& str);
			xdl_int initKeyMap();
			xdl_int readKeyboardXmlInfo(TiXmlDocument* document, const xdl_char* moduleName);
			xdl_int update();
			xdl_int RunThread(thread::ThreadArgument*);
			xdl_int notify(XdevLEvent& event);
			xdl_int attach(XdevLWindow* window, const xdl_char* moduleName);
			xdl_int init();
			xdl_int shutdown();
			xdl_int reset();

			xdl_int registerDelegate(const XdevLString& id, const XdevLButtonIdDelegateType& delegate);
			xdl_int registerDelegate(const XdevLButtonDelegateType& delegate);
	};

	template<typename T>
	xdl_int  XdevlKeyboardBase<T>::registerDelegate(const XdevLString& id, const XdevLButtonIdDelegateType& delegate) {
		XdevLButtonId idType = getButtonId(id);
		m_buttonIdDelegates.insert(std::pair<const XdevLButtonId, const XdevLButtonIdDelegateType>(static_cast<XdevLButtonId>(idType), delegate));
		return ERR_OK;
	}

	template<typename T>
	xdl_int  XdevlKeyboardBase<T>::registerDelegate(const XdevLButtonDelegateType& delegate) {
		m_buttonDelegates.push_back(delegate);
		return ERR_OK;
	}


	template<typename T>
	xdl_bool XdevlKeyboardBase<T>::getAttached() {

		m_mutex.Lock();
		xdl_bool tmp;
		tmp = m_attached;
		m_mutex.Unlock();

		return tmp;
	}

	template<typename T>
	void XdevlKeyboardBase<T>::setAttached(xdl_bool state) {
		m_mutex.Lock();
		m_attached = state;
		m_mutex.Unlock();
	}

	template<typename T>
	xdl_int XdevlKeyboardBase<T>::init() {
		initKeyMap();
		m_initialized = xdl_true;
		return ERR_OK;
	}

	template<typename T>
	xdl_int XdevlKeyboardBase<T>::shutdown() {
		// Delete all XdevLButton objects.
		for(auto& button : m_Buttons) {
			delete button.second;
		}

		m_Buttons.clear();

		setAttached(false);

		// Wait unit the thread has finished his job.
		if(m_threaded) {
			Join();
		}
		m_initialized = xdl_false;
		return ERR_OK;
	}

	template<typename T>
	xdl_int XdevlKeyboardBase<T>::reset() {
		for(size_t a = 0; a < m_Buttons.size(); ++a)
			m_Buttons[a]->setState(false);
		return ERR_OK;
	}

	template<typename T>
	xdl_int XdevlKeyboardBase<T>::RunThread(thread::ThreadArgument*) {
		XDEVL_MODULE_INFO("Starting threading mode.\n");
		while(getAttached()) {
			update();
			sleep(m_sleep);
		}
		return 0;
	}

	template<typename T>
	xdl_int XdevlKeyboardBase<T>::update() {
		return ERR_OK;
	}

	template<typename T>
	xdl_int XdevlKeyboardBase<T>::notify(XdevLEvent& event) {
		//
		// Did the user press a keyboard button?
		//
		if(event.type == ButtonPressed.getHashCode()) {
			XdevLButtonImpl* button = m_Buttons[event.key.keycode];
			if(button == nullptr) {
				m_Buttons[event.key.keycode] = new XdevLButtonImpl(&m_mutex);
				button = m_Buttons[event.key.keycode];
			}

			if(!m_key_down) {
				button->capturePressTime(event.common.timestamp);
			}

			button->setState(true);
			m_key_down = true;

			auto pp = m_buttonIdDelegates.equal_range(static_cast<XdevLButtonId>(event.key.keycode));
			for(auto it = pp.first; it != pp.second; ++it) {
				auto delegate = it->second;
				delegate(BUTTON_PRESSED);
			}

		}
		//
		// Did the user released a keyboard button?
		//
		else if(event.type == ButtonReleased.getHashCode()) {

			XdevLButtonImpl* button = m_Buttons[event.key.keycode];
			if(button == nullptr) {
				m_Buttons[event.key.keycode] =  new XdevLButtonImpl(&m_mutex);
				button = m_Buttons[event.key.keycode];
			}

			if(m_key_down) {
				button->captureReleaseTime(event.common.timestamp);
			}

			button->setState(false);
			m_key_down = false;

			auto pp = m_buttonIdDelegates.equal_range(static_cast<XdevLButtonId>(event.key.keycode));
			for(auto it = pp.first; it != pp.second; ++it) {
				auto delegate = it->second;
				delegate(BUTTON_RELEASED);
			}
		}
		//
		// Did the core sent a message?
		//
		else if(event.type == XDEVL_MODULE_EVENT) {
			// Initialize this module.
			if(event.module.event == XDEVL_MODULE_INIT) {
				return init();
			}
			// Shutdown this module.
			else if(event.module.event == XDEVL_MODULE_SHUTDOWN) {
				return shutdown();
			}
		}

		return ERR_OK;
	}

	template<typename T>
	XdevLButtonId XdevlKeyboardBase<T>::getButtonId(const XdevLString& str) {
		std::string tmp(str);
		xstd::trim(tmp);

		KeyMapType::iterator it = keyButtonId.find(str);
		if(it != keyButtonId.end()) {
			return it->second;
		}

		return BUTTON_UNKOWN;
	}


	template<typename T>
	xdl_int XdevlKeyboardBase<T>::attach(XdevLWindow* window, const xdl_char* moduleName) {
		if(!window) {
			XDEVL_MODULE_ERROR("Could not attach device to window. No valid XdevLWindowDevice specified.\n");
			return ERR_ERROR;
		}

		if(xdl_false == m_initialized) {
			if(init() != ERR_OK) {
				return ERR_ERROR;
			}
		}

		if(getAttached()) {
			setAttached(false);
			Join();
		}

		if(nullptr != this->getMediator()) {
			if(this->getMediator()->getXmlFilename() != XdevLFileName()) {
				TiXmlDocument xmlDocument;
				if(!xmlDocument.LoadFile(this->getMediator()->getXmlFilename())) {
					XDEVL_MODULE_ERROR("Could not parse xml file: " << this->getMediator()->getXmlFilename() << std::endl);
					return ERR_ERROR;
				}
				if(readKeyboardXmlInfo(&xmlDocument, moduleName) != ERR_OK)
					return ERR_ERROR;
			}
		}

		setAttached(true);

		return ERR_OK;
	}

	template<typename T>
	xdl_int XdevlKeyboardBase<T>::readKeyboardXmlInfo(TiXmlDocument* document, const xdl_char* moduleName) {
		TiXmlHandle docHandle(document);
		TiXmlElement* root = docHandle.FirstChild(XdevLCorePropertiesName.c_str()).FirstChildElement(moduleName).ToElement();
		if(!root) {
			return ERR_OK;
		}

		if(root->Attribute("id")) {
			XdevLID id(root->Attribute("id"));
			if(this->getID() != id) {
				return ERR_OK;
			}
		} else {
			XDEVL_MODULE_ERROR("No 'id' attribute specified.\n");
			return ERR_ERROR;
		}

		// If there is a crt attribute, set this value for all buttons.
		if(root->Attribute("crt")) {
			xdl_double crt = xstd::from_string<xdl_int>(root->Attribute("crt"));
			for(size_t a = 0; a < m_Buttons.size(); ++a)
				m_Buttons[a]->setClickResponseTime(crt);
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
			if(child->ValueTStr() == "ClickResponseTime") {
				int button = 0;
				if(child->Attribute("button")) {
					button = keyButtonId[STRING(child->Attribute("button"))];
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

	template<typename T>
	xdl_int XdevlKeyboardBase<T>::initKeyMap() {

		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_ENTER"),KEY_ENTER));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_ESCAPE"),KEY_ESCAPE));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_BACKSPACE"),KEY_BACKSPACE));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_TAB"),KEY_TAB));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_SPACE"),KEY_SPACE));

		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_PLUS"),KEY_PLUS));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_COMMA"),KEY_COMMA));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_MINUS"),KEY_MINUS));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_PERIOD"),KEY_PERIOD));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_SLASH"),KEY_SLASH));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_BACKSLASH"),KEY_BACKSLASH));

		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_0"),KEY_0));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_1"),KEY_1));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_2"),KEY_2));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_3"),KEY_3));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_4"),KEY_4));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_5"),KEY_5));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_6"),KEY_6));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_7"),KEY_7));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_8"),KEY_8));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_9"),KEY_9));

		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_A"),KEY_A));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_B"),KEY_B));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_C"),KEY_C));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_D"),KEY_D));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_E"),KEY_E));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F"),KEY_F));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_G"),KEY_G));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_H"),KEY_H));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_I"),KEY_I));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_J"),KEY_J));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_K"),KEY_K));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_L"),KEY_L));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_M"),KEY_M));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_N"),KEY_N));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_O"),KEY_O));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_P"),KEY_P));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_Q"),KEY_Q));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_R"),KEY_R));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_S"),KEY_S));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_T"),KEY_T));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_U"),KEY_U));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_V"),KEY_V));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_W"),KEY_W));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_X"),KEY_X));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_Y"),KEY_Y));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_Z"),KEY_Z));

		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F1"),KEY_F1));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F2"),KEY_F2));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F3"),KEY_F3));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F4"),KEY_F4));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F5"),KEY_F5));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F6"),KEY_F6));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F7"),KEY_F7));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F8"),KEY_F8));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F9"),KEY_F9));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F10"),KEY_F10));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F11"),KEY_F11));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F12"),KEY_F12));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F13"),KEY_F13));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F14"),KEY_F14));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F15"),KEY_F15));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F16"),KEY_F16));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F17"),KEY_F17));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F18"),KEY_F18));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F19"),KEY_F19));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F20"),KEY_F20));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F21"),KEY_F21));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F22"),KEY_F22));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F23"),KEY_F23));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_F24"),KEY_F24));


		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_PRINTSCREEN"),KEY_PRINTSCREEN));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_SCROLLLOCK"),KEY_SCROLLLOCK));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_PAUSE"),KEY_PAUSE));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_INSERT"),KEY_INSERT));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_HOME"),KEY_HOME));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_PAGEUP"),KEY_PAGEUP));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_DELETE"),KEY_DELETE));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_END"),KEY_END));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_PAGEDOWN"),KEY_PAGEDOWN));

		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_RIGHT"),KEY_RIGHT));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_LEFT"),KEY_LEFT));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_DOWN"),KEY_DOWN));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_UP"),KEY_UP));

		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_CAPSLOCK"),KEY_CAPSLOCK));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_NUMLOCK"),KEY_NUMLOCK));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_KP_DIVIDE"),KEY_KP_DIVIDE));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_KP_MULTIPLY"),KEY_KP_MULTIPLY));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_KP_MINUS"),KEY_KP_MINUS));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_KP_PLUS"),KEY_KP_PLUS));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_KP_ENTER"),KEY_KP_ENTER));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_KP_0"),KEY_KP_0));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_KP_1"),KEY_KP_1));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_KP_2"),KEY_KP_2));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_KP_3"),KEY_KP_3));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_KP_4"),KEY_KP_4));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_KP_5"),KEY_KP_5));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_KP_6"),KEY_KP_6));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_KP_7"),KEY_KP_7));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_KP_8"),KEY_KP_8));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_KP_9"),KEY_KP_9));
		keyButtonId.insert(KeyMapType::value_type(STRING("KEY_KP_PERIOD"),KEY_KP_PERIOD));

		m_Buttons[KEY_0] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_1] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_2] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_3] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_4] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_5] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_6] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_7] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_8] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_9] = new XdevLButtonImpl(&m_mutex);

		m_Buttons[KEY_A] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_B] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_C] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_D] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_E] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_G] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_H] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_I] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_J] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_K] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_L] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_M] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_N] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_O] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_P] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_Q] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_R] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_S] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_T] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_U] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_V] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_W] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_X] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_Y] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_Z] = new XdevLButtonImpl(&m_mutex);

		m_Buttons[KEY_F1] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F2] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F3] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F4] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F5] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F6] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F7] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F8] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F9] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F10] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F11] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F12] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F13] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F14] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F15] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F16] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F17] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F18] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F19] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F20] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F21] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F22] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F23] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_F24] = new XdevLButtonImpl(&m_mutex);

		m_Buttons[KEY_HOME] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_END] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_PAGEUP] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_PAGEDOWN] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_DELETE] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_INSERT] = new XdevLButtonImpl(&m_mutex);

		m_Buttons[KEY_UP] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_DOWN] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_LEFT] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_RIGHT] = new XdevLButtonImpl(&m_mutex);

		m_Buttons[KEY_ESCAPE] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_ENTER] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_TAB] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_SPACE] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_BACKSPACE] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_SLASH] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_BACKSLASH] = new XdevLButtonImpl(&m_mutex);

		m_Buttons[KEY_PLUS] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_MINUS] = new XdevLButtonImpl(&m_mutex);

		m_Buttons[KEY_KP_MULTIPLY] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_KP_DIVIDE] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_KP_MINUS] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_KP_PLUS] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_KP_ENTER] = new XdevLButtonImpl(&m_mutex);

		m_Buttons[KEY_KP_PERIOD] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_KP_0] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_KP_1] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_KP_2] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_KP_3] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_KP_4] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_KP_5] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_KP_6] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_KP_7] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_KP_8] = new XdevLButtonImpl(&m_mutex);
		m_Buttons[KEY_KP_9] = new XdevLButtonImpl(&m_mutex);

		return ERR_OK;
	}

// ----------------------------------------------------------------------------
// Module 1
//

	/**
		@class XdevLKeyboardImpl
		@brief Implementation class for the XdevLKeyboard interface
		@author Cengiz Terzibas
	*/
	class XdevLKeyboardImpl : public XdevlKeyboardBase<XdevLKeyboard> {
		public:
			XdevLKeyboardImpl(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor);
			virtual ~XdevLKeyboardImpl();

			// --------------------------------------------------------------------------
			// XdevLModule function
			//
			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;

			virtual xdl_int registerDelegate(const XdevLString& id, const XdevLButtonIdDelegateType& delegate) override;
			virtual xdl_int registerDelegate(const XdevLString& id, const XdevLAxisIdDelegateType& delegate) override;
			virtual xdl_int registerDelegate(const XdevLButtonDelegateType& delegate) override;
			virtual xdl_int registerDelegate(const XdevLAxisDelegateType& delegate) override;

			// --------------------------------------------------------------------------
			// XdevLKeyboard functions
			//
			virtual xdl_int attach(XdevLWindow* window) override;

			// --------------------------------------------------------------------------
			// XdevLEventCatalystFactory functions
			//
			virtual xdl_uint getNumButtons() const;
			virtual xdl_int getButton(const xdl_uint idx, XdevLButton** button);
			virtual xdl_uint getNumAxis() const;
			virtual xdl_int getAxis(const xdl_uint idx, XdevLAxis** axis) const;
			virtual xdl_uint getNumPOV() const;
			virtual xdl_int getPOV(const xdl_uint idx, XdevLPOV** pov) const;

			// --------------------------------------------------------------------------
			// XdevLButton functions
			//
			/// Returns the state of a button.
			virtual xdl_bool getPressed(const xdl_uint key) override;
			virtual xdl_bool getClicked(const xdl_uint key) override;
			virtual void setClickResponseTimeForAll(xdl_double crt) override;
			virtual void setClickResponseTime(const xdl_uint key, xdl_double crt) override;
			virtual xdl_double getClickResponseTime(const xdl_uint key) override;

			virtual void setAxisRangeMinMax(const xdl_uint axis, xdl_float min, xdl_float max) override;
			virtual void setAxisRangeMin(const xdl_uint axis, xdl_float min) override;
			virtual void seAxisRangeMax(const xdl_uint axis, xdl_float max) override;
			virtual void getAxisRangeMinMax(const xdl_uint axis, xdl_float* min, xdl_float* max) override;
			virtual xdl_float getAxisRangeMin(const xdl_uint axis) const override;
			virtual xdl_float getAxisRangeMax(const xdl_uint axis) const override;
	};





}

extern "C" XDEVL_EXPORT xdl::xdl_int _createKeyboardDevice(xdl::XdevLModuleCreateParameter* parameter);
extern "C" XDEVL_EXPORT void _deleteKeyboardDevice(xdl::XdevLModule* obj);
extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getKeyboardPluginInfo();

#endif
