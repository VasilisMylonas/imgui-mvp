#pragma once

#include "mvc/View.hpp"

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

namespace mvc
{
    class IWindow : public IView
    {
    public:
        virtual void select() = 0;
        virtual void frame_begin() = 0;
        virtual void frame_end() = 0;
        virtual GLFWwindow *window() = 0;
    };

    template <class C>
    class Window : public View<C>, public IWindow
    {
    public:
        Window(int width, int height, const std::string &title)
            : m_title{title}, m_window{glfwCreateWindow(width, height, m_title.c_str(), nullptr, nullptr)}
        {
            m_context = ImGui::CreateContext();

            select();
            glfwSwapInterval(0);

            if (s_counter == 0)
            {
                gladLoadGL();
            }

            ImGui_ImplGlfw_InitForOpenGL(m_window, true);
            ImGui_ImplOpenGL3_Init();

            s_counter++;
        }

        virtual ~Window()
        {
            s_counter--;

            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();

            ImGui::DestroyContext(m_context);

            glfwDestroyWindow(m_window);
        }

        virtual GLFWwindow *window() override
        {
            return m_window;
        }

    private:
        virtual void select() override
        {
            glfwMakeContextCurrent(m_window);
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

            glfwSwapBuffers(m_window);
        }

        std::string m_title;
        ImGuiContext *m_context;
        GLFWwindow *m_window;
        static unsigned s_counter;
    };

    template <class C>
    unsigned Window<C>::s_counter = 0;
} // namespace mvc
