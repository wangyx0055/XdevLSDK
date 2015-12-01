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

#ifndef XDEVL_FILESYSTEM_H
#define XDEVL_FILESYSTEM_H

#include <XdevLModule.h>
#include <XdevLStream.h>
#include <XdevLDelegate.h>
#include <string>

namespace xdl {

	class XdevLAccessMode {};

	class XdevLOpenForReadOnly : public XdevLAccessMode {};

	class XdevLOpenForWriteOnly : public XdevLAccessMode {};

	class XdevLOpenForReadWrite : public XdevLAccessMode {};

	class XdevLOpenForAppend : public XdevLAccessMode {};

	class XdevLSeekMode {};

	/**
			@class XdevLSeekSet Cast class to set seek to set.
	*/
	class XdevLSeekSet : public XdevLSeekMode {};

	/**
			@class XdevLSeekSet Cast class to set seek to curr.
	*/
	class XdevLSeekCurr : public XdevLSeekMode {};

	/**
			@class XdevLSeekSet Cast class to set seek to end.
	*/
	class XdevLSeekEnd : public XdevLSeekMode {};

	/**
		@class XdevLDirectoryEntity
		@brief Information about an entity in the directory.
        
        This class holds information about one entity in the directory. Either
        it is a file or a folder. 
	*/
	class XdevLDirectoryEntity {
		public:

			enum class Type {
			    UNKNOWN, REGULAR, DIRECTORY
			};

			XdevLDirectoryEntity()
				:
				m_type(Type::UNKNOWN) {
			};

			XdevLDirectoryEntity(const xdl_char* filename, Type type)
				:
				m_filename(filename),
				m_type(type) {
			};

			/// Returns the name of the entity.
			const XdevLString& getName() const {
				return m_filename;
			}
            
            /// Returns the type.
            /**
                @return One of the Type:
            */
			Type getType() {
				return m_type;
			}
            
            /// Returns the type as string.
			XdevLString getTypeAsString() {
				switch(m_type) {
					case Type::REGULAR:
						return XdevLString("Regular");
					case Type::DIRECTORY:
						return XdevLString("Directory");
					case Type::UNKNOWN:
					default:
						return XdevLString("Unknown");
				}
			}
			
            /// Does this entity represent a directory?
			xdl_bool isDirectory() {
				return (m_type == Type::DIRECTORY ? xdl_true : xdl_false);
			}
			
            /// Is this entity a regular file?
			xdl_bool isRegular() {
				return (m_type == Type::REGULAR ? xdl_true : xdl_false);
			}

			bool operator==(const XdevLString& entityName) {
				return (m_filename == entityName);
			}

			bool operator!=(const XdevLString& entityName) {
				return !(m_filename == entityName);
			}

		protected:

			// Holds the name of the file or folder.
			XdevLString m_filename;

			// Holds the type of item.
			Type m_type;
	};

	/**
		@class XdevLFile
		@brief Class to handle files.
		
		@section xdevl_file Plugin & Module information

		Here the feature list of this module:

		- Open/Create files
		- Read/Write from/to files
		- Rename files
		- Remove files
		- Read/Write from/to files with specific types of files like uint8/uint16/double/float etc ...
		
		@todo Support UTF8/16/32 reading/writing.
		@todo Support archived formats as zip, tar etc..
	*/
	class XdevLFile : public XdevLStream {
		public:

			virtual ~XdevLFile() {}

			using XdevLStream::open;

			/// Open an existing file to read only.
			virtual xdl_int open(const XdevLOpenForReadOnly& readOnly, const XdevLFileName& filename) = 0;
			virtual xdl_int open(const XdevLOpenForWriteOnly& writeOnly, const XdevLFileName& filename) = 0;
			virtual xdl_int open(const XdevLOpenForReadWrite& readWrite, const XdevLFileName& filename) = 0;
			virtual xdl_int open(const XdevLOpenForAppend& append, const XdevLFileName& filename) = 0;

			/// Close the file.
			virtual xdl_int close() = 0;

			/// Moves the file pointer.
			virtual xdl_int seek(const XdevLSeekSet& mode, xdl_uint64 offset) = 0;
			virtual xdl_int seek(const XdevLSeekCurr& mode, xdl_uint64 offset) = 0;
			virtual xdl_int seek(const XdevLSeekEnd& mode, xdl_uint64 offset) = 0;

			/// Returns the current filename of the open file.
			virtual const XdevLFileName getFileName() const = 0;

			/// Remove the file.
			virtual xdl_int remove(const XdevLFileName& filename) = 0;

			/// Rename the file.
			virtual xdl_int rename(const XdevLFileName& oldFileName, const XdevLFileName& newFileName) = 0;

			/// Reads a unsigned 8-bit value from the stream.
			virtual xdl_int readU8(xdl_uint8& value) = 0;

			/// Reads a signed 8-bit value from the stream.
			virtual xdl_int read8(xdl_int8& value) = 0;

			/// Reads a unsigned 16-bit value from the stream.
			virtual xdl_int readU16(xdl_uint16& value) = 0;

			/// Reads a signed 16-bit value from the stream.
			virtual xdl_int read16(xdl_int16& value) = 0;

			/// Reads a unsigned 32-bit value from the stream.
			virtual xdl_int readU32(xdl_uint32& value) = 0;

			/// Reads a signed 32-bit value from the stream.
			virtual xdl_int read32(xdl_int32& value) = 0;

			/// Reads a unsigned 64-bit value from the stream.
			virtual xdl_int readU64(xdl_uint64& value) = 0;

			/// Reads a signed 64-bit value from the stream.
			virtual xdl_int read64(xdl_int64& value) = 0;

			/// Reads a float value from the stream.
			virtual xdl_int readFloat(xdl_float& value) = 0;

			/// Reads a double value from the stream.
			virtual xdl_int readDouble(xdl_double& value) = 0;

			/// Read a string.
			virtual xdl_int readString(XdevLString& value) = 0;

			/// Writes an unsigned 8-bit value to the stream.
			virtual xdl_int writeU8(xdl_uint8 value) = 0;

			/// Writes an signed 8-bit value to the stream.
			virtual xdl_int write8(xdl_int8 value) = 0;

			/// Writes an unsigned 16-bit value to the stream.
			virtual xdl_int writeU16(xdl_uint16 value) = 0;

			/// Writes an signed 16-bit value to the stream.
			virtual xdl_int write16(xdl_int16 value) = 0;

			/// Writes an unsigned 32-bit value to the stream.
			virtual xdl_int writeU32(xdl_uint32 value) = 0;

			/// Writes an signed 32-bit value to the stream.
			virtual xdl_int write32(xdl_int32 value) = 0;

			/// Writes an unsigned 64-bit value to the stream.
			virtual xdl_int writeU64(xdl_uint64 value) = 0;

			/// Writes an signed 64-bit value to the stream.
			virtual xdl_int write64(xdl_int64 value) = 0;

			/// Writes a float value to the stream.
			virtual xdl_int writeFloat(xdl_float value) = 0;

			/// Writes a double value to the stream.
			virtual xdl_int writeDouble(xdl_double value) = 0;

			/// Writes a string.
			virtual xdl_int writeString(const XdevLString& str) = 0;

	};

	/**
		@class XdevLDirectory
		@brief A class holding directory information.
	*/
	class XdevLDirectory : public XdevLModule {
		public:
			virtual ~XdevLDirectory() {
			}

			/// Open the specified directory.
			/**
							Opens the directory.
							@param dirname A null terminated string which represents the directory name to open.
							@return
											- @b ERR_OK if it was successful.
											- @b ERR_ERROR if an error occurred.
			*/
			virtual xdl_int open(const XdevLString& dirname) = 0;

			/// Close the specified directory.
			/**
							After opening the directory you should close it again.
							@return
											- @b ERR_OK if it was successful.
											- @b ERR_ERROR if an error occurred.
			*/
			virtual xdl_int close() = 0;
			
			/// Find all folders recursivly.
			virtual std::vector<XdevLString> getAllDirectories(const XdevLString& directoryName, xdl_bool recursive) = 0;

			/// Find all files recursivly.
			virtual std::vector<XdevLString> getAllFiles(const XdevLString& directoryName, xdl_bool recursive, const XdevLString& pattern = XdevLString("")) = 0;
			
			/// Find all directories that contains a file with a specific pattern.
			virtual std::vector<XdevLString> getAllDirectoriesContainsFile(const XdevLString& directoryName, xdl_bool recursive, const XdevLString& pattern = XdevLString("")) = 0;


			/// Returns the entities in a folder.
			virtual std::vector<XdevLDirectoryEntity> getEntities() = 0;

			/// Read one entity of the directory.
			/**
				Reads one entity from folder and moves the entity cursor to the next.
				@return
				- @b ERR_OK if it was successful.
				- @b ERR_ERROR if an error occurred. Error can be that the cursor reached the end of the items in the directory.
			*/
			virtual xdl_int getEntity(XdevLDirectoryEntity& entity) = 0;

			/// Returns the number of entities in the directory.
			virtual xdl_uint64 getNumberOfEntities() = 0;

			/// Change current directory.
			virtual xdl_int changeTo(const XdevLString& dirname) = 0;

			/// Makes a new directory.
			/**
				@param dirname A null terminated string which the name of the directory
				to make.
				@return
				- @b ERR_OK if it was successful.
				- @b ERR_ERROR if an error occurred.
			*/
			virtual xdl_int make(const XdevLString& dirname) = 0;

			/// Removes a directory.
			/**
				@param dirname A null terminated string which is the name of the directory
				to remove.
				@return
				- @b ERR_OK if it was successful.
				- @b ERR_ERROR if an error occurred.
			*/
			virtual xdl_int remove(const XdevLString& dirname) = 0;

			/// Rename a folder.
			/**
				@param oldname A null terminated string which represents the name of the old directory.
				@param newname A null terminated string which represents the new directory name.
				@return
				- @b ERR_OK if it was successful.
				- @b ERR_ERROR if an error occurred.
			*/
			virtual xdl_int rename(const XdevLString& oldname, const XdevLString& newname) = 0;

			/// Returns the current working directory path.
			/**
				@return A null terminated string which represents the call
			*/
			virtual XdevLString getWorkingDirectory() = 0;

			/// Searches in the current opened directory for an entity.
			/**
				@param name A null terminated string that represents the name of the entity. (File/Folder name).
			*/
			virtual xdl_bool find(const XdevLString& name, XdevLDirectoryEntity& entity) = 0;
	};


	/**
	 * @class XdevLDirectoryWatcher
	 * @brief Watches for changes in a directory.
	 */
	class XdevLDirectoryWatcher : public XdevLModule {
		public:
			enum ItemTypes {FILE, DIRECTORY};
			enum EventTypes { DW_OPEN, DW_CLOSE, DW_CREATE, DW_MODIFY, DW_ACCESS, DW_DELETE, DW_UNKNOWN };

			typedef XdevLDelegate<void, const ItemTypes&, const EventTypes&, const XdevLString&> XdevLDirectoryWatcherDelegateType;

			virtual ~XdevLDirectoryWatcher() {};


			/// Start watching directories.
			/**
				@return Returns ERR_OK for success else ERR_ERROR.
			*/
			virtual int start() = 0;

			/// Stop watching directories.
			/**
				@return
				- @b ERR_OK if it was successful.
				- @b ERR_ERROR if an error occurred.
			*/
			virtual int stop() = 0;

			/// Add directory to watch.
			/**
				@return
				- @b ERR_OK if it was successful.
				- @b ERR_ERROR if an error occurred.
			*/
			virtual int addDirectoryToWatch(const XdevLString& folder) = 0;

			/// Register delegate for a watcher.
			/**
				@return
				- @b ERR_OK if it was successful.
				- @b ERR_ERROR if an error occurred.
			*/
			virtual int registerDelegate(const XdevLDirectoryWatcherDelegateType& delegate) = 0;

			/// Unregister delegate of a watcher.
			/**
				@return
				- @b ERR_OK if it was successful.
				- @b ERR_ERROR if an error occurred.
			*/
			virtual int unregisterDelegate(const XdevLDirectoryWatcherDelegateType& delegate) = 0;
	};



	typedef XdevLFile* 				IPXdevLFile;
	typedef XdevLDirectory* 		IPXdevLDirectory;
	typedef XdevLDirectoryWatcher* 	IPXdevLDirectoryWatcher;
}
#endif
