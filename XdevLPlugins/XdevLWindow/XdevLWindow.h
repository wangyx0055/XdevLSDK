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

#ifndef XDEVL_WINDOW_H
#define XDEVL_WINDOW_H

#include <XdevLModule.h>

namespace xdl {

	/**
		@enum XdevLWindowEvents
		@brief Events a Window device can fire.
	*/
	enum XdevLWindowEvents {
	  XDEVL_WINDOW_SHOWN				= (1 << 0),		/// Window was shown.
	  XDEVL_WINDOW_HIDDEN				= (1 << 1),		/// Window was hidden.
	  XDEVL_WINDOW_EXPOSED			= (1 << 2),		/// Window got exposed.
	  XDEVL_WINDOW_MOVED				= (1 << 3),		/// Window got moved.
	  XDEVL_WINDOW_RESIZED			= (1 << 4),		/// Window got resized.
	  XDEVL_WINDOW_MINIMIZED			= (1 << 5),		/// Window got minimized.
	  XDEVL_WINDOW_MAXIMIZED			= (1 << 6),		/// Window got maximized.
	  XDEVL_WINDOW_RESTORED			= (1 << 7),		/// Window got restored. (After icon state for example)
	  XDEVL_WINDOW_ENTER				= (1 << 8),		/// Mouse cursor entered window area.
	  XDEVL_WINDOW_LEAVE				= (1 << 9),		/// Mouse cursor left window area.
	  XDEVL_WINDOW_INPUT_FOCUS_GAINED	= (1 << 10),	/// Keyboard focus gained.
	  XDEVL_WINDOW_INPUT_FOCUS_LOST	= (1 << 11),	/// Keyboard focus lost.
	  XDEVL_WINDOW_CLOSE				= (1 << 12),	/// Window closed.
	  XDEVL_WINDOW_FULLSCREEN			= (1 << 13),	/// Window got into fullscreen.
	  XDEVL_WINDOW_CREATE				= (1 << 14), 	/// Window got created.
	  XDEVL_WINDOW_HITTEST			= (1 << 15),	/// Window hittest area got clicked.
	};


	enum XdevLWindowTypes {
	  XDEVL_WINDOW_TYPE_NORMAL,				// Normal window with a titlebar and borders.
	  XDEVL_WINDOW_TYPE_TOOLTIP,			// A window without any titlebar and borders.
	  XDEVL_WINDOW_TYPE_POPUP,				// A window without any titlebar and borders.
	  XDEVL_WINDOW_TYPE_DROPDOWN_MENU,// A window without any titlebar and borders.
	  XDEVL_WINDOW_TYPE_SPLASH,				// A window without titlebar and borders and will be placed in the center.
	  XDEVL_WINDOW_TYPE_NOTIFICATION,	// A window that will be displayed in the notification system of the OS.
	  XDEVL_WINDOW_TYPE_UNKNOWN
	};


	typedef XdevLPosition XdevLWindowPosition;
	typedef XdevLSize XdevLWindowSize;
	typedef XdevLString XdevLWindowTitle;

	typedef XdevLPosition XdevLDisplayPosition;
	typedef XdevLSize XdevLDisplaySize;
	typedef XdevLString XdevLDisplayTitle;

	class XdevLDisplayMode {
		public:

			XdevLDisplayMode() :
				rate(60) {

			}

			/// Return the size of the display.
			const XdevLDisplaySize& getSize() const {
				return size;
			}

			/// Return the refresh rate of the display.
			const xdl_int16& getRate() const {
				return rate;
			}

		public:

			XdevLDisplaySize size;
			xdl_int16 rate;
	};
	typedef std::vector<XdevLDisplayMode> XdevLDisplayModes;

	// Must be signed value because -1 is used for invalid id's.
	typedef xdl_int XdevLDisplayModeId;


	/**
		@class XdevLWindowAttribute
		@brief Structure that holds Window information.
		
		In this structure one can define the title, position and size of a window.
		Furthermore you can decide the type. The type decides how the window will
		look like and where it will be positioned.
		For example, a window with the type XDEVL_WINDOW_TYPE_SPLASH will have no
		borders and will be centered in the middle of the window.
	*/
	class XdevLWindowAttribute {
		public:
			XdevLWindowAttribute() :
				title(XdevLWindowTitle("XdevL (c) 2007 - 20016 Cengiz Terzibas")),
				position(0, 0),
				size(640, 480),
				type(XDEVL_WINDOW_TYPE_NORMAL) {}

			XdevLWindowTitle title;
			XdevLWindowPosition position;
			XdevLWindowSize size;
			XdevLWindowTypes type;
	};

	class XdevLDisplay : public XdevLModule {
		public:
			virtual ~XdevLDisplay() {}

			/// Returns a list of supported display modes.
			virtual XdevLDisplayModes getDisplayModes() = 0;

			/// Returns the id of the display mode.
			virtual XdevLDisplayModeId getDisplayModeId(const XdevLDisplayMode& mode) = 0;

			/// Returns the id of the the closest display mode of the specified mode.
			virtual XdevLDisplayModeId getClosestDisplayModeId(const XdevLDisplayMode& mode) = 0;

			/// Set display resolution.
			virtual xdl_int setDisplayMode(const XdevLDisplayModeId& mode) = 0;

			/// Restore the previous size of the display.
			virtual xdl_int restore() = 0;
	};

	/**
		@class XdevLWindow
		@brief Interface class for a window.
		@author Cengiz Terzibas

		This interface supports a OS window. The behavior is defined in the
		@xdevlWindowImplementors section

		@section xdevlWindowImplementors Information for implementors

		- Creating a window must result in a hidden window.

	*/
	class XdevLWindow : public XdevLModule {
		public:

			virtual ~XdevLWindow() {}

			/// Creates window using default values.
			virtual xdl_int create() = 0;

			/// Create a window using the attribute structure.
			virtual xdl_int create(const XdevLWindowAttribute& attribute) = 0;

			/// Returns the ID of the windows in the window server.
			virtual xdl_uint64 getWindowID() = 0;

			/// Returns the parent window of this window.
			virtual XdevLWindow* getParent() = 0;

			/// Returns the title of the window
			virtual const XdevLWindowTitle& getTitle() = 0;

			/// Returns the x position of the window on the desktop
			virtual const XdevLWindowPosition& getPosition() = 0;

			/// Returns the size of the window.
			virtual const XdevLWindowSize& getSize() = 0;

			/// Returns the window type.
			virtual XdevLWindowTypes getType() = 0;

			/// Returns the x position of the window.
			virtual XdevLWindowPosition::type getX() = 0;

			/// Returns the y position of the window.
			virtual XdevLWindowPosition::type getY() = 0;

			/// Returns the width of the window.
			virtual XdevLWindowSize::type getWidth() = 0;

			/// Returns the height of the window.
			virtual XdevLWindowSize::type getHeight() = 0;

			/// Sets the parent window.
			virtual void setParent(XdevLWindow* window) = 0;

			/// Sets the title of the window.
			virtual void setTitle(const XdevLWindowTitle& title) = 0;

			/// Sets the position of the window.
			virtual void setPosition(const XdevLWindowPosition& position) = 0;

			/// Sets the size of the window.
			virtual void setSize(const XdevLWindowSize& size) = 0;

			/// Sets the window type.
			virtual void setType(XdevLWindowTypes type) = 0;

			/// Sets the x position of the window.
			virtual void setX(XdevLWindowPosition::type x) = 0;

			/// Sets the y position of the window.
			virtual void setY(XdevLWindowPosition::type y) = 0;

			/// Sets the width of the window.
			virtual void setWidth(XdevLWindowSize::type width) = 0;

			/// Sets the height of the window.
			virtual void setHeight(XdevLWindowSize::type height) = 0;

			/// Sets the fullscreen mode.
			virtual void setFullscreen(xdl_bool state) = 0;

			/// Sets the keyboard input focus.
			virtual void setInputFocus() = 0;

			/// Show the window.
			virtual void show() = 0;

			/// Hide the window.
			virtual void hide() = 0;

			/// Raise window above others.
			/**
				The window will be restacked and will become the top most ( in Z order)
			*/
			virtual void raise() = 0;

			/// Checks if the window has focus.
			virtual xdl_bool hasFocus() = 0;

			/// Checks if the cursor is inside the window.
			virtual xdl_bool isPointerInside() = 0;

			/// Returns if the window is hidden or not.
			virtual xdl_bool isHidden() = 0;

			/// Returns if the window is in fullscreen mode.
			virtual xdl_bool isFullscreen() = 0;

			/// Returns if the mouse pointer is hidden or not.
			virtual xdl_bool isPointerHidden() = 0;

	};

	/**
		@class XdevLWindowServer
		@brief Manages windows

		If you need faster creation and deletion of a OS window use this server
		to do so. You can use XdevLWindowServer for you UI toolkits to create and
		delete OS windows faster.
		The create and delete method will skip the process of the XdevLCore module
		methods which can take time.
	*/
	class XdevLWindowServer : public XdevLModule {
		public:
			virtual ~XdevLWindowServer() {}

			/// Creates a new window.
			virtual xdl_int create(XdevLWindow** window, const XdevLWindowAttribute& attribute) = 0;

			/// Destroys the specified window.
			virtual xdl_int destroy(XdevLWindow* window) = 0;
	};

	class XdevLWindowEventServer : public XdevLModule {
		public:
			virtual ~XdevLWindowEventServer() {}

			virtual xdl_bool isWindowRegistered(XdevLWindow* window) = 0;
			virtual xdl_int registerWindowForEvents(XdevLWindow* window) = 0;
			virtual xdl_int unregisterWindowFromEvents(XdevLWindow* window) = 0;
			virtual XdevLWindow* getWindow(xdl_uint64 id) = 0;
			virtual XdevLWindow* getFocus() const = 0;
			virtual void flush() = 0;
	};

	/**
		@class XdevLCursor
		@brief Manages cursors

		If you want to change the look and behaviour of the cursor use this class.
	*/
	class XdevLCursor : public XdevLModule {
		public:
			virtual ~XdevLCursor() {}

			/// Attach the cursor to a window.
			virtual xdl_int attach(XdevLWindow* window) = 0;

			/// Show the pointer.
			virtual void show() = 0;

			/// Hide the pointer.
			virtual void hide() = 0;

			/// Sets the pointer position.
			virtual void setPosition(xdl_uint x, xdl_uint y) = 0;

			/// Clips the position in screen coordinates.
			virtual xdl_int clip(xdl_uint x, xdl_uint y, xdl_uint width, xdl_uint height) = 0;

			/// Release clipping position.
			virtual void releaseClip() = 0;

			/// Enable relative motion mode.
			virtual xdl_int enableRelativeMotion() = 0;

			/// Disable relative motion mode.
			virtual void disableRelativeMotion() = 0;

			/// Checks if the relative motion is activated.
			virtual xdl_bool isRelativeMotionEnabled() = 0;
	};

	typedef XdevLDisplay*		IPXdevLDisplay;
	typedef XdevLDisplay		IXdevLDisplay;

	typedef XdevLWindow*		IPXdevLWindow;
	typedef XdevLWindow			IXdevLWindow;

	typedef XdevLWindowServer*	IPXdevLWindowServer;
	typedef XdevLWindowServer	IXdevLWindowServer;

	typedef XdevLWindowEventServer* IPXdevLWindowEventServer;
	typedef XdevLWindowEventServer 	IXdevLWindowEventServer;

	typedef XdevLCursor*		IPXdevLCursor;
	typedef XdevLCursor			IXdevLCursor;

	XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DECLARATION(XdevLDisplay)
	XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DECLARATION(XdevLWindow)
	XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DECLARATION(XdevLWindowEventServer)
	XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DECLARATION(XdevLCursor)

	XDEVL_EXPORT_PLUGIN_INIT_FUNCTION_DECLARATION(XdevLWindow)
	XDEVL_EXPORT_PLUGIN_SHUTDOWN_FUNCTION_DECLARATION(XdevLWindow)

}

#endif
