#include "glad/glad.h"

#include "Application.hpp"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

void Application::Run()
{
    // Imgui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Window creation
    auto window = Init();

    // OpenGL init
    glfw::makeContextCurrent(window);
    gladLoadGL();
    glfw::swapInterval(0);

    // Imgui backend init
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    while (!window.shouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Begin new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // User render
        Render();

        // Render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfw::pollEvents();
        window.swapBuffers();
    }

    // Imgui backend shutdown
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
}
