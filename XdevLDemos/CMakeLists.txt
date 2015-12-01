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
	${XDEVL_PLUGINS}/XdevLOpenGL
	${Boost_INCLUDE_DIR}
)

link_directories(
	${XDEVL_HOME}/lib
	${XDEVL_EXTERNALS}/lib
	${OGRE_LIBRARY_PATH}
)

if(APPLE)
set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -F${XDEVL_EXTERNALS}/lib" )
endif()

add_executable(udp_server_demo udp_server_demo/udp_server_demo.cpp)
add_executable(udp_client_demo udp_client_demo/udp_client_demo.cpp)
add_executable(udp_server_array_demo udp_server_array_demo/udp_server_array_demo.cpp)
add_executable(udp_client_array_demo udp_client_array_demo/udp_client_array_demo.cpp)
add_executable(tcp_server_demo tcp_server_demo/tcp_server_demo.cpp)
add_executable(tcp_client_demo tcp_client_demo/tcp_client_demo.cpp)

add_executable(devices_demo devices_demo/devices_demo.cpp)
add_executable(events_demo events_demo/events_demo.cpp)
add_executable(audio_demo audio_demo/audio_demo.cpp)
add_executable(filesystem_demo filesystem_demo/filesystem_demo.cpp)
add_executable(xml_demo xml_demo/xml_demo.cpp)
add_executable(opengl_demo opengl_demo/opengl_demo.cpp)
add_executable(glsl_demo glsl_demo/glsl_demo.cpp)
add_executable(ogre_demo ogre_demo/ogre_demo.cpp)

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(LIBS XdevLd)
else()
	set(LIBS XdevL)
endif()

if(APPLE)
	target_link_libraries(audio_demo ${LIBS} )
	target_link_libraries(devices_demo ${LIBS} )
	target_link_libraries(events_demo ${LIBS} )
	target_link_libraries(filesystem_demo ${LIBS} )
	target_link_libraries(glsl_demo ${LIBS} "-framework OpenGL")
	target_link_libraries(opengl_demo ${LIBS} "-framework OpenGL")
	target_link_libraries(ogre_demo ${LIBS} "-framework Ogre" boost_system)
	
	target_link_libraries(udp_server_demo ${LIBS} )
	target_link_libraries(udp_client_demo ${LIBS} )
	target_link_libraries(udp_server_array_demo ${LIBS} )
	target_link_libraries(udp_client_array_demo ${LIBS} )
	target_link_libraries(tcp_server_demo ${LIBS} )
	target_link_libraries(tcp_client_demo ${LIBS} )
	target_link_libraries(xml_demo ${LIBS} )

elseif(UNIX)
	target_link_libraries(udp_server_demo ${LIBS} dl)
	target_link_libraries(udp_client_demo ${LIBS} dl)
	target_link_libraries(udp_server_array_demo ${LIBS} dl)
	target_link_libraries(udp_client_array_demo ${LIBS} dl)
	target_link_libraries(tcp_server_demo ${LIBS} dl)
	target_link_libraries(tcp_client_demo ${LIBS} dl)

	target_link_libraries(devices_demo ${LIBS} dl)
	target_link_libraries(events_demo ${LIBS} dl pthread)
	target_link_libraries(audio_demo ${LIBS} dl openal)
	target_link_libraries(filesystem_demo ${LIBS} dl)
	target_link_libraries(xml_demo ${LIBS} dl pthread)
	target_link_libraries(opengl_demo ${LIBS} GL dl )
	target_link_libraries(glsl_demo ${LIBS} GL dl )
	target_link_libraries(ogre_demo ${LIBS} dl OgreMain boost_system)

elseif(MINGW)
elseif(WIN32)
	target_link_libraries(udp_server_demo ${LIBS} )
	target_link_libraries(udp_client_demo ${LIBS} )
	target_link_libraries(udp_server_array_demo ${LIBS} )
	target_link_libraries(udp_client_array_demo ${LIBS} )
	target_link_libraries(tcp_server_demo ${LIBS} )
	target_link_libraries(tcp_client_demo ${LIBS} )

	target_link_libraries(devices_demo ${LIBS})
	target_link_libraries(events_demo ${LIBS})
#	target_link_libraries(audio_demo ${LIBS} )
	target_link_libraries(filesystem_demo)
	target_link_libraries(xml_demo ${LIBS})
	target_link_libraries(opengl_demo ${LIBS} ${OPENGL_gl_LIBRARY})
	target_link_libraries(glsl_demo ${LIBS} ${OPENGL_gl_LIBRARY})
#	target_link_libraries(ogre_demo ${LIBS})
endif()



