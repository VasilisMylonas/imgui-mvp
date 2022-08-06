#pragma once

#include <utility>
#include "glfwpp/glfwpp.h"

#include "imgui.h"

class Application
{
public:
    void Run();

protected:
    virtual glfw::Window Init() = 0;
    virtual void Render() = 0;

    Application() : library{glfw::init()}
    {
    }

private:
    glfw::GlfwLibrary library;
};
