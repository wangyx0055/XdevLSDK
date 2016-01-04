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

#ifndef XDEVL_SERIAL_H
#define XDEVL_SERIAL_H

#include <XdevLStream.h>

namespace xdl {

	/**
		@enum XdevLSerialStopBits
		@brief The stop bits of the serial port.
	*/
	enum XdevLSerialStopBits {
	    SERIAL_SB_1,
	    SERIAL_SB_2
	};

	inline XdevLString xdevLStopBitsToString(xdl_int stopBits) {
		if(stopBits == SERIAL_SB_1) {
			return XdevLString("SERIAL_SB_1");
		} else if(stopBits == SERIAL_SB_2) {
			return XdevLString("SERIAL_SB_2");
		}
		return XdevLString("SERIAL_SB_UNKOWN");
	}

	/**
		@enum XdevLSerialByteSize
		@brief The byte size of the data on the serial port.
	*/
	enum XdevLSerialByteSize {
	    SERIAL_BSIZE_5,
	    SERIAL_BSIZE_6,
	    SERIAL_BSIZE_7,
	    SERIAL_BSIZE_8
	};

	inline XdevLString xdevLByteSizeToString(xdl_int byteSize) {
		if(byteSize == SERIAL_BSIZE_5) {
			return XdevLString("SERIAL_BSIZE_5");
		} else if(byteSize == SERIAL_BSIZE_6) {
			return XdevLString("SERIAL_BSIZE_6");
		} else if(byteSize == SERIAL_BSIZE_7) {
			return XdevLString("SERIAL_BSIZE_7");
		} else if(byteSize == SERIAL_BSIZE_8) {
			return XdevLString("SERIAL_BSIZE_8");
		}
		return XdevLString("SERIAL_BSIZE_UNKOWN");
	}

	/**
		@enum XdevLSerialBaudRate
		@brief The baud rate of the serial port.
	*/
	enum XdevLSerialBaudRate {
	    SERIAL_BAUD_0 = 0,
	    SERIAL_BAUD_50 = 50,
	    SERIAL_BAUD_75 = 75,
	    SERIAL_BAUD_110 = 110,
	    SERIAL_BAUD_134 = 134,
	    SERIAL_BAUD_150 = 150,
	    SERIAL_BAUD_200 = 200,
	    SERIAL_BAUD_300 = 300,
	    SERIAL_BAUD_600 = 600,
	    SERIAL_BAUD_1200 = 1200,
	    SERIAL_BAUD_1800 = 1800,
	    SERIAL_BAUD_2400 = 2400,
	    SERIAL_BAUD_4800 = 4800,
	    SERIAL_BAUD_9600 = 9600,
	    SERIAL_BAUD_19200 = 19200,
	    SERIAL_BAUD_38400 = 38400,
	    SERIAL_BAUD_57600 = 57600,
	    SERIAL_BAUD_115200 = 115200,
	    SERIAL_BAUD_230400 = 230400,
	    SERIAL_BAUD_460800 = 460800,
	    SERIAL_BAUD_500000 = 500000,
	    SERIAL_BAUD_576000 = 576000,
	    SERIAL_BAUD_921600 = 921600,
	    SERIAL_BAUD_1000000 = 1000000,
	    SERIAL_BAUD_1152000 = 1152000,
	    SERIAL_BAUD_1382400 = 1382400,
	    SERIAL_BAUD_1500000 = 1500000,
	    SERIAL_BAUD_2000000 = 2000000,
	    SERIAL_BAUD_2500000 = 2500000,
	    SERIAL_BAUD_3000000 = 3000000,
	    SERIAL_BAUD_3500000 = 3500000,
	    SERIAL_BAUD_4000000 = 4000000
	};

	/**
		@enum XdevLSerialParity
		@brief The parity of the serial port.
	*/
	enum XdevLSerialParity {
	    SERIAL_EVEN_PARITY,
	    SERIAL_MARK_PARITY,
	    SERIAL_NO_PARITY,
	    SERIAL_ODD_PARITY,
	    SERIAL_SPACE_PARITY
	};

	inline XdevLString xdevLParityToString(xdl_int parity) {
		if(parity == SERIAL_EVEN_PARITY) {
			return XdevLString("SERIAL_EVEN_PARITY");
		} else if(parity == SERIAL_MARK_PARITY) {
			return XdevLString("SERIAL_MARK_PARITY");
		} else if(parity == SERIAL_NO_PARITY) {
			return XdevLString("SERIAL_NO_PARITY");
		} else if(parity == SERIAL_ODD_PARITY) {
			return XdevLString("SERIAL_ODD_PARITY");
		} else if(parity == SERIAL_SPACE_PARITY) {
			return XdevLString("SERIAL_SPACE_PARITY");
		}
		return XdevLString("SERIAL_PARITY_UNKOWN");
	}

	/**
		@enum XdevLSerialFlowControl
		@brief The control mode of the serial port.

		Flow control means the ability to slow down the flow of bytes in a wire. That
		means to be able to stop/restart the flow without loosing bytes. 
	*/
	enum XdevLSerialFlowControl {
	    SERIAL_FLOW_CONTROL_NONE,
	    SERIAL_FLOW_CONTROL_HARDWARE,
	    SERIAL_FLOW_CONTROL_SOFTWARE
	};

	inline XdevLString xdevLFlowControlToString(xdl_int flowControl) {
		if(flowControl == SERIAL_FLOW_CONTROL_NONE) {
			return XdevLString("SERIAL_FLOW_CONTROL_NONE");
		} else if(flowControl == SERIAL_FLOW_CONTROL_HARDWARE) {
			return XdevLString("SERIAL_FLOW_CONTROL_HARDWARE");
		} else if(flowControl == SERIAL_FLOW_CONTROL_SOFTWARE) {
			return XdevLString("SERIAL_FLOW_CONTROL_SOFTWARE");
		}
		return XdevLString("SERIAL_FLOW_CONTROL_UNKOWN");
	}

	/**
		@class XdevLSerial
		@brief Interface for Serial Port devices.
		@author Cengiz Terzibas
	*/
	class XdevLSerial : public XdevLStream {
		public:
			virtual ~XdevLSerial() {};
			/// Open a connection to a specific serial port using only XML file parameters.
			/**
				If nothing is specified in the XML file default values will be used. The parameters
				for the serial port can be changed afterwards using the
				@return
					- @b ERR_ERROR if it failed.
					- @b ERR_OK if it was successful.
			*/
			virtual xdl_int open() = 0;
			/// Open a connection to a serial port using a device name.
			/**
				Parameters of the serial port will be imported from the XML file.
				If no parameters are specified in the XML file default values will be used.
				@return
					- @b ERR_ERROR if it failed.
					- @b ERR_OK if it was successful.
			*/
			virtual xdl_int open(const XdevLFileName& name) = 0;
			/// Close the serial port connection.
			/**
				@return
					- @b ERR_ERROR if it failed.
					- @b ERR_OK if it was successful.
			*/
			virtual xdl_int close() = 0;
			/// Send data to a open serial port connection.
			/**
				@param[in] src The memory buffer to copy from.
				@param[in] size The number of bytes to send.
				@return
					- @b -1 if it failed.
					- @b Returns the number of bytes sent if it was successful.
			*/
			virtual xdl_int write(xdl_uint8* src, xdl_int size) = 0;
			/// Receive data from a open serial port connection.
			/**
				@param[out] dst The buffer where the message should be stored.
				@param[in] size The length in bytes of the buffer pointed by dst.
				@return
					- @b -1 if it failed.
					- @b Returns the length of the message in bytes if it was successful. If no message are avaiable it will return 0.
			*/
			virtual xdl_int read(xdl_uint8*  dst, xdl_int size) = 0;
			/// Set the serial port communication properties.
			/**
				@param baudrate The baudrate of the serial port.
				@param bytesize The byte size of the data on the serial port.
				@param parity The parity of the serial port.
				@param stopbits The stop bits of the serial port.
				@param timeout Sets the timeout of the reading and writting bytes on the serial port.
				@return
					- @b ERR_ERROR if it failed.
					- @b ERR_OK if it was successful.
			*/
			virtual xdl_int setStates(xdl_int baudrate, XdevLSerialByteSize bytesize, XdevLSerialParity parity, XdevLSerialStopBits stopbits, XdevLSerialFlowControl flowcontrol, xdl_int timeout = -1) = 0;

			/// Returns the number of bytes waiting in the buffer.
			/**
				@return The number of bytes waiting in the buffer.
			*/
			virtual xdl_int waiting() = 0;
	};

	typedef XdevLSerial* IPXdevLSerial;

}

#endif
