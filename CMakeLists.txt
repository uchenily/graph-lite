cmake_minimum_required(VERSION 3.20.0)
project(graph-lite VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "Version: " "${CMAKE_PROJECT_VERSION}")
message(STATUS "C++ standard: " "${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: " "${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Build type: " "${CMAKE_BUILD_TYPE}")

set(CGRAPH_LITE_TUTORIAL_LIST
    T01-Simple
    # T02-Param
)

foreach(name ${CGRAPH_LITE_TUTORIAL_LIST})
    add_executable(${name}
        ${name}.cpp
    )
endforeach()
