#include <AnataAisite.h>

#include "AnataAisite/Input.h"

class ExampleLayer : public Aisite::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {
    }

    void OnUpdate() override
    {

        if (Aisite::Input::IsKeyPressed(AT_KEY_TAB))
            AT_TRACE("Tab key is pressed (poll)!");
        auto [x, y] = Aisite::Input::GetMousePosition();

        // AT_INFO("mouse position ({},{})", x, y);
        // AT_INFO("ExampleLayer::Update");

        // AT_ASSERT(!Aisite::Input::IsKeyPressed(65), "Key \"a\" Down")
    }

    void OnEvent(Aisite::Event& event) override
    {
        if (event.GetEventType() == Aisite::EventType::KeyPressed)
        {
            Aisite::KeyPressedEvent& e = (Aisite::KeyPressedEvent&)event;
            if (e.GetKeyCode() == AT_KEY_TAB)
                AT_TRACE("Tab key is pressed (event)!");
            AT_TRACE("{0}", (char)e.GetKeyCode());
        }
    }

};


class Sandbox : public Aisite::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
        PushOverlay(new Aisite::ImGuiLayer());
    }
};

AISITE_APP(Sandbox);