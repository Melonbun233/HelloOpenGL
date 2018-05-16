cmake_minimum_required (VERSION 3.0)
project (HelloTriangle)

#Set output folder
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)

#Include header files
include_directories(include)
file(GLOB SOURCES "src/*.c*")

add_executable(HelloTriangle ${SOURCES})

find_package(glfw3 3.2 REQUIRED)
target_link_libraries(HelloTriangle glfw)



