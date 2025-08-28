#include <GLFW/glfw3.h>


#include "AnataAisite/Application.h"
#include "AnataAisite/ImGuiLayer.h"
#include "renderer/RenderCommand.h"

namespace Aisite
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
    Application* Application::s_Instance = nullptr;

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
    bool Application::OnWindowClose(const WindowCloseEvent& _)
    {
        m_Running = false;
        return true;
    }


   void Application::Run()
    {
        while (m_Running) {
            RenderCommand::SetClearColor({.1f, .1f, .1f, 0});
            RenderCommand::Clear();

            const float time = (float)glfwGetTime();
            const Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            m_UnSimulatedTime += timestep;
            while (m_UnSimulatedTime >= FixedDeltaTime)
                for (Layer* layer: m_LayerStack) layer->OnFixedUpdate(),
                m_UnSimulatedTime -= FixedDeltaTime;


            for (Layer* layer: m_LayerStack) layer->OnUpdate(timestep);
            m_ImGuiLayer->Begin();
                for (Layer* layer: m_LayerStack) layer->OnDebugUIRender();
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }




    Application::Application()
    {
        AT_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
        // m_Window->SetVSync(false);

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }
    Application::~Application() = default;
}
