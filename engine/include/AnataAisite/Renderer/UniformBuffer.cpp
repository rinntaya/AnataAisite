#include "UniformBuffer.h"

#include "Renderer.h"
#include "platform/opengl/OpenGLUniformBuffer.h"

namespace Aisite {


    Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
    {
        switch (Renderer::GetAPI())
        {
            case Renderer::API::None:    AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case Renderer::API::OpenGL:  return CreateRef<OpenGLUniformBuffer>(size, binding);
        }

        AT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

} // Aisite