cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(cmake/flags.cmake)

project(LlFlex LANGUAGES CXX)


add_library(${PROJECT_NAME} INTERFACE)

AddDefaultFlags(${PROJECT_NAME} SCOPE INTERFACE)

target_include_directories(${PROJECT_NAME}
INTERFACE
    src/
)


option(BUILD_UTEST "Build tests" ON)

if(BUILD_UTEST)
    add_subdirectory(utest)
endif()

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
