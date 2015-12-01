#include "XdevLPhysXImpl.h"
#include <string>

phsx::XdevLPhysXPluginDescriptor m_physXPluginDescriptor;
phsx::XdevLPhysXModuleDescriptor phsx::XdevLPhysXImpl::m_physXModuleDesc;


extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	// Create the "OpenGL" module.
	if (std::string(phsx::XdevLPhysXImpl::m_physXModuleDesc.getName()) == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new phsx::XdevLPhysXImpl(parameter);
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
	return &m_physXPluginDescriptor;
}

namespace phsx {

XdevLPhysXImpl::XdevLPhysXImpl(XdevLModuleCreateParameter* parameter) :
	XdevLModuleImpl<XdevLPhysX>(parameter),
	m_physicsSDK(NULL){

}

XdevLModuleDescriptor* XdevLPhysXImpl::getDescriptor() const {
	return &m_physXModuleDesc;
}

xdl_int XdevLPhysXImpl::init(){
	m_physicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);
	if(!m_physicsSDK){
		XDEVL_MODULE_ERROR("PhysXSDK can't be created.\n");
		return ERR_ERROR;
	}
	return ERR_OK;
}

xdl_int XdevLPhysXImpl::shutdown(){
	if(m_physicsSDK)	
		m_physicsSDK->release();
	return ERR_OK;
}

NxPhysicsSDK* XdevLPhysXImpl::getPhysXSDK(){
	return m_physicsSDK;
}

} // End of namespace