#pragma once

#include "Window.h"
#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "ImGuiLayer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"


namespace Aisite
{
    class AISITE_API Application
    {
    public:
        Application();
        ~Application();

        void Run();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        inline Window& GetWindow() const { return *m_Window; }
        inline static Application& Get() { return *s_Instance; }

        void OnEvent(Event& e) ;

    private:
        bool OnWindowClose(const WindowCloseEvent& _);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        static Application* s_Instance;

        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;


        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;
        float m_UnSimulatedTime = 0.0f;
    public:
        float FixedDeltaTime = 0.01f;

    };


    Application* CreateApplication();
}
