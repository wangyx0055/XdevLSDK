cmake_minimum_required(VERSION 2.8)

project(XdevLComputeDeviceCL)

#
# Set the names for the stuff we are going to create

set(XdevLComputeDeviceCLibrary XdevLComputeDeviceCL)


#
# Set the Plugin version
#

set(XDEVLCOMPUTE_DEVICE_CL_MAJOR_VERSION 0)
set(XDEVLCOMPUTE_DEVICE_CL_MINOR_VERSION 1)
set(XDEVLCOMPUTE_DEVICE_CL_PATCH_VERSION 0)
set(XDEVLCOMPUTE_DEVICE_CL_VERSION "${XDEVLCOMPUTE_DEVICE_CL_MAJOR_VERSION}.${XDEVLCOMPUTE_DEVICE_CL_MINOR_VERSION}.${XDEVLCOMPUTE_DEVICE_CL_PATCH_VERSION}")

add_definitions(
	-DXDEVLCOMPUTE_DEVICE_CL_MAJOR_VERSION=${XDEVLCOMPUTE_DEVICE_CL_MAJOR_VERSION}
	-DXDEVLCOMPUTE_DEVICE_CL_MINOR_VERSION=${XDEVLCOMPUTE_DEVICE_CL_MINOR_VERSION} 
	-DXDEVLCOMPUTE_DEVICE_CL_PATCH_VERSION=${XDEVLCOMPUTE_DEVICE_CL_PATCH_VERSION}
)

#
# Set the Module version
#

set(XDEVLCOMPUTE_DEVICE_CL_MODULE_MAJOR_VERSION 0)
set(XDEVLCOMPUTE_DEVICE_CL_MODULE_MINOR_VERSION 2)
set(XDEVLCOMPUTE_DEVICE_CL_MODULE_PATCH_VERSION 0)
set(XDEVLCOMPUTE_DEVICE_CL_MODULE_VERSION "${XDEVLCOMPUTE_DEVICE_CL_MODULE_MAJOR_VERSION}.${XDEVLCOMPUTE_DEVICE_CL_MODULE_MINOR_VERSION}.${XDEVLCOMPUTE_DEVICE_CL_MODULE_PATCH_VERSION}")

add_definitions(
	-DXDEVLCOMPUTE_DEVICE_CL_MODULE_MAJOR_VERSION=${XDEVLCOMPUTE_DEVICE_CL_MODULE_MAJOR_VERSION}
	-DXDEVLCOMPUTE_DEVICE_CL_MODULE_MINOR_VERSION=${XDEVLCOMPUTE_DEVICE_CL_MODULE_MINOR_VERSION} 
	-DXDEVLCOMPUTE_DEVICE_CL_MODULE_PATCH_VERSION=${XDEVLCOMPUTE_DEVICE_CL_MODULE_PATCH_VERSION}
)


#
# Set variable names for the library

set(HEADERS_DIR ${XdevLComputeDeviceCL_SOURCE_DIR})
set(SRC_DIR ${XdevLComputeDeviceCL_SOURCE_DIR})

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
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${XDEVL_PLUGINS}/lib)

#
# Set header files for the library.

set(HEADER_INTERFACE_FILE ${XDEVL_PLUGINS}/XdevLComputeDevice/XdevLComputeDevice.h)

set(HEADER_FILES
	${HEADER_INTERFACE_FILE}
	${SRC_DIR}/XdevLComputeDeviceCL.h
)

#
# Set the source files for the library.

set(SOURCE_FILES
	${SRC_DIR}/XdevLComputeDeviceCL.cpp
)

if(NOT XDEVL_BUILD_STATIC)
	set(SOURCE_FILES ${SOURCE_FILES} ${SRC_DIR}/XdevLComputeDeviceCLPlugin.cpp)
endif()

#
# Add external libraries folder

link_directories(
	${XdevL_BINARY_DIR}/lib
)
	
#
# Add include directories
#
include_directories( 
	.
	${XDEVL_HOME}/include
	${XDEVL_PLUGINS}/XdevLComputeDevice
	${XDEVL_PLUGINS}
	${OPENCL_INCLUDE_DIRS}
)	
	
#
# Create the library

add_library(${XdevLComputeDeviceCLibrary} SHARED
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

#
# Add external ibraries

target_link_libraries( ${XdevLComputeDeviceCLibrary} XdevL ${OPENCL_LIBRARIES})

#
# Change the outputname of the library
set_property(TARGET ${XdevLComputeDeviceCLibrary} PROPERTY PREFIX "${XDEVL_SHARED_LIBRARY_FILE_PREFIX}")

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set_property(TARGET ${XdevLComputeDeviceCLibrary} PROPERTY SUFFIX "-${XDEVLCOMPUTE_DEVICE_CL_VERSION}d${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
else()
	set_property(TARGET ${XdevLComputeDeviceCLibrary} PROPERTY SUFFIX "-${XDEVLCOMPUTE_DEVICE_CL_VERSION}${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
endif()


install(FILES ${HEADER_INTERFACE_FILE} DESTINATION "${XDEVL_PLUGINS_INCLUDE_INSTALL_PATH}/${PROJECT_NAME}")
install(TARGETS ${XdevLComputeDeviceCLibrary} DESTINATION ${XDEVL_PLUGINS_INSTALL_PATH})
