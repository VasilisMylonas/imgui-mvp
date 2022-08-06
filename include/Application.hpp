#pragma once

#include <utility>
#include <glfwpp/glfwpp.h>

#include <imgui.h>

class Application
{
public:
    void run();

protected:
    virtual void init() = 0;
    virtual void render() = 0;

    Application(int width, int height, const std::string &title);

private:
    std::string m_title;
    glfw::GlfwLibrary m_library;
    glfw::Window m_window;
};
