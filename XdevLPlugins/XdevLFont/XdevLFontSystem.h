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
			typedef XdevLTexture* (*createTextureFromFileCallbackFunction)(const xdl_char* imageFileName);

			virtual ~XdevLFontSystem() {};

			/// Initialize the font system.
			virtual xdl_int init(xdl_uint screenWidth, xdl_uint screenHeight, XdevLRAI* rai) = 0;

			/// Create a font from the font file.
			virtual XdevLFont* createFromFontFile(const xdl_char* fontInfoFilename) = 0;

			/// Create font from texture.
			virtual XdevLFont* createFontFromTexture(const xdl_char* fontInfoFilename, XdevLTexture* texture) = 0;

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
