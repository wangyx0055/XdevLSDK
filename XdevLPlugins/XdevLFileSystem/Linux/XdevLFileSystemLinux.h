/*
        XdevL eXtended DEVice Library.

        Copyright © 2005-2012 Cengiz Terzibas

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
