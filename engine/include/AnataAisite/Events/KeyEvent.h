#pragma once

#include "Event.h"
#include "AnataAisite/KeyCodes.h"


namespace Aisite {

    class AISITE_API KeyEvent : public Event
    {
    public:
        [[nodiscard]] inline KeyCode GetKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    protected:
        explicit KeyEvent(KeyCode keycode)
            : m_KeyCode(keycode) {}

        KeyCode m_KeyCode;
    };

    class AISITE_API KeyPressedEvent final : public KeyEvent
    {
    public:
        KeyPressedEvent(const KeyCode keycode, uint16_t repeatCount)
            : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

        [[nodiscard]] inline uint16_t GetRepeatCount() const { return m_RepeatCount; }
        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        uint16_t m_RepeatCount;
    };

    class AISITE_API KeyReleasedEvent final : public KeyEvent
    {
    public:
        explicit KeyReleasedEvent(const KeyCode keycode): KeyEvent(keycode) {}

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };


    class AISITE_API KeyTypedEvent : public Event
    {
    public:
        explicit KeyTypedEvent(unsigned int unicode)
            : m_Unicode(unicode) {}
    public:
        [[nodiscard]] inline unsigned int GetChar() const { return m_Unicode; }
        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_Unicode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    protected:
        unsigned int m_Unicode;
    };
}
