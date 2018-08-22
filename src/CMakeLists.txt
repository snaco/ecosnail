macro (ecosnail_project)
    set (options "")
    set (oneValueArgs NAME)
    set (multiValueArgs SOURCES DEPENDS EXAMPLES)
    cmake_parse_arguments (
        PROJECT
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    if (PROJECT_SOURCES)
        add_library (${PROJECT_NAME} ${PROJECT_SOURCES})
        target_include_directories (${PROJECT_NAME} PRIVATE include)
        if (PROJECT_DEPENDS)
            target_link_libraries (${PROJECT_NAME} PRIVATE ${PROJECT_DEPENDS})
        endif ()
    else ()
        add_library (${PROJECT_NAME} INTERFACE)
        target_include_directories (${PROJECT_NAME} INTERFACE include)
        if (PROJECT_DEPENDS)
            target_link_libraries (${PROJECT_NAME} INTERFACE ${PROJECT_DEPENDS})
        endif ()
    endif ()

    if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests")
        add_subdirectory (tests)
    endif ()

    foreach (example_name ${PROJECT_EXAMPLES})
        set (source_path "${CMAKE_CURRENT_SOURCE_DIR}/examples/${example_name}.cpp")
        if (NOT EXISTS ${source_path})
            message (SEND_ERROR "Cannot find example ${example_name} for project ${PROJECT_NAME}")
            continue ()
        endif ()

        add_executable (${example_name} ${source_path})
        target_link_libraries (${example_name} ${PROJECT_NAME})
    endforeach ()
endmacro ()

add_subdirectory (blabla)
add_subdirectory (config)
add_subdirectory (dimentia)
add_subdirectory (evening)
add_subdirectory (fuzz)
add_subdirectory (tail)
add_subdirectory (tempo)
add_subdirectory (tops)
