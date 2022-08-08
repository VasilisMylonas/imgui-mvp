# ImGui
add_library(imgui
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC imgui imgui/backends)

# GLFW
add_subdirectory(glfw)

# GLAD
add_library(glad glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)
