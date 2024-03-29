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

#include "XdevLFileSystemLinux.h"


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
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(pluginDescriptor)

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
		xdl::XdevLModule* obj = new xdl::XdevLDirectoryWatcherLinux(nullptr, moduleDirectoryWatcherDescriptor);
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
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLDirectoryWatcherLinux, moduleDirectoryWatcherDescriptor)
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}


namespace xdl {

	XdevLDirectoryWatcherLinux::XdevLDirectoryWatcherLinux(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		XdevLModuleImpl<XdevLDirectoryWatcher>(parameter, descriptor),
		m_fd(-1) {
	}

	xdl_int XdevLDirectoryWatcherLinux::init() {
		//
		// Initialize notification system.
		//
		m_fd = inotify_init1(0); // Same as inotify_init
		if(m_fd == -1) {
			XDEVL_MODULE_INFO("Initialize XdevLDirectoryWatcher failed.\n");
			return ERR_ERROR;
		}

		XDEVL_MODULE_INFO("Initialize XdevLDirectoryWatcher was successful.\n");
		return ERR_OK;
	}

	xdl_int XdevLDirectoryWatcherLinux::shutdown() {
		//
		// If the notifications wasn't initialized we just return back now.
		//
		if(m_fd == -1) {
			return ERR_OK;
		}

		//
		// Check if the notification thread is running. If yes, stop the thread first.
		//
		if(m_runThread) {
			stop();
		}

		//
		// Now close all opened files.
		//
		close(m_fd);
		m_fd = -1;
		for(auto watcherfd : m_dfdList) {
			close(watcherfd);
		}

		return ERR_OK;
	}

	int XdevLDirectoryWatcherLinux::start() {

		//
		// Start the thread that handles the events.
		//
		m_watcherThread = std::thread(&XdevLDirectoryWatcherLinux::threadHandle, this);
		m_runThread			= xdl_true;

		return ERR_OK;
	}

	int XdevLDirectoryWatcherLinux::stop() {

		//
		// Stop the thread that handles the events.
		//
		if(xdl_true != m_runThread) {
			return ERR_OK;
		}

		m_runThread = xdl_false;
		m_watcherThread.join();

		return ERR_OK;
	}

	xdl_int XdevLDirectoryWatcherLinux::addDirectoryToWatch(const XdevLString& folder) {

		//
		// Add new directory watcher.
		//
		int wd = inotify_add_watch(m_fd, folder.toString().c_str(), IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO | IN_MODIFY);
		if(wd == -1) {
			XDEVL_MODULE_INFO("Adding directory: " << folder.toString() << " to watch failed.: " << strerror(errno) << std::endl);
			return -1;
		}
		m_dfdList.push_back(wd);

		return 0;
	}

	int XdevLDirectoryWatcherLinux::registerDelegate(const XdevLDirectoryWatcherDelegateType& delegate) {

		//
		// Add delegate only if not added already.
		//
		auto foundDelegate = std::find(m_delegates.begin(), m_delegates.end(), delegate);
		if(foundDelegate == m_delegates.end()) {
			m_delegates.push_back(delegate);
		}

		return ERR_OK;
	}

	int XdevLDirectoryWatcherLinux::unregisterDelegate(const XdevLDirectoryWatcherDelegateType& delegate) {

		//
		// Remove delegate only if added already.
		//
		auto foundDelegate = std::find(m_delegates.begin(), m_delegates.end(), delegate);
		if(foundDelegate != m_delegates.end()) {
			m_delegates.erase(foundDelegate);
		}

		return ERR_OK;
	}

	void XdevLDirectoryWatcherLinux::threadHandle() {
		XDEVL_MODULE_INFO("Starting Thread ...");

		struct inotify_event* event;

		struct timeval timeout;
		fd_set set;

		//
		// Here we start the main loop. First we use select to see if there are any events.
		// If yes, then we handle those events. If not we use the timeout to checkout if
		// we have to stop the event. This might not be the best way to exit a blocking thread
		// but it works.
		for(;;) {

			//
			// Reset timout variables.
			//
			timeout.tv_sec = 0;
			timeout.tv_usec = 1000000;
			FD_ZERO(&set);
			FD_SET(m_fd, &set);


			m_buffer = {{0}};

			xdl_int rv = select(m_fd + 1, &set, NULL, NULL, &timeout);
			if(rv == -1) {
				XDEVL_MODULE_INFO("select failed: " << strerror(errno) << std::endl);
			} else if(rv == 0) {
				// Ok we timed out here. Lets see if the user stopped the thread.
				if(xdl_false == m_runThread) {
					break;
				}
			} else {
				xdl_int len = read(m_fd, m_buffer.data(), 1024);
				xdl_int i = 0;
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
					}
					//
					// A file or folder got created.
					//
					else if(event->mask & IN_CREATE) {
						eventType = EventTypes::DW_CREATE;
					}
					//
					// A file or folder got modified.
					//
					else if(event->mask & IN_MODIFY) {
						eventType = EventTypes::DW_MODIFY;
					}
					//
					// The flowing ones are not really implemented.
					//
					else if(event->mask & IN_ATTRIB) {
						XDEVL_MODULE_INFO("IN_ATTRIB not handled.\n");
					} else if(event->mask & IN_ACCESS) {
						eventType = EventTypes::DW_ACCESS;
					} else if(event->mask & IN_CLOSE_WRITE) {
						eventType = EventTypes::DW_CLOSE;
					} else if(event->mask & IN_CLOSE_NOWRITE) {
						eventType = EventTypes::DW_CLOSE;
					} else if(event->mask & IN_DELETE_SELF) {
						XDEVL_MODULE_INFO("IN_DELETE_SELF not handled.\n");
					}
					//
					// A file or folder got removed.
					// We handle it as delete.
					//
					else if(event->mask & IN_MOVED_FROM) {
						eventType = EventTypes::DW_DELETE;
					}
					//
					// A file or folder got renamed.
					// We handle this as create because renaming
					// involves IN_MOVED_FROM then IN_MOVED_TO.
					//
					else if(event->mask & IN_MOVED_TO) {
						eventType = EventTypes::DW_CREATE;
					}
					//
					// A file or folder got really deleted.
					//
					else if(event->mask & IN_DELETE) {
						eventType = EventTypes::DW_DELETE;
					}

					//
					// Now check which event type occurred and use the delegate
					// to inform everyone.
					//
					if(eventType != EventTypes::DW_UNKNOWN) {
						for(auto& delegate : m_delegates) {
							delegate(types, eventType, XdevLString(event->name));
						}
					}

					/* update index to start of next event */
					i += sizeof(struct inotify_event) + event->len;
				}
			}
		}

		XDEVL_MODULE_INFO("Stopping Thread ...");
	}

}
