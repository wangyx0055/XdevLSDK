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

#include <XdevLModule.h>
#include <XdevLCoreMediator.h>
#include <XdevLUtils.h>
#include "XdevLSocketImpl.h"
#include <string>
#include <cstring>


xdl::XdevLModuleDescriptor xdl::XdevLUDPSocketImpl::m_UDPSocketModuleDesc {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[0],
	xdl::copyright,
	xdl::descriptionUDP,
	xdl::XdevLUDPSocketMajorVersion,
	xdl::XdevLUDPSocketMinorVersion,
	xdl::XdevLUDPSocketPatchVersion
};

xdl::XdevLModuleDescriptor xdl::XdevLUDPSocket2Impl::m_UDPSocket2ModuleDesc {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[2],
	xdl::copyright,
	xdl::descriptionUDP,
	xdl::XdevLUDPSocket2MajorVersion,
	xdl::XdevLUDPSocket2MinorVersion,
	xdl::XdevLUDPSocket2PatchVersion
};

xdl::XdevLModuleDescriptor xdl::XdevLTCPSocketImpl::m_TCPSocketModuleDesc {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[1],
	xdl::copyright,
	xdl::descriptionTCP,
	xdl::XdevLTCPSocketMajorVersion,
	xdl::XdevLTCPSocketMinorVersion,
	xdl::XdevLTCPSocketPatchVersion
};

xdl::XdevLPluginDescriptor m_socketPluginDescriptor {
	xdl::pluginName,
	xdl::moduleNames,
	xdl::XdevLSocketPluginMajorVersion,
	xdl::XdevLSocketPluginMinorVersion,
	xdl::XdevLSocketPluginPatchVersion
};

extern "C" XDEVL_EXPORT xdl::XdevLModule* _createModule(const xdl::XdevLPluginDescriptor& pluginDescriptor, const xdl::XdevLModuleDescriptor& moduleDescriptor) {

	if(xdl::XdevLUDPSocketImpl::m_UDPSocketModuleDesc.getName() == moduleDescriptor.getName()) {
		xdl::XdevLUDPSocketImpl* obj = new xdl::XdevLUDPSocketImpl(nullptr);
		if(!obj)
			return nullptr;
		// TODO How to do initialization and shutdown?
		return obj;
	}
	if(xdl::XdevLTCPSocketImpl::m_TCPSocketModuleDesc.getName() == moduleDescriptor.getName()) {
		xdl::XdevLTCPSocketImpl* obj = new xdl::XdevLTCPSocketImpl(nullptr);
		if(!obj)
			return nullptr;
		// TODO How to do initialization and shutdown?
		return obj;
	}
	if(xdl::XdevLUDPSocket2Impl::m_UDPSocket2ModuleDesc.getName() == moduleDescriptor.getName()) {
		xdl::XdevLUDPSocket2Impl* obj = new xdl::XdevLUDPSocket2Impl(nullptr);
		if(!obj)
			return nullptr;
		// TODO How to do initialization and shutdown?
		return obj;
	}


	return nullptr;
}

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	if(xdl::XdevLUDPSocketImpl::m_UDPSocketModuleDesc.getName() == parameter->getModuleName()) {
		xdl::XdevLUDPSocketImpl* obj = new xdl::XdevLUDPSocketImpl(parameter);
		if(!obj)
			return xdl::ERR_ERROR;

		parameter->setModuleInstance(obj);
		return xdl::ERR_OK;
	}
	if(xdl::XdevLTCPSocketImpl::m_TCPSocketModuleDesc.getName() == parameter->getModuleName()) {
		xdl::XdevLTCPSocketImpl* obj = new xdl::XdevLTCPSocketImpl(parameter);
		if(!obj)
			return xdl::ERR_ERROR;

		parameter->setModuleInstance(obj);
		return xdl::ERR_OK;
	}
	if(xdl::XdevLUDPSocket2Impl::m_UDPSocket2ModuleDesc.getName() == parameter->getModuleName()) {
		xdl::XdevLUDPSocket2Impl* obj = new xdl::XdevLUDPSocket2Impl(parameter);
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
	return &m_socketPluginDescriptor;
}

namespace xdl {


	xdl_uint32 htonf(xdl_float f) {
		xdl_uint32 p;
		xdl_uint32 sign;

		if(f < 0) {
			sign = 1;
			f = -f;
		} else {
			sign = 0;
		}

		p = ((((xdl_uint32)f)&0x7fff)<<16) | (sign<<31); // whole part and sign
		p |= (xdl_uint32)(((f - (xdl_int)f) * 65536.0f))&0xffff; // fraction

		return p;
	}

	float ntohf(xdl_uint32 p) {
		float f = (float)((p>>16)&0x7fff); // whole part
		f += (p&0xffff) / 65536.0f; // fraction

		if(((p>>31)&0x1) == 0x1) {
			f = -f;  // sign bit set
		}

		return f;
	}
	/*

	xdl_uint64 pack754(xdl_double f, unsigned bits, unsigned expbits) {
		xdl_double fnorm;
		xdl_int shift;
		xdl_uint64 sign, exp, significand;
		xdl_uint64 significandbits = bits - expbits - 1; // -1 for sign bit

		if (f == 0.0) return 0; // get this special case out of the way

		// check sign and begin normalization
		if (f < 0) {
			sign = 1;
			fnorm = -f;
		} else {
			sign = 0;
			fnorm = f;
		}

		// get the normalized form of f and track the exponent
		shift = 0;
		while (fnorm >= 2.0) {
			fnorm /= 2.0;
			shift++;
		}
		while (fnorm < 1.0) {
			fnorm *= 2.0;
			shift--;
		}
		fnorm = fnorm - 1.0;

		// calculate the binary form (non-float) of the significand data
		significand = fnorm * ((1LL<<significandbits) + 0.5f);

		// get the biased exponent
		exp = shift + ((1<<(expbits-1)) - 1); // shift + bias

		// return the final answer
		return (sign<<(bits-1)) | (exp<<(bits-expbits-1)) | significand;
	}

	xdl_double unpack754(xdl_uint64 i, unsigned bits, unsigned expbits) {
		long double result;
		xdl_int64 shift;
		unsigned bias;
		unsigned significandbits = bits - expbits - 1; // -1 for sign bit

		if (i == 0) return 0.0;

		// pull the significand
		result = (i&((1LL<<significandbits)-1)); // mask
		result /= (1LL<<significandbits); // convert back to float
		result += 1.0f; // add the one back on

		// deal with the exponent
		bias = (1<<(expbits-1)) - 1;
		shift = ((i>>significandbits)&((1LL<<expbits)-1)) - bias;
		while (shift > 0) {
			result *= 2.0;
			shift--;
		}
		while (shift < 0) {
			result /= 2.0;
			shift++;
		}

		// sign it
		result *= (i>>(bits-1))&1? -1.0: 1.0;

		return result;
	}
	 * */
	// packi16() -- store a 16-bit int into a xdl_char buffer (like htons())
	void packi16(xdl_uint8 *buf, xdl_uint16 i) {
		*buf++ = (xdl_uint8)(i>>8);
		*buf++ = (xdl_uint8)i;
	}

	// packi32() -- store a 32-bit int into a xdl_char buffer (like htonl())
	void packi32(xdl_uint8 *buf, xdl_uint32 i) {
		*buf++ = (xdl_uint8)(i>>24);
		*buf++ = (xdl_uint8)(i>>16);
		*buf++ = (xdl_uint8)(i>>8);
		*buf++ = (xdl_uint8)(i);
	}

	// packi64() -- store a 32-bit int into a xdl_char buffer (like htonl())
	void packi64(xdl_uint8 *buf, xdl_uint64 i) {
		*buf++ = (xdl_uint8)(i>>56);
		*buf++ = (xdl_uint8)(i>>48);
		*buf++ = (xdl_uint8)(i>>40);
		*buf++ = (xdl_uint8)(i>>32);
		*buf++ = (xdl_uint8)(i>>24);
		*buf++ = (xdl_uint8)(i>>16);
		*buf++ = (xdl_uint8)(i>>8);
		*buf++ = (xdl_uint8)(i);
	}

	// unpacki16() -- unpack a 16-bit int from a xdl_char buffer (like ntohs())
	xdl_uint16 unpacki16(xdl_uint8 *buf) {
		return (xdl_uint16)((buf[0]<<8) | buf[1]);
	}


	// unpacki32() -- unpack a 32-bit int from a xdl_char buffer (like ntohl())
	xdl_uint32 unpacki32(xdl_uint8 *buf) {
		return (xdl_uint32)((buf[0]<<24) | (buf[1]<<16) | (buf[2]<<8) | buf[3]);
	}

	// unpacki64() -- unpack a 64-bit int from a xdl_char buffer.
	xdl_uint64 unpacki64(xdl_uint8 *buf) {
		return (xdl_uint64)(((xdl_uint64)(buf[0])<<56) |
		                    ((xdl_uint64)(buf[1])<<48) |
		                    ((xdl_uint64)(buf[2])<<40) |
		                    ((xdl_uint64)(buf[3])<<32) |
		                    (xdl_uint64)(buf[4]<<24) |
		                    (xdl_uint64)(buf[5]<<16) |
		                    (xdl_uint64)(buf[6]<<8)  |
		                    (xdl_uint64)(buf[7]));
	}

	XdevLSocketImpl::XdevLSocketImpl() : m_socket(XdevLInvalidSocket) {

	}


	void XdevLSocketImpl::destroy() {
		XdevLCloseSocket(m_socket);
	}

	xdl_int XdevLSocketImpl::init() {
#ifdef _WIN32
		// Check for winsocket 2.2
		if(WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0) {
			std::cerr << "## -> WSAStartup failed.\n";
			return ERR_ERROR;
		}
#endif
		return ERR_OK;
	}

	xdl_int XdevLSocketImpl::shutdown() {
#ifdef _WIN32
		if(WSACleanup() == -1) {
			std::cerr << "## -> WSACleanup failed.\n";
			return ERR_ERROR;
		}
#endif
		return ERR_OK;
	}

	xdl_int XdevLSocketImpl::close() {
		if(m_socket != XdevLInvalidSocket) {
			if(XdevLCloseSocket(m_socket) != 0) {
				return getError();
			} else
				m_socket = XdevLInvalidSocket;
		} else {
			return getError();
		}
		return ERR_OK;
	}

	xdl_int XdevLSocketImpl::flush() {
		return ERR_ERROR;
	}

	xdl_int XdevLSocketImpl::getError() {
		return XdevLSocketGetLastError();
	}


	xdl_int XdevLSocketImpl::setOpt(XdevLSocketOptionParam& opt) {
		switch(opt.option) {
			case XDEVL_SO_BROADCAST: {
				int ret = 0;
				xdl_int* tmp = static_cast<xdl_int*>(opt.value);
				if((ret = setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (xdl_char*)tmp, sizeof(xdl_int))) == XdevLSocketError) {
//			opt.error = getError();
					return ERR_ERROR;
				}

			}
			break;
			case XDEVL_SO_RCVBUF: {
				int ret = 0;
				xdl_int* tmp = static_cast<xdl_int*>(opt.value);
				if((ret = setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (xdl_char*)tmp, sizeof(xdl_int))) == XdevLSocketError) {
//			opt.error = getError();
					return ERR_ERROR;
				}
			}
			break;
			case XDEVL_SO_SNDBUF: {
				int ret = 0;
				xdl_int* tmp = static_cast<xdl_int*>(opt.value);
				if((ret = setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (xdl_char*)tmp, sizeof(xdl_int))) == XdevLSocketError) {
//			opt.error = getError();
					return ERR_ERROR;
				}
			}
			break;
			case XDEVL_SO_RCVTIMEO: {
				int ret = 0;
//				struct timeval tv;
				if((ret = setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (xdl_char*)opt.value, sizeof(struct timeval))) == XdevLSocketError) {
//			opt.error = getError();
					return ERR_ERROR;
				}
			}
			break;
			case XDEVL_SO_SNDTIMEO: {
				int ret = 0;
//				struct timeval tv;
				if((ret = setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (xdl_char*)opt.value, sizeof(struct timeval))) == XdevLSocketError) {
//			opt.error = getError();
					return ERR_ERROR;
				}
			}
			break;
			case XDEVL_SO_NONBLOCKING: {
				xdl_int* tmp = static_cast<xdl_int*>(opt.value);
				if(*tmp == 1) {
#ifdef WIN32
					int ret = 0;
					unsigned long ul = 1;
					if((ret = ioctlsocket(m_socket, FIONBIO, &ul)) == XdevLSocketError) {
						return ERR_ERROR;
					}
#else
					fcntl(m_socket, F_SETFL, O_NONBLOCK);
#endif
				} else {
#ifdef WIN32
					int ret = 0;
					unsigned long ul = 0;
					if((ret = ioctlsocket(m_socket, FIONBIO, &ul)) == XdevLSocketError) {
						return ERR_ERROR;
					}
#else
					//	fcntl(m_socket, F_SETFL, O_NONBLOCK);
#endif
				}
			}
			break;
			case XDEVL_SO_REUSEADDR: {
				xdl_int ret = 0;
				xdl_int* tmp = static_cast<xdl_int*>(opt.value);
				if((ret = setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (xdl_char*)tmp, sizeof(xdl_int))) == XdevLSocketError) {
//			opt.error = getError();
					return ERR_ERROR;
				}
			}
			case XDEVL_TCP_NODELAY: {
				xdl_int ret = 0;
				xdl_int* tmp = static_cast<xdl_int*>(opt.value);
				if((ret = setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (xdl_char*)tmp, sizeof(xdl_int))) == XdevLSocketError) {
//			opt.error = getError();
					return ERR_ERROR;
				}
			}
			break;
			case XDEVL_SO_DEBUG: {
				xdl_int ret = 0;
				xdl_int* tmp = static_cast<xdl_int*>(opt.value);
				if((ret = setsockopt(m_socket, IPPROTO_TCP, SO_DEBUG, (xdl_char*)tmp, sizeof(xdl_int))) == XdevLSocketError) {
//			opt.error = getError();
					return ERR_ERROR;
				}
			}
			break;
			case XDEVL_SO_LINGER: {
				xdl_int ret = 0;
				xdl_int* tmp = static_cast<xdl_int*>(opt.value);
				if((ret = setsockopt(m_socket, IPPROTO_TCP, SO_LINGER, (xdl_char*)tmp, sizeof(xdl_int))) == XdevLSocketError) {
//			opt.error = getError();
					return ERR_ERROR;
				}
			}
			break;

			default:
				;
		}//switch

		return ERR_OK;
	}


	xdl_int XdevLSocketImpl::receiveArray(xdl_uint8* dst, XdevLArrayDeclaration& decl) {

		ssize_t 		ret 													{0};
		xdl_uint8 	src[SOCKET_RECV_BUFFER_SIZE]	{0};
		xdl_uint8*	destination										{dst};
		xdl_uint8* 	source												{src};
		xdl_uint16 	num_elements									{0};

		if((ret = ::recv(m_socket,
		                 (xdl_char*)src,
		                 SOCKET_RECV_BUFFER_SIZE,
		                 0)) == -1) {
			return -1;
		} else {

			decl.clear();

			num_elements = unpacki16(source);

			source += sizeof(xdl_uint16);

			for(auto idx = 0; idx < num_elements; ++idx) {
				XdevLArrayElement array_element = (XdevLArrayElement)unpacki16(source);
				source+= sizeof(xdl_uint16);
				xdl_int32 num_items = unpacki32(source);
				source += sizeof(xdl_uint32);

				decl.add(array_element, num_items);

			}
		}



		for(auto idx = 0; idx < decl.getNumber(); ++idx) {

			xdl::xdl_int16 	element 	= decl.get(idx).element;
			xdl_uint32 			number		= decl.get(idx).number;

			switch(element) {
				case ARRAY_UCHAR_ELEMENT:
				case ARRAY_CHAR_ELEMENT:
				case ARRAY_UINT8_ELEMENT:
				case ARRAY_INT8_ELEMENT: {
					*((xdl_int8*)destination) = *((xdl_int8*)source);
					destination+=sizeof(xdl_int8)*number;
					source+=sizeof(xdl_int8)*number;
				}
				break;
				case ARRAY_UINT16_ELEMENT: {
					*((xdl_uint16*)destination) = ntohs(*((xdl_uint16*)source));
					destination+=sizeof(xdl_uint16)*number;
					source+=sizeof(xdl_uint16)*number;
				}
				break;
				case ARRAY_INT16_ELEMENT: {
					*((xdl_int16*)destination) = ntohs(*((xdl_uint16*)source));
					destination+=sizeof(xdl_int16)*number;
					source+=sizeof(xdl_int16)*number;
				}
				break;
				case ARRAY_UINT32_ELEMENT: {
					*((xdl_uint32*)destination) = ntohl(*((xdl_uint32*)source));
					destination+=sizeof(xdl_uint32)*number;
					source+=sizeof(xdl_uint32)*number;
				}
				break;
				case ARRAY_INT32_ELEMENT: {
					*((xdl_int32*)destination) = ntohl(*((xdl_int32*)source));
					destination+=sizeof(xdl_int32)*number;
					source+=sizeof(xdl_int32)*number;
				}
				break;
				case ARRAY_UINT64_ELEMENT: {
					*((xdl_uint64*)destination) = (xdl_uint64)unpacki64(source);
					destination+=sizeof(xdl_uint64)*number;
					source+=sizeof(xdl_uint64)*number;
				}
				break;
				case ARRAY_INT64_ELEMENT: {
					*((xdl_int64*)destination) = (xdl_int64)unpacki64(source);
					destination+=sizeof(xdl_int64)*number;
					source+=sizeof(xdl_int64)*number;
				}
				break;
				case ARRAY_FLOAT_ELEMENT: {
					*((xdl_float*)destination) = *((xdl_float*)source);
					destination+=sizeof(xdl_float)*number;
					source+=sizeof(xdl_float)*number;
				}
				break;
				case ARRAY_DOUBLE_ELEMENT: {
					*((xdl_double*)destination) = *((xdl_double*)source);
					destination+=sizeof(xdl_double)*number;
					source+=sizeof(xdl_double)*number;
				}
				break;
				default:
					break;
			}
		}

		return (xdl_int)ret;
	}

	xdl_int XdevLSocketImpl::sendArray(xdl_uint8* src, const XdevLArrayDeclaration& decl) {

		// Is the arraydeclaration structure size bigger than the UDP send buffer
		// size?
		if(SOCKET_SEND_BUFFER_SIZE < decl.getSize()) {
			return -1;
		}

		xdl_uint8		buffer[SOCKET_SEND_BUFFER_SIZE] {0};
		xdl_uint64	buffer_cursor 									{0};
		xdl_uint8* 	dp 															{buffer};
		xdl_uint8* 	sp															{src};

		// Store the number of elements into the buffer.
		//*(xdl_uint16*)buffer = (xdl_uint16)decl.getNumber();
		packi16(buffer, (xdl_uint16)decl.getNumber());
		dp+= sizeof(xdl_uint16);
		buffer_cursor += sizeof(xdl_uint16);

		// Store the elements into the buffer.
		for(xdl_int idx = 0; idx < decl.getNumber(); ++idx) {
			packi16(dp,(xdl_uint16)decl.get(idx).element);
			dp+=sizeof(xdl_uint16);
			buffer_cursor += sizeof(xdl_uint16);
			packi32(dp,(xdl_uint32)decl.get(idx).number);
			dp+=sizeof(xdl_uint32);
			buffer_cursor += sizeof(xdl_uint32);

		}

		for(xdl_int idx = 0; idx < decl.getNumber(); ++idx) {

			xdl::xdl_int16 	element 	= decl.get(idx).element;
			xdl_uint32 			number		= decl.get(idx).number;

			switch(element) {
				case ARRAY_UCHAR_ELEMENT:
				case ARRAY_CHAR_ELEMENT:
				case ARRAY_UINT8_ELEMENT:
				case ARRAY_INT8_ELEMENT: {
					// No conversion needed here.
					*dp = *sp;
					dp+=sizeof(xdl_int8)*number;
					sp+=sizeof(xdl_int8)*number;
					buffer_cursor+=sizeof(xdl_int8)*number;
				}
				break;
				case ARRAY_UINT16_ELEMENT: {
					*((xdl_uint16*)dp) = htons(*((xdl_uint16*)sp));
					dp+=sizeof(xdl_uint16)*number;
					sp+=sizeof(xdl_uint16)*number;
					buffer_cursor+=sizeof(xdl_uint16)*number;
				}
				break;
				case ARRAY_INT16_ELEMENT: {
					*((xdl_int16*)dp) = htons(*((xdl_int16*)sp));
					dp+=sizeof(xdl_int16)*number;
					sp+=sizeof(xdl_int16)*number;
					buffer_cursor+=sizeof(xdl_int16)*number;
				}
				break;
				case ARRAY_UINT32_ELEMENT: {
					*((xdl_uint32*)dp) = htonl(*((xdl_int32*)sp));
					dp+=sizeof(xdl_uint32)*number;
					sp+=sizeof(xdl_uint32)*number;
					buffer_cursor+=sizeof(xdl_uint32)*number;
				}
				break;
				case ARRAY_INT32_ELEMENT: {
					*((xdl_int32*)dp) = htonl(*((xdl_int32*)sp));
					dp+=sizeof(xdl_int32)*number;
					sp+=sizeof(xdl_int32)*number;
					buffer_cursor+=sizeof(xdl_int32)*number;
				}
				break;
				case ARRAY_UINT64_ELEMENT: {
					packi64(dp, *(sp));
					dp+=sizeof(xdl_uint64)*number;
					sp+=sizeof(xdl_uint64)*number;
					buffer_cursor+=sizeof(xdl_uint64)*number;
				}
				break;
				case ARRAY_INT64_ELEMENT: {
					packi64(dp, *(sp));
					dp+=sizeof(xdl_int64)*number;
					sp+=sizeof(xdl_int64)*number;
					buffer_cursor+=sizeof(xdl_int64)*number;
				}
				break;
				case ARRAY_FLOAT_ELEMENT: {
					memcpy(dp,sp,sizeof(xdl_float)*number);
					//packi32(dp, val);
					dp+=sizeof(xdl_float)*number;
					sp+=sizeof(xdl_float)*number;
					buffer_cursor+=sizeof(xdl_float)*number;
				}
				break;
				case ARRAY_DOUBLE_ELEMENT: {
					memcpy(dp,sp,sizeof(xdl_double)*number);
					//packi32(dp, val);
					dp+=sizeof(xdl_double)*number;
					sp+=sizeof(xdl_double)*number;
					buffer_cursor+=sizeof(xdl_double)*number;
				}
				break;
				default:
					break;
			};

		}
		int ret = 0;
//		if((ret = ::send(m_socket,
//		                 (xdl_char*)buffer,
//		                 buffer_cursor,
//		                 0)) == XdevLSocketError) {
//			return -1;
//		}
		
		socklen_t tmpsize = sizeof(m_connection);
		if((ret = ::sendto(m_socket,
		                   (xdl_char*)buffer,
		                   buffer_cursor,
		                   0,
		                   (struct sockaddr*)(&m_connection),
		                   tmpsize))  == XdevLSocketError) {
			return -1;
		}

		return (xdl_int)ret;
	}


// -----------------------------------------------------------------------------

	void XdevLUDPSocketBase::destroy() {

	}

	XdevLUDPSocketBase::XdevLUDPSocketBase() :
		m_initialized(false) {
	}

	xdl_int XdevLUDPSocketBase::init() {
		if(XdevLSocketImpl::init() != ERR_OK)
			return ERR_ERROR;

		return open();
	}

	xdl_int XdevLUDPSocketBase::shutdown() {

		// TODO: Do not ignore the error handling of the close method.
		xdl_int ret = XdevLUDPSocketBase::close();

		return (ret == ERR_ERROR) ? ret : XdevLSocketImpl::shutdown();
	}

	xdl_int XdevLUDPSocketBase::open() {

		// If we have a valid socket close it.
		if(m_socket != XdevLInvalidSocket) {
			return close();
		}

		m_socket = ::socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(XdevLInvalidSocket == m_socket) {
			return ERR_ERROR;
		}
		m_initialized = true;
		return ERR_OK;
	}

	xdl_int XdevLUDPSocketBase::open(const XdevLFileName& name) {
		// TODO
		return ERR_ERROR;
	}

	xdl_int XdevLUDPSocketBase::close() {
		if(xdl_false == m_initialized)
			return ERR_OK;

		if(XdevLSocketImpl::close() != ERR_OK)
			return ERR_ERROR;

		m_initialized = false;

		return ERR_OK;
	}

	xdl_int  XdevLUDPSocketBase::flush() {
		// TODO
		return XdevLSocketImpl::flush();
	}

	xdl_int XdevLUDPSocketBase::connect(const XdevLHostName& host, const XdevLPort& port) {
		m_connection.sin_port					= htons(port);
		m_connection.sin_family				= AF_INET;
		// We need information about the server's location and port
		memset(m_connection.sin_zero, '\0', sizeof(m_connection.sin_zero));

		// Check first if we use broadcast.
		if(host != XdevLHostName("broadcast")) {
			in_addr_t hostAdress = inet_addr(host);
			if(!XdevLValidInetAddr(hostAdress)) {
				struct hostent* host_name = gethostbyname(host);
				if(!host_name) {
					// XDEVL_MODULE_ERROR("Hostname resolution failed\n");
					return ERR_ERROR;
				} else {
					// We have to convert a host name into a host adress with 4 digits. this is done here
					memcpy(&hostAdress,host_name->h_addr,host_name->h_length);
				}
			}
			m_connection.sin_addr.s_addr	= hostAdress;
		} else {
			// Set the socket option to broadcast mode.
			xdl::xdl_int val = 1;
			XdevLSocketOptionParam param(XDEVL_SO_BROADCAST, &val);
			// Use the broadcast IP address.
			m_connection.sin_addr.s_addr = htonl(INADDR_BROADCAST);
		}

//		if(::connect(m_socket, (struct sockaddr*)&m_connection, sizeof(m_connection)) == -1) {
//			return ERR_ERROR;
//		}

		return ERR_OK;
	}


	xdl_int XdevLUDPSocketBase::bind(const XdevLPort& port, const XdevLHostName& host) {

		m_connection.sin_port					= htons(port);
		m_connection.sin_family				= AF_INET;
		if(host == XdevLHostName())
			m_connection.sin_addr.s_addr	= htonl(INADDR_ANY);
		else {
			in_addr_t hostAdress = inet_addr(host);
			if(!XdevLValidInetAddr(hostAdress)) {
				struct hostent* host_name = gethostbyname(host);
				if(!host_name) {
					std::cerr << "Hostname resolution failed.\n";
					return ERR_ERROR;
				} else {
					// We have to convert a host name into a host adress with 4 digits. this is done here

					memcpy(&hostAdress,host_name->h_addr,host_name->h_length);
					m_connection.sin_addr.s_addr	= hostAdress;
				}
			}
		}
		// We need information about the server's location and port
		memset(m_connection.sin_zero, '\0', sizeof(m_connection.sin_zero));

		// Now we bind that socked to a port number.
		if(::bind(m_socket, (struct sockaddr *)&m_connection, sizeof(sockaddr_in)) == XdevLSocketError) {
			return ERR_ERROR;
		}
		return ERR_OK;
	}

	xdl_int XdevLUDPSocketBase::write(xdl_uint8* buffer, xdl_int size) {
		ssize_t ret = 0;
		/*
		if ((ret = ::send(m_socket, (xdl_char*)buffer, size, 0)) == XdevLSocketError) {
			return -1;
		}
		*/
		socklen_t tmpsize = sizeof(m_connection);
		if((ret = ::sendto(m_socket,
		                   (xdl_char*)buffer,
		                   size,
		                   0,
		                   (struct sockaddr*)(&m_connection),
		                   tmpsize))  == XdevLSocketError) {
			return -1;
		}

		return (xdl_int)ret;
	}


	xdl_int XdevLUDPSocketBase::read(xdl_uint8* dst, xdl_int size) {
		ssize_t ret = 0;
		socklen_t tmpsize = sizeof(m_connection);
		if((ret = ::recvfrom(m_socket,
		                     (xdl_char*)dst,
		                     size,
		                     0,
		                     (sockaddr*)(&m_connection),
		                     &tmpsize))  == XdevLSocketError) {
			return -1;
		}
		return (xdl_int)ret;
	}

	xdl_int XdevLUDPSocketBase::receiveArray(XdevLIPv4AddressPtr& ip, xdl_uint8* dst, XdevLArrayDeclaration& decl) {
		XdevLIPv4AddressImpl* tmp = static_cast<XdevLIPv4AddressImpl*>(ip.get());
		if(NULL == tmp)
			return ERR_ERROR;

		ssize_t ret = 0;
		xdl_uint8 src[SOCKET_RECV_BUFFER_SIZE] = {0};
		xdl_uint8*	dp = dst;
		xdl_uint8* sp = src;
		xdl_uint16 num_elements = 0;

		socklen_t tmpsize = tmp->size();

		if((ret = ::recvfrom(m_socket,
		                     (xdl_char*)src,
		                     SOCKET_RECV_BUFFER_SIZE,
		                     0,
		                     (sockaddr*)(tmp->sockAddress()),
		                     &tmpsize)) == -1) {
			std::cout << XdevLSocketGetLastError() << std::endl;
			return -1;
		} else {
			num_elements = unpacki16(sp);
			sp += sizeof(xdl_uint16);
			for(xdl_int idx = 0; idx < num_elements; ++idx) {
				XdevLArrayElement array_element = (XdevLArrayElement)unpacki16(sp);
				sp+= sizeof(xdl_uint16);
				xdl_int32 num_items = unpacki32(sp);
				sp += sizeof(xdl_uint32);

				m_decl.add(array_element, num_items);

			}
		}

		for(xdl_int idx = 0; idx < decl.getNumber(); ++idx) {
			xdl::xdl_int16 element = decl.get(idx).element;
			xdl_int32 number = decl.get(idx).number;
			switch(element) {
				case ARRAY_UINT8_ELEMENT: {
					memcpy((void*)dp, (void*)sp, sizeof(xdl_uint8)*number);
					dp+=sizeof(xdl_int8)*number;
					sp+=sizeof(xdl_int8)*number;
				}
				break;
				case ARRAY_INT8_ELEMENT: {
					memcpy((void*)dp, (void*)sp, sizeof(xdl_int8)*number);
					dp+=sizeof(xdl_int8)*number;
					sp+=sizeof(xdl_int8)*number;
				}
				break;
				case ARRAY_UINT16_ELEMENT: {
					xdl_uint16 val = (xdl_uint16)unpacki16(sp);
					memcpy((void*)dp, (void*)&val, sizeof(xdl_uint16)*number);
					dp+=sizeof(xdl_uint16)*number;
					sp+=sizeof(xdl_uint16)*number;
				}
				break;
				case ARRAY_INT16_ELEMENT: {
					xdl_int16 val = (xdl_int16)unpacki16(sp);
					memcpy((void*)dp, (void*)&val, sizeof(xdl_int16)*number);
					dp+=sizeof(xdl_int16)*number;
					sp+=sizeof(xdl_int16)*number;
				}
				break;
				case ARRAY_UINT32_ELEMENT: {
					xdl_uint32 val = (xdl_uint32)unpacki32(sp);
					memcpy((void*)dp, (void*)&val, sizeof(xdl_uint32)*number);
					dp+=sizeof(xdl_uint32)*number;
					sp+=sizeof(xdl_uint32)*number;
				}
				break;
				case ARRAY_INT32_ELEMENT: {
					xdl_int32 val = (xdl_int32)unpacki32(sp);
					memcpy((void*)dp, (void*)&val, sizeof(xdl_int32)*number);
					dp+=sizeof(xdl_int32)*number;
					sp+=sizeof(xdl_int32)*number;
				}
				break;
				case ARRAY_UINT64_ELEMENT: {
					xdl_uint64 val = (xdl_uint64)unpacki64(sp);
					memcpy((void*)dp, (void*)&val, sizeof(xdl_uint64)*number);
					dp+=sizeof(xdl_uint64)*number;
					sp+=sizeof(xdl_uint64)*number;
				}
				break;
				case ARRAY_INT64_ELEMENT: {
					memcpy((void*)dp, (void*)sp, sizeof(xdl_int64)*number);
					dp+=sizeof(xdl_int64)*number;
					sp+=sizeof(xdl_int64)*number;
				}
				break;
				case ARRAY_FLOAT_ELEMENT: {
					memcpy((void*)dp, (void*)sp, sizeof(xdl_float)*number);
					dp+=sizeof(xdl_float)*number;
					sp+=sizeof(xdl_float)*number;
				}
				break;
				case ARRAY_DOUBLE_ELEMENT: {
					memcpy((void*)dp, (void*)sp, sizeof(xdl_double)*number);
					dp+=sizeof(xdl_double)*number;
					sp+=sizeof(xdl_double)*number;
				}
				break;
				default:
					;
			};
		}

		return (xdl_int)ret;
	}

	xdl_int XdevLUDPSocketBase::receiveArray(xdl_uint8* dst, XdevLArrayDeclaration& decl) {

		ssize_t 		ret 													{0};
		xdl_uint8 	src[SOCKET_RECV_BUFFER_SIZE]	{0};
		xdl_uint8*	destination										{dst};
		xdl_uint8* 	source												{src};
		xdl_uint16 	num_elements									{0};
		
		socklen_t tmpsize = sizeof(m_connection);
		if((ret = ::recvfrom(m_socket,
		                     (xdl_char*)src,
		                     SOCKET_RECV_BUFFER_SIZE,
		                     0,
		                     (sockaddr*)(&m_connection),
		                     &tmpsize)) == -1) {
			std::cout << XdevLSocketGetLastError() << std::endl;
			return -1;
		} else {

			decl.clear();

			num_elements = unpacki16(source);

			source += sizeof(xdl_uint16);

			for(auto idx = 0; idx < num_elements; ++idx) {
				XdevLArrayElement array_element = (XdevLArrayElement)unpacki16(source);
				source+= sizeof(xdl_uint16);
				xdl_int32 num_items = unpacki32(source);
				source += sizeof(xdl_uint32);

				decl.add(array_element, num_items);

			}
		}



		for(auto idx = 0; idx < decl.getNumber(); ++idx) {

			xdl::xdl_int16 	element 	= decl.get(idx).element;
			xdl_uint32 			number		= decl.get(idx).number;

			switch(element) {
				case ARRAY_UCHAR_ELEMENT:
				case ARRAY_CHAR_ELEMENT:
				case ARRAY_UINT8_ELEMENT:
				case ARRAY_INT8_ELEMENT: {
					*((xdl_int8*)destination) = *((xdl_int8*)source);
					destination+=sizeof(xdl_int8)*number;
					source+=sizeof(xdl_int8)*number;
				}
				break;
				case ARRAY_UINT16_ELEMENT: {
					*((xdl_uint16*)destination) = ntohs(*((xdl_uint16*)source));
					destination+=sizeof(xdl_uint16)*number;
					source+=sizeof(xdl_uint16)*number;
				}
				break;
				case ARRAY_INT16_ELEMENT: {
					*((xdl_int16*)destination) = ntohs(*((xdl_uint16*)source));
					destination+=sizeof(xdl_int16)*number;
					source+=sizeof(xdl_int16)*number;
				}
				break;
				case ARRAY_UINT32_ELEMENT: {
					*((xdl_uint32*)destination) = ntohl(*((xdl_uint32*)source));
					destination+=sizeof(xdl_uint32)*number;
					source+=sizeof(xdl_uint32)*number;
				}
				break;
				case ARRAY_INT32_ELEMENT: {
					*((xdl_int32*)destination) = ntohl(*((xdl_int32*)source));
					destination+=sizeof(xdl_int32)*number;
					source+=sizeof(xdl_int32)*number;
				}
				break;
				case ARRAY_UINT64_ELEMENT: {
					*((xdl_uint64*)destination) = (xdl_uint64)unpacki64(source);
					destination+=sizeof(xdl_uint64)*number;
					source+=sizeof(xdl_uint64)*number;
				}
				break;
				case ARRAY_INT64_ELEMENT: {
					*((xdl_int64*)destination) = (xdl_int64)unpacki64(source);
					destination+=sizeof(xdl_int64)*number;
					source+=sizeof(xdl_int64)*number;
				}
				break;
				case ARRAY_FLOAT_ELEMENT: {
					*((xdl_float*)destination) = *((xdl_float*)source);
					destination+=sizeof(xdl_float)*number;
					source+=sizeof(xdl_float)*number;
				}
				break;
				case ARRAY_DOUBLE_ELEMENT: {
					*((xdl_double*)destination) = *((xdl_double*)source);
					destination+=sizeof(xdl_double)*number;
					source+=sizeof(xdl_double)*number;
				}
				break;
				default:
					break;
			}
		}

		return (xdl_int)ret;
	}


	xdl_int XdevLUDPSocketBase::sendArray(xdl_uint8* src, const XdevLArrayDeclaration& decl) {

		// Is the arraydeclaration structure size bigger than the UDP send buffer
		// size?
		if(SOCKET_SEND_BUFFER_SIZE < decl.getSize()) {
			return -1;
		}

		xdl_uint8		buffer[SOCKET_SEND_BUFFER_SIZE] {0};
		xdl_uint64	buffer_cursor 									{0};
		xdl_uint8* 	dp 															{buffer};
		xdl_uint8* 	sp															{src};

		// Store the number of elements into the buffer.
		//*(xdl_uint16*)buffer = (xdl_uint16)decl.getNumber();
		packi16(buffer, (xdl_uint16)decl.getNumber());
		dp+= sizeof(xdl_uint16);
		buffer_cursor += sizeof(xdl_uint16);

		// Store the elements into the buffer.
		for(xdl_int idx = 0; idx < decl.getNumber(); ++idx) {
			packi16(dp,(xdl_uint16)decl.get(idx).element);
			dp+=sizeof(xdl_uint16);
			buffer_cursor += sizeof(xdl_uint16);
			packi32(dp,(xdl_uint32)decl.get(idx).number);
			dp+=sizeof(xdl_uint32);
			buffer_cursor += sizeof(xdl_uint32);

		}

		for(xdl_int idx = 0; idx < decl.getNumber(); ++idx) {

			xdl::xdl_int16 	element 	= decl.get(idx).element;
			xdl_uint32 			number		= decl.get(idx).number;

			switch(element) {
				case ARRAY_UCHAR_ELEMENT:
				case ARRAY_CHAR_ELEMENT:
				case ARRAY_UINT8_ELEMENT:
				case ARRAY_INT8_ELEMENT: {
					// No conversion needed here.
					*dp = *sp;
					dp+=sizeof(xdl_int8)*number;
					sp+=sizeof(xdl_int8)*number;
					buffer_cursor+=sizeof(xdl_int8)*number;
				}
				break;
				case ARRAY_UINT16_ELEMENT: {
					*((xdl_uint16*)dp) = htons(*((xdl_uint16*)sp));
					dp+=sizeof(xdl_uint16)*number;
					sp+=sizeof(xdl_uint16)*number;
					buffer_cursor+=sizeof(xdl_uint16)*number;
				}
				break;
				case ARRAY_INT16_ELEMENT: {
					*((xdl_int16*)dp) = htons(*((xdl_int16*)sp));
					dp+=sizeof(xdl_int16)*number;
					sp+=sizeof(xdl_int16)*number;
					buffer_cursor+=sizeof(xdl_int16)*number;
				}
				break;
				case ARRAY_UINT32_ELEMENT: {
					*((xdl_uint32*)dp) = htonl(*((xdl_int32*)sp));
					dp+=sizeof(xdl_uint32)*number;
					sp+=sizeof(xdl_uint32)*number;
					buffer_cursor+=sizeof(xdl_uint32)*number;
				}
				break;
				case ARRAY_INT32_ELEMENT: {
					*((xdl_int32*)dp) = htonl(*((xdl_int32*)sp));
					dp+=sizeof(xdl_int32)*number;
					sp+=sizeof(xdl_int32)*number;
					buffer_cursor+=sizeof(xdl_int32)*number;
				}
				break;
				case ARRAY_UINT64_ELEMENT: {
					packi64(dp, *(sp));
					dp+=sizeof(xdl_uint64)*number;
					sp+=sizeof(xdl_uint64)*number;
					buffer_cursor+=sizeof(xdl_uint64)*number;
				}
				break;
				case ARRAY_INT64_ELEMENT: {
					packi64(dp, *(sp));
					dp+=sizeof(xdl_int64)*number;
					sp+=sizeof(xdl_int64)*number;
					buffer_cursor+=sizeof(xdl_int64)*number;
				}
				break;
				case ARRAY_FLOAT_ELEMENT: {
					memcpy(dp,sp,sizeof(xdl_float)*number);
					//packi32(dp, val);
					dp+=sizeof(xdl_float)*number;
					sp+=sizeof(xdl_float)*number;
					buffer_cursor+=sizeof(xdl_float)*number;
				}
				break;
				case ARRAY_DOUBLE_ELEMENT: {
					memcpy(dp,sp,sizeof(xdl_double)*number);
					//packi32(dp, val);
					dp+=sizeof(xdl_double)*number;
					sp+=sizeof(xdl_double)*number;
					buffer_cursor+=sizeof(xdl_double)*number;
				}
				break;
				default:
					break;
			};

		}

		socklen_t tmpsize = sizeof(m_connection);
		ssize_t ret = 0;
		if((ret = ::sendto(m_socket,
		                   (xdl_char*)buffer,
		                   buffer_cursor,
		                   0,
		                   (sockaddr*)(&m_connection),
		                   tmpsize)) == XdevLSocketError) {
			return -1;
		}


		return (xdl_int)ret;
	}

	xdl_int XdevLUDPSocketBase::sendArray(XdevLIPv4AddressPtr& ip, xdl_uint8* src, const XdevLArrayDeclaration& decl) {

		XdevLIPv4AddressImpl* tmp = static_cast<XdevLIPv4AddressImpl*>(ip.get());
		if(nullptr == tmp) {
			return ERR_ERROR;
		}

		// Is the arraydeclaration structure size bigger than the UDP send buffer
		// size?
		if(SOCKET_SEND_BUFFER_SIZE < decl.getSize()) {
			return -1;
		}

		xdl_uint8 	buffer[SOCKET_SEND_BUFFER_SIZE] {0};
		xdl_uint64	buffer_cursor 									{0};
		xdl_uint8* 	dp 															{buffer};
		xdl_uint8* 	sp 															{src};

		// Store the number of elements into the buffer.
		//*(xdl_uint16*)buffer = (xdl_uint16)decl.getNumber();
		packi16(buffer, (xdl_uint16)decl.getNumber());
		dp+= sizeof(xdl_uint16);
		buffer_cursor += sizeof(xdl_uint16);

		// Store the elements into the buffer.
		for(xdl_int idx = 0; idx < decl.getNumber(); ++idx) {
			packi16(dp,(xdl_uint16)decl.get(idx).element);
			dp+=sizeof(xdl_uint16);
			buffer_cursor += sizeof(xdl_uint16);
			packi32(dp,(xdl_uint32)decl.get(idx).number);
			dp+=sizeof(xdl_uint32);
			buffer_cursor += sizeof(xdl_uint32);
		}

		for(xdl_int idx = 0; idx < decl.getNumber(); ++idx) {
			xdl::xdl_int16 element = decl.get(idx).element;
			xdl_int32 number = decl.get(idx).number;
			switch(element) {
				case ARRAY_UCHAR_ELEMENT: {
					memcpy(dp,sp,sizeof(xdl_uchar)*number);
					dp+=sizeof(xdl_uchar)*number;
					sp+=sizeof(xdl_uchar)*number;
					buffer_cursor+=sizeof(xdl_uchar)*number;
				}
				break;
				case ARRAY_CHAR_ELEMENT: {
					memcpy(dp,sp,sizeof(xdl_char)*number);
					dp+=sizeof(xdl_char)*number;
					sp+=sizeof(xdl_char)*number;
					buffer_cursor+=sizeof(xdl_char)*number;
				}
				break;
				case ARRAY_UINT8_ELEMENT: {
					memcpy(dp,sp,sizeof(xdl_uint8)*number);
					dp+=sizeof(xdl_uint8)*number;
					sp+=sizeof(xdl_uint8)*number;
					buffer_cursor+=sizeof(xdl_uint8)*number;
				}
				break;
				case ARRAY_INT8_ELEMENT: {
					memcpy(dp,sp,sizeof(xdl_int8)*number);
					dp+=sizeof(xdl_int8)*number;
					sp+=sizeof(xdl_int8)*number;
					buffer_cursor+=sizeof(xdl_int8)*number;
				}
				break;
				case ARRAY_UINT16_ELEMENT: {
					memcpy(dp,sp,sizeof(xdl_uint16)*number);
					packi16(dp, *(dp));
					dp+=sizeof(xdl_uint16)*number;
					sp+=sizeof(xdl_uint16)*number;
					buffer_cursor+=sizeof(xdl_uint16)*number;
				}
				break;
				case ARRAY_INT16_ELEMENT: {
					memcpy(dp,sp,sizeof(xdl_int16)*number);
					packi16(dp, *(dp));
					dp+=sizeof(xdl_int16)*number;
					sp+=sizeof(xdl_int16)*number;
					buffer_cursor+=sizeof(xdl_int16)*number;
				}
				break;
				case ARRAY_UINT32_ELEMENT: {
					memcpy(dp,sp,sizeof(xdl_uint32)*number);
					packi32(dp, *(dp));
					dp+=sizeof(xdl_uint32)*number;
					sp+=sizeof(xdl_uint32)*number;
					buffer_cursor+=sizeof(xdl_uint32)*number;
				}
				break;
				case ARRAY_INT32_ELEMENT: {
					memcpy(dp,sp,sizeof(xdl_int32)*number);
					packi32(dp, *(dp));
					dp+=sizeof(xdl_int32)*number;
					sp+=sizeof(xdl_int32)*number;
					buffer_cursor+=sizeof(xdl_int32)*number;
				}
				break;
				case ARRAY_UINT64_ELEMENT: {
					memcpy(dp,sp,sizeof(xdl_uint64)*number);
					packi64(dp, *(dp));
					dp+=sizeof(xdl_uint64)*number;
					sp+=sizeof(xdl_uint64)*number;
					buffer_cursor+=sizeof(xdl_uint64)*number;
				}
				break;
				case ARRAY_INT64_ELEMENT: {
					memcpy(dp,sp,sizeof(xdl_int64)*number);
					//packi64(dp, *(dp));
					dp+=sizeof(xdl_int64)*number;
					sp+=sizeof(xdl_int64)*number;
					buffer_cursor+=sizeof(xdl_int64)*number;
				}
				break;
				case ARRAY_FLOAT_ELEMENT: {
					memcpy(dp,sp,sizeof(xdl_float)*number);
					//packi32(dp, val);
					dp+=sizeof(xdl_float)*number;
					sp+=sizeof(xdl_float)*number;
					buffer_cursor+=sizeof(xdl_float)*number;
				}
				break;
				case ARRAY_DOUBLE_ELEMENT: {
					memcpy(dp,sp,sizeof(xdl_double)*number);
					//packi32(dp, val);
					dp+=sizeof(xdl_double)*number;
					sp+=sizeof(xdl_double)*number;
					buffer_cursor+=sizeof(xdl_double)*number;
				}
				break;


				default:
					break;
			};

		}
		ssize_t ret = 0;
		if((ret = ::sendto(m_socket,
		                   (xdl_char*)buffer,
		                   buffer_cursor,
		                   0,
		                   (sockaddr*)(tmp->sockAddress()),
		                   tmp->size())) == XdevLSocketError) {
			return -1;
		}

		return (xdl_int)ret;
	}



	xdl_int XdevLUDPSocketBase::getError() {
		return XdevLSocketImpl::getError();
	}


// --------------------------------------------------------------------------

	XdevLUDPSocketImpl::XdevLUDPSocketImpl(XdevLModuleCreateParameter* parameter) :
		XdevLModuleImpl<XdevLUDPSocket>(parameter, m_UDPSocketModuleDesc) {
	}

	XdevLUDPSocketImpl::~XdevLUDPSocketImpl() {
		XdevLCloseSocket(m_socket);
	}

	xdl_int XdevLUDPSocketImpl::init() {
		return XdevLUDPSocketBase::init();
	}

	xdl_int XdevLUDPSocketImpl::shutdown() {

		xdl_int ret = XdevLUDPSocketImpl::close();

		return ((ret == ERR_ERROR) ? ERR_ERROR : XdevLSocketImpl::shutdown());
	}

	xdl_int XdevLUDPSocketImpl::close() {
		if(!m_initialized)
			return ERR_OK;

		if(XdevLSocketImpl::close() != ERR_OK)
			return ERR_ERROR;

		m_initialized = false;

		return ERR_OK;
	}

	xdl_int XdevLUDPSocketImpl::connect(const XdevLHostName& host, const XdevLPort& port) {
		m_connection.sin_port					= htons(port);
		m_connection.sin_family				= AF_INET;
		// We need information about the server's location and port
		memset(m_connection.sin_zero, '\0', sizeof(m_connection.sin_zero));

		in_addr_t hostAdress = inet_addr(host);
		if(!XdevLValidInetAddr(hostAdress)) {
			struct hostent* host_name = gethostbyname(host);
			if(!host_name) {
				XDEVL_MODULE_ERROR("Hostname resolution failed\n");
				return ERR_ERROR;
			} else {
				// We have to convert a host name into a host adress with 4 digits. this is done here
				memcpy(&hostAdress,host_name->h_addr,host_name->h_length);
			}
		}
		m_connection.sin_addr.s_addr	= hostAdress;

//		if(::connect(m_socket, (struct sockaddr*)&m_connection, sizeof(m_connection)) == -1) {
//			XDEVL_MODULE_ERROR("connection error: " << XdevLSocketGetLastError() << std::endl);
//			return ERR_ERROR;
//		}

		return ERR_OK;
	}


	xdl_int XdevLUDPSocketImpl::bind(const XdevLPort& port, const XdevLHostName& host) {

		m_connection.sin_port					= htons(port);
		m_connection.sin_family				= AF_INET;
		if(!host)
			m_connection.sin_addr.s_addr	= htonl(INADDR_ANY);
		else {
			unsigned int hostAdress = inet_addr(host);
			if(!XdevLValidInetAddr(hostAdress)) {
				struct hostent* host_name = gethostbyname(host);
				if(!host_name) {
					std::cerr << "Hostname resolution failed.\n";
					return ERR_ERROR;
				} else {
					// We have to convert a host name into a host adress with 4 digits. this is done here

					memcpy(&hostAdress,host_name->h_addr,host_name->h_length);
					m_connection.sin_addr.s_addr	= hostAdress;
				}
			}
		}
		// We need information about the server's location and port
		memset(m_connection.sin_zero, '\0', sizeof(m_connection.sin_zero));

		// Now we bind that socked to a port number.
		if(::bind(m_socket, (struct sockaddr *)&m_connection, sizeof(sockaddr_in)) == XdevLSocketError) {
			return ERR_ERROR;
		}
		return ERR_OK;
	}

	xdl_int XdevLUDPSocketImpl::write(xdl_uint8* buffer, xdl_int size) {
		ssize_t ret = 0;
		if((ret = ::sendto(m_socket, (xdl_char*)buffer, size, 0, (sockaddr*)(&m_connection), sizeof(m_connection))) == XdevLSocketError) {
			return -1;
		}
		return (xdl_int)ret;
	}


	xdl_int XdevLUDPSocketImpl::read(xdl_uint8* dst, xdl_int size) {
		socklen_t tmpsize = sizeof(m_connection);
		ssize_t ret = 0;
		if((ret =  ::recvfrom(m_socket, (xdl_char*)dst, size, 0, (sockaddr*)(&m_connection), &tmpsize)) == XdevLSocketError) {
			return -1;
		}
		return (xdl_int)ret;
	}

	xdl_int XdevLUDPSocketImpl::getError() {
		return XdevLSocketImpl::getError();
	}


	xdl_int XdevLUDPSocketImpl::setOpt(XdevLSocketOptionParam& opt) {
		return XdevLSocketImpl::setOpt(opt);
	}

	xdl_int XdevLUDPSocketImpl::open(const XdevLFileName& name) {
		return XdevLUDPSocketBase::open(name);
	}

	xdl_int XdevLUDPSocketImpl::open() {
		return XdevLUDPSocketBase::open();
	}

	xdl_int XdevLUDPSocketImpl::flush() {
		return XdevLUDPSocketBase::flush();
	}




	XdevLTCPSocketImpl::XdevLTCPSocketImpl(XdevLModuleCreateParameter* parameter) :
		XdevLModuleImpl<XdevLTCPSocket>(parameter, m_TCPSocketModuleDesc),
		m_acceptedConnection(XdevLInvalidSocket),
		m_mode(0),
		m_initialized(false) {
	}

	void XdevLTCPSocketImpl::destroy() {

	}

	xdl_int XdevLTCPSocketImpl::getMode() {
		return m_mode;
	}

	xdl_int XdevLTCPSocketImpl::init() {
		if(XdevLSocketImpl::init() != ERR_OK)
			return ERR_ERROR;

		// If we have a valid socket close it.
		if(m_socket != XdevLInvalidSocket) {
			close();
		}
		// Create a TCP socket.
		m_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(XdevLInvalidSocket == m_socket) {
			std::cout << "# -> Could not create SOCK_STREAM.\n";
			return ERR_ERROR;
		}
		m_initialized = true;
		return ERR_OK;
	}

	xdl_int XdevLTCPSocketImpl::shutdown() {

		return XdevLSocketImpl::shutdown();
	}

	xdl_int XdevLTCPSocketImpl::open() {
		// TODO
		return ERR_ERROR;
	}

	xdl_int XdevLTCPSocketImpl::open(const XdevLFileName& name) {
		// TODO
		return ERR_ERROR;
	}

	xdl_int XdevLTCPSocketImpl::close() {
		if(!m_initialized)
			return ERR_OK;

		if(m_acceptedConnection == XdevLInvalidSocket) {
			XDEVL_MODULE_ERROR("Accepted connection is invalid. Cannot close.\n");
		} else {
			XdevLCloseSocket(m_acceptedConnection);
		}

		if(XdevLSocketImpl::close() != ERR_OK)
			return ERR_OK;

		m_initialized = false;
		return ERR_OK;
	}

	xdl_int XdevLTCPSocketImpl::flush() {
		return ERR_ERROR;
	}

	xdl_int XdevLTCPSocketImpl::connect(const XdevLHostName& host, const XdevLPort& port) {
		m_connection.sin_port					= htons(port);
		m_connection.sin_family				= AF_INET;
		// We need information about the server's location and port
		memset(m_connection.sin_zero, '\0', sizeof(m_connection.sin_zero));

		in_addr_t hostAdress = inet_addr(host);
		if(!XdevLValidInetAddr(hostAdress)) {
			struct hostent* host_name = gethostbyname(host);
			if(!host_name) {
				XDEVL_MODULE_ERROR("Hostname resolution failed\n");
				return ERR_ERROR;
			} else {
				// We have to convert a host name into a host adress with 4 digits. this is done here
				memcpy(&hostAdress,host_name->h_addr,host_name->h_length);
			}
		}
		m_connection.sin_addr.s_addr	= hostAdress;

		if(::connect(m_socket, (struct sockaddr*)&m_connection, sizeof(m_connection)) == -1) {
			XDEVL_MODULE_ERROR("connection error: " << XdevLSocketGetLastError() << std::endl);
			return ERR_ERROR;
		}
		return ERR_OK;
	}

	xdl_int XdevLTCPSocketImpl::bind(const XdevLPort& port, const XdevLHostName& host) {

		m_connection.sin_port					= htons(port);
		m_connection.sin_family				= AF_INET;
		if(!host)
			m_connection.sin_addr.s_addr	= INADDR_ANY;
		else {
			in_addr_t hostAdress = inet_addr(host);
			if(!XdevLValidInetAddr(hostAdress)) {
				struct hostent* host_name = gethostbyname(host);
				if(!host_name) {
					XDEVL_MODULE_ERROR("Hostname resolution failed.\n");
					return ERR_ERROR;
				} else {
					// We have to convert a host name into a host adress with 4 digits. this is done here

					memcpy(&hostAdress,host_name->h_addr,host_name->h_length);
					m_connection.sin_addr.s_addr	= hostAdress;
				}
			}
		}
		// We need information about the server's location and port
		memset(m_connection.sin_zero, '\0', sizeof(m_connection.sin_zero));

		// Now we bind that socked to a port number.
		if(::bind(m_socket, (struct sockaddr *)&m_connection, sizeof(sockaddr_in)) == -1) {
			XDEVL_MODULE_ERROR("Bind failed.\n");
			return ERR_ERROR;
		}
		return ERR_OK;
	}


	xdl_int XdevLTCPSocketImpl::startServer(const XdevLHostName& host, const XdevLPort& port, xdl_int backlog) {
		if(bind(port, host) != ERR_OK) {
			return ERR_ERROR;
		}
		if(XdevLSocketError == ::listen(m_socket, backlog)) {
			return ERR_ERROR;
		}
		m_mode = 1;
		return ERR_OK;
	}

	xdl_int XdevLTCPSocketImpl::accept() {
		struct sockaddr_in l_sock_addr;
		socklen_t size = sizeof(l_sock_addr);
		if((m_acceptedConnection = ::accept(m_socket, (struct sockaddr*)&l_sock_addr, &size)) == XdevLInvalidSocket) {
			XDEVL_MODULE_ERROR("Accept failed.\n");
			return ERR_ERROR;
		}
		m_acceptedConnections = l_sock_addr;
		XDEVL_MODULE_INFO("Connection accepted from IP: " << inet_ntoa(l_sock_addr.sin_addr) << ", Port: " << ntohs(l_sock_addr.sin_port) << std::endl);

		return ERR_OK;
	}

	xdl_int XdevLTCPSocketImpl::write(xdl_uint8* buffer, xdl_int size) {
		ssize_t ret = 0;
		if(m_acceptedConnection != XdevLInvalidSocket) {
			if((ret = ::send(m_acceptedConnection, (char*)buffer, size, 0)) == XdevLSocketError) {
				return -1;
			}
		} else {
			if((ret =::send(m_socket, (char*)buffer, size, 0)) == XdevLSocketError) {
				return -1;
			}
		}
		return (xdl_int)ret;
	}

	xdl_int XdevLTCPSocketImpl::read(xdl_uint8* buffer, xdl_int size) {
		ssize_t ret = 0;
		if(m_acceptedConnection != XdevLInvalidSocket) {
			if((ret = ::recv(m_acceptedConnection, (char*)buffer, size, 0)) == XdevLSocketError) {
				return -1;
			}
		} else {
			if((ret = ::recv(m_socket, (char*)buffer, size, 0)) == XdevLSocketError) {
				return -1;
			}
		}
		return (xdl_int)ret;
	}

	xdl_int XdevLTCPSocketImpl::setOpt(XdevLSocketOptionParam& opt) {
		return XdevLSocketImpl::setOpt(opt);
	}


	xdl_int XdevLTCPSocketImpl::getError() {
		return XdevLSocketImpl::getError();
	}

	xdl_int XdevLTCPSocketImpl::receiveArray(xdl_uint8* dst, XdevLArrayDeclaration& decl) {
		return XdevLSocketImpl::receiveArray(dst, decl);
	}
	xdl_int XdevLTCPSocketImpl::sendArray(xdl_uint8* src, const XdevLArrayDeclaration& decl) {
		return XdevLSocketImpl::sendArray(src, decl);
	}


//
// XdevLUDPSocket2
//

	void XdevLUDPSocket2Impl::destroy() {
		XdevLUDPSocketBase::destroy();
	}

	xdl_int XdevLUDPSocket2Impl::connect(XdevLIPv4AddressPtr& addr) {
		XdevLIPv4AddressImpl* tmp = static_cast<XdevLIPv4AddressImpl*>(addr.get());

		if(::connect(m_socket, (struct sockaddr*)tmp->sockAddress(), tmp->size()) == -1) {
			return ERR_ERROR;
		}
		return ERR_OK;
	}

	xdl_int XdevLUDPSocket2Impl::getAddress(XdevLIPv4AddressPtr& addr, const XdevLPort& port, const XdevLHostName& host) {
		XdevLIPv4AddressImpl* addr_tmp = new XdevLIPv4AddressImpl();
		if(addr_tmp->init(port, host) != ERR_OK)
			return ERR_ERROR;
		XdevLIPv4AddressPtr tmp(addr_tmp);
		addr = tmp;
		return ERR_OK;
	}

	xdl_int XdevLUDPSocket2Impl::getAddress(XdevLIPv4AddressPtr& addr) {
		XdevLIPv4AddressPtr tmp(new XdevLIPv4AddressImpl());
		addr = tmp;
		return ERR_OK;
	}


	xdl_int XdevLUDPSocket2Impl::bind(XdevLIPv4AddressPtr& ip) {
		XdevLIPv4AddressImpl* tmp = static_cast<XdevLIPv4AddressImpl*>(ip.get());
		if(NULL == tmp)
			return ERR_ERROR;
		// Now we bind that socked to a port number.
		if(::bind(m_socket, (struct sockaddr *)tmp->sockAddress(), sizeof(sockaddr_in)) == XdevLSocketError) {
			return ERR_ERROR;
		}
		return ERR_OK;
	}


	xdl_int XdevLUDPSocket2Impl::send(XdevLIPv4AddressPtr& ip, xdl_uint8* src, xdl_int size) {
		XdevLIPv4AddressImpl* tmp = static_cast<XdevLIPv4AddressImpl*>(ip.get());
		if(NULL == tmp || tmp->valid() == xdl_false)
			return ERR_ERROR;

		ssize_t ret = 0;
		if((ret = ::sendto(m_socket, (xdl_char*)src, size, 0, (sockaddr*)(tmp->sockAddress()), tmp->size())) == XdevLSocketError) {
			return -1;
		}
		return (xdl_int)ret;
	}

	xdl_int XdevLUDPSocket2Impl::receive(XdevLIPv4AddressPtr& ip, xdl_uint8* dst, xdl_int size) {

		struct sockaddr_in tmp;
		socklen_t tmpsize = sizeof(tmp);
		ssize_t ret = 0;
		if((ret =  ::recvfrom(m_socket, (xdl_char*)dst, size, 0, (sockaddr*)(&tmp), &tmpsize)) == XdevLSocketError) {
			return -1;
		}
		// Check if the user has a valid IP Address object.
		if(ip.get() != NULL)
			static_cast<XdevLIPv4AddressImpl*>(ip.get())->sockAddress(tmp);
		else {
			XdevLIPv4AddressImpl* addr_tmp = new XdevLIPv4AddressImpl();
			if(addr_tmp->init(tmp) != ERR_OK)
				return -1;
			ip = XdevLIPv4AddressPtr(addr_tmp);
		}
		return (xdl_int)ret;
	}

	xdl_int XdevLUDPSocket2Impl::receiveArray(XdevLIPv4AddressPtr& ip, xdl_uint8* dst, XdevLArrayDeclaration& decl) {
		return XdevLUDPSocketBase::receiveArray(ip, dst, decl);
	}

	xdl_int XdevLUDPSocket2Impl::receiveArray(xdl_uint8* dst, XdevLArrayDeclaration& decl) {
		return XdevLUDPSocketBase::receiveArray(dst, decl);
	}

	xdl_int XdevLUDPSocket2Impl::sendArray(xdl_uint8* src, const XdevLArrayDeclaration& decl) {
		return XdevLUDPSocketBase::sendArray(src, decl);
	}

	xdl_int  XdevLUDPSocket2Impl::sendArray(XdevLIPv4AddressPtr& ip, xdl_uint8* src, const XdevLArrayDeclaration& decl) {
		return XdevLUDPSocketBase::sendArray(ip, src, decl);
	}

	XdevLUDPSocket2Impl::XdevLUDPSocket2Impl(XdevLModuleCreateParameter* parameter) :
		XdevLModuleImpl<XdevLUDPSocket2>(parameter, m_UDPSocket2ModuleDesc) {
	}

	xdl_int XdevLUDPSocket2Impl::setOpt(XdevLSocketOptionParam& opt) {
		return XdevLSocketImpl::setOpt(opt);
	}
	xdl_int XdevLUDPSocket2Impl::connect(const XdevLHostName& host, const XdevLPort& port) {
		return XdevLUDPSocketBase::connect(host, port);
	}

	xdl_int XdevLUDPSocket2Impl::open() {
		return XdevLUDPSocketBase::open();
	}

	xdl_int XdevLUDPSocket2Impl::open(const XdevLFileName& name) {
		return XdevLUDPSocketBase::open(name);
	}

	xdl_int XdevLUDPSocket2Impl::close() {
		return XdevLUDPSocketBase::close();
	}

	xdl_int XdevLUDPSocket2Impl::bind(const XdevLPort& port, const XdevLHostName& host) {
		return XdevLUDPSocketBase::bind(port, host);
	}

	xdl_int XdevLUDPSocket2Impl::write(xdl_uint8* buffer, xdl_int size) {
		return XdevLUDPSocketBase::write(buffer, size);
	}

	xdl_int XdevLUDPSocket2Impl::read(xdl_uint8* buffer, xdl_int size) {
		return XdevLUDPSocketBase::read(buffer, size);
	}

	xdl_int XdevLUDPSocket2Impl::getError() {
		return XdevLSocketImpl::getError();
	}

	xdl_int XdevLUDPSocket2Impl::init() {
		return XdevLUDPSocketBase::init();
	}

	xdl_int XdevLUDPSocket2Impl::flush() {
		return XdevLUDPSocketBase::flush();
	}

	xdl_int XdevLUDPSocket2Impl::shutdown() {

		xdl_int ret = close();

		return ((ret == ERR_ERROR) ? ERR_ERROR : XdevLSocketImpl::shutdown());
	}





}
