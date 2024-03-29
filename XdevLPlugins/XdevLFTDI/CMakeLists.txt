cmake_minimum_required(VERSION 2.8)

project(XdevLFTDI)

#
# Set the names for the stuff we are going to create

set(XdevLFTDILibrary XdevLFTDI)

#
# Set the Plugin version
#

set(XDEVLFTDI_MAJOR_VERSION 0)
set(XDEVLFTDI_MINOR_VERSION 1)
set(XDEVLFTDI_PATCH_VERSION 0)
set(XDEVLFTDI_VERSION "${XDEVLFTDI_MAJOR_VERSION}.${XDEVLFTDI_MINOR_VERSION}.${XDEVLFTDI_PATCH_VERSION}")

add_definitions(
	-DXDEVLFTDI_MAJOR_VERSION=${XDEVLFTDI_MAJOR_VERSION}
	-DXDEVLFTDI_MINOR_VERSION=${XDEVLFTDI_MINOR_VERSION} 
	-DXDEVLFTDI_PATCH_VERSION=${XDEVLFTDI_PATCH_VERSION}
)

#
# Set the AL Module version
#

set(XDEVLFTDI_MODULE_MAJOR_VERSION 0)
set(XDEVLFTDI_MODULE_MINOR_VERSION 1)
set(XDEVLFTDI_MODULE_PATCH_VERSION 0)
set(XDEVLFTDI_MODULE_VERSION "${XDEVLFTDI_MODULE_MAJOR_VERSION}.${XDEVLFTDI_MODULE_MINOR_VERSION}.${XDEVLFTDI_MODULE_PATCH_VERSION}")

add_definitions(
	-DXDEVLFTDI_MODULE_MAJOR_VERSION=${XDEVLFTDI_MODULE_MAJOR_VERSION}
	-DXDEVLFTDI_MODULE_MINOR_VERSION=${XDEVLFTDI_MODULE_MINOR_VERSION} 
	-DXDEVLFTDI_MODULE_PATCH_VERSION=${XDEVLFTDI_MODULE_PATCH_VERSION}
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
	${HEADERS_DIR}/XdevLFTDI.h
)

#
# Set the source files for the library.

set(SOURCE_FILES
	${SRC_DIR}/XdevLFTDI.cpp
)

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    MESSAGE( "64 bit compiler detected" )
    SET( EX_PLATFORM 64 )
    SET( EX_PLATFORM_NAME "x64" )
else() 
    MESSAGE( "32 bit compiler detected" )
    SET( EX_PLATFORM 32 )
    SET( EX_PLATFORM_NAME "x86" )
endif()

	
#
# Add external libraries folder
set(LIBRARY_DIRS ${XdevL_BINARY_DIR}/lib)

#
# Add specific folder for the FTDI library
if(EX_PLATFORM EQUAL 64)
	set(LIBRARY_DIRS ${LIBRARY_DIRS}
		${XDEVL_EXTERNALS}/ftdid2xx/build/x64_64
	)
elseif(EX_PLATFORM EQUAL 64)
	set(LIBRARY_DIRS ${LIBRARY_DIRS}
		${XDEVL_EXTERNALS}/ftdid2xx/build/i386
	)
endif()


message(${LIBRARY_DIRS})

	
link_directories( ${LIBRARY_DIRS} )	
	
#
# Create the library
#
add_library(${XdevLFTDILibrary} SHARED
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

#
# Add include directories
#
include_directories( 
	.
	${XDEVL_HOME}/include
	${XDEVL_PLUGINS}
	${XDEVL_EXTERNALS}/ftdid2xx
)

#
# Add external ibraries

target_link_libraries( ${XdevLFTDILibrary} XdevL ftd2xx )

#
# Change the outputname of the library

set_property(TARGET ${XdevLFTDILibrary} PROPERTY PREFIX "${XDEVL_SHARED_LIBRARY_FILE_PREFIX}")

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set_property(TARGET ${XdevLFTDILibrary} PROPERTY SUFFIX "-${XDEVLFTDI_VERSION}d${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
else()
	set_property(TARGET ${XdevLFTDILibrary} PROPERTY SUFFIX "-${XDEVLFTDI_VERSION}${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
endif()

