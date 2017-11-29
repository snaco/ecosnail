add_library (fuzz INTERFACE)

target_sources (fuzz INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ecosnail/fuzz.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ecosnail/fuzz/defunction.hpp)
target_include_directories (fuzz INTERFACE include/)

target_link_libraries (fuzz INTERFACE tail)

add_subdirectory (tests)
enable_testing ()
add_test (NAME fuzz_test COMMAND fuzz_test)