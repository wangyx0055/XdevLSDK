#ifndef XDEVL_OPENGL_UTILS_H
#define XDEVL_OPENGL_UTILS_H

namespace xdl {
		#define BUFFER_OFFSET(i) ((char *)NULL + (i))

    void openGLDebugOutput(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length, const GLchar* message, const void* userParam);
	void shaderLog(xdl_uint shaderId);
	void checkOpenGLError(const char* funcName);
}

#endif
