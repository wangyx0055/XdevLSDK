#ifndef XDEVL_SHADER_H
#define XDEVL_SHADER_H

#include <XdevLTypes.h>

namespace xdl {

	enum XdevLShaderType {
	    XDEVL_FRAGMENT_SHADER = 0x8B30,
	    XDEVL_VERTEX_SHADER		= 0x8B31,
	    XDEVL_GEOMETRY_SHADER	= 0x8DD9
	};

	/**
		@class XdevLShader
		@brief A Shader.

		The id() method return the OpenGL shader id.
	*/
	class XdevLShader {
		public:

			virtual ~XdevLShader() {}

			/// Adds shader source code to the current source code from a buffer.
			/**
				This function can help you to reuse code. Just add source fragments to this shader source code.
				@param shadercode A null terminated string that represents code fragments.
			*/
			virtual void addShaderCode(const xdl_char* shaderCode) = 0;

			/// Adds shader source code from a file.
			/**
				This method is the same like addShaderCode but instead of specifying a string buffer
				you have to specify a filename that holds the source code.
				@param shaderCodeFileName A null terminated string that specifies the filename of the source code file.
			*/
			virtual xdl_int addShaderCodeFromFile(const xdl_char* shaderCodeFileName) = 0;

			/// Adds and compiles at the same time a whole shader source code.
			virtual xdl_int compileFromFile(const xdl_char* shaderCodeFileName) = 0;

			/// Compile the current shader source code.
			virtual xdl_int compile() = 0;

			/// Returns the identification code of this shader.
			virtual xdl_int id() = 0;

			/// Returns the type of the shader.
			virtual XdevLShaderType getShaderType() = 0;
	};

	class XdevLVertexShader : public XdevLShader {
		public:
			virtual ~XdevLVertexShader() {}

	};

	class XdevLFragmentShader : public XdevLShader {
		public:
			virtual ~XdevLFragmentShader() {}
	};

	class XdevLGeometryShader : public XdevLShader {
		public:
			virtual ~XdevLGeometryShader() {}
	};
}

#endif
