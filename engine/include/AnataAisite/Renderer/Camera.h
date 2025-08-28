#pragma once


#include <glm/glm.hpp>

namespace Aisite
{
    class AISITE_API OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);

        const glm::vec3& Position() const { return m_Position; }
        void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
        void AddPosition(const glm::vec3& position) { m_Position += position; RecalculateViewMatrix(); }

        float Rotation() const { return m_Rotation; }
        void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
        void AddRotation(float rotation) { m_Rotation += rotation; RecalculateViewMatrix(); }

        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
        void FlushProjectionMatrix(float left, float right, float bottom, float top);
    private:
        void RecalculateViewMatrix();
    private:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        glm::vec3 m_Position{ 0.f };
        float m_Rotation = 0.f;
    };
}

