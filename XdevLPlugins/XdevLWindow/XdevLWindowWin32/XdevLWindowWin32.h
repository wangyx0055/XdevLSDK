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

#ifndef XDEVL_WINDOW_WINDOWS_H
#define XDEVL_WINDOW_WINDOWS_H

#include <XdevLPlatform.h>
#include <XdevLPlugin.h>
#include <XdevLWindow/XdevLWindowImpl.h>
#include <XdevLTypes.h>
#include <XdevLCoreMediator.h>
#include <tinyxml.h>

namespace xdl {

	// Holds the Major version number.
	const xdl_uint XdevLWindowMajorVersion = XDEVLWINDOWS_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLWindowMinorVersion = XDEVLWINDOWS_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLWindowPatchVersion = XDEVLWINDOWS_MODULE_PATCH_VERSION;


	// Holds the Major version number.
	const xdl_uint XdevLWindowEventServerMajorVersion = XDEVLWINDOWS_EVENT_SERVER_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLWindowEventServerMinorVersion = XDEVLWINDOWS_EVENT_SERVER_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLWindowEventServerPatchVersion = XDEVLWINDOWS_EVENT_SERVER_MODULE_PATCH_VERSION;


	static const XdevLString windowPluginName{
		"XdevLWindowWindow"
	};

	static const XdevLString windowDescription{
		"Support for creating a window using native OS methods."
	};

	class XdevLWindowDeviceWin32 : public XdevLWindowImpl, public thread::Thread {
public:
	XdevLWindowDeviceWin32(XdevLModuleCreateParameter* parameter);

	virtual ~XdevLWindowDeviceWin32() {
	}

	virtual xdl_int init();
	virtual xdl_int shutdown();
	virtual void* getInternal(const xdl_char* id);
	virtual xdl_int update();
	virtual xdl_int reset();

	const XdevLWindowPosition& getPosition();
	const XdevLWindowSize& getSize();

	virtual void setSize(const XdevLWindowSize& size);
	virtual void setPosition(const XdevLWindowPosition& position);
	virtual void setTitle(const XdevLWindowTitle& title);
	virtual void setFullscreen(xdl_bool state);
	virtual void showPointer();
	virtual void hidePointer();
	virtual void setPointerPosition(xdl_uint x, xdl_uint y);
	virtual void clipPointerPosition(xdl_uint x, xdl_uint y, xdl_uint width, xdl_uint height);
	virtual void setX(XdevLWindowPosition::type x);
	virtual void setY(XdevLWindowPosition::type y);
	virtual void setWidth(XdevLWindowSize::type width);
	virtual void setHeight(XdevLWindowSize::type height);
	virtual void setType(XdevLWindowTypes type);
	virtual const XdevLWindowTitle& getTitle();
	virtual xdl_bool getFullscreen();
	virtual xdl_int getColorDepth();
	virtual xdl_bool getHidePointer();
	virtual XdevLWindowSize::type getWidth();
	virtual XdevLWindowSize::type getHeight();
	virtual XdevLWindowPosition::type getX();
	virtual XdevLWindowPosition::type getY();

	virtual void setHidePointer(xdl_bool state);
	virtual void show();
	virtual void hide();
	virtual xdl_bool isHidden();
	virtual void raise();
	virtual void grabPointer();
	virtual void ungrabPointer();
	virtual void grabKeyboard();
	virtual void ungrabKeyboard();
	virtual void setInputFocus();
	virtual xdl_bool hasFocus();
	virtual xdl_int getInputFocus(XdevLWindow** window);

protected:
	std::string m_winClassId;
protected:
	/// win32 instance handle.
	HINSTANCE		m_instance;
	/// Holds the win32 window handle.
	HWND				m_wnd;
	/// Holds the win32 graphics context.
	HDC					m_DC;
	/// Holds the window style value.
	unsigned long	m_windowStyle;
	/// Holds the windows extented style value.
	unsigned long	m_windowStyleEx;
	DEVMODE m_oldDevMode;
	DEVMODE m_matchingVideoMode;
	std::vector<DISPLAY_DEVICE> displayInfoList;
	xdl_bool m_isHidden;
private:
	///creates a win32 window
	int create();

//	virtual xdl_int recvEvent(XdevLEvent* ev);
	
};


	class XdevLWindowServerWindows : public XdevLWindowServerImpl {
	public:
		XdevLWindowServerWindows(XdevLModuleCreateParameter* parameter);
		virtual ~XdevLWindowServerWindows();

		/// Creates a new window.
		virtual xdl_int createWindow(XdevLWindow** window,
			const XdevLWindowTitle& title,
			const XdevLWindowPosition& position,
			const XdevLWindowSize& size
			);
	};


	class XdevLWindowWindowsEventServer : public XdevLWindowEventServerImpl {
	public:
		XdevLWindowWindowsEventServer(XdevLModuleCreateParameter* parameter);
		virtual xdl_int init() override;
		virtual xdl_int shutdown() override;
		virtual void* getInternal(const XdevLInternalName& id) override;
		virtual xdl_int update() override;

		virtual xdl_int registerWindowForEvents(XdevLWindow* window) override;
		virtual xdl_int unregisterWindowFromEvents(XdevLWindow* window) override;
		void flush() override;

		/// windows message function callback
		static LRESULT CALLBACK callbackProxy(HWND hWnd, UINT	uMsg, WPARAM wParam, LPARAM lParam);

	private:
//		int pollEvents();
		/// win32 message handling
		LRESULT  callbackProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	};
}

extern "C" XDEVL_EXPORT int _createWindowDevice(xdl::XdevLModuleCreateParameter* parameter);
extern "C" XDEVL_EXPORT void _deleteWindowDevice(xdl::XdevLModule* obj);
extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getWindowPluginInfo();

#endif
