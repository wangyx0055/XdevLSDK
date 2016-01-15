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

	/**
		@enum XdevLWindowModuleName
		@brief ID to get the correct modules names for the XdevLWindow plugins.
	*/
	enum XdevLWindowModuleName {
	  XDEVL_WINDOW_MODULE_NAME,
	  XDEVL_WINDOW_SERVER_MODULE_NAME,
	  XDEVL_WINDOW_EVENT_SERVER_MODULE_NAME,
	  XDEVL_CURSOR_MODULE_NAME
	};

	static const XdevLString window_vendor 				{
		"www.codeposer.net"
	};
	static const XdevLString window_author				{
		"Cengiz Terzibas"
	};
	static const XdevLString window_copyright			{
		"(c) 2005 - 2016 Cengiz Terzibas."
	};
	static const XdevLString windowServerDescription	{
		"Window Server that manages windows."
	};

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
			virtual xdl_int create(const XdevLWindowAttribute& attribute);
			virtual xdl_uint64 getWindowID();
			virtual XdevLWindowPosition::type getX() const;
			virtual XdevLWindowPosition::type getY() const;
			virtual XdevLWindowSize::type getWidth() const;
			virtual XdevLWindowSize::type getHeight() const;
			virtual const XdevLWindowTitle& getTitle();
			virtual const XdevLWindowSize& getSize();
			virtual const XdevLWindowPosition& getPosition();
			virtual xdl_bool getFullscreen() const;
			virtual xdl_int getColorDepth() const;
			virtual xdl_bool getHidePointer() const;
			virtual void setX(XdevLWindowPosition::type x);
			virtual void setY(XdevLWindowPosition::type y);
			virtual void setWidth(XdevLWindowSize::type width);
			virtual void setHeight(XdevLWindowSize::type height);
			void setColorDepth(int depth);
			virtual void setTitle(const XdevLWindowTitle& title);
			virtual void setPosition(const XdevLWindowPosition& position);
			virtual void setSize(const XdevLWindowSize& size);
			virtual void setHidePointer(xdl_bool state);
			static void increaseWindowCounter();
			static xdl_uint getWindowsCounter();
			void setParent(XdevLWindow* window);
			virtual xdl_int notify(XdevLEvent& event) override;
			virtual void setWindowDecoration(xdl_bool enable);
			virtual xdl_bool isPointerInside() override;
			virtual XdevLWindowTypes getType() override;
		protected:
			// Holds the root window.
			XdevLWindow*	m_rootWindow;

			// The identification code.
			xdl_uint64 m_id;

			// Holds the window attributes.
			XdevLWindowAttribute m_attribute;

			// Holds the windows title.
			XdevLWindowTitle m_title;

			// The name of the root window.
			XdevLWindowTitle m_rootTitle;

			/// Full screen yes or no
			xdl_bool m_fullScreen;

			/// color depth of the framebuffer
			xdl_int m_colorDepth;

			// Should the mouse pointer be hided?
			xdl_bool m_hideMouse;

			// Holds the background color.
			xdl_uint m_backgroundColor[4];

			xdl_bool m_pointerIsInside;
		protected:
			xdl_int readWindowInfo(TiXmlDocument& document);
			xdl_int stringToWindowType(const std::string& string);

	};

	class XdevLWindowServerImpl : public XdevLModuleAutoImpl<XdevLWindowServer> {
		public:
			XdevLWindowServerImpl(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor);
			virtual ~XdevLWindowServerImpl();

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
	extern xdl::xdl_int initDefaultWindowInstances(xdl::XdevLCoreMediator* parameter);
}

#endif
