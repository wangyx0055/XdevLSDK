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

#ifndef XDEVL_TYPES_H
#define XDEVL_TYPES_H

#include <XdevLPlatform.h>
#include <string>
#include <memory>

#ifdef __GNUC__
#include <stdint.h>
#endif

namespace xdl {

#ifdef _MSC_VER

	typedef __int8 						xdl_int8;
	typedef __int16 					xdl_int16;
	typedef __int32 					xdl_int32;
	typedef __int64 					xdl_int64;
	typedef unsigned __int8 	xdl_uint8;
	typedef unsigned __int16 	xdl_uint16;
	typedef unsigned __int32 	xdl_uint32;
	typedef unsigned __int64 	xdl_uint64;

// C99 related

	typedef __int8 						int8_t;
	typedef __int16 					int16_t;
	typedef __int32 					int32_t;
	typedef __int64 					int64_t;
	typedef unsigned __int8 	uint8_t;
	typedef unsigned __int16 	uint16_t;
	typedef unsigned __int32 	uint32_t;
	typedef unsigned __int64 	uint64_t;

	typedef int8_t    int_least8_t;
	typedef int16_t   int_least16_t;
	typedef int32_t   int_least32_t;
	typedef int64_t   int_least64_t;
	typedef uint8_t   uint_least8_t;
	typedef uint16_t  uint_least16_t;
	typedef uint32_t  uint_least32_t;
	typedef uint64_t  uint_least64_t;

	typedef int8_t    int_fast8_t;
	typedef int16_t   int_fast16_t;
	typedef int32_t   int_fast32_t;
	typedef int64_t   int_fast64_t;
	typedef uint8_t   uint_fast8_t;
	typedef uint16_t  uint_fast16_t;
	typedef uint32_t  uint_fast32_t;
	typedef uint64_t  uint_fast64_t;

#endif
#ifdef __GNUC__
	typedef int8_t 				xdl_int8;
	typedef int16_t 			xdl_int16;
	typedef int32_t				xdl_int32;
	typedef int64_t				xdl_int64;
	typedef uint8_t 			xdl_uint8;
	typedef uint16_t 			xdl_uint16;
	typedef uint32_t			xdl_uint32;
	typedef uint64_t			xdl_uint64;
#endif

	typedef bool			xdl_bool;
	typedef unsigned char 	xdl_ubyte;
	typedef char			xdl_byte;
	typedef unsigned char	xdl_uchar;
	typedef char			xdl_char;
	typedef xdl_uint32		xdl_uint;
	typedef xdl_int32		xdl_int;
	typedef xdl_int64		xdl_long;
	typedef xdl_uint64		xdl_ulong;
	typedef float			xdl_float;
	typedef double			xdl_double;

	const bool xdl_true		= 1;
	const bool xdl_false	= 0;
	const bool xdl_yes		= 1;
	const bool xdl_no		= 0;

	/**
	@struct XdevSize
	*/
	struct XdevLSize {
		typedef xdl_int type;

		XdevLSize() : width(0), height(0) {}
		XdevLSize(type w, type h) : width(w), height(h) {}

		type width;
		type height;
	};

	/**
		@struct XdevLPosition
		@brief A structure that holds 2d position data.
	*/
	struct XdevLPosition {
		typedef xdl_int type;

		XdevLPosition() : x(0), y(0) {}
		XdevLPosition(type _x, type _y) : x(_x), y(_y) {}

		/// Returns the x coordinate of the position.
		const type& getX() const {
			return x;
		}

		/// Returns the y coordinate of the position.
		const type& getY() const {
			return y;
		}

		/// Sets the x coordinate of the position.
		void setX(const type& value) {
			x = value;
		}

		/// Sets the y coordinate of the position.
		void setY(const type& value) {
			y = value;
		}

		/// X Coordinate of the position.
		type x;

		/// Y Coordinate of the position.
		type y;
	};

	/**
		@struct XdevLName
		@brief A string structure that holds a string.

	*/
	class XdevLString {
		public:
			typedef std::string type;
			
			XdevLString() : m_value("Not Specified") {}

			explicit XdevLString(const char* t) : m_value(t) {}

			explicit XdevLString(const std::string& t) : m_value(t) {}

			const std::string& toString() const {
				return this->m_value;
			}

			size_t characterSize() const {
				return 1; // TODO At the moment we use xdl_char.
			}

			void clear() {
				this->m_value.clear();
			}

			size_t length() const {
				return this->m_value.length();
			}

			size_t size() {
				return this->m_value.size();
			}

			friend std::ostream& operator << (std::ostream& os, const XdevLString& name) {
				os << name.m_value;
				return os;
			}

			friend XdevLString operator + (const XdevLString& str1, const XdevLString& str2) {
				XdevLString tmp(str1);
				tmp += str2;
				return std::move(tmp);
			}

			XdevLString& operator = (const xdl_char* str) {
				m_value = type(str);
				return *this;
			}

			XdevLString& operator += (const xdl_char str) {
				m_value += str;
				return *this;
			}

			XdevLString& operator += (const XdevLString& str) {
				m_value += str;
				return *this;
			}

			friend bool operator == (const XdevLString& s1, const XdevLString& s2) {
				return (s1.m_value == s2.m_value);
			}

			friend bool operator != (const XdevLString& s1, const XdevLString& s2) {
				return !(s1.m_value == s2.m_value);
			}

			friend bool operator < (const XdevLString& s1, const XdevLString& s2) {
				return (s1.m_value < s2.m_value);
			}

			operator const char* () const {
				return m_value.c_str();
			}

			size_t find(const XdevLString& str) const {
				return m_value.find(str.toString());
			}

			const char* begin() const {
				return m_value.data();
			}

			const char* end() const {
				return &(m_value.data())[m_value.length()];
			}

			void trim() {
				const type& whitespace = " \t";
				const auto strBegin = m_value.find_first_not_of(whitespace);

				if(strBegin == std::string::npos) {
					return ; 	// No content
				}

				const auto strEnd = m_value.find_last_not_of(whitespace);
				const auto strRange = strEnd - strBegin + 1;

				m_value = m_value.substr(strBegin, strRange);
			}

			type m_value;
	};

	/**
		@class XdevLPluginName
		@brief A class that holds the name of a plugin.

	*/
	class XdevLPluginName : public XdevLString {
		public:
			XdevLPluginName() : XdevLString() {}

			explicit XdevLPluginName(const char* t) : XdevLString(t) {}

			explicit XdevLPluginName(const std::string& t) : XdevLString(t) {}

			XdevLPluginName& operator = (XdevLString::type t) {
				m_value = t;
				return *this;
			}

	};

	/**
		@class XdevLModuleName
		@brief A class that holds the name of a module.
	*/
	class XdevLModuleName : public XdevLString {
		public:
			XdevLModuleName() : XdevLString() {}

			explicit XdevLModuleName(const char* t) : XdevLString(t) {}

			explicit XdevLModuleName(const std::string& t) : XdevLString(t) {}

			XdevLModuleName& operator = (XdevLString::type t) {
				m_value = t;
				return *this;
			}
	};

	/**
		@class XdevLInternalName
		@brief A class that holds the name of an internal object within a module.
	*/
	class XdevLInternalName : public XdevLString {
		public:
			XdevLInternalName() : XdevLString() {}

			explicit XdevLInternalName(const char* t) : XdevLString(t) {}

			explicit XdevLInternalName(const std::string& t) : XdevLString(t) {}

			XdevLInternalName& operator = (XdevLString::type t) {
				m_value = t;
				return *this;
			}

	};

	/**
		@class XdevLFileName
		@brief A class that holds the name of a file.

		Supports basic filename manipulation methods.
	*/
	class XdevLFileName : public XdevLString {
		public:
			XdevLFileName() : XdevLString() {}

			explicit XdevLFileName(const char* t) : XdevLString(t) {}

			explicit XdevLFileName(const std::string& t) : XdevLString(t) {}

			XdevLFileName& operator = (XdevLString::type t) {
				m_value = t;
				return *this;
			}

			/// Returns the extension of the filename.
			/**
				If the filename does not have any extension a empty XdevLString will
				be returned.
			*/
			XdevLString getExtension() {

				//
				// Extract extions of the filename.
				//
				size_t ext_pos = m_value.find_last_of(".");
				if(ext_pos != std::string::npos) {
					// Extract the extension of the file.
					return XdevLString(m_value.substr(ext_pos + 1, std::string::npos));
				}
				return XdevLString();
			}
			
			/// Removes the file extension if it has one.
			XdevLFileName& removeExtension() {
				size_t pos = m_value.find_last_of(".");
				if(pos == XdevLString::type::npos) {
					return *this;
				}
				m_value = m_value.substr(0, pos);
				return *this;
			}

			/// Returns the path of the filename.
			/**
				If the filename does not have a file, it will return the current path
				'.'.
			*/
			XdevLString getPath() {

				//
				// Extract path of the filename.
				//
				size_t path_pos = m_value.find_last_of("/\\");
				if(path_pos != XdevLString::type::npos) {
					// Extract the path of the file.
					return XdevLString(m_value.substr(0, ++path_pos));
				}

				return XdevLString(".");
			}
			
			/// Removes the path from the filename
			XdevLFileName& removePath() {
				size_t pos = m_value.find_last_of("/\\");
				if(pos == XdevLString::type::npos) {
					return *this;
				}
				
				m_value = m_value.substr(pos + 1, m_value.size());
				return *this;
			}

			/// Returns the filename without path and extension.
			/**
				This function will return a string that represents the filename
				without path and exntension information.
			*/
			XdevLString getFilename() {

				//
				// Extract extions of the filename.
				//
				size_t ext_pos = m_value.find_last_of(".");
				if(ext_pos == std::string::npos) {
					// No extension found.
					ext_pos = m_value.size();
				}

				//
				// Extract path of the filename.
				//
				size_t path_pos = m_value.find_last_of("/\\");
				if(path_pos == std::string::npos) {
					// No path found.
					path_pos	= 0;
				}

				// Extract the filename.
				size_t number_to_read = ext_pos - path_pos ;

				return XdevLString(m_value.substr(path_pos, number_to_read));
			}
	};

#define STRING(MSG) xdl::XdevLString(MSG)
}

#endif
