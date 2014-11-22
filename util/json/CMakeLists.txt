INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

add_library(json json.cc)
target_link_libraries(json string_piece)