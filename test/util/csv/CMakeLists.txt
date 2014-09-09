INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

add_executable(csv_test csv_test.cc)
target_link_libraries(csv_test gtest)
target_link_libraries(csv_test parser)

add_test(csv_test csv_test)
