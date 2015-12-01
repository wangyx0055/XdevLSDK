/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2015 Cengiz Terzibas

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

#ifndef XDEVL_SOCKETIMPL_H
#define XDEVL_SOCKETIMPL_H

#include <XdevLPluginImpl.h>
#include <XdevLSocket.h>
#include <XdevLTypes.h>
#include <XdevLPlatform.h>
#include <stack>

namespace xdl {

	// Holds the plugin major version.
	const xdl_uint XdevLSocketPluginMajorVersion = XDEVLNETWORK_MAJOR_VERSION;

	// Holds the plugin minor version.
	const xdl_uint XdevLSocketPluginMinorVersion = XDEVLNETWORK_MINOR_VERSION;

	// Holds the plugin patch version.
	const xdl_uint XdevLSocketPluginPatchVersion = XDEVLNETWORK_PATCH_VERSION;


	// Holds the Major version number.
	const xdl_uint XdevLUDPSocketMajorVersion = XDEVLNETWORK_UDP_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLUDPSocketMinorVersion = XDEVLNETWORK_UDP_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLUDPSocketPatchVersion = XDEVLNETWORK_UDP_MODULE_PATCH_VERSION;


	// Holds the Major version number.
	const xdl_uint XdevLUDPSocket2MajorVersion = XDEVLNETWORK_UDP2_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLUDPSocket2MinorVersion = XDEVLNETWORK_UDP2_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLUDPSocket2PatchVersion = XDEVLNETWORK_UDP2_MODULE_PATCH_VERSION;


	// Holds the Major version number.
	const xdl_uint XdevLTCPSocketMajorVersion = XDEVLNETWORK_TCP_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLTCPSocketMinorVersion = XDEVLNETWORK_TCP_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLTCPSocketPatchVersion = XDEVLNETWORK_TCP_MODULE_PATCH_VERSION;


	static const XdevLString vendor {
		"www.codeposer.net"
	};
	static const XdevLString author {
		"Cengiz Terzibas"
	};
	static const XdevLString copyright {
		"(c) 2005-2015 Cengiz Terzibas."
	};
	static const XdevLString descriptionUDP {
		"UDP socket for connection less oriented communication"
	};
	static const XdevLString descriptionTCP {
		"TCP socket for connection oriented communication"
	};
	static const XdevLString pluginName {
		"XdevLNetwork"
	};
	static const std::vector<XdevLModuleName> moduleNames {
		XdevLModuleName("XdevLUDPSocket"),
		XdevLModuleName("XdevLTCPSocket"),
		XdevLModuleName("XdevLUDPSocket2")
	};

	class XdevLModuleCreateParameter;

	const xdl_int SOCKET_RECV_BUFFER_SIZE = 8192;
	const xdl_int SOCKET_SEND_BUFFER_SIZE = 8192;


#if defined (XDEVL_PLATFORM_WINDOWS) || defined(XDEVL_PLATFORM_MINGW)
#define XdevLSocketGetLastError() WSAGetLastError()
#define XdevLGetInetAddr(host,addrPtr) *addrPtr = inet_addr(host)
#define XdevLValidInetAddr(addr) (addr != INADDR_NONE)
#define XdevLInvalidSocket INVALID_SOCKET
#define XdevLSocketError SOCKET_ERROR
#define XdevLCloseSocket(m_socket) closesocket(m_socket)
	typedef SOCKET XDEVL_SOCKET;
	typedef int ssize_t;
	typedef unsigned long in_addr_t;
#elif XDEVL_PLATFORM_UNIX
#define XdevLSocketGetLastError() errno
#define XdevLGetInetAddr(host,addrPtr) *addrPtr = inet_addr(host)
#define XdevLInvalidSocket -1
#define XdevLValidInetAddr(addr) (addr != -1)
#define XdevLSocketError -1
#define XdevLCloseSocket(m_socket) ::close(m_socket)
	typedef int XDEVL_SOCKET;
#endif


	// host to network float
	xdl_uint32 htonf(float f);
	// network to host float
	float ntohf(xdl_uint32 p);
	// pack long double value
	xdl_uint64 pack754(xdl_double f, unsigned bits, unsigned expbits);
	// unpack long double value
	xdl_double unpack754(xdl_uint64 i, unsigned bits, unsigned expbits);
	// packi16() -- store a 16-bit int into a xdl_char buffer (like htons())
	void packi16(xdl_uint8 *buf, xdl_uint16 i);
	// packi32() -- store a 32-bit int into a xdl_char buffer (like htonl())
	void packi32(xdl_uint8 *buf, xdl_uint32 i);
	// packi64() -- store a 64-bit int into a xdl_char buffer (like ltonl())
	void packi64(xdl_uint8 *buf, xdl_uint64 i);
	// unpacki16() -- unpack a 16-bit int from a xdl_char buffer (like ntohs())
	xdl_uint16 unpacki16(xdl_uint8 *buf);
	// unpacki32() -- unpack a 32-bit int from a xdl_char buffer (like ntohl())
	xdl_uint32 unpacki32(xdl_uint8 *buf);
	// unpacki64() -- unpack a 64-bit int from a xdl_char buffer (like ltohl())
	xdl_uint64 unpacki64(xdl_uint8 *buf);

#define pack754_32(f) (pack754((f), 32, 8))
#define pack754_64(f) (pack754((f), 64, 11))
#define unpack754_32(i) (unpack754((i), 32, 8))
#define unpack754_64(i) (unpack754((i), 64, 11))

	struct XdevLIPv4AddressImpl: public XdevLIPv4Address {

			xdl_int init(struct sockaddr_in connection) {
				m_connection = connection;
				return ERR_OK;
			}

			XdevLIPv4AddressImpl() : m_valid(xdl_false), m_port(-1) {
			}

			xdl_int init(const XdevLPort& port, const XdevLHostName& host) {
				// Set the port address.
				m_connection.sin_port	= htons((xdl_uint16)port);
				// Set the familiy type.
				m_connection.sin_family = PF_INET;

				memset(m_connection.sin_zero, '\0', sizeof(m_connection.sin_zero));
				if(host == XdevLString()) { // OK, the user doesn't want to specify a peer name.
					m_connection.sin_addr.s_addr = htonl(INADDR_ANY);
					m_valid = xdl_true;
				} else {
					m_host = host;
					// Check first if we use broadcast.
					if(strcmp(host, "broadcast") != 0) {
						in_addr_t hostAdress = inet_addr(host);
						if(!XdevLValidInetAddr(hostAdress)) {
							struct hostent* host_name = gethostbyname(host);
							if(!host_name) {
								// XDEVL_MODULE_ERROR("Hostname resolution failed\n");
								return ERR_ERROR;
							} else {
								// We have to convert a host name into a host adress with 4 digits. this is done here
								memcpy(&hostAdress,host_name->h_addr,host_name->h_length);
								m_host = std::string(host);
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
				}
				m_port = port;
				m_valid = true;
				return ERR_OK;
			}
			virtual const XdevLHostName& hostname() const {
				return m_host;
			}
			virtual const XdevLPort&	port() const {
				return m_port;
			}
			sockaddr_in* sockAddress() {
				return &m_connection;
			}
			void sockAddress(sockaddr_in tmp) {
				m_connection = tmp;
				m_valid = true;
			}
			xdl_int size() {
				return sizeof(m_connection);
			}
			xdl_bool valid() {
				return m_valid;
			}

		protected:
			xdl_bool 			m_valid;
			XdevLPort 		m_port;
			XdevLHostName m_host;
			struct sockaddr_in m_connection;
	};

	struct XdevLIpv6AddressImpl: public XdevLIPv6Address {
			virtual const XdevLHostName& hostname() const {
				return m_host;
			}
			virtual const XdevLPort& port() const {
				return m_port;
			}
		protected:
			XdevLHostName m_host;
			XdevLPort 		m_port;
	};


	class XdevLSocketImpl {
		public:
			XdevLSocketImpl();
			virtual void destroy();
			virtual xdl_int init();
			virtual xdl_int shutdown();
			virtual xdl_int close();
			virtual xdl_int flush();
			virtual xdl_int setOpt(XdevLSocketOptionParam& opt);
			virtual xdl_int getError();
			virtual xdl_int receiveArray(xdl_uint8* dst, XdevLArrayDeclaration& decl);
			virtual xdl_int sendArray(xdl_uint8* src, const XdevLArrayDeclaration& decl);

		protected:
#ifdef _WIN32
			WSADATA m_wsaData;
#endif
			XDEVL_SOCKET m_socket;
			// Holds address of the receiver socket.
			struct sockaddr_in m_connection;
			// Holds the received array descriptor.
			XdevLArrayDeclaration m_decl;
	};


	class XdevLUDPSocketBase : public XdevLSocketImpl {
		public:
			XdevLUDPSocketBase();
			virtual void destroy();
			virtual xdl_int open();
			virtual xdl_int open(const XdevLFileName& name);
			virtual xdl_int close();
			virtual xdl_int flush();
			virtual xdl_int connect(const XdevLHostName& host, const XdevLPort& port);
			virtual xdl_int bind(const XdevLPort& port, const XdevLHostName& host);
			virtual xdl_int write(xdl_uint8* buffer, xdl_int size);
			virtual xdl_int sendArray(xdl_uint8* src, const XdevLArrayDeclaration& decl);
			virtual xdl_int sendArray(XdevLIPv4AddressPtr& ip, xdl_uint8* src, const XdevLArrayDeclaration& decl);
			virtual xdl_int read(xdl_uint8* buffer, xdl_int size);
			virtual xdl_int receiveArray(xdl_uint8* dst, XdevLArrayDeclaration& decl);
			virtual xdl_int receiveArray(XdevLIPv4AddressPtr& ip, xdl_uint8* dst, XdevLArrayDeclaration& decl);
			virtual xdl_int getError();
			virtual xdl_int init();
			virtual xdl_int shutdown();

		protected:
			xdl_bool	m_initialized;
	};


	class XdevLUDPSocketImpl : public XdevLUDPSocketBase, public XdevLModuleImpl<XdevLUDPSocket> {
		public:
			XdevLUDPSocketImpl(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLUDPSocketImpl();
			virtual xdl_int setOpt(XdevLSocketOptionParam& opt);
			virtual xdl_int connect(const XdevLHostName& host, const XdevLPort& port);
			virtual xdl_int open();
			virtual xdl_int open(const XdevLFileName& name);
			virtual xdl_int close();
			virtual xdl_int flush();
			virtual xdl_int bind(const XdevLPort& port, const XdevLHostName& host);
			virtual xdl_int write(xdl_uint8* buffer, xdl_int size);
			virtual xdl_int read(xdl_uint8* buffer, xdl_int size);
			virtual xdl_int getError();
			/// Holds the plugins XdevLModuleDescriptor.
			static XdevLModuleDescriptor m_UDPSocketModuleDesc;
			// --------------------------------------------------------------------------
			// XdevLModule function
			//
			virtual xdl_int init();
			virtual xdl_int shutdown();
	};


	class XdevLTCPSocketImpl : public XdevLSocketImpl, public XdevLModuleImpl<XdevLTCPSocket> {
		public:
			XdevLTCPSocketImpl(XdevLModuleCreateParameter* parameter);
			virtual void destroy();
			virtual xdl_int open();
			virtual xdl_int open(const XdevLFileName& name);
			virtual xdl_int close();
			virtual xdl_int flush();
			virtual xdl_int bind(const XdevLPort& port, const XdevLHostName& host);
			virtual xdl_int write(xdl_uint8* buffer, xdl_int size);
			virtual xdl_int read(xdl_uint8* buffer, xdl_int size);
			virtual xdl_int setOpt(XdevLSocketOptionParam& opt);
			virtual xdl_int startServer(const XdevLHostName& host, const XdevLPort& port, xdl_int backlog);
			virtual xdl_int connect(const XdevLHostName& host, const XdevLPort& port);
			virtual xdl_int accept();
			virtual xdl_int receiveArray(xdl_uint8* dst, XdevLArrayDeclaration& decl);
			virtual xdl_int sendArray(xdl_uint8* src, const XdevLArrayDeclaration& decl);

			virtual xdl_int getMode();
			virtual xdl_int getError();

			static XdevLModuleDescriptor m_TCPSocketModuleDesc;
			// --------------------------------------------------------------------------
			// XdevLModule function
			//
			virtual xdl_int init();
			virtual xdl_int shutdown();
		protected:
			XDEVL_SOCKET 									m_acceptedConnection;
			struct sockaddr_in 						m_acceptedConnections;
			xdl_int												m_mode;
			xdl_bool											m_initialized;
	};


	class XdevLUDPSocket2Impl : public XdevLUDPSocketBase, public XdevLModuleImpl<XdevLUDPSocket2> {
		public:
			XdevLUDPSocket2Impl(XdevLModuleCreateParameter* parameter);
			virtual void destroy();

			static XdevLModuleDescriptor m_UDPSocket2ModuleDesc;

			virtual xdl_int connect(XdevLIPv4AddressPtr& addr);
			virtual xdl_int getAddress(XdevLIPv4AddressPtr& addr, const XdevLPort& port, const XdevLHostName& host = XdevLHostName());
			virtual xdl_int getAddress(XdevLIPv4AddressPtr& addr);
			virtual xdl_int bind(XdevLIPv4AddressPtr& ip);
			virtual xdl_int send(XdevLIPv4AddressPtr& ip, xdl_uint8* src, xdl_int size);
			virtual xdl_int receive(XdevLIPv4AddressPtr& ip, xdl_uint8* dst, xdl_int size);

			virtual xdl_int receiveArray(xdl_uint8* dst, XdevLArrayDeclaration& decl);
			virtual xdl_int receiveArray(XdevLIPv4AddressPtr& ip, xdl_uint8* dst, XdevLArrayDeclaration& decl);
			virtual xdl_int sendArray(xdl_uint8* src, const XdevLArrayDeclaration& decl);
			virtual xdl_int sendArray(XdevLIPv4AddressPtr& ip, xdl_uint8* src, const XdevLArrayDeclaration& decl);

			virtual xdl_int setOpt(XdevLSocketOptionParam& opt);
			virtual xdl_int connect(const XdevLHostName& host, const XdevLPort& port);
			virtual xdl_int open();
			virtual xdl_int open(const XdevLFileName& name);
			virtual xdl_int close();
			virtual xdl_int flush();
			virtual xdl_int bind(const XdevLPort& port, const XdevLHostName& host);
			virtual xdl_int write(xdl_uint8* buffer, xdl_int size);
			virtual xdl_int read(xdl_uint8* buffer, xdl_int size);
			virtual xdl_int getError();
			// --------------------------------------------------------------------------
			// XdevLModule function
			//
			virtual xdl_int init();
			virtual xdl_int shutdown();


	};


}

#endif
