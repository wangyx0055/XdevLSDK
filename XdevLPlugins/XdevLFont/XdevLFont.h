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
#ifndef XDEVL_FONT_H
#define XDEVL_FONT_H

#include <XdevLRAI/XdevLTexture.h>

namespace xdl {

	/**
		@struct XdevLFontVertex
		@brief The structure of the vertex for a font triangle.
	*/
	struct XdevLGlyphVertex {
		XdevLGlyphVertex() :
			x(0.0f),
			y(0.0f),
			r(0),
			g(0),
			b(0),
			a(255),
			s(0.0f),
			t(0.0f)
		{}

		xdl_float x,y;		/// Position data.
		xdl_uint8 r,g,b,a;	/// Color data.
		xdl_float s, t;		/// Texture coordinate data.
	};

	/**
		@struct XdevLGlyphMetric
		@brief A structure that describes the metric of a glyph.

	      Glyph metrics:
	      --------------

	                            xmin                     xmax
	                             |                         |
	                             |<-------- width -------->|
	                             |                         |
	                   |         +-------------------------+----------------- ymax
	                   |         |    aaaaaaaaa   aaaaa    |     ^        ^
	                   |         |   a:::::::::aaa::::a    |     |        |
	                   |         |  a:::::::::::::::::a    |     |        |
	                   |         | a::::::aaaaa::::::aa    |     |        |
	                   |         | a:::::a     a:::::a     |     |        |
	       brearing_x -|-------->| a:::::a     a:::::a     |  brearing_y  |
	                   |         | a:::::a     a:::::a     |     |        |
	                   |         | a::::::a    a:::::a     |     |        |
	                   |         | a:::::::aaaaa:::::a     |     |        |
	                   |         |  a::::::::::::::::a     |     |      height
	                   |         |   aa::::::::::::::a     |     |        |
	       baseline ---*---------|---- aaaaaaaa::::::a-----*--------      |
	                 / |         |             a:::::a     |              |
	          origin   |         | aaaaaa      a:::::a     |              |
	                   |         | a:::::aa   aa:::::a     |              |
	                   |         |  a::::::aaa:::::::a     |              |
	                   |         |   aa:::::::::::::a      |              |
	                   |         |     aaa::::::aaa        |              |
	                   |         |         aaaaaa          |              v
	                   |         +-------------------------+----------------- ymin
	                   |                                   |
	                   |------------- advance_h ---------->|
	*/
	struct XdevLGlyphMetric {
		XdevLGlyphMetric() :
			texture_id(0),
			character_code(0),
			left(0.0f),
			top(0.0f),
			width(0.0f),
			height(0.0f),
			advance_h(0.0f),
			advance_v(0.0f),
			brearing_x(0.0f),
			brearing_y(0.0f) {
		}
		uint32_t			texture_id;
		uint32_t 			character_code;
		xdl_float 			left;
		xdl_float 			top;
		xdl_float 			width;
		xdl_float 			height;
		xdl_float 			advance_h;
		xdl_float 			advance_v;
		xdl_float 			brearing_x;
		xdl_float 			brearing_y;
		XdevLGlyphVertex	vertices[4];
	};

	/**
		@class XdevLFont
		@brief This is an interface for font support.
	*/
	class XdevLFont {
		public:
			virtual ~XdevLFont() {}

			/// Returns a glphy's metric information.
			virtual XdevLGlyphMetric& getGlyphMetric(xdl_uint32 unicode) = 0;

			/// Returns the font size in pixels.
			/**
				This font size is the maximum witdh in horizontal direction.

				@return Font size in pixel units.
			*/
			virtual xdl_float getFontSize() const = 0;

			/// Returns the new line size.
			virtual xdl_float getNewLineSize() const = 0;

			/// Returns the number of textures used by this font.
			virtual xdl_uint getNumberOfTextures() const = 0;

			/// Returns a specific texture used for this font.
			virtual IPXdevLTexture getTexture(xdl_uint idx) = 0;

	};
	
	typedef XdevLFont IXdevLFont;
	typedef XdevLFont* IPXdevLFont;

}

#endif // XDEVL_FONT_H
