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

#ifndef XDEVL_FONT_SYSTEM_IMPL_H
#define XDEVL_FONT_SYSTEM_IMPL_H

#include <XdevLPluginImpl.h>
#include <XdevLUtils.h>
#include <XdevLFont/XdevLFontSystem.h>
#include <XdevLRAI/XdevLRAI.h>

#include <list>

namespace xdl {

	class XdevLFontImpl;


	class XdevLFontSystemImpl : public XdevLModuleImpl<XdevLFontSystem> {
		public:

			XdevLFontSystemImpl(XdevLModuleCreateParameter* parameter, xdl::XdevLModuleDescriptor& moduleDescriptor);

			virtual ~XdevLFontSystemImpl();

			virtual xdl_int init(xdl_uint screenWidth, xdl_uint screenHeight, IPXdevLRAI rai) override;
			virtual xdl_int shutdown() override;
			virtual IPXdevLFont createFromFontFile(const XdevLFileName& fontInfoFilename) override;
			virtual IPXdevLFont createFontFromTexture(const XdevLFileName& fontInfoFilename, IPXdevLTexture texture) override;
			virtual void setCreateTextureCallback(XdevLFontSystem::createTextureFromFileCallbackFunction function) override;
			virtual xdl_uint getScreenWidth() const;
			virtual xdl_uint getScreenHeight() const;

		private:
			XdevLGlyphMetric& readLine(std::ifstream& os, XdevLGlyphMetric& gp);
			void calculateGlyphInformation(XdevLFontImpl* font, std::ifstream& os);
		private:
			xdl_uint screenWidth;
			xdl_uint screenHeight;
			IPXdevLRAI m_rai;
			XdevLFontSystem::createTextureFromFileCallbackFunction createTextureFromFile;
			std::list<IPXdevLFont> m_fonts;
	};

}

#endif // XDEVL_FONT_SYSTEM_H
