#ifndef XDEVL_IMAGE_IMPL_H
#define XDEVL_IMAGE_IMPL_H

#include <XdevLImage/XdevLImage.h>

#include <XdevLPluginImpl.h>

namespace xdl {

	static const XdevLString vendor {
		"Vendors Name"
	};

	static const XdevLString author {
		"Authors Name"
	};

	static const XdevLString copyright {
		"Copyright here."
	};

	static const XdevLString pluginName {
		"XdevLImagePluginBMP"
	};

	static const XdevLString moduleDescription {
		"This plugin helps to import BMP files."
	};

	static std::vector<XdevLModuleName>	moduleNames {
		XdevLModuleName("XdevLImagePluginBMP"),
	};


#define BMP_TYPE 0x4D42
#define BMP_SIZE 40
#define BMP_OFF_BITS 54


	enum BmpFormat {
	  UNKNOWN				= -1L,
	  BMP_RGB				= 0L,
	  BMP_RLE8			= 1L,
	  BMP_RLE4			= 2L,
	  BMP_BITFIELDS = 3L,
	};

	struct BmpFileHeader {
		BmpFileHeader() : bfType(0),
			bfSize(0),
			bfReserved1(0),
			bfReserved2(0),
			bfOffBits(0) {}
		int16_t		bfType;
		int32_t		bfSize;
		int16_t		bfReserved1;
		int16_t		bfReserved2;
		int32_t		bfOffBits;
	};

	struct BmpInfoHeader {
		uint32_t	biSize;
		int32_t		biWidth;
		int32_t		biHeight;
		uint16_t	biPlanes;
		uint16_t	biBitCount;
		uint32_t	biCompression;
		uint32_t	biSizeImage;
		int32_t		biXPelsPerMeter;
		int32_t		biYPelsPerMeter;
		uint32_t	biClrUsed;
		uint32_t	biClrImportant;

		BmpInfoHeader() : biSize(0),
			biWidth(0),
			biHeight(0),
			biPlanes(0),
			biBitCount(0),
			biSizeImage(0),
			biXPelsPerMeter(0),
			biYPelsPerMeter(0),
			biClrUsed(0),
			biClrImportant(0) {}
	};

	class XdevLImagePluginBMP : public XdevLModuleImpl<XdevLImagePlugin> {

		public:

			XdevLImagePluginBMP(XdevLModuleCreateParameter* parameter);

			virtual ~XdevLImagePluginBMP();

			virtual xdl_int readInfo(const XdevLFileName& filename, XdevLImageObject* pInfo) override;
			virtual xdl_int load(const XdevLFileName& filename, XdevLImageObject* pImageObject) override;
			virtual const XdevLString& getExtension() const;
		private:
			void Read1Bit(std::ifstream& in, BmpInfoHeader* pHeader, xdl_uint8* pBuffer);
			void Read4Bit(std::ifstream& in, BmpInfoHeader* pHeader, xdl_uint8* pBuffer);
			void Read8Bit(std::ifstream& in, BmpInfoHeader* pHeader, xdl_uint8* pBuffer);
			void Read24Bit(std::ifstream& in, BmpInfoHeader* pHeader, xdl_uint8* pBuffer);
			void ReadRLE8(std::ifstream& in, BmpInfoHeader* pHeader, xdl_uint8* pBuffer);
		private:
			const XdevLString m_extension;

	};


}

#endif
