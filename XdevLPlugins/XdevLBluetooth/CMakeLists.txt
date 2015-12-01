cmake_minimum_required(VERSION 2.8)

project(XdevLBluetooth)

#
# Set the names for the stuff we are going to create
#

set(XdevLBluetoothLibrary XdevLBluetooth)

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
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE .${XDEVL_PLUGINS}/lib)

#
# Set header files for the library.
#

set(HEADER_FILES
	${HEADERS_DIR}/XdevLBluetooth.h
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
#
set_property(TARGET ${XdevLBluetoothLibrary} PROPERTY PREFIX "")

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set_property(TARGET ${XdevLBluetoothLibrary} PROPERTY SUFFIX "-${VERSION}d${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
else()
	set_property(TARGET ${XdevLBluetoothLibrary} PROPERTY SUFFIX "-${VERSION}${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}")
endif()
