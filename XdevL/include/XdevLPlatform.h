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

#ifndef XDEVL_PLATFORM_H
#define XDEVL_PLATFORM_H

#ifdef __MINGW32__
	#define XDEVL_PLATFORM_MINGW 1
#elif defined(_MSC_VER)
	#define XDEVL_PLATFORM_WINDOWS 1
#elif defined(__linux__)
	#define XDEVL_PLATFORM_LINUX 1
	#define XDEVL_PLATFORM_UNIX 1
#elif defined(__APPLE__)
	#define XDEVL_PLATFORM_APPLE 1
	#define XDEVL_PLATFORM_UNIX 1
#elif defined(__unix__) // All unixes, not all compilers.
	#define XDEVL_PLATFORM_UNIX
#elif defined(__DragonFly__)
	#define XDEVL_PLATFORM_DRAGONFLY 1
	#define XDEVL_PLATFORM_UNIX 1
#elif defined(__FreeBSD__)
	#define XDEVL_PLATFORM_FREEBSD 1
	#define XDEVL_PLATFORM_UNIX 1
#elif defined(__NetBSD__)
	#define XDEVL_PLATFORM_NETBSD 1
	#define XDEVL_PLATFORM_UNIX 1
#elif defined(__OpenBSD__)
	#define XDEVL_PLATFORM_OPENBSD 1
	#define XDEVL_PLATFORM_UNIX 1
#else
	#error XDEVL_UNKNOWN_PLATFORM
#endif

#if __STDC_VERSION__ < 199901L
# if __GNUC__ >= 2
#  define __func__ __FUNCTION__
# elif _MSC_VER
#  define __func__ __FUNCTION__
# else 
# define __func__ "<unknown>"
# endif
#endif

#ifdef _MSC_VER
	#pragma warning(disable : 4996)
	#pragma warning(disable : 4100)
	#define XDEVL_EXPORT __declspec(dllexport)
#elif __GNUC__
	#define XDEVL_EXPORT
#endif

#ifdef _WIN32
	// We need this definition to tell the gcc compiler that we need at least the win32 api for WindowsXP
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0600
	#endif
	#include <Winsock2.h>
	#include <Ws2tcpip.h>
	#if defined (XDEVL_PLATFORM_WINDOWS) || defined(XDEVL_PLATFORM_MINGW)
		#include <Wspiapi.h>
	#endif
	#include <Windows.h>
	#include <Mmsystem.h>
	#include <iphlpapi.h>
	#include <Process.h>
	#include <WinBase.h>
	#include <Synchapi.h>
#elif defined (XDEVL_PLATFORM_APPLE)
	#include <dlfcn.h>
	#include <semaphore.h>
	#include <sys/mman.h>
	#include <sys/types.h>	
	#include <netdb.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>	
#elif defined (XDEVL_PLATFORM_UNIX)
	#include <pthread.h>
	#include <semaphore.h>
	#include <sys/mman.h>
	#include <sys/types.h>
	#include <endian.h>
	#include <dlfcn.h>
	#include <netdb.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>
#endif

#include <errno.h>
#include <cerrno>
#include <iostream>

#endif
