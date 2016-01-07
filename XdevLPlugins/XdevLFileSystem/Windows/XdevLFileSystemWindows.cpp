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
	CHAR errorMsgText[256];

	bool sort_name_dir_entitiy(const XdevLDirectoryEntity& i, const XdevLDirectoryEntity& j) {
		std::string si(i.getName());
		std::string sj(j.getName());
		return (si < sj);
	}


// usage
//     CHAR msgText[256];
//     getLastErrorText(msgText,sizeof(msgText));
	static CHAR *getLastErrorText(CHAR *pBuf, ULONG bufSize) {
		DWORD retSize;
		LPTSTR pTemp=NULL;

		if (bufSize < 16) {
			if (bufSize > 0) {
				pBuf[0]='\0';
			}
			return(pBuf);
		}
		retSize=FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|
		                      FORMAT_MESSAGE_FROM_SYSTEM|
		                      FORMAT_MESSAGE_ARGUMENT_ARRAY,
		                      NULL,
		                      GetLastError(),
		                      LANG_NEUTRAL,
		                      (LPTSTR)&pTemp,
		                      0,
		                      NULL );
		if (!retSize || pTemp == NULL) {
			pBuf[0]='\0';
		} else {
			pTemp[strlen(pTemp)-2]='\0'; //remove cr and newline character
			sprintf(pBuf,"%0.*s (0x%x)",bufSize-16,pTemp,GetLastError());
			LocalFree((HLOCAL)pTemp);
		}
		return(pBuf);
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
		XdevLModuleImpl<XdevLDirectory>(parameter, moduleDirectoryDescriptor),
		m_handle(INVALID_HANDLE_VALUE) {
	}

	xdl_int XdevLDirectoryWindows::init() {
		return ERR_OK;
	}

	void* XdevLDirectoryWindows::getInternal(const XdevLInternalName& id) {
		return nullptr;
	}

	xdl_int XdevLDirectoryWindows::shutdown() {
		return close();
	}

	xdl_int XdevLDirectoryWindows::open() {
		return open(XdevLString("."));
	}

	xdl_int XdevLDirectoryWindows::open(const XdevLString& dirName) {

		WIN32_FIND_DATA fdata;

		std::string dirname = dirName.toString();
		if(dirname[dirname.size()-1] == '\\' || dirname[dirname.size()-1] == '/') {
			dirname = dirname.substr(0,dirname.size()-1);
		}

		m_handle = FindFirstFile(std::string(dirname).append("\\*").c_str(), &fdata);
		if (m_handle != INVALID_HANDLE_VALUE) {
			do {
				if (strcmp(fdata.cFileName, ".") != 0 &&
				    strcmp(fdata.cFileName, "..") != 0) {
					if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
						XdevLDirectoryEntity entity(XdevLString(fdata.cFileName), XdevLDirectoryEntity::Type::DIRECTORY);
						m_directoryItems.push_back(entity);
					} else {
						XdevLDirectoryEntity entity(XdevLString(fdata.cFileName), XdevLDirectoryEntity::Type::REGULAR);
						m_directoryItems.push_back(entity);
					}
				}
			} while (FindNextFile(m_handle, &fdata) != 0);
		} else {
			XDEVL_MODULE_ERROR("Can't open directory\n");
			return ERR_ERROR;
		}

		if (GetLastError() != ERROR_NO_MORE_FILES) {
			FindClose(m_handle);
			XDEVL_MODULE_ERROR("Can't open directory: " << GetLastError() << std::endl);
			return ERR_ERROR;
		}
		m_directoryItemsIterator = m_directoryItems.begin();
		return ERR_OK;
	}

	xdl_int XdevLDirectoryWindows::close() {
		if(INVALID_HANDLE_VALUE != m_handle) {
			FindClose(m_handle);
			m_handle = INVALID_HANDLE_VALUE;
		}
		return ERR_OK;
	}

	xdl_int XdevLDirectoryWindows::changeTo(const XdevLString& dirname) {
		if(::SetCurrentDirectory(dirname.toString().c_str()) == 0) {
			XDEVL_MODULE_ERROR(getLastErrorText(errorMsgText, sizeof(errorMsgText)));
			return ERR_ERROR;
		}
		return ERR_OK;
	}

	xdl_int XdevLDirectoryWindows::make(const XdevLString& dirname) {
		if(::CreateDirectory(dirname.toString().c_str(), nullptr) == 0) {
			XDEVL_MODULE_ERROR(getLastErrorText(errorMsgText, sizeof(errorMsgText)));
			return ERR_ERROR;
		}
		return ERR_OK;
	}

	xdl_int XdevLDirectoryWindows::remove(const XdevLString& dirname) {
		if(::RemoveDirectory(dirname.toString().c_str()) == 0) {
			XDEVL_MODULE_ERROR(getLastErrorText(errorMsgText, sizeof(errorMsgText)));
			return ERR_ERROR;
		}
		return ERR_OK;
	}

	xdl_int XdevLDirectoryWindows::rename(const XdevLString& oldname, const XdevLString& newname) {
		if(::MoveFile(oldname.toString().c_str(), newname.toString().c_str()) == 0) {
			XDEVL_MODULE_ERROR(getLastErrorText(errorMsgText, sizeof(errorMsgText)));
			return ERR_ERROR;
		}
		return ERR_OK;
	}

	XdevLString XdevLDirectoryWindows::getWorkingDirectory() {
		char buffer[MAX_PATH];
		GetModuleFileName( NULL, buffer, MAX_PATH );
		XdevLString tmp(buffer);
		return std::move(tmp);
	}

	xdl_uint64 XdevLDirectoryWindows::getNumberOfEntities() {
		return m_directoryItems.size();
	}

	xdl_int XdevLDirectoryWindows::getEntity(XdevLDirectoryEntity& entity) {
		entity = *m_directoryItemsIterator;
		return ERR_OK;
	}

	std::vector<XdevLDirectoryEntity> XdevLDirectoryWindows::getEntities() {
		std::vector<XdevLDirectoryEntity>	m_entity_list(m_directoryItems);
	

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

		for(auto& watcher : m_watcherItems) {
			destroyWatch(watcher);
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

	XdevLDirectoryWatcherWindowsItem* XdevLDirectoryWatcherWindows::createWatch(LPCTSTR szDirectory, bool recursive, DWORD mNotifyFilter) {
		XdevLDirectoryWatcherWindowsItem* watcherItem = nullptr;
		size_t ptrsize = sizeof(*watcherItem);
		watcherItem = static_cast<XdevLDirectoryWatcherWindowsItem*>(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ptrsize));

		watcherItem->m_handle = ::CreateFile(szDirectory,
		                                     FILE_LIST_DIRECTORY,
		                                     FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		                                     nullptr,
		                                     OPEN_EXISTING,
		                                     FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
		                                     nullptr);

		if ( watcherItem->m_handle == INVALID_HANDLE_VALUE ) {
			return nullptr;
		}
		watcherItem->m_overlapped.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
		watcherItem->m_recursive = recursive;
		watcherItem->m_notifyFilter = mNotifyFilter;

		if(refreshWatch(watcherItem)) {
			return watcherItem;
		} else {
			CloseHandle(watcherItem->m_overlapped.hEvent);
			CloseHandle(watcherItem->m_handle);
		}

		HeapFree(GetProcessHeap(), 0, watcherItem);
		return nullptr;
	}

	void XdevLDirectoryWatcherWindows::destroyWatch(XdevLDirectoryWatcherWindowsItem* watcherItem) {
		if (watcherItem) {
			CancelIo(watcherItem->m_handle);
			refreshWatch(watcherItem, true);

			if (!HasOverlappedIoCompleted(&watcherItem->m_overlapped)) {
				SleepEx(5, TRUE);
			}

			CloseHandle(watcherItem->m_overlapped.hEvent);
			CloseHandle(watcherItem->m_handle);
			HeapFree(GetProcessHeap(), 0, watcherItem);
		}
	}



	xdl_int XdevLDirectoryWatcherWindows::addDirectoryToWatch(const XdevLString& folder) {
		xdl_bool recursive = xdl_false;
		XdevLDirectoryWatcherWindowsItem* watcherItem = createWatch(folder.toString().c_str(),
		    recursive,
		    FILE_NOTIFY_CHANGE_CREATION |
		    FILE_NOTIFY_CHANGE_SIZE |
		    FILE_NOTIFY_CHANGE_FILE_NAME |
		    FILE_NOTIFY_CHANGE_DIR_NAME);
		if(nullptr == watcherItem) {
			return ERR_ERROR;
		}
		watcherItem->m_directorName = folder;
		watcherItem->m_directoryWatcher = this;
		m_watcherItems.push_back(watcherItem);
		XDEVL_MODULE_INFO("Adding directory: " << folder << " to watch.\n");
	}

	bool XdevLDirectoryWatcherWindows::refreshWatch(XdevLDirectoryWatcherWindowsItem* watcherItem, xdl_bool clear) {
		return ReadDirectoryChangesW(watcherItem->m_handle, watcherItem->m_buffer, sizeof(watcherItem->m_buffer), watcherItem->m_recursive,
		                             watcherItem->m_notifyFilter, NULL, &watcherItem->m_overlapped, clear ? 0 : ChangeNotification) != 0;
	}


	int XdevLDirectoryWatcherWindows::registerDelegate(const XdevLDirectoryWatcherDelegateType& delegate) {
		m_delegates.push_back(delegate);
		return ERR_OK;
	}

	int XdevLDirectoryWatcherWindows::unregisterDelegate(const XdevLDirectoryWatcherDelegateType& delegate) {
		//m_delegates.find(delegate);
		return ERR_OK;
	}

	void XdevLDirectoryWatcherWindows::ChangeNotification(::DWORD Error, ::DWORD NumBytes, LPOVERLAPPED InOverlapped) {
		TCHAR szFile[MAX_PATH];
		PFILE_NOTIFY_INFORMATION pNotify;
		size_t offset = 0;

		if(NumBytes == 0)
			return;

		XdevLDirectoryWatcherWindowsItem* watcherItem = (XdevLDirectoryWatcherWindowsItem*) InOverlapped;
		if (Error == ERROR_SUCCESS) {
			do {
				pNotify = (PFILE_NOTIFY_INFORMATION) &watcherItem->m_buffer[offset];
				offset += pNotify->NextEntryOffset;

#if defined(UNICODE)
				{
					lstrcpynW(szFile, pNotify->FileName, min(MAX_PATH, pNotify->FileNameLength / sizeof(WCHAR) + 1));
				}
#else
				{
					int count = WideCharToMultiByte(CP_ACP, 0, pNotify->FileName, pNotify->FileNameLength / sizeof(WCHAR),  szFile, MAX_PATH - 1, NULL, NULL);
					szFile[count] = TEXT('\0');
				}
#endif


				watcherItem->m_directoryWatcher->handleChange(watcherItem, XdevLString(szFile), pNotify->Action);

			} while (pNotify->NextEntryOffset != 0);
		}

		if (!watcherItem->m_stopNow) {
			watcherItem->m_directoryWatcher->refreshWatch(watcherItem);
		}

	}

	void XdevLDirectoryWatcherWindows::handleChange(XdevLDirectoryWatcherWindowsItem* watchteritem, const XdevLString& filename, unsigned long action) {
		ItemTypes types = ItemTypes::FILE;
		EventTypes eventType = EventTypes::DW_UNKNOWN;

		switch(action) {
			case FILE_ACTION_RENAMED_NEW_NAME:
			case FILE_ACTION_ADDED:
				eventType = EventTypes::DW_CREATE;
				break;
			case FILE_ACTION_RENAMED_OLD_NAME:
			case FILE_ACTION_REMOVED:
				eventType = EventTypes::DW_DELETE;
				break;
			case FILE_ACTION_MODIFIED:
				eventType = EventTypes::DW_MODIFY;
				break;
		};

		//
		// Now check which event type occurred and use the delegate
		// to inform everyone.
		//
		if(eventType != EventTypes::DW_UNKNOWN) {
			for(auto& delegate : m_delegates) {
				delegate(types, eventType, filename);
			}
		}
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
