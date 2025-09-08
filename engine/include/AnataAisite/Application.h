#pragma once

#include "Window.h"
#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "ImGuiLayer.h"


namespace Aisite
{

    struct AISITE_API ApplicationCommandLineArgs
    {
        int Count = 0;
        char** Args = nullptr;

        const char* operator[](int index) const
        {
            AT_CORE_ASSERT(index < Count);
            return Args[index];
        }
    };

    class AISITE_API Application
    {
    public:
        Application(const std::string& name = "Aisite Player", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
        ~Application();

        void Run();
        void Close();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        inline Window& GetWindow() const { return *m_Window; }
        ImGuiLayer* GetImGuiLayer() const { return m_ImGuiLayer; }
        inline static Application& Get() { return *s_Instance; }

        void OnEvent(Event& e) ;

        ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }

    private:
        bool OnWindowClose(const WindowCloseEvent& _);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        static Application* s_Instance;
        ApplicationCommandLineArgs m_CommandLineArgs;

        Scope<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;


        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;
        float m_UnSimulatedTime = 0.0f;
    public:
        float FixedDeltaTime = 0.01f;

    };


    Application* CreateApplication(ApplicationCommandLineArgs args);
}
