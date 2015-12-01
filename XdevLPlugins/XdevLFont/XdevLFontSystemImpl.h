#ifndef XDEVL_FONT_SYSTEM_IMPL_H
#define XDEVL_FONT_SYSTEM_IMPL_H

#include <XdevLPluginImpl.h>
#include <XdevLUtils.h>
#include <XdevLFont/XdevLFontSystem.h>
#include <XdevLRAI/XdevLRAI.h>


namespace xdl {

	class XdevLFontImpl;


	class XdevLFontSystemImpl : public XdevLModuleImpl<XdevLFontSystem> {
		public:

			XdevLFontSystemImpl(XdevLModuleCreateParameter* parameter, xdl::XdevLModuleDescriptor& moduleDescriptor);

			virtual ~XdevLFontSystemImpl();

			virtual xdl_int init(xdl_uint screenWidth, xdl_uint screenHeight, XdevLRAI* rai) override;

			virtual XdevLFont* createFromFontFile(const xdl_char* fontInfoFilename) override;

			virtual XdevLFont* createFontFromTexture(const xdl_char* fontInfoFilename, XdevLTexture* texture) override;

			virtual void setCreateTextureCallback(XdevLFontSystem::createTextureFromFileCallbackFunction function) override;

			virtual xdl_uint getScreenWidth() const;

			virtual xdl_uint getScreenHeight() const;

		private:
			XdevLGlyphMetric& readLine(std::ifstream& os, XdevLGlyphMetric& gp);
			void calculateGlyphInformation(XdevLFontImpl* font, std::ifstream& os);
		private:
			xdl_uint screenWidth;
			xdl_uint screenHeight;
			XdevLRAI* m_rai;
			XdevLFontSystem::createTextureFromFileCallbackFunction createTextureFromFile;
	};

}

#endif // XDEVL_FONT_SYSTEM_H
