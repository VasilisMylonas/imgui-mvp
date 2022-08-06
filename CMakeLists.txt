cmake_minimum_required(VERSION 3.0.0)
project(image_editor VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(GLFWPP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # disable building GLFWPP examples

add_subdirectory(vendor/glfwpp)

add_library(glad vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

add_library(imgui
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/imgui.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC vendor/imgui vendor/imgui/backends)

add_executable(image_editor main.cpp)
target_compile_features(image_editor PRIVATE cxx_std_17)
target_link_libraries(image_editor GLFWPP imgui glad)
