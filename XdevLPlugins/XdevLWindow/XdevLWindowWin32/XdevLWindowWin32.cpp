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

#if XDEVL_PLATFORM_MINGW
#define MAPVK_VSC_TO_VK_EX 3
#endif

xdl::XdevLPluginDescriptor windowWindowPluginDescriptor {
	xdl::windowPluginName,
	xdl::window_moduleNames,
	XDEVLWINDOWS_PLUGIN_MAJOR_VERSION,
	XDEVLWINDOWS_PLUGIN_MINOR_VERSION,
	XDEVLWINDOWS_PLUGIN_PATCH_VERSION
};

xdl::XdevLModuleDescriptor windowModuleDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_WINDOW_MODULE_NAME],
	xdl::window_copyright,
	xdl::windowDescription,
	XDEVLWINDOWS_MODULE_MAJOR_VERSION,
	XDEVLWINDOWS_MODULE_MINOR_VERSION,
	XDEVLWINDOWS_MODULE_PATCH_VERSION
};

xdl::XdevLModuleDescriptor windowEventServerModuleDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_WINDOW_EVENT_SERVER_MODULE_NAME],
	xdl::window_copyright,
	xdl::windowServerDescription,
	XDEVLWINDOWS_EVENT_SERVER_MODULE_MAJOR_VERSION,
	XDEVLWINDOWS_EVENT_SERVER_MODULE_MINOR_VERSION,
	XDEVLWINDOWS_EVENT_SERVER_MODULE_PATCH_VERSION
};

xdl::XdevLModuleDescriptor windowCursorModuleDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_CURSOR_MODULE_NAME],
	xdl::window_copyright,
	xdl::windowServerDescription,
	XDEVLWINDOWS_CURSOR_MODULE_MAJOR_VERSION,
	XDEVLWINDOWS_CURSOR_MODULE_MINOR_VERSION,
	XDEVLWINDOWS_CURSOR_MODULE_PATCH_VERSION
};

static xdl::XdevLCursorWindows* cursorWindows = nullptr;
static xdl::xdl_uint64 windowID = 0;

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter)  {

	//
	// Create XdevLWindow instance.
	//
	if (windowModuleDesc.getName() == parameter->getModuleName()) {

		// Initialize once default instances
		if (xdl::initDefaultWindowInstances(parameter) != xdl::ERR_OK) {
			return xdl::ERR_ERROR;
		}

		xdl::XdevLWindowDeviceWin32* window = new xdl::XdevLWindowDeviceWin32(parameter);
		parameter->setModuleInstance(window);
	}

	//
	// Create XdevLWindowServer instance.
	//
	else if (xdl::XdevLWindowServerImpl::m_windowServerModuleDesc.getName() == parameter->getModuleName()) {

		// Initialize once default instances
		if (xdl::initDefaultWindowInstances(parameter) != xdl::ERR_OK) {
			return xdl::ERR_ERROR;
		}

		xdl::XdevLWindowServerWindows* windowServer = new xdl::XdevLWindowServerWindows(parameter);
		parameter->setModuleInstance(windowServer);
	}

	//
	// Create XdevLWindowEventServer instance.
	//
	else if (windowEventServerModuleDesc.getName() == parameter->getModuleName()) {

		xdl::windowEventServer = new xdl::XdevLWindowWindowsEventServer(parameter);
		parameter->setModuleInstance(xdl::windowEventServer);
		xdl::XdevLWindowEventServerParameter = parameter;
	}

	//
	// Create XdevLCursor instance.
	//
	else if (windowCursorModuleDesc.getName() == parameter->getModuleName()) {

		cursorWindows = new xdl::XdevLCursorWindows(parameter);
		xdl::cursor = cursorWindows;
		parameter->setModuleInstance(xdl::cursor);

	} else {
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

	static const XdevLID ButtonPressed("XDEVL_BUTTON_PRESSED");
	static const XdevLID ButtonReleased("XDEVL_BUTTON_RELEASED");
	static const XdevLID MouseButtonPressed("XDEVL_MOUSE_BUTTON_PRESSED");
	static const XdevLID MouseButtonReleased("XDEVL_MOUSE_BUTTON_RELEASED");
	static const XdevLID MouseMotion("XDEVL_MOUSE_MOTION");
	static const XdevLID WindowEvent("XDEVL_WINDOW_EVENT");



	XdevLButtonId WindowsVirtualKeyToXdevLKeyCode(WPARAM wParam, LPARAM lParam) {
		switch (wParam) {
			case VK_BACK:
				return KEY_BACKSPACE;
			case VK_TAB:
				return KEY_TAB;
//			case VK_CLEAR: return ? ? ? ;
			case VK_RETURN:
				return KEY_ENTER;
			case VK_SHIFT: {
				WPARAM tmp = MapVirtualKey(((lParam & 0x00ff0000) >> 16), MAPVK_VSC_TO_VK_EX);
				return ((tmp == 0) ? KEY_LSHIFT : KEY_RSHIFT);
			}
			case VK_CONTROL: {
				WPARAM tmp = (lParam & 0x1000000);
				return ((tmp == 0) ? KEY_LCTRL : KEY_RCTRL);
			}
			case VK_MENU: {
				WPARAM tmp = (lParam & 0x1000000);
				return ((tmp == 0) ? KEY_LALT : KEY_RALT);
			}
			case VK_PAUSE:
				return KEY_PAUSE;
			case VK_CAPITAL:
				return KEY_CAPSLOCK;
//			case VK_KANA: return ? ? ? ; // Kana  mode
//			case VK_HANGUEL: return ? ? ? ;  // IME Hanguel mode (maintained for compatibility; use VK_HANGUL)
//			case VK_HANGUL: return ? ? ? ;  // Hangul mode
//			case VK_JUNJA: return ? ? ? ;  // Junja mode
//			case VK_FINAL: return ? ? ? ; // Final mode
//			case VK_HANJA: return ? ? ? ; // Hanja mode
//			case VK_KANJI: return ? ? ? ; // Kanji mode
			case VK_ESCAPE:
				return KEY_ESCAPE;
//			case VK_CONVERT: return ? ? ? ; // IME convert
//			case VK_NONCONVERT: return ? ? ? ; // IME nonconvert
//			case VK_ACCEPT: return ? ? ? ; // IME accept
//			case VK_MODECHANGE: return ? ? ? ; // IME mode change request
			case VK_SPACE:
				return KEY_SPACE;
			case VK_PRIOR:
				return KEY_PAGEUP;
			case VK_NEXT:
				return KEY_PAGEDOWN;
			case VK_END:
				return KEY_END;
			case VK_HOME:
				return KEY_HOME;
			case VK_LEFT:
				return KEY_LEFT;
			case VK_UP:
				return KEY_UP;
			case VK_RIGHT:
				return KEY_RIGHT;
			case VK_DOWN:
				return KEY_DOWN;
//			case VK_SELECT: return ??? // The select key
//			case VK_PRINT: return ???;
//			case VK_EXECUTE: return ???;
			case VK_SNAPSHOT:
				return KEY_PRINTSCREEN;
			case VK_INSERT:
				return KEY_INSERT;
			case VK_DELETE:
				return KEY_DELETE;
//				case VK_HELP:: return ? ? ? ;

			case 0x30:
				return KEY_0;
			case 0x31:
				return KEY_1;
			case 0x32:
				return KEY_2;
			case 0x33:
				return KEY_3;
			case 0x34:
				return KEY_4;
			case 0x35:
				return KEY_5;
			case 0x36:
				return KEY_6;
			case 0x37:
				return KEY_7;
			case 0x38:
				return KEY_8;
			case 0x39:
				return KEY_9;

			case 0x41:
				return KEY_A;
			case 0x42:
				return KEY_B;
			case 0x43:
				return KEY_C;
			case 0x44:
				return KEY_D;
			case 0x45:
				return KEY_E;
			case 0x46:
				return KEY_F;
			case 0x47:
				return KEY_G;
			case 0x48:
				return KEY_H;
			case 0x49:
				return KEY_I;

			case 0xaa:
				return KEY_J;
			case 0x4b:
				return KEY_K;
			case 0x4c:
				return KEY_L;
			case 0x4d:
				return KEY_M;
			case 0x4e:
				return KEY_N;
			case 0x4f:
				return KEY_O;
			case 0x50:
				return KEY_P;
			case 0x51:
				return KEY_Q;
			case 0x52:
				return KEY_R;
			case 0x53:
				return KEY_S;
			case 0x54:
				return KEY_T;
			case 0x55:
				return KEY_U;
			case 0x56:
				return KEY_V;
			case 0x57:
				return KEY_W;
			case 0x58:
				return KEY_X;
			case 0x59:
				return KEY_Y;
			case 0x5a:
				return KEY_Z;

			case VK_LWIN:
				return KEY_LGUI;
			case VK_RWIN:
				return KEY_RGUI;
//			case VK_APPS: return ? ? ? ;
//			case VK_SLEEP: return ? ? ? ;

			case VK_NUMPAD0:
				return KEY_KP_0;
			case VK_NUMPAD1:
				return KEY_KP_1;
			case VK_NUMPAD2:
				return KEY_KP_2;
			case VK_NUMPAD3:
				return KEY_KP_3;
			case VK_NUMPAD4:
				return KEY_KP_4;
			case VK_NUMPAD5:
				return KEY_KP_5;
			case VK_NUMPAD6:
				return KEY_KP_6;
			case VK_NUMPAD7:
				return KEY_KP_7;
			case VK_NUMPAD8:
				return KEY_KP_8;
			case VK_NUMPAD9:
				return KEY_KP_9;

			case VK_MULTIPLY:
				return KEY_KP_MULTIPLY;
			case VK_ADD:
				return KEY_KP_PLUS;
				//			case VK_SEPARATOR: return ? ? ? ;
			case VK_SUBTRACT:
				return KEY_KP_MINUS;
			case VK_DECIMAL:
				return KEY_KP_PERIOD;
			case VK_DIVIDE:
				return KEY_KP_DIVIDE;

			case VK_F1:
				return KEY_F1;
			case VK_F2:
				return KEY_F2;
			case VK_F3:
				return KEY_F3;
			case VK_F4:
				return KEY_F4;
			case VK_F5:
				return KEY_F5;
			case VK_F6:
				return KEY_F6;
			case VK_F7:
				return KEY_F7;
			case VK_F8:
				return KEY_F8;
			case VK_F9:
				return KEY_F9;
			case VK_F10:
				return KEY_F10;
			case VK_F11:
				return KEY_F11;
			case VK_F12:
				return KEY_F12;
			case VK_F13:
				return KEY_F13;
			case VK_F14:
				return KEY_F14;
			case VK_F15:
				return KEY_F15;
			case VK_F16:
				return KEY_F16;
			case VK_F17:
				return KEY_F17;
			case VK_F18:
				return KEY_F18;
			case VK_F19:
				return KEY_F19;
			case VK_F20:
				return KEY_F20;
			case VK_F21:
				return KEY_F21;
			case VK_F22:
				return KEY_F22;
			case VK_F23:
				return KEY_F23;

			case VK_NUMLOCK:
				return KEY_KP_NUM_LOCK;
			case VK_SCROLL:
				return KEY_SCROLLLOCK;
			case VK_LSHIFT:
				return KEY_LSHIFT;
			case VK_RSHIFT:
				return KEY_RSHIFT;
			case VK_LCONTROL:
				return KEY_LCTRL;
			case VK_RCONTROL:
				return KEY_RCTRL;
			case VK_LMENU:
				return KEY_LALT;
			case VK_RMENU:
				return KEY_RALT;

				// Still others left.
			default:
				break;
		}
		return BUTTON_UNKOWN;
	}

	XdevLWindowDeviceWin32::XdevLWindowDeviceWin32(XdevLModuleCreateParameter* parameter) :
		XdevLWindowImpl(XdevLWindowImpl::getWindowsCounter(), parameter, windowModuleDesc),
		m_instance(NULL),
		m_DC(NULL),
		m_windowStyle(0),
		m_windowStyleEx(0),
		m_isHidden(xdl_false) {
	}

	XdevLWindowDeviceWin32::~XdevLWindowDeviceWin32() {

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

	xdl_int XdevLWindowDeviceWin32::create() {


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


		if ((m_windowType == WINDOW_TOOLTIP) || (m_windowType == WINDOW_POPUP) || (m_windowType == WINDOW_SPLASH) || (m_windowType == WINDOW_NOTIFICATION)) {

			// The window is an overlapped window.
			m_windowStyleEx = WS_EX_WINDOWEDGE;

			// Do not show in Taskbar
			m_windowStyleEx |= WS_EX_TOOLWINDOW;

			m_windowStyle = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		} else {
			m_windowStyle = WS_OVERLAPPED |
			                WS_SYSMENU |		// Has a system menu.
			                WS_BORDER |			// Has thin line border.
			                WS_CAPTION |		// Has a title bar.
			                WS_MAXIMIZEBOX |	// Has a maximize button.
			                WS_MINIMIZEBOX |	// Has a minimize button.
			                WS_SIZEBOX;			// Has a resize box (bottom right corner).

			m_windowStyleEx = WS_EX_APPWINDOW;
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

		SetRect(&rc, 0, 0, getWidth(), getHeight());
		// Ok, we do that because the user wants a client area width the dimenstion(widht, height)
		// and not the whole windows include the decoration of a windows.
		AdjustWindowRectEx( &rc, m_windowStyle, false, m_windowStyleEx);

		m_wnd = CreateWindowEx( m_windowStyleEx,
		                        WndClass.lpszClassName,
		                        getTitle(),
		                        m_windowStyle,
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

		//
		// TODO This mechanism is bad and should be changed later. We use the getWindowID
		// to register the window in the event server. This doesn't look quite good and
		// it should be changed in later releases.
		//

		// We are going to store this window instance pointer in GWLP_USERDATA.
		SetWindowLongPtr(m_wnd, GWLP_USERDATA, (LONG_PTR)this);
		m_id = windowID++;

		windowEventServer->registerWindowForEvents(this);

		//
		// ------------------------------------------------------------------------
		//

		UpdateWindow(m_wnd);

		return ERR_OK;
	}

	void* XdevLWindowDeviceWin32::getInternal(const XdevLInternalName& id) {
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

	xdl_int XdevLWindowDeviceWin32::update() {
		return ERR_OK;
	}

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
		} else {
			::SetActiveWindow(m_wnd);
		}
	}

	void XdevLWindowDeviceWin32::grabPointer() {
		SetCapture(m_wnd);
	}

	void XdevLWindowDeviceWin32::ungrabPointer() {
		ReleaseCapture();
	}

	void XdevLWindowDeviceWin32::setInputFocus() {
		::SetFocus(m_wnd);
	}

	xdl_bool  XdevLWindowDeviceWin32::hasFocus() {
		return (::GetFocus() != nullptr) ? xdl_true : xdl_false;
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

	xdl_bool  XdevLWindowDeviceWin32::getHidePointer() {
		return XdevLWindowImpl::getHidePointer();
	}

	void XdevLWindowDeviceWin32::setFullscreen(xdl_bool state) {

	}

	void XdevLWindowDeviceWin32::clipPointerPosition(xdl_uint x, xdl_uint y, xdl_uint width, xdl_uint height) {

	}

	void XdevLWindowDeviceWin32::setType(XdevLWindowTypes type) {
		xdl_int32 value = 0;
		switch (type) {
			case WINDOW_NORMAL: {
				m_windowStyle = WS_SYSMENU |		// Has a system menu.
				                WS_BORDER |			// Has thin line border.
				                WS_CAPTION |		// Has a title bar.
				                WS_MAXIMIZEBOX |	// Has a maximize button.
				                WS_MINIMIZEBOX |	// Has a minimize button.
				                WS_SIZEBOX;			// Has a resize box (bottom right corner).

				m_windowStyleEx = WS_EX_APPWINDOW;
				SetWindowLong(m_wnd, GWL_STYLE, m_windowStyle);
				SetWindowLong(m_wnd, GWL_EXSTYLE, m_windowStyleEx);
			}
			break;
			case WINDOW_POPUP:
			case WINDOW_DROPDOWN_MENU:
			case WINDOW_SPLASH:
			case WINDOW_TOOLTIP: {
				m_windowStyle = WS_POPUP |
				                WS_CLIPCHILDREN |
				                WS_CLIPSIBLINGS;

				m_windowStyleEx =	WS_EX_WINDOWEDGE |
				                    WS_EX_TOOLWINDOW; // Do not show in taskbar.


				SetWindowLong(m_wnd, GWL_STYLE, m_windowStyle);
				SetWindowLong(m_wnd, GWL_EXSTYLE, m_windowStyleEx);

			}
			break;
			case WINDOW_UNKNOWN:
			default: {

			} break;
		}
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

	HWND XdevLWindowDeviceWin32::getNativeWindow() {
		return m_wnd;
	}

	void XdevLWindowDeviceWin32::setParent(XdevLWindow* window) {
		XdevLWindowImpl::setParent(window);
	}
//
// -------------------------------------------------------------------------
//

	XdevLWindowServerWindows::XdevLWindowServerWindows(XdevLModuleCreateParameter* parameter) :
		XdevLWindowServerImpl(parameter) {

	}

	XdevLWindowServerWindows::~XdevLWindowServerWindows() {

	}

	xdl_int XdevLWindowServerWindows::createWindow(XdevLWindow** window,
	        const XdevLWindowTitle& title,
	        const XdevLWindowPosition& position,
	        const XdevLWindowSize& size) {

		*window = new XdevLWindowDeviceWin32(nullptr);

		return ERR_OK;
	}



//
// -----------------------------------------------------------------------------
//

	XdevLWindowWindowsEventServer::XdevLWindowWindowsEventServer(XdevLModuleCreateParameter* parameter) :
		XdevLWindowEventServerImpl(parameter, windowEventServerModuleDesc),
		m_pointerIsInsideWindow(nullptr)
	{}


	xdl_int XdevLWindowWindowsEventServer::registerWindowForEvents(XdevLWindow* window) {
		return XdevLWindowEventServerImpl::registerWindowForEvents(window);
	}

	xdl_int XdevLWindowWindowsEventServer::unregisterWindowFromEvents(XdevLWindow* window) {
		return XdevLWindowEventServerImpl::unregisterWindowFromEvents(window);
	}

	xdl_int XdevLWindowWindowsEventServer::init() {
		XDEVL_MODULE_SUCCESS("Window created successfully" << std::endl);
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

//	XdevLWindow* window = windowEventServer->getWindow((xdl_uint64)GetWindowLong(hWnd, GWL_ID));
		XdevLWindow* window = (XdevLWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (windowEventServer->isWindowRegistered(window) == xdl_false) {
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}

		XdevLEvent ev;
		ev.common.timestamp = getMediator()->getTimer().getTime64();

		switch (uMsg) {
			case WM_CLOSE: {

				ev.type = XDEVL_WINDOW_EVENT;
				ev.window.event = XDEVL_WINDOW_CLOSE;
				ev.window.windowid = window->getWindowID();

				getMediator()->fireEvent(ev);

				// Make a core event.
				ev.type = XDEVL_CORE_EVENT;
				ev.core.event = XDEVL_CORE_SHUTDOWN;

				getMediator()->fireEvent(ev);

			}
			break;
			case WM_SHOWWINDOW: {
				switch (lParam) {
					case SW_PARENTCLOSING:
						XDEVL_MODULE_INFO("SW_PARENTCLOSING\n");
						break;
					case SW_PARENTOPENING:
						ev.type = XDEVL_WINDOW_EVENT;
						ev.type = XDEVL_WINDOW_EVENT;
						ev.window.event = XDEVL_WINDOW_SHOWN;
						ev.window.windowid = window->getWindowID();

						getMediator()->fireEvent(ev);
						XDEVL_MODULE_INFO("SW_PARENTOPENING\n");
						break;
					default:
						break;
				}
			}
			break;
			case WM_SYSKEYUP:
			case WM_KEYUP: {

				XdevLKeyMode keyMode = KEY_MOD_NONE;
				ev.type = ButtonReleased.getHashCode();
				ev.key.windowid = window->getWindowID();
				ev.key.repeat = (lParam & 0x40000000) != 0;
				//xdl_uint32 vk = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
				ev.key.keycode = WindowsVirtualKeyToXdevLKeyCode(wParam, lParam);
				ev.key.mod = keyMode;

				getMediator()->fireEvent(ev);

			}
			break;
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN: {

				XdevLKeyMode keyMode = KEY_MOD_NONE;
				ev.type = ButtonPressed.getHashCode();
				ev.key.windowid = window->getWindowID();
				ev.key.repeat = (lParam & 0x40000000) != 0;
				//xdl_uint32 vk = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
				ev.key.keycode = WindowsVirtualKeyToXdevLKeyCode(wParam, lParam);
				ev.key.mod = keyMode;

				getMediator()->fireEvent(ev);

			}
			break;
			case WM_CREATE:
				break;
			case WM_SETFOCUS: {

				if (wParam != 0) {
					HWND wnd = (HWND)wParam;
					XdevLWindow* window = windowEventServer->getWindow((xdl_uint64)GetWindowLong(wnd, GWL_ID));
					if (window != nullptr) {
						ev.type = XDEVL_WINDOW_EVENT;
						ev.window.event = XDEVL_WINDOW_INPUT_FOCUS_LOST;
						ev.window.windowid = window->getWindowID();

						focusGained(window);

						getMediator()->fireEvent(ev);
					} else {
						XDEVL_MODULE_ERROR("Odd error happend.\n");
					}
				}

				ev.type = XDEVL_WINDOW_EVENT;
				ev.window.event = XDEVL_WINDOW_INPUT_FOCUS_GAINED;
				ev.window.windowid = window->getWindowID();

				focusGained(window);

				getMediator()->fireEvent(ev);
			}
			break;
			case WM_ACTIVATE: {
				if (wParam == WA_CLICKACTIVE) {
					/*ev.type = XDEVL_WINDOW_EVENT;
					ev.window.event = XDEVL_WINDOW_INPUT_FOCUS_GAINED;
					ev.window.windowid = window->getWindowID();

					focusGained(window);

					getMediator()->fireEvent(ev);*/
				} else if (wParam == WA_INACTIVE) {
					/*	ev.type = XDEVL_WINDOW_EVENT;
						ev.window.event = XDEVL_WINDOW_INPUT_FOCUS_LOST;
						ev.window.windowid = window->getWindowID();

						focusGained(window);

						getMediator()->fireEvent(ev);*/
				}
			} break;
			case WM_SIZE: {
				//		window->setSize(XdevLWindowSize(LOWORD(lParam), HIWORD(lParam)));
			} break;
			case WM_MOVE: {
				//		window->setPosition(XdevLWindowPosition(LOWORD(lParam), HIWORD(lParam)));
			} break;
			case WM_ENTERSIZEMOVE: {

			}
			case WM_EXITSIZEMOVE: {
			}
			case WM_SIZING:
				break;
			case WM_MOUSEHOVER:
				if ( m_pointerIsInsideWindow != hWnd) {
					m_pointerIsInsideWindow = hWnd;

					ev.type = XDEVL_WINDOW_EVENT;
					ev.window.event = XDEVL_WINDOW_ENTER;
					ev.window.data1 = LOWORD(lParam);
					ev.window.data2 = HIWORD(lParam);
					ev.window.windowid = window->getWindowID();

					getMediator()->fireEvent(ev);
				}
				break;
			case WM_MOUSELEAVE: {
				m_pointerIsInsideWindow = nullptr;

				ev.type = XDEVL_WINDOW_EVENT;
				ev.window.event = XDEVL_WINDOW_LEAVE;
				ev.window.data1 = LOWORD(lParam);
				ev.window.data2 = HIWORD(lParam);
				ev.window.windowid = window->getWindowID();

				getMediator()->fireEvent(ev);
			}
			break;
			case WM_MOUSEMOVE: {
				ev.type = MouseMotion.getHashCode();

				ev.motion.windowid = window->getWindowID();
				ev.motion.x = LOWORD(lParam);
				ev.motion.y = HIWORD(lParam);
				ev.motion.xrel = 0;
				ev.motion.yrel = 0;

				getMediator()->fireEvent(ev);

				// TODO How about fullscreen window? Do we have to do this? But what if a client is withing the parent and
				// needs this events too.
				// TODO This causes to crash at shutdown and I don't know why.

				//TRACKMOUSEEVENT tme;
				//tme.cbSize = sizeof(TRACKMOUSEEVENT);
				//tme.dwFlags = TME_HOVER | TME_LEAVE; //Type of events to track & trigger.
				//tme.dwHoverTime = 1; //How long the mouse has to be in the window to trigger a hover event.
				//tme.hwndTrack = hWnd;
				//TrackMouseEvent(&tme);

			}
			break;
			case WM_LBUTTONDOWN: {
				ev.type = MouseButtonPressed.getHashCode();

				ev.button.windowid = window->getWindowID();
				ev.button.button = BUTTON_LEFT;
				ev.button.x = LOWORD(lParam);
				ev.button.y = HIWORD(lParam);

				getMediator()->fireEvent(ev);

			}
			break;
			case WM_LBUTTONUP: {
				ev.type = MouseButtonReleased.getHashCode();

				ev.button.windowid = window->getWindowID();
				ev.button.button = BUTTON_LEFT;
				ev.button.x = LOWORD(lParam);
				ev.button.y = HIWORD(lParam);

				getMediator()->fireEvent(ev);
			}
			break;

			case WM_RBUTTONDOWN: {
				ev.type = MouseButtonPressed.getHashCode();

				ev.button.windowid = window->getWindowID();
				ev.button.button = BUTTON_RIGHT;
				ev.button.x = LOWORD(lParam);
				ev.button.y = HIWORD(lParam);

				getMediator()->fireEvent(ev);
			}
			break;
			case WM_RBUTTONUP: {
				ev.type = MouseButtonReleased.getHashCode();

				ev.button.windowid = window->getWindowID();
				ev.button.button = BUTTON_RIGHT;
				ev.button.x = LOWORD(lParam);
				ev.button.y = HIWORD(lParam);

				getMediator()->fireEvent(ev);
			}
			break;
			case WM_MBUTTONDOWN: {
				ev.type = MouseButtonPressed.getHashCode();

				ev.button.windowid = window->getWindowID();
				ev.button.button = BUTTON_MIDDLE;
				ev.button.x = LOWORD(lParam);
				ev.button.y = HIWORD(lParam);

				getMediator()->fireEvent(ev);
			}
			break;
			case WM_MBUTTONUP: {
				ev.type = MouseButtonReleased.getHashCode();

				ev.button.windowid = window->getWindowID();
				ev.button.button = BUTTON_MIDDLE;
				ev.button.x = LOWORD(lParam);
				ev.button.y = HIWORD(lParam);

				getMediator()->fireEvent(ev);
			}
			break;
			default:
				break;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}

	void XdevLWindowWindowsEventServer::flush() {
//	pollEvents();
	}

//
// XdevLCursorWindow
//
	XdevLCursorWindows::XdevLCursorWindows(XdevLModuleCreateParameter* parameter) :
		XdevLModuleImpl<XdevLCursor>(parameter, windowCursorModuleDesc) {

	}

	XdevLCursorWindows::~XdevLCursorWindows() {
	}

	xdl_int XdevLCursorWindows::init() {
		return ERR_OK;
	}
	xdl_int XdevLCursorWindows::shutdown() {
		return ERR_OK;
	}
	void* XdevLCursorWindows::getInternal(const XdevLInternalName& id) {
		return nullptr;
	}

	xdl_int XdevLCursorWindows::attach(XdevLWindow* window) {
		return ERR_OK;
	}

	void XdevLCursorWindows::show() {
		while (::ShowCursor(true) < 0);
	}

	void XdevLCursorWindows::hide() {
		while (::ShowCursor(false) >= 0);
	}

	void XdevLCursorWindows::setPosition(xdl_uint x, xdl_uint y) {
		::SetCursorPos(x, y);
	}

	xdl_int XdevLCursorWindows::clip(xdl_uint x1, xdl_uint y1, xdl_uint x2, xdl_uint y2) {
		RECT rect;
		rect.left = x1;
		rect.top = y1;
		rect.right = x2;
		rect.bottom = y2;
		ClipCursor(&rect);
		return ERR_ERROR;
	}

	void XdevLCursorWindows::releaseClip() {
		ClipCursor(nullptr);
	}

	xdl_int XdevLCursorWindows::enableRelativeMotion() {
		return ERR_ERROR;
	}

	void XdevLCursorWindows::disableRelativeMotion() {

	}
}
