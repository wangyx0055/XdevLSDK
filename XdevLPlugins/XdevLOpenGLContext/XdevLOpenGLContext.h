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

#ifndef XDEVL_OPENGL_CONTEXT_H
#define XDEVL_OPENGL_CONTEXT_H

#include <XdevLModule.h>

namespace xdl {

	class XdevLWindow;

	enum XdevLOpenGLContextProfiles {
	    XDEVL_OPENGL_CONTEXT_CORE_PROFILE,
	    XDEVL_OPENGL_CONTEXT_COMPATIBILITY,
	    XDEVL_OPENGL_CONTEXT_ES
	};

	enum XdevLOpenGLContextFlags {
	    XDEVL_OPENGL_CONTEXT_FLAGS_NONE						= 0,
	    XDEVL_OPENGL_CONTEXT_FLAGS_DEBUG_BIT				= 0x00000001,
	    XDEVL_OPENGL_CONTEXT_FLAGS_FORWARD_COMPATIBLE_BIT	= 0x00000002
	};

	class XdevLOpenGLContextAttributes {
		public:
			XdevLOpenGLContextAttributes() :
				red_size(8),
				green_size(8),
				blue_size(8),
				alpha_size(8),
				color_buffer_size(32),
				double_buffer(1),
				depth_size(24),
				stencil_size(8),
				accum_red_size(0),
				accum_green_size(0),
				accum_blue_size(0),
				accum_alpha_size(0),
				stereo(0),
				multisample_buffers(0),
				multisample_samples(0),
				context_major_version(3),
				context_minor_version(2),
				context_profile_mask(XDEVL_OPENGL_CONTEXT_CORE_PROFILE),
				context_flags(XDEVL_OPENGL_CONTEXT_FLAGS_NONE) {

			}

			xdl_uint	red_size;
			xdl_uint	green_size;
			xdl_uint	blue_size;
			xdl_uint	alpha_size;
			xdl_uint	color_buffer_size;
			xdl_uint	double_buffer;
			xdl_uint	depth_size;
			xdl_uint	stencil_size;
			xdl_uint	accum_red_size;
			xdl_uint	accum_green_size;
			xdl_uint	accum_blue_size;
			xdl_uint	accum_alpha_size;
			xdl_uint	stereo;
			xdl_uint	multisample_buffers;
			xdl_uint	multisample_samples;
			xdl_uint	context_major_version;
			xdl_uint	context_minor_version;
			xdl_uint	context_profile_mask;
			xdl_uint	context_flags;
	};

	/**
		@class XdevLOpenGLContext
		@brief Support for handling OpenGL context for the GLX platform.
		@author Cengiz Terzibas
	*/
	class XdevLOpenGLContext : public XdevLModule {

		public:
			virtual ~XdevLOpenGLContext() {};

			/// Creates the context.
			/**
				@return Returns
					@b ERR_OK on success.
					@b ERR_ERROR else.
			*/
			virtual xdl_int create(XdevLWindow* window) = 0;

			/// Gets the current attributes of the OpenGL context.
			virtual xdl_int getAttributes(XdevLOpenGLContextAttributes& attributes) = 0;

			/// Sets the attributes for the OpenGL context.
			/**
				Set the attributes you like to set before you call the create() method.

				@return Returns
					@b ERR_OK on success.
					@b ERR_ERROR else.
			*/
			virtual xdl_int setAttributes(const XdevLOpenGLContextAttributes& attributes) = 0;

			/// Make this context current to the specified window.
			virtual xdl_int makeCurrent(XdevLWindow* window) = 0;

			/// Swap the buffer.
			virtual xdl_int swapBuffers() = 0;

			/// Returns the function address of the specified string representation of the OpenGL function.
			virtual void* getProcAddress(const xdl_char* func) = 0;

			/// Enable/Disable Vertical Sync.
			virtual xdl_int setVSync(xdl_bool enableVSync) = 0;
	};


	typedef XdevLOpenGLContext	IXdevLOpenGLContext;
	typedef XdevLOpenGLContext*	IPXdevLOpenGLContext;
}


#endif
