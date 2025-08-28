#pragma once
#include "AnataAisite/Window.h"
#include "GLFW/glfw3.h"
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


        inline void TitleFps() const
        {
            static double time0 = glfwGetTime();
            static double time1;
            static double dt;
            static int dframe = -1;
            static std::stringstream info;
            time1 = glfwGetTime();
            dframe++;
            if ((dt = time1 - time0) >= 1) {
                info.precision(1);
                info << m_Data.Title << "                 " << std::fixed << dframe / dt << " FPS";
                glfwSetWindowTitle(m_Window, info.str().c_str());
                info.str(""); //别忘了在设置完窗口标题后清空所用的stringstream
                time0 = time1;
                dframe = 0;
            }
        }


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
