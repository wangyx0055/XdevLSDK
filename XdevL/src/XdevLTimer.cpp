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

#include "XdevLTimer.h"

// This is for the MINGW case
#if __GNUG__
#include <sys/time.h>
#endif

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

// -----------------------------------------------------------------------------
// Do timer stuff on Windows
//

#ifdef _WIN32

#include <Windows.h>
#include <time.h>
#include <XdevLTypes.h>

xdl::xdl_uint64 getTimeGlobalMicroSeconds() {
	
	xdl::xdl_int64 Frequency = 0;
	xdl::xdl_int64 CurClock = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency );
	QueryPerformanceCounter((LARGE_INTEGER*)&CurClock);

	return (CurClock/Frequency)*1000000;
}

xdl::xdl_double getTimeGlobal() {
	
	xdl::xdl_int64 Frequency = 0;
	xdl::xdl_int64 CurClock = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency );
	QueryPerformanceCounter((LARGE_INTEGER*)&CurClock);

	return (xdl::xdl_double( CurClock ) / xdl::xdl_double( Frequency ));
}

// -----------------------------------------------------------------------------
// Do timer stuff on UNIX systems.
//
#elif defined __GNUG__ && !defined _WIN32

#include <netinet/in.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

xdl::xdl_uint64 getTimeGlobalMicroSeconds() {
	
	#ifndef __LINUX__
		timeval tp;
		gettimeofday(&tp, NULL);
		return ((xdl::xdl_uint64)tp.tv_sec*1000000 + (xdl::xdl_double)tp.tv_usec);
		
	#elif defined(__LINUX__)
		struct timespec     clock_resolution;
		int stat = clock_getres(CLOCK_MONOTONIC_RAW, &clock_resolution);
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
		return (ts.tv_sec*1000000 + ts.tv_nsec*1000);	
	
	#endif

}

xdl::xdl_double getTimeGlobal() {
	
	#ifndef __LINUX__
		timeval tp;
		gettimeofday(&tp, NULL);
		return ((xdl::xdl_double)tp.tv_sec + (xdl::xdl_double)tp.tv_usec*0.000001);
		
	#elif defined(__LINUX__)
		struct timespec     clock_resolution;
		int stat = clock_getres(CLOCK_MONOTONIC_RAW, &clock_resolution);
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
		return (ts.tv_sec + ts.tv_nsec*0.000000001);	
	
	#endif

}

#endif

namespace xdl {

XdevLTimer::XdevLTimer() : 
	m_StartingTime(0), 
	m_CurrTime(0), 
	m_OldTime(0) {
	
	reset();
}


xdl::xdl_double XdevLTimer::getDT() {
	xdl_double dT = (xdl_double)(getTimeGlobalMicroSeconds() - m_OldTime)*0.000001;
	m_OldTime = getTimeGlobalMicroSeconds();
	return dT;
}

xdl::xdl_uint64 XdevLTimer::getDT64() {
	xdl_uint64 dT = (getTimeGlobalMicroSeconds() - m_OldTime);
	m_OldTime = getTimeGlobalMicroSeconds();
	return dT;
}

xdl_double XdevLTimer::getTime() {
	return (xdl_double)(getTimeGlobalMicroSeconds() - m_StartingTime)*0.000001;
}

xdl_uint64 XdevLTimer::getTime64() {
	return (getTimeGlobalMicroSeconds() - m_StartingTime);
}

void XdevLTimer::reset() {
	m_CurrTime = m_OldTime = 0;
	m_StartingTime = getTimeGlobalMicroSeconds();
}

void XdevLTimer::setT1(){
	m_t1 = getTimeGlobalMicroSeconds();
}

void XdevLTimer::setT2(){
	m_t2 = getTimeGlobalMicroSeconds();
}

xdl_double XdevLTimer::getT2T1(){
	return (xdl_double)(m_t2 - m_t1)*0.000001;
}

xdl_uint64 XdevLTimer::getT2T164(){
	return (m_t2 - m_t1);
}

void XdevLTimer::getTimeOfDay(XdevLTime* time){
#if defined(_MSC_VER)
	FILETIME ft;
	unsigned __int64 tmpres = 0;
	static int tzflag;
 

	GetSystemTimeAsFileTime(&ft);
 
	tmpres |= ft.dwHighDateTime;
	tmpres <<= 32;
	tmpres |= ft.dwLowDateTime;
 
	/*converting file time to unix epoch*/
	tmpres -= DELTA_EPOCH_IN_MICROSECS; 
	tmpres /= 10;  /*convert into microseconds*/
	time->sec = (long)(tmpres / 1000000UL);
	time->usec = (long)(tmpres % 1000000UL);
  
 
	if (!tzflag)
	{
		_tzset();
		tzflag++;
	}
	time->minuteswest	= _timezone / 60;
	time->dsttime		= _daylight;

#elif defined __GNUG__ && !defined _WIN32
	timeval val;
	gettimeofday(&val,NULL);
	time->sec = val.tv_sec;
	time->usec = val.tv_usec;
#elif defined(_WIN32) && ! defined(_MSC_VER)
	timeval val;
	gettimeofday(&val,NULL);
	time->sec = val.tv_sec;
	time->usec = val.tv_usec;
#endif

}


}
