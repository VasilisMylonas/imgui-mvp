#include <iostream>
#include <filesystem>

#include "Application.hpp"
#include "Image.hpp"

class App : public Application
{
public:
    App(int width, int height, const std::string &title) : Application{width, height, title}
    {
    }

protected:
    virtual void init() override
    {
        // Config
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGuiStyle &style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;

        folderIcon.upload();
        fileIcon.upload();

        return;
    }

    Image folderIcon{"../folder-outline.png"};
    Image fileIcon{"../document-outline.png"};
    std::filesystem::path currentDir = std::filesystem::current_path();
    bool showHiddenFiles = false;

    virtual void render()
    {
        ImGui::DockSpaceOverViewport();
        DrawMenuBar();
        DrawFileManager();
    }

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
                    ImGui::ImageButton((file.is_directory() ? folderIcon : fileIcon).texture(), {64, 64});

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

    void DrawMenuBar()
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

            ImGui::EndMainMenuBar();
        }
    }
};

int main()
{
    App app = {1280, 720, "Main Window"};
    app.run();
}
