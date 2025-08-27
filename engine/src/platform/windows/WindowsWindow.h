#pragma once
#include "AnataAisite/Window.h"
#include "renderer/GraphicsContext.h"

struct GLFWwindow;
namespace Aisite
{

    class WindowsWindow : public Window
    {
    public:
        explicit WindowsWindow(const WindowProps& props);
        ~WindowsWindow() override;

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }

        // Window attributes
        inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;
        inline float GetDpi(float* vec2) override
        {
            if (vec2) vec2[0] = m_DpiX, vec2[1] = m_DpiY;
            return m_DpiX;
        };

        inline void* GetNativeWindow() const override { return m_Window; }

    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();


    private:
        GLFWwindow* m_Window;
        GraphicsContext* m_Context;

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        float m_DpiX, m_DpiY;
        WindowData m_Data;
    };

}
