cmake_minimum_required(VERSION 2.8)

project(XdevLCore)

message(STATUS "Building XdevLCore: " ${XDEVLCORE_VERSION})

#
# Set the names for the stuff we are going to create

set(XdevLCoreLibrary XdevLCore)


#
# Set the Plugin version
#

set(XDEVLCORE_MAJOR_VERSION 0)
set(XDEVLCORE_MINOR_VERSION 5)
set(XDEVLCORE_PATCH_VERSION 0)
set(XDEVLCORE_VERSION "${XDEVLCORE_MAJOR_VERSION}.${XDEVLCORE_MINOR_VERSION}.${XDEVLCORE_PATCH_VERSION}")

add_definitions(
	-DXDEVLCORE_MAJOR_VERSION=${XDEVLCORE_MAJOR_VERSION}
	-DXDEVLCORE_MINOR_VERSION=${XDEVLCORE_MINOR_VERSION} 
	-DXDEVLCORE_PATCH_VERSION=${XDEVLCORE_PATCH_VERSION}
)

#
# Set the Module version
#

set(XDEVLCORE_MODULE_MAJOR_VERSION 0)
set(XDEVLCORE_MODULE_MINOR_VERSION 5)
set(XDEVLCORE_MODULE_PATCH_VERSION 0)
set(XDEVLCORE_MODULE_VERSION "${XDEVLCORE_MODULE_MAJOR_VERSION}.${XDEVLCORE_MODULE_MINOR_VERSION}.${XDEVLCORE_MODULE_PATCH_VERSION}")

add_definitions(
	-DXDEVLCORE_MODULE_MAJOR_VERSION=${XDEVLCORE_MODULE_MAJOR_VERSION}
	-DXDEVLCORE_MODULE_MINOR_VERSION=${XDEVLCORE_MODULE_MINOR_VERSION} 
	-DXDEVLCORE_MODULE_PATCH_VERSION=${XDEVLCORE_MODULE_PATCH_VERSION}
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
    ${HEADERS_DIR}/XdevLCoreImpl.h	
)

#
# Set the source files for the library.

set(SOURCE_FILES
	${SRC_DIR}/XdevLCoreImpl.cpp
)


link_directories(${XdevL_BINARY_DIR}/lib)

#
# Create the library

add_library(${XdevLCoreLibrary} SHARED
    ${HEADER_FILES}
    ${SOURCE_FILES}
)


#
# Set the compiler property

set_property(TARGET ${XdevLCoreLibrary} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${XdevLCoreLibrary} PROPERTY CXX_STANDARD 11)

#
# Add include directories

include_directories(
	.
	${XDEVL_HOME}/include
	${XDEVL_PLUGINS}
)

#
# Add libraries

if(CMAKE_BUILD_TYPE MATCHES Debug)
	if(UNIX AND NOT APPLE)
		target_link_libraries(${XdevLCoreLibrary}
			 XdevLd
			 pthread
	)
	elseif(APPLE)
		target_link_libraries(${XdevLCoreLibrary}
				 XdevLd
				 pthread
		)
	elseif(WIN32)
		target_link_libraries(${XdevLCoreLibrary}
				 XdevLd
		)
	endif()
else()
	if(UNIX AND NOT APPLE)
		target_link_libraries(${XdevLCoreLibrary}
			 XdevL
			 pthread
		)
	elseif(APPLE)
			target_link_libraries(${XdevLCoreLibrary}
			 XdevL
			 pthread
		)
	elseif(WIN32)
		target_link_libraries(${XdevLCoreLibrary}
				 XdevL
		)
	endif()
endif()


#
# Change the outputname of the library
#
set_property(TARGET ${XdevLCoreLibrary} PROPERTY PREFIX "")

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set_property(TARGET ${XdevLCoreLibrary} PROPERTY SUFFIX "-${XDEVLCORE_VERSION}d${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
else()
	set_property(TARGET ${XdevLCoreLibrary} PROPERTY SUFFIX "-${XDEVLCORE_VERSION}${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
endif()
