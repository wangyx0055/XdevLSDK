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
			XdevLOpenGLContextBase(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
				XdevLModuleAutoImpl(parameter, descriptor),
				m_vsnyc(xdl_false),
				m_fsaa(0),
				m_debugMode(xdl_false) {

			}

			virtual ~XdevLOpenGLContextBase() {};

			virtual xdl_int init() {
				if(getMediator() == nullptr) {
					return ERR_OK;
				}

				TiXmlDocument xmlDocument;

				if(!xmlDocument.LoadFile(getMediator()->getXmlFilename())) {
					XDEVL_MODULE_WARNING("Could not parse xml file: " << getMediator()->getXmlFilename() << std::endl);
					return ERR_OK;
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
								} else if(tmp == XdevLString("ES1")) {
									m_attributes.context_profile_mask = XDEVL_OPENGL_CONTEXT_ES1;
								} else if(tmp == XdevLString("ES2")) {
									m_attributes.context_profile_mask = XDEVL_OPENGL_CONTEXT_ES2;
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
