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

#ifndef XDEVL_SHADER_PROGRAM_IMPL_H
#define XDEVL_SHADER_PROGRAM_IMPL_H

#include <XdevLTypes.h>
#include <XdevLRAI/XdevLShaderProgram.h>

namespace xdl {

class XdevLOpenGLProgramImpl: public XdevLShaderProgram {
	public:
		XdevLOpenGLProgramImpl();
		virtual ~XdevLOpenGLProgramImpl();

		virtual xdl_int attach(XdevLShader* shader) ;

		virtual xdl_int link();

		virtual xdl_int getUniformLocation(const char* id);

		virtual xdl_int getAttribLocation(const char* id);

		virtual void activate();
		virtual void deactivate();
		virtual void enable();
		virtual void disable();

		virtual void setUniformMatrix4(xdl_uint id, xdl_uint count, const xdl_float* v1);
		virtual void setUniformMatrix3(xdl_uint id, xdl_uint count, const xdl_float* v1);
		virtual void setUniformMatrix2(xdl_uint id, xdl_uint count, const xdl_float* v1);

		virtual void setUniformi(xdl_uint id, xdl_int v1);
		virtual void setUniformi(xdl_uint id, xdl_int v1, xdl_int v2);
		virtual void setUniformi(xdl_uint id, xdl_int v1, xdl_int v2, xdl_int v3);
		virtual void setUniformi(xdl_uint id, xdl_int v1, xdl_int v2, xdl_int v3, xdl_int v4);

		virtual void setUniformui(xdl_uint id, xdl_uint v1);
		virtual void setUniformui(xdl_uint id, xdl_uint v1, xdl_uint v2);
		virtual void setUniformui(xdl_uint id, xdl_uint v1, xdl_uint v2, xdl_uint v3);
		virtual void setUniformui(xdl_uint id, xdl_uint v1, xdl_uint v2, xdl_uint v3, xdl_uint v4);

		virtual void setUniform1uiv(xdl_uint id, xdl_uint count, xdl_uint* v1);
		virtual void setUniform2uiv(xdl_uint id, xdl_uint count, xdl_uint* v1);
		virtual void setUniform3uiv(xdl_uint id, xdl_uint count, xdl_uint* v1);
		virtual void setUniform4uiv(xdl_uint id, xdl_uint count, xdl_uint* v1);


		virtual void setUniform(xdl_uint id, xdl_float v1);
		virtual void setUniform(xdl_uint id, xdl_float v1, xdl_float v2);
		virtual void setUniform(xdl_uint id, xdl_float v1, xdl_float v2, xdl_float v3);
		virtual void setUniform(xdl_uint id, xdl_float v1, xdl_float v2, xdl_float v3, xdl_float v4);

		virtual void setUniform1v(xdl_uint id, xdl_uint number, xdl_float* array);
		virtual void setUniform2v(xdl_uint id, xdl_uint number, xdl_float* array);
		virtual void setUniform3v(xdl_uint id, xdl_uint number, xdl_float* array);
		virtual void setUniform4v(xdl_uint id, xdl_uint number, xdl_float* array);

		virtual void setVertexAttrib(xdl_uint id, xdl_float v1);
		virtual void setVertexAttrib(xdl_uint id, xdl_float v1, xdl_float v2);
		virtual void setVertexAttrib(xdl_uint id, xdl_float v1, xdl_float v2, xdl_float v3);
		virtual void setVertexAttrib(xdl_uint id, xdl_float v1, xdl_float v2, xdl_float v3, xdl_float v4);

		virtual xdl_uint id();
		void setGLid(GLuint id);
		GLuint m_id;
		xdl_bool m_inUse;
	};

}

#endif
