cmake_minimum_required(VERSION 2.8)

project(XdevL)

#
# Set the names for the stuff we are going to create

set(XdevLLibrary XdevL)

#
# Set the version

set(XDEVL_MAJOR_VERSION 0)
set(XDEVL_MINOR_VERSION 5)
set(XDEVL_PATCH_VERSION 0)
set(XDEVL_VERSION "${XDEVL_MAJOR_VERSION}.${XDEVL_MINOR_VERSION}.${XDEVL_PATCH_VERSION}")

add_definitions(
	-DXDEVL_MAJOR_VERSION=${XDEVL_MAJOR_VERSION}
	-DXDEVL_MINOR_VERSION=${XDEVL_MINOR_VERSION} 
	-DXDEVL_PATCH_VERSION=${XDEVL_PATCH_VERSION}
)


#
# Set the output directories

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${XDEVL_HOME}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${XDEVL_HOME}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${XDEVL_HOME}/lib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${XDEVL_HOME}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${XDEVL_HOME}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${XDEVL_HOME}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${XDEVL_HOME}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${XDEVL_HOME}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${XDEVL_HOME}/lib)

#
# Set variable names for the XdevL library

set(SRC_DIR src)
set(HEADERS_DIR include)

#
# Set the output directories for executables and static libraries

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)

if(CMAKE_BUILD_TYPE MATCHES Debug)
	if(UNIX)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC")
	endif()
else()
	if(UNIX)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
	endif()
endif()

#
# Set header files for the XdevL library.

set(XDEVL_HEADER_FILES
	${HEADERS_DIR}/tinydir.h
	${HEADERS_DIR}/tinydir_entity.h
	${HEADERS_DIR}/tinystr.h
	${HEADERS_DIR}/tinyxml.h
	${HEADERS_DIR}/XdevLApplication.h
	${HEADERS_DIR}/XdevL.h
	${HEADERS_DIR}/XdevLVersion.h
	${HEADERS_DIR}/XdevLID.h
	${HEADERS_DIR}/XdevLArrayModifier.h	
	${HEADERS_DIR}/XdevLClassifier.h
	${HEADERS_DIR}/XdevLCommandLine.h
	${HEADERS_DIR}/XdevLCore.h
	${HEADERS_DIR}/XdevLCoreMediator.h
	${HEADERS_DIR}/XdevLDelegate.h
	${HEADERS_DIR}/XdevLDynLib.h
	${HEADERS_DIR}/XdevLEvent.h
	${HEADERS_DIR}/XdevLError.h
	${HEADERS_DIR}/XdevLInputSystem.h
	${HEADERS_DIR}/XdevLInputSystemUtils.h
	${HEADERS_DIR}/XdevLListener.h
	${HEADERS_DIR}/XdevLLog.h
	${HEADERS_DIR}/XdevLManagedBuffer.h	
	${HEADERS_DIR}/XdevLModule.h
	${HEADERS_DIR}/XdevLMutex.h
	${HEADERS_DIR}/XdevLConditionalVariable.h
	${HEADERS_DIR}/XdevLPlatform.h
	${HEADERS_DIR}/XdevLPlugin.h
	${HEADERS_DIR}/XdevLPluginImpl.h
	${HEADERS_DIR}/XdevLRingBuffer.h
	${HEADERS_DIR}/XdevLSemaphore.h
	${HEADERS_DIR}/XdevLSharedMemory.h
	${HEADERS_DIR}/XdevLStream.h
	${HEADERS_DIR}/XdevLThread.h
	${HEADERS_DIR}/XdevLTimer.h
	${HEADERS_DIR}/XdevLTypes.h
	${HEADERS_DIR}/XdevLUtils.h
	${HEADERS_DIR}/XdevLXfstring.h
	${HEADERS_DIR}/XdevLXstring.h
)

#
# Set the source files for the bakka library.

set(XDEVL_SOURCE_FILES
	${SRC_DIR}/tinydir.cpp
	${SRC_DIR}/tinydir_entity.cpp
	${SRC_DIR}/tinystr.cpp
	${SRC_DIR}/tinyxml.cpp
	${SRC_DIR}/tinyxmlerror.cpp
	${SRC_DIR}/tinyxmlparser.cpp
	${SRC_DIR}/XdevL.cpp
	${SRC_DIR}/XdevLDynLib.cpp
	${SRC_DIR}/XdevLMutex.cpp
	${SRC_DIR}/XdevLLog.cpp
	${SRC_DIR}/XdevLConditionalVariable.cpp
	${SRC_DIR}/XdevLThread.cpp
	${SRC_DIR}/XdevLTimer.cpp
	${SRC_DIR}/XdevLUtils.cpp
)


#
# Create the XdevL library

add_library(${XdevLLibrary} STATIC
    ${XDEVL_HEADER_FILES}
    ${XDEVL_SOURCE_FILES}
)

#
# Set the compiler property
set_property(TARGET ${XdevLLibrary} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${XdevLLibrary} PROPERTY CXX_STANDARD 11)

#
# Add include directories

include_directories( 
	${HEADERS_DIR} 
	${Boost_INCLUDE_DIRS} 
	${XDEVL_PLUGINS}
	)


#
# Add additional libraries to link

target_link_libraries(${XdevLLibrary}

)


if(CMAKE_BUILD_TYPE MATCHES Debug)
	if(COMPILER_GCC OR COMPILER_CLANG)
		set_property(TARGET ${XdevLLibrary} PROPERTY SUFFIX "d.a")
	elseif(COMPILER_VISUAL_STUDIO)
		set_property(TARGET ${XdevLLibrary} PROPERTY SUFFIX "d.lib")
	endif()
else()
	if(COMPILER_GCC OR COMPILER_CLANG)
		set_property(TARGET ${XdevLLibrary} PROPERTY SUFFIX ".a")
	elseif(COMPILER_VISUAL_STUDIO)
		set_property(TARGET ${XdevLLibrary} PROPERTY SUFFIX ".lib")
	endif()
endif()
