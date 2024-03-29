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

#ifndef XDEVL_WINDOW_WAYLAND_H
#define XDEVL_WINDOW_WAYLAND_H

#include <tinyxml.h>
#include <XdevLPlatform.h>
#include <XdevLPlugin.h>
#include <XdevLTypes.h>
#include <XdevLPluginImpl.h>
#include <XdevLCoreMediator.h>
#include <XdevLSharedMemory.h>
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

	class XdevLEGL {
		public:
			EGLDisplay 		m_eglDisplay;
			EGLSurface 		m_eglSurface;
			EGLContext 		m_eglContext;
			EGLConfig  		m_eglConfig;
			wl_egl_window*	m_eglWindow;
	};

	class XdevLWindowWayland : public XdevLWindowImpl  {
		public:
			XdevLWindowWayland(XdevLModuleCreateParameter* parameter);

			static XdevLModuleDescriptor m_moduleDescriptor;

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

			void setFrameCallback(wl_callback* frameCallback);
			wl_surface* getSurface() const;
			wl_shell_surface* getShellSurface() const;
			wl_callback* getFrameCallback() const;
			wl_buffer* getBuffer() const;
			virtual void onPaint();
			virtual void onSizeChanged(xdl_int width, xdl_int height);
			
			wl_surface* getNativeSurface() {return m_surface;}
			wl_shell_surface* getNativeShellSurface() {return m_shellSurface;}
			
		protected:
			int setCloexecOrClose(int fd);
			int createTmpFileCloExec(char *tmpname) ;
			xdl_int createAnonymousFile(off_t size) ;
			xdl_int createBuffer();
			void createOpaqueRegion(xdl_int x, xdl_int y, xdl_int width, xdl_int height);

			void paint_pixels();
			xdl_int initializeEGL();
		protected:
			wl_surface*			m_surface;
			wl_shell_surface* 	m_shellSurface;
			wl_callback*		m_frameCallback;
			wl_shm_pool*		m_pool;
			wl_buffer*			m_buffer;
			wl_region*			m_region;

			XdevLEGL m_egl;
			xdl_int m_bufferSize;
			xdl_int m_fd;
			xdl_uint8* m_shm_data;
	};

	class XdevLWindowServerWayland : public XdevLWindowServerImpl {
		public:
			XdevLWindowServerWayland(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLWindowServerWayland();

			/// Creates a new window.
			virtual xdl_int create(XdevLWindow** window, const XdevLWindowAttribute& attribute);
	};

	class XdevLWindowEventServerWayland : public XdevLWindowEventServerImpl {
		public:
			XdevLWindowEventServerWayland(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLWindowEventServerWayland();

			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;
			virtual void* getInternal(const XdevLInternalName& id) override;
			virtual xdl_int update() override;

			virtual xdl_int registerWindowForEvents(XdevLWindow* window) override;
			virtual xdl_int unregisterWindowFromEvents(XdevLWindow* window) override;
			void flush() override;
			
			static void setSeat();
			void setPointer(wl_pointer* pointer);
			void setKeyboard(wl_keyboard* keyboard);
			void setCurrentWindow(wl_surface* surface);
			void setFocusWindow(XdevLWindow* window);
			void setCurrentPointerPosition(xdl_int x, xdl_int y);

			void getCurrentPointerPosition(xdl_int& x, xdl_int& y);
			wl_surface* getCurrentWindow() {return m_currentSurface;}
			XdevLWindow* getFocusWindow();


		private:
			wl_pointer* m_pointer; 
			wl_keyboard* m_keyboard;
			wl_surface* m_currentSurface;
			XdevLWindow* m_focusWindow;
			xdl_int m_pointerPositionX;
			xdl_int m_pointerPositionY;
	};


	class XdevLCursorWayland : public XdevLModuleImpl<XdevLCursor>  {
		public:
			XdevLCursorWayland(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLCursorWayland();

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
	};
	
	extern xdl_int initWayland();
	extern void shutdownWayland();
}


#endif
