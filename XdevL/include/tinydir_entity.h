/*
	TinyDir.

	Copyright © 2005-2012 Cengiz Terzibas

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

#ifndef DIR_ENTITY
#define DIR_ENTITY

#include <string>

namespace tdir {

	/**
		@class dire_entity
		@brief Holds one element information in a directory.
	*/
	class dir_entity {
	public:
		dir_entity();
		dir_entity(const char* filename);
		friend class directory;
		/// Returns the name of the entity.
		const char* get_name();
	protected:
		void set_name(const char* filename);
		void set_length(long int length);
	protected:
		/// Holds the entity name found in the directory.
		std::string m_filename;

		long int m_length;
	};

} // End of namespace

#endif
