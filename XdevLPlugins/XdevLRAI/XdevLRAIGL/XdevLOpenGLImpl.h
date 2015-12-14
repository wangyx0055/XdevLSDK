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

#ifndef XDEVL_OPENGL_IMPL_H
#define XDEVL_OPENGL_IMPL_H

#include <tinyxml.h>

#include <XdevLPlatform.h>
#include <XdevLListener.h>
#include <XdevLModule.h>
#include <XdevLPluginImpl.h>

#include <XdevLRAI.h>
#include <XdevLOpenGLContext/XdevLOpenGLContext.h>

#include <GL/glew.h>

namespace xdl {

	// Holds the major version number of the plugin.
	const xdl_uint XdevLRAIGLPluginMajorVersion = XDEVLRAIGL_MAJOR_VERSION;
	// Holds the Minor version number of the plugin.
	const xdl_uint XdevLRAIGLPluginMinorVersion = XDEVLRAIGL_MINOR_VERSION;
	// Holds the Patch version number of the plugin.
	const xdl_uint XdevLRAIGLPluginPatchVersion = XDEVLRAIGL_PATCH_VERSION;


	// Holds the Major version number.
	const xdl_uint XdevLRAIGLMajorVersion = XDEVLRAIGL_MODULE_MAJOR_VERSION;
	// Holds the Minor version number.
	const xdl_uint XdevLRAIGLMinorVersion = XDEVLRAIGL_MODULE_MINOR_VERSION;
	// Holds the Patch version number.
	const xdl_uint XdevLRAIGLPatchVersion = XDEVLRAIGL_MODULE_PATCH_VERSION;
	
	static const XdevLString	vendor 				{"www.codeposer.net"
	};
	static const XdevLString	author				{"Cengiz Terzibas"
	};
	static const XdevLString	copyright			{"(c) 2005 - 2012 Cengiz Terzibas."
	};
	static const XdevLString	pluginName 		{"XdevLRAIGL"
	};
	static const XdevLString 	description		{"Creates a OpenGL context for rendering 3D graphics. Depending on \
	your graphics card all supported extensions will be avaiable."
	};

	static const std::vector<XdevLModuleName>	moduleNames	{
		XdevLModuleName("XdevLRAIGL")
	};

	/**
		@class XdevLOpenGLImpl
		@brief Base class for OpenGL support.
		@author Cengiz Terzibas
	*/
	class XdevLOpenGLImpl : public XdevLModuleImpl<XdevLRAI> {
		public:
			XdevLOpenGLImpl(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLOpenGLImpl();


			virtual xdl_int create(XdevLWindow* window) override;
			virtual xdl_int init();
			virtual xdl_int initGLEW();
			virtual const xdl_char* getVersion();
			virtual const xdl_char* getVendor();
			virtual xdl_int getNumExtensions() const;
			virtual const xdl_char* getExtension(xdl_int idx);
			virtual xdl_bool extensionSupported(const xdl_char* extensionName);


			virtual const xdl_char* getShaderVersion();

			virtual void setActiveDepthTest(xdl_bool enableDepthTest) override;
			virtual void setActiveBlendMode(xdl_bool enableBlendMode) override;
			virtual xdl_int setBlendMode(XdevLBlendModes src, XdevLBlendModes dst) override;
			virtual xdl_int clearColorTargets(xdl_float r, xdl_float g, xdl_float b, xdl_float a) override;
			virtual xdl_int clearDepthTarget(xdl_float clear_value) override;
			virtual xdl_int setViewport(xdl_float x, xdl_float y, xdl_float width, xdl_float height) override;
			
			virtual xdl_int setActiveRenderWindow(XdevLWindow* window);
			virtual xdl_int swapBuffers();


			virtual xdl_int createVertexDeclaration(XdevLVertexDeclaration** vertexDeclaration);
			virtual xdl_int createVertexShader(XdevLVertexShader** vertex_shader);
			virtual xdl_int createFragmentShader(XdevLFragmentShader** fragment_shader);
			virtual xdl_int createGeometryShader(XdevLGeometryShader** geometry_shader);
			virtual xdl_int createShaderProgram(XdevLShaderProgram** program);

			virtual xdl_int createTexture(XdevLTexture** texture);
			virtual xdl_int createTextureCube(XdevLTextureCube** textureCube);
			virtual xdl_int createTexture3D(XdevLTexture3D** texture3D);

			virtual xdl_int createFrameBuffer(XdevLFrameBuffer** fbo);


			virtual xdl_int createVertexArray(XdevLVertexArray** vertexArray);

			virtual xdl_int setActiveFrameBuffer(XdevLFrameBuffer* frambuffer);
			virtual xdl_int setActiveVertexArray(XdevLVertexArray* vertexArray);
			virtual xdl_int setActiveShaderProgram(XdevLShaderProgram* shaderProgram);


			virtual xdl_int drawVertexArray(XdevLPrimitiveType primitiveType, xdl_uint numberOfElements);
			virtual xdl_int drawInstancedVertexArray(XdevLPrimitiveType primitiveType, xdl_uint numberOfElements, xdl_uint number);
			virtual xdl_int drawVertexBuffer(XdevLPrimitiveType primitiveType, xdl_uint numberOfElements, XdevLVertexBuffer* vertexBuffer, XdevLVertexDeclaration* vertexDeclaration);
			virtual xdl_int drawVertexBuffer(XdevLPrimitiveType primitiveType, xdl_uint numberOfElements, XdevLVertexBuffer* vertexBuffer, XdevLVertexDeclaration* vertexDeclaration,XdevLIndexBuffer* indexBuffer);

			virtual xdl_int createVertexBuffer(XdevLVertexBuffer** vertexBuffer);
			virtual xdl_int createIndexBuffer(XdevLIndexBuffer** indexBuffer);

			virtual xdl_int destroy(XdevLVertexDeclaration* vertexDeclaration);
			virtual xdl_int destroy(XdevLShaderProgram* shaderProgram);
			virtual xdl_int destroy(XdevLShader* shader);
			virtual xdl_int destroy(XdevLVertexArray* vertexArray);
			virtual xdl_int destroy(XdevLVertexBuffer* vertexBuffer);
			virtual xdl_int destroy(XdevLIndexBuffer* indexBuffer);
			virtual xdl_int destroy(XdevLFrameBuffer* frameBuffer);
			virtual xdl_int destroy(XdevLTexture* texture);
			virtual xdl_int destroy(XdevLTextureCube* textureCube);

			void shaderLog(xdl_uint shaderID);
		protected:
			XdevLWindow* m_window;
			XdevLOpenGLContext* m_gl_context;
			/// Holds the stencil depth value.
			xdl_int m_StencilDepth;
			/// Holds the color depth value.
			xdl_int m_ColorDepth;
			/// Holds the z-depth value.
			xdl_int m_ZDepth;
			/// Holds the vsync state.
			xdl_bool m_VSync;
			/// Holds the FSAA state.
			xdl_int m_fsaa;
			/// Holds the number of extensions supported by the graphics card.
			xdl_int	m_numExtensions;
			/// Holds the supported exstension names.
			std::vector<std::string> m_extensions;
			/// Holds the vendor name of the OpenGL driver.
			std::string m_vendor;
			/// Holds the version of the OpenGL driver.
			std::string m_version;

			/// Holds the context version number.
			xdl_int m_major;
			xdl_int m_minor;
			xdl_bool mXDEVL_DEBUG;
			std::string m_profile;
		protected:
			// Gets OpenGL information from the drivers.
			void getOpenGLInfos();
			/// Reads module information from a XML file.
			xdl_int readModuleInformation(TiXmlDocument* document);
			// Supported vertex shader
			xdl_int m_vertex_shader_version;
			// Supported fragment shader
			xdl_int m_fragment_shader_version;
			// Supported geometry shader
			xdl_int m_geometry_shader_version;

			XdevLFrameBuffer*	m_activeFrameBuffer;
			XdevLVertexArray* 	m_activeVertexArray;
			XdevLShaderProgram*	m_activeShaderProgram;
	};

}

#endif
