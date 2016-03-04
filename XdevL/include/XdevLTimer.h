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

#ifndef XDEVL_TIMER_H
#define XDEVL_TIMER_H

#include <XdevLTypes.h>
#include <iostream>

namespace xdl {

	/**
		@class XdevLTime
		@brief A structure to hold time information.
	*/
	struct XdevLTime {
		/// The UTC second value.
		xdl::xdl_int64 sec;
		/// The UTC microsecond value.
		xdl::xdl_int64 usec;
		/// Minutes W of Greewich
		xdl::xdl_int64 minuteswest;
		/// DST correction
		xdl::xdl_int64 dsttime;
	};

	/**
		@class XdevLTimer
		@brief High resolution timer.

		@section xdevl_timer Usage
		Here a super simple example how to use the timer.

		@code
		XdevLTimer timer;
		while(true){
			double time = timer.getTime();
		}
		@endcode

		The getTime() method will allways return the time past since the creation of the timer object.

	*/
	class XdevLTimer {
		public:
			XdevLTimer();

			/// Returns the time past since UTC
			/**
				This function will fill a valid XdevLTime instance with the current UTC time value of this computer.
				@param time Pointer to a valid XdevLTime instance.
			*/
			void getTimeOfDay(XdevLTime* time);

			/// Returns the current time in seconds.
			/**
				The elapsed time is from the creation time of the instance.
				@return Returns the local time of the XdevLTimer.
			*/
			xdl_double getTime();

			/// Returns the local time in microseconds.
			/**
				Time elapsed since the XdevLTimer was created or reseted.
				@return Returns the local time of the XdevLTimer as a uint64_t microsecond value.
			*/
			xdl_uint64 getTime64();

			/// Resets the XdevLTimer.
			/**
				Resets the timer.
			*/
			void reset();

			/// Returns the elapsed time.
			/**
				Returns the elapsed time since the call of the getDT() method.
				method was called.
				@return Returns the delta time.
			*/
			xdl_double getDT();
			xdl_uint64 getDT64();

			/// Sets the t1 value.
			void setT1();

			/// Sets the t2 value.
			void setT2();

			/// Returns the elapsed time bewteen the  setT1() and setT2() calls in seconds as.
			/**
				@return Returns (t2 - t1).
			*/
			xdl_double getT2T1();

			/// Returns the elapsed time bewteen the  setT1() and setT2() calls in microseconds as uint64 value.
			/**
				@return Returns (t2 - t1).
			*/
			xdl_uint64 getT2T164();

		protected:

			// Holds start time of the XdevLTimer.
			xdl_uint64 m_StartingTime;
			// Holds the new time.
			xdl_uint64 m_CurrTime;
			// Holds the time one frame before.
			xdl_uint64 m_OldTime;
			// Holds the t1 time value.
			xdl_uint64 m_t1;
			// Holds the t2 time value.
			xdl_uint64 m_t2;
	};

	extern "C" xdl::xdl_uint64 getTimeGlobalMicroSeconds();

}

#endif
