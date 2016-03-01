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



#include "XdevLRAIVulkan.h"
#include "XdevLVertexBufferVulkan.h"

namespace xdl {
	XdevLRAIVulkan::XdevLRAIVulkan() :
		m_activeFrameBuffer(nullptr),
		m_activeVertexArray(nullptr),
		m_activeShaderProgram(nullptr),
		m_defaultFrameBuffer(nullptr),
		m_default2DFrameBuffer(nullptr) {

	}

	XdevLRAIVulkan::~XdevLRAIVulkan() {

	}

	xdl_int XdevLRAIVulkan::create(IPXdevLWindow window) {
        return ERR_ERROR;
	}

	XdevLFrameBuffer* XdevLRAIVulkan::getDefaultFrameBuffer() {
		return m_defaultFrameBuffer;
	}

	XdevLFrameBuffer* XdevLRAIVulkan::get2DFrameBuffer() {
		return m_default2DFrameBuffer;
	}

	xdl_int XdevLRAIVulkan::setActiveInternalFrameBuffer(xdl_bool state) {
		return ERR_ERROR;
	}

	xdl_int XdevLRAIVulkan::setActive2DFrameBuffer(xdl_bool state) {
		return ERR_ERROR;
	}

	void XdevLRAIVulkan::setPointSize(xdl_float size) {
	}

	void XdevLRAIVulkan::setLineSize(xdl_float size) {
	}

	void XdevLRAIVulkan::setActiveDepthTest(xdl_bool enableDepthTest) {
	}

	void XdevLRAIVulkan::setActiveBlendMode(xdl_bool enableBlendMode) {
	}

	xdl_int XdevLRAIVulkan::setBlendMode(XdevLBlendModes src, XdevLBlendModes dst) {
		return ERR_ERROR;
	}

	xdl_int XdevLRAIVulkan::clearColorTargets(xdl_float r, xdl_float g, xdl_float b, xdl_float a) {
		return ERR_ERROR;
	}

	xdl_int XdevLRAIVulkan::clearDepthTarget(xdl_float clear_value) {
		return ERR_ERROR;
	}

	xdl_int XdevLRAIVulkan::setViewport(xdl_float x, xdl_float y, xdl_float width, xdl_float height) {
		return ERR_ERROR;
	}

	xdl_int XdevLRAIVulkan::setActiveRenderWindow(IPXdevLWindow window) {
		m_activeWindow = window;
		return ERR_OK;
	}

	xdl_int XdevLRAIVulkan::swapBuffers() {
		return ERR_ERROR;
	}

	IPXdevLVertexDeclaration XdevLRAIVulkan::createVertexDeclaration() {
		return std::make_shared<XdevLVertexDeclaration>();
	}

	IPXdevLVertexShader XdevLRAIVulkan::createVertexShader() {
		return nullptr;
	}

	IPXdevLFragmentShader XdevLRAIVulkan::createFragmentShader() {
		return nullptr;
	}

	IPXdevLGeometryShader XdevLRAIVulkan::createGeometryShader() {
		return nullptr;
	}

	IPXdevLShaderProgram XdevLRAIVulkan::createShaderProgram() {
		return nullptr;
	}

	IPXdevLTexture XdevLRAIVulkan::createTexture() {
		return nullptr;
	}

	IPXdevLTextureCube XdevLRAIVulkan::createTextureCube() {
		return nullptr;
	}

	IPXdevLTexture3D XdevLRAIVulkan::createTexture3D() {
		return nullptr;
	}

	IPXdevLFrameBuffer XdevLRAIVulkan::createFrameBuffer() {
		return nullptr;
	}

	IPXdevLVertexBuffer XdevLRAIVulkan::createVertexBuffer() {
		return std::make_shared<XdevLVertexBufferVulkan>();
	}

	IPXdevLIndexBuffer XdevLRAIVulkan::createIndexBuffer() {
		return nullptr;
	}

	IPXdevLVertexArray XdevLRAIVulkan::createVertexArray() {
		return nullptr;
	}

	xdl_int XdevLRAIVulkan::setActiveFrameBuffer(IPXdevLFrameBuffer framebuffer) {
		m_activeFrameBuffer = framebuffer;
		return ERR_OK;
	}

	xdl_int XdevLRAIVulkan::setActiveVertexArray(IPXdevLVertexArray vertexArray) {
		m_activeVertexArray = vertexArray;
		return ERR_OK;
	}

	xdl_int XdevLRAIVulkan::setActiveShaderProgram(IPXdevLShaderProgram shaderProgram) {
		m_activeShaderProgram = shaderProgram;
		return ERR_OK;
	}

	xdl_int XdevLRAIVulkan::drawVertexArray(XdevLPrimitiveType primitiveType, xdl_uint numberOfElements) {
		return ERR_ERROR;
	}

	xdl_int XdevLRAIVulkan::drawInstancedVertexArray(XdevLPrimitiveType primitiveType, xdl_uint numberOfElements, xdl_uint number) {
		return ERR_ERROR;
	}

}
