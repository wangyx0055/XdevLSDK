cmake_minimum_required(VERSION 2.8)

project(XdevLBluetooth)

#
# Set the names for the stuff we are going to create
#

set(XdevLBluetoothLibrary XdevLBluetooth)

#
# Set the Plugin version
#

set(XDEVLBLUETOOTH_MAJOR_VERSION 0)
set(XDEVLBLUETOOTH_MINOR_VERSION 1)
set(XDEVLBLUETOOTH_PATCH_VERSION 0)
set(XDEVLBLUETOOTH_VERSION "${XDEVLBLUETOOTH_MAJOR_VERSION}.${XDEVLBLUETOOTH_MINOR_VERSION}.${XDEVLBLUETOOTH_PATCH_VERSION}")

add_definitions(
	-DXDEVLBLUETOOTH_MAJOR_VERSION=${XDEVLBLUETOOTH_MAJOR_VERSION}
	-DXDEVLBLUETOOTH_MINOR_VERSION=${XDEVLBLUETOOTH_MINOR_VERSION} 
	-DXDEVLBLUETOOTH_PATCH_VERSION=${XDEVLBLUETOOTH_PATCH_VERSION}
)

#
# Set the AL Module version
#

set(XDEVLBLUETOOTH_MODULE_MAJOR_VERSION 0)
set(XDEVLBLUETOOTH_MODULE_MINOR_VERSION 1)
set(XDEVLBLUETOOTH_MODULE_PATCH_VERSION 0)
set(XDEVLBLUETOOTH_MODULE_VERSION "${XDEVLBLUETOOTH_MODULE_MAJOR_VERSION}.${XDEVLBLUETOOTH_MODULE_MINOR_VERSION}.${XDEVLBLUETOOTH_MODULE_PATCH_VERSION}")

add_definitions(
	-DXDEVLBLUETOOTH_MODULE_MAJOR_VERSION=${XDEVLBLUETOOTH_MODULE_MAJOR_VERSION}
	-DXDEVLBLUETOOTH_MODULE_MINOR_VERSION=${XDEVLBLUETOOTH_MODULE_MINOR_VERSION} 
	-DXDEVLBLUETOOTH_MODULE_PATCH_VERSION=${XDEVLBLUETOOTH_MODULE_PATCH_VERSION}
)

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
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE .${XDEVL_PLUGINS}/lib)

#
# Set header files for the library.
#

set(HEADER_INTERFACE_FILE ${HEADERS_DIR}/XdevLBluetooth.h)

set(HEADER_FILES
	${HEADER_INTERFACE_FILE}
)

#
# Set the source files for the library.
#
set(SOURCE_FILES

)

if(APPLE)


	set(HEADER_FILES ${HEADER_FILES}
		${HEADERS_DIR}/XdevLBluetoothMacOSXImpl.h
	)

	set(SOURCE_FILES ${SOURCE_FILES}
		${SRC_DIR}/XdevLBluetoothMacOSXImpl.cpp
	)

elseif(UNIX)
	set(HEADER_FILES ${HEADER_FILES}
		${HEADERS_DIR}/XdevLBluetoothLinuxImpl.h
	)

	set(SOURCE_FILES ${SOURCE_FILES}
		${SRC_DIR}/XdevLBluetoothLinuxImpl.cpp
	)
elseif(WIN32)
	set(HEADER_FILES ${HEADER_FILES}
		${HEADERS_DIR}/XdevLBluetoothWinImpl.h
	)

	set(SOURCE_FILES ${SOURCE_FILES}
		${SRC_DIR}/XdevLBluetoothWinImpl.cpp
	)

endif()


#
# Add external libraries folder
#
link_directories(${XdevL_BINARY_DIR}/lib)

#
# Create the library
#
add_library(${XdevLBluetoothLibrary} SHARED
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
)

#
# Add external ibraries
#
if(APPLE)
	if(CMAKE_BUILD_TYPE MATCHES Debug)
		target_link_libraries(${XdevLBluetoothLibrary}
			XdevLd
			"-framework IOBluetooth"
			"-framework CoreFoundation"
		)
	else()
		target_link_libraries(${XdevLBluetoothLibrary}
			XdevL
			"-framework IOBluetooth"
			"-framework CoreFoundation"
		)
	endif()
elseif(UNIX)
	if(CMAKE_BUILD_TYPE MATCHES Debug)
		target_link_libraries(${XdevLBluetoothLibrary}
			XdevLd
		)
	else()
		target_link_libraries(${XdevLBluetoothLibrary}
			XdevL
		)
	endif()
elseif(WIN32)
	if(CMAKE_BUILD_TYPE MATCHES Debug)
		target_link_libraries(${XdevLBluetoothLibrary}
			XdevLd
			ws2_32
		)
	else()
		target_link_libraries(${XdevLBluetoothLibrary}
			XdevL
			ws2_32
		)
	endif()
endif()

#
# Change the outputname of the library

set_property(TARGET ${XdevLBluetoothLibrary} PROPERTY PREFIX "${XDEVL_SHARED_LIBRARY_FILE_PREFIX}")

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set_property(TARGET ${XdevLBluetoothLibrary} PROPERTY SUFFIX "-${XDEVLBLUETOOTH_VERSION}d${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
else()
	set_property(TARGET ${XdevLBluetoothLibrary} PROPERTY SUFFIX "-${XDEVLBLUETOOTH_VERSION}${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
endif()

install(FILES ${HEADER_INTERFACE_FILE} DESTINATION "${XDEVL_PLUGINS_INCLUDE_INSTALL_PATH}/${PROJECT_NAME}")
install(TARGETS ${XdevLBluetoothLibrary} DESTINATION ${XDEVL_PLUGINS_INSTALL_PATH})
