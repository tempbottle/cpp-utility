INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

add_subdirectory(csv)
add_subdirectory(json)

add_library(existence_filter existence_filter.cc)
