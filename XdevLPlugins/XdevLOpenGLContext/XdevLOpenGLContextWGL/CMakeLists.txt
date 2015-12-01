cmake_minimum_required(VERSION 2.8)

project(XdevLOpenGLContext)

#
# Set the names for the stuff we are going to create

set(XdevLOpenGLContextWGLLibrary XdevLOpenGLContextWGL)

#
# Set the version

set(VERSION "1.0.0")

#
# Set variable names for the library

set(HEADERS_DIR .)
set(SRC_DIR .)

#
# Set the output directories

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${XDEVL_PLUGINS}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG  ${XDEVL_PLUGINS}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE  ${XDEVL_PLUGINS}/lib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${XDEVL_PLUGINS}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG  ${XDEVL_PLUGINS}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${XDEVL_PLUGINS}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${XDEVL_PLUGINS}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  ${XDEVL_PLUGINS}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${XDEVL_PLUGINS}/lib)

#
# Add include directories

include_directories(
	.
	${XDEVL_HOME}/include
	${XDEVL_PLUGINS}
	${XDEVL_PLUGINS}/XdevLOpenGLContext
)

#
# Set header files for the library.

set(HEADER_FILES
	${XDEVL_PLUGINS}/XdevLOpenGLContext/XdevLOpenGLContext.h
	${HEADERS_DIR}/XdevLOpenGLContextWGL.h
)

#
# Set the source files for the library.

set(SOURCE_FILES
	${SRC_DIR}/XdevLOpenGLContextWGL.cpp
)


#
# Add external libraries folder

link_directories(
	${XdevL_BINARY_DIR}/lib
)

#
# Create the library

add_library(${XdevLOpenGLContextWGLLibrary} SHARED
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

#
# Add include directories

include_directories(
	.
	${XDEVL_HOME}/include
	${XDEVL_PLUGINS}
)

#
# Add external ibraries

if(CMAKE_BUILD_TYPE MATCHES Debug)
	target_link_libraries(${XdevLOpenGLContextLibrary}
		XdevLd
	)
else()
	target_link_libraries(${XdevLOpenGLContextLibrary}
		XdevL
	)
endif()

#
# Change the outputname of the library

set_property(TARGET ${XdevLOpenGLContextWGLLibrary} PROPERTY PREFIX "")

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set_property(TARGET ${XdevLOpenGLContextWGLLibrary} PROPERTY SUFFIX "-${VERSION}d${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
else()
	set_property(TARGET ${XdevLOpenGLContextWGLLibrary} PROPERTY SUFFIX "-${VERSION}${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
endif()
