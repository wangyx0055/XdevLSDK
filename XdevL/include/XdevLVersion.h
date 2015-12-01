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
