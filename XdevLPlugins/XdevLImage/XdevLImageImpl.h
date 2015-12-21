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
		"XdevLImageServer"
	};

	static const XdevLString moduleDescription {
		"This is an empty module"
	};

	static std::vector<XdevLModuleName>	moduleNames {
		XdevLModuleName("XdevLBMP"),
	};

	class XdevLImageImpl : public XdevLModuleImpl<XdevLImageServer> {

		public:

			XdevLImageImpl(XdevLModuleCreateParameter* parameter);

			virtual ~XdevLImageImpl();

			virtual xdl_int readInfo(const char* pFilename, XdevLImageObject* pInfo);
			virtual xdl_int load(const char* pFilename, XdevLImageObject* pImageObject);
	};


}

#endif
