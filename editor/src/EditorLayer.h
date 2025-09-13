#pragma once
#include "AnataAisite.h"
#include "AnataAisite/Events/KeyEvent.h"
#include "panels/ContentBrowserPanel.h"
#include "panels/SceneHierarchyPanel.h"
#include "renderer/EditorCamera.h"


namespace Aisite {

    class EditorLayer : public Layer
    {
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        void OnUpdate(Timestep ts) override;
        virtual void OnDebugUIRender() override;
        void OnEvent(Event& e) override;
    private:
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
        void OnOverlayRender();

        void NewScene();
        void OpenScene();
        void OpenScene(const std::filesystem::path& path);
        void SaveScene();
        void SaveSceneAs();

        void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);


        void OnScenePlay();
        void OnSceneSimulate();
        void OnSceneStop();

        void OnDuplicateEntity() const;
        // UI Panels
        void UI_Toolbar();
    private:
        Ref<Framebuffer> m_EditorFrame;
        Ref<Framebuffer> m_CameraFrame;
        Ref<Framebuffer> m_RuntimeFrame;

        EditorCamera m_EditorCamera;

        Ref<Scene> m_ActiveScene;
        Ref<Scene> m_EditorScene;
        std::filesystem::path m_EditorScenePath;
        Entity m_HoveredEntity;

        bool m_ViewportFocused = false, m_ViewportHovered = false;
        glm::vec2 m_Editor_ViewportSize  = { 100.f, 100.f };
        glm::vec2 m_Camera_ViewportSize  = { 100.f, 100.f };
        glm::vec2 m_Runtime_ViewportSize = { 100.f, 100.f };

        glm::vec2 m_Editor_ViewportBounds[2];

        int m_GizmoType = 0b1111111111;
        bool m_ShowPhysicsColliders = false;


        enum class SceneState
        {
            Edit = 0, Play = 1, Simulate = 2
        };
        SceneState m_SceneState = SceneState::Edit;
        Ref<Texture2D> m_IconPlay, m_IconSimulate, m_IconStop;

        SceneHierarchyPanel m_SceneHierarchyPanel;
        ContentBrowserPanel m_ContentBrowserPanel;
    };

} // Aisite
