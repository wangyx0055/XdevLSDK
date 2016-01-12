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

#ifndef XDEVL_FILESYSTEM_MACOSX_H
#define XDEVL_FILESYSTEM_MACOSX_H

#include <XdevLFileSystem/XdevLFileSystem.h>
#include <XdevLFileSystem/Unix/XdevLFileSystemUnix.h>
#include <vector>

#include <CoreServices/CoreServices.h>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <array>
#include <thread>
#include <mutex>
#include <atomic>

#include <vector>
#include <cassert>

namespace xdl {

	class XdevLDirectoryWatcherMacOSX : public XdevLModuleImpl<XdevLDirectoryWatcher> {
		public:
			XdevLDirectoryWatcherMacOSX(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor)
				: XdevLModuleImpl<XdevLDirectoryWatcher>(parameter, descriptor) {};

			virtual ~XdevLDirectoryWatcherMacOSX() {};

			virtual xdl_int init();
			virtual xdl_int shutdown();

			virtual int start();
			virtual int stop();

			virtual xdl_int addDirectoryToWatch(const XdevLString& folder);

			virtual int registerDelegate(const XdevLDirectoryWatcherDelegateType& delegate);
			virtual int unregisterDelegate(const XdevLDirectoryWatcherDelegateType& delegate);
			void handleChanges(ItemTypes itemType, EventTypes eventType, const XdevLString& path);

		private:

			static void FileSystemEventCallback(ConstFSEventStreamRef,
			                                    void* clientCallBackInfo,
			                                    size_t numEvents,
			                                    void* eventPaths,
			                                    const FSEventStreamEventFlags eventFlags[],
			                                    const FSEventStreamEventId eventIds[]);
		private:
			std::vector<XdevLDirectoryWatcherDelegateType> m_delegates;
		private:
			std::vector<FSEventStreamRef> m_streams;

			std::array<char, 1024> m_buffer;
	};
}

#endif
