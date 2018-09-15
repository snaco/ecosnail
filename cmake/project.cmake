macro (ecosnail_projects)
    foreach (directory ${ARGN})
        add_subdirectory (${directory})
    endforeach ()
endmacro ()

macro (ecosnail_project)
    set (options "")
    set (oneValueArgs "")
    set (multiValueArgs SOURCES DEPENDS EXAMPLES)
    cmake_parse_arguments (
        PROJECT
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    get_filename_component (PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    set (ECOSNAIL_PROJECT ${PROJECT_NAME})
    set (ECOSNAIL_PROJECTS ${ECOSNAIL_PROJECTS} ${PROJECT_NAME} PARENT_SCOPE)
    set (project_key ECOSNAIL_PROJECT_${PROJECT_NAME})

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
        set (${project_key}_HAS_TESTS 1 PARENT_SCOPE)
        add_subdirectory (tests)
    else ()
        set (${project_key}_HAS_TESTS 0 PARENT_SCOPE)
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

macro (ecosnail_test NAME)
    if (${ARGC} GREATER 2)
        set (target "${ECOSNAIL_PROJECT}_${NAME}")
        add_executable (${target} ${ARGN})
    else ()
        set (target "${ECOSNAIL_PROJECT}_${NAME}")
        add_executable (${target} ${NAME})
    endif ()
    target_link_libraries (${target} PRIVATE ${ECOSNAIL_PROJECT})
endmacro ()

macro (print_summary)
    message (STATUS "project | has tests")
    foreach (project ${ECOSNAIL_PROJECTS})
        set (key ECOSNAIL_PROJECT_${project})

        set (has_tests ${${key}_HAS_TESTS})

        string (CONFIGURE "@project@ | @has_tests@" status)
        message (STATUS ${status})
    endforeach ()
endmacro ()
