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

#ifndef XDEVL_SOCKET_H
#define XDEVL_SOCKET_H

#include <XdevLStream.h>
#include <XdevLTypes.h>
#include <XdevLError.h>

#include <memory>

namespace xdl {

	class XdevLArrayDeclaration;

	/**
		@struct XdevLHostName
		@brief Holds a hostname.
	*/
	class XdevLHostName : public XdevLString {
		public:
			XdevLHostName() : XdevLString() {}

			explicit XdevLHostName(const char* t) : XdevLString(t) {}

			explicit XdevLHostName(const std::string& t) : XdevLString(t) {}

			XdevLHostName& operator = (XdevLString::type t) {
				m_value = t;
				return *this;
			}

	};

	/**
		@struct XdevLHostName
		@brief Holds a port number.
	*/
	class XdevLPort {
		public:
			explicit XdevLPort(xdl_int port) : m_port(port)	{}

			operator const xdl_int& () const {
				return m_port;
			}

		private:
			xdl_int m_port;
	};


	/**
		@struct XdevLIPv4Address
		@brief Holds a IPv4 address.
	*/
	struct XdevLIPv4Address {

		virtual ~XdevLIPv4Address() {}

		/// Returns the host name.
		/**
			@return Returns a null terminated string which represents the host name of the address.
		*/
		virtual const XdevLHostName& hostname() const = 0;

		/// Returns the port.
		/**
			@return Returns the port number of the address.
		*/
		virtual const XdevLPort& port() const = 0;

		/// Returns the validity of the address.
		/**
			@return Returns @b 'xdl_true' if the address is valid otherwise it is @b 'xdl_false'
		*/
		virtual xdl_bool valid() = 0;

	};

	/**
		@struct XdevLIPv6Address
		@brief Holds a IPv6 address.
	*/
	struct XdevLIPv6Address {

		virtual ~XdevLIPv6Address() {}

		/// Returns the host name.
		/**
			@return Returns a null terminated string which represents the host name of the address.
		*/
		virtual const XdevLHostName& hostname() const = 0;

		/// Returns the port.
		/**
			@return Returns the port number of the address.
		*/
		virtual const XdevLPort& port() const = 0;

		/// Returns the validity of the address.
		/**
			@return Returns @b 'xdl_true' if the address is valid otherwise it is @b 'xdl_false'
		*/
		virtual xdl_bool valid() = 0;

	};

	typedef XdevLIPv4Address*	IPXdevLIPv4Address;
	typedef XdevLIPv4Address	IXdevLIPv4Address;

	typedef XdevLIPv6Address*	IPXdevLIPv6Address;
	typedef XdevLIPv6Address	IXdevLIPv6Address;

	typedef std::shared_ptr<XdevLIPv4Address> XdevLIPv4AddressPtr;
	typedef std::shared_ptr<XdevLIPv6Address> XdevLIPv6AddressPtr;

	/**
		@enum XdevLSocketOption
		@brief Socket option parameters

	*/
	enum XdevLSocketOption {
	    /// enables recording of debugging information
	    XDEVL_SO_DEBUG,
	    /// enables permission to transmit broadcast messages
	    XDEVL_SO_BROADCAST,
	    /// enables local address reuse
	    XDEVL_SO_REUSEADDR,
	    /// enables duplicate address and port bindings
	    XDEVL_SO_REUSEPORT,
	    /// enables keep connections alive
	    XDEVL_SO_KEEPALIVE,
	    /// enables routing bypass for outgoing messages
	    XDEVL_SO_DONTROUTE,
	    /// linger on close if data present
	    XDEVL_SO_LINGER,
	    /// enables reception of out-of-band data in band
	    XDEVL_SO_OOBINLINE,
	    /// set buffer size for output
	    XDEVL_SO_SNDBUF,
	    /// set buffer size for input
	    XDEVL_SO_RCVBUF,
	    /// set minimum count for input
	    XDEVL_SO_RCVLOWAT,
	    /// set timeout value for input
	    XDEVL_SO_RCVTIMEO,
	    /// set minimum count for output
	    XDEVL_SO_SNDLOWAT,
	    /// set timeout value for output
	    XDEVL_SO_SNDTIMEO,
	    /// get and clear error on the socket (get only)
	    XDEVL_SO_ERROR,
	    /// get the type of the socket (get only)
	    XDEVL_SO_TYPE,
	    /// do not generate SIGPIPE, instead return EPIPE
	    XDEVL_SO_NOSIGPIPE,

	    /// Only XdevL specific
	    XDEVL_SO_NONBLOCKING,

	    /// do not use the
	    XDEVL_TCP_NODELAY
	};

	/**
		@enum XdevLSocketError
		@brief Socket error codes

	*/
	enum XdevLSocketError {
	    XDEVL_EPERM								= 1,
	    XDEVL_ENOENT							= 2,   // No such file or directory
	    XDEVL_ESRCH								= 3,   // No such process
	    XDEVL_EINTR								= 4,   // The system call was interrupted by a signal that was caught
	    XDEVL_EIO									= 5,   // I/O error
	    XDEVL_ENXIO								= 6,   // No such device or address
	    XDEVL_E2BIG								= 7,   // Argument list too long
	    XDEVL_ENOEXEC 						= 8,   // Exec format error
	    XDEVL_EBADF								= 9,   // The file descriptor is not a valid index in the descriptor table
	    XDEVL_ECHILD 							= 10,  // No child processes
	    XDEVL_EAGAIN 							= 11,  // Try again
	    XDEVL_ENOMEM 							= 12,  // Out of memory
	    XDEVL_EACCES 							= 13,  // Permission to create a socket of the specified type and/or protocol is denied.
	    XDEVL_EFAULT 							= 14,  // The socket structure address is outside the user’s address space
	    XDEVL_ENOTBLK							= 15,  // Block device required
	    XDEVL_EBUSY								= 16,  // Device or resource busy
	    XDEVL_EEXIST							= 17,  // File exists
	    XDEVL_EXDEV								= 18,  // Cross-device link
	    XDEVL_ENODEV							= 19,  // No such device
	    XDEVL_ENOTDIR							= 20,  // Not a directory
	    XDEVL_EISDIR 							= 21,  // Is a directory
	    XDEVL_EINVAL 							= 22,  // Invalid argument
	    XDEVL_ENFILE 							= 23,  // File table overflow
	    XDEVL_EMFILE 							= 24,  // Too many open files
	    XDEVL_ENOTTY 							= 25,  // Not a typewriter
	    XDEVL_ETXTBSY 						= 26,  // Text file busy
	    XDEVL_EFBIG								= 27,  // File too large
	    XDEVL_ENOSPC							= 28,  // No space left on device
	    XDEVL_ESPIPE							= 29,  // Illegal seek
	    XDEVL_EROFS								= 30,  // Read-only file system
	    XDEVL_EMLINK							= 31,  // Too many links
	    XDEVL_EPIPE								= 32,  // Broken pipe
	    XDEVL_EWOULDBLOCK					= 33,  // Math argument out of domain of func
	    XDEVL_EINPROGRESS					= 34,  // The socket is non-blocking and the connection cannot be completed immediately
	    XDEVL_EALREADY						= 35,	 // Operation already in progress
	    XDEVL_ENOTSOCK						= 36,	 // The file descriptor is not associated with a socket
	    XDEVL_EDESTADDRREQ				= 37,
	    XDEVL_EMSGSIZE						= 38,
	    XDEVL_EPROTOTYPE					= 39,
	    XDEVL_ENOPROTOOPT					= 40,
	    XDEVL_EPROTONOSUPPORT 		= 41,
	    XDEVL_ESOCKTNOSUPPORT 		= 42,
	    XDEVL_EOPNOTSUPP					= 43,
	    XDEVL_EPFNOSUPPORT				= 44,
	    XDEVL_EAFNOSUPPORT				= 45,
	    XDEVL_EADDRINUSE					= 46,
	    XDEVL_EADDRNOTAVAIL				= 47,
	    XDEVL_ENETDOWN						= 48,
	    XDEVL_ENETUNREACH					= 49, // Network is unreachable
	    XDEVL_ENETRESET						= 50,
	    XDEVL_ECONNABORTED				= 51,
	    XDEVL_ECONNRESET					= 52,
	    XDEVL_ENOBUFS							= 53,
	    XDEVL_EISCONN							= 54, // The socket is already connected
	    XDEVL_ENOTCONN						= 55,
	    XDEVL_ESHUTDOWN						= 56,
	    XDEVL_ETOOMANYREFS				= 57,
	    XDEVL_ETIMEDOUT						= 58, // Timeout while attempting connection
	    XDEVL_ECONNREFUSED				= 59, // No-one listening on the remote address
	    XDEVL_ELOOP								= 60,
	    XDEVL_ENAMETOOLONG				= 61,
	    XDEVL_EHOSTDOWN						= 62,
	    XDEVL_EHOSTUNREACH				= 63,
	    XDEVL_ENOTEMPTY						= 64,
	    XDEVL_EPROCLIM						= 65,
	    XDEVL_EUSERS							= 66,
	    XDEVL_EDQUOT							= 67,
	    XDEVL_ESTALE							= 68,
	    XDEVL_EREMOTE							= 69,
	    XDEVL_ENOSTR							= 70,
	    XDEVL_ETIME								= 71,
	    XDEVL_ENOSR								= 72,
	    XDEVL_ENOMSG							= 73,
	    XDEVL_EBADMSG							= 74,
	    XDEVL_EIDRM								= 75,
	    XDEVL_EDEADLK							= 76,
	    XDEVL_ENOLCK							= 77,
	    XDEVL_ENONET							= 78,
	    XDEVL_ERREMOTE						= 79,
	    XDEVL_ENOLINK							= 80,
	    XDEVL_EADV								= 81,
	    XDEVL_ESRMNT							= 82,
	    XDEVL_ECOMM								= 83,
	    XDEVL_EPROTO							= 84,
	    XDEVL_EMULTIHOP						= 85,
	    XDEVL_EDOTDOT							= 86,
	    XDEVL_EREMCHG							= 87,
	    XDEVL_EIBMBADCALL					= 88,
	    XDEVL_EIBMBADPARM					= 89,
	    XDEVL_EIBMSOCKOUTOFRANGE	= 90,
	    XDEVL_EIBMSOCKINUSE				=	91,
	    XDEVL_EIBMIUCVERR					= 1004,	// Request failed due to IUCV error
	    XDEVL_EIBMCONFLICT				= 1008,	// Conflicting call already outstanding on socket
	    XDEVL_EIBMCANCELLED				= 1009	// Request canceled via socket function CANCEL request
	};

	/**
		@struct XdevLSocketOptionParam
		@brief The parameter for socket options.
	*/
	struct XdevLSocketOptionParam {
		XdevLSocketOptionParam(XdevLSocketOption opt, void* val) : option(opt), value(val), error(ERR_ERROR) {
		}
		/// Holds the option
		XdevLSocketOption option;
		/// Holds the value for the option
		void*							value;
		/// This is the error return value.
		xdl_int						error;
	};


	/**
		@class XdevLSocket
		@brief Base socket class for network communication.
	*/
	class XdevLSocket : public XdevLStream {
		public:
			virtual ~XdevLSocket() {};
			/// Binds the socket to a port and/or host name.
			/**
				@param[in] port The port address that should be used.
				@param[in] host The host name that should be used. Is not used by default.
				@return Returns ERR_OK if it was successful.
			*/
			virtual xdl_int bind(const XdevLPort& port, const XdevLHostName& host = XdevLHostName()) = 0;

			/// Establishes a connection to the specified host and port address.
			/**
				In the case of UDP sockets this sets the default send address. It is not really connecting. Using
				write() will use this address.
				In the case of TCP the socket will be connected as the TCP protocol expects.
			*/
			virtual xdl_int connect(const XdevLHostName& host, const XdevLPort& port) = 0;

			/// Sets socket options.
			/**
				Here is an example how to change the receive buffer size.

				@code
				// Set the receive buffer to 2000 bytes.
				xdl::xdl_int val = 2000;
				xdl::XdevLSocketOptionParam op(xdl::XDEVL_SO_RCVBUF, &val);
				socket->setOpt(op);
				@endcode

				To see which socket options you can change check the xdl::XdevLSocketOption enums.

				@param opt A valid XdevLSocketOptionParam object.
				@return Returns ERR_OK if it was successful.
			*/
			virtual xdl_int setOpt(XdevLSocketOptionParam& opt) = 0;
	};

	/**
		@class XdevLUDPSocket
		@brief UDP Socket class.

		In order to use the send() and recv() method correctly you have to use first
		the connect() method to provide the remote address.
	*/
	class XdevLUDPSocket : public XdevLSocket {
		public:

			virtual ~XdevLUDPSocket() {};

	};

	/**
		@class XdevLTCPSocket
		@brief TCP Socket class.

		Not supported in XdevL Version 0.1 .
	*/
	class XdevLTCPSocket : public XdevLSocket {
		public:
			virtual ~XdevLTCPSocket() {}
			/// Starts a server.
			/**
				@return Returns
				-# ERR_OK If no error occurs.
				-# ERR_ERROR If an error occurs.
			*/
			virtual xdl_int startServer(const XdevLHostName& host, const XdevLPort& port, xdl_int backlog = 5) = 0;


			/// Accepts a client.
			/**
				@return Returns
				-# ERR_OK If no error occurs.
				-# ERR_ERROR If an error occurs.
			*/
			virtual xdl_int accept() = 0;

			/// Receive array data
			/**
				@param[out] dst The buffer where the array should be stored.
				@param[in] decl The declaration of the structure.
				@return Returns the number of bytes sent if it was successful. Otherwise it will return -1.
			*/
			virtual xdl_int receiveArray(xdl_uint8* dst, XdevLArrayDeclaration& decl) = 0;

			/// Send array data
			/**
				@param[in] src The array buffer to copy from.
				@param[in] decl The declaration of the structure.
				@return Returns the number of bytes sent if it was successful. Otherwise it will return -1.
			*/
			virtual xdl_int sendArray(xdl_uint8* src, const XdevLArrayDeclaration& decl) = 0;


			/// Returns the mode.
			/**
				@return Returns 0 if this socket runs in Client mode, 1 if it runs as a Server.
			*/
			virtual xdl_int getMode() = 0;

			/// Returns the current error code.
			virtual xdl_int getError() = 0;
	};

	/**
		@class XdevLUDPSocket2
		@brief Extended UDP Socket class.
	*/
	class XdevLUDPSocket2 : public XdevLUDPSocket {
		public:
			virtual ~XdevLUDPSocket2() {}
			// Allow acces to overloaded function from the base class withing this class.
			using XdevLUDPSocket::bind;
			using XdevLUDPSocket::read;
			using XdevLUDPSocket::write;
			using XdevLUDPSocket::connect;
			using XdevLUDPSocket::flush;

			/// Establishes a connection to the specified host and port address.
			/**
				@param [out] addr A valid xdl::XdevLIPv4AddressPtr instance.
				@return Returns ERR_OK if it was successful. Otherwise it will return ERR_ERROR. Use getError() for a more detailed error message.
			*/
			virtual xdl_int connect(XdevLIPv4AddressPtr& addr) = 0;

			/// Returns an address for the socket.
			/**
				Returns a XdevLIPv4AddressPtr object which holds the socket address. If the return value is ERR_OK addr
				will have a valid address otherwise it will hold a invalid address.
				@param[out] addr A valid XdevLIPv4AddressPtr object which will hold a valid socket address.
				@param[in] port Port number of the address.
				@param[in] host Host name of the address. host is NULL any IP name will be used.
				@return Returns ERR_OK if it was successful. Otherwise it will return ERR_ERROR. Use getError() for a more detailed error message.
			*/
			virtual xdl_int getAddress(XdevLIPv4AddressPtr& addr, const XdevLPort& port, const XdevLHostName& host = XdevLHostName()) = 0;

			/// Returns an empty valid address object.
			virtual xdl_int getAddress(XdevLIPv4AddressPtr& addr) = 0;

			/// Binds the socket to a port and/or hostname.
			/**
				@param[in] ip Should be a valid IPv4 address of the peer.
				@return Returns ERR_OK if it was successful. Otherwise it will return ERR_ERROR. Use getError() for a more detailed error message.
			*/
			virtual xdl_int bind(XdevLIPv4AddressPtr& ip) = 0;

			/// Sends data.
			/**
				Here an example how to send data.
				@code
				xdl::xdl_char buffer[] = {"Hello, World!"};
				socket->send((xdl::xdl_int8*)(buffer), 13);
				@endcode

				@param[in] ip Should be a valid IPv4 address of the peer.
				@param[in] src The memory buffer to copy from.
				@param[in] size The number of bytes to send.
				@return Returns the number of bytes sent if it was successful. Otherwise it will return -1.
			*/
			virtual xdl_int send(XdevLIPv4AddressPtr& ip, xdl_uint8* src, xdl_int size) = 0;

			/// Receive data.
			/**
				Here an example how to receive data.
				@code
				xdl::xdl_char buffer[13] = {0};
				socket->receive((xdl::xdl_int8*)(buffer), 13);
				@endcode

				@param[in] ip Should be a valid IPv4 address of the peer.
				@param[out] dst The buffer where the message should be stored.
				@param[in] size The length in bytes of the buffer pointed by dst.
				@return Returns the length of the message in bytes if it was successful. If no message
				are available it will return 0. Otherwise it will return -1.
			*/
			virtual xdl_int receive(XdevLIPv4AddressPtr& ip, xdl_uint8* dst, xdl_int size) = 0;

			/// Receive array data
			/**
				@param[out] dst The buffer where the array should be stored.
				@param[in] decl The declaration of the structure.
				@return Returns the number of bytes sent if it was successful. Otherwise it will return -1.
			*/
			virtual xdl_int receiveArray(xdl_uint8* dst, XdevLArrayDeclaration& decl) = 0;

			/// Receive array data
			/**
				@param[in] ip Should be a valid IPv4 address of the peer.
				@param[out] dst The buffer where the array should be stored.
				@param[in] decl The declaration of the structure.
				@return Returns the number of bytes sent if it was successful. Otherwise it will return -1.
			*/
			virtual xdl_int receiveArray(XdevLIPv4AddressPtr& ip, xdl_uint8* dst, XdevLArrayDeclaration& decl) = 0;

			/// Send array data
			/**
				@param[in] src The array buffer to copy from.
				@param[in] decl The declaration of the structure.
				@return Returns the number of bytes sent if it was successful. Otherwise it will return -1.
			*/
			virtual xdl_int sendArray(xdl_uint8* src, const XdevLArrayDeclaration& decl) = 0;

			/// Send array data
			/**
				@param[in] ip Should be a valid IPv4 address of the peer.
				@param[in] src The array buffer to copy from.
				@param[in] decl The declaration of the structure.
				@return Returns the number of bytes sent if it was successful. Otherwise it will return -1.
			*/
			virtual xdl_int sendArray(XdevLIPv4AddressPtr& ip, xdl_uint8* src, const XdevLArrayDeclaration& decl) = 0;

			/// Return the current error code.
			virtual xdl_int getError() = 0;
	};


	typedef XdevLUDPSocket		IXdevLUDPSocket;
	typedef XdevLUDPSocket2		IXdevLUDPSocket2;
	typedef XdevLTCPSocket		IXdevLTCPSocket;
	typedef XdevLUDPSocket2*	IPXdevLUDPSocket2;
	typedef XdevLUDPSocket*		IPXdevLUDPSocket;
	typedef XdevLTCPSocket*		IPXdevLTCPSocket;

}

#endif
