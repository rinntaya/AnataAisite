#include "AnataAisite/Renderer/Renderer.h"
#include "AnataAisite/Renderer/RenderCommand.h"
#include "AnataAisite/Renderer/Renderer2D.h"
#include "platform/opengl/OpenGLShader.h"


namespace Aisite {

    Renderer::SceneData* Renderer::s_SceneData  = new SceneData;

    void Renderer::Init()
    {
        RenderCommand::Init();
        Renderer2D::Init();
    }


    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->Bind();

        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

    Renderer::API Renderer::s_API = API::OpenGL;


    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }
}
