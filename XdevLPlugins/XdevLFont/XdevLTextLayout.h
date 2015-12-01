/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2015 Cengiz Terzibas

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

#ifndef XDEVL_TEXT_LAYOUT_H
#define XDEVL_TEXT_LAYOUT_H

#include <XdevLFont/XdevLFont.h>
#include <string>

namespace xdl {

	/**
		@struct XdevLTextLayoutText
		@brief A structure holds data for text rendering.
	*/
	struct XdevLTextLayoutText {
		/// The text to render.
		std::wstring text;
		
		/// The start position in x direction.
		xdl_float x;
		
		/// The start position in y direction.
		xdl_float y;
		
		/// The color of the whole text.
		xdl_uint8 color[4];
		
		/// The scale factor of the whole text.
		xdl_float scale;
	};

	/**
		@class XdevLTextLayout
		@brief A interface for a text layout engine.
	*/
	class XdevLTextLayout : public XdevLModule {
		public:
			virtual ~XdevLTextLayout() {}

			/// Initialize the layout engine.
			virtual xdl_int init(XdevLWindow* window, XdevLRAI* rai) = 0;

			/// Sets the current font to use for rendering text.
			virtual void useFont(XdevLFont* font) = 0;

			/// Print text without optimization.
			virtual void printText(const std::wstring& text, xdl_float x, xdl_float y) = 0;

			/// Add dynamic text.
			/**
				A dyanmic text is a text which change the number of letters and/or letters itself. 
				A vertex buffer is created and continuously updated.
			*/
			virtual void addDynamicText(const std::wstring& text, xdl_float x, xdl_float y) = 0;

			/// Sets the scale factor.
			virtual void setScale(xdl_float scale) = 0;

			/// Sets the DPI.
			virtual void setDPI(xdl_float) = 0;

			/// Sets the current color.
			virtual void setColor(xdl_uint32 r, xdl_uint32 g, xdl_uint32 b, xdl_uint32 a) = 0;

			/// Use signed distance field fonts.
			virtual void setDFT(xdl_bool enable) = 0;

			/// Sets the font rendering effect.
			virtual void setEffect(xdl_uint effectNumber) = 0;

			/// Sets the shadow offset parameter.
			virtual void setShadowOffset(xdl_float xOffset, xdl_float yOffset) = 0;

			/// Sets the screen dimensions.
			virtual void setScreenDimensions(xdl_uint screenWidth, xdl_uint screenHeight) = 0;
			
			/// Sets which units shall be used.
			virtual void usePixelUnits(xdl_bool state) = 0;

			/// Draws everything.
			virtual void render() = 0;

	};

	typedef XdevLTextLayout		IXdevLTextLayout;
	typedef XdevLTextLayout*	IPXdevLTextLayout;
}

#endif
