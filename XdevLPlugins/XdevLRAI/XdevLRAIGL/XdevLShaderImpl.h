#ifndef XDEVL_SHADER_IMPL_H
#define XDEVL_SHADER_IMPL_H

#include <XdevLRAI/XdevLShader.h>
#include "XdevLOpenGLUtils.h"
#include <sstream>
#include <fstream>

namespace xdl {

	template<class Base>
	class XdevLShaderBase : public Base {
		public:
			XdevLShaderBase(XdevLShaderType type) : m_id(0), m_shaderVersion(330), m_shaderType(type) {

				// Create the shader.
				m_id = glCreateShader(m_shaderType);
				if(m_id == 0) {
					shaderLog(m_id);
					assert(0 && "XdevLShaderBase::XdevLShaderBase: OpenGL 'glCreateShader' failed.");
				}

				// Make the version of the shader to 330.
				std::stringstream ss;
				ss << m_shaderVersion;

				// Add shader version into the first line.
				m_shaderVersionDefinition = "#version " + ss.str() + " core" + "\n";
				m_shaderSource.push_back(m_shaderVersionDefinition);


			}
			virtual ~XdevLShaderBase() {
				glDeleteShader(m_id);
			}

			void addShaderCode(const xdl_char* shadercode) {
				m_shaderSource.push_back(std::string(shadercode));
			}

			xdl_int addShaderCodeFromFile(const xdl_char* includeFilename) {

				// Try to open the file.
				std::ifstream fs(includeFilename);
				if(fs.is_open()) {

					// If successfull read everything and include it into the shader
					// source code.
					std::stringstream shader;
					shader << fs.rdbuf();

				//	m_shaderSource.push_back("#line __LINE__\n");
					m_shaderSource.push_back(shader.str());

					return ERR_OK;
				}
				return ERR_ERROR;
			}

			xdl_int compileFromFile(const xdl_char* shaderFilename) {
				std::ifstream fs(shaderFilename);
				if(fs.is_open()) {
					std::stringstream shader;
					shader << fs.rdbuf();
					m_shaderSource.push_back(shader.str());

					if(compile() != ERR_OK) {
						std::cerr << "XdevLShaderBase::compileFromFile: Shader file: " << shaderFilename << std::endl;
						return ERR_ERROR;
					}
					return ERR_OK;
				}
				return ERR_ERROR;
			}

			virtual xdl_int compile() {

				// We have to prepare the number of source code fragments in the source
				// list and the pointers to them.
				std::vector<GLint> lengths;
				std::vector<const xdl::xdl_char*> pointers;

				std::vector<std::string>::iterator ib(m_shaderSource.begin());
				while(ib != m_shaderSource.end()) {
					lengths.push_back((*ib).size());
					pointers.push_back((*ib).data());
					ib++;
				}

				// Tell OpenGL the source we have to far.
				glShaderSource(m_id, m_shaderSource.size(), pointers.data(), lengths.data());

				// Compile and check for errors.
				glCompileShader(m_id);
				GLint status = GL_FALSE;
				glGetShaderiv(m_id, GL_COMPILE_STATUS, &status);
				if(status == GL_FALSE) {
					shaderLog(m_id);
					glDeleteShader(m_id);
					return ERR_ERROR;
				}

				return ERR_OK;

			}

			virtual xdl_int id() {
				return m_id;
			}

			virtual XdevLShaderType getShaderType() {
				return m_shaderType;
			}

			GLuint 														m_id;
			xdl_uint 													m_shaderVersion;
			XdevLShaderType 									m_shaderType;
			std::vector<std::string> 					m_shaderSource;
			std::string 											m_shaderVersionDefinition;
	};

	class XdevLOpenGLVertexShaderImpl : public XdevLShaderBase<XdevLVertexShader> {
		public:
			XdevLOpenGLVertexShaderImpl() : XdevLShaderBase(XDEVL_VERTEX_SHADER) {}

			virtual ~XdevLOpenGLVertexShaderImpl() {}

	};


	class XdevLOpenGLFragmentShaderImpl : public XdevLShaderBase<XdevLFragmentShader> {
		public:
			XdevLOpenGLFragmentShaderImpl() : XdevLShaderBase(XDEVL_FRAGMENT_SHADER) {}

			virtual ~XdevLOpenGLFragmentShaderImpl() {}
	};

	class XdevLOpenGLGeometryShaderImpl : public XdevLShaderBase<XdevLGeometryShader> {
		public:
			XdevLOpenGLGeometryShaderImpl() : XdevLShaderBase(XDEVL_GEOMETRY_SHADER) {}

			virtual ~XdevLOpenGLGeometryShaderImpl() {}

	};
}

#endif
