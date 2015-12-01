cmake_minimum_required(VERSION 2.8)

project(XdevLWindowX11)

#
# Set the names for the stuff we are going to create

set(XdevLWindowX11Library XdevLWindowX11)

#
# Set the Plugin version
#

set(XDEVLX11_MAJOR_VERSION 1)
set(XDEVLX11_MINOR_VERSION 0)
set(XDEVLX11_PATCH_VERSION 0)
set(XDEVLX11_VERSION "${XDEVLX11_MAJOR_VERSION}.${XDEVLX11_MINOR_VERSION}.${XDEVLX11_PATCH_VERSION}")

add_definitions(
	-DXDEVLX11_MAJOR_VERSION=${XDEVLX11_MAJOR_VERSION}
	-DXDEVLX11_MINOR_VERSION=${XDEVLX11_MINOR_VERSION} 
	-DXDEVLX11_PATCH_VERSION=${XDEVLX11_PATCH_VERSION}
)

#
# Set the Module version
#

set(XDEVLX11_MODULE_MAJOR_VERSION 0)
set(XDEVLX11_MODULE_MINOR_VERSION 2)
set(XDEVLX11_MODULE_PATCH_VERSION 0)
set(XDEVLX11_MODULE_VERSION "${XDEVLX11_MODULE_MAJOR_VERSION}.${XDEVLX11_MODULE_MINOR_VERSION}.${XDEVLX11_MODULE_PATCH_VERSION}")

add_definitions(
	-DXDEVLX11_MODULE_MAJOR_VERSION=${XDEVLX11_MODULE_MAJOR_VERSION}
	-DXDEVLX11_MODULE_MINOR_VERSION=${XDEVLX11_MODULE_MINOR_VERSION} 
	-DXDEVLX11_MODULE_PATCH_VERSION=${XDEVLX11_MODULE_PATCH_VERSION}
)

#
# Set variable names for the library

set(HEADERS_DIR .)
set(SRC_DIR .)

#
# Set the output directories

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${XDEVL_PLUGINS}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${XDEVL_PLUGINS}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${XDEVL_PLUGINS}/lib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${XDEVL_PLUGINS}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG  ${XDEVL_PLUGINS}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${XDEVL_PLUGINS}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${XDEVL_PLUGINS}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  ${XDEVL_PLUGINS}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${XDEVL_PLUGINS}/lib)

#
# Set header files for the library.

set(HEADER_FILES
	${XDEVL_PLUGINS}/XdevLWindow/XdevLWindow.h
	${XDEVL_PLUGINS}/XdevLWindow/XdevLWindowImpl.h
	${HEADERS_DIR}/XdevLWindowX11.h
	${HEADERS_DIR}/XdevLWindowX11Keyboard.h
	${HEADERS_DIR}/imKStoUCS.h
	${HEADERS_DIR}/scancodes_linux.h
	${HEADERS_DIR}/scancodes_xfree86.h
	${HEADERS_DIR}/scancodes_darwin.h
)

#
# Set the source files for the library.

set(SOURCE_FILES
	${XDEVL_PLUGINS}/XdevLWindow/XdevLWindowImpl.cpp
	${SRC_DIR}/XdevLWindowX11.cpp
	${SRC_DIR}/XdevLWindowX11Keyboard.cpp
	${SRC_DIR}/imKStoUCS.c
)


#
# Add external libraries folder

set(LIBRARY_DIRECTORY ${XdevL_BINARY_DIR}/lib)
if(APPLE)
	set(LIBRARY_DIRECTORY ${LIBRARY_DIRECTORY} /opt/X11/lib)
elseif(UNIX)
	set(LIBRARY_DIRECTORY ${LIBRARY_DIRECTORY} /usr/X11/lib)
elseif(WINDOWS)
	error("This platform is not supported by this plugin")
endif()
link_directories(${LIBRARY_DIRECTORY})

#
# Create the library

add_library(${XdevLWindowX11Library} SHARED
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

#
# Set the compiler property

set_property(TARGET ${XdevLWindowX11Library} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${XdevLWindowX11Library} PROPERTY CXX_STANDARD 11)

#
# Add include directories

set(INCLUDE_DIRECTORY . ${XDEVL_HOME}/include ${XDEVL_PLUGINS})
if(APPLE)
	set(INCLUDE_DIRECTORY ${INCLUDE_DIRECTORY} /opt/X11/include)
elseif(UNIX)
	set(INCLUDE_DIRECTORY ${INCLUDE_DIRECTORY} /usr/X11/include)
elseif(WINDOWS)
	error("This platform is not supported by this plugin")
endif()
include_directories(${INCLUDE_DIRECTORY})

#
# Add external ibraries

if(APPLE)

if(CMAKE_BUILD_TYPE MATCHES Debug)
		target_link_libraries(${XdevLWindowX11Library}
			XdevLd
			X11
			Xrandr
			pthread
		)
	else()
		target_link_libraries(${XdevLWindowX11Library}
			XdevL
			X11
			Xrandr
			pthread
		)
	endif()

elseif(UNIX)
	if(CMAKE_BUILD_TYPE MATCHES Debug)
		target_link_libraries(${XdevLWindowX11Library}
			XdevLd
			X11
			Xrandr
			pthread
		)
	else()
		target_link_libraries(${XdevLWindowX11Library}
			XdevL
			X11
			Xrandr
			pthread
		)
	endif()
endif()

#
# Change the outputname of the library

set_property(TARGET ${XdevLWindowX11Library} PROPERTY PREFIX "")

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set_property(TARGET ${XdevLWindowX11Library} PROPERTY SUFFIX "-${XDEVLX11_VERSION}d${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
else()
	set_property(TARGET ${XdevLWindowX11Library} PROPERTY SUFFIX "-${XDEVLX11_VERSION}${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
endif()
