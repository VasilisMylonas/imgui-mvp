#pragma once

#include "mvc/Window.hpp"

#include <vector>
#include <memory>

namespace mvc
{
    class Application
    {
    public:
        Application()
        {
            glfwInit();
            IMGUI_CHECKVERSION();
        }

        ~Application()
        {
            glfwTerminate();
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

                    glfwPollEvents();

                    if (glfwWindowShouldClose(window->window()))
                    {
                        m_windows.erase(m_windows.begin() + i);
                    }
                }
            }
        }

    private:
        std::vector<std::shared_ptr<IWindow>> m_windows;
    };
} // namespace mvc
