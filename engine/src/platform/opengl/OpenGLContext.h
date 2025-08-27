#pragma once
#include "renderer/GraphicsContext.h"


struct GLFWwindow;

namespace Aisite
{
    class OpenGLContext : public GraphicsContext
    {
    public:
        explicit OpenGLContext(GLFWwindow* window);

        void Init() override;
        void SwapBuffers() override;

    private:
        GLFWwindow* m_WindowHandle;
    };

}


