INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

add_executable(json_test json_test.cc)
target_link_libraries(json_test gmock pthread json)

add_test(json_test json_test)
