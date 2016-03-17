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

	void XdevLTimer::setT1() {
		m_t1 = getTimeGlobalMicroSeconds();
	}

	void XdevLTimer::setT2() {
		m_t2 = getTimeGlobalMicroSeconds();
	}

	xdl_double XdevLTimer::getT2T1() {
		return (xdl_double)(m_t2 - m_t1)*0.000001;
	}

	xdl_uint64 XdevLTimer::getT2T164() {
		return (m_t2 - m_t1);
	}

}
