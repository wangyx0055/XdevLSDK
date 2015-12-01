/*
	TinyDir.

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

#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <iostream>
#include <string>
#include <vector>

#if defined __GNUG__ && !defined _WIN32
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#elif defined _WIN32
#include <direct.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "tinydir_entity.h"


#define LOCAL_DIR "."
#define PARENT_DIR ".."

namespace tdir {

	/**
		@class directory
		@brief Supports platform independent directory management.
	*/
	class directory {
	public:
		directory();
		~directory();
		/// Open the specified directory.
		/**
			Opens the directory and reads all items.
			@param dirname A null terminated string which represents the directory name to open.
			@return
				- @b '0' if it was successful.
				- @b '-1' if an error occurred.
		*/
		int open(const char* dirname);
		/// Open the local directory.
		/**
			@return
				- @b '0' if it was successful.
				- @b '-1' if an error occurred.
		*/
		int open();
		/// Close the specified directory.
		/**
			@return
				- @b '0' if it was successful.
				- @b '-1' if an error occurred.
		*/
		int close();
		/// Makes a new directory.
		/**
			@param dirname A null terminated string which represents the directory name to make.
			@return
				- @b '0' if it was successful.
				- @b '-1' if an error occurred.
		*/
		int make(const char* dirname);
		/// Rename a folder.
		/**
			@param oldname A null terminated string which represents the old directory name to rename.
			@param newname A null terminated string which represents the new directory name.
			@return
				- @b '0' if it was successful.
				- @b '-1' if an error occurred.
		*/
		int rename (const char* oldname, const char* newname);
		/// Removes a directory.
		/**
			@param dirname A null terminated string which represents the directory name to remove.
			@return
				- @b '0' if it was successful.
				- @b '-1' if an error occurred.
		*/
		int remove(const char* dirname);
		/// Change directory.
		/**
			@param dirname A null terminated string which represents the directory name to change into.
			@return
				- @b '0' if it was successful.
				- @b '-1' if an error occurred.
		*/
		int change(const char* dirname);
		/// Read one entity of the directory.
		/**
			Reads one item in the folder. Call this function till you get
		*/
		int read(dir_entity* entity);
		/// Set a specified position in the directory entity list.
		void seek(long int pos);
		/// Rewind the position in the directory entity list.
		void rewind();
		/// Returns the current position in the directory entity.
		long int tell();
		/// Returns one entity name of the directory.
		const char* getEntity(long int pos);
		/// Returns the number of entities in the directory.
		size_t getNumEntities();
		/// Returns the current working directory path.
		/**
			@return A null terminated string which represents the call
		*/
		const char* getWorkingDirectory();
		static const char* local_dir;
		static const char* parent_dir;
	protected:
		void reload();
	protected:
#if defined __GNUG__ && !defined _WIN32
		DIR* 		m_dir;
#elif defined _WIN32
		intptr_t 						hFile;
		struct _finddata_t  m_info;
		dir_entity 					m_dir;
#endif
		std::vector<dir_entity>				m_entity_list;
		char* m_workingDirectoryBuffer;
	};

}// End of namespace

#endif
