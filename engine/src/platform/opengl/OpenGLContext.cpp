#include "OpenGLContext.h"

#include <glad/glad.h>

#include "GLFW/glfw3.h"

Aisite::OpenGLContext::OpenGLContext(GLFWwindow* window)
    :m_WindowHandle(window)
{
    AT_CORE_ASSERT(m_WindowHandle, "Windows Handle is Null!");

}

void Aisite::OpenGLContext::Init()
{
    AT_PROFILE_FUNCTION();

    glfwMakeContextCurrent(m_WindowHandle);
    const int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    AT_CORE_ASSERT(status, "Failed to initialize Glad!");



    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(m_WindowHandle);

    AT_CORE_INFO("OpenGL Information: ");
    AT_CORE_INFO("      Vendor: {0}", (const char*)glGetString(GL_VENDOR));
    AT_CORE_INFO("      Renderer: {0}", (const char*)glGetString(GL_RENDERER));
    AT_CORE_INFO("      Version: {0}", (const char*)glGetString(GL_VERSION));

    AT_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Requires at least OpenGL version 4.5!");
}

void Aisite::OpenGLContext::SwapBuffers()
{
    AT_PROFILE_FUNCTION();

    glfwSwapBuffers(m_WindowHandle);
}
