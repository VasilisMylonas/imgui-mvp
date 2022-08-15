#pragma once

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <typeindex>
#include <stdexcept>

#include <cstdlib>
#include <cstring>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wcast-align"
#pragma GCC diagnostic ignored "-Wpadded"
#pragma GCC diagnostic ignored "-Wimplicit-int-conversion"
#pragma GCC diagnostic ignored "-Wdouble-promotion"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#pragma GCC diagnostic pop

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

        virtual ~Presenter() = default;

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

        virtual ~Presenter() = default;

    private:
        TView *m_view = nullptr;
    };

    class View
    {
    public:
        virtual void render() = 0;
        virtual ~View() = default;
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

        virtual ~Window()
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
        Application &run()
        {
            on_startup();

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

            on_shutdown();

            return *this;
        }

        Application()
        {
            glfwInit();
        }

        virtual ~Application()
        {
            glfwTerminate();
        }

        Application(const Application &) = delete;
        Application &operator=(const Application &) = delete;
        Application(Application &&) = delete;
        Application &operator=(Application &&) = delete;

    protected:
        virtual void on_startup() = 0;
        virtual void on_shutdown() = 0;

        const std::vector<std::shared_ptr<Window>> &windows() const
        {
            return m_windows;
        }

        std::vector<std::shared_ptr<Window>> &windows()
        {
            return m_windows;
        }

    private:
        std::vector<std::shared_ptr<Window>> m_windows;
    };

    class Image
    {
    public:
        Image(const std::vector<std::byte> &bytes)
        {
            m_data = stbi_load_from_memory(
                reinterpret_cast<const unsigned char *>(bytes.data()),
                static_cast<int>(bytes.size()),
                &m_width,
                &m_height,
                &m_comp,
                0);

            create();
        }

        Image(const std::string &path) : m_path{path}
        {
            m_data = stbi_load(path.c_str(), &m_width, &m_height, &m_comp, 0);
            create();
        }

        Image(const Image &other)
        {
            copy(other);
        }

        Image(Image &&other)
        {
            move(std::move(other));
        }

        Image &operator=(const Image &other)
        {
            destroy();
            copy(other);
            return *this;
        }

        Image &operator=(Image &&other)
        {
            destroy();
            move(std::move(other));
            return *this;
        }

        ~Image()
        {
            destroy();
        }

        int width() const
        {
            return m_width;
        }

        int height() const
        {
            return m_height;
        }

        int comp() const
        {
            return m_comp;
        }

        const std::string &path() const
        {
            return m_path;
        }

        void *texture() const
        {
            return reinterpret_cast<void *>(static_cast<uintptr_t>(m_texture));
        }

        std::size_t size() const
        {
            return static_cast<std::size_t>(m_width * m_height * m_comp);
        }

        std::byte *data()
        {
            return reinterpret_cast<std::byte *>(m_data);
        }

    private:
        void destroy()
        {
            glDeleteTextures(1, &m_texture);
            stbi_image_free(m_data);
        }

        void move(Image &&other)
        {
            m_width = other.m_width;
            m_height = other.m_height;
            m_comp = other.m_comp;

            m_path = std::move(other.m_path);
            m_data = other.m_data;
            m_texture = other.m_texture;

            other.m_data = nullptr;
            other.m_texture = 0;
        }

        void copy(const Image &other)
        {
            m_width = other.m_width;
            m_height = other.m_height;
            m_comp = other.m_comp;

            m_path = other.m_path;
            m_data = std::memcpy(std::malloc(size()), other.m_data, size());
            create();
        }

        void create()
        {
            if (m_data == nullptr)
            {
                throw std::runtime_error("Could not load image");
            }

            glGenTextures(1, &m_texture);
            glBindTexture(GL_TEXTURE_2D, m_texture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
        }

        std::string m_path;
        void *m_data;
        unsigned m_texture;
        int m_width;
        int m_height;
        int m_comp;
    };

} // namespace mvp
