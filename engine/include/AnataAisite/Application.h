#pragma once

#include "AnataAisite/Window.h"
#include "AnataAisite/Core.h"
#include "AnataAisite/Events/Event.h"
#include "AnataAisite/Events/ApplicationEvent.h"
#include "AnataAisite/LayerStack.h"


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
        inline float GetDPI() const { return m_DPI; }
        inline static Application& Get() { return *s_Instance; }

        void OnEvent(Event& e) ;

    private:
        bool OnWindowClose(const WindowCloseEvent& _);


        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        float m_DPI = 1.f;

        LayerStack m_LayerStack;
    private:
        static Application* s_Instance;
    };


    Application* CreateApplication();
}
