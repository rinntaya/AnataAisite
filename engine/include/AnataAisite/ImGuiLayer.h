#pragma once

#include "Layer.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace Aisite {

    class AISITE_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnEvent(Event& event) override;
    private:
        float m_Time = 0.0f;
    private:
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e) const;
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e) const;
        bool OnMouseMovedEvent(MouseMovedEvent& e) const;
        bool OnMouseScrolledEvent(MouseScrolledEvent& e) const;
        bool OnKeyPressedEvent(KeyPressedEvent& e) const;
        bool OnKeyReleasedEvent(KeyReleasedEvent& e) const;
        bool OnKeyTypedEvent(KeyTypedEvent& e) const;
        bool OnWindowResizeEvent(WindowResizeEvent& e) const;
    };

}

