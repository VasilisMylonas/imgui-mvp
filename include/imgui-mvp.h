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
    template <class TInterface>
    using Factory = TInterface *(*)();

    template <class TInterface, class TImpl>
    TInterface *default_factory()
    {
        return static_cast<TInterface *>(new TImpl{});
    }

    template <class TInterface, class TImpl>
    TInterface *singleton_factory()
    {
        static TInterface *instance;
        if (instance == nullptr)
        {
            instance = default_factory<TInterface, TImpl>();
        }
        return instance;
    }

    namespace detail
    {
        inline std::map<std::type_index, Factory<void>> service_registry;

        template <class TInterface>
        std::shared_ptr<TInterface> get_service()
        {
            auto factory = reinterpret_cast<Factory<TInterface>>(service_registry[std::type_index{typeid(TInterface)}]);

            if (factory == nullptr)
            {
                std::string s{"No registered class for interface "};
                s.append(typeid(TInterface).name());

                throw std::logic_error{s};
            }

            return std::shared_ptr<TInterface>{factory()};
        }
    } // namespace detail

    template <class TInterface, class TImpl>
    void register_service(Factory<TInterface> factory = default_factory<TInterface, TImpl>)
    {
        detail::service_registry[std::type_index{typeid(TInterface)}] = reinterpret_cast<Factory<void>>(factory);
    }

    template <class TInterface, class TImpl>
    void register_singleton(Factory<TInterface> factory = singleton_factory<TInterface, TImpl>)
    {
        detail::service_registry[std::type_index{typeid(TInterface)}] = reinterpret_cast<Factory<void>>(factory);
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
        std::shared_ptr<TInterface> service = detail::get_service<TInterface>();
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
        Application &run(std::shared_ptr<Window> window)
        {
            windows().push_back(window);
            return run();
        }

        Application &run()
        {
            while (windows().size() != 0)
            {
                size_t size = windows().size();
                for (size_t i = 0; i < size; i++)
                {
                    auto &window = windows()[i];

                    window->select();
                    window->frame_begin();
                    window->render();
                    window->frame_end();

                    glfwPollEvents();

                    if (glfwWindowShouldClose(window->handle()))
                    {
                        windows().erase(windows().begin() + i);
                    }
                }
            }

            return *this;
        }

        const std::vector<std::shared_ptr<Window>> &windows() const
        {
            return m_windows;
        }

        std::vector<std::shared_ptr<Window>> &windows()
        {
            return m_windows;
        }

        static Application &current()
        {
            return s_instance;
        }

        Application(const Application &) = delete;
        Application &operator=(const Application &) = delete;

    private:
        Application()
        {
            glfwInit();
        }

        ~Application()
        {
            glfwTerminate();
        }

        std::vector<std::shared_ptr<Window>> m_windows;
        static Application s_instance;
    };

    inline Application Application::s_instance;
} // namespace mvp
