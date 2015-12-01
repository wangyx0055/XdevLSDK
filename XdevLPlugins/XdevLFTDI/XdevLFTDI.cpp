#include <XdevLFTDI.h>
#include <XdevLCoreMediator.h>
#include <XdevLPlatform.h>
#include <sstream>
#include <XdevLXstring.h>


xdl::XdevLModuleDescriptor xdl::XdevLFTDILinuxImpl::m_moduleDescriptor{vendor, 
																																						author, 
																																						moduleNames[0],
																																						copyright, 
																																						description, 
																																						XdevLSerialMajorVersion, 
																																						XdevLSerialMinorVersion,
																																						XdevLSerialPatchVersion};
																																			
xdl::XdevLPluginDescriptor pluginDescriptor{	
																					xdl::pluginName,
																					xdl::moduleNames,
																					xdl::XdevLSerialPluginMajorVersion,
																					xdl::XdevLSerialPluginMinorVersion,
																					xdl::XdevLSerialPluginPatchVersion
																				};
																								

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	if(xdl::XdevLFTDILinuxImpl::m_moduleDescriptor.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLFTDILinuxImpl(parameter);
		if(!obj)
			return xdl::ERR_ERROR;
		parameter->setModuleInstance(obj);
		return xdl::ERR_OK;
	}
	return xdl::ERR_MODULE_NOT_FOUND;
}
extern "C" XDEVL_EXPORT void _delete(xdl::XdevLModule* obj) {
	if(obj)
		delete obj;
}

extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor() {
	return &pluginDescriptor;
}

namespace xdl {

	xdl_int XdevLFTDILinuxImpl::init() {

		return ERR_OK;
	}

	xdl_int XdevLFTDILinuxImpl::shutdown() {
		return close();
	}

	xdl_int XdevLFTDILinuxImpl::readInfoFromXMLFile() {
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
					if(root->Attribute("device")) {
						m_deviceName = XdevLString(root->Attribute("device"));
					}
					if(root->Attribute("usb_in_size")) {
						std::istringstream ss(root->Attribute("usb_in_size"));
						ss >> m_usb_in_size;
					}					
					if(root->Attribute("usb_out_size")) {
						std::istringstream ss(root->Attribute("usb_out_size"));
						ss >> m_usb_out_size;
					}			
					if(root->Attribute("latency_timer")) {
						std::istringstream ss(root->Attribute("latency_timer"));
						ss >> m_latency_timer;
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
				}
			} else
				XDEVL_MODULE_ERROR("No 'id' attribute specified. Using default values for the device\n");

			root = root->NextSiblingElement();		}
		return ERR_OK;
	}

	xdl::xdl_int XdevLFTDILinuxImpl::open() {

		// Parse XML file and set the serial port settings.
		if(readInfoFromXMLFile() != ERR_OK) {
			XDEVL_MODULE_WARNING("Parsing problems occurred of the Core XML file." << std::endl);
		}
		return _open();
	}

	xdl::xdl_int XdevLFTDILinuxImpl::open(const XdevLFileName& name) {

		// Parse XML file and set the serial port settings.
		if(readInfoFromXMLFile() != ERR_OK) {
			XDEVL_MODULE_WARNING("Parsing problems occurred of the Core XML file." << std::endl);
		}
		m_deviceName 	= name;
		return _open();
	}

	xdl::xdl_int XdevLFTDILinuxImpl::open(const XdevLFileName& name, const XdevLDeviceModes& mode) {
		// Parse XML file and set the serial port settings.
		if(readInfoFromXMLFile() != ERR_OK) {
			XDEVL_MODULE_WARNING("Parsing problems occurred of the Core XML file." << std::endl);
		}

		m_deviceName 	= name;
		return _open();
	}

	xdl::xdl_int XdevLFTDILinuxImpl::_open() {

		XDEVL_MODULE_INFO("Opening connection to the Serial Port device." << std::endl);


		FT_STATUS	ftStatus;
		DWORD	iNumDevs = 0;

		DWORD lpdwFlags;
		DWORD lpdwType;
		DWORD lpdwID;
		DWORD lpdwLocId;
		CHAR serial[128];
		CHAR desc[128];

		// Lets get the number of all connected devices.
		ftStatus = FT_CreateDeviceInfoList(&iNumDevs);
		if(ftStatus != FT_OK) {
			printf("Error: FT_CreateDeviceInfoList(&iNumDevs)\n");
			return -1;
		}

		// Now, lets get information about those connected devices.
		for(unsigned int i = 0; i < iNumDevs; i++) {
			ftStatus = FT_GetDeviceInfoDetail(i, &lpdwFlags, &lpdwType, &lpdwID, &lpdwLocId, serial, desc, &ftHandle);

			XDEVL_MODULE_INFO("Device Serial: " << &serial[0] << std::endl);
			XDEVL_MODULE_INFO("Device Name  : " << &desc[0] << std::endl);
		}

		ftStatus = FT_OpenEx(serial, FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);
		if(ftStatus != FT_OK) {
			XDEVL_MODULE_ERROR("This can fail if the ftdi_sio driver is loaded use lsmod to check this and rmmod ftdi_sio to remove also rmmod usbserial");
			return ERR_ERROR;
		}


		return setStates(m_baudrate, m_byteSize, m_parity, m_stopBits, m_flowControl, m_timeout);
	}


	xdl_int XdevLFTDILinuxImpl::setStates(xdl_int baudrate, XdevLSerialByteSize bytesize, XdevLSerialParity parity, XdevLSerialStopBits stopbits, XdevLSerialFlowControl flowcontrol, xdl_int timeout) {
		XDEVL_MODULE_INFO("Setting Serial Port Protocol values." << std::endl)

		// Set the internal values.
		XdevLSerialImpl::setStates(baudrate, bytesize, parity, stopbits, flowcontrol, timeout);

		FT_STATUS	ftStatus;

		if((ftStatus = FT_SetBaudRate(ftHandle, baudrate)) != FT_OK) {
			XDEVL_MODULE_ERROR("Could not set the input baudrate: " << baudrate);
		}



		DWORD sbits;
		switch(stopbits) {
			case SERIAL_SB_1:
				sbits = FT_STOP_BITS_1;
				break;
			case SERIAL_SB_2:
			default:
				sbits = FT_STOP_BITS_2;
				break;
		}

		DWORD bsize;
		switch(bytesize) {
			case SERIAL_BSIZE_5:
				assert(NULL &&"SERIAL_BSIZE_5 is not supported by this device.");
				break;
			case SERIAL_BSIZE_6:
				assert(NULL && "SERIAL_BSIZE_6 is not supported by this device.");
				break;
			case SERIAL_BSIZE_7:
				bsize = FT_BITS_7;
				break;
			case SERIAL_BSIZE_8:
			default:
				bsize = FT_BITS_8;
				break;
		}


		// Set the parity.
		DWORD sparity;
		switch(parity) {
			case SERIAL_EVEN_PARITY: {
				sparity = FT_PARITY_EVEN;
			}
			break;
			case SERIAL_ODD_PARITY: {
				sparity = FT_PARITY_ODD;
			}
			break;
			case SERIAL_SPACE_PARITY: {
				sparity = FT_PARITY_SPACE;
			}
			break;
			case SERIAL_MARK_PARITY: {
				sparity = FT_PARITY_MARK;
			}
			case SERIAL_NO_PARITY:
			default: {
				sparity = FT_PARITY_NONE;
			}
			break;
		}

		ftStatus = FT_SetDataCharacteristics(ftHandle, bsize, sbits, sparity);
		if(ftStatus != FT_OK) {
			XDEVL_MODULE_ERROR("Could not set all deivce parameters.\n");
		}

		// Set the flow control.
		DWORD fc;
		switch(flowcontrol) {
			case SERIAL_FLOW_CONTROL_HARDWARE:
				fc = FT_FLOW_RTS_CTS;
				break;
			case SERIAL_FLOW_CONTROL_SOFTWARE:
				fc = FT_FLOW_XON_XOFF;
				break;
			case SERIAL_FLOW_CONTROL_NONE:
			default:
				fc = FT_FLOW_NONE;
				break;
		}

		ftStatus = FT_SetFlowControl(ftHandle, fc, 0, 0);
		if(ftStatus != FT_OK) {
			XDEVL_MODULE_ERROR("Could not set the flow control parameter.\n");
		}

		if((ftStatus = FT_SetUSBParameters(ftHandle, m_usb_in_size, m_usb_out_size)) != FT_OK) {
			XDEVL_MODULE_ERROR("Could set USB parameters" << std::endl);
		} else {
			XDEVL_MODULE_INFO("Device USB in#out size: " << m_usb_in_size << "#" << m_usb_out_size << std::endl);
		}

		if((ftStatus = FT_SetLatencyTimer(ftHandle, m_latency_timer)) != FT_OK) {
			XDEVL_MODULE_ERROR("Could set Latency Timer parameter" << std::endl);
		} else {
			XDEVL_MODULE_INFO("Device Latency timer set to: " << m_latency_timer << std::endl);
		}


		if(timeout != 0) {
			ftStatus = FT_SetTimeouts(ftHandle, m_timeout, m_timeout/1000000);
			if(ftStatus != FT_OK) {
				XDEVL_MODULE_ERROR("Could not set the timeout.\n");
			} else {
				XDEVL_MODULE_INFO("Device Timeout set to: " <<  m_timeout/1000000 << std::endl);
			}
		}

		XDEVL_MODULE_SUCCESS("Setting Serial Port Protocol values was successful.\n")
		return ERR_OK;
	}

	xdl::xdl_int XdevLFTDILinuxImpl::close() {
		XDEVL_MODULE_INFO("Closing connection to Serial Port: '" << m_deviceName << "'" << std::endl);

		// Check if we have a valid handle.
		if(ftHandle == NULL) {
			return ERR_OK;
		}

		FT_Close(ftHandle);

		XDEVL_MODULE_SUCCESS("Connection to Serial Port closed successful." << std::endl);
		return ERR_OK;
	}

	xdl_int XdevLFTDILinuxImpl::write(xdl_uint8* src, xdl_int size) {
		FT_STATUS	ftStatus;
		DWORD 	dwBytesWrote;
		

		// Check if we have a valid handle.
		if(ftHandle == NULL) {
			return -1;
		}

		ftStatus = FT_Write(ftHandle, src, size, &dwBytesWrote);
		if(ftStatus != FT_OK) {
			return -1;
		}
		
		return dwBytesWrote;
	}

	xdl_int XdevLFTDILinuxImpl::read(xdl_uint8* dst, xdl_int size) {
		FT_STATUS	ftStatus;
		DWORD 	dwBytesRead;


		// Check if we have a valid handle.
		if(ftHandle == NULL) {
			return -1;
		}

		ftStatus = FT_Read(ftHandle, dst, size, &dwBytesRead);
		if(ftStatus != FT_OK) {
			return -1;
		}

		return dwBytesRead;

	}

	xdl_int XdevLFTDILinuxImpl::waiting() {
		FT_STATUS	ftStatus;
		DWORD RxBytes;

		// Check if we have a valid handle.
		if(ftHandle == NULL) {
			return -1;
		}
		
		ftStatus = FT_GetQueueStatus(ftHandle,&RxBytes);
		if(ftStatus != FT_OK) {
			return -1;
		}
		
		return RxBytes;
	}

	xdl_int XdevLFTDILinuxImpl::flush() {
		
		FT_STATUS	ftStatus;
		
		// Check if we have a valid handle.
		if(ftHandle == NULL) {
			return ERR_ERROR;
		}
		
		ftStatus = FT_Purge(ftHandle, FT_PURGE_RX | FT_PURGE_TX);
		if(ftStatus != FT_OK) {
			return ERR_ERROR;
		}
		
		return ERR_OK;
	}


}
