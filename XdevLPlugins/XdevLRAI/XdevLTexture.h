/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2014 Cengiz Terzibas

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

#ifndef XDEVL_TEXTURE_H
#define XDEVL_TEXTURE_H

#include <XdevLTypes.h>

namespace xdl {
	/**
		@enum 	XdevLTextureType
		@brief 	This defines, the 6 image IDs for the cubemap texture. You can access only with these IDs the wanted image.
	*/
	enum XdevLTextureType {
	    XDEVL_TEXTURE_1D        = 0x0DE0,
	    XDEVL_TEXTURE_2D        = 0x0DE1,
	    XDEVL_TEXTURE_3D        = 0x806F,
	    XDEVL_TEXTURE_CUBE_MAP  = 0x8513
	};

	/**
		@enum 	XdevLCubemapPosition
		@brief 	This defines, the 6 image IDs for the cubemap texture. You can access only with these IDs the wanted image.
	*/
	enum XdevLCubemapPosition {
	    XDEVL_TEXTURE_CUBE_MAP_POSITIVE_X = 0x8515,
	    XDEVL_TEXTURE_CUBE_MAP_NEGATIVE_X = 0x8516,
	    XDEVL_TEXTURE_CUBE_MAP_POSITIVE_Y = 0x8517,
	    XDEVL_TEXTURE_CUBE_MAP_NEGATIVE_Y = 0x8518,
	    XDEVL_TEXTURE_CUBE_MAP_POSITIVE_Z = 0x8519,
	    XDEVL_TEXTURE_CUBE_MAP_NEGATIVE_Z = 0x851A
	};


	/**
		@enum		XdevLTextureWrap
		@brief 	Definitions of the possible addressing methods of a texture
	*/
	enum XdevLTextureWrap {
	    XDEVL_CLAMP				= 0x2900,
	    XDEVL_REPEAT			= 0x2901,
	    XDEVL_CLAMP_TO_BORDER 	= 0x812D,
	    XDEVL_CLAMP_TO_EDGE 	= 0x812F
	};

	/**
		@enum 	XdevLTextureCoord
		@brief 	Texture coordinate components
	*/
	enum XdevLTextureCoord {
	    XDEVL_TEXTURE_WRAP_S = 0x2802,
	    XDEVL_TEXTURE_WRAP_T = 0x2803,
	    XDEVL_TEXTURE_WRAP_R = 0x8072
	};

	/**
		@enum 	XdevLTextureFilterStage
		@brief 	Filter for textures
	*/
	enum XdevLTextureFilterStage {
	    XDEVL_TEXTURE_MAG_FILTER = 0x2800,
	    XDEVL_TEXTURE_MIN_FILTER = 0x2801
	};

	/**
		@enum 	XdevLTextureFilter
		@brief 	Filter modes for textures
	*/
	enum XdevLTextureFilter {
	    XDEVL_NEAREST					= 0x2600,
	    XDEVL_LINEAR 					= 0x2601,
	    XDEVL_NEAREST_MIPMAP_NEAREST	= 0x2700,
	    XDEVL_LINEAR_MIPMAP_NEAREST		= 0x2701,
	    XDEVL_NEAREST_MIPMAP_LINEAR		= 0x2702,
	    XDEVL_LINEAR_MIPMAP_LINEAR		= 0x2703
	};


	/**
		@enum 	XdevLTextureStage
		@brief 	Texture stages
	*/
	enum XdevLTextureStage {
	    XDEVL_TEX_STAGE0 = 0,
	    XDEVL_TEX_STAGE1,
	    XDEVL_TEX_STAGE2,
	    XDEVL_TEX_STAGE3,
	    XDEVL_TEX_STAGE4,
	    XDEVL_TEX_STAGE5,
	    XDEVL_TEX_STAGE6,
	    XDEVL_TEX_STAGE7,
	    XDEVL_TEX_STAGE8,
	    XDEVL_TEX_STAGE9,
	    XDEVL_TEX_STAGE10,
	    XDEVL_TEX_STAGE11,
	    XDEVL_TEX_STAGE12,
	    XDEVL_TEX_STAGE13,
	    XDEVL_TEX_STAGE14,
	    XDEVL_TEX_STAGE15,
	    XDEVL_TEX_STAGE16,
	    XDEVL_TEX_STAGE17,
	    XDEVL_TEX_STAGE18,
	    XDEVL_TEX_STAGE19,
	    XDEVL_TEX_STAGE20,
	    XDEVL_TEX_STAGE21,
	    XDEVL_TEX_STAGE22,
	    XDEVL_TEX_STAGE23,
	    XDEVL_TEX_STAGE24,
	    XDEVL_TEX_STAGE25,
	    XDEVL_TEX_STAGE26,
	    XDEVL_TEX_STAGE27,
	    XDEVL_TEX_STAGE28,
	    XDEVL_TEX_STAGE29,
	    XDEVL_TEX_STAGE30,
	    XDEVL_TEX_STAGE31
	};

	enum XdevLTextureFormat {

	    XDEVL_RED 					= 0x1903,
	    XDEVL_GREEN					= 0x1904,
	    XDEVL_BLUE 					= 0x1905,
	    XDEVL_ALPHA 				= 0x1906,
	    XDEVL_RGB					= 0x1907,
	    XDEVL_RGBA					= 0x1908,
	    XDEVL_BGR 					= 0x80E0,
	    XDEVL_BGRA 					= 0x80E1,

	    XDEVL_R8					= 0x8229,
	    XDEVL_R16					= 0x822A,
	    XDEVL_RG8					= 0x822B,
	    XDEVL_RG16					= 0x822C,
	    XDEVL_R16F					= 0x822D,
	    XDEVL_R32F					= 0x822E,
	    XDEVL_RG16F					= 0x822F,
	    XDEVL_RG32F					= 0x8230,
	    XDEVL_R8I					= 0x8231,
	    XDEVL_R8UI					= 0x8232,
	    XDEVL_R16I					= 0x8233,
	    XDEVL_R16UI					= 0x8234,
	    XDEVL_R32I					= 0x8235,
	    XDEVL_R32UI					= 0x8236,
	    XDEVL_RG8I					= 0x8237,
	    XDEVL_RG8UI					= 0x8238,
	    XDEVL_RG16I					= 0x8239,
	    XDEVL_RG16UI				= 0x823A,
	    XDEVL_RG32I					= 0x823B,
	    XDEVL_RG32UI				= 0x823C,
	    XDEVL_RGBA32F 				= 0x8814,
	    XDEVL_RGB32F				= 0x8815,
	    XDEVL_RGBA16F				= 0x881A,
	    XDEVL_RGB16F				= 0x881B,
	    XDEVL_RGBA32UI				= 0x8D70,

	    XDEVL_R11F_G11F_B10F		= 0x8C3A,
	    XDEVL_RGB9_E5				= 0x8C3D,
	    XDEVL_SRGB					= 0x8C40,
	    XDEVL_SRGB8					= 0x8C41,
	    XDEVL_SRGB_ALPHA			= 0x8C42,
	    XDEVL_SRGB8_ALPHA8			= 0x8C43,


	    XDEVL_R8_SNORM				= 0x8F94,
	    XDEVL_RG8_SNORM				= 0x8F95,
	    XDEVL_RGB8_SNORM			= 0x8F96,
	    XDEVL_RGBA8_SNORM			= 0x8F97,
	    XDEVL_R16_SNORM				= 0x8F98,
	    XDEVL_RG16_SNORM			= 0x8F99,
	    XDEVL_RGB16_SNORM			= 0x8F9A,
	    XDEVL_RGBA16_SNORM			= 0x8F9B,


	    XDEVL_COMPRESSED_ALPHA 					= 0x84E9,
	    XDEVL_COMPRESSED_LUMINANCE 				= 0x84EA,
	    XDEVL_COMPRESSED_LUMINANCE_ALPHA 		= 0x84EB,
	    XDEVL_COMPRESSED_INTENSITY 				= 0x84EC,
	    XDEVL_COMPRESSED_RGB 					= 0x84ED,
	    XDEVL_COMPRESSED_RGBA 					= 0x84EE,
	    XDEVL_COMPRESSED_SRGB 					= 0x8C48,
	    XDEVL_COMPRESSED_SRGB_ALPHA 			= 0x8C49,
	    XDEVL_COMPRESSED_SLUMINANCE 			= 0x8C4A,
	    XDEVL_COMPRESSED_SLUMINANCE_ALPHA 		= 0x8C4B,
	    XDEVL_COMPRESSED_SRGB_S3TC_DXT1 		= 0x8C4C,
	    XDEVL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1	= 0x8C4D,
	    XDEVL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3	= 0x8C4E,
	    XDEVL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5	= 0x8C4F,

	    XDEVL_DEPTH_COMPONENT 		= 0x1902,
	    XDEVL_DEPTH_COMPONENT16		= 0x81A5,
	    XDEVL_DEPTH_COMPONENT24		= 0x81A6,
	    XDEVL_DEPTH_COMPONENT32		= 0x81A7,
	    XDEVL_DEPTH_COMPONENT32F 	= 0x8CAC,


	    XDEVL_DEPTH_STENCIL			= 0x84F9,
	    XDEVL_DEPTH24_STENCIL8		= 0x88F0,
	    XDEVL_DEPTH32F_STENCIL8		= 0x8CAD
	};

	enum XdevLTextureFormatType {
	    XDEVL_TYPE_DEPTH_COMPONENT 	= 0x1902,
	    XDEVL_TYPE_DEPTH_STENCIL	= 0x84F9,
	    XDEVL_TYPE_RGB 				= 0x1907,
	    XDEVL_TYPE_RGBA 			= 0x1908,
	    XDEVL_TYPE_RG 				= 0x8227

	};

	enum XdevLTextureDataTypes {
	    XDEVL_BYTE 				= 0x1400,
	    XDEVL_UNSIGNED_BYTE		= 0x1401,
	    XDEVL_SHORT				= 0x1402,
	    XDEVL_UNSIGNED_SHORT	= 0x1403,
	    XDEVL_INT				= 0x1404,
	    XDEVL_UNSIGNED_INT		= 0x1405,
	    XDEVL_FLOAT				= 0x1406,
	    XDEVL_2_BYTES			= 0x1407,
	    XDEVL_3_BYTES			= 0x1408,
	    XDEVL_4_BYTES			= 0x1409,
	    XDEVL_DOUBLE			= 0x140A
	};

	/**
		@class XdevLTexture
		@brief A texture.
	*/
	class XdevLTexture {
		public:

			virtual ~XdevLTexture() {}

			/// Initialize a basic texture.
			virtual xdl_int init(xdl_uint width, xdl_uint height, xdl_uint internalFormat) = 0;

			/// Initialize a basic texture from external source.
			virtual xdl_int init(xdl_uint width, xdl_uint height, xdl_uint internalFormat, xdl_uint imageFormat, xdl_uint8* data) = 0;

			/// Lock the texture to modify.
			virtual xdl_int lock() = 0;

			/// Unlock the texture after modification.
			virtual xdl_int unlock() = 0;

			/// Sets the filter type of the texture.
			virtual xdl_int setTextureFilter(XdevLTextureFilterStage filterType, XdevLTextureFilter filter) = 0;

			/// Sets the wrapping mode of the texture.
			/**
				@param texCoord
				@param textureWrap
			*/
			virtual xdl_int setTextureWrap(XdevLTextureCoord pTexCoord, XdevLTextureWrap pTextureWrap) = 0;
			/// Sets the anisotropy filter
			/**
				@param pValue The anisotropy value that should be used
			*/
			virtual xdl_int setTextureMaxAnisotropy(xdl_float value) = 0;

			/// Generates mip map automatically.
			virtual xdl_int generateMipMap() = 0;

			/// Activate the texture.
			/**
				@param idx The texture unit number to activate.
			*/
			virtual xdl_int activate(xdl_uint idx) = 0;

			/// Deactivate the texture.
			virtual xdl_int deactivate() = 0;

			/// Return the identification code.
			virtual xdl_uint id() = 0;

			/// Returns the width of the texture.
			virtual xdl_uint getWidth() = 0;

			/// Returns the height of the texture.
			virtual xdl_uint getHeight() = 0;
	};
}


#endif
