cmake_minimum_required(VERSION 2.8)

project(XdevLSerial)

#
# Set the names for the stuff we are going to create

set(XdevLSerialLibrary XdevLSerial)

#
# Set the Plugin version
#

set(XDEVLSERIAL_MAJOR_VERSION 0)
set(XDEVLSERIAL_MINOR_VERSION 1)
set(XDEVLSERIAL_PATCH_VERSION 0)
set(XDEVLSERIAL_VERSION "${XDEVLSERIAL_MAJOR_VERSION}.${XDEVLSERIAL_MINOR_VERSION}.${XDEVLSERIAL_PATCH_VERSION}")

add_definitions(
	-DXDEVLSERIAL_MAJOR_VERSION=${XDEVLSERIAL_MAJOR_VERSION}
	-DXDEVLSERIAL_MINOR_VERSION=${XDEVLSERIAL_MINOR_VERSION} 
	-DXDEVLSERIAL_PATCH_VERSION=${XDEVLSERIAL_PATCH_VERSION}
)

#
# Set the AL Module version
#

set(XDEVLSERIAL_MODULE_MAJOR_VERSION 0)
set(XDEVLSERIAL_MODULE_MINOR_VERSION 1)
set(XDEVLSERIAL_MODULE_PATCH_VERSION 0)
set(XDEVLSERIAL_MODULE_VERSION "${XDEVLSERIAL_MODULE_MAJOR_VERSION}.${XDEVLSERIAL_MODULE_MINOR_VERSION}.${XDEVLSERIAL_MODULE_PATCH_VERSION}")

add_definitions(
	-DXDEVLSERIAL_MODULE_MAJOR_VERSION=${XDEVLSERIAL_MODULE_MAJOR_VERSION}
	-DXDEVLSERIAL_MODULE_MINOR_VERSION=${XDEVLSERIAL_MODULE_MINOR_VERSION} 
	-DXDEVLSERIAL_MODULE_PATCH_VERSION=${XDEVLSERIAL_MODULE_PATCH_VERSION}
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
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${XDEVL_PLUGINS}/lib)

#
# Set header files for the library.

set(HEADER_INTERFACE_FILE 
	${HEADERS_DIR}/XdevLSerial.h
)

set(HEADER_FILES
	${HEADER_INTERFACE_FILE}
	${HEADERS_DIR}/XdevLSerialImpl.h
)

#
# Set the source files for the library.

set(SOURCE_FILES
	
)


if(APPLE)
	set(HEADER_FILES ${HEADER_FILES} 
		${HEADERS_DIR}/XdevLSerialMacOSX.h
	)

	set(SOURCE_FILES ${SOURCE_FILES}
		${SRC_DIR}/XdevLSerialMacOSX.cpp
	)
elseif(UNIX)
	set(HEADER_FILES ${HEADER_FILES} 
		${HEADERS_DIR}/XdevLSerialLinux.h
	)

	set(SOURCE_FILES ${SOURCE_FILES}
		${SRC_DIR}/XdevLSerialLinux.cpp
	)

elseif(WIN32)
	set(HEADER_FILES ${HEADER_FILES} 
		${HEADERS_DIR}/XdevLSerialWin.h
	)

	set(SOURCE_FILES ${SOURCE_FILES}
		${SRC_DIR}/XdevLSerialWin.cpp
	)

endif()

	
#
# Add external libraries folder

link_directories(${XdevL_BINARY_DIR}/lib)
	
#
# Create the library

add_library(${XdevLSerialLibrary} SHARED
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

target_link_libraries( ${XdevLSerialLibrary} XdevL )

#
# Change the outputname of the library

set_property(TARGET ${XdevLSerialLibrary} PROPERTY PREFIX "${XDEVL_SHARED_LIBRARY_FILE_PREFIX}")

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set_property(TARGET ${XdevLSerialLibrary} PROPERTY SUFFIX "-${XDEVLSERIAL_VERSION}d${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
else()
	set_property(TARGET ${XdevLSerialLibrary} PROPERTY SUFFIX "-${XDEVLSERIAL_VERSION}${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
endif()

install(FILES ${HEADER_INTERFACE_FILE} DESTINATION "${XDEVL_PLUGINS_INCLUDE_INSTALL_PATH}/${PROJECT_NAME}")
install(TARGETS ${XdevLSerialLibrary} DESTINATION ${XDEVL_PLUGINS_INSTALL_PATH})

