cmake_minimum_required (VERSION 3.0)
project (HelloOpenGL)

#set output folder
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)

#include header files
include_directories(include)
file(GLOB SOURCES "src/*.c*")

add_executable(HelloOpenGL ${SOURCES})

find_package(glfw3 REQUIRED)
target_link_libraries(HelloOpenGL glfw)



