#include "AnataAisite/Renderer/Framebuffer.h"

#include "platform/opengl/OpenGLFramebuffer.h"

namespace Aisite {

    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
    {
        switch (Renderer::GetAPI())
        {
            case Renderer::API::None:    AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case Renderer::API::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
        }

        AT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

} // Aisite