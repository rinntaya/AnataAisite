#pragma once
#include <filesystem>

#include "AnataAisite/Renderer/Texture.h"

namespace Aisite
{

    enum class ContentType
    {
        Unknown,
        Directory,
        Scene,
        Binary,
        GlslShader,
        Texture
    };
    struct ContentMeta
    {
        ContentType type;
        std::filesystem::path path;
        std::string extension;
    };
    class ContentBrowserPanel
    {
    public:
        ContentBrowserPanel();
        void DirectoryThreeLoop(const std::filesystem::path& path);

        void OnImGuiRender();
    private:
        std::filesystem::path m_CurrentDirectory;

        Ref<Texture2D> m_DirectoryIcon;
        Ref<Texture2D> m_FileIcon;
        Ref<Texture2D> m_SceneIcon;
        Ref<Texture2D> m_ShaderIcon;

        std::unordered_map<std::filesystem::path, Ref<Texture2D>> m_TextureViewCache;
    };
} // Aisite

