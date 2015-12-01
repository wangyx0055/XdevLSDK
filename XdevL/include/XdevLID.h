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

#ifndef XDEVL_ID_H
#define XDEVL_ID_H

#include <XdevLUtils.h>
#include <iostream>

namespace xdl {

	/**
		@class XdevLID
		@brief Identification object for all XdevL objects.
		
		The main purpose of this class is to provide a faster method to work 
		identifying string types. The string is converted into a integer which 
		can be processed faster in terms of comparing.
		
		// TODO At the moment using adler32 to do the id. This must be changed.
	*/
	class XdevLID {
		public:

			XdevLID() : m_name("XdevLUnknown") {
				m_hashCode = adler32(m_name.toString().c_str(), m_name.size());
			}

			explicit XdevLID(const xdl_char* name) : m_name(name) {
				m_hashCode = adler32(m_name.toString().c_str(), m_name.size());
			}

			/// Returns the string name of the ID.
			const XdevLString& getName() const {
				return m_name;
			}

			/// Returns the hash code of the ID.
			const xdl_uint64& getHashCode() const {
				return m_hashCode;
			}

			/// Assign operator.
			const XdevLID& operator=(const XdevLID& a) {
				this->m_hashCode = a.m_hashCode;
				this->m_name = a.m_name;
				return *this;
			}

			/// Equal operator.
			friend bool operator==(const XdevLID& a, const XdevLID& b) {
				return (a.m_hashCode == b.m_hashCode);
			}

			/// Unequal operator.
			friend bool operator!=(const XdevLID& a, const XdevLID& b) {
				return !(a.m_hashCode == b.m_hashCode);
			}

			/// Stream operations
			inline friend std::ostream& operator<<(std::ostream& out, const XdevLID& id)	{
				out << "[ID: " << id.m_name << ", HASH: " << id.m_hashCode << "]";
				return out;
			}

		private:

			// Holds the literal identification.
			XdevLString				m_name;

			// Holds the hash code for the name.
			xdl_uint64 				m_hashCode;

	};

}

#endif
