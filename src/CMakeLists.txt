macro (ecosnail_project)
    set (options "")
    set (oneValueArgs NAME)
    set (multiValueArgs SOURCES)
    cmake_parse_arguments (
        PROJECT
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    add_library (${PROJECT_NAME} ${PROJECT_SOURCES})
    target_include_directories (${PROJECT_NAME} PUBLIC include)
endmacro ()

add_subdirectory (blabla)
add_subdirectory (config)
add_subdirectory (evening)
add_subdirectory (fuzz)
add_subdirectory (tail)
add_subdirectory (tempo)
add_subdirectory (tops)
