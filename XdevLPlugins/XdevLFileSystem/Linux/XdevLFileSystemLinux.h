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

#ifndef XDEVL_DIRECTORY_LINUX_H
#define XDEVL_DIRECTORY_LINUX_H

#include <XdevLFileSystem/XdevLFileSystem.h>
#include <XdevLFileSystem/Unix/XdevLFileSystemUnix.h>
#include <XdevLPluginImpl.h>
#include <vector>

#include <sys/inotify.h>

#include <array>
#include <thread>
#include <mutex>
#include <atomic>

#include <vector>
#include <cassert>

namespace xdl {

	class XdevLDirectoryWatcherLinux : public XdevLModuleImpl<XdevLDirectoryWatcher> {
		public:
			XdevLDirectoryWatcherLinux(XdevLModuleCreateParameter* parameter)
				:
				XdevLModuleImpl<XdevLDirectoryWatcher>(parameter, m_moduleDescriptorDirectoryWatcher),
				m_threadStarted(xdl_false),
				m_fd(-1) {};

			virtual ~XdevLDirectoryWatcherLinux() {};

			static XdevLModuleDescriptor m_moduleDescriptorDirectoryWatcher;

			virtual xdl_int init();
			virtual xdl_int shutdown();
			virtual int start();
			virtual int stop();
			virtual xdl_int addDirectoryToWatch(const XdevLString& folder);
			virtual int registerDelegate(const XdevLDirectoryWatcherDelegateType& delegate);
			virtual int unregisterDelegate(const XdevLDirectoryWatcherDelegateType& delegate);

		private:

			void threadHandle();

		private:

			std::thread m_watcherThread;
			std::mutex m_mutex;
			std::atomic<xdl_bool> m_runThread;
			std::vector<XdevLDirectoryWatcherDelegateType> m_delegates;
			xdl_bool m_threadStarted;

		private:

			xdl_int m_fd;
			std::vector<xdl_int> m_dfdList;
			std::array<xdl_char, 1024> m_buffer;
	};


}

#endif
