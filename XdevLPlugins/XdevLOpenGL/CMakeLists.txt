cmake_minimum_required(VERSION 2.8)

project(XdevLOpenGL)

find_package(SDL2)
find_package(OpenGL)

#
# Set the names for the stuff we are going to create

set(XdevLOpenGLLibrary XdevLOpenGL)

#
# Set the version

set(VERSION "1.0.0")

#
# Set variable names for the library

set(HEADERS_DIR .)
set(SRC_DIR .)

#
# Set the output directories

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ../lib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ../lib)


#
# Set header files for the library.

set(HEADER_FILES
	${HEADERS_DIR}/XdevLOpenGL.h
	${HEADERS_DIR}/XdevLTexture.h
	${HEADERS_DIR}/XdevLTextureCube.h	
	${HEADERS_DIR}/XdevLTextureImpl.h
	${HEADERS_DIR}/XdevLTextureCubeImpl.h
	${HEADERS_DIR}/XdevLTexture3D.h
	${HEADERS_DIR}/XdevLTexture3DImpl.h
	${HEADERS_DIR}/XdevLFrameBuffer.h
	${HEADERS_DIR}/XdevLFrameBufferImpl.h
	${HEADERS_DIR}/XdevLOpenGLImpl.h
	${HEADERS_DIR}/XdevLVertexBuffer.h
	${HEADERS_DIR}/XdevLVertexBufferImpl.h
	${HEADERS_DIR}/XdevLIndexBuffer.h
	${HEADERS_DIR}/XdevLIndexBufferImpl.h
	${HEADERS_DIR}/XdevLVertexArray.h
	${HEADERS_DIR}/XdevLVertexArrayImpl.h
	${HEADERS_DIR}/XdevLVertexDeclaration.h
	${HEADERS_DIR}/XdevLStreamBuffer.h	
	${HEADERS_DIR}/XdevLShader.h
	${HEADERS_DIR}/XdevLShaderImpl.h
	${HEADERS_DIR}/XdevLShaderProgram.h
	${HEADERS_DIR}/XdevLShaderProgramImpl.h
	${HEADERS_DIR}/XdevLOpenGLUtils.h
)

#
# Set the source files for the library.

set(SOURCE_FILES
	${SRC_DIR}/XdevLOpenGLImpl.cpp
	${SRC_DIR}/XdevLVertexArrayImpl.cpp
	${SRC_DIR}/XdevLFrameBufferImpl.cpp
	${SRC_DIR}/XdevLShaderProgramImpl.cpp
	${SRC_DIR}/XdevLOpenGLUtils.cpp
	${SRC_DIR}/GL/glew.c
)


if(APPLE)
	set(HEADER_FILES ${HEADER_FILES}
		${HEADERS_DIR}/XdevLOpenGLSDL.h
	)

	set(SOURCE_FILES ${SOURCE_FILES}
		${SRC_DIR}/XdevLOpenGLSDL.cpp
	)
elseif(UNIX)
	set(HEADER_FILES ${HEADER_FILES}
		${HEADERS_DIR}/XdevLOpenGLSDL.h
	)

	set(SOURCE_FILES ${SOURCE_FILES}
		${SRC_DIR}/XdevLOpenGLSDL.cpp
	)
elseif(WIN32)
	set(HEADER_FILES ${HEADER_FILES}
		${HEADERS_DIR}/XdevLOpenGLSDL.h
	)

	set(SOURCE_FILES ${SOURCE_FILES}
		${SRC_DIR}/XdevLOpenGLSDL.cpp
	)

endif()


add_definitions(-DGLEW_STATIC)

#
# Add external libraries folder

link_directories(
	${XdevL_BINARY_DIR}/lib
	${SDL2_LIBRARYDIR}
)

#
# Create the library

add_library(${XdevLOpenGLLibrary} SHARED
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

#
# Set the compiler property

set_property(TARGET ${XdevLOpenGLLibrary} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${XdevLOpenGLLibrary} PROPERTY CXX_STANDARD 11)

#
# Add include directories

include_directories(
	.
	${XDEVL_HOME}/include
	${XDEVL_PLUGINS}
	${SDL2_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIR}
)

#
# Add external ibraries

if(APPLE)
	if(CMAKE_BUILD_TYPE MATCHES Debug)
		target_link_libraries(${XdevLOpenGLLibrary}
			XdevLd
			${SDL2_LIBRARY}
			${OPENGL_gl_LIBRARY}
			"-framework AGL"
		)
		else()
			target_link_libraries(${XdevLOpenGLLibrary}
				XdevL
				${SDL2_LIBRARY}
				${OPENGL_gl_LIBRARY}
				"-framework AGL"
		)
	endif()
elseif(UNIX)
	if(CMAKE_BUILD_TYPE MATCHES Debug)
		target_link_libraries(${XdevLOpenGLLibrary}
			XdevLd
			GL
			SDL2
		)
	else()
		target_link_libraries(${XdevLOpenGLLibrary}
			XdevL
			GL
			SDL2
		)
	endif()
elseif(WIN32)
	if(CMAKE_BUILD_TYPE MATCHES Debug)
		target_link_libraries(${XdevLOpenGLLibrary}
			XdevLd
			${SDL2_LIBRARY}
			opengl32
			gdi32
		)
	else()
		target_link_libraries(${XdevLOpenGLLibrary}
			XdevL
			${SDL2_LIBRARY}
			opengl32
			gdi32
		)
	endif()
endif()

#
# Change the outputname of the library

set_property(TARGET ${XdevLOpenGLLibrary} PROPERTY PREFIX "")

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set_property(TARGET ${XdevLOpenGLLibrary} PROPERTY SUFFIX "-${VERSION}d${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
else()
	set_property(TARGET ${XdevLOpenGLLibrary} PROPERTY SUFFIX "-${VERSION}${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
endif()
