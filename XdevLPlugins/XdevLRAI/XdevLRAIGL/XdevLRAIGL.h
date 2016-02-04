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

	static const std::vector<XdevLModuleName>	moduleNames	{
		XdevLModuleName("XdevLRAIGL")
	};

	/**
		@class XdevLRAIGL
		@brief OpenGL implementation of XdevLRAI.
		@author Cengiz Terzibas
	*/
	class XdevLRAIGL : public XdevLModuleImpl<XdevLRAI> {
		public:
			XdevLRAIGL(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor);
			virtual ~XdevLRAIGL();


			virtual xdl_int create(IPXdevLWindow window) override;
			virtual xdl_int init();

			virtual const xdl_char* getVersion();
			virtual const xdl_char* getVendor();
			virtual xdl_int getNumExtensions() const;
			virtual const xdl_char* getExtension(xdl_int idx);
			virtual xdl_bool extensionSupported(const xdl_char* extensionName);
			virtual const xdl_char* getShaderVersion();


			virtual XdevLFrameBuffer* getDefaultFrameBuffer() override;
			virtual XdevLFrameBuffer* get2DFrameBuffer() override;

			virtual xdl_int setActiveInternalFrameBuffer(xdl_bool state) override;
			virtual xdl_int setActive2DFrameBuffer(xdl_bool state) override;
			virtual xdl_int setActiveRenderWindow(XdevLWindow* window) override;
			virtual void setActiveDepthTest(xdl_bool enableDepthTest) override;
			virtual void setActiveBlendMode(xdl_bool enableBlendMode) override;

			virtual void setPointSize(xdl_float size) override;
			virtual void setLineSize(xdl_float size) override;
			virtual xdl_int setBlendMode(XdevLBlendModes src, XdevLBlendModes dst) override;
			virtual xdl_int clearColorTargets(xdl_float r, xdl_float g, xdl_float b, xdl_float a) override;
			virtual xdl_int clearDepthTarget(xdl_float clear_value) override;
			virtual xdl_int setViewport(xdl_float x, xdl_float y, xdl_float width, xdl_float height) override;


			virtual xdl_int swapBuffers();

			virtual IPXdevLVertexDeclaration createVertexDeclaration() override;
			virtual IPXdevLVertexShader createVertexShader() override;
			virtual IPXdevLFragmentShader createFragmentShader() override;
			virtual IPXdevLGeometryShader createGeometryShader() override;
			virtual IPXdevLShaderProgram createShaderProgram() override;
			virtual IPXdevLTexture createTexture() override;
			virtual IPXdevLTextureCube createTextureCube() override;
			virtual IPXdevLTexture3D createTexture3D() override;
			virtual IPXdevLFrameBuffer createFrameBuffer() override;
			virtual IPXdevLVertexBuffer createVertexBuffer() override;
			virtual IPXdevLIndexBuffer createIndexBuffer() override;
			virtual IPXdevLVertexArray createVertexArray() override;


			virtual xdl_int setActiveFrameBuffer(IPXdevLFrameBuffer frambuffer);
			virtual xdl_int setActiveVertexArray(IPXdevLVertexArray vertexArray);
			virtual xdl_int setActiveShaderProgram(IPXdevLShaderProgram shaderProgram);

			virtual xdl_int drawVertexArray(XdevLPrimitiveType primitiveType, xdl_uint numberOfElements);
			virtual xdl_int drawInstancedVertexArray(XdevLPrimitiveType primitiveType, xdl_uint numberOfElements, xdl_uint number);

			virtual xdl_int initGLEW();
			void shaderLog(xdl_uint shaderID);
			xdl_int initExtensions();
			xdl_int createScreenVertexArray();
			xdl_int renderFrameBufferPlane();
			xdl::xdl_int m_frameBufferTextureShaderId;
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
			xdl_bool m_useInternalBuffer;
			IPXdevLFrameBuffer m_defaultFrameBuffer;
			IPXdevLFrameBuffer	m_activeFrameBuffer;
			IPXdevLVertexArray 	m_activeVertexArray;
			IPXdevLShaderProgram	m_activeShaderProgram;
			
			xdl_bool m_use2DFrameBuffer;
			IPXdevLFrameBuffer m_2DFrameBuffer;
			IPXdevLVertexDeclaration m_vertexDeclaration;
			IPXdevLVertexArray m_frameBufferArray;
			IPXdevLShaderProgram m_frameBufferShaderProgram;
			IPXdevLVertexShader m_frameBufferVertexShader;
			IPXdevLFragmentShader m_frameBufferFragmentShader;
	};

}

#endif
