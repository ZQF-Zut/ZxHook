# Main CMakeLists
cmake_minimum_required(VERSION 3.25)

# Project Name
project(Zut_ZxHook_Main)

# Only Windows
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
  message(FATAL_ERROR "Zut::ZxHook Only Windows")
endif()

# On Test Preset
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    include(CMakeMycfg.cmake)
endif()

# Package
add_subdirectory(pkg)

# Source
add_subdirectory(src)

# On Test Config
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory(test)
endif()
