#pragma once
#include "AnataAisite.h"
#include "AnataAisite/Events/KeyEvent.h"
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

        void NewScene();
        void OpenScene();
        void SaveSceneAs() const;
    private:
        Aisite::OrthographicCameraController m_CameraController;

        // Ref<VertexArray> m_SquareVA;
        // Ref<Shader> m_FlatColorShader;
        Ref<Framebuffer> m_Framebuffer;
        Ref<Framebuffer> m_RuntimeFrame;
        EditorCamera m_EditorCamera;

        Ref<Scene> m_ActiveScene;
        Entity m_HoveredEntity;

        // Entity m_SquareEntity;
        // Entity m_CameraEntity;
        // Entity m_SecondCamera;

        // bool m_PrimaryCamera = true;




        Ref<Texture2D> m_CheckerboardTexture;

        bool m_ViewportFocused = false, m_ViewportHovered = false;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        glm::vec2 m_Runtime_ViewportSize = { 0.f, 0.f };

        glm::vec2 m_ViewportBounds[2];

        int m_GizmoType = -1;
        SceneHierarchyPanel m_SceneHierarchyPanel;
    };

} // Aisite
