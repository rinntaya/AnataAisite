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
        KeyPressedEvent(const KeyCode keycode, bool isRepeat = false)
            : KeyEvent(keycode), m_IsRepeat(isRepeat) {}

        bool IsRepeat() const { return m_IsRepeat; }
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_KeyCode << " (repeat = " << m_IsRepeat << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        uint16_t m_IsRepeat;
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
