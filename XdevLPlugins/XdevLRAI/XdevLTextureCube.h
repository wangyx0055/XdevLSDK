#ifndef XDEVL_TEXTURE_CUBE_H
#define XDEVL_TEXTURE_CUBE_H

namespace xdl {

	/**
		@class XdevLTextureCube
		@brief A cube texture.
	*/
	class XdevLTextureCube {
		public:
			virtual ~XdevLTextureCube() {}

			/// Initialize a basic cube texture.
			/**
				@param width The width of the cube map texture.
				@param height The height of the cube map texture.
				@param internalFormat The format which shall be used internally.
				@param cubeMapPositionList A list of cube map position which defines the order of the specified data array. Must be 6 values.
				@param data An array of 6 bitmaps which represents one cube map texutre.
			*/
			virtual xdl_int init(xdl_uint width, xdl_uint height, xdl_uint internalFormat, xdl_uint imageFormat, XdevLCubemapPosition cubeMapPositionList[], xdl_uint8* data[]) = 0;

			/// Upload one Cube Map texture.
			virtual xdl_int upload(XdevLCubemapPosition cubeMapPosition, xdl_uint internalFormat, xdl_uint width, xdl_uint height,  xdl_uint imageFormat, xdl_uint8* data) = 0;

			/// Generate the Mip Maps for the Cube Map.
			virtual xdl_int generateMipMap() = 0;

			/// Activates the Cube Map texture.
			virtual xdl_int activate(xdl_uint idx) = 0;

			/// Deactivates the Cube Map texture.
			virtual xdl_int deactivate() = 0;

			/// Lock the Cube Map for modification.
			virtual xdl_int lock() = 0;

			/// Unlock the Cube Map after modification.
			virtual xdl_int unlock() = 0;

			/// Returns the identification of the Cube Map texture.
			virtual xdl_uint id() = 0;
	};

}

#endif
