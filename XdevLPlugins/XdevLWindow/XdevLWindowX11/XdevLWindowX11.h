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
#include <XdevLWindowX11Keyboard.h>
#include <XdevLTypes.h>
#include <XdevLCoreMediator.h>
#include <XdevLThread.h>
#include <XdevLMutex.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/XInput2.h>
#include <X11/extensions/Xfixes.h>
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



	// Holds the Major version number.
	const xdl_uint XdevLWindowEventServerMajorVersion = XDEVLX11_EVENT_SERVER_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLWindowEventServerMinorVersion = XDEVLX11_EVENT_SERVER_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLWindowEventServerPatchVersion = XDEVLX11_EVENT_SERVER_MODULE_PATCH_VERSION;


	// Holds the Major version number.
	const xdl_uint XdevLCursorMajorVersion = XDEVLX11CURSOR_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLCursorMinorVersion = XDEVLX11CURSOR_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLCursorPatchVersion = XDEVLX11CURSOR_MODULE_PATCH_VERSION;


	static const XdevLString windowX11PluginName {
		"XdevLWindowX11"
	};
	static const XdevLString window_x11_description {
		"Support for X11 window creation."
	};

	class XdevLWindowX11: public XdevLWindowImpl {
		public:

			XdevLWindowX11(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor);
			virtual ~XdevLWindowX11();

			static xdl_int initX11(XdevLPluginCreateParameter* parameter);
			static xdl_int shutdownX11();

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
			virtual void setPointerPosition(xdl_uint x, xdl_uint y) override;
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
			xdl_int create(const XdevLWindowTitle& title, const XdevLWindowPosition& position, const XdevLWindowSize& size, const XdevLWindowTypes& type);

		protected:

			Display* m_display;

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
			xdl_bool m_fullscreenModeActive;

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
			void setWindowBordered(xdl_bool state);
			xdl_int initializeEWMH();
		protected:
			bool m_modeChanged;
			xdl_int m_old_screen_width;
			xdl_int m_old_screen_height;
			Rotation m_oldRotation;
			SizeID m_bestSizeId;

			Cursor m_hideCursor;
			xdl_int timeout_return;
			xdl_int interval_return;
			xdl_int prefer_blanking_return;
			xdl_int allow_exposures_return;
			xdl_long m_event_mask;
	protected:
			void displayFromNormalToFullscreen();
			void displayFromFullscreenToNormal();
			xdl_int getGetClosestVideoMode();
			void setFullscreenVideoMode();
			void restoreFullscreenVideoMode();
			xdl_int disableDecoration();
			xdl_int enableDecoration();
			void setResizeable(xdl_bool state);
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


	class XdevLWindowX11EventServer : public XdevLWindowEventServerImpl {
		public:
			XdevLWindowX11EventServer(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor);
			virtual ~XdevLWindowX11EventServer();
			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;
			virtual void* getInternal(const XdevLInternalName& id) override;
			virtual xdl_int update() override;

			virtual xdl_int registerWindowForEvents(XdevLWindow* window) override;
			virtual xdl_int unregisterWindowFromEvents(XdevLWindow* window) override;
			void flush() override;

		private:
			int pollEvents();
			XdevLWindow* m_focusWindow;
			XdevLWindowX11Keyboard* m_keyboard;
			xdl_int m_event_basep;
			xdl_int m_error_basep;

			Atom WM_PROTOCOLS;
			Atom WM_DELETE_WINDOW;
			Atom _NET_WM_PING;

			XConfigureEvent m_prevConfigureEvent;
	};


	class XdevLCursorX11 : public XdevLModuleImpl<XdevLCursor>  {
		public:
			virtual ~XdevLCursorX11() {}

			XdevLCursorX11(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor);

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
			void onHandleXinputEvent(XGenericEventCookie* cookie, XdevLWindow* window) ;
		private:
			void parseValuators(const double *input_values,unsigned char *mask,int mask_len, double *output_values,int output_values_len);
		private:
			XdevLWindowX11* m_window;
			Cursor		m_invisibleCursor;
			Pixmap		m_invisibleCursorPixmap;
			XColor		m_black;
			XColor		m_dummy;
			Colormap	m_defaultColorMap;
			xdl_int		m_screenNumber;

			//
			// Pointer barrier stuff.
			//
			xdl_bool	m_barriersSupported;
			xdl_int		m_fixes_opcode;
			xdl_int		m_fixes_event;
			xdl_int		m_fixes_error;
			PointerBarrier m_barriers[4];

			xdl_bool	m_xinput2Supported;
			xdl_int		m_xinput2_opcode;
			xdl_int		m_xinput2_event;
			xdl_int		m_xinput2_error;

			xdl_int m_screenWidth;
			xdl_int m_screenHeight;

			xdl_bool m_reltaiveModeEnabled;
	};

}


#endif
