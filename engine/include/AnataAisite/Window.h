#pragma once
#include <_atpch.h>

#include <utility>

#include "Core.h"
#include "Events/Event.h"


namespace Aisite
{
    struct WindowProps
    {
        std::string Title;
        uint32_t Width, Height;
        float XDpi, YDpi;

        explicit WindowProps(std::string  title = "AnataAisite Engine / お願いい！愛して！！！",
                             const uint32_t width = 1280 + 200,
                             const uint32_t height = 720 + 200)
        : Title(std::move(title)), Width(width), Height(height)
        {
        }
    };


    // Interface representing a desktop system based Window
    class AISITE_API Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;

        virtual void OnUpdate() = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        // Window attributes
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;
        virtual float GetDpi(float* vec2 = nullptr) = 0;

        virtual void* GetNativeWindow() const = 0;
        static Scope<Window> Create(const WindowProps& props = WindowProps());
    };
}
