cmake_minimum_required(VERSION 2.6.2)

project(tcp_client_demo)

include_directories(
    $(XDEVL_HOME)/include
    $(XDEVL_PLUGINS)
)

set(CMAKE_CXXFLAGS "${CMAKE_CXXFLAGS}  -O2 -m64")

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  -m64")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" -L\"$(XDEVL_HOME)/lib\" -L\"$(XDEVL_PLUGINS)\" ")

set(SRCS
    tcp_client_demo.cpp
)

add_executable(tcp_client_demo ${SRCS})

target_link_libraries(tcp_client_demo
    XdevL
    dl
)

