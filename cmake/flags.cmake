
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

macro(AddDefaultFlags target)

    set(options)
    set(oneValueArgs SCOPE)
    set(multiValueArgs)
    cmake_parse_arguments(ARGUMENTS "${options}" "${oneValueArgs}"
        "${multiValueArgs}" ${ARGN})

    if(NOT DEFINED ARGUMENTS_SCOPE)
        set(ARGUMENTS_SCOPE PRIVATE)
    endif()


    if (MSVC)
        target_compile_options(${target} ${ARGUMENTS_SCOPE} -W4 /XW)
    else ()
        target_compile_options(${target} ${ARGUMENTS_SCOPE} -Wall -Wextra -pedantic -Wconversion -Werror
            -Wno-unused-function -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-variable
        )
    endif ()

    if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
        target_compile_options(${target} ${ARGUMENTS_SCOPE} -fdiagnostics-color=always)
    elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        target_compile_options(${target} ${ARGUMENTS_SCOPE} -fcolor-diagnostics)
    endif ()

endmacro()