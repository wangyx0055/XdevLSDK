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

#ifndef XDEVL_CLASSIFIER_H
#define XDEVL_CLASSIFIER_H

namespace xdl {

	class XdevLID;
	
	/**
		@class XdevLClassifier
		@brief Identification of class instances.
		@author Cengiz Terzibas
		
		@todo At the moment the ID uses a 64 bit unsigned integer. A string is used to create a hash
		code. The algorithm is at the moment Adler32 which is not good but for the beta version of
		XdevL it will do it's job.
	*/
	class XdevLClassifier {
		public:
			virtual ~XdevLClassifier() {}
			/// Returns the identification code.
			/**
				Used to identify modules and other objects.

				@return Identification code. 
			*/
			virtual const XdevLID& getID() const = 0;
	};

}
#endif
