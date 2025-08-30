#pragma once
#include "Camera.h"
#include "Texture.h"


namespace Aisite {

    class AISITE_API Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

        // Primitives
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
        {
            DrawQuad({position.x, position.y, 0.0f}, size, color);
        }


        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);
        inline static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
        {
            DrawQuad({position.x, position.y, 0.0f}, size, texture);
        }
    };


}

