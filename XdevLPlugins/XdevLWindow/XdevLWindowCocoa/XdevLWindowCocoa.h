/*	XdevL eXtended DEVice Library.	Copyright © 2005 - 2016 Cengiz Terzibas	This library is free software; you can redistribute it and/or modify it under the	terms of the GNU Lesser General Public License as published by the Free Software	Foundation; either version 2.1 of the License, or (at your option) any later version.	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the GNU Lesser General Public License for more details.	You should have received a copy of the GNU Lesser General Public License along with	this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,	Suite 330, Boston, MA 02111-1307 USA	I would appreciate if you report all bugs to: cengiz@terzibas.de*/#ifndef XDEVL_WINDOW_COCOA_H#define XDEVL_WINDOW_COCOA_H#include <tinyxml.h>#include <XdevLPlatform.h>#include <XdevLPlugin.h>#include <XdevLWindow/XdevLWindowImpl.h>#include <XdevLTypes.h>#include "XdevLCursorCocoa.h"#include "XdevLWindowEventServerCocoa.h"#include "XdevLWindowServerCocoa.h"// Import stuff for the cocoa window.#import <Cocoa/Cocoa.h>// Import stuff for the cursor.#import <IOKit/hid/IOHIDKeys.h>#import <IOKit/hidsystem/IOHIDShared.h>namespace xdl {	static const XdevLString windowPluginName {		"XdevLWindowCocoa"	};	static const XdevLString windowDescription {		"Support for creating a window using Cocoa."	};	static const XdevLString cursorDescription {		"Support for handling the cursor using Cocoa."	};	class XdevLWindowCocoa : public XdevLWindowImpl {		public:			XdevLWindowCocoa(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor);			virtual ~XdevLWindowCocoa();			static xdl_int initCocoa(XdevLCoreMediator* core);			static xdl_int shutdownCocoa();			//			// XdevLModule related methods.			//			virtual xdl_int init() override;			virtual xdl_int shutdown() override;			virtual void* getInternal(const XdevLInternalName& id) override;			virtual xdl_int update() override;			//			// XdevLWindow related methods.			//			virtual xdl_int create() override;			virtual xdl_int create(const XdevLWindowAttribute& attribute) override;			virtual const XdevLWindowPosition& getPosition() override;			virtual const XdevLWindowSize& getSize() override;			virtual XdevLWindowPosition::type getX() override;			virtual XdevLWindowPosition::type getY() override;			virtual XdevLWindowSize::type getWidth() override;			virtual XdevLWindowSize::type getHeight() override;			virtual const XdevLWindowTitle& getTitle() override;			virtual xdl_bool isFullscreen() override;			virtual xdl_bool isPointerHidden() override;			virtual void setPosition(const XdevLWindowPosition& position) override;			virtual void setSize(const XdevLWindowSize& size) override;			virtual void setX(XdevLWindowPosition::type x) override;			virtual void setY(XdevLWindowPosition::type y) override;			virtual void setWidth(XdevLWindowSize::type width) override;			virtual void setHeight(XdevLWindowSize::type height) override;			virtual void setTitle(const XdevLWindowTitle& title) override;			virtual void setFullscreen(xdl_bool state) override;			virtual void show() override;			virtual void hide() override;			virtual xdl_bool isHidden() override;			virtual void raise() override;			virtual void setInputFocus() override;			virtual xdl_bool hasFocus() override;			virtual void setParent(XdevLWindow* window) override;			virtual void setType(XdevLWindowTypes type) override;			//			// Internal used methods			//			id getNativeWindow() {				return m_window;			}		private:			id			m_window;			id			m_delegate;			id			m_view;	};class XdevLCococaInit {	public:		XdevLCococaInit(XdevLCoreMediator* core);		~XdevLCococaInit();		XdevLWindowEventServerCocoa* getWindowEventServer() {			return windowEventServer.get();		}		XdevLCursorCocoa* getCursor() {			return cursor.get();		}public:		std::shared_ptr<XdevLWindowEventServerCocoa> windowEventServer;		std::shared_ptr<XdevLCursorCocoa> cursor;	};} // End of namespace//// Cocoa application interface//@interface XdevLCocoaApplication : NSApplication@end//// Cocoa object interface.//@interface XdevLApplicationDelegate : NSObject@end//// Cocoa delegate interface.//@interface XdevLWindowDelegate : NSObject {	xdl::XdevLWindowEventServerCocoa* m_windowEventServer;	xdl::XdevLWindowCocoa* m_window;}- (id)init:(xdl::XdevLWindowEventServerCocoa*)windowEventServer :(xdl::XdevLWindowCocoa*)window;@end//// Cocoa View interface.//@interface XdevLContentView : NSView {	xdl::XdevLWindowCocoa* m_window;	xdl::XdevLWindowEventServerCocoa* m_windowEventServer;	NSTrackingArea* trackingArea;	xdl::xdl_int m_modifierKeys;}- (id)init:(xdl::XdevLWindowEventServerCocoa*)windowEventServer :(xdl::XdevLWindowCocoa*)window;@end#endif