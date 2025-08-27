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


        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        float m_DPI = 1.5f;

        LayerStack m_LayerStack;

    private:
        static Application* s_Instance;

    };


    Application* CreateApplication();
}
