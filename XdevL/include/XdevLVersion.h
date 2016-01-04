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

#ifndef XDEVL_VERSION_H
#define XDEVL_VERSION_H

#include <XdevLError.h>
#include <XdevLTypes.h>
#include <XdevLXstring.h>
#include <iostream>
#include <sstream>

namespace xdl {

	/**
		@class XdevLVersion
		@brief Holds version numbers.
	*/
	class XdevLVersion {
		public:

			XdevLVersion() : m_major(0), m_minor(0), m_patch(0) {}

			XdevLVersion(xdl_uint16 major, xdl_uint16 minor, xdl_uint16 patch) : m_major(major), m_minor(minor), m_patch(patch) {}

			XdevLVersion(const XdevLString& version) {
				fromString(version);
			}

			virtual ~XdevLVersion() {}

			static XdevLVersion zero;

			/// Sets the version numbers.
			void setVersion(xdl_uint16 major, xdl_uint16 minor, xdl_uint16 patch) {
				m_major = major;
				m_minor = minor;
				m_patch = patch;
			}

			/// Returns the major number of the version.
			const xdl_uint16& geMajor() const 	{
				return m_major;
			}

			/// Returns the minor number of the version.
			const xdl_uint16& getMinor() const 	{
				return m_minor;
			}

			/// Returns the patch number of the version.
			const xdl_uint16& getPatch() const 	{
				return m_patch;
			}

			/// Creates the version object from string.
			/**
				The format of the string must be "major.minor.patch". First the major
				value of the version followed by a dot. Then the minor value of the
				version followed by a dot and finally the patch version of the version.
			*/
			xdl_int fromString(const XdevLString& version) {
				std::vector<std::string> values;
				std::string tmp = version.toString();
				xstd::tokenize(tmp, values, ".");

				//
				// Check if the number of version number is correct.
				//
				if(values.size() < 3) {
					return ERR_ERROR;
				}
				std::stringstream ssmajor, ssminor, sspatch;
				ssmajor << values[0];
				ssmajor >> m_major;
				
				ssminor << values[1];
				ssminor >> m_minor;
				
				sspatch << values[2];
				sspatch >> m_patch;
				
				return ERR_OK;
			}

			const std::string toString() const {
				std::stringstream ss;
				ss << m_major << "." << m_minor << "." << m_patch;
				return ss.str();
			}

			// Stream operations
			friend std::ostream& operator << (std::ostream& out, const XdevLVersion& v)	{
				out << "Version [" << v.m_major << "." << v.m_minor << "." << v.m_patch << "]";
				return out;
			}

			friend bool operator == ( const XdevLVersion& v1, const XdevLVersion& v2 ) {
				return (v1.m_major == v2.m_major) && (v1.m_minor == v2.m_minor) && (v1.m_patch == v2.m_patch);
			}

			friend bool operator != ( const XdevLVersion& v1, const XdevLVersion& v2 ) {
				return !(v1 == v2);
			}

		private:
			xdl_uint16 m_major;
			xdl_uint16 m_minor;
			xdl_uint16 m_patch;
	};
}

#endif
