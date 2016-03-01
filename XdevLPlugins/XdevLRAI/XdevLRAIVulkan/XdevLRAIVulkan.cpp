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

namespace xdl {
	XdevLRAIVulkan::XdevLRAIVulkan() {

	}

	XdevLRAIVulkan::~XdevLRAIVulkan() {

	}

	xdl_int XdevLRAIVulkan::create(IPXdevLWindow window) {
	}

	XdevLFrameBuffer* XdevLRAIVulkan::getDefaultFrameBuffer() {
	}

	XdevLFrameBuffer* XdevLRAIVulkan::get2DFrameBuffer() {
	}

	xdl_int XdevLRAIVulkan::setActiveInternalFrameBuffer(xdl_bool state) {
	}

	xdl_int XdevLRAIVulkan::setActive2DFrameBuffer(xdl_bool state) {
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
	}

	xdl_int XdevLRAIVulkan::clearColorTargets(xdl_float r, xdl_float g, xdl_float b, xdl_float a) {
	}

	xdl_int XdevLRAIVulkan::clearDepthTarget(xdl_float clear_value) {
	}

	xdl_int XdevLRAIVulkan::setViewport(xdl_float x, xdl_float y, xdl_float width, xdl_float height) {
	}

	xdl_int XdevLRAIVulkan::setActiveRenderWindow(IPXdevLWindow window) {
	}

	xdl_int XdevLRAIVulkan::swapBuffers() {
	}

	IPXdevLVertexDeclaration XdevLRAIVulkan::createVertexDeclaration() {
	}

	IPXdevLVertexShader XdevLRAIVulkan::createVertexShader() {
	}

	IPXdevLFragmentShader XdevLRAIVulkan::createFragmentShader() {
	}

	IPXdevLGeometryShader XdevLRAIVulkan::createGeometryShader() {
	}

	IPXdevLShaderProgram XdevLRAIVulkan::createShaderProgram() {
	}

	IPXdevLTexture XdevLRAIVulkan::createTexture() {
	}

	IPXdevLTextureCube XdevLRAIVulkan::createTextureCube() {
	}

	IPXdevLTexture3D XdevLRAIVulkan::createTexture3D() {
	}

	IPXdevLFrameBuffer XdevLRAIVulkan::createFrameBuffer() {
	}

	IPXdevLVertexBuffer XdevLRAIVulkan::createVertexBuffer() {
	}

	IPXdevLIndexBuffer XdevLRAIVulkan::createIndexBuffer() {
	}

	IPXdevLVertexArray XdevLRAIVulkan::createVertexArray() {
	}

	xdl_int XdevLRAIVulkan::setActiveFrameBuffer(IPXdevLFrameBuffer frambuffer) {
	}

	xdl_int XdevLRAIVulkan::setActiveVertexArray(IPXdevLVertexArray vertexArray) {
	}

	xdl_int XdevLRAIVulkan::setActiveShaderProgram(IPXdevLShaderProgram shaderProgram) {
	}

	xdl_int XdevLRAIVulkan::drawVertexArray(XdevLPrimitiveType primitiveType, xdl_uint numberOfElements) {
	}

	xdl_int XdevLRAIVulkan::drawInstancedVertexArray(XdevLPrimitiveType primitiveType, xdl_uint numberOfElements, xdl_uint number) {
	}

}
