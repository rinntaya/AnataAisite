
#pragma once
#include "Camera.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Aisite {


    class AISITE_API Renderer
    {
    public:
        enum class API
        {
            None = 0, OpenGL = 1
        };

    public:
		static void Init();

        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();

        static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));


        inline static API GetAPI() { return s_API; }

        static void OnWindowResize(uint32_t width, uint32_t height);
    private:
        static API s_API;
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneData* s_SceneData;
    };


}
