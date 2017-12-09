add_library (evening INTERFACE)

target_sources (evening INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ecosnail/evening.hpp
)
target_include_directories (evening INTERFACE include/)

add_subdirectory (tests)
