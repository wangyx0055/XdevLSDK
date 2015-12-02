/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2012 Cengiz Terzibas

	This library is free software; you can redistribute it and/or modify it under the
	terms of the GNU Lesser General Public License as published by the Free Software
	Foundation; either version 2.1 of the License, or (at your option) any later version.
	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License along with
	this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
	Suite 330, Boston, MA 02111-1307 USA

	I would appreciate if you report all bugs to: cengiz@terzibas.de
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

	/**
		@enum XdevLSerialFlowControl
		@brief The control mode of the serial port.
	*/
	enum XdevLSerialFlowControl {
	    SERIAL_FLOW_CONTROL_NONE,
	    SERIAL_FLOW_CONTROL_HARDWARE,
	    SERIAL_FLOW_CONTROL_SOFTWARE
	};

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
