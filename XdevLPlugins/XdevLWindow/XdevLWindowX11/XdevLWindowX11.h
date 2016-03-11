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

#ifndef XDEVL_WINDOW_X11_H
#define XDEVL_WINDOW_X11_H

#include <tinyxml.h>
#include <XdevLPlatform.h>
#include <XdevLPlugin.h>
#include <XdevLWindow/XdevLWindowImpl.h>
#include <XdevLTypes.h>
#include <XdevLCoreMediator.h>
#include <XdevLThread.h>
#include <XdevLMutex.h>

#include "XdevLDisplayX11.h"
#include "XdevLCursorX11.h"
#include "XdevLWindowEventServerX11.h"

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>

namespace xdl {

	class XdevLWindowX11 :  public XdevLWindowImpl {
		public:

			XdevLWindowX11(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor);
			virtual ~XdevLWindowX11();

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
			virtual xdl_int create(const XdevLWindowAttribute& attribute) override;
			virtual const XdevLWindowPosition& getPosition() override;
			virtual const XdevLWindowSize& getSize() override;
			virtual XdevLWindowPosition::type getX() override;
			virtual XdevLWindowPosition::type getY() override;
			virtual XdevLWindowSize::type getWidth() override;
			virtual XdevLWindowSize::type getHeight() override;
			virtual const XdevLWindowTitle& getTitle() override;
			virtual xdl_bool getFullscreen() override;
			virtual xdl_bool getHidePointer() override;
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

			Display* getNativeDisplay();
			Window getNativeWindow();
			Window getNativeRootWindow();
			xdl_int getNativeScreenNumber();
			Colormap getNativeDefaultColorMap();

			xdl_int create(const XdevLWindowTitle& title, const XdevLWindowPosition& position, const XdevLWindowSize& size, const XdevLWindowTypes& type);

		protected:

			Display* m_display;

			// Holds the default root window. Mostly it is the desktop.
			Window m_rootWindow;

			// Holds our window id.
			Window m_window;

			// Holds the default screen number.
			xdl_int m_screenNumber;

			// Is the window in fullscreen mode or not?
			xdl_bool m_fullscreenModeActive;

			// Stores the default color map.
			Colormap m_defaultColorMap;

		protected:

			xdl_uint32 getNetWMState();

			xdl_int initializeEWMH();

		protected:

			// Holds screensaver info.
			xdl_int timeout_return;
			xdl_int interval_return;
			xdl_int prefer_blanking_return;
			xdl_int allow_exposures_return;
			xdl_long m_event_mask;

		protected:

			void enableFullscreenMode();
			void disableFullscreenMode();
			xdl_int disableDecoration();
			xdl_int enableDecoration();
			void setResizeable(xdl_bool state);

		protected:

			XConfigureEvent m_prevConfigureEvent;

			Atom _MOTIF_WM_HINTS;

			Atom _NET_WM_WINDOW_TYPE;
			Atom _NET_WM_WINDOW_TYPE_DESKTOP;
			Atom _NET_WM_WINDOW_TYPE_DOCK;
			Atom _NET_WM_WINDOW_TYPE_TOOLBAR;
			Atom _NET_WM_WINDOW_TYPE_MENU;
			Atom _NET_WM_WINDOW_TYPE_UTILITY;
			Atom _NET_WM_WINDOW_TYPE_SPLASH;
			Atom _NET_WM_WINDOW_TYPE_DIALOG;
			Atom _NET_WM_WINDOW_TYPE_DROPDOWN_MENU;
			Atom _NET_WM_WINDOW_TYPE_POPUP_MENU;
			Atom _NET_WM_WINDOW_TYPE_TOOLTIP;
			Atom _NET_WM_WINDOW_TYPE_NOTIFICATION;
			Atom _NET_WM_WINDOW_TYPE_COMBO;
			Atom _NET_WM_WINDOW_TYPE_DND;
			Atom _NET_WM_WINDOW_TYPE_NORMAL;
			Atom _NET_WM_BYPASS_COMPOSITOR;
			Atom _NET_WM_ACTION_RESIZE;
			Atom _NET_WM_WINDOW_OPACITY;
			Atom _NET_WM_STATE;
			Atom _NET_WM_STATE_MAXIMIZED_VERT;
			Atom _NET_WM_STATE_MAXIMIZED_HORZ;
			Atom _NET_WM_STATE_FULLSCREEN;
			Atom _NET_WM_STATE_HIDDEN;
			Atom _NET_WM_STATE_FOCUSED;
			Atom _NET_WM_NAME;
			Atom _NET_WM_ICON_NAME;
			Atom _NET_WM_ICON;
			Atom _NET_WM_PING;
			Atom WM_STATE;
			Atom WM_NAME;
			Atom WM_HINTS;
			Atom WM_NORMAL_HINTS;
			Atom WM_ICON_SIZE;
			Atom WM_DELETE_WINDOW;
			Atom WM_PROTOCOLS;
			Atom WM_CLASS;
			Atom WM_TRANSIENT_FOR;

			Atom XdndEnter;
			Atom XdndPosition;
			Atom XdndStatus;
			Atom XdndTypeList;
			Atom XdndActionCopy;
			Atom XdndDrop;
			Atom XdndFinished;
			Atom XdndSelection;

			Atom _KDE_NET_WM_WINDOW_TYPE_OVERRIDE;

			std::vector<Atom> m_atoms;
	};

	class XdevLWindowServerX11 : public XdevLWindowServerImpl {
		public:
			XdevLWindowServerX11(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor);
			virtual ~XdevLWindowServerX11();

			/// Creates a new window.
			virtual xdl_int createWindow(XdevLWindow** window,
			                             const XdevLWindowTitle& title,
			                             const XdevLWindowPosition& position,
			                             const XdevLWindowSize& size,
			                             const XdevLWindowTypes& type
			                            );
	};


	class XdevLX11Display {
		public:
			XdevLX11Display(XdevLCoreMediator* core);
			~XdevLX11Display();

			XdevLDisplayX11* getDisplay() {
				return display.get();
			}

			XdevLWindowEventServerX11* getWindowEventServer() {
				return windowEventServer.get();
			}

			XdevLCursorX11* getCursor() {
				return cursor.get();
			}

		public:
			XdevLCoreMediator* m_core;
			std::shared_ptr<XdevLDisplayX11> display;
			std::shared_ptr<XdevLWindowEventServerX11> windowEventServer;
			std::shared_ptr<XdevLCursorX11> cursor;
	};
}


#endif
