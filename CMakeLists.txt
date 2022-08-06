cmake_minimum_required(VERSION 3.0.0)
project(image_editor VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(vendor)

file(GLOB SOURCES src/*.cpp)

add_executable(image_editor ${SOURCES})
target_link_libraries(image_editor PRIVATE GLFWPP imgui glad)
target_include_directories(image_editor PRIVATE include)
