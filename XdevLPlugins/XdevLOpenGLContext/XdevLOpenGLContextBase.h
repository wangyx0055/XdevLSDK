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

#ifndef XDEVL_OPENGL_CONTEXT_BASE_H
#define XDEVL_OPENGL_CONTEXT_BASE_H

#include <XdevLOpenGLContext/XdevLOpenGLContext.h>

namespace xdl {


	/**
		@class XdevLOpenGLContextBase
		@brief Support for handling OpenGL context for the GLX platform.
		@author Cengiz Terzibas
	*/
	class XdevLOpenGLContextBase : public XdevLModuleAutoImpl<XdevLOpenGLContext> {

		public:
			XdevLOpenGLContextBase(XdevLModuleCreateParameter* parameter, XdevLModuleDescriptor& descriptor) :
				XdevLModuleAutoImpl(parameter, descriptor),
				m_vsnyc(xdl_false),
				m_fsaa(0),
				m_debugMode(xdl_false) {

			}

			virtual ~XdevLOpenGLContextBase() {};

			virtual xdl_int init() {
				TiXmlDocument xmlDocument;

				if(!xmlDocument.LoadFile(getMediator()->getXmlFilename())) {
					XDEVL_MODULE_ERROR("Could not parse xml file: " << getMediator()->getXmlFilename() << std::endl);
					return ERR_ERROR;
				}

				return readModuleInformation(&xmlDocument);
			}

			xdl_int readModuleInformation(TiXmlDocument* document) {
				TiXmlHandle docHandle(document);
				TiXmlElement* root = docHandle.FirstChild(XdevLCorePropertiesName.c_str()).FirstChildElement("XdevLOpenGLContext").ToElement();

				if(nullptr == root) {
					XDEVL_MODULE_INFO("<XdevLOpenGLContext> section not found, skipping proccess.\n");
					return ERR_OK;
				}

				while(root != nullptr) {
					if(root->Attribute("id")) {
						XdevLID id(root->Attribute("id"));
						if(getID() == id) {
							if(root->Attribute("framebuffer_depth")) {
								m_attributes.color_buffer_size = xstd::from_string<xdl_int>(root->Attribute("framebuffer_depth"));
								XDEVL_MODULE_INFO("Framebuffer size request      : " << m_attributes.color_buffer_size << std::endl);
							}
							if(root->Attribute("z_buffer_depth")) {
								m_attributes.depth_size = xstd::from_string<xdl_int>(root->Attribute("z_buffer_depth"));
								XDEVL_MODULE_INFO("Depth Buffer size request     : " << m_attributes.depth_size << std::endl);
							}
							if(root->Attribute("stencil_buffer_depth")) {
								m_attributes.stencil_size = xstd::from_string<xdl_int>(root->Attribute("stencil_buffer_depth"));
								XDEVL_MODULE_INFO("Stencilbuffer depth request   : " << m_attributes.stencil_size << std::endl);
							}
							if(root->Attribute("vsync")) {
								m_vsnyc = xstd::from_string<xdl_bool>(root->Attribute("vsync"));
								XDEVL_MODULE_INFO("Vertical Sync                 : " << (m_vsnyc ? "On" : "Off") << std::endl);
							}
							if(root->Attribute("fsaa")) {
								m_fsaa = xstd::from_string<xdl_int>(root->Attribute("fsaa"));
								XDEVL_MODULE_INFO("FSSA                          : " << m_fsaa << std::endl);
							}
							if(root->Attribute("core_profile")) {
								XdevLString tmp = XdevLString(root->Attribute("core_profile"));
								if(tmp == XdevLString("core_profile")) {
									m_attributes.context_profile_mask = XDEVL_OPENGL_CONTEXT_CORE_PROFILE;
									XDEVL_MODULE_INFO("core_profile                  : " << tmp << "Core Profile" << std::endl);
								} else if(tmp == XdevLString("compatibility")) {
									m_attributes.context_profile_mask = XDEVL_OPENGL_CONTEXT_COMPATIBILITY;
									XDEVL_MODULE_INFO("Core profile                  : " << "Compatibility" << std::endl);
								} else if(tmp == XdevLString("ES")) {
									m_attributes.context_profile_mask = XDEVL_OPENGL_CONTEXT_ES;
								}
							}
							if(root->Attribute("major")) {
								m_attributes.context_major_version = xstd::from_string<xdl_int>(root->Attribute("major"));
								XDEVL_MODULE_INFO("Core Profile Major            : " << m_attributes.context_major_version << std::endl);
							}
							if(root->Attribute("minor")) {
								m_attributes.context_minor_version = xstd::from_string<xdl_int>(root->Attribute("minor"));
								XDEVL_MODULE_INFO("Core Profile Minor            : " << m_attributes.context_minor_version << std::endl);
							}
							if(root->Attribute("debug")) {
								m_debugMode = xstd::from_string<xdl_bool>(root->Attribute("debug"));
								XDEVL_MODULE_INFO("Debug Mode                    : " << (m_debugMode ? "On" : "Off") << std::endl);
							}
						}
					} else {
						XDEVL_MODULE_ERROR("No 'id' attribute specified. Using default values for the device\n");
					}
					root = root->NextSiblingElement();
				}
				return ERR_OK;
			}
			xdl_bool getVSync() {
				return m_vsnyc;
			}
			xdl_bool getDebugMode() {
				return m_debugMode;
			}
		protected:
			XdevLOpenGLContextAttributes m_attributes;
			xdl_bool m_vsnyc;
			xdl_int m_fsaa;
			xdl_bool m_debugMode;
	};


	typedef XdevLOpenGLContext	IXdevLOpenGLContext;
	typedef XdevLOpenGLContext*	IPXdevLOpenGLContext;
}


#endif
