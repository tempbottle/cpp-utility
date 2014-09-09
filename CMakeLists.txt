cmake_minimum_required (VERSION 2.6)
project (cpp-utility)

# Cmake option
option(test "test" ON)

# The version number.
set (CPP_UTILITY_VERSION_MAJOR 1)
set (CPP_UTILITY_VERSION_MINOR 0)

# Compiler flags.
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall")

configure_file(
  "${PROJECT_SOURCE_DIR}/cpp-utility-config.h.in"
  "${PROJECT_BINARY_DIR}/cpp-utility-config.h"
)
include_directories("${PROJECT_BINARY_DIR}")

# for all library
add_subdirectory(base)
add_subdirectory(util)
add_subdirectory(strings)

if (test)
  enable_testing()
  add_subdirectory(test)
endif()

add_executable(cpp-utility cpp-utility.cc)
target_link_libraries(cpp-utility gtest)
