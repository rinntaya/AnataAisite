#include "ContentBrowserPanel.h"

#include <set>

#include "imgui.h"
#include "AnataAisite/Application.h"
#include "AnataAisite/Renderer/Texture.h"

namespace Aisite
{
    // Once we have projects, change this
    extern const std::filesystem::path g_AssetPath = "assets";

    ContentBrowserPanel::ContentBrowserPanel()
        : m_CurrentDirectory(g_AssetPath)
    {
        m_DirectoryIcon = Texture2D::Create("resources/icons/ContentBrowser/3dicons-folder-front-color.png");
        m_FileIcon = Texture2D::Create("resources/icons/ContentBrowser/3dicons-file-text-dynamic-color.png");
        m_SceneIcon = Texture2D::Create("resources/icons/ContentBrowser/3dicons-video-cam-iso-gradient.png");
        m_ShaderIcon = Texture2D::Create("resources/icons/ContentBrowser/3dicons-cube-dynamic-color.png");
    }


    void ContentBrowserPanel::DirectoryThreeLoop(const std::filesystem::path& path)
    {
        for (auto& sub : std::filesystem::directory_iterator(path))
        {
            std::string filename = sub.path().filename().string();
            if (!sub.is_directory())
            {
                // ImGui::BulletText("%s", filename.c_str());
                continue;
            }


            const bool is_tree_open = ImGui::TreeNodeEx(filename.c_str(),
                m_CurrentDirectory == sub.path() ? ImGuiTreeNodeFlags_Selected : 0 |
                ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding);
            if (ImGui::IsItemClicked())
                m_CurrentDirectory = sub.path();

            if (is_tree_open) DirectoryThreeLoop(sub.path());

        }
        ImGui::TreePop();
    }

    void ContentBrowserPanel::OnImGuiRender()
    {
        const auto AssetsDirectory = std::filesystem::path(g_AssetPath);


        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Content Browser", nullptr);
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable) ImGui::DockSpace(ImGui::GetID("Content Browser"), ImVec2(0.0f, 0.0f));

        {
            ImGui::Begin("Directory Tree");
            ImGui::PopStyleVar();
    	    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2.f)); // 节点之间无缝隙


            constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

            // ImGui::Dummy({0.f, 25.f});
            ImGui::SetWindowFontScale(1.13f);
            if (ImGui::TreeNodeEx("☆ Favorites", treeNodeFlags ^ ImGuiTreeNodeFlags_DefaultOpen)) ImGui::TreePop();
            if (ImGui::TreeNodeEx("●  Assets", treeNodeFlags))
            {
                ImGui::SetWindowFontScale(1.03f);
                ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.55f, 0.55f, 0.55f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.40f, 0.40f, 0.40f, 1.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.f, 1.f));

                DirectoryThreeLoop(g_AssetPath);

                ImGui::PopStyleVar();
                ImGui::PopStyleColor(3);
            }
            ImGui::SetWindowFontScale(1.f);
            // ImGui::Dummy({0.f, 25.f});

            ImGui::End();
            ImGui::PopStyleVar();

        }


        {
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

            ImGui::Begin("Content");
            ImGui::PopStyleVar();

            // 动态布局
            float dpi = Application::Get().GetWindow().GetDpi();
            float panelWidth = ImGui::GetContentRegionAvail().x;

            // 想要多少列，这里可以根据缩放调整，比如最小128像素一列
            int columnCount = (int)(panelWidth / (120.0f * dpi));
            if (columnCount < 1) columnCount = 1;

            // 每个 cell 尺寸
            float cellSize = panelWidth / columnCount;
            float innerPadding = 24.0f * dpi;  // 图标和按钮边缘的内边距
            float thumbnailSize = cellSize - innerPadding * 2;

            ImGui::Columns(columnCount, nullptr, false);

            if (m_CurrentDirectory != std::filesystem::path(g_AssetPath))
            {
            ImGui::SetWindowFontScale(3.f);
                ImGui::Button("../", {cellSize, cellSize});
                if (ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip("<- Last directory");
                    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                        m_CurrentDirectory = m_CurrentDirectory.parent_path();
                }
                ImGui::NextColumn();
            ImGui::SetWindowFontScale(1.f);
            }

            for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
            if (directoryEntry.is_directory())
            {


                const auto& path = directoryEntry.path();
                std::string filenameString = path.filename().string();
                ImGui::PushID(filenameString.c_str());


                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(innerPadding, innerPadding));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

                ImGui::ImageButton("##dir", m_DirectoryIcon->GetRendererID(), {thumbnailSize, thumbnailSize }, {0, 1}, {1, 0});

                ImGui::PopStyleColor();
                ImGui::PopStyleVar();


                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    if (directoryEntry.is_directory())
                        m_CurrentDirectory /= path.filename();

                {
                    // 图标和文字之间加点间距
                    ImGui::Dummy({0.f, 4.f});

                    // 处理过长的文件名，自动加省略号
                    std::string displayName = filenameString;
                    float maxTextWidth = cellSize - 8.0f; // 给左右边留点余量
                    ImVec2 textSize = ImGui::CalcTextSize(displayName.c_str());

                    if (textSize.x > maxTextWidth)
                    {
                        while (!displayName.empty() && ImGui::CalcTextSize((displayName + "...").c_str()).x > maxTextWidth)
                            displayName.pop_back();
                        displayName += "...";
                    }

                    // 居中显示
                    float textOffset = (cellSize - ImGui::CalcTextSize(displayName.c_str()).x) * 0.5f;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + textOffset);
                    ImGui::TextUnformatted(displayName.c_str());


                    ImGui::Dummy({0.f, 10.f});
                }

                ImGui::NextColumn();
                ImGui::PopID();
            }


            for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
            if (!directoryEntry.is_directory())
            {

                ContentMeta *meta = new ContentMeta();

                const auto& path = directoryEntry.path();
                meta->path = path;
                std::string filenameString = path.filename().string();
                ImGui::PushID(filenameString.c_str());


                Ref<Texture2D> icon;
                if (directoryEntry.is_directory())
                {
                    meta->type = ContentType::Directory;
                    icon = m_DirectoryIcon;
                } else
                {
                    meta->extension = path.extension().string();
                    for (auto &c : meta->extension) c = std::tolower(c);
                    // std::string ext = meta->extension;
                    // std::transform(ext.begin(), ext.end(), ext.begin(), tolower);

                    const std::set<std::string> imageExtensions = {
                        ".png", ".jpg", ".jpeg", ".bmp", ".tga", ".gif", ".hdr", ".pic", ".pnm", ".psd"
                    };

                    if (imageExtensions.find(meta->extension) != imageExtensions.end())
                    {
                        meta->type = ContentType::Texture;

                        if (const auto icon_cache = m_TextureViewCache[path]; icon_cache) icon = icon_cache;
                        else
                        {
                            icon = Texture2D::Create(path.string());
                            m_TextureViewCache[path] = icon;
                        }

                    } else if (meta->extension == ".scene")
                    {
                        meta->type = ContentType::Scene;
                        icon = m_SceneIcon;
                    } else if (meta->extension == ".glsl")
                    {
                        meta->type = ContentType::GlslShader;
                        icon = m_ShaderIcon;
                    }  else
                    {
                        meta->type = ContentType::Unknown;
                        icon = m_FileIcon;
                    }
                }



                const auto aspectRatio = (float)icon->GetWidth() / (float)icon->GetHeight();
                const auto thumbnailHeight = thumbnailSize / aspectRatio;

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(innerPadding, innerPadding + (thumbnailSize - thumbnailHeight) / 2));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

                ImGui::ImageButton("##", icon->GetRendererID(), {thumbnailSize, thumbnailHeight }, {0, 1}, {1, 0});

                ImGui::PopStyleColor();
                ImGui::PopStyleVar();

                if (ImGui::BeginDragDropSource())
                {
                    // const wchar_t* itemPath = relativePath.c_str();
                    // ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
                    ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", meta, sizeof(ContentMeta));

                    ImGui::Image(icon->GetRendererID(), ImVec2(128, 128 / aspectRatio), {0, 1}, {1, 0});

                    ImGui::SetWindowFontScale(.5f);
                    ImGui::TextWrapped("%S", path.c_str());  // 显示文字
                    ImGui::SetWindowFontScale(1.f);


                    ImGui::EndDragDropSource();
                } else if (ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip("%s", filenameString.c_str());
                }

                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    if (directoryEntry.is_directory())
                        m_CurrentDirectory /= path.filename();

                {
                    // 图标和文字之间加点间距
                    ImGui::Dummy({0.f, 4.f});

                    // 处理过长的文件名，自动加省略号
                    std::string displayName = filenameString;
                    float maxTextWidth = cellSize - 8.0f; // 给左右边留点余量
                    ImVec2 textSize = ImGui::CalcTextSize(displayName.c_str());

                    if (textSize.x > maxTextWidth)
                    {
                        while (!displayName.empty() && ImGui::CalcTextSize((displayName + "...").c_str()).x > maxTextWidth)
                            displayName.pop_back();
                        displayName += "...";
                    }

                    // 居中显示
                    float textOffset = (cellSize - ImGui::CalcTextSize(displayName.c_str()).x) * 0.5f;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + textOffset);
                    ImGui::TextUnformatted(displayName.c_str());


                    ImGui::Dummy({0.f, 10.f});
                }

                ImGui::NextColumn();
                ImGui::PopID();
            }

            ImGui::Columns(1);
            ImGui::Dummy({0.f, 70.f});
            ImGui::End();
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
        }

        ImGui::End();

    }
}
