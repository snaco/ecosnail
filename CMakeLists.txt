cmake_minimum_required (VERSION 3.8)

project (ecosnail)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)

enable_testing ()
add_subdirectory (projects)
