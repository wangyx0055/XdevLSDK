/*
	XdevL eXtended DEVice Library. 
	
	Copyright © 2005-2011 Cengiz Terzibas

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

#ifndef XDEVL_CUDA_H
#define XDEVL_CUDA_H

#include <XdevLModule.h>

namespace xdl {

// Holds the major version number of the plugin.
const xdl_int XdevLCudaPluginMajorVersion = 0;
// Holds the Minor version number of the plugin.
const xdl_int XdevLCudaPluginMinorVersion = 1;
// Holds the Patch version number of the plugin.
const xdl_int XdevLCudaPluginPatchVersion = 0;


// Holds the Major version number.
const xdl_int XdevLCudaMajorVersion = 0;
// Holds the Minor version number.
const xdl_int XdevLCudaMinorVersion = 1;
// Holds the Patch version number.
const xdl_int XdevLCudaPatchVersion = 0;

/**
	@class XdevLCuda
	@brief Core class for CUDA support.
	@author Cengiz Terzibas
*/

class XdevLCuda : public XdevLModule {
public:
	virtual ~XdevLCuda() {};
	/// Matrix multiplication.
	virtual xdl_int multMatrix(xdl_int n, xdl_int m, xdl_int8* buffer) = 0;
};
	
}
#endif
