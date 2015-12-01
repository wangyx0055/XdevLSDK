cmake_minimum_required(VERSION 2.8)

project(XdevLWindow)

#
# Set the names for the stuff we are going to create
#
set(XdevLWindowLibrary XdevLWindow)

#
# Set the version
#
set(VERSION "1.0.0")

#
# Set variable names for the library
#
set(HEADERS_DIR .)
set(SRC_DIR .)

#
# Set the output directories
#
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${XDEVL_PLUGINS}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${XDEVL_PLUGINS}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${XDEVL_PLUGINS}/lib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${XDEVL_PLUGINS}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${XDEVL_PLUGINS}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${XDEVL_PLUGINS}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${XDEVL_PLUGINS}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${XDEVL_PLUGINS}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${XDEVL_PLUGINS}/lib)

#
# Set header files for the library.
#
set(HEADER_FILES
	${XDEVL_PLUGINS}/XdevLWindow/XdevLWindow.h
	${XDEVL_PLUGINS}/XdevLWindow/XdevLWindowImpl.h
	${HEADERS_DIR}/XdevLWindowWin32.h
)

#
# Set the source files for the library.
#
set(SOURCE_FILES
	${XDEVL_PLUGINS}/XdevLWindow/XdevLWindowImpl.cpp
	${SRC_DIR}/XdevLWindowWin32.cpp
)

#
# Add external libraries folder
#
link_directories(${XdevL_BINARY_DIR}/lib)

#
# Create the library

add_library(${XdevLWindowLibrary} SHARED
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
	target_link_libraries(${XdevLWindowLibrary}
		XdevLd
	)
else()
	target_link_libraries(${XdevLWindowLibrary}
		XdevL
	)
endif()


#
# Change the outputname of the library
#
set_property(TARGET ${XdevLWindowLibrary} PROPERTY PREFIX "")

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set_property(TARGET ${XdevLWindowLibrary} PROPERTY SUFFIX "-${VERSION}d${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
else()
	set_property(TARGET ${XdevLWindowLibrary} PROPERTY SUFFIX "-${VERSION}${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
endif()
