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

#ifndef XDEVL_RAI_H
#define XDEVL_RAI_H

#include <XdevLTypes.h>
#include <XdevLRAI/XdevLShader.h>
#include <XdevLRAI/XdevLShaderProgram.h>
#include <XdevLRAI/XdevLVertexBuffer.h>
#include <XdevLRAI/XdevLIndexBuffer.h>
#include <XdevLRAI/XdevLTexture.h>
#include <XdevLRAI/XdevLTextureCube.h>
#include <XdevLRAI/XdevLTexture3D.h>
#include <XdevLRAI/XdevLFrameBuffer.h>
#include <XdevLRAI/XdevLVertexDeclaration.h>
#include <XdevLRAI/XdevLVertexArray.h>
#include <XdevLModule.h>

#include <vector>

namespace xdl {

	class XdevLWindow;

	enum XdevLPrimitiveType {
	    XDEVL_PRIMITIVE_POINTS			= 0x0000,
	    XDEVL_PRIMITIVE_LINES			= 0x0001,
	    XDEVL_PRIMITIVE_LINE_LOOP		= 0x0002,
	    XDEVL_PRIMITIVE_LINE_STRIP		= 0x0003,
	    XDEVL_PRIMITIVE_TRIANGLES		= 0x0004,
	    XDEVL_PRIMITIVE_TRIANGLE_STRIP	= 0x0005,
	    XDEVL_PRIMITIVE_TRIANGLE_FAN	= 0x0006,
	    XDEVL_PRIMITIVE_QUADS			= 0x0007,
	    XDEVL_PRIMITIVE_QUAD_STRIP		= 0x0008,
	    XDEVL_PRIMITIVE_POLYGON			= 0x0009
	};

	enum XdevLBlendModes {
	    XDEVL_BLEND_NONE,
	    XDEVL_BLEND_ZERO,
	    XDEVL_BLEND_ONE,
	    XDEVL_BLEND_SRC_COLOR,
	    XDEVL_BLEND_ONE_MINUS_SRC_COLOR,
	    XDEVL_BLEND_DST_COLOR,
	    XDEVL_BLEND_ONE_MINUS_DST_COLOR,
	    XDEVL_BLEND_SRC_ALPHA,
	    XDEVL_BLEND_ONE_MINUS_SRC_ALPHA,
	    XDEVL_BLEND_DST_ALPHA,
	    XDEVL_BLEND_ONE_MINUS_DST_ALPHA,
	    XDEVL_BLEND_CONSTANT_COLOR,
	    XDEVL_BLEND_ONE_MINUS_CONSTANT_COLOR,
	    XDEVL_BLEND_CONSTANT_ALPHA,
	    XDEVL_BLEND_ONE_MINUS_CONSTANT_ALPHA,
	    XDEVL_BLEND_SRC_ALPHA_SATURATE
	};

	/**
		@class XdevLOpenGL
		@brief Interface class for 3D rendering support.
		@author Cengiz Terzibas

	*/

	class XdevLRAI : public XdevLModule {
		public:
			virtual ~XdevLRAI() {}

			//
			// Create the RAI
			//
			virtual xdl_int create(XdevLWindow* window) = 0;

			//
			// Change Point and Line size.
			// 

			virtual void setPointSize(xdl_float size) = 0;
			virtual void setLineSize(xdl_float size) = 0;

			//
			// Depth test
			//
			virtual void setActiveDepthTest(xdl_bool enableDepthTest) = 0;

			//
			// Blend modes
			//
			virtual void setActiveBlendMode(xdl_bool enableBlendMode) = 0;
			virtual xdl_int setBlendMode(XdevLBlendModes src, XdevLBlendModes dst) = 0;

			//
			// Temp. framebuffer stuff
			//
			/// Clears the Color Targets.
			virtual xdl_int clearColorTargets(xdl_float r, xdl_float g, xdl_float b, xdl_float a) = 0;

			/// Clears the Depth Target.
			virtual xdl_int clearDepthTarget(xdl_float clear_value) = 0;

			/// Set the viewport.
			/**
				@param x Specify the left side of the viewport rectangle, in pixels. The initial value is 0.
				@param y Specify the lower side of the viewport rectangle, in pixels. The initial value is 0.
				@param width Specify the width of the viewport rectangle, in pixels. The initial value is the size of the window after creating the context.
				@param height Specify the height of the viewport rectangle, in pixels. The initial value is the size of the window after creating the context.
			*/
			virtual xdl_int setViewport(xdl_float x, xdl_float y, xdl_float width, xdl_float height) = 0;


			/// Change the rendering window.
			virtual xdl_int setActiveRenderWindow(XdevLWindow* window) = 0;

			/// Swaps buffers.
			virtual xdl_int swapBuffers() = 0;

			//
			// Creation methods.
			//

			/// Create a Vertex Declaration
			virtual xdl_int createVertexDeclaration(IPXdevLVertexDeclaration* vertexDeclaration) = 0;

			/// Create a Vertex Shader.
			virtual xdl_int createVertexShader(IPXdevLVertexShader* vertex_shader) = 0;

			/// Create a Fragment shader.
			virtual xdl_int createFragmentShader(IPXdevLFragmentShader* fragment_shader) = 0;

			/// Create a Geometry shader.
			virtual xdl_int createGeometryShader(IPXdevLGeometryShader* geometry_shader) = 0;

			/// Create a Shader Program.
			virtual xdl_int createShaderProgram(IPXdevLShaderProgram* program) = 0;

			/// Create a Texture.
			virtual xdl_int createTexture(IPXdevLTexture* texture) = 0;

			/// Create a Cube Texture.
			virtual xdl_int createTextureCube(IPXdevLTextureCube* textureCube) = 0;

			/// Create a 3D Texture.
			virtual xdl_int createTexture3D(IPXdevLTexture3D* texture3D) = 0;

			/// Create a Frame Buffer.
			virtual xdl_int createFrameBuffer(IPXdevLFrameBuffer* fbo) = 0;

			/// Create a Vertex Buffer.
			virtual xdl_int createVertexBuffer(IPXdevLVertexBuffer* vertexBuffer) = 0;

			/// Create a Index Buffer.
			virtual xdl_int createIndexBuffer(IPXdevLIndexBuffer* indexBuffer) = 0;

			/// Create a Vertex Array.
			virtual xdl_int createVertexArray(IPXdevLVertexArray* vertexArray) = 0;


			//
			// Activating methods.
			//

			/// Sets the active framebuffer object.
			virtual xdl_int setActiveFrameBuffer(IPXdevLFrameBuffer frambuffer) = 0;

			/// Sets the active Vertex Array Object.
			virtual xdl_int setActiveVertexArray(IPXdevLVertexArray vertexArray) = 0;

			/// Sets the active Shader Program.
			virtual xdl_int setActiveShaderProgram(IPXdevLShaderProgram shaderProgram) = 0;

			//
			// Drawing methods.
			//

			/// Draw a Vertex Array.
			virtual xdl_int drawVertexArray(XdevLPrimitiveType primitiveType,
			                                xdl_uint numberOfElements) = 0;

			/// Draw a Instanced Vertex Array.
			virtual xdl_int drawInstancedVertexArray(XdevLPrimitiveType primitiveType,
			        xdl_uint numberOfElements,
			        xdl_uint number) = 0;

			/// Draw Vertex Buffer.
			virtual xdl_int drawVertexBuffer(XdevLPrimitiveType primitiveType,
			                                 xdl_uint numberOfElements,
			                                 XdevLVertexBuffer* vertexBuffer,
			                                 XdevLVertexDeclaration* vertexDeclaration) = 0;

			/// Draw indexed Vertex Buffer.
			virtual xdl_int drawVertexBuffer(XdevLPrimitiveType primitiveType,
			                                 xdl_uint numberOfElements,
			                                 XdevLVertexBuffer* vertexBuffer,
			                                 XdevLVertexDeclaration* vertexDeclaration,
			                                 XdevLIndexBuffer* indexBuffer) = 0;

			//
			// Destroying methods.
			//
			/// Destroy a Vertex Declaration.
			virtual xdl_int destroy(XdevLVertexDeclaration* vertexDeclaration) = 0;

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

	typedef XdevLRAI	IXdevLRAI;
	typedef XdevLRAI*	IPXdevLRAI;
}


#endif
