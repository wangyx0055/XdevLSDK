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

#ifndef XDEVL_FILESYSTEM_UNIX_H
#define XDEVL_FILESYSTEM_UNIX_H

#include <XdevLPluginImpl.h>
#include <XdevLFileSystem/XdevLFileSystem.h>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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


	class XdevLFileUnix : public XdevLModuleImpl<XdevLFile> {
		public:
			XdevLFileUnix(XdevLModuleCreateParameter* parameter);

			virtual ~XdevLFileUnix() {
			}

			static XdevLModuleDescriptor m_moduleDescriptor2;

			virtual xdl_int open(const XdevLOpenForReadOnly& readOnly, const XdevLFileName& filename) {
				m_fd = ::open(filename, O_RDONLY);
				if(m_fd == -1) {
					XDEVL_MODULE_ERROR("Could not open file: " << filename << std::endl);
					return -1;
				}
				m_filename = filename;
				return 0;
			}

			virtual xdl_int open(const XdevLOpenForWriteOnly& writeOnly, const XdevLFileName& filename) {
				mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
				m_fd = ::open(filename, O_CREAT | O_WRONLY, mode);
				if(m_fd == -1) {
					XDEVL_MODULE_ERROR("Could not open file: " << filename << std::endl);
					return -1;
				}
				m_filename = filename;
				return 0;
			}

			virtual xdl_int open(const XdevLOpenForReadWrite& readWrite, const XdevLFileName& filename) {
				mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
				m_fd = ::open(filename, O_CREAT | O_RDWR, mode);
				if(m_fd == -1) {
					XDEVL_MODULE_ERROR("Could not open file: " << filename << std::endl);
					return -1;
				}
				m_filename = filename;
				return 0;
			}

			virtual xdl_int open(const XdevLOpenForAppend& append, const XdevLFileName& filename) {
				mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
				m_fd = ::open(filename, O_CREAT | O_RDWR | O_APPEND, mode);
				if(m_fd == -1) {
					XDEVL_MODULE_ERROR("Could not open file: " << filename << std::endl);
					return -1;
				}
				m_filename = filename;
				return 0;
			}

			/// Close the file.
			virtual xdl_int close() {
				if(::close(m_fd) == -1) {
					// TODO What to do with the file descriptor?
					return -1;
				}
				m_fd = -1;
				m_filename = "";
				return 0;
			}

			virtual const XdevLFileName getFileName() const {
				return m_filename;
			}

			/// Remove the file.
			virtual xdl_int remove(const XdevLFileName& filename) {
				if(::remove(filename.toString().c_str()) == -1) {
					return -1;
				}
				return 0;
			}

			/// Rename the file.
			virtual xdl_int rename(const XdevLFileName& oldFileName, const XdevLFileName& newFileName) {
				if(::rename(oldFileName.toString().c_str(), newFileName.toString().c_str()) == -1) {
					return -1;
				}
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
				ssize_t ret = -1;
				if((ret = ::lseek(m_fd, offset, SEEK_SET)) == -1) {
					return -1;
				}
				return ret;
			}
			virtual xdl_int seek(const XdevLSeekCurr& mode, xdl_uint64 offset) {
				ssize_t ret = -1;
				if((ret = ::lseek(m_fd, offset, SEEK_CUR)) == -1) {
					return -1;
				}
				return ret;
			}
			virtual xdl_int seek(const XdevLSeekEnd& mode, xdl_uint64 offset) {
				ssize_t ret = -1;
				if((ret = ::lseek(m_fd, offset, SEEK_END)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int read(xdl_uint8* dst, xdl_int size) {
				ssize_t ret = -1;
				if((ret = ::read(m_fd, dst, size)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int write(xdl_uint8* src, xdl_int size) {
				ssize_t ret = -1;
				if((ret = ::write(m_fd, src, size)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int flush() {
				// TODO can we flush here?
				return 0;
			}

			virtual xdl_int readU8(xdl_uint8& value) {
				ssize_t ret = -1;
				if((ret = ::read(m_fd, &value, 1)) == -1) {
					return -1;
				}
				return ret;
			}


			virtual xdl_int read8(xdl_int8& value) {
				ssize_t ret = -1;
				if((ret = ::read(m_fd, &value, 1)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int readU16(xdl_uint16& value) {
				ssize_t ret = -1;
				if((ret = ::read(m_fd, &value, 2)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int read16(xdl_int16& value) {
				ssize_t ret = -1;
				if((ret = ::read(m_fd, &value, 2)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int readU32(xdl_uint32& value) {
				ssize_t ret = -1;
				if((ret = ::read(m_fd, &value, 4)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int read32(xdl_int32& value) {
				ssize_t ret = -1;
				if((ret = ::read(m_fd, &value, 4)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int readU64(xdl_uint64& value) {
				ssize_t ret = -1;
				if((ret = ::read(m_fd, &value, 8)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int read64(xdl_int64& value) {
				ssize_t ret = -1;
				if((ret = ::read(m_fd, &value, 8)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int readFloat(xdl_float& value) {
				ssize_t ret = -1;
				if((ret = ::read(m_fd, &value, 4)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int readDouble(xdl_double& value) {
				ssize_t ret = -1;
				if((ret = ::read(m_fd, &value, 8)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int readString(XdevLString& value) {
				// TODO NO UTF-16/32.
				xdl_char tmp = 0;

				value.clear();

				do {
					tmp = ::read(m_fd, &tmp, 1);
					if(tmp == EOF) {
						break;
					}
					value += tmp;
				} while(tmp != '\0' || tmp != '\n');

				return 0;
			}

			virtual xdl_int writeU8(xdl_uint8 value) {
				ssize_t ret = -1;
				if((ret = ::write(m_fd, &value, 1)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int write8(xdl_int8 value) {
				ssize_t ret = -1;
				if((ret = ::write(m_fd, &value, 1)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int writeU16(xdl_uint16 value) {
				ssize_t ret = -1;
				if((ret = ::write(m_fd, &value, 2)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int write16(xdl_int16 value) {
				ssize_t ret = -1;
				if((ret = ::write(m_fd, &value, 2)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int writeU32(xdl_uint32 value) {
				ssize_t ret = -1;
				if((ret = ::write(m_fd, &value, 4)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int write32(xdl_int32 value) {
				ssize_t ret = -1;
				if((ret = ::write(m_fd, &value, 4)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int writeU64(xdl_uint64 value) {
				ssize_t ret = -1;
				if((ret = ::write(m_fd, &value, 8)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int write64(xdl_int64 value) {
				ssize_t ret = -1;
				if((ret = ::write(m_fd, &value, 8)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int writeFloat(xdl_float value) {
				ssize_t ret = -1;
				if((ret = ::write(m_fd, &value, 4)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int writeDouble(xdl_double value) {
				ssize_t ret = -1;
				if((ret = ::write(m_fd, &value, 8)) == -1) {
					return -1;
				}
				return ret;
			}

			virtual xdl_int writeString(const XdevLString& str) {
				ssize_t ret = -1;
				if((ret = ::write(m_fd, str.toString().data(), str.toString().length())) == -1) {
					return -1;
				}
				return ret;
			}

		private:
			xdl_int									m_fd;
			XdevLFileName 					m_filename;
			std::vector<xdl_char*>	m_lineArray;
	};

	class XdevLDirectoryUnix : public XdevLModuleImpl<XdevLDirectory> {
		public:
			XdevLDirectoryUnix(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLDirectoryUnix() {
				if(nullptr != m_dir) {
					close();
				}
				m_dir = nullptr;
			}
			static XdevLModuleDescriptor m_moduleDescriptor;

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
			
			virtual void seek(xdl_uint64 pos);
			virtual void rewind();
			virtual xdl_uint64 tell();

		private:
			DIR* m_dir;
			xdl_char* m_workingDirectoryBuffer;
	};

}
extern "C" XDEVL_EXPORT void _delete(xdl::XdevLModule* obj);
extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor();

#endif
