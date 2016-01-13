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
	
	typedef XdevLTexture3D IXdevLTexture3D;
	typedef std::shared_ptr<XdevLTexture3D> IPXdevLTexture3D;
}


#endif
