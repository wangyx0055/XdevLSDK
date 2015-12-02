#include "GL/glew.h"
#include "XdevLUtils.h"

namespace xdl {

  void openGLDebugOutput(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cout << message << std::endl;
  }

	void checkOpenGLError(const char* funcName) {
		GLenum error = glGetError();
		if(GL_NO_ERROR != error) {
			switch(error) {
				case GL_INVALID_ENUM: {
					std::cerr << funcName << ":: Error: " << "GL_INVALID_ENUM" <<std::endl;
				}
				break;
				case GL_INVALID_VALUE: {
					std::cerr << funcName << ":: Error: " << "GL_INVALID_VALUE" <<std::endl;
				}
				break;
				case GL_INVALID_OPERATION: {
					std::cerr << funcName << ":: Error: " << "GL_INVALID_OPERATION" <<std::endl;
				}
				break;
				case GL_INVALID_FRAMEBUFFER_OPERATION: {
					std::cerr << funcName << ":: Error: " << "GL_INVALID_FRAMEBUFFER_OPERATION" <<std::endl;
				}
				break;
				case GL_OUT_OF_MEMORY: {
					std::cerr << funcName << ":: Error: " << "GL_OUT_OF_MEMORY" <<std::endl;
				}
				break;
				case GL_STACK_UNDERFLOW: {
					std::cerr << funcName << ":: Error: " << "GL_STACK_UNDERFLOW" <<std::endl;
				}
				break;
				case GL_STACK_OVERFLOW: {
					std::cerr << funcName << ":: Error: " << "GL_STACK_OVERFLOW" <<std::endl;
				}
				break;
			}
		}

	}

	void shaderLog(xdl_uint shaderId) {
		// Get the length of the message.
		GLint Len;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &Len);

		GLsizei ActualLen;
		GLchar *Log = new GLchar[Len];
		glGetShaderInfoLog(shaderId, Len, &ActualLen, Log);
		std::cout << "Shader ID: " << shaderId << ", Message: " << Log << "\n";
		delete [] Log;
	}
}