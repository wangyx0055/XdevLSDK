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
