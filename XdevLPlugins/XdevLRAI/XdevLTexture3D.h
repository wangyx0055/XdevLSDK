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

#ifndef XDEVL_TEXTURE_3D_H
#define XDEVL_TEXTURE_3D_H

#include <XdevLTypes.h>

namespace xdl {

	/**
		@class XdevLTexture3D
		@brief A texture.
	*/
	class XdevLTexture3D {
		public:

			virtual ~XdevLTexture3D() {}

			/// Initialize a basic texture.
			virtual xdl_int init(xdl_uint width, xdl_uint height, xdl_uint depth, xdl_uint internalFormat) = 0;

			/// Initialize a basic texture.
			virtual xdl_int init(xdl_uint width, xdl_uint height, xdl_uint depth, xdl_uint internalFormat, xdl_uint imageFormat, xdl_uint8* data) = 0;

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

			/// Returns the depth of the texture.
			virtual xdl_uint getDepth() = 0;
	};
}


#endif
