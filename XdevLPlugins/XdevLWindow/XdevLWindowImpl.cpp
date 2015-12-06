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

#include "XdevLWindowImpl.h"
#include <XdevLXstring.h>
#include <XdevLCoreMediator.h>
#include <XdevLError.h>
#include <sstream>

namespace xdl {

	static xdl_uint windowCounter = 1;

	xdl::XdevLModuleDescriptor xdl::XdevLWindowServerImpl::m_windowServerModuleDesc {
		window_vendor,
		window_author,
		window_moduleNames[1],
		window_copyright,
		windowServerDescription,
		XdevLWindowServerMajorVersion,
		XdevLWindowServerMinorVersion,
		XdevLWindowServerPatchVersion
	};

	xdl::XdevLWindowEventServer* windowEventServer = nullptr;
	xdl::XdevLCursor* cursor = nullptr;
	xdl::XdevLModuleCreateParameter* XdevLWindowEventServerParameter;

	static xdl_bool eventThreadIsRunning = xdl_false;

	static thread::Mutex m_mutexThreadIsRunning;

	xdl_bool isEventThreadRunning() {
		thread::XdevLScopeLock lock(m_mutexThreadIsRunning);
		return eventThreadIsRunning;
	}

	void startEventThread() {
		thread::XdevLScopeLock lock(m_mutexThreadIsRunning);
		eventThreadIsRunning = xdl_true;
	}

	void stopEventThread() {
		thread::XdevLScopeLock lock(m_mutexThreadIsRunning);
		eventThreadIsRunning = xdl_false;
	}


	XdevLWindowImpl::XdevLWindowImpl(xdl_uint id, XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor) :
		XdevLModuleAutoImpl<XdevLWindow>(parameter, desriptor),
		m_rootWindow(NULL),
		m_id(id),
		m_title("XdevL (c) 2007 - 2009 Cengiz Terzibas"),
		m_rootTitle(""),
		m_fullScreen(xdl_false),
		m_colorDepth(32),
		m_hideMouse(xdl_false),
		m_border(xdl_true),
		m_windowType(WINDOW_NORMAL) {

		m_position.x = 0;
		m_position.y = 0;
		m_size.width = 640;
		m_size.height = 480;

		// Set background color.
		m_backgroundColor[0] = 0;
		m_backgroundColor[1] = 0;
		m_backgroundColor[2] = 0;
		m_backgroundColor[3] = 255;

	}

	xdl_int XdevLWindowImpl::notify(XdevLEvent& event) {

		switch(event.type) {
			case XDEVL_WINDOW_EVENT: {
				switch(event.window.event) {
					case XDEVL_WINDOW_MOVED: {
						m_position.x = event.window.x;
						m_position.y = event.window.y;
						m_size.width = event.window.width;
						m_size.height = event.window.height;
					}break;
					case XDEVL_WINDOW_RESIZED:  {
						m_position.x = event.window.x;
						m_position.y = event.window.y;
						m_size.width = event.window.width;
						m_size.height = event.window.height;
					} break;
				}
			}
			break;
		}

		return 	XdevLModuleAutoImpl::notify(event);;
	}

	void XdevLWindowImpl::increaseWindowCounter() {
		windowCounter++;
	}

	xdl_uint XdevLWindowImpl::getWindowsCounter() {
		return windowCounter++;
	}

	xdl_bool XdevLWindowImpl::getWindowBorder() {
		return m_border;
	}

	xdl_uint64 XdevLWindowImpl::getWindowID() {
		return m_id;
	}

	XdevLWindowSize::type XdevLWindowImpl::getWidth() const {
		return m_size.width;
	}

	XdevLWindowSize::type XdevLWindowImpl::getHeight() const {
		return m_size.height;
	}

	XdevLWindowPosition::type XdevLWindowImpl::getX() const {
		return m_position.x;
	}

	XdevLWindowPosition::type XdevLWindowImpl::getY() const {
		return m_position.y;
	}

	const XdevLWindowTitle& XdevLWindowImpl::getTitle() {
		return m_title;
	}

	xdl_bool XdevLWindowImpl::getFullscreen() const {
		return m_fullScreen;
	}

	xdl_int XdevLWindowImpl::getColorDepth() const {
		return m_colorDepth;
	}

	void XdevLWindowImpl::setX(XdevLWindowPosition::type x) {
		m_position.x = x;
	}

	void XdevLWindowImpl::setY(XdevLWindowPosition::type y) {
		m_position.y = y;
	}

	void XdevLWindowImpl::setWidth(XdevLWindowSize::type width) {
		m_size.width = width;
	}

	void XdevLWindowImpl::setHeight(XdevLWindowSize::type height) {
		m_size.height = height;
	}

	void XdevLWindowImpl::setColorDepth(int depth) {
		m_colorDepth = depth;
	}

	void XdevLWindowImpl::setTitle(const XdevLWindowTitle& title) {
		m_title = title;
	}

	xdl_bool XdevLWindowImpl::getHidePointer() const {
		return m_hideMouse;
	}

	void XdevLWindowImpl::setHidePointer(xdl_bool state) {
		m_hideMouse = state;
	}

	void XdevLWindowImpl::setParent(XdevLWindow* window) {
		m_rootWindow = window;
	}

	void XdevLWindowImpl::setWindowDecoration(xdl_bool enable) {
		m_border = enable;
	}

	int XdevLWindowImpl::readWindowInfo(TiXmlDocument& document) {
		TiXmlHandle docHandle(&document);
		TiXmlElement* root = docHandle.FirstChild(XdevLCorePropertiesName.c_str()).FirstChildElement("XdevLWindow").ToElement();
		if(!root) {
			XDEVL_MODULE_INFO("<XdevLWindow> section not found. Using default values for the device.\n");
			return ERR_OK;
		}

		while(root != NULL) {

			// Does the user specified the id of the module?
			if(root->Attribute("id")) {
				XdevLID id(root->Attribute("id"));

				// Do only if we have the same ID.
				// TODO Maybe change comparison into string comparision.
				if(getID() == id) {
					TiXmlElement* child = nullptr;
					for(child = root->FirstChildElement(); child; child = child->NextSiblingElement()) {
						if(child->ValueTStr() == "Root")
							m_rootTitle = XdevLString(child->GetText());
						if(child->ValueTStr() == "Title")
							m_title = XdevLString(child->GetText());
						if(child->ValueTStr() == "Fullscreen")
							m_fullScreen = xstd::from_string<bool>(child->GetText());
						if(child->ValueTStr() == "X")
							m_position.x = xstd::from_string<int>(child->GetText());
						if(child->ValueTStr() == "Y")
							m_position.y = xstd::from_string<int>(child->GetText());
						if(child->ValueTStr() == "Width")
							m_size.width = xstd::from_string<int>(child->GetText());
						if(child->ValueTStr() == "Height")
							m_size.height = xstd::from_string<int>(child->GetText());
						if(child->ValueTStr() == "HidePointer")
							m_hideMouse = xstd::from_string<xdl_bool>(child->GetText());
						if(child->ValueTStr() == "Border")
							m_border = xstd::from_string<xdl_bool>(child->GetText());
						if(child->ValueTStr() == "BackgroundColor") {
							std::vector<std::string> list;
							xstd::tokenize(child->GetText(), list);
							std::stringstream r(list[0]);
							std::stringstream g(list[1]);
							std::stringstream b(list[2]);
							std::stringstream a(list[3]);
							r >> m_backgroundColor[0];
							g >> m_backgroundColor[1];
							b >> m_backgroundColor[2];
							a >> m_backgroundColor[3];
						}
					}
				}
				root = root->NextSiblingElement();
			}
		}
		return ERR_OK;
	}

	xdl_int XdevLWindowImpl::init() {
		TiXmlDocument xmlDocument;
		if(getMediator()->getXmlFilename()) {
			if(!xmlDocument.LoadFile(getMediator()->getXmlFilename())) {
				std::cerr << "Could not parse xml file: " << getMediator()->getXmlFilename() << std::endl;
				return ERR_ERROR;
			}

			if(readWindowInfo(xmlDocument) != ERR_OK)
				return ERR_ERROR;
		}

		return ERR_OK;
	}

	xdl_int XdevLWindowImpl::shutdown() {

		if(nullptr == windowEventServer) {
			return ERR_OK;
		}

		// TODO We flush here to receive pending events before we unregister from the events server. Maybe harmful?
		windowEventServer->flush();
		windowEventServer->unregisterWindowFromEvents(this);

		return ERR_OK;
	}

	xdl_int XdevLWindowImpl::stringToWindowType(const std::string& tmp) {
		if(tmp == "WINDOW_NORMAL") {
			return WINDOW_NORMAL;
		}

		return WINDOW_UNKNOWN;
	}

	xdl_bool XdevLWindowImpl::isPointerInside() {
		return m_pointerIsInside;
	}

	//
	// -------------------------------------------------------------------------
	//

	XdevLWindowServerImpl::XdevLWindowServerImpl(XdevLModuleCreateParameter* parameter) :
		XdevLModuleAutoImpl<XdevLWindowServer>(parameter, m_windowServerModuleDesc)  {

	}

	XdevLWindowServerImpl::~XdevLWindowServerImpl() {

	}

	xdl_int XdevLWindowServerImpl::destroy(XdevLWindow* window) {
		assert((window) && "XdevLWindowServerImpl::destroy: window == nullptr");

		// Find in the map and remove it and free all resources.
		auto tmp = m_windowList.find(window->getWindowID());

		if(tmp != m_windowList.end()) {
			m_windowList.erase(tmp);

			// We do not destroy using event system. Directly delete which is handles in the descructor.
			delete tmp->second;

			return ERR_OK;
		}
		return ERR_ERROR;
	}



	//
	// -------------------------------------------------------------------------
	//

	XdevLWindowEventServerImpl::XdevLWindowEventServerImpl(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		XdevLModuleAutoImpl<XdevLWindowEventServer>(parameter, descriptor) {

	}

	xdl_bool XdevLWindowEventServerImpl::isWindowRegistered(XdevLWindow* window) {
		WindowEventMapType::iterator it = m_windows.find(window->getWindowID());
		if (it == m_windows.end()) {
			return xdl_false;
		}
		return xdl_true;
	}

	xdl_int XdevLWindowEventServerImpl::registerWindowForEvents(XdevLWindow* window) {
		WindowEventMapType::iterator it = m_windows.find(window->getWindowID());
		if(it == m_windows.end()) {
			m_windows[window->getWindowID()] = window;
			return ERR_OK;
		}
		return ERR_ERROR;
	}

	xdl_int XdevLWindowEventServerImpl::unregisterWindowFromEvents(XdevLWindow* window) {
		WindowEventMapType::iterator it = m_windows.find(window->getWindowID());
		if(it != m_windows.end()) {
			m_windows.erase(it);
			return ERR_OK;
		}
		return ERR_ERROR;
	}

	XdevLWindow* XdevLWindowEventServerImpl::getWindow(xdl_uint64 id) {
		auto window = m_windows.find(id);
		if (window == m_windows.end()) {
			return nullptr;
		}
		return window->second;
	}

	XdevLWindow* XdevLWindowEventServerImpl::getFocus() const {
		return m_focusWindow;
	}

	void XdevLWindowEventServerImpl::focusGained(XdevLWindow* window) {
		m_focusWindow = window;
	}

	void XdevLWindowEventServerImpl::flush() {

	}


}
