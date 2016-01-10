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

#ifndef XDEVL_SERIAL_MACOSX_H
#define XDEVL_SERIAL_MACOSX_H

#include <XdevLSerial.h>
#include <XdevLSerialImpl.h>
#include <XdevLPluginImpl.h>
#include <XdevLUtils.h>

#include <IOKit/IOKitLib.h>
#include <IOKit/serial/IOSerialKeys.h>
#include <IOKit/IOBSD.h>

namespace xdl {

	static const std::vector<XdevLModuleName>	moduleNames	{
		XdevLModuleName("XdevLSerial")
	};


	/**
		@class XdevLSerialMacOSX
		@brief Core class to support Serial Port Communication.
		@author Cengiz Terzibas
	*/
	class XdevLSerialMacOSX : public XdevLSerialImpl, public XdevLModuleImpl<XdevLSerial>  {
		public:
			XdevLSerialMacOSX(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor);
			virtual ~XdevLSerialMacOSX() {}

			virtual xdl_int open();
			virtual xdl_int open(const XdevLFileName& name);
			virtual xdl_int open(const xdl_char* name, const XdevLDeviceModes& mode);
			virtual xdl_int close();
			virtual xdl_int write(xdl_uint8* src, xdl_int size);
			virtual xdl_int read(xdl_uint8* dst, xdl_int size);
			virtual xdl_int setStates(xdl_int baudrate, XdevLSerialByteSize bytesize, XdevLSerialParity parity, XdevLSerialStopBits stopbits, XdevLSerialFlowControl flowcontrol, xdl_int timeout);
			virtual xdl_int waiting();
			virtual xdl_int flush();
		protected:
			virtual xdl_int shutdown();

			xdl_int readInfoFromXMLFile();
			xdl::xdl_int _open();
			xdl_int wrapBaudrate(xdl_int baudrate);
			void dump();
		private:

			xdl_int					m_fd;
			xdl_bool				m_dirtyFlag;
			xdl_int					m_flag;
			tcflag_t 				m_c_cflag;
			timespec*				m_timeoutSpec;
			xdl_bool				m_connected;

	};

}

#endif
