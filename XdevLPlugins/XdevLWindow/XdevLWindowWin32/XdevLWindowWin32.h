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

#ifndef XDEVL_DEVICE_WINDOW_WIN32_H
#define XDEVL_DEVICE_WINDOW_WIN32_H

#include <XdevLPlatform.h>
#include <XdevLPlugin.h>
#include "XdevLWindowDeviceImpl.h"
#include <XdevLTypes.h>
#include <XdevLCoreMediator.h>
#include <tinyxml.h>
#include <OIS.h>

namespace xdl {


class XdevLWindowDeviceWin32 : public XdevLWindowDeviceImpl {
public:
	XdevLWindowDeviceWin32(XdevLModuleCreateParameter* parameter) :
		XdevLWindowDeviceImpl(parameter),
			m_inputManager(NULL),
			m_instance(NULL),
			m_DC(NULL),
			m_windowStyle(0),
			m_windowStyleEx(0){
			this->getMediator()->getMessageQuark("XDEVL_MESSAGE_RESTART", &m_XDEVL_MESSAGE_RESTART);
			XdevLWindowDeviceHide = new XdevLQuark("XdevLWindowDeviceHide");
			XdevLWindowDeviceShow = new XdevLQuark("XdevLWindowDeviceShow");
	}
	~XdevLWindowDeviceWin32() {
	}

	virtual xdl_int init();
	virtual xdl_int shutdown();
	virtual void* getInternal(const xdl_char* id);
	virtual xdl_int update();
	virtual xdl_int reset();
	virtual void setX(xdl_uint x);
	virtual void setY(xdl_uint y);
	virtual void setWidth(xdl_uint width);
	virtual void setHeight(xdl_uint height);
	virtual void setTitle(const xdl_char* title);
	virtual void setFullscreen(xdl_bool state);
	virtual void setHidePointer(xdl_bool state);
	virtual xdl_int show(xdl_bool state);
protected:
	OIS::InputManager* m_inputManager;
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
private:
	///creates a win32 window
	int create();
	/// win32 message handling
	LRESULT  callbackProc(HWND hWnd, UINT	uMsg, WPARAM wParam, LPARAM lParam);
	/// windows message function callback
	static LRESULT CALLBACK callbackProxy( HWND hWnd, UINT	uMsg, WPARAM wParam, LPARAM lParam);
	
	
	virtual xdl_int recvEvent(XdevLEvent* ev);
	XdevLQuark* m_XDEVL_MESSAGE_RESTART;
	XdevLQuark* XdevLWindowDeviceHide;
	XdevLQuark* XdevLWindowDeviceShow;
	
};


}

extern "C" XDEVL_EXPORT int _createWindowDevice(xdl::XdevLModuleCreateParameter* parameter);
extern "C" XDEVL_EXPORT void _deleteWindowDevice(xdl::XdevLModule* obj);
extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getWindowPluginInfo();

#endif
