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
			char* Id;
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
		@class XdevLImageServer
		@brief Interface class for loading and saving images.

		XdevLImageServer supports different image formats. At the moment it converts from
		a specified format into an raw RGB or RGBA image format.
	*/
	class XdevLImageServer : public XdevLModule {

		public:

			virtual ~XdevLImageServer() {};

			/// Reads infos from the specified image.
			virtual xdl_int readInfo(const char* pFilename, XdevLImageObject* pInfo) = 0;

			/// Loads the specified image an converts it into a RGB or RGBA image format.
			virtual xdl_int load(const char* pFilename, XdevLImageObject* pImageObject) = 0;
	};


}

#endif
