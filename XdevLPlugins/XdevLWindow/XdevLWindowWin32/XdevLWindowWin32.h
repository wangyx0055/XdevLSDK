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

	static const XdevLString windowPluginName { "XdevLWindowWindow"
	};

	static const XdevLString windowDescription { "Support for creating a window using native OS methods."
	};

	class XdevLWindowDeviceWin32 : public XdevLWindowImpl, public thread::Thread {
		public:
			XdevLWindowDeviceWin32(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor);

			virtual ~XdevLWindowDeviceWin32();

			//
			// XdevLModule related methods.
			//

			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;
			virtual void* getInternal(const XdevLInternalName& id) override;
			virtual xdl_int update() override;

			//
			// XdevLWindow related methods.
			//

			virtual xdl_int create() override;
			virtual const XdevLWindowPosition& getPosition() override;
			virtual const XdevLWindowSize& getSize() override;
			virtual XdevLWindowPosition::type getX() override;
			virtual XdevLWindowPosition::type getY() override;
			virtual XdevLWindowSize::type getWidth() override;
			virtual XdevLWindowSize::type getHeight() override;
			virtual const XdevLWindowTitle& getTitle() override;
			virtual xdl_bool isFullscreen() override;
			virtual xdl_bool isPointerHidden() override;
			virtual void setPosition(const XdevLWindowPosition& position) override;
			virtual void setSize(const XdevLWindowSize& size) override;
			virtual void setX(XdevLWindowPosition::type x) override;
			virtual void setY(XdevLWindowPosition::type y) override;
			virtual void setWidth(XdevLWindowSize::type width) override;
			virtual void setHeight(XdevLWindowSize::type height) override;
			virtual void setTitle(const XdevLWindowTitle& title) override;
			virtual void setFullscreen(xdl_bool state) override;
			virtual void show() override;
			virtual void hide() override;
			virtual xdl_bool isHidden() override;
			virtual void raise() override;
			virtual void setInputFocus() override;
			virtual xdl_bool hasFocus() override;
			virtual void setParent(XdevLWindow* window) override;
			virtual void setType(XdevLWindowTypes type) override;

			//
			// Internal used methods
			//

			HWND getNativeWindow();

		protected:
			std::string m_winClassId;

		protected:
			/// win32 instance handle.
			HINSTANCE m_instance;
			/// Holds the win32 window handle.
			HWND m_wnd;
			/// Holds the win32 graphics context.
			HDC m_DC;
			/// Holds the window style value.
			unsigned long m_windowStyle;
			/// Holds the windows extented style value.
			unsigned long m_windowStyleEx;
			DEVMODE m_oldDevMode;
			DEVMODE m_matchingVideoMode;
			std::vector<DISPLAY_DEVICE> displayInfoList;
			xdl_bool m_isHidden;
	};

	class XdevLWindowServerWindows : public XdevLWindowServerImpl {
		public:
			XdevLWindowServerWindows(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor);
			virtual ~XdevLWindowServerWindows();

			/// Creates a new window.
			xdl_int create(XdevLWindow** window, const XdevLWindowAttribute& attribute) override;
	};

	class XdevLWindowWindowsEventServer : public XdevLWindowEventServerImpl {
		public:
			XdevLWindowWindowsEventServer(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor);
			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;
			virtual void* getInternal(const XdevLInternalName& id) override;
			virtual xdl_int update() override;

			virtual xdl_int registerWindowForEvents(XdevLWindow* window) override;
			virtual xdl_int unregisterWindowFromEvents(XdevLWindow* window) override;
			void flush() override;

			/// windows message function callback
			static LRESULT CALLBACK callbackProxy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		private:
			//		int pollEvents();
			/// win32 message handling
			LRESULT callbackProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			HWND m_pointerIsInsideWindow;
	};

	class XdevLCursorWindows : public XdevLModuleImpl<XdevLCursor> {
		public:
			XdevLCursorWindows(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor);

			virtual ~XdevLCursorWindows();

			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;
			virtual void* getInternal(const XdevLInternalName& id) override;

			virtual xdl_int attach(XdevLWindow* window) override;
			virtual void show() override;
			virtual void hide() override;
			virtual void setPosition(xdl_uint x, xdl_uint y) override;
			virtual xdl_int clip(xdl_uint x1, xdl_uint y1, xdl_uint x2, xdl_uint y2) override;
			virtual void releaseClip() override;
			virtual xdl_int enableRelativeMotion() override;
			virtual void disableRelativeMotion() override;
			virtual xdl_bool isRelativeMotionEnabled() override;
		private:
			xdl_bool m_reltaiveModeEnabled;
	};


	class XdevLWindowWindowsInit {
		public:
			XdevLWindowWindowsInit(XdevLCoreMediator* core);
			virtual ~XdevLWindowWindowsInit();
private:
			XdevLCoreMediator* m_core;
			std::shared_ptr<XdevLWindowWindowsEventServer> windowEventServer;
			std::shared_ptr<XdevLCursorWindows> cursor;
	};
}

extern "C" XDEVL_EXPORT int _createWindowDevice(xdl::XdevLModuleCreateParameter* parameter);
extern "C" XDEVL_EXPORT void _deleteWindowDevice(xdl::XdevLModule* obj);
extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getWindowPluginInfo();

#endif
