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

#ifndef XDEVL_WINDOW_WAYLAND_H
#define XDEVL_WINDOW_WAYLAND_H

#include <tinyxml.h>
#include <XdevLPlatform.h>
#include <XdevLPlugin.h>
#include <XdevLTypes.h>
#include <XdevLPluginImpl.h>
#include <XdevLCoreMediator.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLWindow/XdevLWindowImpl.h>

#include <wayland-client.h>
#include <wayland-util.h>
#include <wayland-egl.h>


#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>


namespace xdl {


	// Holds the plugin major version.
	const xdl_uint XdevLWindowPluginMajorVersion = XDEVLWAYLAND_MAJOR_VERSION;

	// Holds the plugin minor version.
	const xdl_uint XdevLWindowPluginMinorVersion = XDEVLWAYLAND_MINOR_VERSION;

	// Holds the plugin patch version.
	const xdl_uint XdevLWindowPluginPatchVersion = XDEVLWAYLAND_PATCH_VERSION;


	// Holds the Major version number.
	const xdl_uint XdevLWindowMajorVersion = XDEVLWAYLAND_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLWindowMinorVersion = XDEVLWAYLAND_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLWindowPatchVersion = XDEVLWAYLAND_MODULE_PATCH_VERSION;

	enum XdevLWindowWaylandModules {
	    WAYLAND_MODULE_1,
	    SDL_NUMBER_OF_MODULES
	};

	static const XdevLString windowWaylandVendor {
		"www.codeposer.net"
	};
	static const XdevLString windowWaylandAuthor {
		"Cengiz Terzibas"
	};
	static const XdevLString windowWaylandCopyright {
		"(c) 2005 - 2015 Cengiz Terzibas."
	};
	static const XdevLString windowWaylandPluginName {
		"XdevLWindowWayland"
	};
	static const XdevLString windowWaylandDescription {
		"Support for Wayland window creation."
	};
	static const std::vector<XdevLModuleName> windowWaylandModuleNames {
		XdevLModuleName("XdevLWindow")
	};



	typedef struct wl_display* 			WaylandDisplay;
	typedef struct wl_compositor* 		WaylandCompositor;
	typedef struct wl_registry*			WaylandRegistry;
	typedef struct wl_surface*			WaylandSurface;
	typedef struct wl_shell*			WaylandShell;
	typedef struct wl_shell_surface*	WaylandShellSurface;
	typedef wl_egl_window* 				WaylandEGLWindow;


	class XdevLEGL {
		public:
			EGLDisplay m_eglDisplay;
			EGLContext m_eglContext;
			EGLConfig  m_eglConfig;
	};

	class XdevLWindowWayland : public XdevLWindowImpl  {
		public:
			XdevLWindowWayland(XdevLModuleCreateParameter* parameter);

			static XdevLModuleDescriptor m_moduleDescriptor;

			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;
			virtual void* getInternal(const XdevLInternalName& id) override;
			virtual xdl_int update() override;

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

			/// Sets the window type.
			virtual void SetType(XdevLWindowTypes type);

			/// Show the pointer.
			virtual void showPointer();

			/// Hide the pointer.
			virtual void hidePointer();

			/// Sets the pointer position.
			virtual void setPointerPosition(xdl_uint x, xdl_uint y);

			/// Clips the pointer position.
			virtual void clipPointerPosition(xdl_uint x, xdl_uint y, xdl_uint width, xdl_uint height);

			/// Show the window.
			/**
				Shows the window without setting the input focus.
			*/
			virtual void show();

			/// Hide the window.
			virtual void hide();

			/// Returns if the window is hidden.
			virtual xdl_bool isHidden();

			/// Raise window above others.
			/**
				The window will be restacked and will become the top most ( in Z order)
			*/
			virtual void raise();

			/// Grabs the pointer.
			virtual void grabPointer();

			/// Ungrabs the pointer.
			virtual void ungrabPointer();

			/// Grabs the keyboard.
			virtual void grabKeyboard();

			/// Ungrabs the keyboard.
			virtual void ungrabKeyboard();

			/// Sets the input focus.
			virtual void setInputFocus();

			/// Checks if the window has focus.
			virtual xdl_bool hasFocus();

			/// Returns the input focus window.
			virtual xdl_int getInputFocus(XdevLWindow** window);

			/// Sets the parent window.
			virtual void setParent(XdevLWindow* window);

			void setCompositor(WaylandCompositor compositor);
			void setShell(WaylandShell shell);
			xdl_int InitEGL();
		protected:
			WaylandCompositor 		m_compositor;
			WaylandRegistry				m_registry;
			WaylandSurface				m_surface;
			WaylandShell					m_shell;
			WaylandShellSurface 	m_shellSurface;
			WaylandEGLWindow			m_eglWindow;

			XdevLEGL							m_egl;
			xdl_int								m_bufferSize;
	};

}


#endif
