#include <GLFW/glfw3.h>


#include "AnataAisite/Application.h"
#include "AnataAisite/ImGuiLayer.h"
#include "AnataAisite/Renderer/RenderCommand.h"
#include "AnataAisite/Renderer/Renderer.h"

namespace Aisite
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
    Application* Application::s_Instance = nullptr;

    void Application::PushLayer(Layer* layer)
    {
        AT_PROFILE_FUNCTION();
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }
    void Application::PushOverlay(Layer* layer)
    {
        AT_PROFILE_FUNCTION();
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }
    void Application::OnEvent(Event& e)
    {
        AT_PROFILE_FUNCTION();
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
            if (e.Handled)
                break;
            (*--it)->OnEvent(e);
        }
    }
    bool Application::OnWindowClose(const WindowCloseEvent& _)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        AT_PROFILE_FUNCTION();

        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return false;
    }


    void Application::Run()
    {
        AT_PROFILE_FUNCTION();

        while (m_Running) {

            AT_PROFILE_SCOPE("RunLoop");

            const float time = (float)glfwGetTime();
            const Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            m_UnSimulatedTime += timestep;
            while (m_UnSimulatedTime >= FixedDeltaTime)
                for (Layer* layer: m_LayerStack) layer->OnFixedUpdate(),
                m_UnSimulatedTime -= FixedDeltaTime;



            if (!m_Minimized) {
                AT_PROFILE_SCOPE("LayerStack OnUpdate");
                for (Layer* layer : m_LayerStack) layer->OnUpdate(timestep);
            };
            m_ImGuiLayer->Begin();
            {
                AT_PROFILE_SCOPE("LayerStack OnDebugUIRender");
                for (Layer* layer: m_LayerStack) layer->OnDebugUIRender();
            }
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    void Application::Close()
    {
		m_Running = false;
    }


    Application::Application(const ApplicationSpecification& specification)
        : m_Specification(specification)
    {
        AT_PROFILE_FUNCTION();
        AT_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;


        // Set working directory here
        if (!m_Specification.WorkingDirectory.empty())
            std::filesystem::current_path(m_Specification.WorkingDirectory);

        m_Window = Window::Create(WindowProps(m_Specification.Name));
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
        // m_Window->SetVSync(false);

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }
    Application::~Application() {
        AT_PROFILE_FUNCTION();
        Renderer::Shutdown();
    };
}
