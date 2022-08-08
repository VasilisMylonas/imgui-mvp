cmake_minimum_required(VERSION 3.0.0)
project(imgui-mvc VERSION 0.1.0)

add_subdirectory(deps)

add_library(imgui-mvc INTERFACE)
target_compile_features(imgui-mvc INTERFACE cxx_std_20)
target_link_libraries(imgui-mvc INTERFACE glfw imgui glad)
target_include_directories(imgui-mvc INTERFACE include)
