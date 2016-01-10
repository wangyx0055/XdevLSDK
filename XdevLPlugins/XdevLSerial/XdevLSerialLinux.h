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

#ifndef XDEVL_SERIAL_LINUX_H
#define XDEVL_SERIAL_LINUX_H

#include <XdevLSerial.h>
#include <XdevLSerialImpl.h>
#include <XdevLPluginImpl.h>
#include <XdevLUtils.h>

namespace xdl {

	static const std::vector<XdevLModuleName>	moduleNames	{
		XdevLModuleName("XdevLSerial")
	};

	/**
		@class XdevLSerialLinux
		@brief Core class to support Serial Port Communication.
		@author Cengiz Terzibas
	*/
	class XdevLSerialLinux : public XdevLSerialImpl, public XdevLModuleImpl<XdevLSerial>  {
		public:
			XdevLSerialLinux(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor);
			virtual ~XdevLSerialLinux() {}

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
			timespec*				m_timeoutSpec;


	};

}

#endif
