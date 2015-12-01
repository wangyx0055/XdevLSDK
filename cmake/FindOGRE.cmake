if(APPLE)
	find_path(OGRE_INCLUDE_DIR OGRE/Ogre.h
	  HINTS
	  PATH_SUFFIXES include
	  PATHS
	 ${XDEVL_EXTERNALS}/include
	)

	find_library(OGRE_LIBRARY
	  NAMES Ogre OgreMain OgreOverlay OgrePaging
	  HINTS
	  PATH_SUFFIXES Release
	  PATHS
	  ${XDEVL_EXTERNALS}/lib
	)
elseif(UNIX)
	find_path(OGRE_INCLUDE_DIR OGRE/Ogre.h
	  HINTS
	  PATH_SUFFIXES include
	  PATHS
	  /usr/
	)

	find_library(OGRE_LIBRARY
	  NAMES Ogre OgreMain OgreOverlay OgrePaging
	  HINTS
	  PATH_SUFFIXES x86_64-linux-gnu
	  PATHS
	  /usr/lib/
	)
endif()


include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OGRE  DEFAULT_MSG  OGRE_LIBRARY OGRE_INCLUDE_DIR)
mark_as_advanced(OGRE_LIBRARY OGRE_INCLUDE_DIR)
