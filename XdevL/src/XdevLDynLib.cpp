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

#include <XdevLDynLib.h>
#include <XdevLXfstring.h>
#include <string>
#include <iostream>

#define TOSTRING(x) #x

namespace xdl {

	XdevLString XdevLSharedLibrary::extension = XdevLString(".xdp");

	XdevLSharedLibrary::XdevLSharedLibrary() : m_Module(0) {
	}

	XdevLSharedLibrary::~XdevLSharedLibrary() {
	}

	xdl_int XdevLSharedLibrary::open(const xdl_char* pName) {

		if(m_Module) {
			if(close() != 0) {
				// TODO Do we have to do something here?
			}
		}

		// ATTN: The file extension must not be written, otherwise it will fail.
		std::string filename(pName);
		xstd::make_unix_file(filename);
		std::string path		= xstd::get_path(filename);
		std::string pathless = xstd::get_pathless(filename);
		std::string extless;
		// Check if the file has a valid extension.
		if(hasValidExtension(filename) == false) {
			extless = pathless;
		} else {
			extless = xstd::get_extless(pathless);
		}
#if defined (XDEVL_PLATFORM_UNIX)
		filename.clear();
		if(path.size() == 0)
			filename = "./";
		else
			filename = path + "/";
		filename += extless + extension.toString();
		m_Module = dlopen(filename.c_str(), RTLD_LAZY);
		if(m_Module == NULL) {
			std::cout << "XdevLSharedLibrary::LoadLib: dlopen failed:" << dlerror() << std::endl;
			return 1;
		}
#elif defined (XDEVL_PLATFORM_WINDOWS)
		// Ok, this is because LoadLibrary can not handle "./xxx.dll" properly.
		if(path == xstd::current_path_id)
			filename = extless + extension.toString();
		else
			filename = path + "/" + extless + extension.toString();

		m_Module = LoadLibraryA(filename.c_str());
		if(m_Module == NULL) {
			std::cout << "XdevLSharedLibrary::LoadLib: LoadLibraryA failed:"  << GetLastError() << std::endl;
			return 1;
		}
#elif defined(XDEVL_PLATFORM_MINGW)
		if(path == xstd::current_path_id)
			filename = extless + extension.toString();
		else
			filename = path + "/" + extless + extension.toString();
		m_Module = LoadLibraryA(filename.c_str());
		if(m_Module == NULL) {
			return 1;
		}
#else
#error XDEVL_UNKNOWN_PLATFORM
#endif

		return 0;
	}

	xdl_int XdevLSharedLibrary::close() {

		// Already desytroyed?
		if(m_Module == 0) {
			return 1;
		}
#if defined (XDEVL_PLATFORM_UNIX)
		if(dlclose(m_Module) != 0) {
			std::cout << "XdevLSharedLibrary::close: dlclose failed: " << dlerror() << std::endl;
			return 1;
		}
#elif defined (XDEVL_PLATFORM_WINDOWS) ||  defined(XDEVL_PLATFORM_MINGW)
		if(FreeLibrary(static_cast<HMODULE>(m_Module)) == 0) {
			std::cerr << "XdevLSharedLibrary::FreeLib: FreeLibrary failed: " << GetLastError() << std::endl;
			return 1;
		}
#else
#error XDEVL_UNKNOWN_PLATFORM
#endif
		m_Module = 0;
		return 0;
	}

	SHARED_FUNCTION XdevLSharedLibrary::getFunctionAddress(const xdl_char* pName) {

		SHARED_FUNCTION lProc = 0;

#if defined (XDEVL_PLATFORM_UNIX)
		lProc = (SHARED_FUNCTION)dlsym(m_Module, pName);
		if(lProc == 0) {
			std::cout << "XdevLSharedLibrary::GetFunctionAddress: dlsym failed: " << dlerror() << std::endl;
			return lProc;
		}
#elif defined (XDEVL_PLATFORM_WINDOWS) ||  defined(XDEVL_PLATFORM_MINGW)
		lProc = (SHARED_FUNCTION)(GetProcAddress(static_cast<HMODULE>(m_Module), pName));
		if(lProc == 0) {
			std::cerr << "XdevLSharedLibrary::GetFunctionAddress: GetProcAddress failed: " << GetLastError() << std::endl;
			return lProc;
		}
#else
#error XDEVL_UNKNOWN_PLATFORM
#endif
		return lProc;
	}

	bool  XdevLSharedLibrary::hasValidExtension(std::string& str) {
		std::string ext	= xstd::get_ext(str);
		if(XdevLSharedLibrary::extension.toString().find(ext) != std::string::npos) {
			return true;
		}

		return false;
	}
}
