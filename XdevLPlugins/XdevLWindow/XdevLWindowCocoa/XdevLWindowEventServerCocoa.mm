// Import stuff for the cocoa window.
#import <Cocoa/Cocoa.h>

// Import stuff for the cursor.
#import <IOKit/hid/IOHIDKeys.h>
#import <IOKit/hidsystem/IOHIDShared.h>

#include <XdevLInputSystem.h>

#include "XdevLWindowCocoa.h"
#include "XdevLWindowEventServerCocoa.h"

namespace xdl {

	const XdevLID ButtonPressed("XDEVL_BUTTON_PRESSED");
	const XdevLID ButtonReleased("XDEVL_BUTTON_RELEASED");
	const XdevLID MouseButtonPressed("XDEVL_MOUSE_BUTTON_PRESSED");
	const XdevLID MouseButtonReleased("XDEVL_MOUSE_BUTTON_RELEASED");
	const XdevLID MouseMotion("XDEVL_MOUSE_MOTION");
	const XdevLID MouseMotionRelative("XDEVL_MOUSE_MOTION_RELATIVE");
	const XdevLID WindowEvent("XDEVL_WINDOW_EVENT");

	static int translateKey(unsigned int key) {
		// Keyboard symbol translation table
		static const unsigned int table[128] = {
			/* 00 */ KEY_A,
			/* 01 */ KEY_S,
			/* 02 */ KEY_D,
			/* 03 */ KEY_F,
			/* 04 */ KEY_H,
			/* 05 */ KEY_G,
			/* 06 */ KEY_Z,
			/* 07 */ KEY_X,
			/* 08 */ KEY_C,
			/* 09 */ KEY_V,
			/* 0a */ KEY_UNKNOWN,
			/* 0b */ KEY_B,
			/* 0c */ KEY_Q,
			/* 0d */ KEY_W,
			/* 0e */ KEY_E,
			/* 0f */ KEY_R,
			/* 10 */ KEY_Y,
			/* 11 */ KEY_T,
			/* 12 */ KEY_1,
			/* 13 */ KEY_2,
			/* 14 */ KEY_3,
			/* 15 */ KEY_4,
			/* 16 */ KEY_6,
			/* 17 */ KEY_5,
			/* 18 */ KEY_EQUALS,
			/* 19 */ KEY_9,
			/* 1a */ KEY_7,
			/* 1b */ KEY_MINUS,
			/* 1c */ KEY_8,
			/* 1d */ KEY_0,
			/* 1e */ KEY_RIGHTBRACKET,
			/* 1f */ KEY_O,
			/* 20 */ KEY_U,
			/* 21 */ KEY_LEFTBRACKET,
			/* 22 */ KEY_I,
			/* 23 */ KEY_P,
			/* 24 */ KEY_ENTER,
			/* 25 */ KEY_L,
			/* 26 */ KEY_J,
			/* 27 */ KEY_APOSTROPHE,
			/* 28 */ KEY_K,
			/* 29 */ KEY_SEMICOLON,
			/* 2a */ KEY_BACKSLASH,
			/* 2b */ KEY_COMMA,
			/* 2c */ KEY_SLASH,
			/* 2d */ KEY_N,
			/* 2e */ KEY_M,
			/* 2f */ KEY_PERIOD,
			/* 30 */ KEY_TAB,
			/* 31 */ KEY_SPACE,
			/* 32 */ KEY_GRAVE,
			/* 33 */ KEY_BACKSPACE,
			/* 34 */ KEY_UNKNOWN,
			/* 35 */ KEY_ESCAPE,
			/* 36 */ KEY_RGUI,
			/* 37 */ KEY_LGUI,
			/* 38 */ KEY_LSHIFT,
			/* 39 */ KEY_CAPSLOCK,
			/* 3a */ KEY_LALT,
			/* 3b */ KEY_LCTRL,
			/* 3c */ KEY_RSHIFT,
			/* 3d */ KEY_RALT,
			/* 3e */ KEY_RCTRL,
			/* 3f */ KEY_UNKNOWN, /* Function */
			/* 40 */ KEY_F17,
			/* 41 */ KEY_KP_PERIOD,
			/* 42 */ KEY_UNKNOWN,
			/* 43 */ KEY_KP_MULTIPLY,
			/* 44 */ KEY_UNKNOWN,
			/* 45 */ KEY_KP_PLUS,
			/* 46 */ KEY_UNKNOWN,
			/* 47 */ KEY_NUMLOCK, /* Really KeypadClear... */
			/* 48 */ KEY_UNKNOWN, /* VolumeUp */
			/* 49 */ KEY_UNKNOWN, /* VolumeDown */
			/* 4a */ KEY_UNKNOWN, /* Mute */
			/* 4b */ KEY_KP_DIVIDE,
			/* 4c */ KEY_KP_ENTER,
			/* 4d */ KEY_UNKNOWN,
			/* 4e */ KEY_KP_MINUS,
			/* 4f */ KEY_F18,
			/* 50 */ KEY_F19,
			/* 51 */ KEY_KP_ENTER,
			/* 52 */ KEY_KP_0,
			/* 53 */ KEY_KP_1,
			/* 54 */ KEY_KP_2,
			/* 55 */ KEY_KP_3,
			/* 56 */ KEY_KP_4,
			/* 57 */ KEY_KP_5,
			/* 58 */ KEY_KP_6,
			/* 59 */ KEY_KP_7,
			/* 5a */ KEY_F20,
			/* 5b */ KEY_KP_8,
			/* 5c */ KEY_KP_9,
			/* 5d */ KEY_UNKNOWN,
			/* 5e */ KEY_UNKNOWN,
			/* 5f */ KEY_UNKNOWN,
			/* 60 */ KEY_F5,
			/* 61 */ KEY_F6,
			/* 62 */ KEY_F7,
			/* 63 */ KEY_F3,
			/* 64 */ KEY_F8,
			/* 65 */ KEY_F9,
			/* 66 */ KEY_UNKNOWN,
			/* 67 */ KEY_F11,
			/* 68 */ KEY_UNKNOWN,
			/* 69 */ KEY_F13,
			/* 6a */ KEY_F16,
			/* 6b */ KEY_F14,
			/* 6c */ KEY_UNKNOWN,
			/* 6d */ KEY_F10,
			/* 6e */ KEY_UNKNOWN,
			/* 6f */ KEY_F12,
			/* 70 */ KEY_UNKNOWN,
			/* 71 */ KEY_F15,
			/* 72 */ KEY_INSERT,
			/* 73 */ KEY_HOME,
			/* 74 */ KEY_PAGEUP,
			/* 75 */ KEY_DELETE,
			/* 76 */ KEY_F4,
			/* 77 */ KEY_END,
			/* 78 */ KEY_F2,
			/* 79 */ KEY_PAGEDOWN,
			/* 7a */ KEY_F1,
			/* 7b */ KEY_LEFT,
			/* 7c */ KEY_RIGHT,
			/* 7d */ KEY_DOWN,
			/* 7e */ KEY_UP,
			/* 7f */ KEY_UNKNOWN,
		};

		if(key >= 128)
			return KEY_UNKNOWN;

		return table[key];
	}


	static int translateFlags(NSUInteger flags) {
		int mods = 0;

		if(flags & NSShiftKeyMask)
			mods |= KEY_MOD_LSHIFT;
		if(flags & NSControlKeyMask)
			mods |= KEY_MOD_LCTRL;
		if(flags & NSAlternateKeyMask)
			mods |= KEY_MOD_LALT;
		if(flags & NSCommandKeyMask)
			mods |= KEY_MOD_LGUI;
		if(flags & NSAlphaShiftKeyMask)
			mods |= KEY_MOD_CAPS;

		return mods;
	}



	XdevLWindowEventServerCocoa::XdevLWindowEventServerCocoa(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor) :
		XdevLWindowEventServerImpl(parameter, desriptor)
	{}


	xdl_int XdevLWindowEventServerCocoa::registerWindowForEvents(XdevLWindow* window) {
		return XdevLWindowEventServerImpl::registerWindowForEvents(window);
	}

	xdl_int XdevLWindowEventServerCocoa::unregisterWindowFromEvents(XdevLWindow* window) {
		return XdevLWindowEventServerImpl::unregisterWindowFromEvents(window);
	}

	xdl_int XdevLWindowEventServerCocoa::init() {
		XDEVL_MODULE_SUCCESS("Window created successfully" << std::endl);
		return ERR_OK;
	}

	void* XdevLWindowEventServerCocoa::getInternal(const XdevLInternalName& id) {

		return nullptr;
	}

	xdl_int XdevLWindowEventServerCocoa::shutdown() {
		XDEVL_MODULE_SUCCESS("Shutdown process was successful.\n");
		return ERR_OK;
	}

	xdl_int XdevLWindowEventServerCocoa::update() {
		return ERR_ERROR;
	}


	void XdevLWindowEventServerCocoa::flush() {

	}

}


@implementation XdevLContentView

+ (void)initialize {
	if(self == [XdevLContentView class]) {

	}
}

- (id)init:
(xdl::XdevLWindowEventServerCocoa *)initWindow :
(xdl::XdevLWindowCocoa*)window {
	self = [super init];
	if(self != nil) {
		m_window = window;
		m_windowEventServer = initWindow;
		trackingArea = nil;
		m_modifierKeys = 0;

		[self updateTrackingAreas];
[self registerForDraggedTypes:[NSArray arrayWithObjects:
		NSFilenamesPboardType, nil]];
	}

	return self;
}

-(void)dealloc {
	[trackingArea release];
	[super dealloc];
}

- (BOOL)isOpaque {
	return YES;
}

- (BOOL)canBecomeKeyView {
	return YES;
}

- (BOOL)acceptsFirstResponder {
	return YES;
}

- (void)cursorUpdate:
(NSEvent *)event {

}

- (void)mouseDown:
(NSEvent *)event {
	xdl::XdevLEvent ev;
	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();
	ev.type 				= xdl::MouseButtonPressed.getHashCode();
	ev.button.button		= xdl::BUTTON_LEFT;
	ev.button.x				= 0;
	ev.button.y				= 0;
	ev.window.windowid		= m_window->getWindowID();

	m_windowEventServer->getMediator()->fireEvent(ev);
}

- (void)mouseDragged:
(NSEvent *)event {
[self mouseMoved:event];
}

- (void)mouseUp:
(NSEvent *)event {
	xdl::XdevLEvent ev;
	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();
	ev.type 				= xdl::MouseButtonReleased.getHashCode();
	ev.button.button		= xdl::BUTTON_LEFT;
	ev.button.x				= 0;
	ev.button.y				= 0;
	ev.window.windowid		= m_window->getWindowID();

	m_windowEventServer->getMediator()->fireEvent(ev);
}

- (void)mouseMoved: (NSEvent *)event {

	NSRect windowRect = [m_window->getNativeWindow() frame];
	if([event window]) {
		if(!NSPointInRect([event locationInWindow], windowRect)) {
			return;
		}
	}

//	const NSPoint location =  [NSEvent mouseLocation];
	const NSPoint p = [event locationInWindow];

	xdl::XdevLEvent ev;
	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();
	ev.type 							= xdl::MouseMotion.getHashCode();
	ev.motion.windowid		= m_window->getWindowID();
	ev.motion.x						= ((2.0 / (float)windowRect.size.width) * p.x - 1.0f) * 32768.0f;
	ev.motion.y						= ((2.0 / (float)windowRect.size.height) * p.y - 1.0f) * 32768.0f;
	ev.motion.xrel				= [event deltaX];
	ev.motion.yrel				= [event deltaY];

	m_windowEventServer->getMediator()->fireEvent(ev);


	ev.type 							= xdl::MouseMotionRelative.getHashCode();
	ev.motion.timestamp	= m_windowEventServer->getMediator()->getTimer().getTime64();
	ev.window.windowid		= m_window->getWindowID();
	ev.motion.x						= ev.motion.x;
	ev.motion.y						= ev.motion.y	;
	ev.motion.xrel				= ev.motion.xrel;
	ev.motion.yrel				= ev.motion.yrel;

	m_windowEventServer->getMediator()->fireEvent(ev);
}

- (void)rightMouseDown:
(NSEvent *)event {
	xdl::XdevLEvent ev;
	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();
	ev.type 				= xdl::MouseButtonPressed.getHashCode();
	ev.button.button		= xdl::BUTTON_RIGHT;
	ev.button.x				= 0;
	ev.button.y				= 0;
	ev.window.windowid		= m_window->getWindowID();

	m_windowEventServer->getMediator()->fireEvent(ev);
}

- (void)rightMouseDragged:
(NSEvent *)event {
[self mouseMoved:event];
}

- (void)rightMouseUp:
(NSEvent *)event {
	xdl::XdevLEvent ev;
	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();
	ev.type 				= xdl::MouseButtonReleased.getHashCode();
	ev.button.button		= xdl::BUTTON_RIGHT;
	ev.button.x				= 0;
	ev.button.y				= 0;
	ev.window.windowid		= m_window->getWindowID();

	m_windowEventServer->getMediator()->fireEvent(ev);
}

- (void)otherMouseDown:
(NSEvent *)event {
	xdl::XdevLEvent ev;
	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();
	ev.type 				= xdl::MouseButtonPressed.getHashCode();
	ev.button.button		= xdl::BUTTON_MIDDLE;
	ev.button.x				= 0;
	ev.button.y				= 0;
	ev.window.windowid		= m_window->getWindowID();

	m_windowEventServer->getMediator()->fireEvent(ev);
}

- (void)otherMouseDragged:
(NSEvent *)event {
[self mouseMoved:event];
}

- (void)otherMouseUp:
(NSEvent *)event {
	xdl::XdevLEvent ev;
	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();
	ev.type 				= xdl::MouseButtonReleased.getHashCode();
	ev.button.button		= xdl::BUTTON_MIDDLE;
	ev.button.x				= 0;
	ev.button.y				= 0;
	ev.window.windowid		= m_window->getWindowID();

	m_windowEventServer->getMediator()->fireEvent(ev);
}

- (void)mouseExited:
(NSEvent *)event {
	xdl::XdevLEvent ev;
	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();
	ev.type					= xdl::XDEVL_WINDOW_LEAVE;
	ev.window.event 		= xdl::XDEVL_WINDOW_ENTER;
	ev.window.data1			= 0;
	ev.window.data2			= 0;
	ev.window.windowid		= m_window->getWindowID();

	m_windowEventServer->getMediator()->fireEvent(ev);
}

- (void)mouseEntered:
(NSEvent *)event {
	xdl::XdevLEvent ev;
	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();
	ev.type					= xdl::XDEVL_WINDOW_EVENT;
	ev.window.event 		= xdl::XDEVL_WINDOW_ENTER;
	ev.window.data1			= 0;
	ev.window.data2			= 0;
	ev.window.windowid		= m_window->getWindowID();

	m_windowEventServer->getMediator()->fireEvent(ev);
}

- (void)viewDidChangeBackingProperties {

}

- (void)updateTrackingAreas {
	if(trackingArea != nil) {
[self removeTrackingArea:trackingArea];
		[trackingArea release];
	}

	NSTrackingAreaOptions options = NSTrackingMouseEnteredAndExited |
	NSTrackingActiveInKeyWindow |
	NSTrackingCursorUpdate |
	NSTrackingInVisibleRect;

trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
options:options
owner:self
userInfo:nil];

[self addTrackingArea:trackingArea];
	[super updateTrackingAreas];
}

- (void)keyDown:
(NSEvent *)event {
	const int key = xdl::translateKey([event keyCode]);
	const int mods = xdl::translateFlags([event modifierFlags]);

	xdl::XdevLEvent ev;
	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();
	ev.type 				= xdl::ButtonPressed.getHashCode();
	ev.window.windowid		= m_window->getWindowID();
//	ev.key.repeat 			= repeat;							// Is repeat on or off?
	ev.key.keycode			= key;
	ev.key.mod 				= mods;

	m_windowEventServer->getMediator()->fireEvent(ev);
}

- (void)flagsChanged: (NSEvent *)event {
	int action;
//	unsigned int newModifierFlags = [event modifierFlags] & NSDeviceIndependentModifierFlagsMask;
//
//	if(m_modifierKeys & KEY_MOD_LSHIFT) {
//		if(newModifierFlags & KEY_MOD_LSHIFT) {
//		}
//	}

//if (flags & NSShiftKeyMask)
//       mods |= KEY_MOD_LSHIFT;
//   if (flags & NSControlKeyMask)
//       mods |= KEY_MOD_LCTRL;
//   if (flags & NSAlternateKeyMask)
//       mods |= KEY_MOD_LALT;
//   if (flags & NSCommandKeyMask)
//       mods |= KEY_MOD_LGUI;
//	if (flags & NSAlphaShiftKeyMask)
//		mods |= KEY_MOD_CAPS;

	m_modifierKeys = xdl::translateFlags([event modifierFlags]);



	std::cout << m_modifierKeys << std::endl;

}

- (void)keyUp:
(NSEvent *)event {
	const int key = xdl::translateKey([event keyCode]);
	const int mods = xdl::translateFlags([event modifierFlags]);

	xdl::XdevLEvent ev;
	ev.common.timestamp 	= m_windowEventServer->getMediator()->getTimer().getTime64();
	ev.type 				= xdl::ButtonReleased.getHashCode();
	ev.window.windowid		= m_window->getWindowID();
//	ev.key.repeat 			= repeat;							// Is repeat on or off?
	ev.key.keycode			= key;
	ev.key.mod 				= mods;

	m_windowEventServer->getMediator()->fireEvent(ev);
}

- (void)scrollWheel:
(NSEvent *)event {

}

- (void)resetCursorRects {

}

- (NSDragOperation)draggingEntered:
(id <NSDraggingInfo>)sender {
	return NSDragOperationNone;
}

- (BOOL)prepareForDragOperation:
(id <NSDraggingInfo>)sender {
[self setNeedsDisplay:YES];
	return YES;
}

- (BOOL)performDragOperation:
(id <NSDraggingInfo>)sender {

	return YES;
}

- (void)concludeDragOperation:
(id <NSDraggingInfo>)sender {
[self setNeedsDisplay:YES];
}

@end
