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

#ifndef XDEVL_CUDA_IMPL_H
#define XDEVL_CUDA_IMPL_H

#include <cuda.h>
#include <cuda_runtime.h>
#include "XdevLCuda.h"
#include <XdevLPluginImpl.h>

namespace xdl {

static const xdl_char*	vendor 					= {"www.codeposer.net"};
static const xdl_char*	author					= {"Cengiz Terzibas"};
static const xdl_char*	copyright				= {"(c) 2005 - 2012 Cengiz Terzibas."};
static const xdl_char*	moduleNames[]		= {"XdevLCuda"};
static const xdl_char*	pluginName 			= {"XdevLCuda"};

class XdevLCudaPluginDescriptor : public XdevLPluginDescriptor {

public:
	virtual ~XdevLCudaPluginDescriptor() {}

	virtual const xdl_char* getName() const 	{
		return pluginName;
	}
	virtual xdl_int getNumModules() const {
		return sizeof(moduleNames);
	}		
	virtual const xdl_char* getModuleName(const xdl_int idx) const {
		return moduleNames[idx];
	}
	virtual void getVersion(xdl_int* major, xdl_int* minor, xdl_int* patch) {
		*major = XdevLCudaPluginMajorVersion;
		*minor = XdevLCudaPluginMinorVersion;
		*patch = XdevLCudaPluginPatchVersion;
	}
};

class XdevLCudaModuleDescriptor : public XdevLModuleDescriptor {

public:
	virtual ~XdevLCudaModuleDescriptor() {}

	virtual const xdl_char* getVendor() const {
		return vendor;
	}

	virtual const xdl_char* getAuthor() const {
		return author;
	}

	virtual const xdl_char* getName() const 	{
		return moduleNames[0];
	}

	virtual const xdl_char* getCopyright() const {
		return copyright;
	}

	virtual const xdl_char* getDescription() const {
		static xdl_char description[] = "Creates a Cuda context for rendering 3D graphics. Depending on \
your graphics card all supported extensions will be avaiable.";
		return description;
	}
	// Should return the version number.
	virtual void getVersion(xdl_int* major, xdl_int* minor, xdl_int* patch){
		*major = XdevLCudaMajorVersion;
		*minor = XdevLCudaMinorVersion;
		*patch = XdevLCudaPatchVersion;
	}
};	
	
	
/**
	@class XdevLCudaImpl
	@brief Core class for CUDA support.
	@author Cengiz Terzibas
*/

class XdevLCudaImpl : public XdevLModuleImpl<XdevLCuda> {
public:
	static XdevLCudaModuleDescriptor m_pluginDesc;	

	virtual ~XdevLCudaImpl() {};
	XdevLCudaImpl(XdevLModuleCreateParameter* parameter);
	void* getInternal(const xdl_char* id);
	
	virtual xdl_int init();
	virtual xdl_int shutdown();
	virtual XdevLModuleDescriptor* getDescriptor() const;	
	virtual xdl_int readModuleInformation(TiXmlDocument* document);
	
	/// Matrix multiplication.
	virtual xdl_int multMatrix(xdl_int n, xdl_int m, xdl_int8* buffer);\
	
protected:
	xdl_int 				m_devID;
	cudaDeviceProp 	m_prop;	
};
	
}

#endif