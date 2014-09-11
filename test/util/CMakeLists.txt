INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
add_subdirectory(csv)

add_executable(registerer_test registerer_test.cc)
target_link_libraries(registerer_test gtest)
target_link_libraries(registerer_test boost_thread-mt)
target_link_libraries(registerer_test boost_system-mt)

add_executable(singleton_test singleton_test.cc)
target_link_libraries(singleton_test gtest)
target_link_libraries(singleton_test boost_thread-mt)
target_link_libraries(singleton_test boost_system-mt)

add_executable(comparator_test comparator_test.cc)
target_link_libraries(comparator_test gtest)

add_executable(copy_on_write_test copy_on_write_test.cc)
target_link_libraries(copy_on_write_test gtest)

add_test(registerer_test registerer_test)
add_test(singleton_test singleton_test)
add_test(comparator_test comparator_test)
add_test(copy_on_write_test copy_on_write_test)
