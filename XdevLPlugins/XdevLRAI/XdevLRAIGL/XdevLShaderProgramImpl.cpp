#include "GL/glew.h"
#include "XdevLShaderProgramImpl.h"
#include "XdevLOpenGLUtils.h"
#include <XdevLError.h>
#include <cassert>
#include <iostream>

namespace xdl {


	XdevLOpenGLProgramImpl::XdevLOpenGLProgramImpl() : m_id(0),
																		m_inUse(xdl_false) {
		m_id = glCreateProgram();
		if(m_id == 0) {
		}
	}

	XdevLOpenGLProgramImpl::~XdevLOpenGLProgramImpl() {
		if(m_id != 0) {
			glDeleteProgram(m_id);
		}
	}

	xdl_int XdevLOpenGLProgramImpl::attach(XdevLShader* shader) {
		glAttachShader(m_id, shader->id());
		return ERR_OK;
	}

	xdl_int XdevLOpenGLProgramImpl::link() {
		glLinkProgram(m_id);

		GLint LinkStatus;
		glGetProgramiv(m_id, GL_LINK_STATUS, &LinkStatus);
		if(LinkStatus == GL_FALSE) {
			return ERR_ERROR;
		}
		return ERR_OK;
	}

	xdl_int XdevLOpenGLProgramImpl::getUniformLocation(const char* id) {
		GLint tmp =  glGetUniformLocation(this->m_id, id);
		if(tmp == -1) {
		}
		return tmp;
	}

	xdl_int XdevLOpenGLProgramImpl::getAttribLocation(const char* id) {
		GLint tmp = glGetAttribLocation(this->m_id, id);
		if(tmp == -1) {
		}
		return tmp;
	}

	void XdevLOpenGLProgramImpl::activate() {
		glUseProgram(m_id);
		m_inUse = true;
	}
	void XdevLOpenGLProgramImpl::deactivate() {
		glUseProgram(0);
		m_inUse = false;
	}

	void XdevLOpenGLProgramImpl::enable() {
		glUseProgram(m_id);
	}

	void XdevLOpenGLProgramImpl::disable() {
		glUseProgram(0);
	}

	void XdevLOpenGLProgramImpl::setUniformMatrix4(xdl_uint id, xdl_uint count, const xdl_float* v1) {
		assert(m_inUse && "activate() not used.\n");
		glUniformMatrix4fv(id, count, 0, v1);
	}

	void XdevLOpenGLProgramImpl::setUniformMatrix3(xdl_uint id, xdl_uint count, const xdl_float* v1) {
		assert(m_inUse && "activate() not used.\n");
		glUniformMatrix3fv(id, count, 0, v1);
	}

	void XdevLOpenGLProgramImpl::setUniformMatrix2(xdl_uint id, xdl_uint count, const xdl_float* v1) {
		assert(m_inUse && "activate() not used.\n");
		glUniformMatrix2fv(id, count, 0, v1);
	}

void XdevLOpenGLProgramImpl::setUniformi(xdl_uint id, xdl_int v1) {
		assert(m_inUse && "activate() not used.\n");
		glUniform1i(id, v1);
	}

	void XdevLOpenGLProgramImpl::setUniformi(xdl_uint id, xdl_int v1, xdl_int v2) {
		assert(m_inUse && "activate() not used.\n");
		glUniform2i(id, v1, v2);
	}

	void XdevLOpenGLProgramImpl::setUniformi(xdl_uint id, xdl_int v1, xdl_int v2, xdl_int v3) {
		assert(m_inUse && "activate() not used.\n");
		glUniform3i(id, v1, v2, v3);
	}

	void XdevLOpenGLProgramImpl::setUniformi(xdl_uint id, xdl_int v1, xdl_int v2, xdl_int v3, xdl_int v4) {
		assert(m_inUse && "activate() not used.\n");
		glUniform4i(id, v1, v2, v3, v4);
	}


	void XdevLOpenGLProgramImpl::setUniformui(xdl_uint id, xdl_uint v1) {
		assert(m_inUse && "activate() not used.\n");
		glUniform1ui(id, v1);
	}

	void XdevLOpenGLProgramImpl::setUniformui(xdl_uint id, xdl_uint v1, xdl_uint v2) {
		assert(m_inUse && "activate() not used.\n");
		glUniform2ui(id, v1, v2);
	}

	void XdevLOpenGLProgramImpl::setUniformui(xdl_uint id, xdl_uint v1, xdl_uint v2, xdl_uint v3) {
		assert(m_inUse && "activate() not used.\n");
		glUniform3ui(id, v1, v2, v3);
	}

	void XdevLOpenGLProgramImpl::setUniformui(xdl_uint id, xdl_uint v1, xdl_uint v2, xdl_uint v3, xdl_uint v4) {
		assert(m_inUse && "activate() not used.\n");
		glUniform4ui(id, v1, v2, v3, v4);
	}

  void XdevLOpenGLProgramImpl::setUniform1uiv(xdl_uint id, xdl_uint count, xdl_uint* v) {
  assert(m_inUse && "activate() not used.\n");
  glUniform1uiv(id, count, v);
  }

  void XdevLOpenGLProgramImpl::setUniform2uiv(xdl_uint id, xdl_uint count, xdl_uint* v) {
  assert(m_inUse && "activate() not used.\n");
  glUniform2uiv(id, count, v);
  }

  void XdevLOpenGLProgramImpl::setUniform3uiv(xdl_uint id, xdl_uint count, xdl_uint* v) {
  assert(m_inUse && "activate() not used.\n");
  glUniform4uiv(id, count, v);
  }

  void XdevLOpenGLProgramImpl::setUniform4uiv(xdl_uint id, xdl_uint count, xdl_uint* v) {
  assert(m_inUse && "activate() not used.\n");
  glUniform4uiv(id, count, v);
  }


	void XdevLOpenGLProgramImpl::setUniform(xdl_uint id, xdl_float v1) {
		assert(m_inUse && "activate() not used.\n");
		glUniform1f(id, v1);
	}
	void XdevLOpenGLProgramImpl::setUniform(xdl_uint id, xdl_float v1, xdl_float v2) {
		assert(m_inUse && "activate() not used.\n");
		glUniform2f(id, v1, v2);
	}
	void XdevLOpenGLProgramImpl::setUniform(xdl_uint id, xdl_float v1, xdl_float v2, xdl_float v3) {
		assert(m_inUse && "activate() not used.\n");
		glUniform3f(id, v1, v2, v3);
	}
	void XdevLOpenGLProgramImpl::setUniform(xdl_uint id, xdl_float v1, xdl_float v2, xdl_float v3, xdl_float v4) {
		assert(m_inUse && "activate() not used.\n");
		glUniform4f(id, v1, v2, v3, v4);
	}
	void XdevLOpenGLProgramImpl::setUniform1v(xdl_uint id, xdl_uint number, xdl_float* array) {
		assert(m_inUse && "activate() not used.\n");
		glUniform1fv(id, number, array);
	}
	void XdevLOpenGLProgramImpl::setUniform2v(xdl_uint id, xdl_uint number, xdl_float* array) {
		assert(m_inUse && "activate() not used.\n");
		glUniform2fv(id, number, array);
	}
	void XdevLOpenGLProgramImpl::setUniform3v(xdl_uint id, xdl_uint number, xdl_float* array) {
		assert(m_inUse && "activate() not used.\n");
		glUniform3fv(id, number, array);
	}
	void XdevLOpenGLProgramImpl::setUniform4v(xdl_uint id, xdl_uint number, xdl_float* array) {
		assert(m_inUse && "activate() not used.\n");
		glUniform4fv(id, number, array);
	}


	void XdevLOpenGLProgramImpl::setVertexAttrib(xdl_uint id, xdl_float v1) {
		assert(m_inUse && "begin() not used.\n");
		glVertexAttrib1f(id, v1);
	}
	void XdevLOpenGLProgramImpl::setVertexAttrib(xdl_uint id, xdl_float v1, xdl_float v2) {
		assert(m_inUse && "begin() not used.\n");
		glVertexAttrib2f(id, v1, v2);
	}
	void XdevLOpenGLProgramImpl::setVertexAttrib(xdl_uint id, xdl_float v1, xdl_float v2, xdl_float v3) {
		assert(m_inUse && "begin() not used.\n");
		glVertexAttrib3f(id, v1, v2, v3);
	}
	void XdevLOpenGLProgramImpl::setVertexAttrib(xdl_uint id, xdl_float v1, xdl_float v2, xdl_float v3, xdl_float v4) {
		assert(m_inUse && "begin() not used.\n");
		glVertexAttrib4f(id, v1, v2, v3, v4);
	}

	xdl_uint XdevLOpenGLProgramImpl::id() {
		return m_id;
	}
	void XdevLOpenGLProgramImpl::setGLid(GLuint id) {
		m_id = id;
	}


}
