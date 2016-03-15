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

#ifndef XDEVL_WINDOW_SDL_H
#define XDEVL_WINDOW_SDL_H

#include <tinyxml.h>
#include <XdevLPlatform.h>
#include <XdevLPlugin.h>
#include <XdevLTypes.h>
#include <XdevLPluginImpl.h>
#include <XdevLCoreMediator.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLWindow/XdevLWindowImpl.h>

#include <SDL.h>
#include <SDL_mouse.h>
#include <SDL_syswm.h>

namespace xdl {

	// Holds the plugin major version.
	const xdl_uint XdevLWindowPluginMajorVersion = XDEVLSDL_MAJOR_VERSION;

	// Holds the plugin minor version.
	const xdl_uint XdevLWindowPluginMinorVersion = XDEVLSDL_MINOR_VERSION;

	// Holds the plugin patch version.
	const xdl_uint XdevLWindowPluginPatchVersion = XDEVLSDL_PATCH_VERSION;



	// Holds the Major version number.
	const xdl_uint XdevLWindowMajorVersion = XDEVLSDL_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLWindowMinorVersion = XDEVLSDL_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLWindowPatchVersion = XDEVLSDL_MODULE_PATCH_VERSION;



	// Holds the Major version number.
	const xdl_uint XdevLWindowEventServerMajorVersion = XDEVLSDL_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLWindowEventServerMinorVersion = XDEVLSDL_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLWindowEventServerPatchVersion = XDEVLSDL_MODULE_PATCH_VERSION;



	static const XdevLString windowPluginName {
		"XdevLWindowSDL"
	};

	static const XdevLString windowDescription {
		"Support for creating a window using SDL."
	};

	static const XdevLString cursorDescription {
		"Support for handling the cursor using SDL."
	};

	class XdevLWindowSDL : public XdevLWindowImpl, public thread::Thread  {
		public:
			XdevLWindowSDL(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor);
			virtual ~XdevLWindowSDL();

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

			SDL_Window* getNativeWindow() {
				return m_window;
			}

		protected:

			xdl_int RunThread(thread::ThreadArgument* argument);



		protected:
			SDL_Window*				m_window;
			SDL_SysWMinfo 			m_wmInfo;

	};


	class XdevLWindowServerSDL : public XdevLWindowServerImpl {
		public:
			XdevLWindowServerSDL(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor);
			virtual ~XdevLWindowServerSDL();

			/// Creates a new window.
			virtual xdl_int createWindow(XdevLWindow** window,
			                             const XdevLWindowTitle& title,
			                             const XdevLWindowPosition& position,
			                             const XdevLWindowSize& size,
			                             const XdevLWindowTypes& type);
	};


	class XdevLWindowSDLEventServer : public XdevLWindowEventServerImpl {
		public:
			XdevLWindowSDLEventServer(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor);
			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;
			virtual void* getInternal(const XdevLInternalName& id) override;
			virtual xdl_int update() override;
			virtual xdl_int notify(XdevLEvent& event) override;

			virtual xdl_int registerWindowForEvents(XdevLWindow* window) override;
			virtual xdl_int unregisterWindowFromEvents(XdevLWindow* window) override;
			void flush() override;
		private:
			int pollEvents();
	};


	class XdevLCursorSDL : public XdevLModuleImpl<XdevLCursor>  {
		public:
			virtual ~XdevLCursorSDL() {}

			XdevLCursorSDL(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor);

			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;
			virtual void* getInternal(const XdevLInternalName& id) override;

			virtual xdl_int attach(XdevLWindow* window) override;
			virtual void show();
			virtual void hide();
			virtual void setPosition(xdl_uint x, xdl_uint y);
			virtual xdl_int clip(xdl_uint x, xdl_uint y, xdl_uint width, xdl_uint height);
			virtual void releaseClip() override;
			virtual xdl_int enableRelativeMotion();
			virtual void disableRelativeMotion();
			virtual xdl_bool isRelativeMotionEnabled() override;
		private:
			XdevLWindowSDL* m_window;

	};
}


#endif
