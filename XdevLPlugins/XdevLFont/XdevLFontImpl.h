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

#ifndef XDEVL_FONT_IMPL_H
#define XDEVL_FONT_IMPL_H

#include <XdevLWindow/XdevLWindow.h>
#include <XdevLRAI/XdevLRAI.h>
#include <XdevLFont/XdevLFont.h>
#include <map>

// TODO Add alignment stuff, like LEFT_TOP, TOP, RIGHT_TOP etc.

namespace xdl {

	// Holds the major version number of the plugin.
	const xdl_uint XdevLFontPluginMajorVersion = XDEVLFONT_MAJOR_VERSION;

	// Holds the Minor version number of the plugin.
	const xdl_uint XdevLFontPluginMinorVersion = XDEVLFONT_MINOR_VERSION;

	// Holds the Patch version number of the plugin.
	const xdl_uint XdevLFontPluginPatchVersion = XDEVLFONT_PATCH_VERSION;



	// Holds the Major version number.
	const xdl_uint XdevLFontSystemMajorVersion = XDEVLFONTSYSTEM_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLFontSystemMinorVersion = XDEVLFONTSYSTEM_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLFontSystemPatchVersion = XDEVLFONTSYSTEM_MODULE_PATCH_VERSION;



	// Holds the Major version number.
	const xdl_uint XdevLTextLayoutMajorVersion = XDEVLTEXTLAYOUT_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLTextLayoutMinorVersion = XDEVLTEXTLAYOUT_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLTextLayoutPatchVersion = XDEVLTEXTLAYOUT_MODULE_PATCH_VERSION;
	


	static const XdevLString vendor {
		"www.codeposer.net"
	};
	static const XdevLString author {
		"Cengiz Terzibas"
	};
	static const XdevLString copyright {
		"(c) 2005 - 2015 Cengiz Terzibas."
	};
	static const XdevLString pluginName {
		"XdevLFont"
	};
	static const XdevLString description {
		"This plugin helps rendering fonts."
	};

	static std::vector<XdevLModuleName>	moduleNames {
		XdevLModuleName("XdevLFontSystem"),
		XdevLModuleName("XdevLTextLayout")
	};


	class XdevLFontImpl : public XdevLFont {
		public:
			typedef std::vector<XdevLTexture*> XdevLTextureArray;
			typedef std::map<xdl_uint32, XdevLGlyphMetric> XdevLGlyphMetricMap;

			XdevLFontImpl();

			virtual ~XdevLFontImpl();

			virtual XdevLGlyphMetric& getGlyphMetric(xdl_uint32 unicode);

			virtual xdl_float getFontSize() const;

			virtual xdl_float getNewLineSize() const;

			virtual xdl_uint getNumberOfTextures() const;

			virtual XdevLTexture* getTexture(xdl_uint idx);

		public:

			void addFontTexture(XdevLTexture* fontTexture) {
				m_textureList.push_back(fontTexture);
			}

			XdevLTexture* getFontTexture(const XdevLGlyphMetric& gp) {
				return m_textureList[gp.texture_id];
			}

			void setNewLineSize(xdl_float newLineSize) {
				m_newLine = newLineSize;
			}

			void setUnitX(xdl_float unitX) {
				m_unitX = unitX;
			}

			void setUnitY(xdl_float unitY) {
				m_unitY = unitY;
			}

			void setFontSize(xdl_uint fontSize) {
				m_fontSize = static_cast<xdl_float>(fontSize);
			}

			void addGlyph(const XdevLGlyphMetric& glyph) {
				m_glyphMap[glyph.character_code] = glyph;
			}

		private:

			xdl_float 			m_fontSize;
			xdl_float			m_newLine;
			xdl_float 			m_unitX;
			xdl_float 			m_unitY;
			XdevLGlyphMetric	m_dummyGlyph;
			XdevLGlyphMetricMap	m_glyphMap;
			XdevLTextureArray	m_textureList;
	};

	extern xdl::XdevLModuleDescriptor fontSystemModuleDesc;
	extern xdl::XdevLModuleDescriptor textLayoutModuleDesc;
}

#endif
