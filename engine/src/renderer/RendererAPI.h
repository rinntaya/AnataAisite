#pragma once


#include "AnataAisite/Renderer/VertexArray.h"
#include "glm/vec4.hpp"


namespace Aisite
{
    class RendererAPI
    {

    public:
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
    };
}
