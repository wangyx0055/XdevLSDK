cmake_minimum_required(VERSION 2.8)

project(XdevLMouse)

#
# Set the names for the stuff we are going to create

set(XdevLMouseLibrary XdevLMouse)

#
# Set the Plugin version
#

set(XDEVLMOUSE_MAJOR_VERSION 1)
set(XDEVLMOUSE_MINOR_VERSION 0)
set(XDEVLMOUSE_PATCH_VERSION 0)
set(XDEVLMOUSE_VERSION "${XDEVLMOUSE_MAJOR_VERSION}.${XDEVLMOUSE_MINOR_VERSION}.${XDEVLMOUSE_PATCH_VERSION}")

add_definitions(
	-DXDEVLMOUSE_MAJOR_VERSION=${XDEVLMOUSE_MAJOR_VERSION}
	-DXDEVLMOUSE_MINOR_VERSION=${XDEVLMOUSE_MINOR_VERSION} 
	-DXDEVLMOUSE_PATCH_VERSION=${XDEVLMOUSE_PATCH_VERSION}
)

#
# Set the Module version
#

set(XDEVLMOUSE_MODULE_MAJOR_VERSION 0)
set(XDEVLMOUSE_MODULE_MINOR_VERSION 2)
set(XDEVLMOUSE_MODULE_PATCH_VERSION 0)
set(XDEVLMOUSE_MODULE_VERSION "${XDEVLMOUSE_MODULE_MAJOR_VERSION}.${XDEVLMOUSE_MODULE_MINOR_VERSION}.${XDEVLMOUSE_MODULE_PATCH_VERSION}")

add_definitions(
	-DXDEVLMOUSE_MODULE_MAJOR_VERSION=${XDEVLMOUSE_MODULE_MAJOR_VERSION}
	-DXDEVLMOUSE_MODULE_MINOR_VERSION=${XDEVLMOUSE_MODULE_MINOR_VERSION} 
	-DXDEVLMOUSE_MODULE_PATCH_VERSION=${XDEVLMOUSE_MODULE_PATCH_VERSION}
)

#
# Set variable names for the library

set(HEADERS_DIR .)
set(SRC_DIR .)

#
# Set the output directories

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${XDEVL_PLUGINS}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${XDEVL_PLUGINS}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${XDEVL_PLUGINS}/lib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${XDEVL_PLUGINS}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${XDEVL_PLUGINS}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${XDEVL_PLUGINS}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${XDEVL_PLUGINS}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${XDEVL_PLUGINS}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE .${XDEVL_PLUGINS}/lib)

#
# Set header files for the library.

set(HEADER_FILES
	${HEADERS_DIR}/XdevLMouse.h
	${HEADERS_DIR}/XdevLMouseImpl.h
)

#
# Set the source files for the library.

set(SOURCE_FILES
	${SRC_DIR}/XdevLMouseImpl.cpp
)


#
# Add external libraries folder

link_directories(${XdevL_BINARY_DIR}/lib)

#
# Create the library

add_library(${XdevLMouseLibrary} SHARED
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

#
# Set the compiler property

set_property(TARGET ${XdevLMouseLibrary} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${XdevLMouseLibrary} PROPERTY CXX_STANDARD 11)


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
	target_link_libraries(
		${XdevLMouseLibrary}
		XdevLd
	)
else()
	target_link_libraries(
		${XdevLMouseLibrary}
		XdevL
	)
endif()


#
# Change the outputname of the library
#
set_property(TARGET ${XdevLMouseLibrary} PROPERTY PREFIX "")

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set_property(TARGET ${XdevLMouseLibrary} PROPERTY SUFFIX "-${XDEVLMOUSE_VERSION}d${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
else()
	set_property(TARGET ${XdevLMouseLibrary} PROPERTY SUFFIX "-${XDEVLMOUSE_VERSION}${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
endif()
