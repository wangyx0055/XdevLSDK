/*	XdevL eXtended DEVice Library.	Copyright © 2005-2012 Cengiz Terzibas	This library is free software; you can redistribute it and/or modify it under the	terms of the GNU Lesser General Public License as published by the Free Software	Foundation; either version 2.1 of the License, or (at your option) any later version.	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the GNU Lesser General Public License for more details.	You should have received a copy of the GNU Lesser General Public License along with	this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,	Suite 330, Boston, MA 02111-1307 USA	I would appreciate if you report all bugs to: cengiz@terzibas.de*/#ifndef XDEVL_WINDOW_COCOA_H#define XDEVL_WINDOW_COCOA_H#include <tinyxml.h>#include <XdevLPlatform.h>#include <XdevLPlugin.h>#include <XdevLWindow/XdevLWindowImpl.h>#include <XdevLTypes.h>#import <Cocoa/Cocoa.h>namespace xdl {	// Holds the plugin major version.	const xdl_uint XdevLWindowPluginMajorVersion = XDEVLCOCOA_MAJOR_VERSION;	// Holds the plugin minor version.	const xdl_uint XdevLWindowPluginMinorVersion = XDEVLCOCOA_MINOR_VERSION;	// Holds the plugin patch version.	const xdl_uint XdevLWindowPluginPatchVersion = XDEVLCOCOA_PATCH_VERSION;	// Holds the Major version number.	const xdl_uint XdevLWindowMajorVersion = XDEVLCOCOA_MODULE_MAJOR_VERSION;	// Holds the Minor version number.	const xdl_uint XdevLWindowMinorVersion = XDEVLCOCOA_MODULE_MINOR_VERSION;	// Holds the Patch version number.	const xdl_uint XdevLWindowPatchVersion = XDEVLCOCOA_MODULE_PATCH_VERSION;	// Holds the Major version number.	const xdl_uint XdevLWindowEventServerMajorVersion = XDEVLCOCOA_MODULE_MAJOR_VERSION;	// Holds the Minor version number.	const xdl_uint XdevLWindowEventServerMinorVersion = XDEVLCOCOA_MODULE_MINOR_VERSION;	// Holds the Patch version number.	const xdl_uint XdevLWindowEventServerPatchVersion = XDEVLCOCOA_MODULE_PATCH_VERSION;	static const XdevLString windowPluginName {		"XdevLWindowCocoa"	};	static const XdevLString windowDescription {		"Support for creating a window using Cocoa."	};	static const XdevLString cursorDescription {		"Support for handling the cursor using Cocoa."	};	class XdevLWindowCocoa : public XdevLWindowImpl, public thread::Thread  {		public:			XdevLWindowCocoa(XdevLModuleCreateParameter* parameter);			virtual ~XdevLWindowCocoa() {			}			//			// XdevLModule related methods.			//			virtual xdl_int init() override;			virtual xdl_int shutdown() override;			virtual void* getInternal(const XdevLInternalName& id) override;			virtual xdl_int update() override;			//			// XdevLWindow related methods.			//			virtual xdl_int create() override;			virtual const XdevLWindowPosition& getPosition() override;			virtual const XdevLWindowSize& getSize() override;			virtual XdevLWindowPosition::type getX() override;			virtual XdevLWindowPosition::type getY() override;			virtual XdevLWindowSize::type getWidth() override;			virtual XdevLWindowSize::type getHeight() override;			virtual const XdevLWindowTitle& getTitle() override;			virtual xdl_bool getFullscreen() override;			virtual xdl_bool getHidePointer() override;			virtual xdl_int getColorDepth() override;			virtual void setPosition(const XdevLWindowPosition& position) override;			virtual void setSize(const XdevLWindowSize& size) override;			virtual void setX(XdevLWindowPosition::type x) override;			virtual void setY(XdevLWindowPosition::type y) override;			virtual void setWidth(XdevLWindowSize::type width) override;			virtual void setHeight(XdevLWindowSize::type height) override;			virtual void setTitle(const XdevLWindowTitle& title) override;			virtual void setFullscreen(xdl_bool state) override;			virtual void showPointer() override;			virtual void hidePointer() override;			virtual void setPointerPosition(xdl_uint x, xdl_uint y) override;			virtual void show() override;			virtual void hide() override;			virtual xdl_bool isHidden() override;			virtual void raise() override;			virtual void grabPointer() override;			virtual void ungrabPointer() override;			virtual void setInputFocus() override;			virtual xdl_bool hasFocus() override;			virtual void setParent(XdevLWindow* window) override;			virtual void setType(XdevLWindowTypes type) override;			//			// Internal used methods			//			id getNativeWindow() { return m_window; }		private:			id			m_window;			id			m_delegate;			id			m_view;		};	class XdevLWindowServerCocoa : public XdevLWindowServerImpl {		public:			XdevLWindowServerCocoa(XdevLModuleCreateParameter* parameter);			virtual ~XdevLWindowServerCocoa();			/// Creates a new window.			virtual xdl_int createWindow(XdevLWindow** window,			                             const XdevLWindowTitle& title,			                             const XdevLWindowPosition& position,			                             const XdevLWindowSize& size);	};	class XdevLWindowCocoaEventServer : public XdevLWindowEventServerImpl {		public:			XdevLWindowCocoaEventServer(XdevLModuleCreateParameter* parameter);			virtual xdl_int init() override;			virtual xdl_int shutdown() override;			virtual void* getInternal(const XdevLInternalName& id) override;			virtual xdl_int update() override;			virtual xdl_int registerWindowForEvents(XdevLWindow* window) override;			virtual xdl_int unregisterWindowFromEvents(XdevLWindow* window) override;			void flush() override;	};	class XdevLCursorCocoa : public XdevLModuleImpl<XdevLCursor>  {		public:			virtual ~XdevLCursorCocoa() {}			XdevLCursorCocoa(XdevLModuleCreateParameter* parameter);			virtual xdl_int init() override;			virtual xdl_int shutdown() override;			virtual void* getInternal(const XdevLInternalName& id) override;			virtual xdl_int attach(XdevLWindow* window) override;			virtual void show();			virtual void hide();			virtual void setPosition(xdl_uint x, xdl_uint y);			virtual xdl_int clip(xdl_uint x, xdl_uint y, xdl_uint width, xdl_uint height);			virtual void releaseClip();			xdl_int enableRelativeMotion();			void disableRelativeMotion();		private:			NSCursor* m_cursor;	};} // End of namespace@interface XdevLCocoaApplication : NSApplication@end@implementation XdevLCocoaApplication// From http://cocoadev.com/index.pl?GameKeyboardHandlingAlmost// This works around an AppKit bug, where key up events while holding// down the command key don't get sent to the key window.- (void)sendEvent:(NSEvent *)event{    if ([event type] == NSKeyUp && ([event modifierFlags] & NSCommandKeyMask))        [[self keyWindow] sendEvent:event];    else        [super sendEvent:event];}@end@interface XdevLApplicationDelegate : NSObject@end@implementation XdevLApplicationDelegate- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender{    return NSTerminateCancel;}- (void)applicationDidHide:(NSNotification *)notification{	std::cout << "applicationDidHide" << std::endl;}- (void)applicationDidUnhide:(NSNotification *)notification{	std::cout << "applicationDidUnhide" << std::endl;}- (void)applicationDidChangeScreenParameters:(NSNotification *) notification{	std::cout << "applicationDidChangeScreenParameters" << std::endl;}@end@interface XdevLWindowDelegate : NSObject{    xdl::XdevLWindowCocoaEventServer* m_windowEventServer;	xdl::XdevLWindowCocoa* m_window;}- (id)init:(xdl::XdevLWindowCocoaEventServer*)windowEventServer :(xdl::XdevLWindowCocoa*)window;@end@interface XdevLContentView : NSView{	xdl::XdevLWindowCocoa* m_window;    xdl::XdevLWindowCocoaEventServer* m_windowEventServer;    NSTrackingArea* trackingArea;	xdl::xdl_int m_modifierKeys; }- (id)init:(xdl::XdevLWindowCocoaEventServer*)windowEventServer :(xdl::XdevLWindowCocoa*)window;@endextern "C" XDEVL_EXPORT int _createWindowDevice(xdl::XdevLModuleCreateParameter* parameter);extern "C" XDEVL_EXPORT void _deleteWindowDevice(xdl::XdevLModule* obj);extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getWindowPluginInfo();#endif