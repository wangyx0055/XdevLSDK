#ifndef XDEVL_SHADER_PROGRAM_H
#define XDEVL_SHADER_PROGRAM_H

#include <XdevLOpenGL/XdevLShader.h>

namespace xdl {

	/**
		@class XdevLOpenGLProgram
		@brief A OpenGL Shader Program.

		This Shader Programs holds the OpenGL Shader Program.
	*/
	class XdevLShaderProgram {
	public:
		virtual ~XdevLShaderProgram() {}

		virtual xdl_int attach(XdevLShader* shader) = 0;
		virtual xdl_int link() = 0;

		virtual xdl_int getUniformLocation(const char* id) = 0;
		virtual xdl_int getAttribLocation(const char* id) = 0;



		virtual void activate() = 0;
		virtual void deactivate() = 0;

		virtual void setUniformMatrix4(xdl_uint id, xdl_uint count, const xdl_float* v1) = 0;
		virtual void setUniformMatrix3(xdl_uint id, xdl_uint count, const xdl_float* v1) = 0;
		virtual void setUniformMatrix2(xdl_uint id, xdl_uint count, const xdl_float* v1) = 0;

		virtual void setUniformi(xdl_uint id, xdl_int v1) = 0;
		virtual void setUniformi(xdl_uint id, xdl_int v1, xdl_int v2) = 0;
		virtual void setUniformi(xdl_uint id, xdl_int v1, xdl_int v2, xdl_int v3) = 0;
		virtual void setUniformi(xdl_uint id, xdl_int v1, xdl_int v2, xdl_int v3, xdl_int v4) = 0;

		virtual void setUniformui(xdl_uint id, xdl_uint v1) = 0;
		virtual void setUniformui(xdl_uint id, xdl_uint v1, xdl_uint v2) = 0;
		virtual void setUniformui(xdl_uint id, xdl_uint v1, xdl_uint v2, xdl_uint v3) = 0;
		virtual void setUniformui(xdl_uint id, xdl_uint v1, xdl_uint v2, xdl_uint v3, xdl_uint v4) = 0;

		virtual void setUniform1uiv(xdl_uint id, xdl_uint count, xdl_uint* v1) = 0;
		virtual void setUniform2uiv(xdl_uint id, xdl_uint count, xdl_uint* v1) = 0;
		virtual void setUniform3uiv(xdl_uint id, xdl_uint count, xdl_uint* v1) = 0;
		virtual void setUniform4uiv(xdl_uint id, xdl_uint count, xdl_uint* v1) = 0;



		virtual void setUniform(xdl_uint id, xdl_float v1) = 0;
		virtual void setUniform(xdl_uint id, xdl_float v1, xdl_float v2) = 0;
		virtual void setUniform(xdl_uint id, xdl_float v1, xdl_float v2, xdl_float v3) = 0;
		virtual void setUniform(xdl_uint id, xdl_float v1, xdl_float v2, xdl_float v3, xdl_float v4) = 0;

		virtual void setUniform1v(xdl_uint id, xdl_uint number, xdl_float* array) = 0;
		virtual void setUniform2v(xdl_uint id, xdl_uint number, xdl_float* array) = 0;
		virtual void setUniform3v(xdl_uint id, xdl_uint number, xdl_float* array) = 0;
		virtual void setUniform4v(xdl_uint id, xdl_uint number, xdl_float* array) = 0;

		virtual void setVertexAttrib(xdl_uint id, xdl_float v1) = 0;
		virtual void setVertexAttrib(xdl_uint id, xdl_float v1, xdl_float v2) = 0;
		virtual void setVertexAttrib(xdl_uint id, xdl_float v1, xdl_float v2, xdl_float v3) = 0;
		virtual void setVertexAttrib(xdl_uint id, xdl_float v1, xdl_float v2, xdl_float v3, xdl_float v4) = 0;



		virtual xdl_uint id() = 0;
	};

}


#endif
