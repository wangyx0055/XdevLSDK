project(XdevLDemos)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${XDEVL_DEMOS}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${XDEVL_DEMOS}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${XDEVL_DEMOS}/bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${XDEVL_DEMOS}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG  ${XDEVL_DEMOS}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${XDEVL_DEMOS}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${XDEVL_DEMOS}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  ${XDEVL_DEMOS}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${XDEVL_DEMOS}/bin)

set(CMAKE_DEBUG_POSTFIX d)


include_directories(
	${XDEVL_HOME}/include
	${XDEVL_PLUGINS}
	${XDEVL_PLUGINS}/XdevLRAI
)

link_directories(
	${XDEVL_HOME}/lib
	${XDEVL_EXTERNALS}/lib
	${OGRE_LIBRARY_PATH}
)

if(APPLE)
set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -F${XDEVL_EXTERNALS}/lib" )
endif()


if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(LIBS XdevLd)
else()
	set(LIBS XdevL)
endif()

if(PLATFORM_LINUX)
	set(LIBS ${LIBS} dl pthread)
endif()


add_executable(udp_server_demo udp_server_demo/udp_server_demo.cpp)
add_executable(udp_client_demo udp_client_demo/udp_client_demo.cpp)
add_executable(udp_server_array_demo udp_server_array_demo/udp_server_array_demo.cpp)
add_executable(udp_client_array_demo udp_client_array_demo/udp_client_array_demo.cpp)
add_executable(tcp_server_demo tcp_server_demo/tcp_server_demo.cpp)
add_executable(tcp_client_demo tcp_client_demo/tcp_client_demo.cpp)

add_executable(devices_demo devices_demo/devices_demo.cpp)
add_executable(events_demo events_demo/events_demo.cpp)
add_executable(filesystem_demo filesystem_demo/filesystem_demo.cpp)
add_executable(xml_demo xml_demo/xml_demo.cpp)

if(OPENAL_FOUND)
	add_subdirectory(audio_demo)
else()
	message(STATUS "audio_demo will not be build.")
endif()

if(OPENGL_FOUND)
	add_subdirectory(rai_demo)
else()
	message(STATUS "rai_demo will not be build.")
endif()

if(OPENGL_FOUND)
	add_subdirectory(glsl_demo)
else()
	message(STATUS "glsl_demo will not be build.")
endif()

if(OGRE_FOUND AND Boost_FOUND)
	add_subdirectory(ogre_demo)
else()
	message(STATUS "ogre_demo will not be build.")
endif()


target_link_libraries(udp_server_demo ${LIBS})
target_link_libraries(udp_client_demo ${LIBS})
target_link_libraries(udp_server_array_demo ${LIBS})
target_link_libraries(udp_client_array_demo ${LIBS})
target_link_libraries(tcp_server_demo ${LIBS})
target_link_libraries(tcp_client_demo ${LIBS})

target_link_libraries(devices_demo ${LIBS})
target_link_libraries(events_demo ${LIBS})
target_link_libraries(filesystem_demo ${LIBS})
target_link_libraries(xml_demo ${LIBS})

