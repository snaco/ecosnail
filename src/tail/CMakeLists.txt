add_library (tail INTERFACE)

target_sources (tail INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ecosnail/tail/hetero_pack.hpp)
target_include_directories (tail INTERFACE include/)

add_subdirectory (examples)
add_subdirectory (tests)
