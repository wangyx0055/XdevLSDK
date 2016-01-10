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

#include "XdevLImagePluginBMP.h"

#include <fstream>

xdl::XdevLPluginDescriptor pluginDescriptor {
	xdl::pluginName,
	xdl::moduleNames,
	XDEVLIMAGE_PLUGIN_MAJOR_VERSION,
	XDEVLIMAGE_PLUGIN_MINOR_VERSION,
	XDEVLIMAGE_PLUGIN_PATCH_VERSION
};

xdl::XdevLModuleDescriptor moduleDescriptor {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[0],
	xdl::copyright,
	xdl::moduleDescription,
	XDEVLBMP_MODULE_MAJOR_VERSION,
	XDEVLBMP_MODULE_MINOR_VERSION,
	XDEVLBMP_MODULE_PATCH_VERSION
};

XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(pluginDescriptor);

XDEVL_PLUGIN_CREATE_MODULE {
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLImagePluginBMP, moduleDescriptor)
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}

namespace xdl {

	XdevLImagePluginBMP::XdevLImagePluginBMP(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		XdevLModuleImpl<XdevLImagePlugin> (parameter, descriptor),
		m_extension(XdevLString("bmp")) {
	}

	XdevLImagePluginBMP::~XdevLImagePluginBMP() {
	}

	xdl_int XdevLImagePluginBMP::readInfo(const XdevLFileName& filename, XdevLImageObject* pInfo) {
		std::ifstream in(filename.toString().c_str(), std::ios_base::binary);
		if(!in.is_open()) {
			XDEVL_MODULE_ERROR("Could not open: \"" << filename << "\"\n");
			return ERR_ERROR;
		}

		// Read the bmp file and info header into buffer. (we store driver access)
		// if we load all directly into buffer.
		char buffer[54] = {0};
		in.read(buffer, 54);

		// Extract file header the information.
		BmpFileHeader fheader;
		fheader.bfType			= *(int16_t*)(&buffer[0]);
		if(fheader.bfType != BMP_TYPE) {
			XDEVL_MODULE_ERROR("\"" << filename << "\" has not the right Bmp format.\n");
			return ERR_ERROR;
		}
		fheader.bfSize			= *(int32_t*)(&buffer[2]);
		fheader.bfReserved1 = *(int16_t*)(&buffer[6]);
		fheader.bfReserved2 = *(int16_t*)(&buffer[8]);
		fheader.bfOffBits		= *(int32_t*)(&buffer[10]);

		// Extract the info header information.
		BmpInfoHeader iheader;
		iheader.biSize					= *(uint32_t*)(&buffer[14]);
		iheader.biWidth					= *(int32_t*)(&buffer[18]);
		iheader.biHeight				= *(int32_t*)(&buffer[22]);
		iheader.biPlanes				= *(uint16_t*)(&buffer[26]);
		iheader.biBitCount			= *(uint16_t*)(&buffer[28]);
		iheader.biCompression		= *(uint32_t*)(&buffer[30]);
		iheader.biSizeImage			= *(uint32_t*)(&buffer[34]);
		iheader.biXPelsPerMeter	= *(int32_t*)(&buffer[38]);
		iheader.biYPelsPerMeter	= *(int32_t*)(&buffer[42]);
		iheader.biClrUsed				= *(uint32_t*)(&buffer[46]);
		iheader.biClrImportant	= *(uint32_t*)(&buffer[50]);

		pInfo->NumImages							= 1;
		if(!pInfo->Images) {
			pInfo->Images								= new XdevLImage[1];
		}
		pInfo->Images[0].LOD					= 0;
		pInfo->Images[0].Id						= nullptr;
		pInfo->Images[0].Width				= iheader.biWidth;
		pInfo->Images[0].Height				=	iheader.biHeight;
		pInfo->Images[0].BitsPerPixel	= iheader.biBitCount;

		in.close();
		return ERR_OK;
	}

	xdl_int XdevLImagePluginBMP::load(const XdevLFileName& filename, XdevLImageObject* pImageObject) {
		std::ifstream in;
		// Open the image file.
		in.open(filename.toString().c_str(), std::ios_base::binary);
		if(!in.is_open()) {
			return ERR_ERROR;
		}

		// Read the bmp file and info header into buffer. (we store driver access)
		// if we load all directly into buffer.
		char buffer[54] = {0};
		in.read(buffer, 54);

		// Extract file header the information.
		BmpFileHeader fheader;
		fheader.bfType			= *(int16_t*)(&buffer[0]);
		if(fheader.bfType != BMP_TYPE) {
			XDEVL_MODULE_ERROR("\"" << filename << "\" has not the right Bmp format.\n");
			return ERR_ERROR;
		}
		fheader.bfSize			= *(int32_t*)(&buffer[2]);
		fheader.bfReserved1 = *(int16_t*)(&buffer[6]);
		fheader.bfReserved2 = *(int16_t*)(&buffer[8]);
		fheader.bfOffBits		= *(int32_t*)(&buffer[10]);

		// Extract the info header information.
		BmpInfoHeader iheader;
		iheader.biSize					= *(uint32_t*)(&buffer[14]);
		iheader.biWidth					= *(int32_t*)(&buffer[18]);
		iheader.biHeight				= *(int32_t*)(&buffer[22]);
		iheader.biPlanes				= *(uint16_t*)(&buffer[26]);
		iheader.biBitCount			= *(uint16_t*)(&buffer[28]);
		iheader.biCompression		= *(uint32_t*)(&buffer[30]);
		iheader.biSizeImage			= *(uint32_t*)(&buffer[34]);
		iheader.biXPelsPerMeter	= *(int32_t*)(&buffer[38]);
		iheader.biYPelsPerMeter	= *(int32_t*)(&buffer[42]);
		iheader.biClrUsed				= *(uint32_t*)(&buffer[46]);
		iheader.biClrImportant	= *(uint32_t*)(&buffer[50]);

		// We have only one image in a BMP file.
		pImageObject->NumImages										= 1;
		if(!pImageObject->Images) {
			pImageObject->Images										= new XdevLImage[1];
		}
		// We dont have lod images in BMP files.
		pImageObject->Images[0].LOD						= 0;
		pImageObject->Images[0].Id						= nullptr;
		pImageObject->Images[0].Width					= 0;
		pImageObject->Images[0].Height				=	0;
		pImageObject->Images[0].BitsPerPixel	= 0;
		pImageObject->Images[0].Buffer				= 0;

		// We make allways an RGB image.
		pImageObject->Images[0].Buffer = new xdl_uint8[iheader.biHeight*iheader.biWidth*3];
		int compression = UNKNOWN;
		compression =  iheader.biCompression;
		switch(iheader.biCompression) {
				// Read uncompressed data
			case BMP_RGB: {
				switch(iheader.biBitCount) {
					case 1:
						Read1Bit(in, &iheader, pImageObject->Images[0].Buffer);
						break;
					case 4:
						Read4Bit(in, &iheader, pImageObject->Images[0].Buffer);
						break;
					case 8:
						Read8Bit(in, &iheader, pImageObject->Images[0].Buffer);
						break;
					case 24:
						Read24Bit(in, &iheader, pImageObject->Images[0].Buffer);
						break;
					case 32:
						Read24Bit(in, &iheader, pImageObject->Images[0].Buffer);
						break;
					default:
						XDEVL_MODULE_ERROR("Unknown BMP pixel size.\n");
						return ERR_ERROR;
				}
			}
			break;
			// Read compressed data
			case BMP_RLE4:
			case BMP_RLE8:
				ReadRLE8(in, &iheader, pImageObject->Images[0].Buffer);
				break;
		}
		pImageObject->Images[0].Height				= iheader.biHeight;
		pImageObject->Images[0].Width					= iheader.biWidth;
		pImageObject->Images[0].BitsPerPixel	= 24;
		in.close();
		return ERR_OK;
	}

	const XdevLString& XdevLImagePluginBMP::getExtension() const {
		return m_extension;
	}



	void XdevLImagePluginBMP::Read1Bit(std::ifstream& in, BmpInfoHeader* pHeader, xdl_uint8* pBuffer) {
		uint8_t map[2*4];
		uint8_t stridetmp[4];
		// Read the color index map (2 values)
		in.read((char*)(&map), 2*4);

		uint8_t index = 0;
		uint8_t stride = ((32 - (pHeader->biWidth % 32)) / 8) % 4;
		for(int y = pHeader->biHeight - 1; y >= 0; --y)
			for(int x = 0; x < pHeader->biWidth; x+=8) {
				// Get one index for the color map.
				in.read((char*)&index, 1);
				int pos = (y*pHeader->biWidth + x)*3;

				for(int bit = 0; bit < 8; ++bit) {
					uint8_t idx = (index&(128L >> bit)) ? 4 : 0;
					pBuffer[pos + bit*3 + 0 ] = map[idx + 2];
					pBuffer[pos + bit*3 + 1 ] = map[idx + 1];
					pBuffer[pos + bit*3 + 2 ] = map[idx + 0];

				}
				if(stride > 0)
					in.read((char*)(&stridetmp), stride);
			}

	}

	void XdevLImagePluginBMP::Read4Bit(std::ifstream& in, BmpInfoHeader* pHeader, xdl_uint8* pBuffer) {
		uint8_t map[16*4];
		// Read the color index map (16 values)
		in.read((char*)(&map), 16*4);

		uint8_t index = 0;
		for(int y = pHeader->biHeight - 1; y >= 0; --y)
			for(int x = 0; x < pHeader->biWidth; x+=2) {
				// Get one index for the color map.
				in.read((char*)&index, 1);
				int pos = (y*pHeader->biWidth + x)*3;
				uint8_t low = (index&0x0f)*4;
				uint8_t high = ((index&0xf0)*4 >> 4);
				pBuffer[pos + 0] = map[high + 2];
				pBuffer[pos + 1] = map[high + 1];
				pBuffer[pos + 2] = map[high + 0];

				pBuffer[pos + 3] = map[low + 2];
				pBuffer[pos + 4] = map[low + 1];
				pBuffer[pos + 5] = map[low + 0];

			}
	}

	void XdevLImagePluginBMP::Read8Bit(std::ifstream& in, BmpInfoHeader* pHeader, xdl_uint8* pBuffer) {
		uint8_t map[256*4];
		// Read the color index map (256 values)
		in.read((char*)(&map), 256*4);

		uint8_t index = 0;
		for(int y = pHeader->biHeight - 1; y >= 0; --y)
			for(int x = 0; x < pHeader->biWidth; ++x) {
				// Get one index for the color map.
				in.read((char*)&index, 1);
				int pos = (y*pHeader->biWidth + x)*3;
				pBuffer[pos + 0] = map[index*4 + 2];
				pBuffer[pos + 1] = map[index*4 + 1];
				pBuffer[pos + 2] = map[index*4 + 0];
			}
	}

	void XdevLImagePluginBMP::Read24Bit(std::ifstream& in, BmpInfoHeader* pHeader, xdl_uint8* pBuffer) {
		int8_t tmp[3] = {0};
		for(int y = pHeader->biHeight - 1; y >= 0; --y)
			for(int x = 0 ; x < pHeader->biWidth; ++x) {
				in.read((char*)&tmp[0], 3);
				int pos = (y*pHeader->biWidth + x)*3;
				pBuffer[pos + 0] = tmp[2];
				pBuffer[pos + 1] = tmp[1];
				pBuffer[pos + 2] = tmp[0];
			}
	}

	void XdevLImagePluginBMP::ReadRLE8(std::ifstream& in, BmpInfoHeader* pHeader, xdl_uint8* pBuffer) {
		uint8_t map[256*4];
		// Read the color index map (256 values)
		if(pHeader->biClrUsed > 0)
			in.read((char*)(&map), pHeader->biClrUsed*4);
		else
			in.read((char*)(&map), 256*4);


		for(int y = 0; y < pHeader->biHeight; ++y) {
			for(int x = 0; x < pHeader->biHeight; ++x) {
				uint32_t pos = (x + y*pHeader->biWidth)*3;
				pBuffer[pos + 0] = map[0];
				pBuffer[pos + 1] = map[1];
				pBuffer[pos + 2] = map[2];
			}
		}

		uint8_t tmp[2] = {0};
		for(int y = 0; y < pHeader->biHeight; ++y) {
			int x = 0;
			while(true) {
				in.read((char*)(&tmp), 2);

				if(tmp[0] == 0) {
					if(tmp[1] == 0)
						break;
					else if(tmp[1] == 0) {
						y = pHeader->biHeight;
						break;
					} else if(tmp[1] == 2) {
						in.read((char*)(&tmp), 2);
						x += tmp[0];
						y += tmp[1];
						if(y >= pHeader->biHeight)
							break;
					} else {
						if(pHeader->biWidth - x < tmp[1])
							return;
						//
						//uint8_t val;
						//for(int a = 0; a < tmp[1]; ++a){
						//	in.read((xdl_uint8*)(&val), 1);
						//	pBuffer[(y * pHeader->biWidth + x)*3 + 0] = map[val + 2];
						//	pBuffer[(y * pHeader->biWidth + x)*3 + 1] = map[val + 1];
						//	pBuffer[(y * pHeader->biWidth + x)*3 + 2] = map[val + 0];
						//	x++;
						//}
						//if (tmp[1] % 2)
						//	in.read((xdl_uint8*)&tmp[0], 2);
					}
				} else { // if(tmp[0] == 0)
					for(int a = 0; a < tmp[0]; ++a) {
						pBuffer[(y * pHeader->biWidth + x)*3 + 0] = map[tmp[1] + 2];
						pBuffer[(y * pHeader->biWidth + x)*3 + 1] = map[tmp[1] + 1];
						pBuffer[(y * pHeader->biWidth + x)*3 + 2] = map[tmp[1] + 0];
						x++;
					}
				}
			}
		}
		//
		//int y = 0;
		//do{
		//	in.read((xdl_uint8*)(&tmp), 2);
		//	if(tmp[0] == 0 && tmp[1] == 1)
		//		break;
		//	if(tmp[0] == 0 && tmp[1] == 0){
		//		y += pHeader->biWidth;
		//		x = 0;
		//		continue;
		//	}
		//	if(tmp[0] == 0 && tmp[1] == 2){
		//		in.read((xdl_uint8*)(&tmp), 2);
		//		x = tmp[0];
		//		y = tmp[1];
		//		continue;
		//	}
		//	if(tmp[0] == 0 && (tmp[1] >= 0x03 && tmp[1] <= 0xff)){
		//		uint8_t val;
		//		for(int a = 0; a < tmp[1]; ++a){
		//			in.read((xdl_uint8*)(&val), 1);
		//			pBuffer[(x + y)*3 + 0] = map[val + 2];
		//			pBuffer[(x + y)*3 + 1] = map[val + 1];
		//			pBuffer[(x + y)*3 + 2] = map[val + 0];
		//			x++;
		//		}
		//	}
		//	if(tmp[0] >= 3){
		//		for(int a = 0; a<tmp[0]; ++a){
		//			pBuffer[(x + y)*3 + 0] = map[tmp[1]*4	+ 2];
		//			pBuffer[(x + y)*3 + 1] = map[tmp[1]*4 + 1];
		//			pBuffer[(x + y)*3 + 2] = map[tmp[1]*4 + 0];
		//			x++;
		//		}
		//	}
		//}while(true);
	}

}
