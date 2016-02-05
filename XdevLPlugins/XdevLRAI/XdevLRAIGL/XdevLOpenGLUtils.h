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

#ifndef XDEVL_OPENGL_UTILS_H
#define XDEVL_OPENGL_UTILS_H

#include <XdevLUtils.h>

namespace xdl {

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define glExitAndPrintError(ERROR) {std::cout << __func__ << ":" << __LINE__ << ": " << glGetErrorAsString(ERROR) << std::endl; exit(0);}

#define glCheckError() {\
		GLuint ret = glGetError();\
		if(ret != GL_NO_ERROR) {\
			glExitAndPrintError(ret);\
		}\
	}

	void openGLDebugOutput(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length, const GLchar* message, const void* userParam);
	const char* glGetErrorAsString(GLint error);
	void shaderLog(xdl_uint shaderId);
	void checkOpenGLError(const char* funcName);
	void glExitOnVertexArrayBound(const xdl_char* message);

#ifdef XDEVL_RAIGL_CHECK_VAO_BOUND
	#define GL_CHECK_VAO_BOUND(MESSAGE) glExitOnVertexArrayBound(MESSAGE)
#else
	#define GL_CHECK_VAO_BOUND(MESSAGE)
#endif
	
}

#endif
