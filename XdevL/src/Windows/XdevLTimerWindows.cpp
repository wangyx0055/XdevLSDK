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
#include <Windows.h>


namespace xdl {

	static const unsigned __int64 epoch = ((unsigned __int64) 116444736000000000ULL);


#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif


#include <Windows.h>
#include <time.h>

	extern "C" xdl::xdl_uint64 getTimeGlobalMicroSeconds() {

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

	int gettimeofday(struct timeval * tp, struct timezone * tzp) {
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

	void XdevLTimer::getTimeOfDay(XdevLTime* time) {

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


		if (!tzflag) {
			_tzset();
			tzflag++;
		}
		time->minuteswest	= _timezone / 60;
		time->dsttime		= _daylight;
	}


}
