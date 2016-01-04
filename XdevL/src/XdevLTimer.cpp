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

#include "XdevLTimer.h"

// This is for the MINGW case
#if defined(_WIN32) || defined(__MINGW32_VERSION)
#include <Windows.h>
static const unsigned __int64 epoch = ((unsigned __int64) 116444736000000000ULL);

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>	/* POSIX flags */
#include <time.h>	/* clock_gettime(), time() */
#include <sys/time.h>	/* gethrtime(), gettimeofday() */

#if defined(__MACH__) && defined(__APPLE__)
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#else
#error "Unable to define getRealTime( ) for an unknown OS."
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

//#include <Windows.h>
//#include <time.h>
//#include <XdevLTypes.h>

xdl::xdl_uint64 getTimeGlobalMicroSeconds() {

	LARGE_INTEGER Frequency;
	LARGE_INTEGER CurClock;
	QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&CurClock);

	return (CurClock.QuadPart * 1000000) / Frequency.QuadPart;
}

xdl::xdl_double getTimeGlobal() {

	LARGE_INTEGER Frequency;
	LARGE_INTEGER CurClock;
	QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&CurClock);

	return (xdl::xdl_double(CurClock.QuadPart) / xdl::xdl_double(Frequency.QuadPart));
}

int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
    FILETIME    file_time;
    SYSTEMTIME  system_time;
    ULARGE_INTEGER ularge;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    ularge.LowPart = file_time.dwLowDateTime;
    ularge.HighPart = file_time.dwHighDateTime;

    tp->tv_sec = (long) ((ularge.QuadPart - epoch) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);

    return 0;
}

// -----------------------------------------------------------------------------
// Do timer stuff on UNIX systems.
//
#elif defined __GNUG__ && !defined _WIN32

//#include <netinet/in.h>
//#include <sys/timeb.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <unistd.h>
//#include <pthread.h>

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
