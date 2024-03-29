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

#include "XdevLWindowImpl.h"
#include <XdevLXstring.h>
#include <XdevLCoreMediator.h>
#include <XdevLError.h>
#include <sstream>

namespace xdl {

	static xdl_uint windowCounter = 1;

	xdl_int initDefaultWindowInstances(xdl::XdevLCoreMediator* mediator) {

		if(xdl::windowEventServer == nullptr) {
			xdl::windowEventServer = static_cast<xdl::XdevLWindowEventServer*>(mediator->createModule(xdl::XdevLModuleName("XdevLWindowEventServer"), xdl::XdevLID("XdevLWindowEventServer")));
			if(nullptr == xdl::windowEventServer) {
				return ERR_ERROR;
			}
		}

		if(xdl::cursor == nullptr) {
			xdl::cursor = static_cast<xdl::XdevLCursor*>(mediator->createModule(xdl::XdevLModuleName("XdevLCursor"), xdl::XdevLID("XdevLCursor")));
			if(nullptr == xdl::cursor) {
				return ERR_ERROR;
			}
		}

		return ERR_OK;
	}

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
		m_rootTitle(""),
		m_isfullScreen(xdl_false),
		m_colorDepth(32),
		m_isPointerHidden(xdl_false) {

		// Set background color.
		m_backgroundColor[0] = 0;
		m_backgroundColor[1] = 0;
		m_backgroundColor[2] = 0;
		m_backgroundColor[3] = 255;

	}

	xdl_int XdevLWindowImpl::create(const XdevLWindowAttribute& attribute) {
		m_attribute = attribute;
		return ERR_OK;
	}

	xdl_int XdevLWindowImpl::notify(XdevLEvent& event) {

		switch(event.type) {
			case XDEVL_WINDOW_EVENT: {
				switch(event.window.event) {
					case XDEVL_WINDOW_MOVED: {
						m_attribute.position.x = event.window.x;
						m_attribute.position.y = event.window.y;
						m_attribute.size.width = event.window.width;
						m_attribute.size.height = event.window.height;
					}
					break;
					case XDEVL_WINDOW_RESIZED:  {
						m_attribute.position.x = event.window.x;
						m_attribute.position.y = event.window.y;
						m_attribute.size.width = event.window.width;
						m_attribute.size.height = event.window.height;
					}
					break;
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

	xdl_uint64 XdevLWindowImpl::getWindowID() {
		return m_id;
	}

	XdevLWindow* XdevLWindowImpl::getParent() {
		return m_rootWindow;
	}

	XdevLWindowSize::type XdevLWindowImpl::getWidth() {
		return m_attribute.size.width;
	}

	XdevLWindowSize::type XdevLWindowImpl::getHeight() {
		return m_attribute.size.height;
	}

	XdevLWindowPosition::type XdevLWindowImpl::getX() {
		return m_attribute.position.x;
	}

	XdevLWindowPosition::type XdevLWindowImpl::getY() {
		return m_attribute.position.y;
	}

	const XdevLWindowTitle& XdevLWindowImpl::getTitle() {
		return m_attribute.title;
	}

	const XdevLWindowPosition& XdevLWindowImpl::getPosition() {
		return m_attribute.position;
	}

	const XdevLWindowSize& XdevLWindowImpl::getSize() {
		return m_attribute.size;
	}

	xdl_bool XdevLWindowImpl::isFullscreen() {
		return m_isfullScreen;
	}

	xdl_int XdevLWindowImpl::getColorDepth() const {
		return m_colorDepth;
	}

	void XdevLWindowImpl::setX(XdevLWindowPosition::type x) {
		m_attribute.position.x = x;
	}

	void XdevLWindowImpl::setY(XdevLWindowPosition::type y) {
		m_attribute.position.y = y;
	}

	void XdevLWindowImpl::setWidth(XdevLWindowSize::type width) {
		m_attribute.size.width = width;
	}

	void XdevLWindowImpl::setHeight(XdevLWindowSize::type height) {
		m_attribute.size.height = height;
	}

	void XdevLWindowImpl::setColorDepth(int depth) {
		m_colorDepth = depth;
	}

	void XdevLWindowImpl::setTitle(const XdevLWindowTitle& title) {
		m_attribute.title = title;
	}

	xdl_bool XdevLWindowImpl::isPointerHidden() {
		return m_isPointerHidden;
	}

	void XdevLWindowImpl::setHidePointer(xdl_bool state) {
		m_isPointerHidden = state;
	}

	void XdevLWindowImpl::setParent(XdevLWindow* window) {
		m_rootWindow = window;
	}

	void XdevLWindowImpl::setWindowDecoration(xdl_bool enable) {

	}

	XdevLWindowTypes XdevLWindowImpl::getType() {
		return  m_attribute.type;
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
							m_attribute.title = XdevLString(child->GetText());
						if(child->ValueTStr() == "Fullscreen")
							m_isfullScreen = xstd::from_string<bool>(child->GetText());
						if(child->ValueTStr() == "X")
							m_attribute.position.x = xstd::from_string<int>(child->GetText());
						if(child->ValueTStr() == "Y")
							m_attribute.position.y = xstd::from_string<int>(child->GetText());
						if(child->ValueTStr() == "Width")
							m_attribute.size.width = xstd::from_string<int>(child->GetText());
						if(child->ValueTStr() == "Height")
							m_attribute.size.height = xstd::from_string<int>(child->GetText());
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
		if(getMediator() != nullptr) {
			TiXmlDocument xmlDocument;
			if(getMediator()->getXmlFilename()) {
				if(!xmlDocument.LoadFile(getMediator()->getXmlFilename())) {
					XDEVL_MODULE_WARNING("Could not parse xml file: " << getMediator()->getXmlFilename() << "\n");
					return ERR_OK;
				}

				if(readWindowInfo(xmlDocument) != ERR_OK)
					XDEVL_MODULE_WARNING("Some issues happend when parsing the XML file.\n");
				return ERR_OK;
			}
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
		if(tmp == "XDEVL_WINDOW_TYPE_NORMAL") {
			return XDEVL_WINDOW_TYPE_NORMAL;
		}

		return XDEVL_WINDOW_TYPE_UNKNOWN;
	}

	xdl_bool XdevLWindowImpl::isPointerInside() {
		return m_pointerIsInside;
	}

	void XdevLWindowImpl::setPosition(const XdevLWindowPosition& position) {
		m_attribute.position = position;
	}

	void XdevLWindowImpl::setSize(const XdevLWindowSize& size) {
		m_attribute.size = size;
	}

	//
	// -------------------------------------------------------------------------
	//

	XdevLWindowServerImpl::XdevLWindowServerImpl(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor) :
		XdevLModuleAutoImpl<XdevLWindowServer>(parameter, desriptor)  {

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
		if(it == m_windows.end()) {
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
		if(window == m_windows.end()) {
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
