#include <XdevLFTDI.h>
#include <XdevLCoreMediator.h>
#include <XdevLPlatform.h>
#include <sstream>
#include <XdevLXstring.h>

xdl::XdevLPluginDescriptor pluginDescriptor {
	xdl::pluginName,
	xdl::moduleNames,
	XDEVLFTDI_MAJOR_VERSION,
	XDEVLFTDI_MINOR_VERSION,
	XDEVLFTDI_PATCH_VERSION
};

xdl::XdevLModuleDescriptor moduleDescriptor {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[0],
	xdl::copyright,
	xdl::description,
	XDEVLFTDI_MODULE_MAJOR_VERSION,
	XDEVLFTDI_MODULE_MINOR_VERSION,
	XDEVLFTDI_MODULE_PATCH_VERSION
};


XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(pluginDescriptor);

XDEVL_PLUGIN_CREATE_MODULE {
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLFTDI, moduleDescriptor)
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}

namespace xdl {

	DWORD wrapXdevLStopBitsToFTDI(XdevLSerialStopBits stopBits) {
		switch(stopBits) {
			case SERIAL_SB_1:
				return FT_STOP_BITS_1;
			case SERIAL_SB_2:
			default:
				break;
		}
		return FT_STOP_BITS_2;
	}

	DWORD wrapXdevLByteSizeToFTDI(XdevLSerialByteSize byteSize) {
		switch(byteSize) {
			case SERIAL_BSIZE_5:
				XDEVL_ASSERT(nullptr, "SERIAL_BSIZE_5 is not supported by this device.");
				break;
			case SERIAL_BSIZE_6:
				XDEVL_ASSERT(nullptr, "SERIAL_BSIZE_6 is not supported by this device.");
				break;
			case SERIAL_BSIZE_7:
				return FT_BITS_7;
				break;
			case SERIAL_BSIZE_8:
			default:
				break;
		}
		return SERIAL_BSIZE_8;
	}

	DWORD wrapXdevLParityToFTDI(XdevLSerialParity parity) {
		switch(parity) {
			case SERIAL_EVEN_PARITY:
				return FT_PARITY_EVEN;
				break;
			case SERIAL_ODD_PARITY:
				return FT_PARITY_ODD;
				break;
			case SERIAL_SPACE_PARITY:
				return FT_PARITY_SPACE;
				break;
			case SERIAL_MARK_PARITY:
				return FT_PARITY_MARK;
			case SERIAL_NO_PARITY:
			default:
				break;
		}
		return FT_PARITY_NONE;
	}

	DWORD wrapXdevLFlowControlToFTDI(XdevLSerialFlowControl flowcontrol) {
		switch(flowcontrol) {
			case SERIAL_FLOW_CONTROL_HARDWARE:
				return FT_FLOW_RTS_CTS;
				break;
			case SERIAL_FLOW_CONTROL_SOFTWARE:
				return FT_FLOW_XON_XOFF;
				break;
			case SERIAL_FLOW_CONTROL_NONE:
			default:
				break;
		}
		return FT_FLOW_NONE;
	}

	XdevLFTDI::XdevLFTDI(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		XdevLModuleImpl<XdevLSerial>(parameter, descriptor),
		m_usbInSize(2048),
		m_usbOutSize(2048),
		m_latencyTimer(16) {
	};

	XdevLFTDI::~XdevLFTDI() {
	}

	xdl_int XdevLFTDI::init() {
		return ERR_OK;
	}

	xdl_int XdevLFTDI::shutdown() {
		return close();
	}

	xdl_int XdevLFTDI::readInfoFromXMLFile() {
		if(getMediator()->getXmlFilename() == nullptr)
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

		while(root != nullptr) {
			if(root->Attribute("id")) {
				XdevLID id(root->Attribute("id"));
				if(getID() == id) {
					if(root->Attribute("device")) {
						m_deviceName = XdevLString(root->Attribute("device"));
					}
					if(root->Attribute("usb_in_size")) {
						std::istringstream ss(root->Attribute("usb_in_size"));
						ss >> m_usbInSize;
					}
					if(root->Attribute("usb_out_size")) {
						std::istringstream ss(root->Attribute("usb_out_size"));
						ss >> m_usbOutSize;
					}
					if(root->Attribute("latency_timer")) {
						std::istringstream ss(root->Attribute("latency_timer"));
						ss >> m_latencyTimer;
					}
					if(root->Attribute("baudrate")) {
						std::istringstream ss(root->Attribute("baudrate"));
						ss >> m_baudrate;
					}
					if(root->Attribute("bytesize")) {
						m_byteSize = m_byteSizeMap[root->Attribute("bytesize")];
					}
					if(root->Attribute("stopbits")) {
						m_stopBits = m_stopBitsMap[root->Attribute("stopbits")];
					}
					if(root->Attribute("parity")) {
						m_parity = m_parityMap[root->Attribute("parity")];
					}
					if(root->Attribute("flowcontrol")) {
						m_flowControl = m_flowControlMap[root->Attribute("flowcontrol")];
					}
					if(root->Attribute("timeout")) {
						std::istringstream ss(root->Attribute("timeout"));
						ss >> m_timeout;
					}
					if(root->Attribute("xon")) {
						std::istringstream ss(root->Attribute("xon"));
						ss >> m_xon;
					}
					if(root->Attribute("xoff")) {
						std::istringstream ss(root->Attribute("xoff"));
						ss >> m_xoff;
					}
				}
			} else
				XDEVL_MODULE_ERROR("No 'id' attribute specified. Using default values for the device\n");

			root = root->NextSiblingElement();
		}
		return ERR_OK;
	}

	xdl::xdl_int XdevLFTDI::open() {

		// Parse XML file and set the serial port settings.
		if(readInfoFromXMLFile() != ERR_OK) {
			XDEVL_MODULE_WARNING("Parsing problems occurred of the Core XML file." << std::endl);
		}
		return openConnection();
	}

	xdl::xdl_int XdevLFTDI::open(const XdevLFileName& name) {

		// Parse XML file and set the serial port settings.
		if(readInfoFromXMLFile() != ERR_OK) {
			XDEVL_MODULE_WARNING("Parsing problems occurred of the Core XML file." << std::endl);
		}
		m_deviceName = name;
		return openConnection();
	}

	xdl::xdl_int XdevLFTDI::open(const XdevLFileName& name, const XdevLDeviceModes& mode) {
		// Parse XML file and set the serial port settings.
		if(readInfoFromXMLFile() != ERR_OK) {
			XDEVL_MODULE_WARNING("Parsing problems occurred of the Core XML file." << std::endl);
		}

		m_deviceName = name;
		return openConnection();
	}

	xdl::xdl_int XdevLFTDI::openConnection() {
		XDEVL_MODULE_INFO("Checking for FTDI devices." << currentDevice.serial << std::endl);

		FT_STATUS ftStatus;
		DWORD iNumDevs = 0;

		// Lets get the number of all connected devices.
		ftStatus = FT_CreateDeviceInfoList(&iNumDevs);
		if(ftStatus != FT_OK) {
			printf("Error: FT_CreateDeviceInfoList(&iNumDevs)\n");
			return -1;
		}

		// Now, lets get information about those connected devices.
		for(xdl_uint i = 0; i < iNumDevs; i++) {
			XdevLFTDIDeviceInfo deviceInfo;
			ftStatus = FT_GetDeviceInfoDetail(i, deviceInfo.flags, deviceInfo.type, deviceInfo.id, deviceInfo.locId, deviceInfo.serial, deviceInfo.desc, &deviceInfo.handle);
			XDEVL_MODULE_INFO("--------------\n");
			XDEVL_MODULE_INFO("Found device : " << deviceInfo.serial << " (Serial Number)" << std::endl);
			XDEVL_MODULE_INFO("Name         : " << deviceInfo.desc << std::endl);
			XDEVL_MODULE_INFO("--------------\n");

			m_foundDevices.push_back(deviceInfo);
		}
		XDEVL_MODULE_INFO("------------------------------------------------------------\n");

		//
		// Go through all devices and check if we find the one the user specified
		//
		xdl_bool deviceFound = xdl_false;
		for(auto& device : m_foundDevices) {
			if(strcmp(m_deviceName.toString().c_str(), (const xdl_char*)device.serial) == 0) {
				deviceFound = xdl_true;
				currentDevice = device;
				currentDevice.usbInSize = m_usbInSize;
				currentDevice.usbOutSize = m_usbOutSize;
			}
		}
		if(xdl_false == deviceFound) {
			XDEVL_MODULE_ERROR("Specified device: " << m_deviceName << " not found. For FTDI devices use the serial number you can see above.\n");
			return ERR_ERROR;
		}

		XDEVL_MODULE_INFO("Opening connection to the Serial Port device: " << currentDevice.serial << std::endl);

		//
		// Open connection to the device
		//
		ftStatus = FT_OpenEx(currentDevice.serial, FT_OPEN_BY_SERIAL_NUMBER, &currentDevice.handle);
		if(ftStatus != FT_OK) {
			XDEVL_MODULE_ERROR("This can fail if the ftdi_sio driver is loaded use lsmod to check this and rmmod ftdi_sio to remove also rmmod usbserial");
			XDEVL_MODULE_ERROR("Or specified device: " << m_deviceName << " not found. For FTDI devices use the serial number you can see above.\n");
			return ERR_ERROR;
		}
		XDEVL_MODULE_INFO("Connection opened successfully." << std::endl);

		//
		// Reset device
		//
		ftStatus = FT_ResetDevice(currentDevice.handle);
		if (ftStatus != FT_OK) {
			XDEVL_MODULE_ERROR("FT_ResetDevice failed: " << (xdl_int)ftStatus << std::endl);
			return ERR_ERROR;
		}

		return setStates(m_baudrate, m_byteSize, m_parity, m_stopBits, m_flowControl, m_timeout);
	}


	xdl_int XdevLFTDI::setStates(xdl_int baudrate, XdevLSerialByteSize bytesize, XdevLSerialParity parity, XdevLSerialStopBits stopbits, XdevLSerialFlowControl flowcontrol, xdl_int timeout) {
		XDEVL_MODULE_INFO("Setting Serial Port connection attributes." << std::endl)

		// Set the internal values.
		XdevLSerialImpl::setStates(baudrate, bytesize, parity, stopbits, flowcontrol, timeout);

		FT_STATUS	ftStatus;

		//
		// Set the baudrate
		//
		if((ftStatus = FT_SetBaudRate(currentDevice.handle, baudrate)) != FT_OK) {
			XDEVL_MODULE_ERROR("FT_SetBaudRate failed: " << (xdl_int)ftStatus << "\n");
			return ERR_ERROR;
		}

		//
		// Wrap the stop bits.
		//
		DWORD sbits = wrapXdevLStopBitsToFTDI(stopbits);

		//
		// Wrap the byte size.
		//
		DWORD bsize = wrapXdevLByteSizeToFTDI(bytesize);

		//
		// Wrap the parity.
		//
		DWORD sparity = wrapXdevLParityToFTDI(parity);

		//
		// Set byte size, stop bits and parity.
		//
		ftStatus = FT_SetDataCharacteristics(currentDevice.handle, bsize, sbits, sparity);
		if(ftStatus != FT_OK) {
			XDEVL_MODULE_ERROR("FT_SetDataCharacteristics failed: " << (xdl_int)ftStatus << "\n");
			return ERR_ERROR;
		}
		XDEVL_MODULE_INFO("--------------\n");
		XDEVL_MODULE_INFO("Baudrate           : " << baudrate << std::endl);
		XDEVL_MODULE_INFO("Stop bits          : " << xdevLStopBitsToString(stopbits) << std::endl);
		XDEVL_MODULE_INFO("Byte size          : " << xdevLByteSizeToString(bytesize) << std::endl);
		XDEVL_MODULE_INFO("Parity             : " << xdevLParityToString(parity) << std::endl);

		//
		// Set the flow control.
		//
		DWORD fc = wrapXdevLFlowControlToFTDI(flowcontrol);

		ftStatus = FT_SetFlowControl(currentDevice.handle, fc, m_xon, m_xoff);
		if(ftStatus != FT_OK) {
			XDEVL_MODULE_ERROR("FT_SetFlowControl failed: " << (xdl_int)ftStatus << "\n");
			return ERR_ERROR;
		}
		XDEVL_MODULE_INFO("Flowcontrol        : " << xdevLFlowControlToString(flowcontrol) << std::endl);

		//
		// Set USB buffer parameters. (IN/OUT buffer size)
		//
		if((ftStatus = FT_SetUSBParameters(currentDevice.handle, currentDevice.usbInSize, currentDevice.usbOutSize)) != FT_OK) {
			XDEVL_MODULE_ERROR("FT_SetUSBParameters failed: " << (xdl_int)ftStatus << "\n");
			return ERR_ERROR;
		}
		XDEVL_MODULE_INFO("USB transfer size  : IN: " << currentDevice.usbInSize << ": OUT: " << currentDevice.usbOutSize << " bytes." << std::endl);


		//
		// Set the latency timer for better latency :D
		//
		if((ftStatus = FT_SetLatencyTimer(currentDevice.handle, m_latencyTimer)) != FT_OK) {
			XDEVL_MODULE_ERROR("FT_SetLatencyTimer failed: " << (xdl_int)ftStatus << "\n");
			return ERR_ERROR;
		}
		XDEVL_MODULE_INFO("Latency timer      : " << m_latencyTimer << " ms" << std::endl);
		XDEVL_MODULE_INFO("--------------\n");

		//
		// Set the time out times for reading and writing.
		//
		if(timeout != 0) {
			ftStatus = FT_SetTimeouts(currentDevice.handle, m_timeout, m_timeout/1000000);
			if(ftStatus != FT_OK) {
				XDEVL_MODULE_ERROR("Could not set the timeout.\n");
			} else {
				XDEVL_MODULE_INFO("Read/Write timeout : " <<  m_timeout/1000000 << " s" << std::endl);
			}
		}

		XDEVL_MODULE_SUCCESS("Setting Serial Port Protocol values was successful.\n")
		return ERR_OK;
	}

	xdl::xdl_int XdevLFTDI::close() {
		XDEVL_MODULE_INFO("Closing connection to Serial Port: '" << m_deviceName << "'" << std::endl);

		// Check if we have a valid handle.
		if(currentDevice.handle != nullptr) {
			FT_Close(currentDevice.handle);
		}

		XDEVL_MODULE_SUCCESS("Connection to Serial Port closed successful." << std::endl);
		return ERR_OK;
	}

	xdl_int XdevLFTDI::write(xdl_uint8* src, xdl_int size) {
		XDEVL_ASSERT(currentDevice.handle, "It seems like the device wasn't created successfully.");

		DWORD 	dwBytesWrote;
		FT_STATUS ftStatus = FT_Write(currentDevice.handle, src, size, &dwBytesWrote);
		if(ftStatus != FT_OK) {
			XDEVL_MODULE_ERROR("FT_Write error: " << ftStatus << "\n");
			return -1;
		}

		return dwBytesWrote;
	}

	xdl_int XdevLFTDI::read(xdl_uint8* dst, xdl_int size) {
		XDEVL_ASSERT(currentDevice.handle, "It seems like the device wasn't created successfully.");

		DWORD 	dwBytesRead;
		FT_STATUS ftStatus = FT_Read(currentDevice.handle, dst, size, &dwBytesRead);
		if(ftStatus != FT_OK) {
			XDEVL_MODULE_ERROR("FT_Read error: " << ftStatus << "\n");
			return -1;
		}

		return dwBytesRead;

	}

	xdl_int XdevLFTDI::waiting() {
		XDEVL_ASSERT(currentDevice.handle, "It seems like the device wasn't created successfully.");

		DWORD RxBytes;
		FT_STATUS ftStatus = FT_GetQueueStatus(currentDevice.handle,&RxBytes);
		if(ftStatus != FT_OK) {
			XDEVL_MODULE_ERROR("FT_GetQueueStatus error: " << ftStatus << "\n");
			return -1;
		}

		return RxBytes;
	}

	xdl_int XdevLFTDI::flush() {
		XDEVL_ASSERT(currentDevice.handle, "It seems like the device wasn't created successfully.");

		FT_STATUS ftStatus = FT_Purge(currentDevice.handle, FT_PURGE_RX | FT_PURGE_TX);
		if(ftStatus != FT_OK) {
			return ERR_ERROR;
		}

		return ERR_OK;
	}


}
