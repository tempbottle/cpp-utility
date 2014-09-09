INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

add_library(split split.cc)

add_library(string_piece string_piece.cc)

add_library(stringprintf stringprintf.cc)

add_library(strip strip.cc)
target_link_libraries(strip string_piece)
