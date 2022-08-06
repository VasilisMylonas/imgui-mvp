#pragma once

#include "View.hpp"

#include <utility>

#include <glad/glad.h>
#include <glfwpp/glfwpp.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

class IWindow : public IView
{
public:
    virtual void select() = 0;
    virtual void frame_begin() = 0;
    virtual void frame_end() = 0;
    virtual glfw::Window &window() = 0;
};

template <class C>
class Window : public View<C>, public IWindow
{
public:
    Window(int width, int height, const std::string title)
        : m_title{title}, m_window{width, height, m_title.c_str()}
    {
        m_context = ImGui::CreateContext();

        select();
        glfw::swapInterval(0);

        if (s_counter == 0)
        {
            gladLoadGL();
        }

        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init();

        s_counter++;
    }

    ~Window()
    {
        s_counter--;

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();

        ImGui::DestroyContext(m_context);
    }

    virtual glfw::Window &window() override
    {
        return m_window;
    }

private:
    virtual void select() override
    {
        glfw::makeContextCurrent(m_window);
        ImGui::SetCurrentContext(m_context);
    }

    virtual void frame_begin() override
    {
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    virtual void frame_end() override
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        m_window.swapBuffers();
    }

    std::string m_title;
    ImGuiContext *m_context;
    glfw::Window m_window;
    static unsigned s_counter;
};

template <class C>
unsigned Window<C>::s_counter = 0;
