#include "AnataAisite/Renderer/VertexArray.h"
#include "AnataAisite/Renderer/Renderer.h"


#include "platform/opengl/OpenGLVertexArray.h"

namespace Aisite {

    VertexArray* VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
            case Renderer::API::None:    AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case Renderer::API::OpenGL:  return new OpenGLVertexArray();
        }

        AT_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}
