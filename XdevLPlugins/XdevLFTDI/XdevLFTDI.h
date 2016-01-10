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

#ifndef XDEVL_FTDI_LINUX_IMPL_H
#define XDEVL_FTDI_LINUX_IMPL_H

#include <XdevLSerial/XdevLSerial.h>
#include <XdevLSerial/XdevLSerialImpl.h>
#include <XdevLPluginImpl.h>
#include <XdevLUtils.h>
#include <ftd2xx.h>

namespace xdl {

	static const XdevLString vendor {
		"www.codeposer.net"
	};
	static const XdevLString author {
		"Cengiz Terzibas"
	};
	static const XdevLString copyright {
		"(c) 2005 - 2015 Cengiz Terzibas."
	};
	static const XdevLString pluginName {
		"XdevLFTDI"
	};
	static const XdevLString description {
		"Creates a connection and read/write methods to a FTDI Serial Port chipsets."
	};

	static const std::vector<XdevLModuleName> moduleNames {
		XdevLModuleName("XdevLSerial")
	};

	class XdevLFTDIDeviceInfo {
		public:
			XdevLFTDIDeviceInfo() :
				handle(0),
				usbInSize(2048),
				usbOutSize(2048) {
			}

			FT_HANDLE handle;
			DWORD flags[128];
			DWORD type[128];
			DWORD id[128];
			DWORD locId[128];
			CHAR serial[128];
			CHAR desc[128];
			DWORD usbInSize;
			DWORD usbOutSize;
	};

	/**
		@class XdevLFTDI
		@brief Core class to support Serial Port Communication for FTDI devices.
		@author Cengiz Terzibas
	*/
	class XdevLFTDI : public XdevLSerialImpl, public XdevLModuleImpl<XdevLSerial>  {
		public:
			XdevLFTDI(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor);
			virtual ~XdevLFTDI();

			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;

			virtual xdl_int open() override;
			virtual xdl_int open(const XdevLFileName& name) override;
			virtual xdl_int open(const XdevLFileName& name, const XdevLDeviceModes& mode);
			virtual xdl_int close();
			virtual xdl_int write(xdl_uint8* src, xdl_int size) override;
			virtual xdl_int read(xdl_uint8* dst, xdl_int size) override;
			virtual xdl_int setStates(xdl_int baudrate,
			                          XdevLSerialByteSize bytesize,
			                          XdevLSerialParity parity,
			                          XdevLSerialStopBits stopbits,
			                          XdevLSerialFlowControl flowcontrol, xdl_int timeout) override;
			virtual xdl_int waiting() override;
			virtual xdl_int flush() override;

		protected:

			xdl_int readInfoFromXMLFile();
			xdl::xdl_int openConnection();

		private:
			XdevLFTDIDeviceInfo currentDevice;
			DWORD m_usbInSize;
			DWORD m_usbOutSize;
			DWORD m_latencyTimer;
			XdevLString	m_deviceName;
			std::vector<XdevLFTDIDeviceInfo> m_foundDevices;
	};

}

#endif
