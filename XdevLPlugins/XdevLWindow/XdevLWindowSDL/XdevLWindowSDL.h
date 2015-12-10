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
			XdevLWindowSDL(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLWindowSDL();

			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;
			virtual void* getInternal(const XdevLInternalName& id) override;
			virtual xdl_int update();

			virtual const XdevLWindowPosition& getPosition();
			virtual const XdevLWindowSize& getSize();

			virtual void showPointer();
			virtual void hidePointer();
			virtual void setPointerPosition(xdl_uint x, xdl_uint y);
			virtual void clipPointerPosition(xdl_uint x, xdl_uint y, xdl_uint width, xdl_uint height) ;

			virtual void setSize(const XdevLWindowSize& size);
			virtual void setPosition(const XdevLWindowPosition& position);
			virtual void setTitle(const XdevLWindowTitle& title);
			virtual void setFullscreen(xdl_bool state);
			virtual void setX(XdevLWindowPosition::type x);
			virtual void setY(XdevLWindowPosition::type y);
			virtual void setWidth(XdevLWindowSize::type width);
			virtual void setHeight(XdevLWindowSize::type height);
			virtual void setType(XdevLWindowTypes type);
			virtual XdevLWindowSize::type getWidth();
			virtual XdevLWindowSize::type getHeight();
			virtual XdevLWindowPosition::type getX();
			virtual XdevLWindowPosition::type getY();

			virtual const XdevLWindowTitle& getTitle();
			virtual xdl_bool getFullscreen();
			virtual xdl_int getColorDepth();
			virtual xdl_bool getHidePointer();
			virtual void show();
			virtual void hide();
			virtual xdl_bool isHidden();
			virtual void raise();
			virtual void grabPointer();
			virtual void ungrabPointer();
			virtual void grabKeyboard();
			virtual void ungrabKeyboard();
			virtual void setInputFocus();
			virtual xdl_bool hasFocus();

			virtual void setParent(XdevLWindow* window);

			virtual xdl_int create() override;
			
			SDL_Window* getNativeWindow() {return m_window;}

		protected:

			xdl_int RunThread(thread::ThreadArgument* argument);



		protected:
			SDL_Window*				m_window;
			SDL_SysWMinfo 			m_wmInfo;

	};


	class XdevLWindowServerSDL : public XdevLWindowServerImpl {
		public:
			XdevLWindowServerSDL(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLWindowServerSDL();

			/// Creates a new window.
			virtual xdl_int createWindow(XdevLWindow** window,
			                             const XdevLWindowTitle& title,
			                             const XdevLWindowPosition& position,
			                             const XdevLWindowSize& size);
	};


	class XdevLWindowSDLEventServer : public XdevLWindowEventServerImpl {
		public:
			XdevLWindowSDLEventServer(XdevLModuleCreateParameter* parameter);
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
			
			XdevLCursorSDL(XdevLModuleCreateParameter* parameter);

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
		private:
			XdevLWindowSDL* m_window;

	};
}


#endif
