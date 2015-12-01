#ifndef XDEVL_SERIAL_IMPL_H
#define XDEVL_SERIAL_IMPL_H

#include <map>
#include <string>
#include <XdevLSerial/XdevLSerial.h>

namespace xdl {

	class XdevLSerialImpl {
	public:
		XdevLSerialImpl() : m_deviceMode(XdevLStream::AccesType::XDEVL_DEVICE_READ_ONLY),
												m_baudrate(9600), 
												m_byteSize(SERIAL_BSIZE_8), 
												m_parity(SERIAL_NO_PARITY),
												m_stopBits(SERIAL_SB_1),
												m_flowControl(SERIAL_FLOW_CONTROL_NONE),
												m_timeout(0),
												m_dirty(xdl_false) {
													
													
				m_deviceModeMap["XDEVL_DEVICE_READ_ONLY"] 	= XdevLStream::AccesType::XDEVL_DEVICE_READ_ONLY;
				m_deviceModeMap["XDEVL_DEVICE_WRITE_ONLY"] 	= XdevLStream::AccesType::XDEVL_DEVICE_WRITE_ONLY;
				m_deviceModeMap["XDEVL_DEVICE_READ_WRITE"] 	= XdevLStream::AccesType::XDEVL_DEVICE_READ_WRITE;
													
				m_byteSizeMap["SERIAL_BSIZE_5"] = SERIAL_BSIZE_5;
				m_byteSizeMap["SERIAL_BSIZE_6"] = SERIAL_BSIZE_6;	
				m_byteSizeMap["SERIAL_BSIZE_7"] = SERIAL_BSIZE_7;	
				m_byteSizeMap["SERIAL_BSIZE_8"] = SERIAL_BSIZE_8;	
				
				m_parityMap["SERIAL_NO_PARITY"] 		= SERIAL_NO_PARITY;	
				m_parityMap["SERIAL_EVEN_PARITY"] 	= SERIAL_EVEN_PARITY;	
				m_parityMap["SERIAL_ODD_PARITY"] 		= SERIAL_ODD_PARITY;	
				m_parityMap["SERIAL_SPACE_PARITY"] 	= SERIAL_SPACE_PARITY;	
			
				m_stopBitsMap["SERIAL_SB_1"] = SERIAL_SB_1;	
				m_stopBitsMap["SERIAL_SB_2"] = SERIAL_SB_2;	

				m_flowControlMap["SERIAL_FLOW_CONTROL_NONE"] 			= SERIAL_FLOW_CONTROL_NONE;	
				m_flowControlMap["SERIAL_FLOW_CONTROL_HARDWARE"] 	= SERIAL_FLOW_CONTROL_HARDWARE;	
				m_flowControlMap["SERIAL_FLOW_CONTROL_SOFTWARE"] 	= SERIAL_FLOW_CONTROL_SOFTWARE;	

		}
	void setStates(xdl_int baudrate, XdevLSerialByteSize bytesize, XdevLSerialParity parity, XdevLSerialStopBits stopbits, XdevLSerialFlowControl flowcontrol, xdl_int timeout = -1) {
		this->m_baudrate 		= baudrate;
		this->m_byteSize 		= bytesize;
		this->m_parity			= parity;
		this->m_stopBits		= stopbits;
		this->m_flowControl	= flowcontrol;
		this->m_timeout			= timeout;
	}

	protected:
		XdevLFileName					m_deviceName;
		XdevLStream::AccesType	m_deviceMode;
		xdl::xdl_int 						m_baudrate;
		XdevLSerialByteSize			m_byteSize;	
		XdevLSerialParity 			m_parity;
		XdevLSerialStopBits 		m_stopBits;
		XdevLSerialFlowControl 	m_flowControl;
		xdl_int									m_timeout;
		xdl_bool								m_dirty;
		
		std::map<std::string, XdevLStream::AccesType>	m_deviceModeMap;	
		std::map<std::string, XdevLSerialByteSize> 		m_byteSizeMap;	
		std::map<std::string, XdevLSerialParity> 			m_parityMap;	
		std::map<std::string, XdevLSerialStopBits> 		m_stopBitsMap;	
		std::map<std::string, XdevLSerialFlowControl> m_flowControlMap;		
	};

}

#endif
