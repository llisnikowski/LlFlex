cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(cmake/flags.cmake)

project(LlParam LANGUAGES CXX)


add_library(${PROJECT_NAME} INTERFACE)

AddDefaultFlags(${PROJECT_NAME} SCOPE INTERFACE)

target_include_directories(${PROJECT_NAME}
INTERFACE
    src/
)


option(BUILD_UTEST "Build tests" ON)
option(BUILD_EXAMPLE "Build tests" ON)

if(BUILD_UTEST)
    add_subdirectory(utest)
endif()
if(BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
