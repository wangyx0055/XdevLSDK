/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2014 Cengiz Terzibas

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
	    WINDOW_NORMAL,
	    WINDOW_TOOLTIP,
	    WINDOW_POPUP,
	    WINDOW_DROPDOWN_MENU,
	    WINDOW_SPLASH,
	    WINDOW_NOTIFICATION,
	    WINDOW_UNKNOWN
	};


	typedef XdevLPosition XdevLWindowPosition;

	typedef XdevLSize XdevLWindowSize;

	typedef XdevLString XdevLWindowTitle;

	/**
		@class XdevLWindowAttribute
		@brief Structure that holds Window information.
	*/
	class XdevLWindowAttribute {
		public:
			XdevLWindowAttribute() :
				title(XdevLWindowTitle("XdevL (c) 2007 - 2009 Cengiz Terzibas")),
				position(0, 0),
				size(640, 480),
				type(WINDOW_NORMAL) {}

			XdevLWindowTitle title;
			XdevLWindowPosition position;
			XdevLWindowSize size;
			XdevLWindowTypes type;
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

			/// Creates the window in manual mode.
			/**
				This method is used in combination with the XdevLWindowServer. Do not
				use this if you ceate a window using XdevLCore system.
				The procedure would be that you create a window object using
				the XdevLWindowServer method, set all window parameters
				and call the create method. After calling this method the OS window
				will be created using the parameters specified.
			*/
			virtual xdl_int create() {
				return ERR_ERROR;
			}

			/// Create a window using the attribute structure.
			virtual xdl_int create(const XdevLWindowAttribute& attribute) = 0;

			/// Returns the ID of the windows in the window server.
			virtual xdl_uint64 getWindowID() = 0;

			/// Returns the x position of the window on the desktop
			virtual const XdevLWindowPosition& getPosition() = 0;

			/// Returns the size of the window.
			virtual const XdevLWindowSize& getSize() = 0;

			/// Returns the x position of the window.
			virtual XdevLWindowPosition::type getX() = 0;

			/// Returns the y position of the window.
			virtual XdevLWindowPosition::type getY() = 0;

			/// Returns the width of the window.
			virtual XdevLWindowSize::type getWidth() = 0;

			/// Returns the height of the window.
			virtual XdevLWindowSize::type getHeight() = 0;

			/// Returns the title of the window
			virtual const XdevLWindowTitle& getTitle() = 0;

			/// Returns the screen mode state.
			virtual xdl_bool getFullscreen() = 0;

			/// Return if the mouse pointer is hidden.
			virtual xdl_bool getHidePointer() = 0;

			/// Returns the color buffer depth.
			virtual xdl_int getColorDepth() = 0;

			/// Sets the position of the window.
			virtual void setPosition(const XdevLWindowPosition& position) = 0;

			/// Sets the size of the window.
			virtual void setSize(const XdevLWindowSize& size) = 0;

			/// Sets the x position of the window.
			virtual void setX(XdevLWindowPosition::type x) = 0;

			/// Sets the y position of the window.
			virtual void setY(XdevLWindowPosition::type y) = 0;

			/// Sets the width of the window.
			virtual void setWidth(XdevLWindowSize::type width) = 0;

			/// Sets the height of the window.
			virtual void setHeight(XdevLWindowSize::type height) = 0;

			/// Sets the title of the window.
			virtual void setTitle(const XdevLWindowTitle& title) = 0;

			/// Sets the fullscreen mode.
			virtual void setFullscreen(xdl_bool state) = 0;

			/// Sets the window type.
			virtual void setType(XdevLWindowTypes type) = 0;

			/// Show the window.
			/**
				Shows the window without setting the input focus.
			*/
			virtual void show() = 0;

			/// Hide the window.
			virtual void hide() = 0;

			/// Returns if the window is hidden.
			virtual xdl_bool isHidden() = 0;

			/// Raise window above others.
			/**
				The window will be restacked and will become the top most ( in Z order)
			*/
			virtual void raise() = 0;

			/// Show pointer.
			virtual void showPointer() = 0;

			/// Hide pointer.
			virtual void hidePointer() = 0;

			/// Set pointer position on the screen.
			virtual void setPointerPosition(xdl_uint x, xdl_uint y) = 0;

			/// Grabs the pointer.
			virtual void grabPointer() = 0;

			/// Ungrabs the pointer.
			virtual void ungrabPointer() = 0;

			/// Sets the input focus.
			virtual void setInputFocus() = 0;

			/// Checks if the window has focus.
			virtual xdl_bool hasFocus() = 0;

			/// Sets the parent window.
			virtual void setParent(XdevLWindow* window) = 0;

			/// Sets the window decoration
			virtual void setWindowDecoration(xdl_bool enable) = 0;

			virtual xdl_bool isPointerInside() = 0;

			/// Returns the window type.
			virtual XdevLWindowTypes getType() = 0;

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
			virtual xdl_int createWindow(XdevLWindow** window,
			                             const XdevLWindowTitle& title,
			                             const XdevLWindowPosition& position,
			                             const XdevLWindowSize& size,
			                             const XdevLWindowTypes& type) = 0;

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

			/// Clips the position.
			virtual xdl_int clip(xdl_uint x, xdl_uint y, xdl_uint width, xdl_uint height) = 0;

			/// Release clipping position.
			virtual void releaseClip() = 0;

			/// Enable relative motion mode.
			virtual xdl_int enableRelativeMotion() = 0;

			/// Disable relative motion mode.
			virtual void disableRelativeMotion() = 0;
	};

	typedef XdevLWindow*		IPXdevLWindow;
	typedef XdevLWindow			IXdevLWindow;

	typedef XdevLWindowServer*	IPXdevLWindowServer;
	typedef XdevLWindowServer	IXdevLWindowServer;

	typedef XdevLWindowEventServer* IPXdevLWindowEventServer;
	typedef XdevLWindowEventServer 	IXdevLWindowEventServer;

	typedef XdevLCursor*		IPXdevLCursor;
	typedef XdevLCursor			IXdevLCursor;

}

#endif
