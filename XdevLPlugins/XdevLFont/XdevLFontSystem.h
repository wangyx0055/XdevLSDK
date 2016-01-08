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

#ifndef XDEVL_FONT_SYSTEM_H
#define XDEVL_FONT_SYSTEM_H

#include <XdevLRAI/XdevLRAI.h>
#include <XdevLFont/XdevLFont.h>

namespace xdl {

	/**
		@class XdevLFontSystem
		@brief Manages creating and destroying font files.
	
		The XdevLFontSystem helps to create XdevLFonts which are then used for rendering text. The first
		implementation of this system is using textures. The font system supports at the moment only 
		using a super simple texture based font, or a more advanced texture font with multiple texture
		to support for example big fonts and unicode for Arabic, Japanese, Chinese etc. fonts.
		
		If no image to XdevLTexture conversion method is specified using the setCreateTextureCallback method,
		XdevLFontSystem will use a internal image conversion tool that only supports PNG files.
		You may specify an external image to XdevLTexture conversion tool using the setCreateTextureCallback
		method. The specified function muss be of the form createTextureFromFileCallbackFunction.
		
		
	*/
	class XdevLFontSystem : public XdevLModule {
		public:
			/// The function pointer that creates a texture out of an image.
			typedef IPXdevLTexture (*createTextureFromFileCallbackFunction)(const XdevLFileName& imageFileName);

			virtual ~XdevLFontSystem() {};

			/// Initialize the font system.
			virtual xdl_int init(xdl_uint screenWidth, xdl_uint screenHeight, IPXdevLRAI rai) = 0;

			/// Create a font from the font file.
			virtual IPXdevLFont createFromFontFile(const XdevLFileName& fontInfoFilename) = 0;

			/// Create font from texture.
			virtual IPXdevLFont createFontFromTexture(const XdevLFileName& fontInfoFilename, IPXdevLTexture texture) = 0;

			/// Sets the function which imports a image and creates a texture.
			virtual void setCreateTextureCallback(createTextureFromFileCallbackFunction function) = 0;
			
			/// Returns the screen width.
			virtual xdl_uint getScreenWidth() const = 0;
			
			/// Returns the screen height.
			virtual xdl_uint getScreenHeight() const = 0;
	};

	typedef XdevLFontSystem		IXdevLFontSystem;
	typedef XdevLFontSystem*	IPXdevLFontSystem;
}

#endif // XDEVL_FONT_SYSTEM_H
