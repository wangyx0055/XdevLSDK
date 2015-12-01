/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2008 Cengiz Terzibas

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

#ifndef XDEVL_BLANK_MODULEL_IMPL_H
#define XDEVL_BLANK_MODULEL_IMPL_H

#include <XdevLModule.h>

namespace xdl {


	/**
		@class XdevLBlankModule
	*/
	class XdevLBlankModule : public XdevLModule {
		public:
			virtual ~XdevLBlankModule() {};
	};



	typedef XdevLBlankModule	IXdevLBlankModule;
	typedef XdevLBlankModule*	IPXdevLBlankModule;

}

#endif
