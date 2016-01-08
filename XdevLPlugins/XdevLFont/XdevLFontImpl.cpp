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

#include "XdevLFontImpl.h"
#include "XdevLFontSystemImpl.h"
#include "XdevLTextLayoutImpl.h"
#include "lodepng.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>


xdl::XdevLPluginDescriptor pluginDescriptor {
	xdl::pluginName,
	xdl::moduleNames,
	XDEVLFONT_MAJOR_VERSION,
	XDEVLFONT_MINOR_VERSION,
	XDEVLFONT_PATCH_VERSION
};

xdl::XdevLModuleDescriptor fontSystemModuleDesc {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[0],
	xdl::copyright,
	xdl::description,
	XDEVLFONTSYSTEM_MODULE_MAJOR_VERSION,
	XDEVLFONTSYSTEM_MODULE_MINOR_VERSION,
	XDEVLFONTSYSTEM_MODULE_PATCH_VERSION
};


xdl::XdevLModuleDescriptor textLayoutModuleDesc {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[1],
	xdl::copyright,
	xdl::description,
	XDEVLTEXTLAYOUT_MODULE_MAJOR_VERSION,
	XDEVLTEXTLAYOUT_MODULE_MINOR_VERSION,
	XDEVLTEXTLAYOUT_MODULE_PATCH_VERSION
};

extern "C" XDEVL_EXPORT xdl::xdl_int _init_plugin(xdl::XdevLPluginCreateParameter* parameter) {
	return xdl::ERR_OK;
}

extern "C" XDEVL_EXPORT xdl::xdl_int _shutdown_plugin() {
	return xdl::ERR_OK;
}

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	if(fontSystemModuleDesc.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLFontSystemImpl(parameter, fontSystemModuleDesc);
		if(!obj)
			return xdl::ERR_ERROR;
		parameter->setModuleInstance(obj);
		return xdl::ERR_OK;
	} else if(textLayoutModuleDesc.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLTextLayoutImpl(parameter, textLayoutModuleDesc);
		if(!obj)
			return xdl::ERR_ERROR;
		parameter->setModuleInstance(obj);
		return xdl::ERR_OK;
	} else 


	return xdl::ERR_MODULE_NOT_FOUND;
}
extern "C" XDEVL_EXPORT void _delete(xdl::XdevLModule* obj) {
	if(obj)
		delete obj;
}

extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor() {
	return &pluginDescriptor;
}


namespace xdl {


	XdevLFontImpl::XdevLFontImpl() :
		m_fontSize(0.0f),
		m_newLine(0.0f),
		m_unitX(1.0f),
		m_unitY(1.0f) {

		//
		// Create dummy glyph for unknown glyphs. This will be returned if the method getGlyphMetric()
		// fails.
		//
		m_dummyGlyph.width		= 50;
		m_dummyGlyph.height		= 50;
		m_dummyGlyph.advance_h	= m_dummyGlyph.width;
		m_dummyGlyph.advance_v	= m_dummyGlyph.height;

		m_dummyGlyph.vertices[0].x = 0;
		m_dummyGlyph.vertices[0].y = (m_dummyGlyph.height - m_dummyGlyph.brearing_y);
		m_dummyGlyph.vertices[0].s = 0.0f;
		m_dummyGlyph.vertices[0].t = 0.0f;

		m_dummyGlyph.vertices[1].x = m_dummyGlyph.brearing_x;
		m_dummyGlyph.vertices[1].y = m_dummyGlyph.brearing_y;
		m_dummyGlyph.vertices[1].s = 0.0f;
		m_dummyGlyph.vertices[1].t = 1.0f;

		m_dummyGlyph.vertices[2].x = m_dummyGlyph.brearing_x + m_dummyGlyph.width;
		m_dummyGlyph.vertices[2].y = m_dummyGlyph.brearing_y;
		m_dummyGlyph.vertices[2].s = 1.0f;
		m_dummyGlyph.vertices[2].t = 1.0f;

		m_dummyGlyph.vertices[3].x = m_dummyGlyph.brearing_x + m_dummyGlyph.width;
		m_dummyGlyph.vertices[3].y = (m_dummyGlyph.height - m_dummyGlyph.brearing_y);
		m_dummyGlyph.vertices[3].s = 1.0f;
		m_dummyGlyph.vertices[3].t = 0.0f;


	}

	XdevLFontImpl::~XdevLFontImpl() {

	}

	XdevLGlyphMetric& XdevLFontImpl::getGlyphMetric(xdl_uint32 unicode) {

		auto ib = m_glyphMap.find(unicode);
		if(ib == m_glyphMap.end()) {
			return m_dummyGlyph;
		}

		return ib->second;
	}

	xdl_float  XdevLFontImpl::getFontSize() const {
		return m_fontSize;
	}

	xdl_float XdevLFontImpl::getNewLineSize() const {
		return m_newLine;
	}

	IPXdevLTexture XdevLFontImpl::getTexture(xdl_uint idx) {
		assert((idx < m_textureList.size()) && "XdevLFontImpl::getTexture: Specified index out of range.");
		return m_textureList[idx];
	}

	xdl_uint XdevLFontImpl::getNumberOfTextures() const {
		return m_textureList.size();
	}

}

//	wchar_t tmp1 [] = {	32, 33, 34, 35, 36, 37, 38, 39, 10,
//											40, 41, 42, 43, 44, 45, 46, 47, 10,
//											48, 49, 50, 51, 52, 53, 54, 55, 10,
//											56, 57, 58, 59, 60, 61, 62, 63, 10,
//											64, 65, 66, 67, 68, 69, 70, 71, 10,
//											72, 73, 74, 75, 76, 77, 78, 79, 10,
//											80, 81, 82, 83, 84, 85, 86, 87, 10,
//											88, 89, 90, 91, 92, 93, 94, 95};
//	message = tmp1;


//	wchar_t tmp2 [] = {	96,   97,  98,  99, 100, 101, 102, 103, 10,
//											104, 105, 106, 107, 108, 109, 110, 111, 10,
//											112, 113, 114, 115, 116, 117, 118, 119, 10,
//											120, 121, 122, 123, 124, 125, 126, 160, 10,
//											161, 162, 163, 164, 165, 166, 167, 168, 10,
//											169, 170, 171, 172, 173, 174, 175, 176, 10,
//											177, 178, 179, 180, 181, 182, 183, 184, 10,
//											185, 186, 187, 188, 189, 190, 191, 192};
//	message = tmp2;


//	wchar_t tmp3 [] = {	193, 194, 195, 196, 197, 198, 199, 200, 10,
//											201, 202, 203, 204, 205, 206, 207, 208, 10,
//											209, 210, 211, 212, 213, 214, 215, 216, 10,
//											217, 218, 219, 220, 221, 222, 223, 224, 10,
//											225, 226, 227, 228, 229, 230, 231, 232, 10,
//											233, 234, 235, 236, 237, 238, 239, 240, 10,
//											241, 242, 243, 244, 245, 246, 247, 248, 10,
//											249, 250, 251, 252, 253, 254, 255, 262};
//	message = tmp3;
