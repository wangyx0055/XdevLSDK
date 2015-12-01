#include <XdevLBluetoothLinuxImpl.h>
#include <XdevLCoreMediator.h>
#include <XdevLPlatform.h>
#include <cstring>


xdl::XdevLModuleDescriptor xdl::XdevLBluetoothLinuxImpl::m_bluetoothModuleDescriptor{	vendor, 
																																													author, 
																																													moduleNames[0],
																																													copyright, 
																																													description, 
																																													XdevLBluetoothMajorVersion, 
																																													XdevLBluetoothMinorVersion,
																																													XdevLBluetoothPatchVersion};
																																			
xdl::XdevLPluginDescriptor m_bluetoothPluginDescriptor{xdl::pluginName,
																													xdl::moduleNames,
																													xdl::XdevLBluetoothPluginMajorVersion,
																													xdl::XdevLBluetoothPluginMinorVersion,
																													xdl::XdevLBluetoothPluginPatchVersion};

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	if(xdl::XdevLBluetoothLinuxImpl::m_bluetoothModuleDescriptor.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLBluetoothLinuxImpl(parameter);
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
	return &m_bluetoothPluginDescriptor;
}

namespace xdl {

 
XdevLBluetoothLinuxImpl::XdevLBluetoothLinuxImpl(XdevLModuleCreateParameter* parameter) : XdevLModuleImpl<XdevLBluetooth>(parameter, m_bluetoothModuleDescriptor), 
                                                                                          m_socket(-1) {};  
  
xdl_int XdevLBluetoothLinuxImpl::init() {
	if(readInfoFromXMLFile() != ERR_OK) {
		XDEVL_MODULE_WARNING("Parsing problems occurred of the Core XML file.\n");
	}

	return ERR_OK;
}

xdl_int XdevLBluetoothLinuxImpl::shutdown() {
	return close();
}

xdl_int XdevLBluetoothLinuxImpl::readInfoFromXMLFile() {
	if(getMediator()->getXmlFilename() == XdevLFileName())
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
	while(root != NULL){
		if(root->Attribute("id")) {
			XdevLID id(root->Attribute("id"));
			if(getID() == id) {
				if (root->Attribute("device")) {
						m_deviceName = root->Attribute("device");
				}
			}
			root = root->NextSiblingElement();
		}else
				XDEVL_MODULE_ERROR("No 'id' attribute specified. Using default values for the device\n");
	}
	return ERR_OK;
}

xdl_int XdevLBluetoothLinuxImpl::scan(const xdl_char* device) {
	inquiry_info *ii = NULL;
	int max_rsp, num_rsp;
	int dev_id, sock, len, flags;
	int i;
	char addr[19] = { 0 };
	char name[248] = { 0 };
	dev_id = hci_get_route(NULL);
	sock = hci_open_dev( dev_id );
	if (dev_id < 0 || sock < 0) {
		return ERR_ERROR;
	}

	len = 8;
	max_rsp = 255;
	flags = IREQ_CACHE_FLUSH;
	ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));
	num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
	if( num_rsp < 0 )
		perror("hci_inquiry");

	
	for (i = 0; i < num_rsp; i++) {
		ba2str(&(ii+i)->bdaddr, addr);
		memset(name, 0, sizeof(name));
		if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), name, 0) < 0){
			strcpy(name, "[unknown]");
		}
		m_scanedDevices += std::string(addr) + "," + std::string(name) + ",";
			
		std::cout << m_scanedDevices << std::endl;
	}

	free( ii );

	if(::close( sock ) == -1)
		return ERR_ERROR;

	return ERR_OK;
}

xdl::xdl_int XdevLBluetoothLinuxImpl::open() {
	return open(m_deviceName);
}

xdl::xdl_int XdevLBluetoothLinuxImpl::open(const XdevLFileName& host) {
	// If the user didn't specify the name of the device try to open
	// with the internal name wich is loaded by the xml file.
	if(XdevLFileName("Not Specified") == host) {
		return open();
	}

	m_socket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	if(m_socket == -1) {
		XDEVL_MODULE_ERROR("Could not create bluetooth socket.\n")
		return ERR_ERROR;
	}

	struct sockaddr_rc addr = {0};
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = 1;
	str2ba(host, &addr.rc_bdaddr);

	if(::connect(m_socket, (struct sockaddr*)&addr, sizeof(sockaddr_rc)) == -1) {
		XDEVL_MODULE_ERROR("connect failed: " << strerror(errno) << "\n");
		return ERR_ERROR;
	}

	return ERR_OK;
}

xdl::xdl_int XdevLBluetoothLinuxImpl::close() {
	if(m_socket == -1) {
		return ERR_OK;
	}
	
	if(m_acceptedSockets.size() > 0) {
		for(size_t a = 0; a < m_acceptedSockets.size(); a++) {
			if(m_acceptedSockets[a])
				delete m_acceptedSockets[a];
		}
	}
	if(::close(m_socket) == -1)
		return ERR_ERROR;

	return ERR_OK;
}

xdl_int XdevLBluetoothLinuxImpl::write(xdl_uint8* src, xdl_int size) {
	xdl_int ret = 0;
	if ((ret = ::send(m_socket, (xdl_char*)src, size, 0)) == -1) {
		return -1;
	}
	return ret;
}

xdl_int XdevLBluetoothLinuxImpl::read(xdl_uint8* dst, xdl_int size) {
	xdl_int ret = 0;
	if ((ret =  ::recv(m_socket, (xdl_char*)dst, size, 0)) == -1) {
		return -1;
	}
	return ret;
}

xdl_int XdevLBluetoothLinuxImpl::flush() {
	return ERR_OK;
}

xdl_int XdevLBluetoothLinuxImpl::bind(const xdl_char* host) {
	struct sockaddr_rc addr = {0};
	addr.rc_family 	= AF_BLUETOOTH;
	addr.rc_channel = (xdl::xdl_uint8)1;

	if (::bind(m_socket, (struct sockaddr *)&addr, sizeof(sockaddr_in)) == -1) {
		return ERR_ERROR;
	}
	return ERR_OK;

}

xdl_int XdevLBluetoothLinuxImpl::listen(xdl::xdl_int backlog) {
	if(::listen(m_socket, backlog) == -1)
		return -1;

	return 0;
}

xdl_int XdevLBluetoothLinuxImpl::accept(XdevLBluetoothAddress** addr) {

	struct sockaddr_rc tmp = {0};
	xdl::xdl_uint len = sizeof(tmp);
	xdl::xdl_int a_socket = ::accept(m_socket,(struct sockaddr *)&tmp, &len);
	if(a_socket == -1)
		return -1;

	// Create a new connection info. Don't worry about deleting it. Will be done in
	// the close method.
	XdevLBluetoothAddressImpl* tmp_arr = new XdevLBluetoothAddressImpl(a_socket, tmp);
	*addr = tmp_arr;
	m_acceptedSockets.push_back(tmp_arr);
	return 0;
}

}
