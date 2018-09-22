macro (ecosnail_projects)
    foreach (directory ${ARGN})
        add_subdirectory (${directory})
    endforeach ()
endmacro ()

macro (ecosnail_project)
    set (options TESTS)
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
        set (scope PUBLIC)
    else ()
        add_library (${PROJECT_NAME} INTERFACE)
        set (scope INTERFACE)
    endif ()

    target_include_directories (${PROJECT_NAME} ${scope} include)
    if (PROJECT_DEPENDS)
        target_link_libraries (${PROJECT_NAME} ${scope} ${PROJECT_DEPENDS})
        set (${project_key}_DEPENDS ${PROJECT_DEPENDS} PARENT_SCOPE)
    endif ()

    if (PROJECT_TESTS)
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
        get_filename_component (name ${NAME} NAME_WE)
        set (target "${ECOSNAIL_PROJECT}_${name}")
        add_executable (${target} ${NAME})
    endif ()
    target_link_libraries (${target} PRIVATE ${ECOSNAIL_PROJECT})
    add_test (NAME ${target} COMMAND ${target})
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

macro (write_depends_file)
    set (target_file "${CMAKE_CURRENT_SOURCE_DIR}/../.depends.json")
    file (WRITE ${target_file} "{\n")

    list (LENGTH ECOSNAIL_PROJECTS project_count)
    math (EXPR project_max_index "${project_count} - 1")
    foreach (project_index RANGE ${project_max_index})
        list (GET ECOSNAIL_PROJECTS ${project_index} project)

        file (APPEND ${target_file} "    \"${project}\": [")

        set (key ECOSNAIL_PROJECT_${project})
        set (depends ${${key}_DEPENDS})

        if (depends)
            list (GET depends 0 first_element)
            file (APPEND ${target_file} "\"${first_element}\"")
            list (REMOVE_AT depends 0)

            foreach (dependency ${depends})
                file (APPEND ${target_file} ", \"${dependency}\"")
            endforeach ()
        endif ()

        file (APPEND ${target_file} "]")
        if (project_index LESS project_max_index)
            file (APPEND ${target_file} ",")
        endif ()
        file (APPEND ${target_file} "\n")
    endforeach ()

    file (APPEND ${target_file} "}\n")
endmacro ()
