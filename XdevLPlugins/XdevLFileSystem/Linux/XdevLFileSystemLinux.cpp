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

#include "XdevLFileSystemLinux.h"

xdl::XdevLModuleDescriptor xdl::XdevLDirectoryWatcherLinux::m_moduleDescriptorDirectoryWatcher {	XdevLFileSystemVendor,
    XdevLFileSystemAuthor,
    XdevLFileSystemModuleName[2],
    XdevLFileSystemCopyright,
    XdevLDescriptionForFileSystem,
    XdevLDirectoryWatcherMajorVersion,
    XdevLDirectoryWatcherMinorVersion,
    XdevLDirectoryWatcherPatchVersion
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

	} else if(xdl::XdevLDirectoryWatcherLinux::m_moduleDescriptorDirectoryWatcher.getName() == moduleDescriptor.getName()) {
		xdl::XdevLModule* obj = new xdl::XdevLDirectoryWatcherLinux(nullptr);
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
	} else if(xdl::XdevLDirectoryWatcherLinux::m_moduleDescriptorDirectoryWatcher.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj = new xdl::XdevLDirectoryWatcherLinux(parameter);
		if(nullptr == obj) {
			return xdl::ERR_ERROR;
		}

		parameter->setModuleInstance(obj);

		return xdl::ERR_OK;
	}

	return xdl::ERR_MODULE_NOT_FOUND;
}

namespace xdl {

xdl_int XdevLDirectoryWatcherLinux::init() {
	// Initialize notification system.
	m_fd = inotify_init1(0); // Same as inotify_init
	if(m_fd == -1) {
		XDEVL_MODULE_INFO("Initialize XdevLDirectoryWatcher failed.\n");
		return ERR_ERROR;
	}
	XDEVL_MODULE_INFO("Initialize XdevLDirectoryWatcher was successful.\n");
	return ERR_OK;
}

xdl_int XdevLDirectoryWatcherLinux::shutdown() {

	close(m_fd);
	
	if(xdl_true == m_threadStarted) {
		stop();
	}
		
	return 0;
}

int XdevLDirectoryWatcherLinux::start() {
	if(xdl_true == m_threadStarted) {
		return ERR_ERROR;
	}
	
	m_watcherThread = std::thread(&XdevLDirectoryWatcherLinux::threadHandle, this);
	m_threadStarted = xdl_true;
	m_runThread			= xdl_true;
	
	return ERR_OK;
}

int XdevLDirectoryWatcherLinux::stop() {
	if(xdl_true != m_threadStarted) {
		return ERR_ERROR;
	}

	m_runThread = false;
//	m_watcherThread.join();
	
	return ERR_OK;
}

xdl_int XdevLDirectoryWatcherLinux::addDirectoryToWatch(const XdevLString& folder) {
//	std::lock_guard<std::mutex> lock(m_mutex);

	int wd = inotify_add_watch(m_fd, folder.toString().c_str(), IN_CREATE | IN_ALL_EVENTS);
	if(wd == -1) {
		XDEVL_MODULE_INFO("Adding directory: " << folder.toString() << " to watch failed.\n");
		return -1;
	}
	m_dfdList.push_back(wd);

	return 0;
}
	
	int XdevLDirectoryWatcherLinux::registerDelegate(const XdevLDirectoryWatcherDelegateType& delegate) {
		m_delegates.push_back(delegate);
		return ERR_OK;
	}

	int XdevLDirectoryWatcherLinux::unregisterDelegate(const XdevLDirectoryWatcherDelegateType& delegate) {
		//m_delegates.find(delegate);
		return ERR_OK;
	}

	void XdevLDirectoryWatcherLinux::threadHandle() {
		XDEVL_MODULE_INFO("Starting Thread ...");
		
		struct inotify_event* event;
		
		// TODO As you can see this part is quite inefficient. Using a vector and
		// collecting first all events and then call the delegates.
		while(m_runThread) {

			std::lock_guard<std::mutex> lock(m_mutex);
			
			m_buffer = {{0}};
			
			int i = 0;
			int len = read(m_fd, m_buffer.data(), 1024);

			while(i < len) {
				event = (struct inotify_event*)&m_buffer[i];
				
				// Is it a file or directory?
				ItemTypes types = ItemTypes::FILE;
				EventTypes eventType = EventTypes::DW_UNKNOWN;
				
				if(event->mask & IN_ISDIR) {
					types = ItemTypes::DIRECTORY;
				}
				
				/* check for changes */
				if(event->mask & IN_OPEN) {
					eventType = EventTypes::DW_OPEN;
				} else
				if(event->mask & IN_CREATE) {
					eventType = EventTypes::DW_CREATE;
				} else
				if(event->mask & IN_MODIFY) {
					eventType = EventTypes::DW_MODIFY;	
				} else
				if(event->mask & IN_ATTRIB) {
					XDEVL_MODULE_INFO("IN_ATTRIB not handled.\n");
				} else
				if(event->mask & IN_ACCESS) {
					eventType = EventTypes::DW_ACCESS;
				} else
				if(event->mask & IN_CLOSE_WRITE) {
					eventType = EventTypes::DW_CLOSE;
				} else
				if(event->mask & IN_CLOSE_NOWRITE) {
					eventType = EventTypes::DW_CLOSE;
				} else
				if(event->mask & IN_DELETE_SELF) {
					XDEVL_MODULE_INFO("IN_DELETE_SELF not handled.\n");
				} else
				if(event->mask & IN_DELETE) {
					eventType = EventTypes::DW_DELETE;
				}
				
				if(eventType != EventTypes::DW_UNKNOWN) {
					for(auto& delegate : m_delegates) {
							delegate(types, eventType, XdevLString(event->name));
					}
				}
				
				/* update index to start of next event */
				i += sizeof(struct inotify_event) + event->len;
			}

		}
		
		XDEVL_MODULE_INFO("Stopping Thread ...");
	}

}
