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

#include "XdevLFileSystemMacOSX.h"

xdl::XdevLPluginDescriptor pluginDescriptor {
	xdl::XdevLFileSystemPluginName,
	xdl::XdevLFileSystemModuleName,
	xdl::XdevLFileSystemPluginMajorVersion,
	xdl::XdevLFileSystemPluginMinorVersion,
	xdl::XdevLFileSystemPluginPatchVersion
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

XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(pluginDescriptor);

extern "C" XDEVL_EXPORT xdl::XdevLModule* _createModule(const xdl::XdevLPluginDescriptor& pluginDescriptor, const xdl::XdevLModuleDescriptor& moduleDescriptor) {

	if(moduleDirectoryDescriptor.getName() == moduleDescriptor.getName()) {
		xdl::XdevLModule* obj = new xdl::XdevLDirectoryUnix(nullptr, moduleDirectoryDescriptor);
		if(!obj)
			return nullptr;

		return obj;

	} else if(moduleFileDescriptor.getName() == moduleDescriptor.getName()) {
		xdl::XdevLModule* obj = new xdl::XdevLFileUnix(nullptr, moduleFileDescriptor);
		if(!obj) {
			return nullptr;
		}

		return obj;

	} else if(moduleDirectoryWatcherDescriptor.getName() == moduleDescriptor.getName()) {
		xdl::XdevLModule* obj = new xdl::XdevLDirectoryWatcherMacOSX(nullptr, moduleDirectoryWatcherDescriptor);
		if(!obj) {
			return nullptr;
		}

		return obj;
	}

	return nullptr;
}

XDEVL_PLUGIN_CREATE_MODULE {
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLFileUnix, moduleFileDescriptor)
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLDirectoryUnix, moduleDirectoryDescriptor)
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLDirectoryWatcherMacOSX, moduleDirectoryWatcherDescriptor)
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}

namespace xdl {

	xdl_int XdevLDirectoryWatcherMacOSX::init() {
		return 0;
	}

	xdl_int XdevLDirectoryWatcherMacOSX::shutdown() {
		for(auto stream : m_streams) {
			FSEventStreamStop(stream);
			FSEventStreamInvalidate(stream);
			FSEventStreamRelease(stream);
		}
		return 0;
	}

	int XdevLDirectoryWatcherMacOSX::start() {
		for(auto stream : m_streams) {
			FSEventStreamScheduleWithRunLoop(stream, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
			FSEventStreamStart(stream);
		}
		CFRunLoopRun();
		return ERR_OK;
	}

	int XdevLDirectoryWatcherMacOSX::stop() {
		for(auto stream : m_streams) {
			FSEventStreamStop(stream);
			FSEventStreamInvalidate(stream);
			FSEventStreamRelease(stream);
		}
		return ERR_OK;
	}

	xdl_int XdevLDirectoryWatcherMacOSX::addDirectoryToWatch(const XdevLString& folder) {

		CFStringRef pathToWatchCF = CFStringCreateWithCString(kCFAllocatorDefault, folder.toString().c_str(), kCFStringEncodingUTF8);
		CFArrayRef pathsToWatch = CFArrayCreate(nullptr, (const void **)&pathToWatchCF, 1, nullptr);

		FSEventStreamContext context {0, this, nullptr, nullptr, nullptr};
		FSEventStreamRef streamRef = FSEventStreamCreate(nullptr,
		                             &XdevLDirectoryWatcherMacOSX::FileSystemEventCallback,
		                             &context,
		                             pathsToWatch,
		                             kFSEventStreamEventIdSinceNow,
		                             1.0,
		                             kFSEventStreamCreateFlagFileEvents);

		CFRelease(pathToWatchCF);

		m_streams.push_back(streamRef);

		return ERR_OK;
	}

	int XdevLDirectoryWatcherMacOSX::registerDelegate(const XdevLDirectoryWatcherDelegateType& delegate) {
		m_delegates.push_back(delegate);
		return ERR_OK;
	}

	int XdevLDirectoryWatcherMacOSX::unregisterDelegate(const XdevLDirectoryWatcherDelegateType& delegate) {
		auto foundDelegate = std::find(m_delegates.begin(), m_delegates.end(), delegate);
		if(foundDelegate != m_delegates.end()) {
			m_delegates.erase(foundDelegate);
		}
		return ERR_OK;
	}

	void XdevLDirectoryWatcherMacOSX::handleChanges(ItemTypes itemType, EventTypes eventType, const XdevLString& path) {
		//
		// Now check which event type occurred and use the delegate
		// to inform everyone.
		//
		if(eventType != EventTypes::DW_UNKNOWN) {
			for(auto& delegate : m_delegates) {
				delegate(itemType, eventType, path);
			}
		}
	}

	void XdevLDirectoryWatcherMacOSX::FileSystemEventCallback(ConstFSEventStreamRef streamRef,
	    void *clientCallBackInfo,
	    size_t numEvents,
	    void *eventPaths,
	    const FSEventStreamEventFlags eventFlags[],
	    const FSEventStreamEventId eventIds[]) {
		char **paths = (char **)eventPaths;

		XdevLDirectoryWatcherMacOSX* directoryWatcher = static_cast<XdevLDirectoryWatcherMacOSX*>(clientCallBackInfo);

		ItemTypes types = ItemTypes::FILE;
		EventTypes eventType = EventTypes::DW_UNKNOWN;
		for(size_t i=0; i<numEvents; i++) {
			if(eventFlags[i] & kFSEventStreamEventFlagItemCreated) {
				eventType = EventTypes::DW_CREATE;
			} else if(eventFlags[i] & kFSEventStreamEventFlagItemRemoved) {
				eventType = EventTypes::DW_DELETE;
			} else if(eventFlags[i] & kFSEventStreamEventFlagItemModified) {
				eventType = EventTypes::DW_MODIFY;
			}

			if(eventFlags[i] & kFSEventStreamEventFlagItemIsFile) {
				types = ItemTypes::FILE;
			} else if(eventFlags[i] & kFSEventStreamEventFlagItemIsDir) {
				types = ItemTypes::DIRECTORY;
			}

			directoryWatcher->handleChanges(types, eventType, XdevLString(paths[i]));

		}

	}


}
