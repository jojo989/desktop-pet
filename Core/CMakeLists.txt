cmake_minimum_required(VERSION 3.15)

add_library(Core STATIC)

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Source/*.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/Include/*.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h"
)


target_link_libraries(Core
    PUBLIC
        sfml-graphics
        sfml-window
        sfml-system
)

if(WIN32)
    target_link_libraries(Core PRIVATE dwmapi)
endif()

target_sources(Core PRIVATE ${CORE_SOURCES})

target_compile_options(Core PRIVATE -O3 -ffast-math)

target_include_directories(Core PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source>
	$<INSTALL_INTERFACE:include>
)

target_compile_features(Core PUBLIC cxx_std_23)
