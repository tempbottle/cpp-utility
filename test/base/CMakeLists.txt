INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

add_executable(sysinfo_test sysinfo_test.cc)
target_link_libraries(sysinfo_test gtest)
target_link_libraries(sysinfo_test sysinfo)

add_test(sysinfo_test sysinfo_test)
