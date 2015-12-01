#include <XdevLCoreMediator.h>
#include <XdevLError.h>
#include "XdevLCudaImpl.h"

xdl::XdevLCudaModuleDescriptor xdl::XdevLCudaImpl::m_pluginDesc;
xdl::XdevLCudaPluginDescriptor m_openglDescriptor;

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	// Create the "CUDA" module.
	if (std::string(xdl::XdevLCudaImpl::m_pluginDesc.getName()) == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLCudaImpl(parameter);

		if (!obj)
			return xdl::ERR_ERROR;

		parameter->setModuleInstance(obj);

		return xdl::ERR_OK;
	}
	return xdl::ERR_MODULE_NOT_FOUND;
}


extern "C" XDEVL_EXPORT void _delete(xdl::XdevLModule* obj) {
	if (obj)
		delete obj;
}

extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor() {
	return &m_openglDescriptor;
}

namespace xdl {


XdevLCudaImpl::XdevLCudaImpl(XdevLModuleCreateParameter* parameter) : XdevLModuleImpl<XdevLCuda>(parameter) {
}

XdevLModuleDescriptor* XdevLCudaImpl::getDescriptor() const {
	return &m_pluginDesc;
}

xdl_int XdevLCudaImpl::init() {
	TiXmlDocument xmlDocument;

	if (!xmlDocument.LoadFile(getMediator()->getXmlFilename())) {
		XDEVL_MODULE_ERROR("Could not parse xml file: " << getMediator()->getXmlFilename() << std::endl);
		return ERR_ERROR;
	}

	if (readModuleInformation(&xmlDocument) != ERR_OK)
		return ERR_ERROR;


	cudaGetDevice(&m_devID);
  cudaGetDeviceProperties(&m_prop, m_devID);
	
	return ERR_OK;
}

xdl_int XdevLCudaImpl::shutdown() {
	return ERR_OK;
}

void* XdevLCudaImpl::getInternal(const xdl_char* id) {
	std::string data(id);
//	if (data == "WIN32_DC")
//		return m_DC;
	return NULL;
}

xdl_int XdevLCudaImpl::readModuleInformation(TiXmlDocument* document) {
	TiXmlHandle docHandle(document);
	TiXmlElement* root = docHandle.FirstChild(XdevLCorePropertiesName.c_str()).FirstChildElement("XdevLOpenGL").ToElement();

	if (!root) {
		XDEVL_MODULE_INFO("<XdevLCuda> section not found, skipping proccess.\n");
		return ERR_OK;
	}

	if(root->Attribute("id")){
		XdevLModuleId id(root->Attribute("id"));
		if(*getId() != id){
			return ERR_OK;
		}		
	}else{
		XDEVL_MODULE_ERROR("No 'id' attribute specified.");
		return ERR_ERROR;
	}

//	if (root->Attribute("framebuffer_depth")){
//		m_ColorDepth = xstd::from_string<xdl_int>(root->Attribute("framebuffer_depth"));
//		XDEVL_MODULE_INFO("Framebuffer depth request: " << m_ColorDepth << std::endl);
//	}

	return ERR_OK;
}


xdl_int XdevLCudaImpl::multMatrix(xdl_int n, xdl_int m, xdl_int8* buffer){
	return ERR_OK;
}

}