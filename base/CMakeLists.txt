INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

add_definitions(-DOS_MACOSX)

add_library(sysinfo sysinfo.cc)
target_link_libraries(sysinfo boost_thread-mt)
target_link_libraries(sysinfo boost_system-mt)

add_library(cycleclock cycleclock.cc)
target_link_libraries(cycleclock sysinfo)

add_library(walltime walltime.cc)
target_link_libraries(walltime cycleclock)
