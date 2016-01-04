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

#ifndef XDEVL_EVENT_H
#define XDEVL_EVENT_H

#include <XdevLTypes.h>
#include <XdevLError.h>

namespace xdl
{
	enum XdevLEventType {
		XDEVL_CORE_EVENT,
		XDEVL_MODULE_EVENT,
		XDEVL_WINDOW_EVENT,
		XDEVL_KEY_PRESSED,
		XDEVL_KEY_RELEASED,
		XDEVL_MOUSE_MOTION,
		XDEVL_MOUSE_BUTTON_PRESSED,
		XDEVL_MOUSE_BUTTON_RELEASED,
		XDEVL_JOYSTICK_REQ_DEVICES_INFO,
		XDEVL_JOYSTICK_RPLY_DEVICES_INFO,
		XDEVL_JOYSTICK_MOTION,
		XDEVL_JOYSTICK_BUTTON_PRESSED,
		XDEVL_JOYSTICK_BUTTON_RELEASED,
		XDEVL_JOYSTICK_POV,
		XDEVL_USER_EVENT
	};
	
	struct XdevLCommonEvent {
		xdl_uint64 type;
		xdl_uint64 timestamp;
		xdl_uint64 sender;
	};
	
	/**
		@struct XdevLUserEvent
		@brief Event structure that can be emmited by the user.
	*/
	struct XdevLUserEvent {
		xdl_uint64 type;
		xdl_uint64 timestamp;
		xdl_uint64 sender;
		
		xdl_uint8	event;
		xdl_uint32 	data1;
		xdl_uint32 	data2;
		xdl_uint64	data3;
		xdl_uint64	data4;
	};
	
	/**
		@struct XdevLCoreEvent
		@brief Event structure emitted by the XdevLCore module.
	*/
	struct XdevLCoreEvent {
		xdl_uint64 type;
		xdl_uint64 timestamp;
		xdl_uint64 sender;
		
		xdl_uint8 event;
		xdl_uint32 data1;
		xdl_uint32 data2;
	};
	
	/**
		@struct XdevLModuleEvent
		@brief Event structure emitted by an XdevL module.
	*/
	struct XdevLModuleEvent {
		xdl_uint64 type;
		xdl_uint64 timestamp;
		xdl_uint64 sender;
		
		xdl_uint8 event;
		xdl_uint32 data1;
		xdl_uint32 data2;
	};

	/**
		@struct XdevLWindowEvent
		@brief Event structure emitted by the XdevLWindow module.
	*/
	struct XdevLWindowEvent {
		xdl_uint64 type;
		xdl_uint64 timestamp;
		xdl_uint64 sender;

		xdl_uint64 windowid;
		xdl_uint32 event;
		xdl_int32  x;
		xdl_int32  y;
		xdl_uint32 width;
		xdl_uint32 height;
		
		xdl_uint32 data1;
		xdl_uint32 data2;
	};
	
	/**
		@struct XdevLMouseMotionEvent
		@brief Event structure which holds mouse motion information.
	*/
	struct XdevLMouseMotionEvent {
		xdl_uint64 type;
		xdl_uint64 timestamp;
		xdl_uint64 sender;
		
		xdl_uint64 windowid;
		xdl_int32 x;
		xdl_int32 y;
		xdl_int32 xrel;
		xdl_int32 yrel;
	};

	/**
		@struct XdevLJoystickButtonEvent
		@brief Event structure which holds joystick button clicks information.
	*/
	struct XdevLJoystickButtonEvent {
		xdl_uint64 type;
		xdl_uint64 timestamp;
		xdl_uint64 sender;

		xdl_uint16 joystickid;
		xdl_uint16 buttonid;
	};

	/**
		@struct XdevLJoystickMotionEvent
		@brief Event structure which holds joystick motion information.
	*/
	struct XdevLJoystickMotionEvent {
		xdl_uint64 type;
		xdl_uint64 timestamp;
		xdl_uint64 sender;

		xdl_uint16 joystickid;
		xdl_uint16 axisid;
		xdl_int32 value;
	};

	/**
		@struct XdevLMouseButtonEvent
		@brief Event structure which holds mouse button clicks information.
	*/
	struct XdevLMouseButtonEvent {
		xdl_uint64 type;
		xdl_uint64 timestamp;
		xdl_uint64 sender;
		
		xdl_uint64 windowid;
		xdl_uint8 button;
		xdl_int32 x;
		xdl_int32 y;
	};
	
	
	/**
		@struct XdevLJoystickDevicesInfo
		@brief Max. 4 supported
	*/
	struct XdevLJoystickDevicesInfo {
		xdl_uint64 type;
		xdl_uint64 timestamp;
		xdl_uint64 sender;
		
		xdl_uint8 command;
		xdl_uint16 number_devices;
		xdl_uint16 number_buttons[4];
		xdl_uint16 number_axis[4];
	};
	
	/**
		@struct XdevLJoystickPOVEvent
		@brief Event structure which holds Joystick POV clicks information.
	*/
	struct XdevLJoystickPOVEvent {
		xdl_uint64 type;
		xdl_uint64 timestamp;
		xdl_uint64 sender;
		
		xdl_uint8 direction;
	};	

	struct XdevLKeyEvent {
		xdl_uint64 type;
		xdl_uint64 timestamp;
		xdl_uint64 sender;
		
		xdl_uint64 windowid;
		xdl_uint32 keycode;
		xdl_uint16 mod;
		xdl_uint8 repeat;
		xdl_uint8 device_type;
	};
	
	/**
		@struct XdevLEvent
		@brief The main structure of a XdevL event.
		
		Version 1
	*/
	union XdevLEvent {
		xdl_uint64 type;
		XdevLCommonEvent common;
		XdevLUserEvent user;
		XdevLCoreEvent core;
		XdevLModuleEvent module;
		XdevLWindowEvent window;
		XdevLKeyEvent key;
		XdevLMouseMotionEvent motion;
		XdevLMouseButtonEvent button;
		XdevLJoystickDevicesInfo jdeviceinfo;
		XdevLJoystickButtonEvent jbutton;
		XdevLJoystickMotionEvent jaxis;
		XdevLJoystickPOVEvent jpov;

		xdl_uint8	data[56];
	};

}

#endif
