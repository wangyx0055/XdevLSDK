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

#ifndef XDEVL_SERIAL_WIN_H
#define XDEVL_SERIAL_WINL_H

#include <XdevLSerial.h>
#include <XdevLSerialImpl.h>
#include <XdevLPluginImpl.h>
#include <XdevLUtils.h>
#include <fstream>

namespace xdl {

	// Holds the major version number of the plugin.
	const xdl_uint XdevLSerialPluginMajorVersion = XDEVLSERIAL_MAJOR_VERSION;
	
	// Holds the Minor version number of the plugin.
	const xdl_uint XdevLSerialPluginMinorVersion = XDEVLSERIAL_MINOR_VERSION;
	
	// Holds the Patch version number of the plugin.
	const xdl_uint XdevLSerialPluginPatchVersion = XDEVLSERIAL_PATCH_VERSION;


	// Holds the Major version number.
	const xdl_uint XdevLSerialMajorVersion = XDEVLSERIAL_MODULE_MAJOR_VERSION;
	
	// Holds the Minor version number.
	const xdl_uint XdevLSerialMinorVersion = XDEVLSERIAL_MODULE_MINOR_VERSION;
	
	// Holds the Patch version number.
	const xdl_uint XdevLSerialPatchVersion = XDEVLSERIAL_MODULE_PATCH_VERSION;
	

	static const XdevLString					vendor		{ "www.codeposer.net"
	};
	static const XdevLString					author		{ "Cengiz Terzibas"
	};
	static const XdevLString					copyright	{ "(c) 2005 - 2015 Cengiz Terzibas."
	};
	static const XdevLString					description	{ "Creates a Serial connection."
	};
	static const XdevLString					pluginName	{ "XdevLSerial"
	};

	static const std::vector<XdevLModuleName>	moduleNames	{
		XdevLModuleName("XdevLSerial")
	};

	/**
		@class XdevLSerialWin
		@brief Reference implementation of the XdevLSerial interface for Windows OS.
		@author Cengiz Terzibas
	*/

	class XdevLSerialWin : public XdevLSerialImpl, public XdevLModuleImpl < XdevLSerial > {
		public:
			XdevLSerialWin(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLSerialWin() {}
			static XdevLModuleDescriptor m_serialModuleDescriptor;
			virtual xdl_int open();
			virtual xdl_int open(const XdevLFileName& name);
			virtual xdl_int open(const xdl_char* name);
			virtual xdl_int close();
			virtual xdl_int read(xdl_uint8* dst, xdl_int size);
			virtual xdl_int write(xdl_uint8* src, xdl_int size);
			virtual xdl_int flush();
			virtual xdl_int setStates(xdl_int baudrate, XdevLSerialByteSize bytesize, XdevLSerialParity parity, XdevLSerialStopBits stopbits, XdevLSerialFlowControl flowcontrol, xdl_int timeout);
			virtual xdl_int waiting();
			virtual xdl_int flush() const;
		protected:
			virtual xdl_int _open();
			virtual xdl_int init();
			virtual xdl_int shutdown();

			xdl_int readInfoFromXMLFile();
			void printErrorMessage();
			xdl_int SetTimeOut(xdl_int timeout);
		private:
			HANDLE						m_handle;
	};

}

#endif
