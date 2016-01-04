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
