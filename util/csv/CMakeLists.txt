INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

add_library(parser parser.cc)
target_link_libraries(parser stringprintf)
