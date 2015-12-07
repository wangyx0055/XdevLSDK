/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2015 Cengiz Terzibas

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
		@struct XdevLJoystickMotionEvent
		@brief Event structure which holds joystick motion information.
	*/
	struct XdevLJoystickMotionEvent {
		xdl_uint64 type;
		xdl_uint64 timestamp;
		xdl_uint64 sender;
		
		xdl_uint8  axis;
		xdl_uint32 value;
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
		xdl_uint32 x;
		xdl_uint32 y;
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
		XdevLCommonEvent			common;
		XdevLUserEvent				user;
		XdevLCoreEvent				core;
		XdevLModuleEvent			module;
		XdevLWindowEvent 			window;
		XdevLKeyEvent				key;
		XdevLMouseMotionEvent 		motion;
		XdevLMouseButtonEvent 		button;
		XdevLJoystickDevicesInfo	jdeviceinfo;
		XdevLJoystickMotionEvent 	jaxis;
		XdevLJoystickPOVEvent		jpov;

		xdl_uint8	data[56];
	};

}

#endif
