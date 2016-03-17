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

#include <unistd.h>
#include <time.h>
#include <sys/time.h>

namespace xdl {

	extern "C" xdl::xdl_uint64 getTimeGlobalMicroSeconds() {

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

	void XdevLTimer::getTimeOfDay(XdevLTime* time) {
		timeval val;
		gettimeofday(&val,NULL);
		time->sec = val.tv_sec;
		time->usec = val.tv_usec;
	}

}
