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

#ifndef XDEVL_FRAMEBUFFER_OBJECT_H
#define XDEVL_FRAMEBUFFER_OBJECT_H

#include <XdevLRAI/XdevLTexture.h>
#include <XdevLRAI/XdevLTextureCube.h>

namespace xdl {

	class XdevLTexture;
	
	enum XdevLFrameBufferRenderTarget {
		XDEVL_COLOR_TARGET,
		XDEVL_DEPTH_TARGET,
		XDEVL_STENCIL_TARGET
	};

	enum XdevLFrameBufferColorTargets {
	    XDEVL_COLOR_NONE = 0,
	    XDEVL_COLOR_TARGET0 = 0x8CE0,
	    XDEVL_COLOR_TARGET1 = 0x8CE1,
	    XDEVL_COLOR_TARGET2 = 0x8CE2,
	    XDEVL_COLOR_TARGET3 = 0x8CE3,
	    XDEVL_COLOR_TARGET4 = 0x8CE4,
	    XDEVL_COLOR_TARGET5 = 0x8CE5,
	    XDEVL_COLOR_TARGET6 = 0x8CE6,
	    XDEVL_COLOR_TARGET7 = 0x8CE7,
	    XDEVL_COLOR_TARGET8 = 0x8CE8,
	    XDEVL_COLOR_TARGET9 = 0x8CE9,
	    XDEVL_COLOR_TARGET10 = 0x8CEA,
	    XDEVL_COLOR_TARGET11 = 0x8CEB,
	    XDEVL_COLOR_TARGET12 = 0x8CEC,
	    XDEVL_COLOR_TARGET13 = 0x8CED,
	    XDEVL_COLOR_TARGET14 = 0x8CEE,
	    XDEVL_COLOR_TARGET15 = 0x8CEF
	};

	enum XdevLFrameBufferColorFormat {

	    XDEVL_FB_COLOR_RED 					= 0x1903,
	    XDEVL_FB_COLOR_GREEN				= 0x1904,
	    XDEVL_FB_COLOR_BLUE 				= 0x1905,
	    XDEVL_FB_COLOR_ALPHA 				= 0x1906,
	    XDEVL_FB_COLOR_RGB					= 0x1907,
	    XDEVL_FB_COLOR_RGBA					= 0x1908,
	    XDEVL_FB_COLOR_BGR 					= 0x80E0,
	    XDEVL_FB_COLOR_BGRA 				= 0x80E1,

	    XDEVL_FB_COLOR_R8					= 0x8229,
	    XDEVL_FB_COLOR_R16					= 0x822A,
	    XDEVL_FB_COLOR_RG8					= 0x822B,
	    XDEVL_FB_COLOR_RG16					= 0x822C,
	    XDEVL_FB_COLOR_R16F					= 0x822D,
	    XDEVL_FB_COLOR_R32F					= 0x822E,
	    XDEVL_FB_COLOR_RG16F				= 0x822F,
	    XDEVL_FB_COLOR_RG32F				= 0x8230,
	    XDEVL_FB_COLOR_R8I					= 0x8231,
	    XDEVL_FB_COLOR_R8UI					= 0x8232,
	    XDEVL_FB_COLOR_R16I					= 0x8233,
	    XDEVL_FB_COLOR_R16UI				= 0x8234,
	    XDEVL_FB_COLOR_R32I					= 0x8235,
	    XDEVL_FB_COLOR_R32UI				= 0x8236,
	    XDEVL_FB_COLOR_RG8I					= 0x8237,
	    XDEVL_FB_COLOR_RG8UI				= 0x8238,
	    XDEVL_FB_COLOR_RG16I				= 0x8239,
	    XDEVL_FB_COLOR_RG16UI				= 0x823A,
	    XDEVL_FB_COLOR_RG32I				= 0x823B,
	    XDEVL_FB_COLOR_RG32UI				= 0x823C,
	    XDEVL_FB_COLOR_RGBA32F 				= 0x8814,
	    XDEVL_FB_COLOR_RGB32F				= 0x8815,
	    XDEVL_FB_COLOR_RGBA16F				= 0x881A,
	    XDEVL_FB_COLOR_RGB16F				= 0x881B,

	    XDEVL_FB_COLOR_R11F_G11F_B10F		= 0x8C3A,
	    XDEVL_FB_COLOR_RGB9_E5				= 0x8C3D,
	    XDEVL_FB_COLOR_SRGB					= 0x8C40,
	    XDEVL_FB_COLOR_SRGB8				= 0x8C41,
	    XDEVL_FB_COLOR_SRGB_ALPHA			= 0x8C42,
	    XDEVL_FB_COLOR_SRGB8_ALPHA8			= 0x8C43,


	    XDEVL_FB_COLOR_R8_SNORM				= 0x8F94,
	    XDEVL_FB_COLOR_RG8_SNORM			= 0x8F95,
	    XDEVL_FB_COLOR_RGB8_SNORM			= 0x8F96,
	    XDEVL_FB_COLOR_RGBA8_SNORM			= 0x8F97,
	    XDEVL_FB_COLOR_R16_SNORM			= 0x8F98,
	    XDEVL_FB_COLOR_RG16_SNORM			= 0x8F99,
	    XDEVL_FB_COLOR_RGB16_SNORM			= 0x8F9A,
	    XDEVL_FB_COLOR_RGBA16_SNORM			= 0x8F9B
	};

	enum XdevLFrameBufferDepthFormat {

	    XDEVL_FB_DEPTH_COMPONENT		= 0x1902,
	    XDEVL_FB_DEPTH_COMPONENT16		= 0x81A5,
	    XDEVL_FB_DEPTH_COMPONENT24		= 0x81A6,
	    XDEVL_FB_DEPTH_COMPONENT32		= 0x81A7,
	    XDEVL_FB_DEPTH_COMPONENT32F 	= 0x8CAC

	};

	enum XdevLFrameBufferDepthStencilFormat {

	    XDEVL_FB_DEPTH_STENCIL			= 0x84F9,
	    XDEVL_FB_DEPTH24_STENCIL8		= 0x88F0,
	    XDEVL_FB_DEPTH32F_STENCIL8		= 0x8CAD

	};

	/**
		@class XdevLFrameBuffer
		@brief Allows user-defined Framebuffers.
		@author Cengiz Terzibas
	 */
	class XdevLFrameBuffer {
		public:
			virtual ~XdevLFrameBuffer() {}

			/// Initialize the framebuffer.
			/**
				@param width The width of the framebuffer
				@param height The height of the framebuffer
				@param number_of_color_buffers The number of color buffers.
				@param depth_buffer Use of a depth buffer.
					- 'xdl_true' for yes use a depth buffer.
					- 'xdl_false' do not use a depth buffer.
				@param stencil_buffer Use of a stencil buffer.
					- 'xdl_true' for yes use a stencil buffer.
					- 'xdl_false' do not use a stencil buffer.
			*/
			virtual xdl_int init(xdl_uint width, xdl_uint height) = 0;

			/// Add a color target to the framebuffer.
			virtual xdl_int addColorTarget(xdl_uint target_index, XdevLFrameBufferColorFormat internal_format) = 0;
			virtual xdl_int addColorTarget(xdl_uint target_index, IPXdevLTexture texture) = 0;
			virtual xdl_int addColorTarget(xdl_uint target_index, IPXdevLTextureCube textureCube) = 0;

			/// Add a depth target to the framebuffer.
			virtual xdl_int addDepthTarget(XdevLFrameBufferDepthFormat internal_format) = 0;
			virtual xdl_int addDepthTarget(IPXdevLTexture texture) = 0;

			/// Add a depth stencil target to the framebuffer.
			virtual xdl_int addDepthStencilTarget(XdevLFrameBufferDepthStencilFormat internal_format) = 0;

			/// Activate the frame buffer.
			virtual xdl_int activate() = 0;

			/// Deactivate the frame buffer.
			virtual xdl_int deactivate() = 0;

			/// Activates and/or deactivates a list of Color targets.
			virtual xdl_int activateColorTargets(xdl_uint numberOfTargets, xdl_uint* targetList) = 0;

			/// Activates a position of a cube map.
			virtual xdl_int activateColorTargetCubePosition(xdl_uint target_index, XdevLCubemapPosition cubemapPosition) = 0;

			/// Activates or deactivates the Depth target.
			virtual xdl_int setActiveDepthTest(xdl_bool state) = 0;

			/// Activates or deactivates the Stencil target.
			virtual xdl_int activateStencilTarget(xdl_bool state) = 0;

			/// Clears the Color Targets.
			virtual xdl_int clearColorTargets(xdl_float r, xdl_float g, xdl_float b, xdl_float a) = 0;

			/// Clears the Depth Target.
			virtual xdl_int clearDepthTarget(xdl_float clear_value) = 0;

			/// Clears the Stencil Target.
			virtual xdl_int clearStencilTarget(xdl_int clear_value) = 0;

			/// Returns the width of the framebuffer.
			virtual xdl_uint getWidth() = 0;

			/// Returns the height of the framebuffer.
			virtual xdl_uint getHeight() = 0;

			/// Returns the color target of the framebuffer.
			/**
				The default returned texture is 0.
			*/
			virtual IPXdevLTexture getTexture(xdl_uint idx = 0) = 0;

			/// Returns the depth texture.
			virtual IPXdevLTexture getDepthTexture() = 0;

			/// Returns the number of color targets used.
			virtual xdl_uint getNumColorTextures() = 0;

			/// Returns the id of the framebuffer.
			virtual xdl_uint id() = 0;
			
			virtual void blit(XdevLFrameBuffer* framebuffer, xdl_uint targets = XDEVL_COLOR_TARGET) = 0;
	};

	typedef XdevLFrameBuffer IXdevLFrameBuffer;
	typedef std::shared_ptr<XdevLFrameBuffer> IPXdevLFrameBuffer;
}

#endif
