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

#ifndef XDEVL_PROFILER_H
#define XDEVL_PROFILER_H

#include <XdevLTypes.h>
#include <XdevLError.h>
#include <XdevLTimer.h>

#include <map>

namespace xdl {

	class XdevLProfiler {
		private:
		XdevLProfiler();
		public:
			static XdevLProfiler* getInstance();
			void dump();
			void add(const XdevLString& scopeName, xdl_uint64 timestamp);
		private:
			std::map<xdl::XdevLString, xdl::xdl_uint64> scopes;
			static XdevLProfiler* instance;
	};

	class XdevLProfilerScope {
		public:
			XdevLProfilerScope(const xdl::XdevLString& name);
			~XdevLProfilerScope();
		private:
			xdl::XdevLString scopeName;
			xdl::XdevLTimer timer;
	};
}

#define PROFILE_SCOPE(NAME) xdl::XdevLProfilerScope scope(NAME);

#endif
