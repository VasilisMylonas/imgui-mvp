#include <iostream>

#include "Application.hpp"

#include <iostream>

class App : public Application
{
    virtual glfw::Window init() override
    {
        // Config
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGuiStyle &style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;

        glfw::WindowHints hints;
        hints.clientApi = glfw::ClientApi::OpenGl;
        hints.apply();

        return {1280, 720, "Title"};
    }

    // Image folderIcon{"../folder-outline.png"};
    // Image fileIcon{"../document-outline.png"};
    //  std::filesystem::path currentDir = std::filesystem::current_path();
    // bool showHiddenFiles = false;

    virtual void render()
    {
        // DrawMenu();
        // DrawFileManager();
    }

#if 0
    void DrawFileManager()
    {
        int columns = 8;

        if (ImGui::Begin("Window"))
        {
            if (ImGui::Button("Up"))
            {
                currentDir = currentDir.parent_path();
            }

            if (ImGui::Button("Show/Hide hidden files"))
            {
                showHiddenFiles = !showHiddenFiles;
            }

            if (ImGui::BeginTable("DirView", columns, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody))
            {
                for (const auto &file : std::filesystem::directory_iterator(currentDir))
                {
                    if (!showHiddenFiles && file.path().filename().string()[0] == '.')
                    {
                        continue;
                    }

                    ImGui::TableNextColumn();
                    ImGui::ImageButton((file.is_directory() ? folderIcon : fileIcon).GetTexture(), {64, 64});

                    if (ImGui::IsItemClicked())
                    {
                        if (file.is_directory())
                        {
                            currentDir = file.path();
                        }
                    }

                    ImGui::Text("%s", file.path().filename().c_str());
                }
                ImGui::EndTable();
            }
        }
        ImGui::End();
    }

    void DrawMenu()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open", "Ctrl+O"))
                {
                }

                if (ImGui::MenuItem("Open Folder"))
                {
                }

                if (ImGui::MenuItem("Exit", "Alt+F4"))
                {
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Tools"))
            {
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Help"))
            {
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }
#endif
};

int main()
{
    App app;
    app.run();
}
