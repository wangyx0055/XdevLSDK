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

#ifndef XDEVL_RAI_VULKAN_H
#define XDEVL_RAI_VULKAN_H

#include <XdevLRAI/XdevLRAI.h>

#include <vulkan/vulkan.h>

namespace xdl {

	/**
		@class XdevLRAIVulkan
		@brief Interface class for 3D rendering support using Vulkan.
		@author Cengiz Terzibas
	*/
	class XdevLRAIVulkan : public XdevLRAI {
		public:
			XdevLRAIVulkan();
			virtual ~XdevLRAIVulkan();

			 xdl_int create(IPXdevLWindow window) override;
			 XdevLFrameBuffer* getDefaultFrameBuffer() override;
			 XdevLFrameBuffer* get2DFrameBuffer() override;
			 xdl_int setActiveInternalFrameBuffer(xdl_bool state) override;
			 xdl_int setActive2DFrameBuffer(xdl_bool state) override;
			 void setPointSize(xdl_float size) override;
			 void setLineSize(xdl_float size) override;
			 void setActiveDepthTest(xdl_bool enableDepthTest) override;
			 void setActiveBlendMode(xdl_bool enableBlendMode) override;
			 xdl_int setBlendMode(XdevLBlendModes src, XdevLBlendModes dst) override;
			 xdl_int clearColorTargets(xdl_float r, xdl_float g, xdl_float b, xdl_float a) override;
			 xdl_int clearDepthTarget(xdl_float clear_value) override;
			 xdl_int setViewport(xdl_float x, xdl_float y, xdl_float width, xdl_float height) override;
			 xdl_int setActiveRenderWindow(IPXdevLWindow window) override;
			 xdl_int swapBuffers() override;
			 IPXdevLVertexDeclaration createVertexDeclaration() override;
			 IPXdevLVertexShader createVertexShader() override;
			 IPXdevLFragmentShader createFragmentShader() override;
			 IPXdevLGeometryShader createGeometryShader() override;
			 IPXdevLShaderProgram createShaderProgram() override;
			 IPXdevLTexture createTexture() override;
			 IPXdevLTextureCube createTextureCube() override;
			 IPXdevLTexture3D createTexture3D() override;
			 IPXdevLFrameBuffer createFrameBuffer() override;
			 IPXdevLVertexBuffer createVertexBuffer() override;
			 IPXdevLIndexBuffer createIndexBuffer() override;
			 IPXdevLVertexArray createVertexArray() override;
			 xdl_int setActiveFrameBuffer(IPXdevLFrameBuffer frambuffer) override;
			 xdl_int setActiveVertexArray(IPXdevLVertexArray vertexArray) override;
			 xdl_int setActiveShaderProgram(IPXdevLShaderProgram shaderProgram) override;
			 xdl_int drawVertexArray(XdevLPrimitiveType primitiveType, xdl_uint numberOfElements) override;
			 xdl_int drawInstancedVertexArray(XdevLPrimitiveType primitiveType, xdl_uint numberOfElements, xdl_uint number) override;
	};

	XDEVL_EXPORT_MODULE_CREATE_FUNCTION_DECLARATION(XdevLRAI)
}


#endif
