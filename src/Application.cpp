#include <glad/glad.h>

#include "Application.hpp"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

Application::Application(int width, int height, const std::string &title)
    : m_title{title}, m_library{glfw::init()}, m_window{width, height, m_title.c_str()}
{
}

void Application::run()
{
    // Imgui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // OpenGL init
    glfw::makeContextCurrent(m_window);
    gladLoadGL();
    glfw::swapInterval(0);

    init();

    // Imgui backend init
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init();

    while (!m_window.shouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Begin new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // User render
        render();

        // Render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfw::pollEvents();
        m_window.swapBuffers();
    }

    // Imgui backend shutdown
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
}
