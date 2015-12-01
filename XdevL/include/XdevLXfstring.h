/*
	The MIT License

	Copyright (c) 2005-2015 Cengiz Terzibas & Mesut Bueyuektokatli

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/


#ifndef XFSTRING_H
#define XFSTRING_H

#include <string.h>
#include <string>
#include <algorithm>


namespace xstd {

// If this is defined, platform specific path separator will be used. Otherwise everything
// will be changed to UNIX compatible path separator.
//#define XFSTRING_PSS

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

	const std::string current_path_id		=  ".";
	const std::string parent_path_id		= "..";

	const std::string parent_seperator_id_dos = "\\";
	const std::string parent_seperator_id_unix = "/";

#ifdef XFSTRING_PSS
	const std::string parent_seperator_id = parent_seperator_id_dos;
#else
	const std::string parent_seperator_id = parent_seperator_id_unix;
#endif



template<typename T>
void replace_all(T& str, const T& from, const T& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

	/**
		@fn template<typename T> void make_unix_file(T& str)
		@brief Makes path separators UNIX compatible.
		@param str A std::string which will be changed.
	*/
	template<typename T>
	void make_unix_file(T& str) {
		std::replace(str.begin(), str.end(), '\\', '/');
	}

	/**
		@fn template<typename T> void make_dos_file(T& str)
		@brief Makes path separators dos compatible.
		@param str A std::string which will be changed.
	*/
	template<typename T>
	void make_dos_file(T& str) {
		std::replace(str.begin(), str.end(), '/', '\\');
	}

/// Makes the string to a path. All Backslashes will be
/// replaced by slashes and at the end a slash will be
/// appended, if needed.
	template<typename T>
	inline void make_path(T& str) {
#ifdef XFSTRING_PSS
#ifdef _MSC_VER
		make_unix_file(str);
#elif defined(__GNUC__) && defined(_WIN32)
		make_dos_file(str);
#else
		make_dos_file(str);
#endif
#else
		make_unix_file(str);
#endif
		replace_all<T>(str, "//", "/");
	}

	/**
		@fn template<typename T> bool is_path(const T& str)
		@brief Test std::string/std::wstring if it represents a path.
		@param str A std::string/std::wstring to check.
		@return 'true' if the str represents a path. 'false' if not.
	*/
	template<typename T>
	bool is_path(const T& str) {
		if(!str.empty())
			if(((*(str.end()-1)) == '/') || ((*(str.end()-1)) == '\\'))
				return true;
		return false;
	}

	/**
		@fn bool is_path(const char* str)
		@brief Test std::string/std::wstring if it represents a path.
		@param str A const char* which holds the string to check.
		@return 'true' if the str represents a path. 'false' if not.
	*/
	inline
	bool is_path(const char* str) {
		if(strlen(str) > 0) {
			char c(str[strlen(str)-sizeof(char)]);
			if((c == '/') || (c == '\\'))
				return true;
		}
		return false;
	}
	/**
		@fn bool is_path(const wchar_t* str)
		@brief Test std::string/std::wstring if it represents a path.
		@param str A const wchar_t* which holds the string to check.
		@return 'true' if the str represents a path. 'false' if not.
	*/
	inline
	bool is_path(const wchar_t* str) {
		if(wcslen(str) > 0) {
			wchar_t c(str[wcslen(str)-sizeof(wchar_t)]);
			if((c == '/') || (c == '\\'))
				return true;
		}
		return false;
	}

	/**
		@fn template<typename T> bool is_file(const T& str)
		@brief Test std::string/std::wstring if it represents a file.
		@param str A std::string/std::wstring to check.
		@return 'true' if the str represents a file. 'false' if not.
	*/
	template<typename T>
	bool is_file(const T& str) {
		if(!str.empty()) {
			if(((*(str.end()-1)) == '/') || ((*(str.end()-1)) == '\\'))
				return false;
		}
		return true;
	}

	/**
		@fn bool is_file(const char* str)
		@brief Test std::string/std::wstring if it represents a file.
		@param str A const char* which holds the string to check.
		@return 'true' if the str represents a file. 'false' if not.
	*/
	inline
	bool is_file(const char* str) {
		if(strlen(str) > 0) {
			char c(str[strlen(str)-sizeof(char)]);
			if((c == '/') ||(c == '\\'))
				return false;
		}
		return true;
	}

	/**
		@fn bool is_file(const char* str)
		@brief Test std::string/std::wstring if it represents a file.
		@param str A const wchar_t* which holds the string to check.
		@return 'true' if the str represents a file. 'false' if not.
	*/
	inline
	bool is_file(const wchar_t* str) {
		if(wcslen(str) > 0) {
			wchar_t c(str[wcslen(str)-sizeof(wchar_t)]);
			if((c == '/') || (c == '\\'))
				return false;
		}
		return true;
	}

/// Return the extension of the filename
	template<typename T> T get_ext(const T& str) {
		typename T::size_type pos(T::npos);
		while((pos = str.find_last_of('.', pos)) != T::npos) {
			// Ok let's check if this is a pointer to the current folder
			// this means something like "./" or ".\"
			if((str.at(pos+1) == '\\') || (str.at(pos+1) == '/')) {
				// Ok yes it is. Return empty string.
				return str.substr(0, 0);
			}
			// No, than this should be a file with an extension.
			return (str.c_str() + pos + 1);
		}
		return str.substr(0, 0);
	}

	/**
		@fn template<typename T> T get_extless(const T& str)
		@brief Returns the file name without the extension.
		@param str A std::string/std::wstring which is a file name.
		@return If no extension is found it will return str back.
	*/
	template<typename T> T get_extless(const T& str) {
		typename T::size_type pos(T::npos);
		// Check if it is the local folder or the parent folder
		if((str == ".") || (str == ".."))
			return str;

		while((pos = str.find_last_of('.', pos)) != T::npos) {
			// Ok let's check if this is a pointer to the current folder
			// this means something like "./" or ".\"
			if((str.at(pos+1) == '\\') || (str.at(pos+1) == '/')) {
				// Ok yes it is. Return empty string.
				return str;
			} else // If we are here this meand the filename has an extension
				return str.substr(0, pos);
		}
		return str;
	}


	/**
		@fn template<typename T> T get_path(const T& str)
		@brief Return the path of a file name.
		@param str A std::string/std::wstring which is a file name.
		@return If no path is found it will the current path id = ".".
		Otherwise it will return the path.
	*/
	template<typename T> T get_path(const T& str) {
		typename T::size_type pos(str.find_last_of('/'));
		if(pos == T::npos) {
			pos = str.find_last_of('\\');
			if(pos == T::npos) // No path information found. Ok, this is the local path '.'.
				return current_path_id;
		}
		return str.substr(0, pos);
	}


	/**
		@fn template<typename T> T get_pathless(const T& str)
		@brief Returns the file name without path.
		@param str A std::string which is a file name.
		@return If no path is found it will return str back.
	*/
	template<typename T> T get_pathless(const T& str) {
		typename T::size_type pos(str.find_last_of('/'));
		if(pos == T::npos) {
			pos = str.find_last_of('\\');
			if(pos == T::npos)
				return str;
		}
		if(pos != T::npos)
			return str.substr(pos + 1, str.size() - pos - 1);
		return str;
	}

	template<typename T>
	void get_filename_info(const T& tmp, T& path, T& filename, T& extension) {

		//
		// Extract extions of the filename.
		//
		size_t ext_pos = tmp.find_last_of(".");
		if(ext_pos != std::string::npos) {
			// Extract the extension of the file.
			extension = tmp.substr(ext_pos + 1, std::string::npos);
		} else {
			// No extension found.
			ext_pos = tmp.size();
		}

		//
		// Extract path of the filename.
		//
		size_t path_pos = tmp.find_last_of("/\\");
		if(path_pos != std::string::npos) {
			// Extract the path of the file.
			path 			= tmp.substr(0, ++path_pos);
		} else {
			// No path found.
			path_pos	= 0;
			// Return "."
			path = current_path_id;
		}

		// Extract the filename.
		size_t number_to_read = ext_pos - path_pos ;
		filename = tmp.substr(path_pos, number_to_read);
	}


} // End of namespace xstd

#endif
