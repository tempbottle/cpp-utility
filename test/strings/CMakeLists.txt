INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

add_executable(string_piece_test string_piece_test.cc)
target_link_libraries(string_piece_test gtest string_piece)

add_executable(stringprintf_test stringprintf_test.cc)
target_link_libraries(stringprintf_test gtest stringprintf)

add_executable(split_test split_test.cc)
target_link_libraries(split_test gmock split)


add_executable(encoding_test encoding_test.cc)
target_link_libraries(encoding_test gmock encoding)

add_test(stringprintf_test stringprintf_test)
add_test(string_piece_test string_piece_test)
add_test(split_test split_test)
add_test(encoding_test encoding_test)
