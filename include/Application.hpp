#pragma once

#include <utility>
#include <glfwpp/glfwpp.h>

#include <imgui.h>

class Application
{
public:
    void run();

protected:
    virtual glfw::Window init() = 0;
    virtual void render() = 0;

    Application() : m_library{glfw::init()}
    {
    }

private:
    glfw::GlfwLibrary m_library;
};
