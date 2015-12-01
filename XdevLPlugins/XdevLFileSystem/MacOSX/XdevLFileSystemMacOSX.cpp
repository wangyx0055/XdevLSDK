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


xdl::XdevLModuleDescriptor xdl::XdevLDirectoryWatcherMacOSX::m_moduleDescriptorDirectoryWatcher {
	xdl::XdevLFileSystemVendor,
	xdl::XdevLFileSystemAuthor,
	xdl::XdevLFileSystemModuleName[2],
	xdl::XdevLFileSystemCopyright,
	xdl::XdevLDescriptionForFileSystem,
	xdl::XdevLDirectoryWatcherMajorVersion,
	xdl::XdevLDirectoryWatcherMinorVersion,
	xdl::XdevLDirectoryWatcherPatchVersion
};

extern "C" XDEVL_EXPORT xdl::XdevLModule* _createModule(const xdl::XdevLPluginDescriptor& pluginDescriptor, const xdl::XdevLModuleDescriptor& moduleDescriptor) {

	if(xdl::XdevLDirectoryUnix::m_moduleDescriptor.getName() == moduleDescriptor.getName()) {
		xdl::XdevLModule* obj = new xdl::XdevLDirectoryUnix(nullptr);
		if(!obj)
			return nullptr;
		
		return obj;

	} else if(xdl::XdevLFileUnix::m_moduleDescriptor2.getName() == moduleDescriptor.getName()) {
		xdl::XdevLModule* obj = new xdl::XdevLFileUnix(nullptr);
		if(!obj) {
			return nullptr;
		}

		return obj;

	} else if(xdl::XdevLDirectoryWatcherMacOSX::m_moduleDescriptorDirectoryWatcher.getName() == moduleDescriptor.getName()) {
		xdl::XdevLModule* obj = new xdl::XdevLDirectoryWatcherMacOSX(nullptr);
		if(!obj) {
			return nullptr;
		}

		return obj;
	}

	return nullptr;
}

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {

	if(xdl::XdevLDirectoryUnix::m_moduleDescriptor.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj = new xdl::XdevLDirectoryUnix(parameter);
		if(nullptr == obj) {
			return xdl::ERR_ERROR;
		}

		parameter->setModuleInstance(obj);

		return xdl::ERR_OK;
	} else if(xdl::XdevLFileUnix::m_moduleDescriptor2.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj = new xdl::XdevLFileUnix(parameter);
		if(nullptr == obj) {
			return xdl::ERR_ERROR;
		}

		parameter->setModuleInstance(obj);

		return xdl::ERR_OK;
	} else if(xdl::XdevLDirectoryWatcherMacOSX::m_moduleDescriptorDirectoryWatcher.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj = new xdl::XdevLDirectoryWatcherMacOSX(parameter);
		if(nullptr == obj) {
			return xdl::ERR_ERROR;
		}

		parameter->setModuleInstance(obj);

		return xdl::ERR_OK;
	}

	return xdl::ERR_MODULE_NOT_FOUND;
}

namespace xdl {

	xdl_int XdevLDirectoryWatcherMacOSX::init() {
		return 0;
	}

	xdl_int XdevLDirectoryWatcherMacOSX::shutdown() {

		if(xdl_true == m_threadStarted) {
			stop();
		}

		FSEventStreamStop(m_stream);
		FSEventStreamInvalidate(m_stream);
		FSEventStreamRelease(m_stream);

		return 0;
	}

	int XdevLDirectoryWatcherMacOSX::start() {
		if(xdl_true == m_threadStarted) {
			return ERR_ERROR;
		}

		m_watcherThread = std::thread(&XdevLDirectoryWatcherMacOSX::threadHandle, this);
		m_threadStarted = xdl_true;
		m_runThread			= xdl_true;

		return ERR_OK;
	}

	int XdevLDirectoryWatcherMacOSX::stop() {
		if(xdl_true != m_threadStarted) {
			return ERR_ERROR;
		}

		m_runThread = false;
		m_watcherThread.join();

		return ERR_OK;
	}

	xdl_int XdevLDirectoryWatcherMacOSX::addDirectoryToWatch(const XdevLString& folder) {
		std::lock_guard<std::mutex> lock(m_mutex);

		if(m_stream == nullptr) {
			CFStringRef pathToWatchCF = CFStringCreateWithCString(nullptr, folder.toString().c_str(), kCFStringEncodingUTF8);
			CFArrayRef pathsToWatch = CFArrayCreate(nullptr, (const void **)&pathToWatchCF, 1, nullptr);

			FSEventStreamContext context;
			context.version 					= 0;
			context.release 					= nullptr;
			context.retain 						= nullptr;
			context.copyDescription 	= nullptr;
			context.info 							= this;


			m_stream = FSEventStreamCreate(nullptr, &XdevLDirectoryWatcherMacOSX::FileSystemEventCallback, &context, pathsToWatch, kFSEventStreamEventIdSinceNow, 3.0, kFSEventStreamCreateFlagFileEvents);
			FSEventStreamScheduleWithRunLoop(m_stream, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
			FSEventStreamStart(m_stream);

			CFRelease(pathToWatchCF);
		} else  {
			// Add new path here.
		}

		return 0;
	}

	int XdevLDirectoryWatcherMacOSX::registerDelegate(const XdevLDirectoryWatcherDelegateType& delegate) {
		m_delegates.push_back(delegate);
		return ERR_OK;
	}

	int XdevLDirectoryWatcherMacOSX::unregisterDelegate(const XdevLDirectoryWatcherDelegateType& delegate) {
		//m_delegates.find(delegate);
		return ERR_OK;
	}

	void XdevLDirectoryWatcherMacOSX::threadHandle() {
		XDEVL_MODULE_INFO("Starting Thread ...");

		struct inotify_event* event;

		// Start loop.
		while(m_runThread) {

			std::lock_guard<std::mutex> lock(m_mutex);
		}

		XDEVL_MODULE_INFO("Stopping Thread ...");
	}

	void XdevLDirectoryWatcherMacOSX::FileSystemEventCallback(ConstFSEventStreamRef,
	        void *clientCallBackInfo,
	        size_t numEvents,
	        void *eventPaths,
	        const FSEventStreamEventFlags eventFlags[],
	        const FSEventStreamEventId eventIds[]) {
		char **paths = (char **)eventPaths;

		for(size_t i=0; i<numEvents; i++) {
			// When a file is created we receive first a kFSEventStreamEventFlagItemCreated and second a (kFSEventStreamEventFlagItemCreated & kFSEventStreamEventFlagItemModified)
			// when the file is finally copied. Catch this second event.
			if(eventFlags[i] & kFSEventStreamEventFlagItemCreated
			        && eventFlags[i] & kFSEventStreamEventFlagItemModified
			        && !(eventFlags[i] & kFSEventStreamEventFlagItemIsDir)
			        && !(eventFlags[i] & kFSEventStreamEventFlagItemIsSymlink)
			        && !(eventFlags[i] & kFSEventStreamEventFlagItemFinderInfoMod)) {

//            XdevLDirectoryWatcherMacOSX *watcher = (XdevLDirectoryWatcherMacOSX *)clientCallBackInfo;
//            if (watcher->FileValidator(paths[i]))
//                emit watcher->yourSignalHere();
			}
		}
		printf("**********\n");
	}


}
