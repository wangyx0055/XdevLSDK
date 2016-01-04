/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2016 Cengiz Terzibas

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

#ifndef XDEVL_LISTENER_H
#define XDEVL_LISTENER_H

#include <XdevLClassifier.h>
#include <XdevLError.h>
#include <XdevLEvent.h>

namespace xdl {

	/**
		@class XdevLListener
		@brief Makes the derived class a listener object.

		Listener objects have the ability to listen to events that are send by modules.
		If you think your module should catch some events that are necessary you should
		derive your class from this interface.
	*/
	class XdevLListener : public XdevLClassifier {
		public:
			virtual ~XdevLListener() {}
			/// Callback function for events.
			/**
				This is the function which will be called by the system if a specific event
				occures for that listener. It is also possible to m_sender event directly to the
				modules.

				@return Returns ERR_OK if everything was successful or ERR_ERROR if it fails.
			*/
			virtual xdl_int notify(XdevLEvent& event) {
				return ERR_OK;
			}

	};

}

#endif
