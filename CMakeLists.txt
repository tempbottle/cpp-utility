cmake_minimum_required (VERSION 2.6)
project (cpp-utility)

# Cmake option
option(test "test" ON)

# Export compile command for libclang
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Import and build Googletest
include(ExternalProject)
set_directory_properties(properties EP_PREFIX "${CMAKE_BINARY_DIR}/third_party")
ExternalProject_Add(googletest
    URL "https://googletest.googlecode.com/files/gtest-1.7.0.zip"
    SOURCE_DIR "${CMAKE_BINARY_DIR}/third_party/gtest"
    INSTALL_COMMAND "")
ExternalProject_Get_Property(googletest source_dir)
include_directories(${source_dir}/include)
ExternalProject_Get_Property(googletest binary_dir)
link_directories(${binary_dir})

# Import and build Googlemock
ExternalProject_Add(googlemock
    URL "https://googlemock.googlecode.com/files/gmock-1.7.0.zip"
    SOURCE_DIR "${CMAKE_BINARY_DIR}/third_party/gmock"
    INSTALL_COMMAND "")
ExternalProject_Get_Property(googlemock source_dir)
include_directories(${source_dir}/include)
ExternalProject_Get_Property(googlemock binary_dir)
link_directories(${binary_dir})

# The version number.
set (CPP_UTILITY_VERSION_MAJOR 1)
set (CPP_UTILITY_VERSION_MINOR 0)

# Compiler flags.
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall")
set (GCC_COVERAGE_LINK_FLAGS "-pthread")

configure_file(
  "${PROJECT_SOURCE_DIR}/cpp-utility-config.h.in"
  "${PROJECT_BINARY_DIR}/cpp-utility-config.h"
)

# Set up directories
include_directories("${PROJECT_BINARY_DIR}")

# for all library
add_subdirectory(base)
add_subdirectory(util)
add_subdirectory(strings)

if (test)
  enable_testing()
  add_subdirectory(test)
endif()
