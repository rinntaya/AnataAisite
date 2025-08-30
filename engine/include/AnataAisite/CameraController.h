#pragma once

#include "AnataAisite/Renderer/Camera.h"
#include "AnataAisite/Timestep.h"

#include "AnataAisite/Events/ApplicationEvent.h"
#include "AnataAisite/Events/MouseEvent.h"

namespace Aisite
{
    class AISITE_API OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRatio, bool rotation = false);

        void OnUpdate(Timestep deltaTime);
        void OnEvent(Event& e);

        OrthographicCamera& GetCamera() { return m_Camera; }
        const OrthographicCamera& GetCamera() const { return m_Camera; }

        float GetZoomLevel() const { return m_ZoomLevel; }
        void SetZoomLevel(float level) { m_ZoomLevel = level; }

    private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);

    private:
        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;
        OrthographicCamera m_Camera;

        bool m_Rotation;

        glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
        float m_CameraRotation = 0.0f;
        float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
    };
}
