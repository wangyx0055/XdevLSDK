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
set(XDEVL_TOOLS ${XDEVL_SDK}/XdevLUtils)
set(XDEVL_EXTERNALS ${XDEVL_SDK}/XdevLExternals)
set(XDEVL_SHARED_LIBRARY_FILE_EXTENSION ".xdp")

# Where shall we install the libs, headers and plugins?
set(XDEVL_INCLUDE_INSTALL_PATH "/usr/include/XdevL")
set(XDEVL_LIB_INSTALL_PATH "/usr/lib")
set(XDEVL_PLUGINS_INCLUDE_INSTALL_PATH "/usr/include/XdevLPlugins")
set(XDEVL_PLUGINS_INSTALL_PATH "/usr/lib/XdevLPlugins")

#
# Set the XdevL version
#

set(XDEVLSDK_MAJOR_VERSION 0)
set(XDEVLSDK_MINOR_VERSION 5)
set(XDEVLSDK_PATCH_VERSION 1)
set(XDEVLSDK_VERSION "${XDEVLSDK_MAJOR_VERSION}.${XDEVLSDK_MINOR_VERSION}.${XDEVLSDK_MICRO_VERSION}")

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

message(STATUS ${CMAKE_SYSTEM_NAME})

if(COMPILER_GCC OR COMPILER_CLANG)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	set(CMAKE_CXX_STANDARD 11 )
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
# Select plugins to compile.
#

option(BUILD_NETWORK "Builds the Network plugin" ON)
option(BUILD_KEYBOARD "Builds the Keyboard plugin" ON)
option(BUILD_MOUSE "Builds the Mouse plugin" ON)
option(BUILD_JOYSTICK "Builds the Joystick plugin" ON)
option(BUILD_SERIAL "Builds the serial port plugin" ON)
option(BUILD_BLUETOOTH "Builds the bluetooth plugin" ON)
option(BUILD_FTDI "Builds the FTDI Serial Port plugin" OFF)
option(BUILD_NATIVE_AUDIO "Builds the audio plugin" ON)
option(BUILD_OPENGL "Builds the OpenGL plugin" ON)
option(BUILD_FONT "Builds the Font plugin" ON)
option(BUILD_SDL "Builds the SDL plugin" ON)
option(BUILD_OGRE "Builds the ogre plugin" ON)
option(BUILD_GL_CONTEXT "Builds the new OpenGL context plugin" ON)
option(BUILD_WAYLAND "Builds the Wayland plugin" ON)
option(BUILD_MOUSE_SERVER "Builds the raw mouse server" ON)
option(BUILD_JOYSTICK_SERVER "Builds the raw joystick server" ON)
option(BUILD_FILESYSTEM "Builds the Directory module" ON)
option(BUILD_NATIVE_WINDOWS "Build native window support" ON)
option(BUILD_DEMOS "Builds all demos" ON)
option(BUILD_EGL "Builds the EGL OpenGL context" ON)


if(NOT PLATFORM_LINUX)
	set(BUILD_X11 OFF)
	set(BUILD_WAYLAND OFF)
	set(BUILD_X11 OFF)
	set(BUILD_MOUSE_SERVER OFF)
	set(BUILD_FTDI OFF)
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
	add_subdirectory(XdevLPlugins/XdevLInput/XdevLKeyboard)
endif()

# Build the Mouse plugin.
if(BUILD_MOUSE)
	add_subdirectory(XdevLPlugins/XdevLInput/XdevLMouse)
endif()

# Build the Joystick plugin.
if(BUILD_JOYSTICK)
	if(WIN32)
	elseif(APPLE)
			add_subdirectory(XdevLPlugins/XdevLInput/XdevLJoystick)
			add_subdirectory(XdevLPlugins/XdevLInput/XdevLJoystick/XdevLJoystickServerMac)
	elseif(UNIX)
		find_package(UDev)
		if(UDEV_FOUND)
			add_subdirectory(XdevLPlugins/XdevLInput/XdevLJoystick)
			add_subdirectory(XdevLPlugins/XdevLInput/XdevLJoystick/XdevLJoystickServerLinux)
		else()
			message(STATUS "XdevLJoystickServerLinux and XdevLJoystick will not be build.")
		endif()
	endif()
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
	find_package(Bluetooth)
	if(Bluetooth_FOUND)
		add_subdirectory(XdevLPlugins/XdevLBluetooth)
	else()
		message(STATUS "XdevLBluetooth will not be build.")
	endif()
endif()

add_subdirectory(XdevLPlugins/XdevLBlankPlugin)

# Build the Audio plugin.
if(BUILD_NATIVE_AUDIO)
	if(WIN32)
		# TODO Add here window audio handling
	elseif(APPLE)
		add_subdirectory(XdevLPlugins/XdevLAudio/XdevLAudioCoreAudio)
	elseif(UNIX)
		find_package(ALSA)
		if(ALSA_FOUND)		
			add_subdirectory(XdevLPlugins/XdevLAudio/XdevLAudioAlsa)
		else()
			message(WARNING "XdevLAudioAlsa will not be build.")
		endif()
	endif()
else()
	find_package(OpenAL)
	if(OPENAL_FOUND)
		add_subdirectory(XdevLPlugins/XdevLAudio/XdevLAudioOpenAL)
	else()
		message(STATUS "XdevLAudioOpenAL will not be build.")
	endif()
endif()

# Build the OpenGL plugin.
if(BUILD_OPENGL)
	find_package(OpenGL)
	if(OPENGL_FOUND)
		add_subdirectory(XdevLPlugins/XdevLRAI/XdevLRAIGL)
	else()
		message(STATUS "XdevLRAIGL will not be build.")
	endif()
endif()

# Build the Font plugin.
if(BUILD_FONT)
	add_subdirectory(XdevLPlugins/XdevLFont)
endif()

# Build the SDL plugin.
if(BUILD_SDL)
	find_package(SDL2)
	if(SDL2_FOUND)
		add_subdirectory(XdevLPlugins/XdevLWindow/XdevLWindowSDL)
	else()
		message(WARNING "XdevLWindowSDL will not be build.")
	endif()
endif()

#Build native window plugins.
if(BUILD_NATIVE_WINDOWS)
	if(WIN32)
		add_subdirectory(XdevLPlugins/XdevLWindow/XdevLWindowWin32)
	elseif(APPLE)
		add_subdirectory(XdevLPlugins/XdevLWindow/XdevLWindowCocoa)
	elseif(UNIX)
		find_package(Xrandr)
		if(Xrandr_FOUND)
			add_subdirectory(XdevLPlugins/XdevLWindow/XdevLWindowX11)
		else()
			message(WARNING "XdevLWindowX11 will not be build.")
		endif()

		if(BUILD_WAYLAND)
			find_package(Wayland)
			if(Wayland_FOUND)
				add_subdirectory(XdevLPlugins/XdevLWindow/XdevLWindowWayland)
			else()
				message(STATUS "XdevLWindowWayland will not be build.")
			endif()
		endif()
	endif()
endif()

# Build the ogre plugin.
if(BUILD_OGRE)
	find_package(OGRE)
	find_package(Boost COMPONENTS system)

	if(OGRE_FOUND AND Boost_FOUND)
		add_subdirectory(XdevLPlugins/XdevLOgre)
	else()
		message(STATUS "XdevLOgre will not be build.")
	endif()
endif()

# Build the FTDI Serial Port plugin.
if(BUILD_FTDI)
	add_subdirectory(XdevLPlugins/XdevLFTDI)
endif()


# Build the Mouse Server plugins for raw mouse events.
if(BUILD_MOUSE_SERVER)
	if(PLATFORM_WINDOWS)
	elseif(PLATFORM_LINUX)
		add_subdirectory(XdevLPlugins/XdevLInput/XdevLMouse/XdevLMouseServerLinux)
	elseif(PLATFORM_MACOSX)
	endif()
endif()

# Build the new OpenGL Contect plugin.
if(BUILD_GL_CONTEXT)
	find_package(EGL)
	if(EGL_FOUND)
		add_subdirectory(XdevLPlugins/XdevLOpenGLContext/XdevLOpenGLContextEGL)
	endif()
		
	if(OPENGL_FOUND)
		find_package(SDL2)
		if(SDL2_FOUND)
			add_subdirectory(XdevLPlugins/XdevLOpenGLContext/XdevLOpenGLContextSDL)
		else()
			message(STATUS "XdevLOpenGLContextSDL will not be build.")
		endif()
		
		if(WIN32)
			add_subdirectory(XdevLPlugins/XdevLOpenGLContext/XdevLOpenGLContextWGL)
		elseif(APPLE)
			add_subdirectory(XdevLPlugins/XdevLOpenGLContext/XdevLOpenGLContextCocoa)
		elseif(UNIX)
			add_subdirectory(XdevLPlugins/XdevLOpenGLContext/XdevLOpenGLContextGLX)
		endif()
	else()
		message(STATUS "XdevLOpenGLContextSDL, XdevLOpenGLContextWGL, XdevLOpenGLContextCocoa or XdevLOpenGLContextGLX will not be build.")
	endif()
endif()

#if(BUILD_DEMOS)
#	add_subdirectory(XdevLDemos)
#endif()

add_subdirectory(XdevLUtils/XdevLBT)
add_subdirectory(XdevLUtils/XdevLCodeLiteGenerator)
add_subdirectory(XdevLPlugins/XdevLImage/XdevLImagePluginBMP)

# All packaging stuff

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Cengiz Terzibas") #required
SET(CPACK_PACKAGE_VENDOR "Cengiz Terzibas")
SET(CPACK_PACKAGE_VERSION ${XDEVLSDK_VERSION})
SET(CPACK_PACKAGE_VERSION_MAJOR ${XDEVLSDK_MAJOR_VERSION})
SET(CPACK_PACKAGE_VERSION_MINOR ${XDEVLSDK_MINOR_VERSION})
SET(CPACK_PACKAGE_VERSION_PATCH ${XDEVLSDK_PATCH_VERSION})
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The XdevLSDK framework.")

include(CPack)
