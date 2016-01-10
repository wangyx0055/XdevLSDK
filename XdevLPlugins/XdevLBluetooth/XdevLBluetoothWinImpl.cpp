#include <XdevLBluetoothWinImpl.h>
#include <XdevLCoreMediator.h>
#include <XdevLPlatform.h>
#include <sstream>


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
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLBluetoothWinImpl, bluetoothModuleDescriptor)
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}

namespace xdl {

	int SDPGetPort( const char *addr, LPGUID guid ) {
		int port = 0;
		HANDLE h;
		WSAQUERYSET *qs;
		DWORD flags = 0;
		DWORD qs_len;
		bool done;
		qs_len = sizeof(WSAQUERYSET);
		qs = (WSAQUERYSET*) malloc( qs_len );
		ZeroMemory( qs, qs_len );
		qs->dwSize = sizeof(WSAQUERYSET);
		qs->lpServiceClassId = guid;
		qs->dwNameSpace = NS_BTH;
		qs->dwNumberOfCsAddrs = 0;
		qs->lpszContext = (LPSTR)addr;
		flags = LUP_FLUSHCACHE | LUP_RETURN_ADDR;
		if( SOCKET_ERROR == WSALookupServiceBegin( qs, flags, &h )) {
			ExitProcess(2);
		}
		done = false;
		while ( ! done ) {
			if( SOCKET_ERROR == WSALookupServiceNext( h, flags, &qs_len, qs ) ) {
				int error = WSAGetLastError();
				if( error == WSAEFAULT ) {
					free(qs);
					qs = (WSAQUERYSET*) malloc( qs_len );
				} else if (error == WSA_E_NO_MORE ) {
					done = true;
				} else {
					ExitProcess(2);
				}
			} else {
				port =
				  ((SOCKADDR_BTH*)qs->lpcsaBuffer->RemoteAddr.lpSockaddr)->port;
			}
		}
		free(qs);
		WSALookupServiceEnd( h );
		return port;
	}

	xdl_int XdevLBluetoothWinImpl::init() {
		if(readInfoFromXMLFile() != ERR_OK) {
			XDEVL_MODULE_WARNING("Parsing problems occurred of the Core XML file.\n");
		}
		if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0) {
			XDEVL_MODULE_ERROR("WSAStartup failed.\n");
			return ERR_ERROR;
		}

		return ERR_OK;
	}

	xdl_int XdevLBluetoothWinImpl::shutdown() {
		if(WSACleanup() == -1) {
			XDEVL_MODULE_ERROR(" WSACleanup failed.\n");
			return ERR_ERROR;
		}

		return ERR_OK;
	}

	xdl_int XdevLBluetoothWinImpl::readInfoFromXMLFile() {
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

		if(root->Attribute("id")) {
			XdevLID id(root->Attribute("id"));
			if(getID() != id) {
				return ERR_OK;
			}
		} else {
			XDEVL_MODULE_ERROR("No 'id' attribute specified.");
			return ERR_ERROR;
		}

		return ERR_OK;
	}

	xdl_int  XdevLBluetoothWinImpl::scan(const xdl_char* device) {
		return ERR_ERROR;
	}

	xdl::xdl_int XdevLBluetoothWinImpl::open() {
		return open(m_deviceName.c_str());
	}

	xdl::xdl_int XdevLBluetoothWinImpl::open(const XdevLFileName& host) {
		// If the user didn't specify the name of the device try to open
		// with the internal name wich is loaded by the xml file.
		if ("Not Specified" == host) {
			return ERR_ERROR;
		}
		m_deviceName = host;
		return open();
	}

	xdl::xdl_int XdevLBluetoothWinImpl::open(const xdl_char* name) {
//		DCB dcb;
//		COMMTIMEOUTS timeout;
#define DEFAULT_BUFLEN 512

		// Hard coded directly, got it from the receiver/server
		BT_ADDR addr;
		m_socket = socket (AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
		if (m_socket == INVALID_SOCKET) {
			printf ("Socket creation failed, error %d\n", WSAGetLastError());
			return ERR_ERROR;
		}


		WSAQUERYSET wsaq;
		ZeroMemory(&wsaq, sizeof(wsaq));
		wsaq.dwSize			 = sizeof(wsaq);
		wsaq.dwNameSpace = NS_BTH;
		wsaq.lpcsaBuffer = NULL;

		// Start the service.
		if( WSALookupServiceBegin(&wsaq, LUP_CONTAINERS, &m_handle) == SOCKET_ERROR) {
			XDEVL_MODULE_ERROR("WSALookupServiceBegin() failed: " << WSAGetLastError());
			return ERR_ERROR;
		}
		
		CHAR buf[4096];
		LPWSAQUERYSET results		= (LPWSAQUERYSET) buf;
		ZeroMemory(results, sizeof(WSAQUERYSET));
		results->dwSize					= sizeof(WSAQUERYSET);
		results->dwNameSpace		= NS_BTH;
		results->lpBlob					= NULL;

		DWORD  				dwSize = sizeof(buf);
		
		while (WSALookupServiceNext(m_handle, LUP_RETURN_NAME | LUP_RETURN_ADDR, &dwSize, results) == 0) {
			addr = ((SOCKADDR_BTH*)results->lpcsaBuffer->RemoteAddr.lpSockaddr)->btAddr;
			m_port = ((SOCKADDR_BTH*)results->lpcsaBuffer->RemoteAddr.lpSockaddr)->port;
			char buf[40] = {0};
			DWORD bufsize = sizeof(buf);
			WSAAddressToString( results->lpcsaBuffer->RemoteAddr.lpSockaddr, sizeof(SOCKADDR_BTH), NULL, buf, &bufsize);
			std::string device(buf);
			if(device == std::string(name)) {
				XDEVL_MODULE_INFO("Device: " << name << " found.");
			}
			std::cout << buf << " : " << results->lpszServiceInstanceName  << "\t" << results->lpcsaBuffer->LocalAddr.lpSockaddr << "\t" <<  results->dwNameSpace << std::endl;


			memset (&m_connection, 0, sizeof(m_connection));
			m_connection.addressFamily	= AF_BTH;
			m_connection.btAddr					= addr;
			m_connection.port						=  m_port;
			
			SOCKADDR_BTH sa = { 0 };
			int sa_len = sizeof(SOCKADDR_BTH);
			if( SOCKET_ERROR == WSAStringToAddress((char*)name, AF_BTH, NULL, (LPSOCKADDR)&sa, &sa_len ) ) {
				return ERR_ERROR;
			}



			if (::connect(m_socket, (LPSOCKADDR)&m_connection, sizeof(m_connection)) == -1 ){
					return ERR_ERROR;
			}


		}			
			
			
		// Stop the service.
		if( WSALookupServiceEnd(&m_handle) == SOCKET_ERROR) {
			XDEVL_MODULE_ERROR("WSALookupServiceBegin() failed: " << WSAGetLastError());
			return FALSE;
		}

		/*
			BLUETOOTH_DEVICE_SEARCH_PARAMS btSearchParams;

		    btSearchParams.dwSize = sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS);
		    btSearchParams.cTimeoutMultiplier = 5;  //5*1.28s search timeout
		    btSearchParams.fIssueInquiry = true;    //new inquiry

		    //return all known and unknown devices
		    btSearchParams.fReturnAuthenticated = true;
		    btSearchParams.fReturnConnected = true;
		    btSearchParams.fReturnRemembered = true;
		    btSearchParams.fReturnUnknown = true;

		    btSearchParams.hRadio = NULL;   //search on all local radios



		    BLUETOOTH_DEVICE_INFO btDeviceInfo;
		    ZeroMemory(&btDeviceInfo, sizeof(BLUETOOTH_DEVICE_INFO));   //"initialize"

		    btDeviceInfo.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);

		    HBLUETOOTH_DEVICE_FIND m_btDeviceFindHandle = NULL;

		    m_btDeviceFindHandle = BluetoothFindFirstDevice(&btSearchParams, &btDeviceInfo);
		 //   if(m_btDeviceFindHandle)
		  //  {

		//        m_authCallbackHandle = NULL;

		//        DWORD err = BluetoothRegisterForAuthenticationEx(&btDeviceInfo, &m_authCallbackHandle, &auth_callback_ex, NULL);


		//        if (err != ERROR_SUCCESS)
		//        {
		//            DWORD err = GetLastError();
		//            std::cout << "BluetoothRegisterForAuthentication Error" << err << std::endl;
		//					return false;
		//       }




			m_socket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);

			if (m_socket == -1) {
				XDEVL_MODULE_ERROR("Failed to get bluetooth socket!\n");
				return ERR_ERROR;
			}

			SOCKADDR_BTH address;
			address.addressFamily 	=	AF_BTH;
			//address.btAddr = 0;
			address.port 						= BT_PORT_ANY;
			GUID test = {0x00000003, 0x0000, 0x1000, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB};

		//	address.serviceClassId 	= RFCOMM_PROTOCOL_UUID;
			address.serviceClassId 	= test;

		//	int sa_len = sizeof(SOCKADDR_BTH);
		//	if( SOCKET_ERROR == WSAStringToAddress((char*)host, AF_BTH, NULL, (LPSOCKADDR) &address, &sa_len ) ) {
		//		return ERR_ERROR;
		//	}

		//	address.port = SDPGetPort(host, (LPGUID) &RFCOMM_PROTOCOL_UUID);
		//	if( address.port == 0 ) {
		//		return ERR_ERROR;
		//	}


			if (::connect(m_socket, (LPSOCKADDR)&address, sizeof(address)) == -1 ){
				return -1;
			}
			 */
		return ERR_OK;
	}

	xdl::xdl_int XdevLBluetoothWinImpl::close() {
//	CloseHandle(m_handle);
		closesocket(m_socket);

		return ERR_OK;
	}

	xdl_int XdevLBluetoothWinImpl::write(xdl_uint8* src, xdl_int size) {
//	DWORD number_bytes;
//	if(WriteFile(m_handle, src, size, &number_bytes, NULL) == 0)
//		return -1;
//
//	return number_bytes;
		xdl_int ret = 0;
		if ((ret = ::send(m_socket, (xdl_char*)src, size, 0)) == -1) {
			return -1;
		}
		return ret;
	}

	xdl_int XdevLBluetoothWinImpl::read(xdl_uint8* dst, xdl_int size) {
//	DWORD number_bytes;
//	if(ReadFile(m_handle, dst, size, &number_bytes, NULL) == 0)
//		return -1;
////	PurgeComm(m_handle, PURGE_RXCLEAR|PURGE_TXCLEAR);
//	return number_bytes;

		xdl_int ret = 0;
		if ((ret =  ::recv(m_socket, (xdl_char*)dst, size, 0)) == -1) {
			return -1;
		}
		return ret;
	}

	xdl_int  XdevLBluetoothWinImpl::flush() {
		return ERR_ERROR;
	}

	xdl_int XdevLBluetoothWinImpl::bind(const xdl_char* host) {
		memset(&m_connection,0,sizeof(SOCKADDR_BTH));

		m_connection.addressFamily=AF_BTH;
		//m_connection.serviceClassId=my_Guid;

		// Now we bind that socked to a port number.
		if (::bind(m_socket, (struct sockaddr *)&m_connection, sizeof(sockaddr_in)) == -1) {
			return ERR_ERROR;
		}
		return ERR_OK;

	}

	xdl_int XdevLBluetoothWinImpl::listen(xdl::xdl_int backlog) {
		if(::listen(m_socket, backlog) == -1)
			return -1;

		return 0;
	}

	xdl_int XdevLBluetoothWinImpl::accept(XdevLBluetoothAddress** addr) {
		SOCKADDR_BTH ca;
		int ca_len = sizeof(ca);

		SOCKET a_socket = ::accept(m_socket, (LPSOCKADDR) &ca, &ca_len );
		if(a_socket == -1)
			return -1;

		// Create a new connection info. Don't worry about deleting it. Will be done in
		// the close method.
		XdevLBluetoothAddressImpl* tmp_arr = new XdevLBluetoothAddressImpl(a_socket, ca);
		*addr = tmp_arr;
		m_acceptedSockets.push_back(tmp_arr);
		return 0;
	}

}
