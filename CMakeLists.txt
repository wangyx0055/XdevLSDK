cmake_minimum_required(VERSION 2.8)

project(XdevLSDK)

#set(CMAKE_DEBUG_POSTFIX d)

#
# Set the main variables that holds project folders
#

set(CMAKE_MODULE_PATH "${XdevLSDK_SOURCE_DIR}/cmake")
set(XDEVL_SDK ${XdevLSDK_SOURCE_DIR})
set(XDEVL_HOME ${XDEVL_SDK}/XdevL)
set(XDEVL_PLUGINS ${XDEVL_SDK}/XdevLPlugins)
set(XDEVL_DEMOS ${XDEVL_SDK}/XdevLDemos)
set(XDEVL_EXTERNALS ${XDEVL_SDK}/XdevLExternals)
set(XDEVL_SHARED_LIBRARY_FILE_EXTENSION ".xdp")
#
# Set the XdevL version
#

set(XDEVLSDK_MAJOR_VERSION 0)
set(XDEVLSDK_MINOR_VERSION 5)
set(XDEVLSDK_MICRO_VERSION 0)
set(XDEVLSDK_VERSION "${XDEVLSDK_MAJOR_VERSION}.${XDEVLSDK_MINOR_VERSION}.${XDEVLSDK_MICRO_VERSION}")


#
# Select plugins to compile.
#

option(BUILD_NETWORK "Builds the Network plugin" ON)
option(BUILD_KEYBOARD "Builds the Keyboard plugin" ON)
option(BUILD_MOUSE "Builds the Mouse plugin" ON)
option(BUILD_JOYSTICK "Builds the Joystick plugin" ON)
option(BUILD_SERIAL "Builds the serial port plugin" ON)
option(BUILD_BLUETOOTH "Builds the bluetooth plugin" ON)
option(BUILD_FTDI "Builds the FTDI Serial Port plugin" OFF)
option(BUILD_AUDIO "Builds the audio plugin" ON)
option(BUILD_OPENGL "Builds the OpenGL plugin" ON)
option(BUILD_SDL "Builds the SDL plugin" ON)
option(BUILD_OGRE "Builds the ogre plugin" ON)
option(BUILD_GL_CONTEXT "Builds the new OpenGL context plugin" ON)
option(BUILD_WAYLAND "Builds the Wayland plugin" ON)
option(BUILD_X11 "Builds the X11 plugin" ON)
option(BUILD_MOUSE_SERVER "Builds the raw mouse server" ON)
option(BUILD_FILESYSTEM "Builds the Directory module" ON)

#
# Check for 64 or 32 bit
#

set(SIZEOF_VOIDP ${CMAKE_SIZEOF_VOID_P})
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(ARCH_64 TRUE)
  set(PROCESSOR_ARCH "x64")
else()
  set(ARCH_64 FALSE)
  set(PROCESSOR_ARCH "x86")
endif()


#
# Check compiler
#

if(CMAKE_COMPILER_IS_GNUCC)
  set(COMPILER_GCC TRUE)
elseif(CMAKE_C_COMPILER_ID MATCHES "Clang")
  set(COMPILER_CLANG TRUE)
elseif(MSVC_VERSION GREATER 1400) # VisualStudio 8.0+
  set(COMPILER_VISUAL_STUDIO TRUE)
endif()


#
# Check for platform
#

if(WIN32)
  if(NOT WINDOWS)
    set(PLATFORM_WINDOWS TRUE)
  endif()
elseif(UNIX AND NOT APPLE)
  if(CMAKE_SYSTEM_NAME MATCHES ".*Linux")
    set(PLATFORM_LINUX TRUE)
  endif()
elseif(APPLE)
  if(CMAKE_SYSTEM_NAME MATCHES ".*Darwin.*")
    set(PLATFORM_DARWIN TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES ".*MacOS.*")
    set(PLATFORM_MACOSX TRUE)
  endif()
endif()


if(COMPILER_GCC OR COMPILER_CLANG)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_VISUAL_STUDIO)
endif()

#
# TODO Do it the cmake way. This is a hack.
# Set external libraries

if(APPLE)
	set(CMAKE_MACOSX_RPATH OFF)
elseif(WIN32)
	set(DirectX_INCLUDEDIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include")
	set(DirectX_LIBRARYDIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64")
endif()

#
# This definition we need for the XdevL library

if(CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(-D_LOG -DXDEVL_DEBUG -DXDEVL_SHARED_LIBRARY_FILE_EXTENSION)
else()
	add_definitions(-D_LOG -DXDEVL_SHARED_LIBRARY_FILE_EXTENSION)
endif()


# Build the libraries and plugins.
add_subdirectory(XdevL)
add_subdirectory(XdevLPlugins/XdevLCore)

# Build the Keybaord plugin.
if(BUILD_KEYBOARD)
	add_subdirectory(XdevLPlugins/XdevLKeyboard)
endif()

# Build the Mouse plugin.
if(BUILD_MOUSE)
	add_subdirectory(XdevLPlugins/XdevLMouse)
endif()

# Build the Joystick plugin.
if(BUILD_JOYSTICK)
	add_subdirectory(XdevLPlugins/XdevLJoystick)
endif()

# Build the Network plugin.
if(BUILD_NETWORK)
	add_subdirectory(XdevLPlugins/XdevLNetwork)
endif()

# Build the Directory plugin.
if(BUILD_FILESYSTEM)
	add_subdirectory(XdevLPlugins/XdevLFileSystem)
endif()

# Build the Serial Port Communication plugin.
if(BUILD_SERIAL)
	add_subdirectory(XdevLPlugins/XdevLSerial)
endif()

# Build the Bluetooth Port Communication plugin.
if(BUILD_BLUETOOTH)
	add_subdirectory(XdevLPlugins/XdevLBluetooth)
endif()

# Build the OpenGL plugin.
if(BUILD_OPENGL)
	add_subdirectory(XdevLPlugins/XdevLOpenGL)
endif()

# Build the SDL plugin.
if(BUILD_SDL)
	add_subdirectory(XdevLPlugins/XdevLWindow/XdevLWindowSDL)
endif()

# Build the Audio plugin.
if(BUILD_AUDIO)
	add_subdirectory(XdevLPlugins/XdevLAudio)
endif()

# Build the ogre plugin.
if(BUILD_OGRE)
	add_subdirectory(XdevLPlugins/XdevLOgre)
endif()

# Build the FTDI Serial Port plugin.
if(BUILD_FTDI)
	add_subdirectory(XdevLPlugins/XdevLFTDI)
endif()

# Build the Wayland Window plugin.
if(BUILD_WAYLAND)
	add_subdirectory(XdevLPlugins/XdevLWindow/XdevLWindowWayland)
endif()

# Build the X11 Window plugin.
if(BUILD_X11)
	add_subdirectory(XdevLPlugins/XdevLWindow/XdevLWindowX11)
endif()

# Build the Mouse Server plugins for raw mouse events.
if(BUILD_MOUSE_SERVER)
	if(PLATFORM_WINDOWS)
	elseif(PLATFORM_LINUX)
		add_subdirectory(XdevLPlugins/XdevLMouseServerLinux)
	elseif(PLATFORM_MACOSX)
	endif()
endif()


# Build the new OpenGL Contect plugin.
#if(BUILD_GL_CONTEXT)
	if(WIN32)
		add_subdirectory(XdevLPlugins/XdevLOpenGLContext/XdevLOpenGLContextWGL)
	elseif(APPLE)
		add_subdirectory(XdevLPlugins/XdevLOpenGLContext/XdevLOpenGLContextCocoa)
	elseif(UNIX)
		add_subdirectory(XdevLPlugins/XdevLOpenGLContext/XdevLOpenGLContextGLX)
	endif()
#endif()


add_subdirectory(XdevLDemos)
