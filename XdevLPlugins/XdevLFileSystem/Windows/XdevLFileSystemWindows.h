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

#ifndef XDEVL_FILESYSTEM_WINDOWS_H
#define XDEVL_FILESYSTEM_WINDOWS_H

#include <XdevLFileSystem/XdevLFileSystem.h>
#include <XdevLPluginImpl.h>
#include <vector>

#include <iostream>
#include <array>
#include <thread>
#include <mutex>
#include <atomic>

#include <vector>
#include <cassert>

namespace xdl {


	// Holds the plugins major version number.
	const xdl_uint XdevLFileSystemPluginMajorVersion = XDEVL_FILESYSTEM_MAJOR_VERSION;

	// Holds the plugins minor version number.
	const xdl_uint XdevLFileSystemPluginMinorVersion = XDEVL_FILESYSTEM_MINOR_VERSION;

	// Holds the plugins oatch version number.
	const xdl_uint XdevLFileSystemPluginPatchVersion = XDEVL_FILESYSTEM_PATCH_VERSION;



	// Holds the Major version number.
	const xdl_uint XdevLFileSystemMajorVersion = XDEVL_FILESYSTEM_MODULE_FILE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLFileSystemMinorVersion = XDEVL_FILESYSTEM_MODULE_FILE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLFileSystemPatchVersion = XDEVL_FILESYSTEM_MODULE_FILE_PATCH_VERSION;



	// Holds the Major version number.
	const xdl_uint XdevLDirectoryMajorVersion = XDEVL_FILESYSTEM_MODULE_DIRECTORY_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLDirectoryMinorVersion = XDEVL_FILESYSTEM_MODULE_DIRECTORY_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLDirectoryPatchVersion = XDEVL_FILESYSTEM_MODULE_DIRECTORY_PATCH_VERSION;



	// Holds the Major version number.
	const xdl_uint XdevLDirectoryWatcherMajorVersion = XDEVL_FILESYSTEM_MODULE_DIRECTORYWATCHER_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLDirectoryWatcherMinorVersion = XDEVL_FILESYSTEM_MODULE_DIRECTORYWATCHER_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLDirectoryWatcherPatchVersion = XDEVL_FILESYSTEM_MODULE_DIRECTORYWATCHER_PATCH_VERSION;


	static const XdevLString XdevLFileSystemVendor {
		"www.codeposer.net"
	};
	static const XdevLString XdevLFileSystemAuthor {
		"Cengiz Terzibas"
	};
	static const XdevLString XdevLFileSystemCopyright {
		"(c) 2005 - 2015 Cengiz Terzibas."
	};
	static const XdevLString XdevLFileSystemPluginName {
		"XdevLFileSystem"
	};
	static const XdevLString XdevLDescriptionForFileSystem {
		"Support for file handling."
	};
	static const XdevLString XdevLDescriptionForDirectory {
		"Support for directory handling."
	};
	static const XdevLString XdevLDescriptionForDirectoryWatcher {
		"Support for directory watching."
	};
	static const std::vector<XdevLModuleName> XdevLFileSystemModuleName	{
		XdevLModuleName("XdevLFile"),
		XdevLModuleName("XdevLDirectory"),
		XdevLModuleName("XdevLDirectoryWatcher")
	};

	class XdevLFileWindows;
	class XdevLDirectoryWatcherWindows;

	class XdevLFileWindows : public XdevLModuleImpl<XdevLFile> {
		public:
			XdevLFileWindows(XdevLModuleCreateParameter* parameter);

			virtual ~XdevLFileWindows() {
			}

			virtual xdl_int open(const XdevLOpenForReadOnly& readOnly, const XdevLFileName& filename) {

				m_filename = filename;
				return 0;
			}

			virtual xdl_int open(const XdevLOpenForWriteOnly& writeOnly, const XdevLFileName& filename) {

				m_filename = filename;
				return 0;
			}

			virtual xdl_int open(const XdevLOpenForReadWrite& readWrite, const XdevLFileName& filename) {

				m_filename = filename;
				return 0;
			}

			virtual xdl_int open(const XdevLOpenForAppend& append, const XdevLFileName& filename) {

				m_filename = filename;
				return 0;
			}

			/// Close the file.
			virtual xdl_int close() {

				m_filename = "";
				return 0;
			}

			virtual const XdevLFileName getFileName() const {
				return m_filename;
			}

			/// Remove the file.
			virtual xdl_int remove(const XdevLFileName& filename) {

				return 0;
			}

			/// Rename the file.
			virtual xdl_int rename(const XdevLFileName& oldFileName, const XdevLFileName& newFileName) {

				return 0;
			}

			virtual xdl_int open() {
				XDEVL_MODULE_ERROR("Can't open empty filename object.\n");
				return -1;
			}

			virtual xdl_int open(const XdevLFileName& filename) {
				return open(XdevLOpenForReadOnly(), filename);
			}

			virtual xdl_int seek(const XdevLSeekSet& mode, xdl_uint64 offset) {
				return 0;
			}
			virtual xdl_int seek(const XdevLSeekCurr& mode, xdl_uint64 offset) {
				return 0;
			}
			virtual xdl_int seek(const XdevLSeekEnd& mode, xdl_uint64 offset) {
				return 0;
			}

			virtual xdl_int read(xdl_uint8* dst, xdl_int size) {
				return 0;
			}

			virtual xdl_int write(xdl_uint8* src, xdl_int size) {
				return 0;
			}

			virtual xdl_int flush() {
				// TODO can we flush here?
				return 0;
			}

			virtual xdl_int readU8(xdl_uint8& value) {
				return 0;
			}


			virtual xdl_int read8(xdl_int8& value) {
				return 0;
			}

			virtual xdl_int readU16(xdl_uint16& value) {
				return 0;
			}

			virtual xdl_int read16(xdl_int16& value) {
				return 0;
			}

			virtual xdl_int readU32(xdl_uint32& value) {
				return 0;
			}

			virtual xdl_int read32(xdl_int32& value) {
				return 0;
			}

			virtual xdl_int readU64(xdl_uint64& value) {
				return 0;
			}

			virtual xdl_int read64(xdl_int64& value) {
				return 0;
			}

			virtual xdl_int readFloat(xdl_float& value) {
				return 0;
			}

			virtual xdl_int readDouble(xdl_double& value) {
				return 0;
			}

			virtual xdl_int readString(XdevLString& value) {
				// TODO NO UTF-16/32.
				xdl_char tmp = 0;

				value.clear();

				do {
					//tmp = ::read(m_fd, &tmp, 1); // TODO
					if(tmp == EOF) {
						break;
					}
					value += tmp;
				} while(tmp != '\0' || tmp != '\n');

				return 0;
			}

			virtual xdl_int writeU8(xdl_uint8 value) {
				return 0;
			}

			virtual xdl_int write8(xdl_int8 value) {
				return 0;
			}

			virtual xdl_int writeU16(xdl_uint16 value) {
				return 0;
			}

			virtual xdl_int write16(xdl_int16 value) {
				return 0;
			}

			virtual xdl_int writeU32(xdl_uint32 value) {
				return 0;
			}

			virtual xdl_int write32(xdl_int32 value) {
				return 0;
			}

			virtual xdl_int writeU64(xdl_uint64 value) {
				return 0;
			}

			virtual xdl_int write64(xdl_int64 value) {
				return 0;
			}

			virtual xdl_int writeFloat(xdl_float value) {
				return 0;
			}

			virtual xdl_int writeDouble(xdl_double value) {
				return 0;
			}

			virtual xdl_int writeString(const XdevLString& str) {
				return 0;
			}

		private:
			XdevLFileName m_filename;
			std::vector<xdl_char*>	m_lineArray;
	};

	class XdevLDirectoryWindows : public XdevLModuleImpl<XdevLDirectory> {
		public:
			XdevLDirectoryWindows(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLDirectoryWindows() {
			}

			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;
			virtual void* getInternal(const XdevLInternalName& id) override;

			virtual xdl_int open(const XdevLString& dirname);
			virtual xdl_int open();
			virtual xdl_int close();
			virtual xdl_uint64 getNumberOfEntities();
			virtual xdl_int getEntity(XdevLDirectoryEntity& entity);
			virtual std::vector<XdevLDirectoryEntity> getEntities();

			virtual xdl_int changeTo(const XdevLString& dirname);
			virtual xdl_int make(const XdevLString& dirname);
			virtual xdl_int remove(const XdevLString& dirname);
			virtual xdl_int rename(const XdevLString& oldname, const XdevLString& newname);
			virtual XdevLString getWorkingDirectory();
			virtual xdl_bool find(const XdevLString& name, XdevLDirectoryEntity& entity);

			virtual std::vector<XdevLString> getAllFiles(const XdevLString& directoryName, xdl_bool recursive, const XdevLString& pattern = XdevLString(""));
			virtual std::vector<XdevLString> getAllDirectories(const XdevLString& directoryName, xdl_bool recursive);
			virtual std::vector<XdevLString> getAllDirectoriesContainsFile(const XdevLString& directoryName, xdl_bool recursive, const XdevLString& pattern = XdevLString(""));

		private:

			void getDirectoryList(std::vector<XdevLString>& folderList, const XdevLString& currentDirectoryName, xdl_bool recursive);
			void getFileList(std::vector<XdevLString>& fileList, const XdevLString& currentDirectoryName, xdl_bool recursive, const XdevLString& pattern);
			void getDirectoryContainsFileList(std::vector<XdevLString>& directoryList, const XdevLString& currentDirectoryName, xdl_bool recursive, const XdevLString& pattern);
		private:
			HANDLE m_handle;
			std::vector<XdevLDirectoryEntity> m_directoryItems;
			std::vector<XdevLDirectoryEntity>::iterator m_directoryItemsIterator;
	};


	class XdevLDirectoryWatcherWindowsItem {
		public:
			HANDLE m_handle;
			OVERLAPPED m_overlapped;
			DWORD m_notifyFilter;
			xdl_bool m_recursive;
			xdl_bool m_stopNow;
			XdevLDirectoryWatcherWindows* m_directoryWatcher;
			BYTE m_buffer[32 * 1024];
			XdevLString m_directorName;
	};

	class XdevLDirectoryWatcherWindows : public XdevLModuleImpl<XdevLDirectoryWatcher> {
		public:
			XdevLDirectoryWatcherWindows(XdevLModuleCreateParameter* parameter);

			virtual ~XdevLDirectoryWatcherWindows();

			virtual xdl_int init();
			virtual xdl_int shutdown();
			virtual int start();
			virtual int stop();
			virtual xdl_int addDirectoryToWatch(const XdevLString& folder);
			virtual int registerDelegate(const XdevLDirectoryWatcherDelegateType& delegate);
			virtual int unregisterDelegate(const XdevLDirectoryWatcherDelegateType& delegate);

		public:

			void threadHandle();
			static void CALLBACK ChangeNotification(::DWORD Error, ::DWORD NumBytes, LPOVERLAPPED InOverlapped);
			void handleChange(XdevLDirectoryWatcherWindowsItem* watchteritem, const XdevLString& filename, unsigned long action);

			XdevLDirectoryWatcherWindowsItem* createWatch(LPCTSTR szDirectory, bool recursive, DWORD mNotifyFilter);
			void destroyWatch(XdevLDirectoryWatcherWindowsItem* watcherItem);
			bool refreshWatch(XdevLDirectoryWatcherWindowsItem* watcherItem, xdl_bool clear = xdl_false);

		private:
			std::vector<XdevLDirectoryWatcherWindowsItem*> m_watcherItems;
			std::thread m_watcherThread;
			std::mutex m_mutex;
			std::atomic<xdl_bool> m_runThread;
			std::vector<XdevLDirectoryWatcherDelegateType> m_delegates;
			xdl_bool m_threadStarted;

	};


}

#endif
