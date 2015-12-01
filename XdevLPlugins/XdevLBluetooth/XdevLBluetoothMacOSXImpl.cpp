#include <XdevLBluetoothMacOSXImpl.h>
#include <XdevLCoreMediator.h>
#include <XdevLPlatform.h>

xdl::XdevLModuleDescriptor xdl::XdevLBluetoothMacOSXImpl::m_bluetoothModuleDesc {	vendor,
    author,
    moduleNames[0],
    copyright,
    description,
    XdevLBluetoothMajorVersion,
    XdevLBluetoothMinorVersion,
    XdevLBluetoothPatchVersion };

xdl::XdevLPluginDescriptor bluetoothPluginDescriptor {	xdl::pluginName,
																												xdl::moduleNames,
																												xdl::XdevLBluetoothPluginMajorVersion,
																												xdl::XdevLBluetoothPluginMinorVersion,
																												xdl::XdevLBluetoothPluginPatchVersion };

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	if(xdl::XdevLBluetoothMacOSXImpl::m_bluetoothModuleDesc.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLBluetoothMacOSXImpl(parameter);
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
	return &bluetoothPluginDescriptor;
}

namespace xdl {

	XdevLBluetoothMacOSXImpl::XdevLBluetoothMacOSXImpl(XdevLModuleCreateParameter* parameter) :
		XdevLModuleImpl<XdevLBluetooth>(parameter, m_bluetoothModuleDesc) {};

	xdl_int XdevLBluetoothMacOSXImpl::init() {
		if(readInfoFromXMLFile() != ERR_OK) {
			XDEVL_MODULE_WARNING("Parsing problems occurred of the Core XML file.\n");
		}

		return ERR_OK;
	}

	xdl_int XdevLBluetoothMacOSXImpl::shutdown() {
		return close();
	}

	xdl_int XdevLBluetoothMacOSXImpl::readInfoFromXMLFile() {
		if(getMediator()->getXmlFilename() == NULL)
			return ERR_ERROR;

		TiXmlDocument xmlDocument;
		if(!xmlDocument.LoadFile(getMediator()->getXmlFilename())) {
			XDEVL_MODULE_ERROR("Could not parse xml file: " << getMediator()->getXmlFilename() << std::endl);
			return ERR_ERROR;
		}

		TiXmlHandle docHandle(&xmlDocument);
		TiXmlElement* root = docHandle.FirstChild(XdevLCorePropertiesName.c_str()).FirstChildElement("XdevLBluetooth").ToElement();
		if (!root) {
			XDEVL_MODULE_WARNING("<XdevLBluetooth> section not found. Using default values for the device.\n");
			return ERR_OK;
		}
		while(root != NULL) {
			if(root->Attribute("id")) {
				XdevLID id(root->Attribute("id"));
				if(getID() == id) {
					if (root->Attribute("device")) {
						m_deviceName = root->Attribute("device");
					}
				}
				root = root->NextSiblingElement();
			} else
				XDEVL_MODULE_ERROR("No 'id' attribute specified. Using default values for the device\n");
		}
		return ERR_OK;
	}

	xdl::xdl_int XdevLBluetoothMacOSXImpl::open() {
		return open(m_deviceName);
	}

	xdl::xdl_int XdevLBluetoothMacOSXImpl::open(const XdevLFileName& host) {
		// If the user didn't specify the name of the device try to open
		// with the internal name wich is loaded by the xml file.
		if(NULL == host) {
			return open();
		}
		m_deviceName = host;

		BluetoothDeviceAddress *addr;
		CFStringRef str_addr;
		str_addr = CFStringCreateWithCString(kCFAllocatorDefault, m_deviceName.toString().c_str(), kCFStringEncodingMacRoman);
		IOBluetoothCFStringToDeviceAddress(str_addr, addr);
//	IOBluetoothDeviceRef dev = IOBluetoothDeviceCreateWithAddress(addr);

//	IOBluetoothDeviceOpenRFCOMMChannelAsync(device, &mRFCOMMChannelRef, rfcommChannelID, rfcommEventListener, NULL);

//	IOBluetoothDeviceOpenConnection(dev, &bluetoothCreateConnection_c, NULL);

		return ERR_OK;
	}

	xdl::xdl_int XdevLBluetoothMacOSXImpl::close() {


		return ERR_ERROR;
	}

	xdl_int XdevLBluetoothMacOSXImpl::write(xdl_uint8* src, xdl_int size) {
		xdl_int ret = -1;
		return ret;
	}

	xdl_int XdevLBluetoothMacOSXImpl::read(xdl_uint8* dst, xdl_int size) {
		xdl_int ret = -1;
		return ret;
	}

	xdl_int XdevLBluetoothMacOSXImpl::flush() {
		return ERR_OK;
	}

	xdl_int XdevLBluetoothMacOSXImpl::bind(const xdl_char* host) {
		return ERR_ERROR;
	}

	xdl_int XdevLBluetoothMacOSXImpl::listen(xdl::xdl_int backlog) {

		return 0;
	}

	xdl_int XdevLBluetoothMacOSXImpl::accept(XdevLBluetoothAddress** addr) {

		return 0;
	}

}
