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
		XdevLModuleName("XdevLWindowServer"),
		XdevLModuleName("XdevLWindowEventServer"),
		XdevLModuleName("XdevLCursor"),
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


	class XdevLWindowImpl : public XdevLModuleAutoImpl<XdevLWindow> {
		public:
			XdevLWindowImpl(xdl_uint id, XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor);
			virtual ~XdevLWindowImpl() {}

			virtual xdl_int init();
			virtual xdl_int shutdown();
			virtual xdl_uint64 getWindowID();
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
			virtual xdl_int notify(XdevLEvent& event) override;
			virtual void setWindowDecoration(xdl_bool enable);
			virtual xdl_bool isPointerInside() override;
		protected:
			// Holds the root window.
			XdevLWindow*	m_rootWindow;

			// The identification code.
			xdl_uint64 m_id;

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


			
			XdevLWindowTypes m_windowType;

			xdl_bool m_pointerIsInside;
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

			std::map<xdl_uint64, XdevLWindow*> m_windowList;
	};

	class XdevLWindowEventServerImpl : public XdevLModuleAutoImpl<XdevLWindowEventServer> {
	public:
		typedef std::map<xdl_uint64, XdevLWindow*> WindowEventMapType;

		XdevLWindowEventServerImpl(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor);
		virtual ~XdevLWindowEventServerImpl() {}

		virtual xdl_bool isWindowRegistered(XdevLWindow* window) override;
		virtual xdl_int registerWindowForEvents(XdevLWindow* window) override;
		virtual xdl_int unregisterWindowFromEvents(XdevLWindow* window) override;
		virtual XdevLWindow* getWindow(xdl_uint64 id) override;
		virtual XdevLWindow* getFocus() const override;
		virtual void flush() override;

		void focusGained(XdevLWindow* window);

	private:
		XdevLWindow* m_focusWindow;
		WindowEventMapType m_windows;
		

	};

	extern xdl_bool isEventThreadRunning();

	extern void startEventThread();

	extern void stopEventThread();
	
	extern xdl::XdevLWindowEventServer* windowEventServer;
	extern xdl::XdevLCursor* cursor;
	extern xdl::XdevLModuleCreateParameter* XdevLWindowEventServerParameter;

}

#endif
