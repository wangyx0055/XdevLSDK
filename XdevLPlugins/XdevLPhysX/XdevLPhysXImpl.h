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

#ifndef XDEVL_PHYSX_IMPL_H
#define XDEVL_PHYSX_IMPL_H

#include "XdevLPhysX.h"
#include <XdevLPluginImpl.h>

using namespace xdl;

namespace phsx {

class XdevLPhysXPluginDescriptor : public XdevLPluginDescriptor {

public:
	virtual ~XdevLPhysXPluginDescriptor() {}

	virtual const xdl_char* getName() const 	{
		static xdl_char name[] 		= "XdevLPhysX";
		return name;
	}
	virtual xdl_int getNumModules() const {return 1;}		
	virtual const xdl_char* getModuleName(const xdl_int idx) const {
		static xdl_char moduleName[] = "XdevLPhysX";
		return moduleName;
	}

	virtual void getVersion(xdl_int* major, xdl_int* minor, xdl_int* patch) {
		*major = 1;
		*minor = 0;
		*patch = 0;
	}
};

class XdevLPhysXModuleDescriptor : public XdevLModuleDescriptor {

public:
	virtual ~XdevLPhysXModuleDescriptor() {}

	virtual const xdl_char* getVendor() const {
		static xdl_char vendor[] 	= "www.codeposer.net";
		return vendor;
	}

	virtual const xdl_char* getAuthor() const {
		static xdl_char author[] 	= "Cengiz Terzibas";
		return author;
	}

	virtual const xdl_char* getName() const 	{
		static xdl_char name[] 		= "XdevLPhysX";
		return name;
	}

	virtual const xdl_char* getCopyright() const {
		static xdl_char copyright[] = "(c) 2005-2008 Cengiz Terzibas.";
		return copyright;
	}

	virtual const xdl_char* getDescription() const {
		static xdl_char description[] = "Nivida PhysX module";
		return description;
	}
	// Should return the version number.
	virtual void getVersion(xdl_int* major, xdl_int* minor, xdl_int* patch){
		*major = 1;
		*minor = 0;
		*patch = 0;
	}
};


/**
	@class XdevLPhysXImpl
	@brief Implementation of the XdevLPhysX class.
	@author Cengiz Terzibas
*/

class XdevLPhysXImpl : public XdevLModuleImpl<XdevLPhysX> {

public:
	XdevLPhysXImpl(XdevLModuleCreateParameter* parameter);
	virtual ~XdevLPhysXImpl() {};
	static XdevLPhysXModuleDescriptor m_physXModuleDesc;
	virtual XdevLModuleDescriptor* getDescriptor() const;	
	virtual xdl_int init();
	virtual xdl_int shutdown();
	virtual NxPhysicsSDK* getPhysXSDK();
protected:
	NxPhysicsSDK*	m_physicsSDK;
};

}


#endif
