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
        unsigned int Width, Height;

        explicit WindowProps(std::string  title = "AnataAisite Engine / お願いい！愛して！！！",
                             const unsigned int width = 1280,
                             const unsigned int height = 720)
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

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        // Window attributes
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void* GetNativeWindow() const = 0;
        static Window* Create(const WindowProps& props = WindowProps());
    };
}
