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

#ifndef XDEVL_OGRE_IMPL_H
#define XDEVL_OGRE_IMPL_H

#include <vector>
#include <XdevLPlatform.h>
#include <XdevLCoreMediator.h>
#include <XdevLPluginImpl.h>
#include <XdevLUtils.h>
#include "XdevLOgre.h"
#include <tinyxml.h>

namespace xdl {

	class XdevLOgreImpl;
	class XdevLOgreSpritesImpl;


	// Holds the major version number of the plugin.
	const xdl_uint XdevLOgrePluginMajorVersion = XDEVLOGRE_MAJOR_VERSION;

	// Holds the Minor version number of the plugin.
	const xdl_uint XdevLOgrePluginMinorVersion = XDEVLOGRE_MINOR_VERSION;

	// Holds the Patch version number of the plugin.
	const xdl_uint XdevLOgrePluginPatchVersion = XDEVLOGRE_PATCH_VERSION;



	// Holds the Major version number.
	const xdl_uint XdevLOgreMajorVersion = XDEVLOGRE_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLOgreMinorVersion = XDEVLOGRE_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint XdevLOgrePatchVersion = XDEVLOGRE_MODULE_PATCH_VERSION;


	static const XdevLString vendor {
		"www.codeposer.net"
	};
	static const XdevLString author {
		"Cengiz Terzibas"
	};
	static const XdevLString copyright {
		"(c) 2005 - 2012 Cengiz Terzibas"
	};
	static const XdevLString pluginName {
		"XdevLOgre"
	};
	static const XdevLString description {
		"Integrates Ogre into XdevL"
	};
	static const std::vector<XdevLModuleName> moduleNames {
		XdevLModuleName("XdevLOgre")
	};

	/**
		@class XdevLOgreSprites
		@brief Class wich supports sprites in OGRE
		@author Cengiz Terzibas


	*/
	class XdevLOgreSprites : public XdevLModule {
		public:
			virtual ~XdevLOgreSprites() {}
			virtual xdl_int init(XdevLOgre* ogre, const xdl_char* sceneManagerName) = 0;
			virtual xdl_int addSprite(const xdl_char* materialName, xdl_float x, xdl_float y,  xdl_float width, xdl_float height) = 0;
	};

	/**
		@class XdevLOgreImpl
		@brief Core class for OGRE support
		@author Cengiz Terzibas
	*/
	class XdevLOgreImpl : public XdevLModuleImpl<XdevLOgre> {
		public:
			XdevLOgreImpl(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLOgreImpl();
			static XdevLModuleDescriptor m_moduleDescriptor;
			virtual xdl_int init();
			virtual xdl_int shutdown();

			virtual void* getInternal(const xdl_char* id);
			virtual xdl_int update();
			virtual Ogre::Root* getRoot();
			virtual Ogre::RenderWindow* getWindow();
			virtual xdl_int attach(XdevLWindow* window);
		protected:
			xdl_bool m_attached;
			// Holds the core system;
			XdevLWindow* m_xdlWindow;
			// Holds the color depth
			xdl_int m_ColorDepth;
			// Holds the antialiasing value
			xdl_int m_Antialiasing;
			// Holds the vsync value.
			xdl_bool m_VSync;
			// Holds the color depth value.
			xdl_int	m_colorDepth;
			// Holds if gamma correction should be enabled.
			xdl_bool	m_gamma;
			// Holds the NVPerfHUD flag.
			xdl_bool m_useNVPerfHUD;
			// Holds the display frequence.
			xdl_int m_displayFrequency;
			// Holds the OGRE root object
			Ogre::Root* m_root;
			// Holds the OGRE window object
			Ogre::RenderWindow*	m_window;
			// Holds ogre plugin names.
			std::vector<std::string> m_ogrePluginNames;

		protected:
			xdl_int readOgreInfo(TiXmlDocument* document);
			/// Reads all resource information from xml file
			xdl_int readResourceInfo(TiXmlDocument* xmlDocument);
	};

// -----------------------------------------------------------------------------
// Module 2
//
	/*
	class XdevLOgreSpriteModuleDescriptor : public XdevLModuleDescriptor {
	public:
	virtual ~XdevLOgreSpriteModuleDescriptor() {}
		// Should return the vendor name.
		virtual const xdl_char* getVendor() const  {
			static xdl_char vendor[] 	= "www.codeposer.net";
			return vendor;
		}
		// Should return the author name.
		virtual const xdl_char* getAuthor() const {
			static xdl_char author[] 	= "Cengiz Terzibas";
			return author;
		}
		// Should return the module name.
		virtual const xdl_char* getName() const 			{
			static xdl_char name[] 		= "XdevLOgreSprite";
			return name;
		}
		// Should return the copyright.
		virtual const xdl_char* getCopyright() const {
			static xdl_char copyright[] = "(c) 2005-2008 Cengiz Terzibas.";
			return copyright;
		}
		// Should return a short description.
		virtual const xdl_char* getDescription() const {
			static xdl_char description[] = "Integrates Sprites for 2D gaming development.";
			return description;
		}
		// Should return the version number.
		virtual void getVersion(xdl_int* major, xdl_int* minor, xdl_int* patch){
			*major = 1;
			*minor = 0;
			*patch = 0;
		}
	};


	class XdevLOgreSpritesImpl : public XdevLModuleImpl<XdevLOgreSprites>, public Ogre::RenderQueueListener {
	public:
		XdevLOgreSpritesImpl(XdevLModuleCreateParameter* parameter);
		virtual ~XdevLOgreSpritesImpl();
		static XdevLOgreSpriteModuleDescriptor m_ogreSpriteModuleDesc;
		virtual XdevLModuleDescriptor* getDescriptor() const;

		struct sprite{
			sprite(xdl_float x, xdl_float y, xdl_float width, xdl_float height) : m_x(x), m_y(y), m_width(width), m_height(height){}
			xdl_float m_x, m_y;
			xdl_float m_width, m_height;
		};


		virtual xdl_int init(XdevLOgre* ogre, const char* sceneManagerName);
		virtual xdl_int addSprite(const char* materialName, xdl_float x, xdl_float y,  xdl_float width, xdl_float height);

	protected:
		Ogre::Root* 					m_root;
		Ogre::SceneManager* 	m_sceneManager;
		std::vector<sprite*> 	m_sprites;

		// This is the render operation
		Ogre::RenderOperation m_renderOp;
		// This is the vertex declaration for the vertex buffer
		Ogre::VertexDeclaration* m_vd;
		// This is the vertex buffer.
		Ogre::HardwareVertexBufferSharedPtr m_vb;

		void renderBuffer();
		/// Called by Ogre, for being a render queue listener
		virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String &invocation, bool &skipThisInvocation);
		/// Called by Ogre, for being a render queue listener
		virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String &invocation, bool &repeatThisInvocation);

	};
	*/
}

#endif
