cmake_minimum_required(VERSION 3.28.1)
project(DesktopPet VERSION 0.2

                    DESCRIPTION "modern solution to desktop pets!"
                    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(${CMAKE_SOURCE_DIR}/Dependencies.cmake)

add_subdirectory(Core)
add_subdirectory(App) 
