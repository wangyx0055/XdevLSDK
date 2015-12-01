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
				Resets the local time to 0.0. This means if you call the getTime() function
				directly after reseting it will return 0.0.
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

	class XdevLScopeTime {
	public:
		XdevLScopeTime(XdevLTimer& timer, const std::string& name = "") : m_timer(timer), m_name(name) {
			m_timer.setT1();
		}
		~XdevLScopeTime() {
			m_timer.setT2();
		}
	private:
		XdevLTimer 				m_timer;
		const std::string m_name;
	};
}

#endif
