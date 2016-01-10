#include <XdevLBluetoothMacOSXImpl.h>
#include <XdevLCoreMediator.h>
#include <XdevLPlatform.h>

xdl::XdevLPluginDescriptor bluetoothPluginDescriptor {
	xdl::XdevLString("XdevLBluetooth"),
	xdl::moduleNames,
	XDEVLBLUETOOTH_MAJOR_VERSION,
	XDEVLBLUETOOTH_MINOR_VERSION,
	XDEVLBLUETOOTH_PATCH_VERSION
};

xdl::XdevLModuleDescriptor bluetoothModuleDescriptor {
	xdl::XdevLString("www.codeposer.net"),
	xdl::XdevLString("Cengiz Terzibas"),
	xdl::moduleNames[0],
	xdl::XdevLString("(c) 2005 - 2016 Cengiz Terzibas."),
	xdl::XdevLString("Creates a Bluetooth connection."),
	XDEVLBLUETOOTH_MODULE_MAJOR_VERSION,
	XDEVLBLUETOOTH_MODULE_MINOR_VERSION,
	XDEVLBLUETOOTH_MODULE_PATCH_VERSION
};

XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(bluetoothPluginDescriptor);

XDEVL_PLUGIN_CREATE_MODULE {
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLBluetoothMacOSXImpl, bluetoothModuleDescriptor)
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}

namespace xdl {

	XdevLBluetoothMacOSXImpl::XdevLBluetoothMacOSXImpl(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		XdevLModuleImpl<XdevLBluetooth>(parameter, descriptor) {};

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
