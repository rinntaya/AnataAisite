#include "AnataAisite/Renderer/RenderCommand.h"

#include "platform/opengl/OpenGLRendererAPI.h"


namespace Aisite
{

    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}
