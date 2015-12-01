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

#ifndef XDEVL_WINDOW_X11_H
#define XDEVL_WINDOW_X11_H

#include <tinyxml.h>
#include <XdevLPlatform.h>
#include <XdevLPlugin.h>
#include <XdevLWindow/XdevLWindowImpl.h>
#include <XdevLWindowX11Keyboard.h>
#include <XdevLTypes.h>
#include <XdevLCoreMediator.h>
#include <XdevLThread.h>
#include <XdevLMutex.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/Xrandr.h>
#include <X11/XKBlib.h>


namespace xdl {

	// Holds the plugin major version.
	const xdl_uint XdevLWindowPluginMajorVersion = XDEVLX11_MAJOR_VERSION;

	// Holds the plugin minor version.
	const xdl_uint XdevLWindowPluginMinorVersion = XDEVLX11_MINOR_VERSION;

	// Holds the plugin patch version.
	const xdl_uint XdevLWindowPluginPatchVersion = XDEVLX11_PATCH_VERSION;


	// Holds the Major version number.
	const xdl_uint XdevLWindowMajorVersion = XDEVLX11_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLWindowMinorVersion = XDEVLX11_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLWindowPatchVersion = XDEVLX11_MODULE_PATCH_VERSION;

	static const XdevLString window_x11_pluginName {
		"XdevLWindowX11"
	};
	static const XdevLString window_x11_description {
		"Support for X11 window creation."
	};

	class XdevLWindowLinux: public XdevLWindowImpl, public thread::Thread {
		public:

			XdevLWindowLinux(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLWindowLinux();

			static XdevLModuleDescriptor m_windowX11ModuleDesc;

			virtual xdl_int init();
			virtual xdl_int shutdown();
			virtual void* getInternal(const char* id);
			virtual xdl_int update();


			//
			// XdevLWindow related methods.
			//

			/// Returns the x position of the window on the desktop
			virtual const XdevLWindowPosition& getPosition();

			/// Returns the size of the window.
			virtual const XdevLWindowSize& getSize();

			/// Returns the x position of the window.
			virtual XdevLWindowPosition::type getX();

			/// Returns the y position of the window.
			virtual XdevLWindowPosition::type getY();

			/// Returns the width of the window.
			virtual XdevLWindowSize::type getWidth();

			/// Returns the height of the window.
			virtual XdevLWindowSize::type getHeight();

			/// Returns the title of the window
			virtual const XdevLWindowTitle& getTitle();

			/// Returns the screen mode state.
			virtual xdl_bool getFullscreen();

			/// Return if the mouse pointer is hidden.
			virtual xdl_bool getHidePointer();

			/// Returns the color buffer depth.
			virtual xdl_int getColorDepth();

			/// Sets the position of the window.
			virtual void setPosition(const XdevLWindowPosition& position);

			/// Sets the size of the window.
			virtual void setSize(const XdevLWindowSize& size);

			/// Sets the x position of the window.
			virtual void setX(XdevLWindowPosition::type x);

			/// Sets the y position of the window.
			virtual void setY(XdevLWindowPosition::type y);

			/// Sets the width of the window.
			virtual void setWidth(XdevLWindowSize::type width);

			/// Sets the height of the window.
			virtual void setHeight(XdevLWindowSize::type height);

			/// Sets the title of the window.
			virtual void setTitle(const XdevLWindowTitle& title);

			/// Sets the fullscreen mode.
			virtual void setFullscreen(xdl_bool state);

			/// Show the pointer.
			virtual void showPointer();

			/// Hide the pointer.
			virtual void hidePointer();

			/// Sets the pointer position.
			virtual void setPointerPosition(xdl_uint x, xdl_uint y);
			virtual void clipPointerPosition(xdl_uint x, xdl_uint y, xdl_uint width, xdl_uint height) ;

			/// Show the window.
			virtual void show();

			/// Hide the window.
			virtual void hide();
			virtual xdl_bool isHidden();

			/// Raise window above other and set input focus.
			virtual void raise();

			/// Grabs the pointer.
			virtual void grabPointer();

			/// Ungrabs the pointer.
			virtual void ungrabPointer();

			virtual void grabKeyboard();
			virtual void ungrabKeyboard();

			/// Sets the input focus this this window.
			virtual void setInputFocus();
			virtual xdl_bool hasFocus();

			/// Returns the input focus window.
			virtual xdl_int getInputFocus(XdevLWindow** window);

			virtual void setParent(XdevLWindow* window);

			virtual void SetType(XdevLWindowTypes type);
		protected:

			xdl_int RunThread(thread::ThreadArgument* argument);

		protected:
			/// Holds the default root window. Mostly it is the desktop.
			Window m_rootWindow;

			/// Holds our window id.
			Window m_window;

			/// Holds the default screen number.
			xdl_int m_screenNumber;

			/// Holds the width of the default screen.
			xdl_int m_screenWidth;

			/// Holds the height of the default screen.
			xdl_int m_screenHeight;

			/// Holds the major version number of the xrandr extension.
			xdl_int m_randrMajor;

			/// Holds the minor version number of the xrandr extension.
			xdl_int m_randrMinor;


			xdl_int m_event_basep;
			xdl_int m_error_basep;
			xdl_int m_best_fit_width;
			xdl_int m_best_fit_height;
			xdl_int m_best_fit_rate;

			// Stores the default color map.
			Colormap 								m_defaultColorMap;

			// Original id.
			SizeID 									m_originalSizeId;

			// Original screen resolution.
			XRRScreenConfiguration*	m_originalScreenConfig;

			/// Holds the screen rate.
			short int								m_originalScreenRate;

			// Original rotation.
			Rotation								m_originalRotation;



		protected:

			xdl_uint32 getNetWMState();
			void setWindowBordered();
			xdl_int setWMProtocols();
		protected:
			bool m_modeChanged;
			xdl_int m_old_screen_width;
			xdl_int m_old_screen_height;
			Rotation m_oldRotation;
			SizeID m_sizeId;

			Cursor m_hideCursor;
			xdl_int timeout_return;
			xdl_int interval_return;
			xdl_int prefer_blanking_return;
			xdl_int allow_exposures_return;
			xdl_long m_event_mask;
		protected:
			xdl_int create(const XdevLWindowTitle& title, const XdevLWindowPosition& position, const XdevLWindowSize& size);
			xdl_int create();
			xdl_int getGetClosestVideoMode();
			void setFullscreenVideoMode();
			void restoreFullscreenVideoMode();
			xdl_int disableDecoration();
			xdl_int enableDecoration();
			void showMousePointer(xdl_bool state);
		protected:
			XdevLWindowX11Keyboard* m_keyboard;
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
			Atom _NET_WM_STATE_ADD;
			Atom _NET_WM_STATE_REMOVE;
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

	};

	class XdevLWindowServerX11 : public XdevLWindowServerImpl {
		public:
			XdevLWindowServerX11(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLWindowServerX11();

			/// Creates a new window.
			virtual xdl_int createWindow(XdevLWindow** window,
			                             const XdevLWindowTitle& title,
			                             const XdevLWindowPosition& position,
			                             const XdevLWindowSize& size
			                            );



	};

	typedef XdevLWindowLinux* IPXdevLWindowLinux;
}


#endif
