#include <XdevLSerialWin.h>
#include <XdevLCoreMediator.h>
#include <XdevLPlatform.h>
#include <XdevLXstring.h>
#include <sstream>

xdl::XdevLModuleDescriptor xdl::XdevLSerialWin::m_serialModuleDescriptor {
	vendor,
	author,
	moduleNames[0],
	copyright,
	description,
	XdevLSerialMajorVersion,
	XdevLSerialMinorVersion,
	XdevLSerialPatchVersion
};

xdl::XdevLPluginDescriptor serialPluginDescriptor {
	xdl::pluginName,
	xdl::moduleNames,
	xdl::XdevLSerialPluginMajorVersion,
	xdl::XdevLSerialPluginMinorVersion,
	xdl::XdevLSerialPluginPatchVersion
};


XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(serialPluginDescriptor);

XDEVL_PLUGIN_CREATE_MODULE {
	if (xdl::XdevLSerialWin::m_serialModuleDescriptor.getName() == XDEVL_MODULE_PARAMETER_NAME) {

		xdl::IPXdevLModule module = XDEVL_NEW_MODULE(xdl::XdevLSerialWin,  XDEVL_MODULE_PARAMETER);
		XDEVL_MODULE_SET_MODULE_INSTACE(module);

		return xdl::ERR_OK;
	}
	return xdl::ERR_MODULE_NOT_FOUND;
}

namespace xdl {

	XdevLSerialWin::XdevLSerialWin(XdevLModuleCreateParameter* parameter) :
		XdevLModuleImpl<XdevLSerial>(parameter, m_serialModuleDescriptor),
		m_handle(INVALID_HANDLE_VALUE) {};

	xdl_int XdevLSerialWin::init() {
		if(readInfoFromXMLFile() != ERR_OK) {
			XDEVL_MODULE_WARNING("Parsing problems occurred of the Core XML file.\n");
		}
		return ERR_OK;
	}

	xdl_int XdevLSerialWin::shutdown() {
		return close();
	}

	xdl_int XdevLSerialWin::readInfoFromXMLFile() {
		if(getMediator()->getXmlFilename() == NULL)
			return ERR_ERROR;

		TiXmlDocument xmlDocument;
		if(!xmlDocument.LoadFile(getMediator()->getXmlFilename())) {
			XDEVL_MODULE_ERROR("Could not parse xml file: " << getMediator()->getXmlFilename() << std::endl);
			return ERR_ERROR;
		}

		TiXmlHandle docHandle(&xmlDocument);
		TiXmlElement* root = docHandle.FirstChild(XdevLCorePropertiesName.c_str()).FirstChildElement("XdevLSerial").ToElement();
		if(!root) {
			XDEVL_MODULE_WARNING("<XdevLSerial> section not found. Using default values for the device.\n");
			return ERR_OK;
		}

		while(root != NULL) {
			if(root->Attribute("id")) {
				XdevLID id(root->Attribute("id"));
				if(getID() == id) {
					if (root->Attribute("device")) {
						m_deviceName = root->Attribute("device");
					}
					if (root->Attribute("mode")) {
						m_deviceMode = m_deviceModeMap[root->Attribute("mode")];
					}
					if (root->Attribute("baudrate")) {
						std::istringstream ss(root->Attribute("baudrate"));
						ss >> m_baudrate;
					}
					if (root->Attribute("bytesize")) {
						m_byteSize = m_byteSizeMap[root->Attribute("bytesize")];
					}
					if (root->Attribute("stopbits")) {
						m_stopBits = m_stopBitsMap[root->Attribute("stopbits")];
					}
					if (root->Attribute("parity")) {
						m_parity = m_parityMap[root->Attribute("parity")];
					}
					if(root->Attribute("flowcontrol")) {
						m_flowControl = m_flowControlMap[root->Attribute("flowcontrol")];
					}
					if (root->Attribute("timeout")) {
						std::istringstream ss(root->Attribute("timeout"));
						ss >> m_timeout;
					}
				}
			} else
				XDEVL_MODULE_ERROR("No 'id' attribute specified. Using default values for the device\n");

			root = root->NextSiblingElement();
		}
		return ERR_OK;
	}

	xdl::xdl_int XdevLSerialWin::SetTimeOut(xdl_int timeout) {
		COMMTIMEOUTS tout;

		if(GetCommTimeouts(m_handle, &tout) == 0) {
			LPVOID lpMsgBuf;
			FormatMessage(
			    FORMAT_MESSAGE_ALLOCATE_BUFFER |
			    FORMAT_MESSAGE_FROM_SYSTEM |
			    FORMAT_MESSAGE_IGNORE_INSERTS,
			    NULL,
			    GetLastError(),
			    0, // Default language
			    (LPTSTR) &lpMsgBuf,
			    0,
			    NULL
			);
			XDEVL_MODULE_ERROR("GetCommTimeouts failed: " << (xdl_char*)lpMsgBuf << "\n");
			// Free the buffer.
			LocalFree( lpMsgBuf );
			return ERR_ERROR;
		}
		XDEVL_MODULE_INFO("ReadIntervalTimeout: " << tout.ReadIntervalTimeout << std::endl);
		XDEVL_MODULE_INFO("ReadTotalTimeoutConstant: " << tout.ReadTotalTimeoutConstant << std::endl);
		XDEVL_MODULE_INFO("ReadTotalTimeoutMultiplier: " << tout.ReadTotalTimeoutMultiplier << std::endl);
		XDEVL_MODULE_INFO("WriteTotalTimeoutConstant: " << tout.WriteTotalTimeoutConstant << std::endl);
		XDEVL_MODULE_INFO("WriteTotalTimeoutMultiplier: " << tout.WriteTotalTimeoutMultiplier << std::endl);

		// The maximum amount of time allowed to pass between
		// the arrival of two bytes.(milliseconds)
		tout.ReadIntervalTimeout					=	timeout;
		tout.ReadTotalTimeoutConstant			=	timeout;
		tout.ReadTotalTimeoutMultiplier		=	0;
		tout.WriteTotalTimeoutConstant		=timeout;
		tout.WriteTotalTimeoutMultiplier	=	0;

		if(SetCommTimeouts(m_handle, &tout) == 0) {
			LPVOID lpMsgBuf;
			FormatMessage(
			    FORMAT_MESSAGE_ALLOCATE_BUFFER |
			    FORMAT_MESSAGE_FROM_SYSTEM |
			    FORMAT_MESSAGE_IGNORE_INSERTS,
			    NULL,
			    GetLastError(),
			    0, // Default language
			    (LPTSTR) &lpMsgBuf,
			    0,
			    NULL
			);
			XDEVL_MODULE_ERROR("GetCommTimeouts failed: " << (xdl_char*)lpMsgBuf << "\n");
			// Free the buffer.
			LocalFree( lpMsgBuf );
			return ERR_ERROR;
		}
		m_timeout = timeout;
		return ERR_OK;
	}


	xdl::xdl_int XdevLSerialWin::open() {

		// Parse XML file and set the serial port settings.
		if(readInfoFromXMLFile() != ERR_OK) {
			XDEVL_MODULE_WARNING("Parsing problems occurred of the Core XML file.\n");
		}
		return _open();
	}

	xdl_int XdevLSerialWin::open(const XdevLFileName& name) {
		// Parse XML file and set the serial port settings.
		if (readInfoFromXMLFile() != ERR_OK) {
			XDEVL_MODULE_WARNING("Parsing problems occurred of the Core XML file." << std::endl);
		}
		m_deviceName = name;
		return _open();
	}

	xdl::xdl_int XdevLSerialWin::open(const xdl_char* name) {
		// Parse XML file and set the serial port settings.
		if(readInfoFromXMLFile() != ERR_OK) {
			XDEVL_MODULE_WARNING("Parsing problems occurred of the Core XML file.\n");
		}
		m_deviceName 	= name;
		return _open();
	}

	xdl::xdl_int XdevLSerialWin::_open() {
		XDEVL_MODULE_INFO("Open Serial Port connection.\n");

		// Create the name
		std::string sport = "\\\\.\\";
		sport += m_deviceName.toString();

		DWORD 	desiredAccess = GENERIC_READ | GENERIC_WRITE;
		DWORD	creationDisposition  = OPEN_EXISTING;

		m_handle = CreateFile(sport.c_str(), desiredAccess, 0, 0, creationDisposition, 0, 0);
		if(m_handle == INVALID_HANDLE_VALUE) {
			XDEVL_MODULE_ERROR("connect failed: " << strerror(errno) << "\n");
			return ERR_ERROR;
		} else {
			XDEVL_MODULE_SUCCESS("connect established to: " << m_deviceName << "\n");
		}
		return setStates(m_baudrate, m_byteSize, m_parity, m_stopBits, m_flowControl, m_timeout);
	}

	xdl_int XdevLSerialWin::setStates(xdl_int baudrate, XdevLSerialByteSize bytesize, XdevLSerialParity parity, XdevLSerialStopBits stopbits, XdevLSerialFlowControl flowcontrol, xdl_int timeout) {
		// Set the internal values.
		XdevLSerialImpl::setStates(baudrate, bytesize, parity, stopbits, flowcontrol, timeout);

		// Reset the DCB structure.
		DCB dcb = {0};

		// Set the length of the DCB buffer.
		dcb.DCBlength = sizeof (DCB);

		// Get the default port setting information.
		if(GetCommState(m_handle, &dcb) == 0) {
			printErrorMessage();
		}

		// Set the baudrate.
		dcb.BaudRate 	=	baudrate;														// Baudrate
		dcb.fParity 	= TRUE;              		 							// Enable parity checking

		switch(flowcontrol) {
			case SERIAL_FLOW_CONTROL_NONE:
				dcb.fDtrControl 	= DTR_CONTROL_DISABLE; 				// DTR flow control type
				dcb.fRtsControl		= RTS_CONTROL_DISABLE;				// Request To Send control mode
				dcb.fOutxCtsFlow 	= FALSE;         							// No Clear to Send output flow control
				dcb.fOutxDsrFlow 	= FALSE; 											// No Data set Ready flow control
				dcb.fOutX 				= FALSE;               		 		// No XON/XOFF out flow control
				dcb.fInX 					= FALSE;                 			// No XON/XOFF in flow control
				break;
			case SERIAL_FLOW_CONTROL_HARDWARE:
				dcb.fDtrControl 	= DTR_CONTROL_HANDSHAKE; 			// DTR flow control type
				dcb.fRtsControl		= DTR_CONTROL_HANDSHAKE;			// Request To Send control mode
				dcb.fOutxCtsFlow 	= FALSE;         							// No Clear to Send output flow control
				dcb.fOutxDsrFlow 	= FALSE; 											// No Data set Ready flow control
				dcb.fOutX 				= FALSE;               		 		// No XON/XOFF out flow control
				dcb.fInX 					= FALSE;                 			// No XON/XOFF in flow control
				break;
			case SERIAL_FLOW_CONTROL_SOFTWARE:
				dcb.fDtrControl 	= DTR_CONTROL_DISABLE; 				// DTR flow control type
				dcb.fRtsControl		= RTS_CONTROL_DISABLE;				// Request To Send control mode
				dcb.fOutX 				= TRUE;               		 		// XON/XOFF out flow control
				dcb.fInX 					= TRUE;                 			// XON/XOFF in flow control
				break;
			default:
				break;
		}

		dcb.fErrorChar = FALSE;           		// Disable error replacement
		dcb.fNull = FALSE;                		// Disable null stripping

//	dcb.fBinary = TRUE;               		// Binary mode; no EOF check
//	dcb.fOutxDsrFlow = FALSE;        		 	// No DSR output flow control
//	dcb.fDsrSensitivity = FALSE;      		// DSR sensitivity
//	dcb.fTXContinueOnXoff = TRUE;     		// XOFF continues Tx
//	dcb.fAbortOnError = FALSE;        		// Do not abort reads/writes on error


		switch(parity) {
			case SERIAL_EVEN_PARITY:
				dcb.Parity = EVENPARITY;
				break;
			case SERIAL_MARK_PARITY:
				dcb.Parity = MARKPARITY;
				break;
			case SERIAL_ODD_PARITY:
				dcb.Parity = ODDPARITY;
				break;
			case SERIAL_SPACE_PARITY:
				dcb.Parity = SPACEPARITY;
				break;
			case SERIAL_NO_PARITY:
			default:
				dcb.Parity = NOPARITY;
		}

		switch(stopbits) {
			case SERIAL_SB_2:
				dcb.StopBits = TWOSTOPBITS;
				break;
			case SERIAL_SB_1:
			default:
				dcb.StopBits = ONESTOPBIT;
				break;
		}

		switch(bytesize) {
			case SERIAL_BSIZE_5:
				dcb.ByteSize = 5;
				break;
			case SERIAL_BSIZE_6:
				dcb.ByteSize = 6;
				break;
			case SERIAL_BSIZE_7:
				dcb.ByteSize = 7;
				break;
			case SERIAL_BSIZE_8:
			default:
				dcb.ByteSize = 8;
				break;
		}

		if(SetCommState(m_handle, &dcb) == 0) {
			printErrorMessage();
			return ERR_ERROR;
		}

		SetTimeOut(timeout);

		return ERR_OK;
	}

	xdl::xdl_int XdevLSerialWin::close() {
		if(m_handle == INVALID_HANDLE_VALUE) {
			return ERR_ERROR;
		}
		if(CloseHandle(m_handle) == 0) {
			printErrorMessage();
			return ERR_ERROR;
		}
		return ERR_OK;
	}

	xdl_int XdevLSerialWin::write(xdl_uint8* src, xdl_int size) {
		DWORD number_bytes = 0;
		if(WriteFile(m_handle, src, size, &number_bytes, NULL) == 0) {
			errno = GetLastError();
			return -1;
		}
		return number_bytes;
	}

	xdl_int XdevLSerialWin::read(xdl_uint8* dst, xdl_int size) {
		DWORD number_bytes = 0;
		if(ReadFile(m_handle, dst, size, &number_bytes, NULL) == 0) {
			errno = GetLastError();
			return -1;
		}
		return number_bytes;
	}

	xdl_int  XdevLSerialWin::flush() {
		return ERR_ERROR;
	}

	xdl_int XdevLSerialWin::waiting() {
		xdl_int bytes_avail = 0;
		// TODO Implement how many bytes are waiting at the comport.
		return bytes_avail;
	}

	xdl_int XdevLSerialWin::flush() const {
		PurgeComm(m_handle, PURGE_RXCLEAR | PURGE_TXCLEAR );
		return ERR_OK;
	}

	void XdevLSerialWin::printErrorMessage() {
		LPVOID lpMsgBuf;
		FormatMessage(
		    FORMAT_MESSAGE_ALLOCATE_BUFFER |
		    FORMAT_MESSAGE_FROM_SYSTEM |
		    FORMAT_MESSAGE_IGNORE_INSERTS,
		    NULL,
		    GetLastError(),
		    0, // Default language
		    (LPTSTR) &lpMsgBuf,
		    0,
		    NULL
		);
		XDEVL_MODULE_ERROR((char*)lpMsgBuf << "\n");
		// Free the buffer.
		LocalFree( lpMsgBuf );
	}

}
