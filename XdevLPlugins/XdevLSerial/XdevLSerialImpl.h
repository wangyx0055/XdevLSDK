/*
	Copyright (c) 2005 - 2016 Cengiz Terzibas

	Permission is hereby granted, free of charge, to any person obtaining a copy of 
	this software and associated documentation files (the "Software"), to deal in the 
	Software without restriction, including without limitation the rights to use, copy, 
	modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
	and to permit persons to whom the Software is furnished to do so, subject to the 
	following conditions:

	The above copyright notice and this permission notice shall be included in all copies 
	or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
	DEALINGS IN THE SOFTWARE.

	cengiz@terzibas.de
*/

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
												m_dirty(xdl_false),
												m_xon(0x11),
												m_xoff(0x13){

				m_deviceModeMap["XDEVL_DEVICE_READ_ONLY"] 	= XdevLStream::AccesType::XDEVL_DEVICE_READ_ONLY;
				m_deviceModeMap["XDEVL_DEVICE_WRITE_ONLY"] 	= XdevLStream::AccesType::XDEVL_DEVICE_WRITE_ONLY;
				m_deviceModeMap["XDEVL_DEVICE_READ_WRITE"] 	= XdevLStream::AccesType::XDEVL_DEVICE_READ_WRITE;

				m_byteSizeMap["SERIAL_BSIZE_5"] = SERIAL_BSIZE_5;
				m_byteSizeMap["SERIAL_BSIZE_6"] = SERIAL_BSIZE_6;
				m_byteSizeMap["SERIAL_BSIZE_7"] = SERIAL_BSIZE_7;
				m_byteSizeMap["SERIAL_BSIZE_8"] = SERIAL_BSIZE_8;

				m_parityMap["SERIAL_NO_PARITY"] 	= SERIAL_NO_PARITY;
				m_parityMap["SERIAL_EVEN_PARITY"] 	= SERIAL_EVEN_PARITY;
				m_parityMap["SERIAL_ODD_PARITY"] 	= SERIAL_ODD_PARITY;
				m_parityMap["SERIAL_SPACE_PARITY"] 	= SERIAL_SPACE_PARITY;

				m_stopBitsMap["SERIAL_SB_1"] = SERIAL_SB_1;
				m_stopBitsMap["SERIAL_SB_2"] = SERIAL_SB_2;

				m_flowControlMap["SERIAL_FLOW_CONTROL_NONE"] 		= SERIAL_FLOW_CONTROL_NONE;
				m_flowControlMap["SERIAL_FLOW_CONTROL_HARDWARE"] 	= SERIAL_FLOW_CONTROL_HARDWARE;
				m_flowControlMap["SERIAL_FLOW_CONTROL_SOFTWARE"] 	= SERIAL_FLOW_CONTROL_SOFTWARE;

		}
	void setStates(xdl_int baudrate, XdevLSerialByteSize bytesize, XdevLSerialParity parity, XdevLSerialStopBits stopbits, XdevLSerialFlowControl flowcontrol, xdl_int timeout = -1) {
		this->m_baudrate 		= baudrate;
		this->m_byteSize 		= bytesize;
		this->m_parity			= parity;
		this->m_stopBits		= stopbits;
		this->m_flowControl		= flowcontrol;
		this->m_timeout			= timeout;
	}

	protected:
		XdevLFileName			m_deviceName;
		XdevLStream::AccesType	m_deviceMode;
		xdl::xdl_int 			m_baudrate;
		XdevLSerialByteSize		m_byteSize;	
		XdevLSerialParity 		m_parity;
		XdevLSerialStopBits 	m_stopBits;
		XdevLSerialFlowControl 	m_flowControl;
		xdl_int					m_timeout;
		xdl_bool				m_dirty;
		xdl_int					m_xon;
		xdl_int					m_xoff;
		
		std::map<std::string, XdevLStream::AccesType>	m_deviceModeMap;	
		std::map<std::string, XdevLSerialByteSize> 		m_byteSizeMap;	
		std::map<std::string, XdevLSerialParity> 			m_parityMap;	
		std::map<std::string, XdevLSerialStopBits> 		m_stopBitsMap;	
		std::map<std::string, XdevLSerialFlowControl> m_flowControlMap;		
	};

}

#endif
