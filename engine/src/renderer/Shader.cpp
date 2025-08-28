#include "AnataAisite/Renderer/Shader.h"

#include <glad/glad.h>

#include "AnataAisite/Renderer/Renderer.h"
#include "platform/opengl/OpenGLShader.h"


namespace Aisite
{
    Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        switch (Renderer::GetAPI()) {
            case Renderer::API::None: AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case Renderer::API::OpenGL: return new OpenGLShader(vertexSrc, fragmentSrc);
        }

        AT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
