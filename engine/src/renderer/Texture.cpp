
#include "AnataAisite/Renderer/Texture.h"
#include "AnataAisite/Renderer/Renderer.h"
#include "platform/opengl/OpenGLTexture.h"

namespace Aisite {

    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetAPI()) {
            case Renderer::API::None: AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
            case Renderer::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
        }

        AT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

} // Aisite