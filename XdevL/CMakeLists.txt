cmake_minimum_required(VERSION 2.8)

project(XdevL)

#
# Set the names for the stuff we are going to create

set(XdevLLibrary XdevL)

#
# Set the version

set(XDEVL_MAJOR_VERSION ${XDEVLSDK_MAJOR_VERSION})
set(XDEVL_MINOR_VERSION ${XDEVLSDK_MINOR_VERSION})
set(XDEVL_PATCH_VERSION ${XDEVLSDK_PATCH_VERSION})
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

set(SRC_DIR ${XdevL_SOURCE_DIR}/src)
set(HEADERS_DIR ${XdevL_SOURCE_DIR}/include)

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
	${HEADERS_DIR}/XdevLSignalSlot.h
	${HEADERS_DIR}/XdevLProfiler.h
)

if(PLATFORM_UNIX)
	set(XDEVL_HEADER_FILES ${XDEVL_HEADER_FILES} ${HEADERS_DIR}/XdevL/Unix/XdevLDynLibUnix.h)
	set(XDEVL_HEADER_FILES ${XDEVL_HEADER_FILES} ${HEADERS_DIR}/XdevL/Unix/XdevLThreadUnix.h)
	set(XDEVL_HEADER_FILES ${XDEVL_HEADER_FILES} ${HEADERS_DIR}/XdevL/Unix/XdevLMutexUnix.h)
elseif(PLATFORM_WINDOWS)
	set(XDEVL_HEADER_FILES ${XDEVL_HEADER_FILES} ${HEADERS_DIR}/XdevL/Windows/XdevLDynLibWindows.h)
	set(XDEVL_HEADER_FILES ${XDEVL_HEADER_FILES} ${HEADERS_DIR}/XdevL/Windows/XdevLThreadWindows.h)
	set(XDEVL_HEADER_FILES ${XDEVL_HEADER_FILES} ${HEADERS_DIR}/XdevL/Unix/XdevLMutexWindows.h)
endif()

#
# Set the source files for the bakka library.

set(XDEVL_SOURCE_FILES
	${SRC_DIR}/tinystr.cpp
	${SRC_DIR}/tinyxml.cpp
	${SRC_DIR}/tinyxmlerror.cpp
	${SRC_DIR}/tinyxmlparser.cpp
	${SRC_DIR}/XdevL.cpp
	${SRC_DIR}/XdevLLog.cpp
	${SRC_DIR}/XdevLConditionalVariable.cpp
	${SRC_DIR}/XdevLThread.cpp
	${SRC_DIR}/XdevLUtils.cpp
	${SRC_DIR}/XdevLProfiler.cpp
	${SRC_DIR}/XdevLTimer.cpp
)

if(PLATFORM_UNIX)
	set(XDEVL_SOURCE_FILES ${XDEVL_SOURCE_FILES} ${SRC_DIR}/Unix/XdevLDynLibUnix.cpp)
	set(XDEVL_SOURCE_FILES ${XDEVL_SOURCE_FILES} ${SRC_DIR}/Unix/XdevLTimerUnix.cpp)
	set(XDEVL_SOURCE_FILES ${XDEVL_SOURCE_FILES} ${SRC_DIR}/Unix/XdevLThreadUnix.cpp)
	set(XDEVL_SOURCE_FILES ${XDEVL_SOURCE_FILES} ${SRC_DIR}/Unix/XdevLMutexUnix.cpp)
elseif(PLATFORM_WINDOWS)
	set(XDEVL_SOURCE_FILES ${XDEVL_SOURCE_FILES} ${SRC_DIR}/Unix/XdevLDynLibWindows.cpp)
	set(XDEVL_SOURCE_FILES ${XDEVL_SOURCE_FILES} ${SRC_DIR}/Unix/XdevLTimerWindows.cpp)
	set(XDEVL_SOURCE_FILES ${XDEVL_SOURCE_FILES} ${SRC_DIR}/Unix/XdevLThreadWindows.cpp)
	set(XDEVL_SOURCE_FILES ${XDEVL_SOURCE_FILES} ${SRC_DIR}/Unix/XdevLMutexWindows.cpp)
endif()

#
# Create the XdevL library

add_library(${XdevLLibrary} STATIC
    ${XDEVL_HEADER_FILES}
    ${XDEVL_SOURCE_FILES}
)

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

install(TARGETS ${XdevLLibrary} DESTINATION ${XDEVL_LIB_INSTALL_PATH})
install(FILES ${XDEVL_HEADER_FILES} DESTINATION ${XDEVL_INCLUDE_INSTALL_PATH})
