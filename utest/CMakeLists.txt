cmake_minimum_required(VERSION 3.13)

project(llFlex_ut LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

file(GLOB SOURCES 
    "*.cpp"
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
)

target_include_directories(${PROJECT_NAME}
PRIVATE
    src/
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    llFlex::llFlex
    gtest
    gmock
    gtest_main
)
