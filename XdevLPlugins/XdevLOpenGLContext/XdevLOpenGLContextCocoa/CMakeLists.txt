cmake_minimum_required(VERSION 2.8)

project(XdevLOpenGLContextCocoa)

#
# Set the names for the stuff we are going to create

set(XdevLOpenGLContextCocoaLibrary XdevLOpenGLContextCocoa)

#
# Set the version

set(VERSION "1.0.0")

#
# Set variable names for the library

set(HEADERS_DIR  ${XDEVL_PLUGINS})
set(SRC_DIR  ${XDEVL_PLUGINS})

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
)


#
# Set header files for the library.

set(HEADER_INTERFACE_FILE
	${HEADERS_DIR}/XdevLOpenGLContext/XdevLOpenGLContext.h
)

set(HEADER_FILES
	${HEADER_INTERFACE_FILE}
	${HEADERS_DIR}/XdevLOpenGLContext/XdevLOpenGLContextBase.h
	${HEADERS_DIR}/XdevLOpenGLContext/XdevLOpenGLContextCocoa/XdevLOpenGLContextCocoa.h
)

#
# Set the source files for the library.

set(SOURCE_FILES
	${SRC_DIR}/XdevLOpenGLContext/XdevLOpenGLContextCocoa/XdevLOpenGLContextCocoa.mm
)

#
# Add external libraries folder

link_directories(
	${XdevL_BINARY_DIR}/lib
)

#
# Create the library

add_library(${XdevLOpenGLContextCocoaLibrary} SHARED
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

#
# Add external ibraries

if(CMAKE_BUILD_TYPE MATCHES Debug)
	target_link_libraries(${XdevLOpenGLContextCocoaLibrary}
		XdevLd
		"-framework OpenGL"
		"-framework AGL"
		"-framework Cocoa"
	)
else()
	target_link_libraries(${XdevLOpenGLContextCocoaLibrary}
		XdevL
		"-framework OpenGL"
		"-framework AGL"
		"-framework Cocoa"
)
endif()


#
# Change the outputname of the library

set_property(TARGET ${XdevLOpenGLContextCocoaLibrary} PROPERTY PREFIX "")

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set_property(TARGET ${XdevLOpenGLContextCocoaLibrary} PROPERTY SUFFIX "-${VERSION}d${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
else()
	set_property(TARGET ${XdevLOpenGLContextCocoaLibrary} PROPERTY SUFFIX "-${VERSION}${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
endif()

install(FILES ${HEADER_INTERFACE_FILE} DESTINATION "${XDEVL_PLUGINS_INCLUDE_INSTALL_PATH}/XdevLOpenGLContext")
install(TARGETS ${XdevLOpenGLContextCocoaLibrary} DESTINATION ${XDEVL_PLUGINS_INSTALL_PATH})
