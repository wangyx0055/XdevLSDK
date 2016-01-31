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

#ifndef XDEVL_OPENGL_CONTEXT_H
#define XDEVL_OPENGL_CONTEXT_H

#include <XdevLModule.h>

namespace xdl {

	class XdevLWindow;

	enum XdevLOpenGLContextProfiles {
	  XDEVL_OPENGL_CONTEXT_CORE_PROFILE,
	  XDEVL_OPENGL_CONTEXT_COMPATIBILITY,
	  XDEVL_OPENGL_CONTEXT_ES1,
	  XDEVL_OPENGL_CONTEXT_ES2
	};

	enum XdevLOpenGLContextFlags {
	  XDEVL_OPENGL_CONTEXT_FLAGS_NONE						= 0,
	  XDEVL_OPENGL_CONTEXT_FLAGS_DEBUG_BIT				= 0x00000001,
	  XDEVL_OPENGL_CONTEXT_FLAGS_FORWARD_COMPATIBLE_BIT	= 0x00000002
	};

	class XdevLOpenGLContextAttributes {
		public:
			XdevLOpenGLContextAttributes() :
				red_size(1),
				green_size(1),
				blue_size(1),
				alpha_size(1),
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

			xdl_int	red_size;
			xdl_int	green_size;
			xdl_int	blue_size;
			xdl_int	alpha_size;
			xdl_int	color_buffer_size;
			xdl_int	double_buffer;
			xdl_int	depth_size;
			xdl_int	stencil_size;
			xdl_int	accum_red_size;
			xdl_int	accum_green_size;
			xdl_int	accum_blue_size;
			xdl_int	accum_alpha_size;
			xdl_int	stereo;
			xdl_int	multisample_buffers;
			xdl_int	multisample_samples;
			xdl_int	context_major_version;
			xdl_int	context_minor_version;
			xdl_int	context_profile_mask;
			xdl_int	context_flags;
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

	XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DECLARATION(XdevLOpenGLContext)
}


#endif
