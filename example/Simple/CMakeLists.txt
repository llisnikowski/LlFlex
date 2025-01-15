cmake_minimum_required(VERSION 3.13)

project(LlFlex_example_Class)

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME}
PRIVATE
    LlFlex::LlFlex
)
