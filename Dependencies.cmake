option(USE_SFML "Link against SFML (graphics, window, system)" ON)

if(USE_SFML)
	find_package(SFML 2.5 COMPONENTS graphics window system QUIET)
    
	if(NOT SFML_FOUND)
		include(FetchContent)
		message(STATUS "SFML not found; fetching and building SFML 3.0.2")
		FetchContent_Declare(
			sfml
			GIT_REPOSITORY https://github.com/SFML/SFML.git
			GIT_TAG 3.0.2
		)
		set(SFML_BUILD_EXAMPLES OFF)
		set(SFML_BUILD_TESTS OFF)
		set(SFML_BUILD_DOC OFF)
		FetchContent_MakeAvailable(sfml)
	else()
		message(STATUS "SFML found via find_package.")
	endif()

	if(NOT TARGET sfml-graphics)
		message(FATAL_ERROR "USE_SFML=ON but SFML could not be found or built. Install SFML or set -DUSE_SFML=OFF.")
	endif()
else()
	message(STATUS "Building without SFML (USE_SFML=OFF).")
endif()

if(UNIX AND NOT APPLE)
	find_package(X11 REQUIRED)
endif()