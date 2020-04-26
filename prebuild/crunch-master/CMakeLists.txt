
cmake_minimum_required (VERSION 2.8.10)

set(PROJECT_NAME "crunch")
project(${PROJECT_NAME})


option(CRUNCH_BUILD_SHARED "" ON)

add_subdirectory(crnlib)
# add_subdirectory(crunch)
# add_subdirectory(example1)
# add_subdirectory(example2)
# add_subdirectory(example3)

    
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
