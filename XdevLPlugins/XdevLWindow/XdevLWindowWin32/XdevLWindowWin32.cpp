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

#include <XdevLXstring.h>
#include <XdevLListener.h>
#include <XdevLError.h>
#include <XdevLInputSystem.h>
#include "XdevLWindowWin32.h"
#include <sstream>

xdl::XdevLPluginDescriptor windowWindowPluginDescriptor{
	xdl::windowPluginName,
	xdl::window_moduleNames,
	XDEVLWINDOWS_PLUGIN_MAJOR_VERSION,
	XDEVLWINDOWS_PLUGIN_MINOR_VERSION,
	XDEVLWINDOWS_PLUGIN_PATCH_VERSION
};

xdl::XdevLModuleDescriptor windowModuleDesc{
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[0],
	xdl::window_copyright,
	xdl::windowDescription,
	XDEVLWINDOWS_MODULE_MAJOR_VERSION,
	XDEVLWINDOWS_MODULE_MINOR_VERSION,
	XDEVLWINDOWS_MODULE_PATCH_VERSION
};

xdl::XdevLModuleDescriptor windowEventServerModuleDesc{
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[2],
	xdl::window_copyright,
	xdl::windowServerDescription,
	XDEVLWINDOWS_EVENT_SERVER_MODULE_MAJOR_VERSION,
	XDEVLWINDOWS_EVENT_SERVER_MODULE_MINOR_VERSION,
	XDEVLWINDOWS_EVENT_SERVER_MODULE_PATCH_VERSION
};

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter)  {

	if (windowModuleDesc.getName() == parameter->getModuleName()) {

		// If there is not event server first create one.
		if (xdl::windowEventServer == nullptr) {
			// If there is no even server active, create and activate it.
			xdl::windowEventServer = static_cast<xdl::XdevLWindowWindowsEventServer*>(parameter->getMediator()->createModule(xdl::XdevLModuleName("XdevLWindowEventServer"), xdl::XdevLID("XdevLWindowWindowsEventServerID"), xdl::XdevLPluginName("XdevLWindowWindow")));
		}

		xdl::XdevLWindowDeviceWin32* window = new xdl::XdevLWindowDeviceWin32(parameter);
		parameter->setModuleInstance(window);
	}
	else if (windowEventServerModuleDesc.getName() == parameter->getModuleName()) {
		xdl::windowEventServer = new xdl::XdevLWindowWindowsEventServer(parameter);
		parameter->setModuleInstance(xdl::windowEventServer);
		xdl::XdevLWindowEventServerParameter = parameter;
	}
	else {
		return xdl::ERR_MODULE_NOT_FOUND;
	}


	return xdl::ERR_OK;
}

extern "C" XDEVL_EXPORT void _delete(xdl::XdevLModule* obj) {
	if (obj)
		delete obj;
}

extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor()  {
	return &windowWindowPluginDescriptor;
}


namespace xdl {

	XdevLWindowDeviceWin32::XdevLWindowDeviceWin32(XdevLModuleCreateParameter* parameter) :
		XdevLWindowImpl(XdevLWindowImpl::getWindowsCounter(), parameter, windowModuleDesc),
		m_instance(NULL),
		m_DC(NULL),
		m_windowStyle(0),
		m_windowStyleEx(0),
		m_isHidden(xdl_false) {
	}


xdl_int XdevLWindowDeviceWin32::init() {
	if (XdevLWindowImpl::init() == ERR_ERROR) {
		return ERR_ERROR;
	}
	if (create() != ERR_OK) {
		return ERR_ERROR;
	}
	return ERR_OK;
}

int XdevLWindowDeviceWin32::create() {


	m_instance					=	GetModuleHandle(NULL);
	std::stringstream ss;
	ss << getTitle() << getID().getHashCode();
	m_winClassId = ss.str();
	WNDCLASSEX		WndClass;
	memset( &WndClass, 0, sizeof(WndClass) );
	WndClass.cbSize				= sizeof(WNDCLASSEX);
	WndClass.style				= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	WndClass.lpfnWndProc		= (WNDPROC)XdevLWindowWindowsEventServer::callbackProxy;
	WndClass.cbClsExtra			= 0;
	WndClass.cbWndExtra			= 0;
	WndClass.hInstance			= m_instance;
	WndClass.hIcon				= LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground		= (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName		= NULL;
	WndClass.lpszClassName		= m_winClassId.c_str();
	WndClass.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&WndClass)) {
		XDEVL_MODULE_ERROR("RegisterClassEx failed!\n");
		return ERR_ERROR;
	}

	int numDisplayDevices = 0;

	// Check The number of displays
	DISPLAY_DEVICE displayInfo;
	memset(&displayInfo, 0, sizeof(DISPLAY_DEVICE));
	displayInfo.cb = sizeof(DISPLAY_DEVICE);

	// Get all display devices.
	while (EnumDisplayDevices(NULL, numDisplayDevices, (PDISPLAY_DEVICE)&displayInfo, 0)) {
		displayInfoList.push_back(displayInfo);
		numDisplayDevices++;
	}


	bool desiredDevModeFound = false;
	if (m_fullScreen) {
		// Cache the current display mode so we can switch back when done.
		EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &m_oldDevMode );
		int nMode = 0;
		while ( EnumDisplaySettings( NULL, nMode++, &m_matchingVideoMode )) {
			// Does this XdevLModule mode support a 640 x 480 setting?
			if ( m_matchingVideoMode.dmPelsWidth  != getWidth() || m_matchingVideoMode.dmPelsHeight != getHeight())
				continue;

			// Does this XdevLModule mode support 32-bit color?
			if ( m_matchingVideoMode.dmBitsPerPel != static_cast<DWORD>(getColorDepth()))
				continue;

			// We found a match, but can it be set without rebooting?
			if ( ChangeDisplaySettingsEx(displayInfoList[0].DeviceName, &m_matchingVideoMode, NULL, CDS_TEST, NULL ) == DISP_CHANGE_SUCCESSFUL ) {
				desiredDevModeFound = true;
				break;
			}
		}
	}

	RECT rc;
	if (desiredDevModeFound) {
		// Let's set fullscreen properties.
		m_windowStyle 	= WS_POPUP;
		m_windowStyleEx = WS_EX_APPWINDOW;
		SetRect( &rc, 0, 0, getWidth(), getHeight() );
	}
	else {
		m_windowStyle 	= WS_OVERLAPPEDWINDOW;
		m_windowStyleEx = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		SetRect( &rc, 0, 0, getWidth(), getHeight()  );
	}
	// Ok, we do that because the user wants a client area width the dimenstion(widht, height)
	// and not the whole windows include the decoration of a windows.
	AdjustWindowRectEx( &rc, m_windowStyle, false, m_windowStyleEx);

	m_wnd = CreateWindowEx( m_windowStyleEx,
							WndClass.lpszClassName,
	                        getTitle(),
	                        m_windowStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
	                        desiredDevModeFound ? 0: getX(),
	                        desiredDevModeFound ? 0: getY(),
	                        (rc.right - rc.left),
	                        (rc.bottom - rc.top),
	                        NULL,
	                        NULL,
	                        m_instance,
	                        this );

	if (nullptr == m_wnd) {
		XDEVL_MODULE_ERROR("CreateWindowEx failed!\n");
		return ERR_ERROR;
	}

	if (m_fullScreen) {
		LONG ret = 0;
		if ((ret = ChangeDisplaySettingsEx(displayInfoList[0].DeviceName, &m_matchingVideoMode ,NULL, CDS_FULLSCREEN, NULL ))!= DISP_CHANGE_SUCCESSFUL) {
			XDEVL_MODULE_ERROR("Could not change video mode.\n");
			return ERR_ERROR;
		}
		// Set cursor into the middle of the window. Why?
		SetCursorPos((getWidth() >> 1), (getHeight() >> 1));
	}

	m_id = GetWindowLong(m_wnd, GWL_ID) + 1;
	windowEventServer->registerWindowForEvents(this);

	SetForegroundWindow(m_wnd);
	UpdateWindow(m_wnd);
	ShowWindow(m_wnd, SW_SHOWNORMAL);
	SetFocus(m_wnd);

	return ERR_OK;
}

void* XdevLWindowDeviceWin32::getInternal(const xdl_char* id) {
	std::string data(id);
	if (data == "WIN32_HWND")
		return m_wnd;
	if (data == "WIN32_DC")
		return m_DC;
	if (data == "WIN32_INSTANCE")
		return m_instance;
	return NULL;
}

xdl_int XdevLWindowDeviceWin32::shutdown() {

	if (XdevLWindowImpl::shutdown() == ERR_ERROR)
		return ERR_ERROR;

	// Was the window in fullscreen mode?
	if (m_fullScreen) {
		// Yes, change into the old device mode.
		ChangeDisplaySettingsEx(displayInfoList[0].DeviceName, &m_oldDevMode, NULL, 0, NULL );
	}
	// Destroy the window.
	if (m_wnd) {
		if (DestroyWindow(m_wnd) == 0)
			XDEVL_MODULE_ERROR("DestroyWindow failed!\n");
		m_wnd = NULL;
	}
	// Unregister window class.
	if (UnregisterClass(m_winClassId.c_str(), m_instance) == FALSE) {
		XDEVL_MODULE_ERROR("UnregisterClass failed!\n");
	}

	return ERR_OK;
}

xdl_int XdevLWindowDeviceWin32::reset() {
	shutdown();
	init();
	return ERR_OK;
}

xdl_int XdevLWindowDeviceWin32::update() {
	//MSG			msg;
	//if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}
	return ERR_OK;
}

//xdl_int XdevLWindowDeviceWin32::recvEvent(XdevLEvent* ev) {
//	// Return if no valid event specified
//	if (ev == NULL)
//		return ERR_ERROR;
//
//	if (*ev->msg == *m_XDEVL_MESSAGE_RESTART)
//		return reset();
//
//	if (*ev->msg == *XdevLWindowDeviceHide) {
//		ShowWindow(m_wnd, SW_HIDE);
//	}
//
//	if (*ev->msg == *XdevLWindowDeviceShow) {
//		SetForegroundWindow(m_wnd);
//		//UpdateWindow(m_wnd);
//		SetFocus(m_wnd);
//		ShowWindow(m_wnd, SW_SHOWNORMAL);
//	}
//
//	return XdevLModuleAutoImpl<XdevLWindow>::recvEvent(ev);
//}

void XdevLWindowDeviceWin32::show() {
	SetForegroundWindow(m_wnd);
	SetFocus(m_wnd);
	ShowWindow(m_wnd, SW_SHOWNORMAL);

	m_isHidden = xdl_false;
}

void XdevLWindowDeviceWin32::hide() {
	ShowWindow(m_wnd, SW_HIDE);
	m_isHidden = xdl_true;
}

xdl_bool XdevLWindowDeviceWin32::isHidden() {
	return m_isHidden;
}

void XdevLWindowDeviceWin32::raise() {
	if (::IsIconic(m_wnd)) {
		::ShowWindow(m_wnd, SW_RESTORE);
	}
	else {
		::SetActiveWindow(m_wnd);
	}
}
void XdevLWindowDeviceWin32::grabPointer() {
	SetCapture(m_wnd);
}
void XdevLWindowDeviceWin32::ungrabPointer() {
	ReleaseCapture();
}
void XdevLWindowDeviceWin32::grabKeyboard() {

}
void XdevLWindowDeviceWin32::ungrabKeyboard() {

}
void XdevLWindowDeviceWin32::setInputFocus() {
	::SetFocus(m_wnd);
}
xdl_bool  XdevLWindowDeviceWin32::hasFocus() {
	return (::GetFocus() != nullptr) ? xdl_true : xdl_false;
}

xdl_int XdevLWindowDeviceWin32::getInputFocus(XdevLWindow** window) {
	*window = nullptr;
	return ERR_ERROR;
}

XdevLWindowPosition::type XdevLWindowDeviceWin32::getX() {
	return m_position.x;
}

XdevLWindowPosition::type XdevLWindowDeviceWin32::getY() {
	return m_position.y;
}

XdevLWindowSize::type XdevLWindowDeviceWin32::getWidth() {
	return m_size.width;
}

XdevLWindowSize::type XdevLWindowDeviceWin32::getHeight() {
	return m_size.height;
}

const XdevLWindowSize& XdevLWindowDeviceWin32::getSize() {
	return m_size;
}

const XdevLWindowPosition& XdevLWindowDeviceWin32::getPosition() {
	return m_position;
}

const XdevLWindowTitle& XdevLWindowDeviceWin32::getTitle() {
	return m_title;
}

xdl_bool  XdevLWindowDeviceWin32::getFullscreen() {
	return XdevLWindowImpl::getFullscreen();
}

xdl_int  XdevLWindowDeviceWin32::getColorDepth() {
	return XdevLWindowImpl::getColorDepth();
}

xdl_bool  XdevLWindowDeviceWin32::getHidePointer() {
	return XdevLWindowImpl::getHidePointer();
}

void XdevLWindowDeviceWin32::setFullscreen(xdl_bool state) {

}

void XdevLWindowDeviceWin32::clipPointerPosition(xdl_uint x, xdl_uint y, xdl_uint width, xdl_uint height) {
	
}

void XdevLWindowDeviceWin32::setType(XdevLWindowTypes type) {
	switch (type) {
		case WINDOW_NORMAL: {

		}break;
		case WINDOW_TOOLTIP: {

		}break;
		case WINDOW_POPUP: {

		}break;
		case WINDOW_DROPDOWN_MENU: {

		}break;
		case WINDOW_SPLASH: {

		}break;
		case WINDOW_UNKNOWN:
		default: {
		
		}break;
	}
}

void XdevLWindowDeviceWin32::showPointer() {
	while (::ShowCursor(true) < 0);
}

void XdevLWindowDeviceWin32::hidePointer() {
	while (::ShowCursor(false) >= 0);
}

void XdevLWindowDeviceWin32::setPointerPosition(xdl_uint x, xdl_uint y) {
	::SetCursorPos(x, y);
}


void XdevLWindowDeviceWin32::setSize(const XdevLWindowSize& size) {
	m_size = size;
	SetWindowPos(m_wnd, HWND_TOP, m_position.x, m_position.y, m_size.width, m_size.height, SWP_NOMOVE);
}

void XdevLWindowDeviceWin32::setPosition(const XdevLWindowPosition& position) {
	m_position = position;
	SetWindowPos(m_wnd, HWND_TOP, m_position.x, m_position.y, m_size.width, m_size.height, SWP_NOSIZE);
}

void XdevLWindowDeviceWin32::setX(XdevLWindowPosition::type x) {
	XdevLWindowImpl::setX(x);
	SetWindowPos(m_wnd, HWND_TOP, m_position.x, m_position.y, m_size.width, m_size.height, SWP_NOSIZE);
}

void XdevLWindowDeviceWin32::setY(XdevLWindowPosition::type y) {
	XdevLWindowImpl::setY(y);
	SetWindowPos(m_wnd, HWND_TOP, m_position.x, m_position.y, m_size.width, m_size.height, SWP_NOSIZE);
}

void XdevLWindowDeviceWin32::setWidth(XdevLWindowSize::type width) {
	XdevLWindowImpl::setWidth(width);
	SetWindowPos(m_wnd, HWND_TOP, m_position.x, m_position.y, m_size.width, m_size.height, SWP_NOMOVE);
}

void XdevLWindowDeviceWin32::setHeight(XdevLWindowSize::type height) {
	XdevLWindowImpl::setHeight(height);
	SetWindowPos(m_wnd, HWND_TOP, m_position.x, m_position.y, m_size.width, m_size.height, SWP_NOMOVE);
}

void XdevLWindowDeviceWin32::setTitle(const XdevLWindowTitle& title) {
	XdevLWindowImpl::setTitle(title);

	SetWindowText(m_wnd, title.toString().c_str());
}

void XdevLWindowDeviceWin32::setHidePointer(xdl_bool state) {
	if (state)
		ShowCursor(TRUE);
	else
		ShowCursor(FALSE);
}



//
// -------------------------------------------------------------------------
//


XdevLWindowWindowsEventServer::XdevLWindowWindowsEventServer(XdevLModuleCreateParameter* parameter) :
XdevLWindowEventServerImpl(parameter, windowEventServerModuleDesc)
{}


xdl_int XdevLWindowWindowsEventServer::registerWindowForEvents(XdevLWindow* window) {
	return XdevLWindowEventServerImpl::registerWindowForEvents(window);
}

xdl_int XdevLWindowWindowsEventServer::unregisterWindowFromEvents(XdevLWindow* window) {
	return XdevLWindowEventServerImpl::unregisterWindowFromEvents(window);
}

xdl_int XdevLWindowWindowsEventServer::init() {
	XDEVL_MODULE_SUCCESS("SDL window created successfully" << std::endl);
	return ERR_OK;
}

void* XdevLWindowWindowsEventServer::getInternal(const XdevLInternalName& id) {

	return nullptr;
}

LRESULT CALLBACK XdevLWindowWindowsEventServer::callbackProxy(HWND hWnd, UINT	uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_CREATE) {
		intptr_t pThis = (intptr_t)((((LPCREATESTRUCT)lParam)->lpCreateParams));
		// This is for 32 and 64 bit systems.
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)(pThis));
	}
	intptr_t pInstance = (intptr_t)((GetWindowLongPtr(hWnd, GWLP_USERDATA)));
	XdevLWindowWindowsEventServer* obj = (XdevLWindowWindowsEventServer*)(pInstance);
	if (obj == 0)
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	return obj->callbackProc(hWnd, uMsg, wParam, lParam);
}

xdl_int XdevLWindowWindowsEventServer::shutdown() {
	XDEVL_MODULE_SUCCESS("Shutdown process was successful.\n");
	return ERR_OK;
}

xdl_int XdevLWindowWindowsEventServer::update() {
	MSG Msg;
	while (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return ERR_OK;
}

LRESULT  XdevLWindowWindowsEventServer::callbackProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	XdevLWindow* window = XdevLWindowEventServerImpl::getWindow(GetWindowLong(hWnd, GWL_ID));
	if (window == nullptr) {
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	switch (uMsg) {

		case WM_CREATE:
			break;
		case WM_ACTIVATE: {
		}break;
		case WM_CLOSE:
			break;
		case WM_SYSKEYDOWN:
			break;
		case WM_SIZE: {
			window->setSize(XdevLWindowSize(LOWORD(lParam), HIWORD(lParam)));
		}break;
		case WM_MOVE: {
			window->setPosition(XdevLWindowPosition(LOWORD(lParam), HIWORD(lParam)));
		}break;
		case WM_ENTERSIZEMOVE:{
			//OIS::InputManager::flushInputObjects(m_inputManager);
		}
		case WM_EXITSIZEMOVE:{
			//OIS::InputManager::flushInputObjects(m_inputManager);
			// FIXME: Hey please Cengiz, find another solution for that. At the moment all modules whould be reseted but it shoudl be only keyboard, mouse and joystick.
			//		XdevLQuark q("XDEVL_MESSAGE_RESET");
			//		XdevLEvent ev(this, NULL, &q, NULL);
			//		getMediator()->sendEvent(&ev);
		}
		case WM_SIZING:
			break;
		case WM_MOUSEMOVE: {
			// Register button up event in the Core.
			XdevLEvent ev;
			ev.common.timestamp = getMediator()->getTimer().getTime64();
			ev.type = XDEVL_MOUSE_MOTION;

			ev.motion.windowid = window->getWindowID();
			ev.motion.x = LOWORD(lParam);
			ev.motion.y = HIWORD(lParam);
			ev.motion.xrel = 0;
			ev.motion.yrel = 0;

			getMediator()->fireEvent(ev);
		}break;
		case WM_LBUTTONDOWN: {
			// Register button up event in the Core.
			XdevLEvent ev;
			ev.common.timestamp = getMediator()->getTimer().getTime64();
			ev.type = XDEVL_MOUSE_BUTTON_PRESSED;

			ev.button.windowid = window->getWindowID();
			ev.button.button = BUTTON_LEFT;
			ev.motion.x = LOWORD(lParam);
			ev.motion.y = HIWORD(lParam);

			getMediator()->fireEvent(ev);
		}break;
		case WM_LBUTTONUP: {
			// Register button up event in the Core.
			XdevLEvent ev;
			ev.common.timestamp = getMediator()->getTimer().getTime64();
			ev.type = XDEVL_MOUSE_BUTTON_RELEASED;

			ev.button.windowid = window->getWindowID();
			ev.button.button = BUTTON_LEFT;
			ev.motion.x = LOWORD(lParam);
			ev.motion.y = HIWORD(lParam);

			getMediator()->fireEvent(ev);
		}break;

		case WM_RBUTTONDOWN: {
			// Register button up event in the Core.
			XdevLEvent ev;
			ev.common.timestamp = getMediator()->getTimer().getTime64();
			ev.type = XDEVL_MOUSE_BUTTON_PRESSED;

			ev.button.windowid = window->getWindowID();
			ev.button.button = BUTTON_RIGHT;
			ev.motion.x = LOWORD(lParam);
			ev.motion.y = HIWORD(lParam);

			getMediator()->fireEvent(ev);
		}break;
		case WM_RBUTTONUP: {
			// Register button up event in the Core.
			XdevLEvent ev;
			ev.common.timestamp = getMediator()->getTimer().getTime64();
			ev.type = XDEVL_MOUSE_BUTTON_RELEASED;

			ev.button.windowid = window->getWindowID();
			ev.button.button = BUTTON_RIGHT;
			ev.motion.x = LOWORD(lParam);
			ev.motion.y = HIWORD(lParam);

			getMediator()->fireEvent(ev);
		}break;

		case WM_MBUTTONUP: {
			// Register button up event in the Core.
			XdevLEvent ev;
			ev.common.timestamp = getMediator()->getTimer().getTime64();
			ev.type = XDEVL_MOUSE_BUTTON_PRESSED;

			ev.button.windowid = window->getWindowID();
			ev.button.button = BUTTON_MIDDLE;
			ev.motion.x = LOWORD(lParam);
			ev.motion.y = HIWORD(lParam);

			getMediator()->fireEvent(ev);
		}break;
		case WM_MBUTTONDOWN: {
			// Register button up event in the Core.
			XdevLEvent ev;
			ev.common.timestamp = getMediator()->getTimer().getTime64();
			ev.type = XDEVL_MOUSE_BUTTON_RELEASED;

			ev.button.windowid = window->getWindowID();
			ev.button.button = BUTTON_MIDDLE;
			ev.motion.x = LOWORD(lParam);
			ev.motion.y = HIWORD(lParam);

			getMediator()->fireEvent(ev);
		}break;
		default:
			break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);

}

void XdevLWindowWindowsEventServer::flush() {
//	pollEvents();
}



}
