#pragma once

#include "Window.hpp"

#include <vector>
#include <memory>

class Application
{
public:
    Application() : m_library{glfw::init()}
    {
        IMGUI_CHECKVERSION();
    }

    void add_window(std::shared_ptr<IWindow> window)
    {
        m_windows.push_back(window);
    }

    void run()
    {
        while (m_windows.size() != 0)
        {
            size_t size = m_windows.size();
            for (size_t i = 0; i < size; i++)
            {
                auto &window = m_windows[i];
                window->select();
                window->frame_begin();
                window->render();
                window->frame_end();

                glfw::pollEvents();

                if (window->window().shouldClose())
                {
                    m_windows.erase(m_windows.begin() + i);
                }
            }
        }
    }

private:
    glfw::GlfwLibrary m_library;
    std::vector<std::shared_ptr<IWindow>> m_windows;
};
