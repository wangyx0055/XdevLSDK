if(APPLE)
	set(SDL2_SEARCH_PATHS
		~/Library/Frameworks
		/Library/Frameworks
		/sw # Fink
		/opt/local # DarwinPorts
		/opt/csw # Blastwave
		/opt
	)

	find_path(SDL2_INCLUDE_DIR SDL.h
		HINTS
		$ENV{SDL2DIR}
		PATH_SUFFIXES include/SDL2 include
		PATHS ${SDL2_SEARCH_PATHS}
	)

	find_library(SDL2_LIBRARY
		NAMES SDL2
		HINTS
		$ENV{SDL2DIR}
		PATH_SUFFIXES lib64 lib build_release
		PATHS ${SDL2_SEARCH_PATHS}
	)
elseif(UNIX)
	set(SDL2_SEARCH_PATHS
		/usr/include
		/usr/local/include/SDL2
	  	/usr/include/SDL2
	)

	find_path(SDL2_INCLUDE_DIR SDL.h
		HINTS
		$ENV{SDL2DIR}
		PATH_SUFFIXES include/SDL2 include
		PATHS ${SDL2_SEARCH_PATHS}
	)

	find_library(SDL2_LIBRARY
		NAMES SDL2
		HINTS
		$ENV{SDL2DIR}
		PATH_SUFFIXES lib64 lib build_release
		PATHS ${SDL2_SEARCH_PATHS}
	)
elseif(WIN32)
	set(SDL2_SEARCH_PATHS
		XdevLExternals/
	)

	find_path(SDL2_INCLUDE_DIR SDL.h
		HINTS
		$ENV{SDL2DIR}
		PATH_SUFFIXES include/SDL2 include
		PATHS ${SDL2_SEARCH_PATHS}
	)

	find_library(SDL2_LIBRARY
		NAMES SDL2
		HINTS
		$ENV{SDL2DIR}
		PATH_SUFFIXES lib64 lib build_release
		PATHS ${SDL2_SEARCH_PATHS}
	)
endif()


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2  DEFAULT_MSG  SDL2_LIBRARY SDL2_INCLUDE_DIR)
mark_as_advanced(SDL2_LIBRARY SDL2_INCLUDE_DIR)
