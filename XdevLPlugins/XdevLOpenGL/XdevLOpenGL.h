/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2014 Cengiz Terzibas

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

#ifndef XDEVL_OPENGL_H
#define XDEVL_OPENGL_H

#include <XdevLOpenGL/GL/glew.h>
#include <XdevLTypes.h>
#include <XdevLOpenGL/XdevLShader.h>
#include <XdevLOpenGL/XdevLShaderProgram.h>
#include <XdevLOpenGL/XdevLVertexBuffer.h>
#include <XdevLOpenGL/XdevLIndexBuffer.h>
#include <XdevLOpenGL/XdevLTexture.h>
#include <XdevLOpenGL/XdevLTextureCube.h>
#include <XdevLOpenGL/XdevLTexture3D.h>
#include <XdevLOpenGL/XdevLFrameBuffer.h>
#include <XdevLOpenGL/XdevLVertexDeclaration.h>
#include <XdevLOpenGL/XdevLVertexArray.h>
#include <XdevLModule.h>

#include <vector>

namespace xdl {

	class XdevLWindow;

	// Holds the major version number of the plugin.
	const xdl_uint XdevLOpenGLPluginMajorVersion = 1;
	// Holds the Minor version number of the plugin.
	const xdl_uint XdevLOpenGLPluginMinorVersion = 0;
	// Holds the Patch version number of the plugin.
	const xdl_uint XdevLOpenGLPluginPatchVersion = 0;


	// Holds the Major version number.
	const xdl_uint XdevLOpenGLMajorVersion = 1;
	// Holds the Minor version number.
	const xdl_uint XdevLOpenGLMinorVersion = 0;
	// Holds the Patch version number.
	const xdl_uint XdevLOpenGLPatchVersion = 0;



	enum XdevLPrimitiveType {
	  XDEVL_PRIMITIVE_POINTS					= 0x0000,
	  XDEVL_PRIMITIVE_LINES						= 0x0001,
	  XDEVL_PRIMITIVE_LINE_LOOP				= 0x0002,
	  XDEVL_PRIMITIVE_LINE_STRIP			= 0x0003,
	  XDEVL_PRIMITIVE_TRIANGLES				= 0x0004,
	  XDEVL_PRIMITIVE_TRIANGLE_STRIP	= 0x0005,
	  XDEVL_PRIMITIVE_TRIANGLE_FAN		= 0x0006,
	  XDEVL_PRIMITIVE_QUADS						= 0x0007,
	  XDEVL_PRIMITIVE_QUAD_STRIP			= 0x0008,
	  XDEVL_PRIMITIVE_POLYGON					= 0x0009
	};

	/**
		@class XdevLOpenGL
		@brief Core class for OpenGL support.
		@author Cengiz Terzibas


		This module supports raw OpenGL programming. First it creates an OpenGL context and after that
		depending on your graphics card it will initialize the supported extensions so that you can use
		them right away. To get more information about the OpenGL drivers you can use the following methods:

		- getVendor()
		- getVersion()

		To check which extensions are supported from your graphics card you can use the following methods:

		- getNumExtensions()
		- getExtension()
		- extensionSupported()

		@note Don't include OpenGL header files from your own project. This is done in the XdevLOpenGL.h
		file already.

		@section xdevl_xdevlopengl_xml_create Creating the module

		- plugin_name = @b "XdevLOpenGL"
		- module_name = @b "XdevLOpenGL"
		@section xdevl_xdevlopengl_usage Usage
		Before you start using any of the methods you must use the attach() method and attach this module
		to a already created window. This means of course that you can't render on the window you want :).
		@section xdevl_xdevlopengl_xml_format Xml Format.

		@subsection xdevl_xdevlopengl_xml_format_element "<XdevLOpenGL>" element
			- Attributes:
				- id
					- desc				: Identification code of the module
					- value type	: string
				- framebuffer_depth
					- desc				: Specifies the color depth of the framebuffer
					- value type  : int
					- value range : 16, 24 or 32
					- default     : 32
				- z_buffer_depth
					- desc        : Specifies the z-buffer depth.
					- value type  : int
					- value range : 8, 16 or 24
					- default     : 16
				- stencil_buffer_depth
					- desc        : Specifies the stencil-buffer depth.
					- value type  : int
					- value range : 8, 16
					- default     : 8
			-	Elements:
				- none
			- Textfield
				- none
	*/

	class XdevLOpenGL : public XdevLModule {

		public:
			virtual ~XdevLOpenGL() {};

			/// Create the OpenGL context.
			/**
				You must attach this module to a XdevLWindowDevice before you call any other
				method from this module.
				@param window A valid pointer to an XdevLWindowDevice.
				@return
				- @b ERR_OK if the attachement was successful.
				- @b ERR_ERROR if the attachement wasn't successful.
			*/
			virtual xdl_int createContext(XdevLWindow* window) = 0;
			/// Exchange front and back buffers.
			/**
				Exchanges front framebuffer and the back framebuffer. Only used in double buffered mode.

				@return
				- @b ERR_OK if it was successful.
				- @b ERR_ERROR if it wasn't successful.
			*/
			virtual xdl_int swapBuffers() = 0;
			/// Returns the OpenGL version.
			/**
				@return A null terminated string which represents the version of the OpenGL driver.
			*/
			virtual const xdl_char* getVersion() = 0;

			/// Return the supported Shader version.
			/**
				@return A null terminated string which represents the version of the supported OpenGL Shader version.
			*/
      virtual const xdl_char* getShaderVersion() = 0;

			/// Returns the Vendor name of the OpenGL driver.
			/**
				@return A null terminated string which represents the name of the vendor.
			*/
			virtual const xdl_char* getVendor() = 0;
			/// Returns the number of supported extensions.
			/**
				@return Returns the number of extensions supported by your graphics card.
			*/
			virtual xdl_int getNumExtensions() const = 0;
			/// Returns a supported extension name.
			/**
				@return A null terminated string which represents a extension name depending
				on the idx. Valid values for idx [0, getNumExtensions()[.
			*/
			virtual const xdl_char* getExtension(xdl_int idx) = 0;
			/// Checks if a specific extension is supported.
			/**
				@param extensionName Should be a null terminated string which is the extension name.
				@return
					- @b true if the extension is supported.
					- @b false if the extension isn't supported.
			*/
			virtual xdl_bool extensionSupported(const xdl_char* extensionName) = 0;

      /// Enable/Disable vertical sync.
			virtual xdl_int setVSync(xdl_bool state) = 0;

			/// Enable/Disable Fullscreen Anti Aliasing.
			virtual xdl_int setEnableFSAA(xdl_bool state) = 0;
			
			/// Sets the context to the specified window.
			virtual xdl_int makeCurrent(XdevLWindow* sindow) = 0;
			
			/// Release current context and use previous one.
			virtual xdl_int releaseCurrent() = 0;
	
	};

	class XdevLOpenGL330 : public XdevLOpenGL {
		public:
			virtual ~XdevLOpenGL330() {}
			//
			// Creation methods.
			//

			/// Create a Vertex Shader.
			virtual xdl_int createVertexShader(XdevLVertexShader** vertex_shader) = 0;

			/// Create a Fragment shader.
			virtual xdl_int createFragmentShader(XdevLFragmentShader** fragment_shader) = 0;

			/// Create a Geometry shader.
			virtual xdl_int createGeometryShader(XdevLGeometryShader** geometry_shader) = 0;

			/// Create a Shader Program.
			virtual xdl_int createShaderProgram(XdevLShaderProgram** program) = 0;

			/// Create a Texture.
			virtual xdl_int createTexture(XdevLTexture** texture) = 0;

			/// Create a Cube Texture.
			virtual xdl_int createTextureCube(XdevLTextureCube** textureCube) = 0;

			/// Create a 3D Texture.
			virtual xdl_int createTexture3D(XdevLTexture3D** texture3D) = 0;

			/// Create a Frame Buffer.
			virtual xdl_int createFrameBuffer(XdevLFrameBuffer** fbo) = 0;

			/// Create a Vertex Buffer.
			virtual xdl_int createVertexBuffer(XdevLVertexBuffer** vertexBuffer) = 0;

			/// Create a Index Buffer.
			virtual xdl_int createIndexBuffer(XdevLIndexBuffer** indexBuffer) = 0;

			/// Create a Vertex Array.
			virtual xdl_int createVertexArray(XdevLVertexArray** vertexArray) = 0;


			//
			// Activating methods.
			//
			
			/// Sets the active framebuffer object.
			virtual xdl_int setActiveFrameBuffer(XdevLFrameBuffer* frambuffer) = 0;
			
			/// Sets the active Vertex Array Object.
			virtual xdl_int setActiveVertexArray(XdevLVertexArray* vertexArray) = 0;

			/// Sets the active Shader Program.
			virtual xdl_int setActiveShaderProgram(XdevLShaderProgram* shaderProgram) = 0;

			//
			// Drawing methods.
			//

			/// Draw a Vertex Array.
			virtual xdl_int drawVertexArray(XdevLPrimitiveType primitiveType,
																			xdl_uint numberOfElements) = 0;

			/// Draw a Instanced Vertex Array.
			virtual xdl_int drawInstancedVertexArray(	XdevLPrimitiveType primitiveType,
																								xdl_uint numberOfElements,
																								xdl_uint number) = 0;

			/// Draw Vertex Buffer.
			virtual xdl_int drawVertexBuffer(	XdevLPrimitiveType primitiveType,
																				xdl_uint numberOfElements,
																				XdevLVertexBuffer* vertexBuffer,
																				XdevLVertexDeclaration* vertexDeclaration) = 0;

			/// Draw indexed Vertex Buffer.
			virtual xdl_int drawVertexBuffer(	XdevLPrimitiveType primitiveType,
																				xdl_uint numberOfElements,
																				XdevLVertexBuffer* vertexBuffer,
																				XdevLVertexDeclaration* vertexDeclaration,
																				XdevLIndexBuffer* indexBuffer) = 0;

			//
			// Destroying methods.
			//

			/// Destroy a Shader Program.
			virtual xdl_int destroy(XdevLShaderProgram* shaderProgram) = 0;

			/// Destroy a Shader.
			virtual xdl_int destroy(XdevLShader* shader) = 0;

			/// Destroy a Vertex Array.
			virtual xdl_int destroy(XdevLVertexArray* vertexArray) = 0;

			/// Destroy a Vertex Buffer.
			virtual xdl_int destroy(XdevLVertexBuffer* vertexBuffer) = 0;

			/// Destroy a Index Buffer.
			virtual xdl_int destroy(XdevLIndexBuffer* indexBuffer) = 0;

			/// Destroy a Frame Buffer.
			virtual xdl_int destroy(XdevLFrameBuffer* frameBuffer) = 0;

			/// Destroy a Texture.
			virtual xdl_int destroy(XdevLTexture* texture) = 0;

			/// Destroy a Cube Texture.
			virtual xdl_int destroy(XdevLTextureCube* textureCube) = 0;

	};


	/**
		@class XdevLOpenGLContextScope
		@brief Scope class for the OpenGL interface.
		
		Can be used to make the context current and release
		automatically.
		
	*/
	class XdevLOpenGLContextScope {
		public:
			XdevLOpenGLContextScope(XdevLOpenGL* opengl, XdevLWindow* window) : m_openglContext(opengl) { m_openglContext->makeCurrent(window); }
			virtual ~XdevLOpenGLContextScope() { m_openglContext->releaseCurrent();}
		private:
			XdevLOpenGL* m_openglContext;
	};


	typedef XdevLOpenGL	    IXdevLOpenGL;
	typedef XdevLOpenGL*    IPXdevLOpenGL;
	typedef XdevLOpenGL330	IXdevLOpenGL330;
	typedef XdevLOpenGL330* IPXdevLOpenGL330;
}


#endif
