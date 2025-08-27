#include "AnataAisite/Renderer/Renderer.h"
#include "RenderCommand.h"


namespace Aisite {

    void Renderer::BeginScene()
    {
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
    {
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

    Renderer::API Renderer::s_API = API::OpenGL;

}
