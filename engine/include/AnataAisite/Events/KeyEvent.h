#pragma once

#include "Event.h"


namespace Aisite {

    class AISITE_API KeyEvent : public Event
    {
    public:
        [[nodiscard]] inline int GetKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    protected:
        explicit KeyEvent(int keycode)
            : m_KeyCode(keycode) {}

        int m_KeyCode;
    };

    class AISITE_API KeyPressedEvent final : public KeyEvent
    {
    public:
        KeyPressedEvent(const int keycode, int repeatCount)
            : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

        [[nodiscard]] inline int GetRepeatCount() const { return m_RepeatCount; }
        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int m_RepeatCount;
    };

    class AISITE_API KeyReleasedEvent final : public KeyEvent
    {
    public:
        explicit KeyReleasedEvent(const int keycode): KeyEvent(keycode) {}

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };


    class AISITE_API KeyTypedEvent : public KeyEvent
    {
    public:
        KeyTypedEvent(int keycode)
            : KeyEvent(keycode) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };
}