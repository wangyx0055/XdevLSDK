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

#ifndef XDEVL_WINDOW_IMPL_H
#define XDEVL_WINDOW_IMPL_H

#include <tinyxml.h>
#include <vector>

#include <XdevLPlatform.h>
#include <XdevLPluginImpl.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLThread.h>
#include <XdevLMutex.h>


namespace xdl {

	static const XdevLString window_vendor 				{"www.codeposer.net"};
	static const XdevLString window_author				{"Cengiz Terzibas"};
	static const XdevLString window_copyright			{"(c) 2005 - 2015 Cengiz Terzibas."};
	static const XdevLString windowServerDescription	{"Window Server that manages windows."};

	static const std::vector<XdevLModuleName>	window_moduleNames	{
		XdevLModuleName("XdevLWindow"),
		XdevLModuleName("XdevLWindowServer")
	};

	// Holds the Major version number.
	const xdl_uint XdevLWindowServerMajorVersion = 1;
	
	// Holds the Minor version number.
	const xdl_uint XdevLWindowServerMinorVersion = 0;
	
	// Holds the Patch version number.
	const xdl_uint XdevLWindowServerPatchVersion = 0;

	struct XdevLWindowThreadArgument : public thread::ThreadArgument {
		xdl::XdevLCoreMediator* core;
	};

	class XdevLWindowBase {
		public:
			XdevLWindowBase() :
				m_id(0),
				m_title("XdevL (c) 2005 - 2015 Cengiz Terzibas"),
				m_fullScreen(xdl_false),
				m_colorDepth(32),
				m_hideMouse(xdl_false),
				m_border(xdl_true) {

				m_position.x = 0;
				m_position.y = 0;
				m_size.width = 640;
				m_size.height = 480;

				// Set background color.
				m_backgroundColor[0] = 0;
				m_backgroundColor[1] = 0;
				m_backgroundColor[2] = 0;
				m_backgroundColor[3] = 0;

			}
			virtual ~XdevLWindowBase() {}

			virtual xdl_uint getWindowID();
			virtual XdevLWindowPosition::type getX() const;
			virtual XdevLWindowPosition::type getY() const;
			virtual XdevLWindowSize::type getWidth() const;
			virtual XdevLWindowSize::type getHeight() const;
			virtual const XdevLWindowTitle& getTitle();
			virtual xdl_bool getFullscreen() const;
			virtual xdl_int getColorDepth() const;
			virtual xdl_bool getHidePointer() const;
			virtual void setX(XdevLWindowPosition::type x);
			virtual void setY(XdevLWindowPosition::type y);
			virtual void setWidth(XdevLWindowSize::type width);
			virtual void setHeight(XdevLWindowSize::type height);
			void setColorDepth(int depth);
			virtual void setTitle(const XdevLWindowTitle& title);
			virtual void setHidePointer(xdl_bool state);
			xdl_bool getWindowBorder();

		protected:

			xdl_uint m_id;

			// Holds the windows title.
			XdevLWindowTitle m_title;

			/// Holds the position of the window.
			XdevLWindowPosition m_position;

			/// Holds the size of the window.
			XdevLWindowSize m_size;

			/// fullscreen yes or no
			xdl_bool m_fullScreen;

			/// color depth of the framebuffer
			xdl_int m_colorDepth;

			// Should the mouse pointer be hided?
			xdl_bool m_hideMouse;

			// Use a border around the window?
			xdl_bool m_border;

			// Holds the background color.
			xdl_uint m_backgroundColor[4];

	};

	class XdevLWindowImpl : public XdevLModuleAutoImpl<XdevLWindow> {
		public:
			XdevLWindowImpl(xdl_uint id, XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor);
			virtual ~XdevLWindowImpl() {}

			virtual xdl_int init();
			virtual xdl_int shutdown();
			virtual xdl_uint getWindowID();
			virtual XdevLWindowPosition::type getX() const;
			virtual XdevLWindowPosition::type getY() const;
			virtual XdevLWindowSize::type getWidth() const;
			virtual XdevLWindowSize::type getHeight() const;
			virtual const XdevLWindowTitle& getTitle();
			virtual xdl_bool getFullscreen() const;
			virtual xdl_int getColorDepth() const;
			virtual xdl_bool getHidePointer() const;
			virtual void setX(XdevLWindowPosition::type x);
			virtual void setY(XdevLWindowPosition::type y);
			virtual void setWidth(XdevLWindowSize::type width);
			virtual void setHeight(XdevLWindowSize::type height);
			void setColorDepth(int depth);
			virtual void setTitle(const XdevLWindowTitle& title);
			virtual void setHidePointer(xdl_bool state);
			xdl_bool getWindowBorder();
			static void increaseWindowCounter();
			static xdl_uint getWindowsCounter();
			void setParent(XdevLWindow* window);
			virtual xdl_int notify(XdevLEvent& event);
		protected:
			// Holds the root window.
			XdevLWindow*	m_rootWindow;

			// The identification code.
			xdl_uint m_id;

			// Holds the windows title.
			XdevLWindowTitle m_title;

			// The name of the root window.
			XdevLWindowTitle m_rootTitle;

			/// Holds the position of the window.
			XdevLWindowPosition m_position;

			/// Holds the size of the window.
			XdevLWindowSize			m_size;

			/// fullscreen yes or no
			xdl_bool m_fullScreen;

			/// color depth of the framebuffer
			xdl_int m_colorDepth;

			// Should the mouse pointer be hided?
			xdl_bool m_hideMouse;

			// Use a border around the window?
			xdl_bool m_border;

			// Holds the background color.
			xdl_uint m_backgroundColor[4];

			// TODO This might be changed in future releases
			xdl_uint m_numberOfJoystickDevices;
			xdl_uint m_numberOfJoystickButtons;
			xdl_uint m_numberOfJoystickAxis;
		protected:
			xdl_int readWindowInfo(TiXmlDocument& document);
			xdl_int stringToWindowType(const std::string& string);

	};

	class XdevLWindowServerImpl : public XdevLModuleAutoImpl<XdevLWindowServer> {
		public:
			XdevLWindowServerImpl(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLWindowServerImpl();

			/// The modules descriptor.
			static XdevLModuleDescriptor m_windowServerModuleDesc;

			/// Creates a new window.
			using XdevLWindowServer::createWindow;

			/// Destroys the specified window.
			virtual xdl_int destroy(XdevLWindow* window);

		protected:

			std::map<xdl_uint, XdevLWindow*> m_windowList;
	};

	extern xdl_bool isEventThreadRunning();

	extern void startEventThread();

	extern void stopEventThread();

}

#endif
