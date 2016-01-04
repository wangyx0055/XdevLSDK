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

#ifndef XDEVL_IMAGE_H
#define XDEVL_IMAGE_H

#include <XdevLModule.h>

namespace xdl {

	/**
	  @struct XdevLImage
	  @brief  The image object.
	*/
	class XdevLImage {
		public:

			XdevLImage() : Buffer(0), LOD(0) {}
			~XdevLImage() {
				delete [] Buffer;
				delete LOD;
			}
			xdl_char* Id;
			xdl_uint32 Width;
			xdl_uint32 Height;
			xdl_uint16 BitsPerPixel;
			xdl_uint8* Buffer;
			XdevLImage* LOD;
	};

	class XdevLImageObject {
		public:

			XdevLImageObject() : NumImages(0), Images(0) {}
			~XdevLImageObject() {
				delete [] Images;
			}
			int NumImages;
			XdevLImage* Images;
	};


	/**
		@class XdevLImagePlugin
		@brief Interface class for loading and saving images.

		XdevLImagePlugin supports different image formats. At the moment it converts from
		a specified format into an raw RGB or RGBA image format.
	*/
	class XdevLImagePlugin : public XdevLModule {

		public:

			virtual ~XdevLImagePlugin() {};

			/// Reads infos from the specified image.
			virtual xdl_int readInfo(const XdevLFileName& filename, XdevLImageObject* imageObject) = 0;

			/// Loads the specified image an converts it into a RGB or RGBA image format.
			virtual xdl_int load(const XdevLFileName& filename, XdevLImageObject* imageObject) = 0;

			/// Returns the supported extension.
			virtual const XdevLString& getExtension() const = 0;
	};


	class XdevLImageServer {
		public:

			/// Reads infos from the specified image.
			virtual xdl_int readInfo(const XdevLFileName& filename, XdevLImageObject* imageObject) = 0;

			/// Loads the specified image an converts it into a RGB or RGBA image format.
			virtual xdl_int load(const XdevLFileName& filename, XdevLImageObject* imageObject) = 0;

	};

}

#endif
