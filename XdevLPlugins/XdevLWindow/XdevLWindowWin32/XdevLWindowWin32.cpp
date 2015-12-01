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
#include "XdevLWindowDeviceWin32.h"
#include <sstream>


extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	if (std::string(xdl::XdevLWindowDeviceImpl::m_windowModuleDesc.getName()) == parameter->getModuleName()) {
		xdl::XdevLWindowDeviceWin32* obj = new xdl::XdevLWindowDeviceWin32(parameter);
		if (!obj)
			return xdl::ERR_ERROR;
		parameter->setModuleInstance(obj);

		return xdl::ERR_OK;
	}
	return xdl::ERR_MODULE_NOT_FOUND;
}


namespace xdl {

LRESULT CALLBACK XdevLWindowDeviceWin32::callbackProxy(HWND hWnd, UINT	uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_CREATE) {
		intptr_t pThis = (intptr_t)((((LPCREATESTRUCT)lParam)->lpCreateParams));
		// This is for 32 and 64 bit systems.
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)(pThis));
	}
	intptr_t pInstance = (intptr_t)((GetWindowLongPtr(hWnd, GWLP_USERDATA)));
	XdevLWindowDeviceWin32* obj = (XdevLWindowDeviceWin32*)(pInstance);
	if (obj == 0)
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	return obj->callbackProc(hWnd, uMsg, wParam, lParam);
}

LRESULT  XdevLWindowDeviceWin32::callbackProc(HWND hWnd, UINT	uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CREATE:
		break;

	case WM_ACTIVATE: {
	}
	break;
	case WM_CLOSE:
		break;
	case WM_SYSKEYDOWN:
		break;
	case WM_SIZE: {
		m_width 	= LOWORD(lParam);
		m_height 	= HIWORD(lParam);
	}
	case WM_MOVE: {
		m_x 	= LOWORD(lParam);
		m_y 	= HIWORD(lParam);
	}
	
	break;
	case WM_ENTERSIZEMOVE:{
		//OIS::InputManager::flushInputObjects(m_inputManager);
	}
	case WM_EXITSIZEMOVE:{
		//OIS::InputManager::flushInputObjects(m_inputManager);
		// FIXME: Hey please Cengiz, find another solution for that. At the moment all modules whould be reseted but it shoudl be only keyboard, mouse and joystick.
		XdevLQuark q("XDEVL_MESSAGE_RESET");
		XdevLEvent ev(this, NULL, &q, NULL);
		getMediator()->sendEvent(&ev);
	}	
	case WM_SIZING:
		break;
	default:
		break;
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam);
}

xdl_int XdevLWindowDeviceWin32::init() {
	if (XdevLWindowDeviceImpl::init() == ERR_ERROR) {
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
	ss << getTitle() << getId()->identifier.hashCode;
	m_winClassId = ss.str();
	WNDCLASSEX		WndClass;
	memset( &WndClass, 0, sizeof(WndClass) );
	WndClass.cbSize					=	sizeof(WNDCLASSEX);
	WndClass.style					=	CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	WndClass.lpfnWndProc		=	(WNDPROC)XdevLWindowDeviceWin32::callbackProxy;
	WndClass.cbClsExtra			=	0;
	WndClass.cbWndExtra			=	0;
	WndClass.hInstance			=	m_instance;
	WndClass.hIcon					=	LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor				= LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground	=	(HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName		=	NULL;
	WndClass.lpszClassName	=	m_winClassId.c_str();
	WndClass.hIconSm				=	LoadIcon(NULL, IDI_APPLICATION);

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
	                        m_winClassId.c_str(),
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

	if (!m_wnd) {
		XDEVL_MODULE_ERROR("CreateWindowEx failed!\n");
		return ERR_ERROR;
	}

	// Prepare parameter for the OIS input manager.
	OIS::ParamList pl;
	std::ostringstream windowHndStr;
	windowHndStr << (size_t)(m_wnd);
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	if (!m_hideMouse) {
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	}
	else {
		ShowCursor(FALSE);
	}

	// Insert the following right before calling mInputSystem = OIS::InputManager::createInputSystem( paramList );
	if (!getFullscreen()) {
		pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
		pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
		pl.insert(std::make_pair(std::string("w32_joystick"), std::string("DISCL_FOREGROUND")));
		pl.insert(std::make_pair(std::string("w32_joystick"), std::string("DISCL_NONEXCLUSIVE")));
	}

	// Create OIS input manager.
	m_inputManager = OIS::InputManager::createInputSystem( pl );
	if (!m_inputManager) {
		XDEVL_MODULE_ERROR("Could not create input system.\n");
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


	SetForegroundWindow(m_wnd);
	UpdateWindow(m_wnd);
	SetFocus(m_wnd);

	ShowWindow(m_wnd, SW_SHOWNORMAL);

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
	if (data == "OIS_INPUTMANAGER")
		return m_inputManager;
	return NULL;
}

xdl_int XdevLWindowDeviceWin32::shutdown() {

	if (XdevLWindowDeviceImpl::shutdown() == ERR_ERROR)
		return ERR_ERROR;

	OIS::InputManager::destroyInputSystem(m_inputManager);

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
	MSG			msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return ERR_OK;
}

xdl_int XdevLWindowDeviceWin32::recvEvent(XdevLEvent* ev) {
	// Return if no valid event specified
	if (ev == NULL)
		return ERR_ERROR;

	if (*ev->msg == *m_XDEVL_MESSAGE_RESTART)
		return reset();

	if (*ev->msg == *XdevLWindowDeviceHide) {
		ShowWindow(m_wnd, SW_HIDE);
	}

	if (*ev->msg == *XdevLWindowDeviceShow) {
		SetForegroundWindow(m_wnd);
		//UpdateWindow(m_wnd);
		SetFocus(m_wnd);
		ShowWindow(m_wnd, SW_SHOWNORMAL);
	}

	return XdevLModuleAutoImpl<XdevLWindow>::recvEvent(ev);
}

xdl_int XdevLWindowDeviceWin32::show(xdl_bool state) {
	if (state) {
		SetForegroundWindow(m_wnd);
		SetFocus(m_wnd);
		ShowWindow(m_wnd, SW_SHOWNORMAL);


	}
	else {
		ShowWindow(m_wnd, SW_HIDE);
	}
	return ERR_OK;
}

void XdevLWindowDeviceWin32::setFullscreen(xdl_bool state) {

}

void XdevLWindowDeviceWin32::setX(xdl_uint x) {
	XdevLWindowDeviceImpl::setX(x);
	SetWindowPos(m_wnd, HWND_TOP, m_x, m_y, m_width, m_height, SWP_NOSIZE);
}

void XdevLWindowDeviceWin32::setY(xdl_uint y) {
	XdevLWindowDeviceImpl::setY(y);
	SetWindowPos(m_wnd, HWND_TOP, m_x, m_y, m_width, m_height, SWP_NOSIZE);
}

void XdevLWindowDeviceWin32::setWidth(xdl_uint width) {
	XdevLWindowDeviceImpl::setWidth(width);
	SetWindowPos(m_wnd, HWND_TOP, m_x, m_y, m_width, m_height, SWP_NOMOVE);
}

void XdevLWindowDeviceWin32::setHeight(xdl_uint height) {
	XdevLWindowDeviceImpl::setHeight(height);
	SetWindowPos(m_wnd, HWND_TOP, m_x, m_y, m_width, m_height, SWP_NOMOVE);
}

void XdevLWindowDeviceWin32::setTitle(const xdl_char* title) {
	XdevLWindowDeviceImpl::setTitle(title);
	SetWindowText (m_wnd, m_title.c_str());
}

void XdevLWindowDeviceWin32::setHidePointer(xdl_bool state) {
	if (state)
		ShowCursor(TRUE);
	else
		ShowCursor(FALSE);
}

}
