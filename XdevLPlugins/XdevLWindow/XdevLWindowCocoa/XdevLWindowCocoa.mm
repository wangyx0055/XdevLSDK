/*	XdevL eXtended DEVice Library.	Copyright © 2005-2015 Cengiz Terzibas	This library is free software; you can redistribute it and/or modify it under the	terms of the GNU Lesser General Public License as published by the Free Software	Foundation; either version 2.1 of the License, or (at your option) any later version.	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the GNU Lesser General Public License for more details.	You should have received a copy of the GNU Lesser General Public License along with	this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,	Suite 330, Boston, MA 02111-1307 USA	I would appreciate if you report all bugs to: cengiz@terzibas.de*/#include <sstream>#include "XdevLWindowCocoa.h"#include <XdevLInputSystem.h>static xdl::xdl_int reference_counter = 0;static id applicationDelegate = 0;static xdl::xdl_bool cocoaInitialized = xdl::xdl_false;static std::shared_ptr<xdl::XdevLCococaInit> cocoaInit;xdl::XdevLPluginDescriptor windowCocoaPluginDescriptor {	xdl::windowPluginName,	xdl::window_moduleNames,	xdl::XdevLWindowPluginMajorVersion,	xdl::XdevLWindowPluginMinorVersion,	xdl::XdevLWindowPluginPatchVersion};xdl::XdevLModuleDescriptor windowCocoaDesc {	xdl::window_vendor,	xdl::window_author,	xdl::window_moduleNames[xdl::XDEVL_WINDOW_MODULE_NAME],	xdl::window_copyright,	xdl::windowDescription,	xdl::XdevLWindowMajorVersion,	xdl::XdevLWindowMinorVersion,	xdl::XdevLWindowPatchVersion};xdl::XdevLModuleDescriptor windowServerCocoaDesc {	xdl::window_vendor,	xdl::window_author,	xdl::window_moduleNames[xdl::XDEVL_WINDOW_SERVER_MODULE_NAME],	xdl::window_copyright,	xdl::windowServerDescription,	xdl::XdevLWindowEventServerMajorVersion,	xdl::XdevLWindowEventServerMinorVersion,	xdl::XdevLWindowEventServerPatchVersion};xdl::XdevLModuleDescriptor windowEventServerCocoaDesc {	xdl::window_vendor,	xdl::window_author,	xdl::window_moduleNames[xdl::XDEVL_WINDOW_EVENT_SERVER_MODULE_NAME],	xdl::window_copyright,	xdl::windowServerDescription,	xdl::XdevLWindowEventServerMajorVersion,	xdl::XdevLWindowEventServerMinorVersion,	xdl::XdevLWindowEventServerPatchVersion};xdl::XdevLModuleDescriptor cursorCocoaDesc {	xdl::window_vendor,	xdl::window_author,	xdl::window_moduleNames[xdl::XDEVL_CURSOR_MODULE_NAME],	xdl::window_copyright,	xdl::windowServerDescription,	xdl::XdevLWindowEventServerMajorVersion,	xdl::XdevLWindowEventServerMinorVersion,	xdl::XdevLWindowEventServerPatchVersion};namespace xdl {	const XdevLID ButtonPressed("XDEVL_BUTTON_PRESSED");	const XdevLID ButtonReleased("XDEVL_BUTTON_RELEASED");	const XdevLID MouseButtonPressed("XDEVL_MOUSE_BUTTON_PRESSED");	const XdevLID MouseButtonReleased("XDEVL_MOUSE_BUTTON_RELEASED");	const XdevLID MouseMotion("XDEVL_MOUSE_MOTION");	const XdevLID WindowEvent("XDEVL_WINDOW_EVENT");	static int translateKey(unsigned int key) {		// Keyboard symbol translation table		static const unsigned int table[128] = {			/* 00 */ KEY_A,			/* 01 */ KEY_S,			/* 02 */ KEY_D,			/* 03 */ KEY_F,			/* 04 */ KEY_H,			/* 05 */ KEY_G,			/* 06 */ KEY_Z,			/* 07 */ KEY_X,			/* 08 */ KEY_C,			/* 09 */ KEY_V,			/* 0a */ KEY_UNKNOWN,			/* 0b */ KEY_B,			/* 0c */ KEY_Q,			/* 0d */ KEY_W,			/* 0e */ KEY_E,			/* 0f */ KEY_R,			/* 10 */ KEY_Y,			/* 11 */ KEY_T,			/* 12 */ KEY_1,			/* 13 */ KEY_2,			/* 14 */ KEY_3,			/* 15 */ KEY_4,			/* 16 */ KEY_6,			/* 17 */ KEY_5,			/* 18 */ KEY_EQUALS,			/* 19 */ KEY_9,			/* 1a */ KEY_7,			/* 1b */ KEY_MINUS,			/* 1c */ KEY_8,			/* 1d */ KEY_0,			/* 1e */ KEY_RIGHTBRACKET,			/* 1f */ KEY_O,			/* 20 */ KEY_U,			/* 21 */ KEY_LEFTBRACKET,			/* 22 */ KEY_I,			/* 23 */ KEY_P,			/* 24 */ KEY_ENTER,			/* 25 */ KEY_L,			/* 26 */ KEY_J,			/* 27 */ KEY_APOSTROPHE,			/* 28 */ KEY_K,			/* 29 */ KEY_SEMICOLON,			/* 2a */ KEY_BACKSLASH,			/* 2b */ KEY_COMMA,			/* 2c */ KEY_SLASH,			/* 2d */ KEY_N,			/* 2e */ KEY_M,			/* 2f */ KEY_PERIOD,			/* 30 */ KEY_TAB,			/* 31 */ KEY_SPACE,			/* 32 */ KEY_GRAVE,			/* 33 */ KEY_BACKSPACE,			/* 34 */ KEY_UNKNOWN,			/* 35 */ KEY_ESCAPE,			/* 36 */ KEY_RGUI,			/* 37 */ KEY_LGUI,			/* 38 */ KEY_LSHIFT,			/* 39 */ KEY_CAPSLOCK,			/* 3a */ KEY_LALT,			/* 3b */ KEY_LCTRL,			/* 3c */ KEY_RSHIFT,			/* 3d */ KEY_RALT,			/* 3e */ KEY_RCTRL,			/* 3f */ KEY_UNKNOWN, /* Function */			/* 40 */ KEY_F17,			/* 41 */ KEY_KP_PERIOD,			/* 42 */ KEY_UNKNOWN,			/* 43 */ KEY_KP_MULTIPLY,			/* 44 */ KEY_UNKNOWN,			/* 45 */ KEY_KP_PLUS,			/* 46 */ KEY_UNKNOWN,			/* 47 */ KEY_NUMLOCK, /* Really KeypadClear... */			/* 48 */ KEY_UNKNOWN, /* VolumeUp */			/* 49 */ KEY_UNKNOWN, /* VolumeDown */			/* 4a */ KEY_UNKNOWN, /* Mute */			/* 4b */ KEY_KP_DIVIDE,			/* 4c */ KEY_KP_ENTER,			/* 4d */ KEY_UNKNOWN,			/* 4e */ KEY_KP_MINUS,			/* 4f */ KEY_F18,			/* 50 */ KEY_F19,			/* 51 */ KEY_KP_ENTER,			/* 52 */ KEY_KP_0,			/* 53 */ KEY_KP_1,			/* 54 */ KEY_KP_2,			/* 55 */ KEY_KP_3,			/* 56 */ KEY_KP_4,			/* 57 */ KEY_KP_5,			/* 58 */ KEY_KP_6,			/* 59 */ KEY_KP_7,			/* 5a */ KEY_F20,			/* 5b */ KEY_KP_8,			/* 5c */ KEY_KP_9,			/* 5d */ KEY_UNKNOWN,			/* 5e */ KEY_UNKNOWN,			/* 5f */ KEY_UNKNOWN,			/* 60 */ KEY_F5,			/* 61 */ KEY_F6,			/* 62 */ KEY_F7,			/* 63 */ KEY_F3,			/* 64 */ KEY_F8,			/* 65 */ KEY_F9,			/* 66 */ KEY_UNKNOWN,			/* 67 */ KEY_F11,			/* 68 */ KEY_UNKNOWN,			/* 69 */ KEY_F13,			/* 6a */ KEY_F16,			/* 6b */ KEY_F14,			/* 6c */ KEY_UNKNOWN,			/* 6d */ KEY_F10,			/* 6e */ KEY_UNKNOWN,			/* 6f */ KEY_F12,			/* 70 */ KEY_UNKNOWN,			/* 71 */ KEY_F15,			/* 72 */ KEY_INSERT,			/* 73 */ KEY_HOME,			/* 74 */ KEY_PAGEUP,			/* 75 */ KEY_DELETE,			/* 76 */ KEY_F4,			/* 77 */ KEY_END,			/* 78 */ KEY_F2,			/* 79 */ KEY_PAGEDOWN,			/* 7a */ KEY_F1,			/* 7b */ KEY_LEFT,			/* 7c */ KEY_RIGHT,			/* 7d */ KEY_DOWN,			/* 7e */ KEY_UP,			/* 7f */ KEY_UNKNOWN,		};		if(key >= 128)			return KEY_UNKNOWN;		return table[key];	}	static int translateFlags(NSUInteger flags) {		int mods = 0;		if(flags & NSShiftKeyMask)			mods |= KEY_MOD_LSHIFT;		if(flags & NSControlKeyMask)			mods |= KEY_MOD_LCTRL;		if(flags & NSAlternateKeyMask)			mods |= KEY_MOD_LALT;		if(flags & NSCommandKeyMask)			mods |= KEY_MOD_LGUI;		if(flags & NSAlphaShiftKeyMask)			mods |= KEY_MOD_CAPS;		return mods;	}	XdevLCococaInit::XdevLCococaInit(XdevLCoreMediator* core) {		if(xdl_true == cocoaInitialized) {			return;		}		if(NSApp == nil) {			[XdevLCocoaApplication sharedApplication];[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];[NSApp activateIgnoringOtherApps:YES];			[NSApp finishLaunching];			id menubar = [[NSMenu new] autorelease];			id appMenuItem = [[NSMenuItem new] autorelease];[menubar addItem:appMenuItem];[NSApp setMainMenu:menubar];			id appMenu = [[NSMenu new] autorelease];			id appName = [[NSProcessInfo processInfo] processName];id quitTitle = [@"Quit " stringByAppendingString:appName];id quitMenuItem = [[[NSMenuItem alloc] initWithTitle:quitTitle                    action:@selector(terminate:) keyEquivalent:@"q"] autorelease];[appMenu addItem:quitMenuItem];[appMenuItem setSubmenu:appMenu];			applicationDelegate = [[XdevLApplicationDelegate alloc] init];			if(applicationDelegate != nil) {[		NSApp setDelegate:applicationDelegate];			} else {				std::cerr << "Could not create Application delegate" << std::endl;			}		}		XdevLModuleCreateParameter cparameter;		cparameter.setModuleId(XdevLID("XdevLWindowEventServer"));		windowEventServer = std::make_shared<XdevLWindowEventServerCocoa>(&cparameter, windowEventServerCocoaDesc);		if(core) {			core->registerModule(windowEventServer);		}		cparameter.setModuleId(XdevLID("XdevLCursor"));		cursor = std::make_shared<XdevLCursorCocoa>(&cparameter, cursorCocoaDesc);		if(core) {			core->registerModule(cursor);		}		cocoaInitialized = xdl_true;	}	XdevLCococaInit::~XdevLCococaInit() {		if(NSApp != nil) {[NSApp setDelegate:nil];		}	}//////	XdevLWindowCocoa::XdevLWindowCocoa(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor) :		XdevLWindowImpl(XdevLWindowImpl::getWindowsCounter(), parameter, desriptor),		m_window(0) {		XDEVL_MODULE_INFO("XdevLWindowCocoa()\n");		if(reference_counter == 0) {			XdevLCoreMediator* mediator = nullptr;			if(nullptr != parameter) {				mediator = parameter->getMediator();			}			XdevLWindowCocoa::initCocoa(mediator);		}		reference_counter++;	}	XdevLWindowCocoa::~XdevLWindowCocoa() {	XDEVL_MODULE_INFO("~XdevLWindowCocoa()\n");		if(reference_counter == 1) {			XdevLWindowCocoa::shutdownCocoa();		}		reference_counter--;	}	xdl_int XdevLWindowCocoa::initCocoa(XdevLCoreMediator* core) {		if(nullptr != cocoaInit) {			return ERR_OK;		}		cocoaInit = std::make_shared<XdevLCococaInit>(core);		return xdl::ERR_OK;	}	xdl_int XdevLWindowCocoa::shutdownCocoa() {		return ERR_OK;	}	xdl_int XdevLWindowCocoa::init() {		// Do the internal initialization.		if(XdevLWindowImpl::init() == ERR_ERROR) {			return ERR_ERROR;		}		return ERR_OK;	}	xdl_int XdevLWindowCocoa::shutdown() {[m_window orderOut:nil];[m_window setDelegate:nil];		[m_delegate release];		m_delegate = nil;		[m_view release];		m_view = nil;		[m_window close];		m_window = nil;		return ERR_OK;	}	xdl_int XdevLWindowCocoa::create(const XdevLWindowAttribute& attribute) {		XdevLWindowImpl::create(attribute);		return create();	}	xdl_int XdevLWindowCocoa::create() {		NSAutoreleasePool*	pool = [[NSAutoreleasePool alloc] init];		//		// Create the event delegate		//m_delegate = [[XdevLWindowDelegate alloc] init:cocoaInit->getWindowEventServer() :this];		if(m_delegate == nil) {			XDEVL_MODULE_ERROR("Could not create XdevLWindowDelegate\n");			return ERR_ERROR;		}		//		// Set type of the window		//		xdl_uint style = 0;		if(getType() == XDEVL_WINDOW_TYPE_NORMAL) {			style = (NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask|NSResizableWindowMask);		} else {			style = NSBorderlessWindowMask;		}		XdevLWindowPosition position = getPosition();		XdevLWindowSize size = getSize();		NSRect rect;		rect.origin.x = position.x;		rect.origin.y = position.y;		rect.size.width = size.width;		rect.size.height = size.height;		NSArray* screens = [NSScreen screens];		NSScreen *screen = nil;		for(NSScreen* candidate in screens) {			NSRect screenRect = [candidate frame];			if(rect.origin.x >= screenRect.origin.x &&			    rect.origin.x < screenRect.origin.x + screenRect.size.width &&			    rect.origin.y >= screenRect.origin.y &&			    rect.origin.y < screenRect.origin.y + screenRect.size.height) {				screen = candidate;				rect.origin.x -= screenRect.origin.x;				rect.origin.y -= screenRect.origin.y;			}		}		@try {m_window = [[NSWindow alloc] initWithContentRect:rect styleMask:style backing:NSBackingStoreBuffered defer:NO screen:screen];		}		@catch(NSException *e) {			XDEVL_MODULE_ERROR([[e reason] UTF8String] << std::endl);			[pool release];			return ERR_ERROR;		}m_view = [[XdevLContentView alloc] init:cocoaInit->getWindowEventServer() :this];[m_window setTitle:[NSString stringWithUTF8String:getTitle().toString().c_str()]];[m_window setContentView: m_view];[m_window setDelegate:m_delegate];[m_window setAcceptsMouseMovedEvents:YES];[m_window makeKeyAndOrderFront:nil];		[pool release];		return ERR_OK;	}	xdl_int XdevLWindowCocoa::update() {		XDEVL_ASSERT(m_window != 0, "Window is not created.");		for(;;) {NSEvent* event = [NSApp nextEventMatchingMask:NSAnyEventMask                  untilDate:[NSDate distantPast]                  inMode:NSDefaultRunLoopMode                  dequeue:YES];			if(event == nil)				break;[NSApp sendEvent:event];		}		return ERR_ERROR;	}	void* XdevLWindowCocoa::getInternal(const XdevLInternalName& id) {		XDEVL_ASSERT(m_window != 0, "Window is not created.");		if(id == XdevLString("COCOA_WIN")) {			return m_window;		} else if(id == XdevLString("COCOA_VIEW")) {			return m_view;		}		return nullptr;	}	void XdevLWindowCocoa::setPointerPosition(xdl_uint x, xdl_uint y) {		XDEVL_ASSERT(m_window != 0, "Window is not created.");		CGAssociateMouseAndMouseCursorPosition(false);		if(CGWarpMouseCursorPosition(CGPointMake(x, y)) != kCGErrorSuccess) {			XDEVL_MODULE_ERROR("CGWarpMouseCursorPosition failed.\n");		}		CGAssociateMouseAndMouseCursorPosition(true);	}	const XdevLWindowPosition& XdevLWindowCocoa::getPosition() {		return XdevLWindowImpl::getPosition();	}	const XdevLWindowSize& XdevLWindowCocoa::getSize() {		return XdevLWindowImpl::getSize();	}	XdevLWindowPosition::type XdevLWindowCocoa::getX() {		return XdevLWindowImpl::getX();	}	XdevLWindowPosition::type XdevLWindowCocoa::getY() {		return XdevLWindowImpl::getY();	}	XdevLWindowSize::type XdevLWindowCocoa::getWidth() {		return XdevLWindowImpl::getWidth();	}	XdevLWindowSize::type XdevLWindowCocoa::getHeight() {		return XdevLWindowImpl::getHeight();	}	const XdevLWindowTitle& XdevLWindowCocoa::getTitle() {		return XdevLWindowImpl::getTitle();	}	xdl_bool  XdevLWindowCocoa::getFullscreen() {		return XdevLWindowImpl::getFullscreen();	}	void XdevLWindowCocoa::setFullscreen(xdl_bool state) {	}	void XdevLWindowCocoa::setX(XdevLWindowPosition::type x) {		XdevLWindowImpl::setX(x);	}	void XdevLWindowCocoa::setY(XdevLWindowPosition::type y) {		XdevLWindowImpl::setY(y);	}	void XdevLWindowCocoa::setWidth(XdevLWindowSize::type width) {		XdevLWindowImpl::setWidth(width);	}	void XdevLWindowCocoa::setHeight(XdevLWindowSize::type height) {		XDEVL_ASSERT(m_window != 0, "Window is not created.");		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];		setHeight(height);		NSSize size;		size.width = getWidth();		setHeight(height);		size.height = height;[m_window setContentSize:size];		[pool release];	}	void XdevLWindowCocoa::setTitle(const XdevLWindowTitle& title) {		XDEVL_ASSERT(m_window != 0, "Window is not created.");		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];		XdevLWindowImpl::setTitle(title);NSString* string = [[NSString alloc] initWithUTF8String:m_title.toString().c_str()];[m_window setTitle:string];		[string release];		[pool release];	}	xdl_bool XdevLWindowCocoa::isHidden() {		return xdl_false;	}	void XdevLWindowCocoa::raise() {		XDEVL_ASSERT(m_window != 0, "Window is not created.");		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];[m_window orderFront:nil];		[pool release];	}	xdl_bool  XdevLWindowCocoa::getHidePointer() {		XDEVL_ASSERT(m_window != 0, "Window is not created.");		return XdevLWindowImpl::getHidePointer();	}	void XdevLWindowCocoa::setInputFocus() {		XDEVL_ASSERT(m_window != 0, "Window is not created.");		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];		[m_window makeKeyWindow];		[pool release];	}	void XdevLWindowCocoa::show() {		XDEVL_ASSERT(m_window != 0, "Window is not created.");		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];[m_window orderFront:nil];		[pool release];	}	void XdevLWindowCocoa::hide() {		XDEVL_ASSERT(m_window != 0, "Window is not created.");		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];[m_window orderOut:nil];		[pool release];	}	void XdevLWindowCocoa::setSize(const XdevLWindowSize& size) {		XDEVL_ASSERT(m_window != 0, "Window is not created.");		setSize(size);	}	void XdevLWindowCocoa::setPosition(const XdevLWindowPosition& position) {		XDEVL_ASSERT(m_window != 0, "Window is not created.");		setPosition(position);	}	xdl_bool XdevLWindowCocoa::hasFocus() {		XDEVL_ASSERT(m_window != 0, "Window is not created.");		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];		BOOL value;		[[m_window keyWindow] value];		xdl_bool focus = (value == YES) ? xdl_true : xdl_false;		[pool release];		return focus;	}	void XdevLWindowCocoa::setType(XdevLWindowTypes type) {		XDEVL_ASSERT(m_window != 0, "Window is not created.");		setType(type);	}	void XdevLWindowCocoa::setParent(XdevLWindow* window) {		XDEVL_ASSERT(m_window != 0, "Window is not created.");		XdevLWindowImpl::setParent(window);	}	//	// -------------------------------------------------------------------------	//	XdevLWindowServerCocoa::XdevLWindowServerCocoa(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor) :		XdevLWindowServerImpl(parameter, desriptor) {	}	XdevLWindowServerCocoa::~XdevLWindowServerCocoa() {	}	xdl_int XdevLWindowServerCocoa::createWindow(XdevLWindow** window,	    const XdevLWindowTitle& title,	    const XdevLWindowPosition& position,	    const XdevLWindowSize& size,	    const XdevLWindowTypes& type) {//		XdevLWindowSDL* sdlWindow = new XdevLWindowSDL(nullptr);//		sdlWindow->setTitle(title);//		sdlWindow->setPosition(position);//		sdlWindow->setSize(size);//		sdlWindow->setWindowDecoration(xdl_false);//		sdlWindow->create();//		*window = sdlWindow;//		m_windowList[sdlWindow->getWindowID()] = sdlWindow;		return ERR_OK;	}	XdevLWindowEventServerCocoa::XdevLWindowEventServerCocoa(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor) :		XdevLWindowEventServerImpl(parameter, desriptor)	{}	xdl_int XdevLWindowEventServerCocoa::registerWindowForEvents(XdevLWindow* window) {		return XdevLWindowEventServerImpl::registerWindowForEvents(window);	}	xdl_int XdevLWindowEventServerCocoa::unregisterWindowFromEvents(XdevLWindow* window) {		return XdevLWindowEventServerImpl::unregisterWindowFromEvents(window);	}	xdl_int XdevLWindowEventServerCocoa::init() {		XDEVL_MODULE_SUCCESS("Window created successfully" << std::endl);		return ERR_OK;	}	void* XdevLWindowEventServerCocoa::getInternal(const XdevLInternalName& id) {		return nullptr;	}	xdl_int XdevLWindowEventServerCocoa::shutdown() {		XDEVL_MODULE_SUCCESS("Shutdown process was successful.\n");		return ERR_OK;	}	xdl_int XdevLWindowEventServerCocoa::update() {		return ERR_ERROR;	}	void XdevLWindowEventServerCocoa::flush() {	}//// -----------------------------------------------------------------------------//	XdevLCursorCocoa::XdevLCursorCocoa(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor) :		XdevLModuleImpl<XdevLCursor>(parameter, desriptor)  {	}	xdl_int XdevLCursorCocoa::init() {		return ERR_OK;	}	xdl_int XdevLCursorCocoa::shutdown() {		return ERR_OK;	}	void* XdevLCursorCocoa::getInternal(const XdevLInternalName& id) {		return nullptr;	}	xdl_int XdevLCursorCocoa::attach(XdevLWindow* window) {		return ERR_OK;	}	void XdevLCursorCocoa::show() {		[NSCursor unhide];	}	void XdevLCursorCocoa::hide() {		[NSCursor hide];	}	void XdevLCursorCocoa::setPosition(xdl_uint x, xdl_uint y) {		CGPoint point;		point.x = (xdl_float)x;		point.y = (xdl_float)y;		if(CGWarpMouseCursorPosition(point) != kCGErrorSuccess) {			XDEVL_MODULE_ERROR("CGWarpMouseCursorPosition failed.\n");		}	}	xdl_int XdevLCursorCocoa::clip(xdl_uint x, xdl_uint y, xdl_uint width, xdl_uint height) {		return ERR_ERROR;	}	void XdevLCursorCocoa::releaseClip() {	}	xdl_int XdevLCursorCocoa::enableRelativeMotion() {		return ERR_ERROR;	}	void XdevLCursorCocoa::disableRelativeMotion() {	}	xdl_bool XdevLCursorCocoa::isRelativeMotionEnabled() {		return xdl_false;	}} // End of namespace@implementation XdevLWindowDelegate- (id)init:(xdl::XdevLWindowEventServerCocoa *)windowEventServer :(xdl::XdevLWindowCocoa*)window {	self = [super init];	if(self != nil) {		m_windowEventServer = windowEventServer;		m_window = window;	}	return self;}- (BOOL)windowShouldClose:(id)sender {	xdl::XdevLEvent ev;	ev.type					= xdl::XDEVL_WINDOW_EVENT;	ev.window.event 		= xdl::XDEVL_WINDOW_CLOSE;	ev.window.windowid		= m_window->getWindowID();	m_window->getMediator()->fireEvent(ev);	// Make a core event.	ev.type					= xdl::XDEVL_CORE_EVENT;	ev.core.event 			= xdl::XDEVL_CORE_SHUTDOWN;	m_window->getMediator()->fireEvent(ev);	return NO;}- (void)windowDidResize:(NSNotification *)notification {}- (void)windowDidMove:(NSNotification *)notification {}- (void)windowDidMiniaturize:(NSNotification *)notification {}- (void)windowDidDeminiaturize:(NSNotification *)notification {}- (void)windowDidBecomeKey:(NSNotification *)notification {}- (void)windowDidResignKey:(NSNotification *)notification {}@end@implementation XdevLContentView+ (void)initialize {	if(self == [XdevLContentView class]) {	}}- (id)init:(xdl::XdevLWindowEventServerCocoa *)initWindow :(xdl::XdevLWindowCocoa*)window {	self = [super init];	if(self != nil) {		m_window = window;		m_windowEventServer = initWindow;		trackingArea = nil;		m_modifierKeys = 0;		[self updateTrackingAreas];[self registerForDraggedTypes:[NSArray arrayWithObjects:		NSFilenamesPboardType, nil]];	}	return self;}-(void)dealloc {	[trackingArea release];	[super dealloc];}- (BOOL)isOpaque {	return YES;}- (BOOL)canBecomeKeyView {	return YES;}- (BOOL)acceptsFirstResponder {	return YES;}- (void)cursorUpdate:(NSEvent *)event {}- (void)mouseDown:(NSEvent *)event {	xdl::XdevLEvent ev;	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();	ev.type 				= xdl::MouseButtonPressed.getHashCode();	ev.button.button		= xdl::BUTTON_LEFT;	ev.button.x				= 0;	ev.button.y				= 0;	ev.window.windowid		= m_window->getWindowID();	m_windowEventServer->getMediator()->fireEvent(ev);}- (void)mouseDragged:(NSEvent *)event {[self mouseMoved:event];}- (void)mouseUp:(NSEvent *)event {	xdl::XdevLEvent ev;	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();	ev.type 				= xdl::MouseButtonReleased.getHashCode();	ev.button.button		= xdl::BUTTON_LEFT;	ev.button.x				= 0;	ev.button.y				= 0;	ev.window.windowid		= m_window->getWindowID();	m_windowEventServer->getMediator()->fireEvent(ev);}- (void)mouseMoved:(NSEvent *)event {	NSRect windowRect = [[[event window] contentView] frame];	if([event window]) {		if(!NSPointInRect([event locationInWindow], windowRect)) {			return;		}	}	const NSPoint location =  [NSEvent mouseLocation];	const NSPoint p = [event locationInWindow];	xdl::XdevLEvent ev;	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();	ev.type 				= xdl::MouseMotion.getHashCode();	ev.motion.windowid		= m_window->getWindowID();	ev.motion.x					= (2.0 / windowRect.size.width*p.x - 1.0f) * 32768.0f;	ev.motion.y					= (2.0 / windowRect.size.height *(windowRect.size.height - p.y) - 1.0f) * 32768.0f;	m_windowEventServer->getMediator()->fireEvent(ev);}- (void)rightMouseDown:(NSEvent *)event {	xdl::XdevLEvent ev;	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();	ev.type 				= xdl::MouseButtonPressed.getHashCode();	ev.button.button		= xdl::BUTTON_RIGHT;	ev.button.x				= 0;	ev.button.y				= 0;	ev.window.windowid		= m_window->getWindowID();	m_windowEventServer->getMediator()->fireEvent(ev);}- (void)rightMouseDragged:(NSEvent *)event {[self mouseMoved:event];}- (void)rightMouseUp:(NSEvent *)event {	xdl::XdevLEvent ev;	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();	ev.type 				= xdl::MouseButtonReleased.getHashCode();	ev.button.button		= xdl::BUTTON_RIGHT;	ev.button.x				= 0;	ev.button.y				= 0;	ev.window.windowid		= m_window->getWindowID();	m_windowEventServer->getMediator()->fireEvent(ev);}- (void)otherMouseDown:(NSEvent *)event {	xdl::XdevLEvent ev;	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();	ev.type 				= xdl::MouseButtonPressed.getHashCode();	ev.button.button		= xdl::BUTTON_MIDDLE;	ev.button.x				= 0;	ev.button.y				= 0;	ev.window.windowid		= m_window->getWindowID();	m_windowEventServer->getMediator()->fireEvent(ev);}- (void)otherMouseDragged:(NSEvent *)event {[self mouseMoved:event];}- (void)otherMouseUp:(NSEvent *)event {	xdl::XdevLEvent ev;	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();	ev.type 				= xdl::MouseButtonReleased.getHashCode();	ev.button.button		= xdl::BUTTON_MIDDLE;	ev.button.x				= 0;	ev.button.y				= 0;	ev.window.windowid		= m_window->getWindowID();	m_windowEventServer->getMediator()->fireEvent(ev);}- (void)mouseExited:(NSEvent *)event {	xdl::XdevLEvent ev;	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();	ev.type					= xdl::XDEVL_WINDOW_LEAVE;	ev.window.event 		= xdl::XDEVL_WINDOW_ENTER;	ev.window.data1			= 0;	ev.window.data2			= 0;	ev.window.windowid		= m_window->getWindowID();	m_windowEventServer->getMediator()->fireEvent(ev);}- (void)mouseEntered:(NSEvent *)event {	xdl::XdevLEvent ev;	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();	ev.type					= xdl::XDEVL_WINDOW_EVENT;	ev.window.event 		= xdl::XDEVL_WINDOW_ENTER;	ev.window.data1			= 0;	ev.window.data2			= 0;	ev.window.windowid		= m_window->getWindowID();	m_windowEventServer->getMediator()->fireEvent(ev);}- (void)viewDidChangeBackingProperties {}- (void)updateTrackingAreas {	if(trackingArea != nil) {[self removeTrackingArea:trackingArea];		[trackingArea release];	}	NSTrackingAreaOptions options = NSTrackingMouseEnteredAndExited |	NSTrackingActiveInKeyWindow |	NSTrackingCursorUpdate |	NSTrackingInVisibleRect;trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]options:optionsowner:selfuserInfo:nil];[self addTrackingArea:trackingArea];	[super updateTrackingAreas];}- (void)keyDown:(NSEvent *)event {	const int key = xdl::translateKey([event keyCode]);	const int mods = xdl::translateFlags([event modifierFlags]);	xdl::XdevLEvent ev;	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();	ev.type 				= xdl::ButtonPressed.getHashCode();	ev.window.windowid		= m_window->getWindowID();//	ev.key.repeat 			= repeat;							// Is repeat on or off?	ev.key.keycode			= key;	ev.key.mod 				= mods;	m_windowEventServer->getMediator()->fireEvent(ev);}- (void)flagsChanged:(NSEvent *)event {	int action;	unsigned int newModifierFlags = [event modifierFlags] & NSDeviceIndependentModifierFlagsMask;//	if(m_modifierKeys & KEY_MOD_LSHIFT) {//		if(newModifierFlags & KEY_MOD_LSHIFT) {//		}//	}//if (flags & NSShiftKeyMask)//       mods |= KEY_MOD_LSHIFT;//   if (flags & NSControlKeyMask)//       mods |= KEY_MOD_LCTRL;//   if (flags & NSAlternateKeyMask)//       mods |= KEY_MOD_LALT;//   if (flags & NSCommandKeyMask)//       mods |= KEY_MOD_LGUI;//	if (flags & NSAlphaShiftKeyMask)//		mods |= KEY_MOD_CAPS;	m_modifierKeys = xdl::translateFlags([event modifierFlags]);	std::cout << m_modifierKeys << std::endl;}- (void)keyUp:(NSEvent *)event {	const int key = xdl::translateKey([event keyCode]);	const int mods = xdl::translateFlags([event modifierFlags]);	xdl::XdevLEvent ev;	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();	ev.type 				= xdl::ButtonReleased.getHashCode();	ev.window.windowid		= m_window->getWindowID();//	ev.key.repeat 			= repeat;							// Is repeat on or off?	ev.key.keycode			= key;	ev.key.mod 				= mods;	m_windowEventServer->getMediator()->fireEvent(ev);}- (void)scrollWheel:(NSEvent *)event {}- (void)resetCursorRects {}- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender {	return NSDragOperationNone;}- (BOOL)prepareForDragOperation:(id <NSDraggingInfo>)sender {[self setNeedsDisplay:YES];	return YES;}- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender {	return YES;}- (void)concludeDragOperation:(id <NSDraggingInfo>)sender {[self setNeedsDisplay:YES];}@end//////@implementation XdevLApplicationDelegate- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender {	return NSTerminateCancel;}- (void)applicationDidHide:(NSNotification *)notification {	std::cout << "applicationDidHide" << std::endl;}- (void)applicationDidUnhide:(NSNotification *)notification {	std::cout << "applicationDidUnhide" << std::endl;}- (void)applicationDidChangeScreenParameters:(NSNotification *) notification {	std::cout << "applicationDidChangeScreenParameters" << std::endl;}@end//////@implementation XdevLCocoaApplication// From http://cocoadev.com/index.pl?GameKeyboardHandlingAlmost// This works around an AppKit bug, where key up events while holding// down the command key don't get sent to the key window.- (void)sendEvent:(NSEvent *)event {	if([event type] == NSKeyUp && ([event modifierFlags] & NSCommandKeyMask))[[self keyWindow] sendEvent:event];	else[super sendEvent:event];}@end