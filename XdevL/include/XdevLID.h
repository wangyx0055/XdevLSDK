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
