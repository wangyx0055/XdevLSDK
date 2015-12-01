# - Find the Wayland include file and library
#

SET(Wayland_INC_SEARCH_PATH
    /usr/include
    /usr/local/include
    /usr/include/Wayland
)

SET(Wayland_LIB_SEARCH_PATH
    /usr/local/lib
    /usr/lib
)


FIND_PATH(Wayland_INCLUDE_DIR wayland.h
          ${Wayland_INC_SEARCH_PATH})

FIND_LIBRARY(Wayland_LIBRARIES NAMES wayland PATH ${Wayland_LIB_SEARCH_PATH})

IF (Wayland_INCLUDE_DIR AND Wayland_LIBRARIES)
    SET(Wayland_FOUND TRUE)
ENDIF (Wayland_INCLUDE_DIR AND Wayland_LIBRARIES)

IF (Wayland_FOUND)
    INCLUDE(CheckLibraryExists)

    CHECK_LIBRARY_EXISTS(${Wayland_LIBRARIES}
                         ${Wayland_LIBRARIES}
                         Wayland_HAS_CONFIG)

    IF (NOT Wayland_HAS_CONFIG AND Wayland_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could NOT find Wayland")
    ENDIF (NOT Wayland_HAS_CONFIG AND Wayland_FIND_REQUIRED)
ENDIF (Wayland_FOUND)

MARK_AS_ADVANCED(
    Wayland_INCLUDE_DIR
    Wayland_LIBRARIES
    )