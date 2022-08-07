cmake_minimum_required(VERSION 3.0.0)
project(imgui-mvc VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(deps)

add_library(imgui-mvc INTERFACE)
target_link_libraries(imgui-mvc INTERFACE glfwpp imgui glad)
target_include_directories(imgui-mvc INTERFACE include)
