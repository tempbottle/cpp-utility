INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

add_library(sysinfo sysinfo.cc)
target_link_libraries(sysinfo)

add_library(cycleclock cycleclock.cc)
target_link_libraries(cycleclock sysinfo)

add_library(walltime walltime.cc)
target_link_libraries(walltime cycleclock)
