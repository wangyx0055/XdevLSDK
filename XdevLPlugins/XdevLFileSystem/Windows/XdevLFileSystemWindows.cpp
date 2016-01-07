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

#include "XdevLFileSystemWindows.h"

xdl::XdevLPluginDescriptor pluginDescriptor {
	xdl::XdevLFileSystemPluginName,
	xdl::XdevLFileSystemModuleName,
	xdl::XdevLFileSystemPluginMajorVersion,
	xdl::XdevLFileSystemPluginMinorVersion,
	xdl::XdevLFileSystemPluginPatchVersion
};


xdl::XdevLModuleDescriptor moduleDirectoryWatcherDescriptor {
	xdl::XdevLFileSystemVendor,
    xdl::XdevLFileSystemAuthor,
    xdl::XdevLFileSystemModuleName[2],
    xdl::XdevLFileSystemCopyright,
    xdl::XdevLDescriptionForFileSystem,
    xdl::XdevLDirectoryWatcherMajorVersion,
    xdl::XdevLDirectoryWatcherMinorVersion,
    xdl::XdevLDirectoryWatcherPatchVersion
                                                                                               };

xdl::XdevLModuleDescriptor moduleDirectoryDescriptor {
	xdl::XdevLFileSystemVendor,
	xdl::XdevLFileSystemAuthor,
	xdl::XdevLFileSystemModuleName[1],
	xdl::XdevLFileSystemCopyright,
	xdl::XdevLDescriptionForDirectory,
	xdl::XdevLDirectoryMajorVersion,
	xdl::XdevLDirectoryMinorVersion,
	xdl::XdevLDirectoryPatchVersion
};

xdl::XdevLModuleDescriptor moduleFileDescriptor {
	xdl::XdevLFileSystemVendor,
	xdl::XdevLFileSystemAuthor,
	xdl::XdevLFileSystemModuleName[0],
	xdl::XdevLFileSystemCopyright,
	xdl::XdevLDescriptionForFileSystem,
	xdl::XdevLFileSystemMajorVersion,
	xdl::XdevLFileSystemMinorVersion,
	xdl::XdevLFileSystemPatchVersion
};

extern "C" XDEVL_EXPORT xdl::xdl_int _init_plugin(xdl::XdevLPluginCreateParameter* parameter) {
	return xdl::ERR_OK;
}

extern "C" XDEVL_EXPORT xdl::xdl_int _shutdown_plugin() {
	return xdl::ERR_OK;
}

extern "C" XDEVL_EXPORT xdl::XdevLModule* _createModule(const xdl::XdevLPluginDescriptor& pluginDescriptor, const xdl::XdevLModuleDescriptor& moduleDescriptor) {

	//
	// Creaete XdevLFile instance.
	//
	if(moduleFileDescriptor.getName() == moduleDescriptor.getName()) {
		xdl::XdevLModule* obj = new xdl::XdevLFileWindows(nullptr);
		if(!obj) {
			return nullptr;
		}

		return obj;

	}

	//
	// Create XdevLDirectory instance.
	//
	else if(moduleDirectoryDescriptor.getName() == moduleDescriptor.getName()) {
		xdl::XdevLModule* obj = new xdl::XdevLDirectoryWindows(nullptr);
		if(!obj)
			return nullptr;
		
		return obj;

	}

	//
	// Create XdevLDirectoryWatcher instance.
	//
	else if(moduleDirectoryWatcherDescriptor.getName() == moduleDescriptor.getName()) {
		xdl::XdevLModule* obj = new xdl::XdevLDirectoryWatcherWindows(nullptr);
		if(!obj) {
			return nullptr;
		}

		return obj;
	}

	return nullptr;
}

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {

	//
	// Creaete XdevLFile instance.
	//
	if(moduleFileDescriptor.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj = new xdl::XdevLFileWindows(parameter);
		if(nullptr == obj) {
			return xdl::ERR_ERROR;
		}

		parameter->setModuleInstance(obj);

	} 

	//
	// Create XdevLDirectory instance.
	//
	else if(moduleDirectoryDescriptor.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj = new xdl::XdevLDirectoryWindows(parameter);
		if(nullptr == obj) {
			return xdl::ERR_ERROR;
		}

		parameter->setModuleInstance(obj);

	}

	//
	// Create XdevLDirectoryWatcher instance.
	//
	else if(moduleDirectoryWatcherDescriptor.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj = new xdl::XdevLDirectoryWatcherWindows(parameter);
		if(nullptr == obj) {
			return xdl::ERR_ERROR;
		}

		parameter->setModuleInstance(obj);

	} else {
		return xdl::ERR_MODULE_NOT_FOUND;
	}

	return xdl::ERR_OK;
}

extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor()  {
	return &pluginDescriptor;
}

extern "C" XDEVL_EXPORT void _delete(xdl::XdevLModule* obj) {
	if(obj)
		delete obj;
}

namespace xdl {

	const char* local_dir = ".";

	bool sort_name_dir_entitiy(const XdevLDirectoryEntity& i, const XdevLDirectoryEntity& j) {
		std::string si(i.getName());
		std::string sj(j.getName());
		return (si < sj);
	}



	XdevLFileWindows::XdevLFileWindows(XdevLModuleCreateParameter* parameter) :
		XdevLModuleImpl<XdevLFile>(parameter, moduleFileDescriptor) {

	}



	XdevLDirectoryEntity::Type extractType(struct dirent* ent) {
//		switch(ent->d_type) {
//			case DT_REG:
//				return XdevLDirectoryEntity::Type::REGULAR;
//			case DT_DIR:
//				return XdevLDirectoryEntity::Type::DIRECTORY;
//			default:
//				return XdevLDirectoryEntity::Type::UNKNOWN;
//		}
		return XdevLDirectoryEntity::Type::UNKNOWN;
	}

//
// XdevLDirectory
//	

	XdevLDirectoryWindows::XdevLDirectoryWindows(XdevLModuleCreateParameter* parameter) :
		XdevLModuleImpl<XdevLDirectory>(parameter, moduleDirectoryDescriptor) { 
	}

	xdl_int XdevLDirectoryWindows::init() {
	//	m_workingDirectoryBuffer = new char[512];

		return ERR_OK;
	}

	void* XdevLDirectoryWindows::getInternal(const XdevLInternalName& id) {
		return nullptr;
	}

	xdl_int XdevLDirectoryWindows::shutdown() {
	//	delete [] m_workingDirectoryBuffer;
		return ERR_OK;
	}

	xdl_int XdevLDirectoryWindows::open(const XdevLString& dirname) {

		return ERR_OK;
	}

	xdl_int XdevLDirectoryWindows::close() {

		return ERR_OK;
	}

	xdl_int XdevLDirectoryWindows::changeTo(const XdevLString& dirname) {

		return ERR_OK;
	}

	xdl_int XdevLDirectoryWindows::open() {
		return 0;
	}

	xdl_int XdevLDirectoryWindows::make(const XdevLString& dirname) {

		return ERR_OK;
	}

	xdl_int XdevLDirectoryWindows::remove(const XdevLString& dirname) {

		return ERR_OK;
	}

	xdl_int XdevLDirectoryWindows::rename(const XdevLString& oldname, const XdevLString& newname) {
		return ERR_OK;
	}

	XdevLString XdevLDirectoryWindows::getWorkingDirectory() {

		XdevLString tmp;

		return tmp;
	}

	xdl_uint64 XdevLDirectoryWindows::getNumberOfEntities() {
		xdl_uint64 numberOfEntities = 0;

		
		return numberOfEntities;
	}

	xdl_int XdevLDirectoryWindows::getEntity(XdevLDirectoryEntity& entity) {

		return ERR_OK;
	}

	std::vector<XdevLDirectoryEntity> XdevLDirectoryWindows::getEntities() {
		std::vector<XdevLDirectoryEntity>	m_entity_list;


		return std::move(m_entity_list);
	}

	xdl_bool XdevLDirectoryWindows::find(const XdevLString& name, XdevLDirectoryEntity& entity) {

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
//		rewind();

		return xdl_true;
	}

	std::vector<XdevLString> XdevLDirectoryWindows::getAllDirectories(const XdevLString& directoryName, xdl_bool recursive) {
		std::vector<XdevLString> tmp;
	//	getDirectoryList(tmp, directoryName, recursive);
		return std::move(tmp);
	}

	void XdevLDirectoryWindows::getDirectoryList(std::vector<XdevLString>& folderList, const XdevLString& currentDirectoryName, xdl_bool recursive) {
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

	std::vector<XdevLString>  XdevLDirectoryWindows::getAllFiles(const XdevLString& directoryName, xdl_bool recursive, const XdevLString& pattern) {
		std::vector<XdevLString> tmp;
		getFileList(tmp, directoryName, recursive, pattern);
		return std::move(tmp);
	}

	void  XdevLDirectoryWindows::getFileList(std::vector<XdevLString>& fileList, const XdevLString& currentDirectoryName, xdl_bool recursive, const XdevLString& pattern) {
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


	std::vector<XdevLString>  XdevLDirectoryWindows::getAllDirectoriesContainsFile(const XdevLString& directoryName, xdl_bool recursive, const XdevLString& pattern) {
		std::vector<XdevLString> tmp;
		getDirectoryContainsFileList(tmp, directoryName, recursive, pattern);
		return std::move(tmp);
	}

	void XdevLDirectoryWindows::getDirectoryContainsFileList(std::vector<XdevLString>& directoryList, const XdevLString& currentDirectoryName, xdl_bool recursive, const XdevLString& pattern) {
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



//
// XdevLDirectoryWatcher
//	

XdevLDirectoryWatcherWindows::XdevLDirectoryWatcherWindows(XdevLModuleCreateParameter* parameter) :
	XdevLModuleImpl<XdevLDirectoryWatcher>(parameter, moduleDirectoryWatcherDescriptor),
	m_threadStarted(xdl_false) {
	};

XdevLDirectoryWatcherWindows::~XdevLDirectoryWatcherWindows() {
};

xdl_int XdevLDirectoryWatcherWindows::init() {
	XDEVL_MODULE_INFO("Initialize XdevLDirectoryWatcher was successful.\n");
	return ERR_OK;
}

xdl_int XdevLDirectoryWatcherWindows::shutdown() {

	if(xdl_true == m_threadStarted) {
		stop();
	}
		
	return 0;
}

int XdevLDirectoryWatcherWindows::start() {
	if(xdl_true == m_threadStarted) {
		return ERR_ERROR;
	}
	
	m_watcherThread = std::thread(&XdevLDirectoryWatcherWindows::threadHandle, this);
	m_threadStarted = xdl_true;
	m_runThread	= xdl_true;
	
	return ERR_OK;
}

int XdevLDirectoryWatcherWindows::stop() {
	if(xdl_true != m_threadStarted) {
		return ERR_ERROR;
	}

	m_runThread = false;
//	m_watcherThread.join();
	
	return ERR_OK;
}

xdl_int XdevLDirectoryWatcherWindows::addDirectoryToWatch(const XdevLString& folder) {


	return 0;
}
	
	int XdevLDirectoryWatcherWindows::registerDelegate(const XdevLDirectoryWatcherDelegateType& delegate) {
		m_delegates.push_back(delegate);
		return ERR_OK;
	}

	int XdevLDirectoryWatcherWindows::unregisterDelegate(const XdevLDirectoryWatcherDelegateType& delegate) {
		//m_delegates.find(delegate);
		return ERR_OK;
	}

	void XdevLDirectoryWatcherWindows::threadHandle() {
		XDEVL_MODULE_INFO("Starting Thread ...");
		

		// TODO As you can see this part is quite inefficient. Using a vector and
		// collecting first all events and then call the delegates.
		while(m_runThread) {
			std::lock_guard<std::mutex> lock(m_mutex);

		}
		
		XDEVL_MODULE_INFO("Stopping Thread ...");
	}

}
