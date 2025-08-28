#pragma once
#include "AnataAisite/Renderer/VertexArray.h"
#include "../../renderer/RendererAPI.h"


namespace Aisite {


    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
    };


} // Aisite
