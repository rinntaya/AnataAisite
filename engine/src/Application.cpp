#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "AnataAisite/Application.h"


#include "AnataAisite/Input.h"

namespace Aisite
{
#define BIND_EVENT_FN(x) std::bind_front(&Application::x, this)
    Application* Application::s_Instance = nullptr;

    Application::~Application() = default;

    Application::Application()
    {
        AT_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        float xscale, yscale;
        glfwGetMonitorContentScale(monitor, &xscale, &yscale);
        m_DPI = xscale;
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }


    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }


    void Application::Run()
    {
        while (m_Running) {
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            for (Layer* layer: m_LayerStack) layer->OnUpdate();
            m_Window->OnUpdate();
        }
    }


    bool Application::OnWindowClose(const WindowCloseEvent& _)
    {
        m_Running = false;
        return true;
    }
}
