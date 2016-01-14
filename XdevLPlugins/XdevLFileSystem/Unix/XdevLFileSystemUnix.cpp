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

#include "XdevLFileSystemUnix.h"


namespace xdl {
	const char* parent_dir = "..";
	const char* local_dir = ".";

	bool sort_name_dir_entitiy(const XdevLDirectoryEntity& i, const XdevLDirectoryEntity& j) {
		std::string si(i.getName());
		std::string sj(j.getName());
		return (si < sj);
	}



	XdevLFileUnix::XdevLFileUnix(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		XdevLModuleImpl<XdevLFile>(parameter, descriptor),
		m_fd(-1) {

	}



	XdevLDirectoryEntity::Type extractType(struct dirent* ent) {
		switch(ent->d_type) {
			case DT_REG:
				return XdevLDirectoryEntity::Type::REGULAR;
			case DT_DIR:
				return XdevLDirectoryEntity::Type::DIRECTORY;
			default:
				return XdevLDirectoryEntity::Type::UNKNOWN;
		}
	}

	XdevLDirectoryUnix::XdevLDirectoryUnix(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		XdevLModuleImpl<XdevLDirectory>(parameter, descriptor), m_dir(nullptr) {}

	xdl_int XdevLDirectoryUnix::init() {
		m_workingDirectoryBuffer = new char[512];

		return ERR_OK;
	}

	void* XdevLDirectoryUnix::getInternal(const XdevLInternalName& id) {
		return nullptr;
	}

	xdl_int XdevLDirectoryUnix::shutdown() {
		delete [] m_workingDirectoryBuffer;
		return ERR_OK;
	}

	xdl_int XdevLDirectoryUnix::open(const XdevLString& dirname) {
		m_dir = opendir(dirname.toString().c_str());
		if(nullptr == m_dir) {
			errno = ENOENT;
			return ERR_ERROR;
		}

		return ERR_OK;
	}

	xdl_int XdevLDirectoryUnix::close() {
		if(m_dir) {
			if(-1 == closedir(m_dir)) {
				return ERR_ERROR;
			}
		}

		return ERR_OK;
	}

	xdl_int XdevLDirectoryUnix::changeTo(const XdevLString& dirname) {
		if(::chdir(dirname.toString().c_str()) == -1)
			return ERR_ERROR;

		if(open() == ERR_ERROR) {
			return ERR_ERROR;
		}

		return ERR_OK;
	}

	xdl_int XdevLDirectoryUnix::open() {
		m_dir = opendir(".");
		if(nullptr == m_dir) {
			errno = ENOENT;
			return ERR_ERROR;
		}

		return 0;
	}

	xdl_int XdevLDirectoryUnix::make(const XdevLString& dirname) {
		if(::mkdir(dirname.toString().c_str(), S_IRWXU | S_IRGRP |  S_IROTH | S_IXGRP | S_IXOTH) == -1) {
			return ERR_ERROR;
		}

		return ERR_OK;
	}

	xdl_int XdevLDirectoryUnix::remove(const XdevLString& dirname) {
		if(rmdir(dirname.toString().c_str()) == -1) {
			return ERR_ERROR;
		}

		return ERR_OK;
	}

	xdl_int XdevLDirectoryUnix::rename(const XdevLString& oldname, const XdevLString& newname) {
		if(::rename(oldname.toString().c_str(), newname.toString().c_str()) == -1) {
			return ERR_ERROR;
		}
		return ERR_OK;
	}

	void XdevLDirectoryUnix::seek(xdl_uint64 pos) {
		if(m_dir) {
#if XDEVL_PLATFORM_ANDROID
#else
			seekdir(m_dir, pos);
#endif
		}
	}

	void XdevLDirectoryUnix::rewind() {
		if(m_dir) {
			rewinddir(m_dir);
		}
	}

	xdl_uint64 XdevLDirectoryUnix::tell() {
#if XDEVL_PLATFORM_ANDROID
		return 0;
#else
		return telldir(m_dir);
#endif
	}

	XdevLString XdevLDirectoryUnix::getWorkingDirectory() {
		if(getcwd(m_workingDirectoryBuffer, 512) != m_workingDirectoryBuffer) {
			return XdevLString("");
		}
		XdevLString tmp(m_workingDirectoryBuffer);

		return tmp;
	}

	xdl_uint64 XdevLDirectoryUnix::getNumberOfEntities() {
		xdl_uint64 numberOfEntities = 0;

		if(nullptr == m_dir) {
			return numberOfEntities;
		}

		int ret = 0;
		do {
			XdevLDirectoryEntity ent;
			ret = getEntity(ent);
			if(ret != ERR_ERROR) {
				numberOfEntities++;
			}
		} while(ret != ERR_ERROR);

		// Rewind back the pointer to the begining of the list.
		rewind();

		return numberOfEntities;
	}

	xdl_int XdevLDirectoryUnix::getEntity(XdevLDirectoryEntity& entity) {
		struct dirent* ent = nullptr;

		// Read one item from the directory.
		ent = readdir(m_dir);

		if(ent == nullptr) {
			// TODO How to deal with the error EBADF?
			if(errno == EBADF) {
				return ERR_ERROR;
			}
			return ERR_ERROR;
		}

		// Extract the type of item. (File? Folder?)
		XdevLDirectoryEntity::Type type = extractType(ent);

		// Assign the new entity to the variable.
		entity = XdevLDirectoryEntity(ent->d_name, type);

		return ERR_OK;
	}

	std::vector<XdevLDirectoryEntity> XdevLDirectoryUnix::getEntities() {
		// If we do not have an open directory, return empty.
		if(nullptr == m_dir) {
			return std::vector<XdevLDirectoryEntity>();
		}

		std::vector<XdevLDirectoryEntity>	m_entity_list;

		int ret = 0;
		do {
			XdevLDirectoryEntity ent;
			ret = getEntity(ent);
			if(ret != ERR_ERROR) {
				m_entity_list.push_back(ent);
			}
		} while(ret != ERR_ERROR);

		// Rewind back the pointer to the begining of the list.
		rewind();

		return std::move(m_entity_list);
	}

	xdl_bool XdevLDirectoryUnix::find(const XdevLString& name, XdevLDirectoryEntity& entity) {

		std::vector<XdevLDirectoryEntity> entities = getEntities();
		if(entities.size() == 0) {
			entity = XdevLDirectoryEntity();
			return xdl_false;
		}
		std::vector<XdevLDirectoryEntity>::iterator it = std::find(entities.begin(), entities.end(), name);
		if(it == entities.end()) {
			return xdl_false;
		}
		entity = *it;

		// Rewind back the pointer to the begining of the list.
		rewind();

		return xdl_true;
	}

	std::vector<XdevLString> XdevLDirectoryUnix::getAllDirectories(const XdevLString& directoryName, xdl_bool recursive) {
		std::vector<XdevLString> tmp;
		getDirectoryList(tmp, directoryName, recursive);
		return std::move(tmp);
	}

	void XdevLDirectoryUnix::getDirectoryList(std::vector<XdevLString>& folderList, const XdevLString& currentDirectoryName, xdl_bool recursive) {
		close();
				
		if(open(currentDirectoryName) == ERR_ERROR) {
			return;
		}

		std::vector<XdevLDirectoryEntity> folderEntities = getEntities();
		if(folderEntities.size() == 0) {
			return;
		}

		for(auto& entity : folderEntities) {

			if((entity.getType() != XdevLDirectoryEntity::Type::DIRECTORY) ||
			        (entity.getName() == XdevLString(".")) ||
			        (entity.getName() == XdevLString(".."))) {
				continue;
			}

			XdevLString tmp = currentDirectoryName + STRING("/") + entity.getName();
			folderList.push_back(tmp);
			if(recursive) {
				getDirectoryList(folderList, tmp, recursive);
			}
		}
	}

	std::vector<XdevLString>  XdevLDirectoryUnix::getAllFiles(const XdevLString& directoryName, xdl_bool recursive, const XdevLString& pattern) {
		std::vector<XdevLString> tmp;
		getFileList(tmp, directoryName, recursive, pattern);
		return std::move(tmp);
	}

	void  XdevLDirectoryUnix::getFileList(std::vector<XdevLString>& fileList, const XdevLString& currentDirectoryName, xdl_bool recursive, const XdevLString& pattern) {
		close();
		
		if(open(currentDirectoryName) == ERR_ERROR) {
			return;
		}

		std::vector<XdevLDirectoryEntity> folderEntities = getEntities();
		if(folderEntities.size() == 0) {
			return;
		}

		for(auto& entity : folderEntities) {
			// TODO Maybe this goes wrong for a Unknown item?
			XdevLString tmp = currentDirectoryName + STRING("/");

			if(entity.isRegular()) {
				auto result = entity.getName().find(pattern);
				// TODO the comparison with -1 should be npos but for now we use this hack.
				if(result != (size_t)(-1)) {
					XdevLString filePath = tmp + entity.getName();
					fileList.push_back(filePath);
				}
			} else if(recursive &&
			          entity.isDirectory() &&
			          (entity.getName() != XdevLString(".")) &&
			          (entity.getName() != XdevLString(".."))) {
				XdevLString folder = tmp + entity.getName();
				getFileList(fileList, folder, recursive, pattern);
			}
		}
	}


	std::vector<XdevLString>  XdevLDirectoryUnix::getAllDirectoriesContainsFile(const XdevLString& directoryName, xdl_bool recursive, const XdevLString& pattern) {
		std::vector<XdevLString> tmp;
		getDirectoryContainsFileList(tmp, directoryName, recursive, pattern);
		return std::move(tmp);
	}

	void XdevLDirectoryUnix::getDirectoryContainsFileList(std::vector<XdevLString>& directoryList, const XdevLString& currentDirectoryName, xdl_bool recursive, const XdevLString& pattern) {
		close();
		
		if(open(currentDirectoryName) == ERR_ERROR) {
			return;
		}

		std::vector<XdevLDirectoryEntity> folderEntities = getEntities();
		if(folderEntities.size() == 0) {
			return;
		}
		xdl_bool addFolderNames = xdl_true;
		for(auto& entity : folderEntities) {

			if((entity.getType() == XdevLDirectoryEntity::Type::REGULAR) && addFolderNames) {
				auto result = entity.getName().find(pattern);
				// TODO the comparison with -1 should be npos but for now we use this hack.
				if(result != (size_t)(-1)) {
					directoryList.push_back(currentDirectoryName);
					addFolderNames = false;
				}
			} else if(recursive &&
			          (entity.getType() == XdevLDirectoryEntity::Type::DIRECTORY) &&
			          (entity.getName() != XdevLString(".")) &&
			          (entity.getName() != XdevLString(".."))) {
				XdevLString recDir = currentDirectoryName + STRING("/") + entity.getName();
				getDirectoryContainsFileList(directoryList, recDir, recursive, pattern);
			}
		}
	}



}
