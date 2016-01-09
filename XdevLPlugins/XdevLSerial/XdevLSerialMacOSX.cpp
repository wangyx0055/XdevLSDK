#include <termios.h> // POSIX terminal control definitions
#include <iomanip>
#include <fcntl.h>   // File control definitions
#include <errno.h>   // Error number definitions
#include <sys/ioctl.h>

#include <XdevLSerialMacOSX.h>
#include <XdevLCoreMediator.h>
#include <XdevLPlatform.h>
#include <sstream>
#include <XdevLXstring.h>


xdl::XdevLModuleDescriptor xdl::XdevLSerialMacOSX::m_moduleDescriptor {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[0],
	xdl::copyright,
	xdl::description,
	xdl::XdevLSerialMajorVersion,
	xdl::XdevLSerialMinorVersion,
	xdl::XdevLSerialPatchVersion
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
	if(xdl::XdevLSerialMacOSX::m_moduleDescriptor.getName() == XDEVL_MODULE_PARAMETER_NAME) {

		xdl::IPXdevLModule module = XDEVL_NEW_MODULE(xdl::XdevLSerialMacOSX,  XDEVL_MODULE_PARAMETER);
		XDEVL_MODULE_SET_MODULE_INSTACE(module);

		return xdl::ERR_OK;
	}
	return xdl::ERR_MODULE_NOT_FOUND;
}

namespace xdl {

	XdevLSerialMacOSX::XdevLSerialMacOSX(XdevLModuleCreateParameter* parameter) :
		XdevLModuleImpl<XdevLSerial>(parameter, m_moduleDescriptor), 	m_fd(-1),
		m_timeoutSpec(NULL) {};

	xdl_int XdevLSerialMacOSX::shutdown() {
		return close();
	}

	xdl_int XdevLSerialMacOSX::readInfoFromXMLFile() {
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
						m_deviceName = root->Attribute("device");
					}
					if(root->Attribute("mode")) {
						m_deviceMode = m_deviceModeMap[root->Attribute("mode")];
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

			root = root->NextSiblingElement();
		}
		return ERR_OK;
	}

	xdl::xdl_int XdevLSerialMacOSX::open() {

		// Parse XML file and set the serial port settings.
		if(readInfoFromXMLFile() != ERR_OK) {
			XDEVL_MODULE_WARNING("Parsing problems occurred of the Core XML file." << std::endl);
		}
		return _open();
	}

	xdl::xdl_int XdevLSerialMacOSX::open(const XdevLFileName& name) {

		// Parse XML file and set the serial port settings.
		if(readInfoFromXMLFile() != ERR_OK) {
			XDEVL_MODULE_WARNING("Parsing problems occurred of the Core XML file." << std::endl);
		}
		m_deviceName 	= name;
		return _open();
	}

	xdl::xdl_int XdevLSerialMacOSX::open(const xdl_char* name, const XdevLDeviceModes& mode) {
		// Parse XML file and set the serial port settings.
		if(readInfoFromXMLFile() != ERR_OK) {
			XDEVL_MODULE_WARNING("Parsing problems occurred of the Core XML file." << std::endl);
		}

		m_deviceName 	= name;
		return _open();
	}

	xdl::xdl_int XdevLSerialMacOSX::_open() {

		XDEVL_MODULE_INFO("Opening connection to the Serial Port device." << std::endl);

		// Open the device.
		m_flag = O_RDWR | O_NOCTTY;
		m_fd = ::open(m_deviceName.toString().c_str(), m_flag);
		if(m_fd == -1) {
			XDEVL_MODULE_ERROR("Connect to: '" << m_deviceName << "' failed: " << strerror(errno) << std::endl);
			return ERR_ERROR;
		} else {
			XDEVL_MODULE_SUCCESS("Connecting to: '" << m_deviceName << "' established." << std::endl);
		}


		return setStates(m_baudrate, m_byteSize, m_parity, m_stopBits, m_flowControl, m_timeout);
	}


	xdl_int XdevLSerialMacOSX::setStates(xdl_int baudrate, XdevLSerialByteSize bytesize, XdevLSerialParity parity, XdevLSerialStopBits stopbits, XdevLSerialFlowControl flowcontrol, xdl_int timeout) {
		XDEVL_MODULE_INFO("Setting Serial Port Protocol values." << std::endl)

		// Set the internal values.
		XdevLSerialImpl::setStates(baudrate, bytesize, parity, stopbits, flowcontrol, timeout);


		// Get the current options for the port...
		termios options;
		if(tcgetattr(m_fd, &options) == -1) {
			XDEVL_MODULE_ERROR("Could not retrieve terminal attributes: " << strerror(errno) << std::endl);
			return xdl::ERR_ERROR;
		}


		// Set the baudrate.
		speed_t br = wrapBaudrate(baudrate);
		if(cfsetispeed(&options, br) == -1) {
			XDEVL_MODULE_ERROR("Could not set the input baudrate: " << br);
		}
		if(cfsetospeed(&options, br) == -1) {
			XDEVL_MODULE_ERROR("Could not set the output baudrate: " << br);
		}



		// Enable the receiver and set local mode...
		options.c_cflag |= (CLOCAL | CREAD);

		switch(stopbits) {
			case SERIAL_SB_1:
				options.c_cflag &= ~CSTOPB;
				break;
			case SERIAL_SB_2:
			default:
				options.c_cflag |= CSTOPB;
				break;
		}

		options.c_cflag &= ~CSIZE;
		switch(bytesize) {
			case SERIAL_BSIZE_5:
				options.c_cflag |= CS5;
				break;
			case SERIAL_BSIZE_6:
				options.c_cflag |= CS6;
				break;
			case SERIAL_BSIZE_7:
				options.c_cflag |= CS7;
				break;
			case SERIAL_BSIZE_8:
			default:
				options.c_cflag |= CS8;
				break;
		}


		// Set the parity.
		switch(parity) {
			case SERIAL_EVEN_PARITY: {
				options.c_iflag &= ~(IGNPAR | PARMRK);
				options.c_iflag |= INPCK;
				options.c_cflag |= PARENB;
				options.c_cflag &= ~PARODD;
			}
			break;
			case SERIAL_ODD_PARITY: {
				options.c_iflag &= ~(IGNPAR | PARMRK);
				options.c_iflag |= INPCK;
				options.c_cflag |= (PARENB | PARODD);
			}
			break;
			case SERIAL_SPACE_PARITY: {

			}
			break;
			case SERIAL_MARK_PARITY: {

			}
			case SERIAL_NO_PARITY:
			default: {
				options.c_iflag |= IGNPAR;
				options.c_cflag &= ~(PARENB | PARODD);
			}
			break;
		}


		// Set the flow control.
		switch(flowcontrol) {
			case SERIAL_FLOW_CONTROL_HARDWARE:
				options.c_cflag |= CRTSCTS;					// Enable hardware handshake (Which is hardware control).
				options.c_iflag &= ~(IXOFF | IXON);	// Disable Xon/Xoff (Which is software control).
				break;
			case SERIAL_FLOW_CONTROL_SOFTWARE:
				options.c_iflag |= IXOFF | IXON;		// Enable Xon/Xoff.
				options.c_cflag &= ~CRTSCTS;				// Disable hardare handshake.
				break;
			case SERIAL_FLOW_CONTROL_NONE:
			default:
				options.c_cflag &= ~CRTSCTS;				// Disable hardware handshake.
				options.c_iflag &= ~(IXOFF | IXON);	// Disable software handshake.
				break;
		}


		// Disable cannon mode. We don't need that really.
		options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
		options.c_oflag &= ~OPOST;

		if(timeout != 0) {
			m_timeoutSpec = new timespec();
			m_timeoutSpec->tv_sec = m_timeout/1000000000;
			m_timeoutSpec->tv_nsec = m_timeout;
		}

		// Flush everything before setting anything.
		if(tcflush(m_fd, TCIFLUSH) == -1) {
			XDEVL_MODULE_ERROR("Could not flush: " << strerror(errno) << std::endl);
			return xdl::ERR_ERROR;
		}

		// Set the new options for the port.
		if(tcsetattr(m_fd, TCSANOW, &options) == -1) {
			XDEVL_MODULE_ERROR("Could not set attributes to the device: " << strerror(errno) << std::endl);
			return xdl::ERR_ERROR;
		}
		XDEVL_MODULE_SUCCESS("Setting Serial Port Protocol values was successful.\n")
		return ERR_OK;
	}

	xdl::xdl_int XdevLSerialMacOSX::close() {
		XDEVL_MODULE_INFO("Closing connection to Serial Port: '" << m_deviceName << "'" << std::endl);
		if(m_fd != -1) {
			if(::close(m_fd) == -1) {
				XDEVL_MODULE_ERROR(strerror(errno) << "\n");
				return ERR_ERROR;
			}
		}

		m_fd = -1;

		XDEVL_MODULE_SUCCESS("Connection to Serial Port closed successful." << std::endl);
		return ERR_OK;
	}

	xdl_int XdevLSerialMacOSX::write(xdl_uint8* src, xdl_int size) {
		if(m_fd == -1) {
			return -1;
		}
		xdl_int number_bytes = -1;

		// Why do we use pselect the read values from the buffer? pselect allows us
		// a timeout in nano seconds.
		fd_set read_fds, write_fds, except_fds;
		FD_ZERO(&read_fds);
		FD_ZERO(&write_fds);
		FD_ZERO(&except_fds);
		FD_SET(m_fd, &read_fds);

		if(pselect(m_fd + 1, &read_fds, &write_fds, &except_fds, m_timeoutSpec, NULL) == 1) {
			number_bytes=::write(m_fd, src, size);
		} else {
			// TODO: If we are here a timeout occured.
		}

		return number_bytes;
	}

	xdl_int XdevLSerialMacOSX::read(xdl_uint8* dst, xdl_int size) {
		xdl_int number_bytes = -1;
		// Check if we have a valid file descriptor.
		if(m_fd == -1) {
			return number_bytes;
		}
		// Why do we use pselect the read values from the buffer? pselect allows us
		// a timeout in nano seconds.
		fd_set read_fds, write_fds, except_fds;
		FD_ZERO(&read_fds);
		FD_ZERO(&write_fds);
		FD_ZERO(&except_fds);
		FD_SET(m_fd, &read_fds);

		if(pselect(m_fd + 1, &read_fds, &write_fds, &except_fds, m_timeoutSpec, NULL) == 1) {
			number_bytes=::read(m_fd, dst, size);
		} else {
			// TODO: If we are here a timeout occured.
		}
		return number_bytes;

	}

	xdl_int XdevLSerialMacOSX::waiting() {
		// Get the values waiting in the buffer.
		xdl_int bytes_avail = 0;
		if(ioctl(m_fd, FIONREAD, &bytes_avail) == -1) {
			XDEVL_MODULE_ERROR("Retrieve waiting packages: " << strerror(errno) << std::endl);
			return xdl::ERR_ERROR;
		}
		return bytes_avail;
	}

	xdl_int XdevLSerialMacOSX::flush() {
		// Flush the buffer.
		if(tcflush(m_fd, TCIFLUSH) == -1) {
			XDEVL_MODULE_ERROR("Flush: " << strerror(errno) << std::endl);
			return xdl::ERR_ERROR;
		}
		return ERR_OK;
	}

	xdl_int XdevLSerialMacOSX::wrapBaudrate(xdl_int baudrate) {
		switch(baudrate) {
			case 0:
				return B0;
			case 50:
				return B50;
			case 75:
				return B75;
			case 110:
				return B110;
			case 134:
				return B134;
			case 150:
				return B150;
			case 200:
				return B200;
			case 300:
				return B300;
			case 600:
				return B600;
			case 1200:
				return B1200;
			case 2400:
				return B2400;
			case 4800:
				return B4800;
			case 9600:
				return B9600;
			case 19200:
				return B19200;
			case 38400:
				return B38400;
			case 57600:
				return B57600;
			case 115200:
				return B115200;
			case 230400:
				return B230400;
			default:
				break;
		}
		return B9600;
	}

	void XdevLSerialMacOSX::dump() {
		termios options;
		tcgetattr(m_fd, &options);

		XDEVL_MODULE_INFO("Filedescriptor: " 		<< m_fd 						<< std::endl);
		XDEVL_MODULE_INFO("Input Baudrate: "					<< cfgetispeed(&options)<< std::endl);
		XDEVL_MODULE_INFO("Input Baudrate: "					<< cfgetospeed(&options)<< std::endl);
		XDEVL_MODULE_INFO("Dirty Flag: " 				<< m_dirtyFlag 			<< std::endl);
		XDEVL_MODULE_INFO("Device name: " 			<< m_deviceName 		<< std::endl);
		XDEVL_MODULE_INFO("Options c_cflag: " 	<< options.c_cflag	<< std::endl);
		XDEVL_MODULE_INFO("Options c_iflag: " 	<< options.c_iflag 	<< std::endl);
		XDEVL_MODULE_INFO("Options c_ispeed: " 	<< options.c_ispeed << std::endl);
		XDEVL_MODULE_INFO("Options c_lflag: " 	<< options.c_lflag 	<< std::endl);
		XDEVL_MODULE_INFO("Options c_oflag: " 	<< options.c_oflag 	<< std::endl);
		XDEVL_MODULE_INFO("Options c_ospeed: " 	<< options.c_ospeed << std::endl);
	}

}
