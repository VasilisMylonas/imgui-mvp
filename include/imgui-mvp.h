#pragma once

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <typeindex>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

namespace mvp
{
    template <class TInterface, class TImpl>
    static void register_service()
    {
        TInterface *(*creator)() = []()
        {
            return static_cast<TInterface *>(new TImpl{});
        };

        detail::service_registry[std::type_index{typeid(TInterface)}] = reinterpret_cast<void *(*)()>(creator);
    }

    template <class TInterface>
    class Service
    {
    public:
        TInterface *operator->()
        {
            return service.operator->();
        }

    private:
        std::unique_ptr<TInterface> service = detail::get_service<TInterface>();
    };

    template <class TView, class TModel>
    class Presenter
    {
    public:
        void connect(TView *view)
        {
            m_view = view;
        }

        void connect(TModel *model)
        {
            m_model = model;
        }

        TView &view()
        {
            return *m_view;
        }

        const TView &view() const
        {
            return *m_view;
        }

        TModel &model()
        {
            return *m_model;
        }

        const TModel &model() const
        {
            return *m_model;
        }

    private:
        TView *m_view = nullptr;
        TModel *m_model = nullptr;
    };

    template <class TView>
    class Presenter<TView, void>
    {
    public:
        void connect(TView *view)
        {
            m_view = view;
        }

        TView &view()
        {
            return *m_view;
        }

        const TView &view() const
        {
            return *m_view;
        }

    private:
        TView *m_view = nullptr;
    };

    class View
    {
    public:
        virtual void render() = 0;
    };

    class Window : public View
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

        ~Window()
        {
            s_counter--;

            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();

            ImGui::DestroyContext(m_context);

            glfwDestroyWindow(m_window);
        }

        GLFWwindow *handle()
        {
            return m_window;
        }

    private:
        friend class Application;

        void select()
        {
            glfwMakeContextCurrent(m_window);
            ImGui::SetCurrentContext(m_context);
        }

        void frame_begin()
        {
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        void frame_end()
        {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(m_window);
        }

        std::string m_title;
        ImGuiContext *m_context;
        GLFWwindow *m_window;
        static inline unsigned s_counter = 0;
    };

    class Application
    {
    public:
        Application()
        {
            glfwInit();
        }

        ~Application()
        {
            glfwTerminate();
        }

        void add_window(std::shared_ptr<Window> window)
        {
            m_windows.push_back(window);
        }

        void run(std::shared_ptr<Window> window)
        {
            add_window(window);
            run();
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

                    if (glfwWindowShouldClose(window->handle()))
                    {
                        m_windows.erase(m_windows.begin() + i);
                    }
                }
            }
        }

    private:
        std::vector<std::shared_ptr<Window>> m_windows;
    };

    namespace detail
    {
        template <class TInterface>
        static std::unique_ptr<TInterface> get_service()
        {
            auto ctor = reinterpret_cast<TInterface *(*)()>(service_registry[std::type_index{typeid(TInterface)}]);

            if (ctor == nullptr)
            {
                std::string s{"No registered class for interface "};
                s.append(typeid(TInterface).name());

                throw std::logic_error{s};
            }

            return std::unique_ptr<TInterface>{ctor()};
        }

        static inline std::map<std::type_index, void *(*)()> service_registry;
    } // namespace detail
} // namespace mvp
