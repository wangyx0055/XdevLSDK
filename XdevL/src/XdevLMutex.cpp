/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2014 Cengiz Terzibas

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

#include <XdevLMutex.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <cerrno>

namespace thread {

Mutex::Mutex() {
#if defined (__GNUC__) && !defined (WIN32)
	if ( pthread_mutex_init(&m_mutex, NULL) != 0 ) {
		throw("Mutex::Could not create mutex");
	}
#else
	InitializeCriticalSection(&m_mutex);
#endif
}

Mutex::~Mutex() {
#if defined (__GNUC__) && !defined (WIN32)
	pthread_mutex_destroy(&m_mutex);
#else
	DeleteCriticalSection(&m_mutex);
#endif

}

int Mutex::Lock() {
#if defined (__GNUC__) && !defined (WIN32)
	if (pthread_mutex_lock(&m_mutex) != 0) {
		std::cerr << "Mutex::Could not lock mutex: " << strerror(errno) << std::endl;
		return 1;
	}
#else
	EnterCriticalSection(&m_mutex);
#endif
	return 0;
}

int Mutex::Unlock() {
#if defined (__GNUC__) && !defined (WIN32)
	if (pthread_mutex_unlock(&m_mutex) != 0) {
		std::cerr << "Mutex::Could not unlock mutex: " << strerror(errno) << std::endl;
		return 1;
	}
#else
	LeaveCriticalSection(&m_mutex);
#endif
	return 0;
}

THREAD_MUTEX& Mutex::getNativeMutex() { 
	return m_mutex;
}

}
