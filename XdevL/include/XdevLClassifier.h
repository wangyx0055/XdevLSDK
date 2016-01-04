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
