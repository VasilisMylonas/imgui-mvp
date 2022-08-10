cmake_minimum_required(VERSION 3.0.0)
project(imgui-mvp VERSION 0.1.0)

add_subdirectory(deps)

add_library(imgui-mvp INTERFACE)
target_compile_features(imgui-mvp INTERFACE cxx_std_20)
target_link_libraries(imgui-mvp INTERFACE glfw imgui glad)
target_include_directories(imgui-mvp INTERFACE include)
