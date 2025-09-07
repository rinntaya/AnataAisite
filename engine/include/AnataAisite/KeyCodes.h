#pragma once

namespace Aisite
{

typedef enum class KeyCode : uint16_t
	{
		Space               = 32,
		Apostrophe          = 39, /* ' */
		Comma               = 44, /* , */
		Minus               = 45, /* - */
		Period              = 46, /* . */
		Slash               = 47, /* / */

		D0                  = 48, /* 0 */
		D1                  = 49, /* 1 */
		D2                  = 50, /* 2 */
		D3                  = 51, /* 3 */
		D4                  = 52, /* 4 */
		D5                  = 53, /* 5 */
		D6                  = 54, /* 6 */
		D7                  = 55, /* 7 */
		D8                  = 56, /* 8 */
		D9                  = 57, /* 9 */

		Semicolon           = 59, /* ; */
		Equal               = 61, /* = */

		A                   = 65,
		B                   = 66,
		C                   = 67,
		D                   = 68,
		E                   = 69,
		F                   = 70,
		G                   = 71,
		H                   = 72,
		I                   = 73,
		J                   = 74,
		K                   = 75,
		L                   = 76,
		M                   = 77,
		N                   = 78,
		O                   = 79,
		P                   = 80,
		Q                   = 81,
		R                   = 82,
		S                   = 83,
		T                   = 84,
		U                   = 85,
		V                   = 86,
		W                   = 87,
		X                   = 88,
		Y                   = 89,
		Z                   = 90,

		LeftBracket         = 91,  /* [ */
		Backslash           = 92,  /* \ */
		RightBracket        = 93,  /* ] */
		GraveAccent         = 96,  /* ` */

		World1              = 161, /* non-US #1 */
		World2              = 162, /* non-US #2 */

		/* Function keys */
		Escape              = 256,
		Enter               = 257,
		Tab                 = 258,
		Backspace           = 259,
		Insert              = 260,
		Delete              = 261,
		Right               = 262,
		Left                = 263,
		Down                = 264,
		Up                  = 265,
		PageUp              = 266,
		PageDown            = 267,
		Home                = 268,
		End                 = 269,
		CapsLock            = 280,
		ScrollLock          = 281,
		NumLock             = 282,
		PrintScreen         = 283,
		Pause               = 284,
		F1                  = 290,
		F2                  = 291,
		F3                  = 292,
		F4                  = 293,
		F5                  = 294,
		F6                  = 295,
		F7                  = 296,
		F8                  = 297,
		F9                  = 298,
		F10                 = 299,
		F11                 = 300,
		F12                 = 301,
		F13                 = 302,
		F14                 = 303,
		F15                 = 304,
		F16                 = 305,
		F17                 = 306,
		F18                 = 307,
		F19                 = 308,
		F20                 = 309,
		F21                 = 310,
		F22                 = 311,
		F23                 = 312,
		F24                 = 313,
		F25                 = 314,

		/* Keypad */
		KP0                 = 320,
		KP1                 = 321,
		KP2                 = 322,
		KP3                 = 323,
		KP4                 = 324,
		KP5                 = 325,
		KP6                 = 326,
		KP7                 = 327,
		KP8                 = 328,
		KP9                 = 329,
		KPDecimal           = 330,
		KPDivide            = 331,
		KPMultiply          = 332,
		KPSubtract          = 333,
		KPAdd               = 334,
		KPEnter             = 335,
		KPEqual             = 336,

		LeftShift           = 340,
		LeftControl         = 341,
		LeftAlt             = 342,
		LeftSuper           = 343,
		RightShift          = 344,
		RightControl        = 345,
		RightAlt            = 346,
		RightSuper          = 347,
		Menu                = 348
	} Key;


	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}

    typedef enum class MouseCode : uint16_t
    {
        // From glfw3.h
        Button0                = 0,
        Button1                = 1,
        Button2                = 2,
        Button3                = 3,
        Button4                = 4,
        Button5                = 5,
        Button6                = 6,
        Button7                = 7,

        ButtonLast             = Button7,
        ButtonLeft             = Button0,
        ButtonRight            = Button1,
        ButtonMiddle           = Button2
    } Mouse;

    inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
    {
        os << static_cast<int32_t>(mouseCode);
        return os;
    }
}


#define AT_KEY_SPACE           ::Aisite::Key::Space
#define AT_KEY_APOSTROPHE      ::Aisite::Key::Apostrophe    /* ' */
#define AT_KEY_COMMA           ::Aisite::Key::Comma         /* , */
#define AT_KEY_MINUS           ::Aisite::Key::Minus         /* - */
#define AT_KEY_PERIOD          ::Aisite::Key::Period        /* . */
#define AT_KEY_SLASH           ::Aisite::Key::Slash         /* / */
#define AT_KEY_0               ::Aisite::Key::D0
#define AT_KEY_1               ::Aisite::Key::D1
#define AT_KEY_2               ::Aisite::Key::D2
#define AT_KEY_3               ::Aisite::Key::D3
#define AT_KEY_4               ::Aisite::Key::D4
#define AT_KEY_5               ::Aisite::Key::D5
#define AT_KEY_6               ::Aisite::Key::D6
#define AT_KEY_7               ::Aisite::Key::D7
#define AT_KEY_8               ::Aisite::Key::D8
#define AT_KEY_9               ::Aisite::Key::D9
#define AT_KEY_SEMICOLON       ::Aisite::Key::Semicolon     /* ; */
#define AT_KEY_EQUAL           ::Aisite::Key::Equal         /* = */
#define AT_KEY_A               ::Aisite::Key::A
#define AT_KEY_B               ::Aisite::Key::B
#define AT_KEY_C               ::Aisite::Key::C
#define AT_KEY_D               ::Aisite::Key::D
#define AT_KEY_E               ::Aisite::Key::E
#define AT_KEY_F               ::Aisite::Key::F
#define AT_KEY_G               ::Aisite::Key::G
#define AT_KEY_H               ::Aisite::Key::H
#define AT_KEY_I               ::Aisite::Key::I
#define AT_KEY_J               ::Aisite::Key::J
#define AT_KEY_K               ::Aisite::Key::K
#define AT_KEY_L               ::Aisite::Key::L
#define AT_KEY_M               ::Aisite::Key::M
#define AT_KEY_N               ::Aisite::Key::N
#define AT_KEY_O               ::Aisite::Key::O
#define AT_KEY_P               ::Aisite::Key::P
#define AT_KEY_Q               ::Aisite::Key::Q
#define AT_KEY_R               ::Aisite::Key::R
#define AT_KEY_S               ::Aisite::Key::S
#define AT_KEY_T               ::Aisite::Key::T
#define AT_KEY_U               ::Aisite::Key::U
#define AT_KEY_V               ::Aisite::Key::V
#define AT_KEY_W               ::Aisite::Key::W
#define AT_KEY_X               ::Aisite::Key::X
#define AT_KEY_Y               ::Aisite::Key::Y
#define AT_KEY_Z               ::Aisite::Key::Z
#define AT_KEY_LEFT_BRACKET    ::Aisite::Key::LeftBracket   /* [ */
#define AT_KEY_BACKSLASH       ::Aisite::Key::Backslash     /* \ */
#define AT_KEY_RIGHT_BRACKET   ::Aisite::Key::RightBracket  /* ] */
#define AT_KEY_GRAVE_ACCENT    ::Aisite::Key::GraveAccent   /* ` */
#define AT_KEY_WORLD_1         ::Aisite::Key::World1        /* non-US #1 */
#define AT_KEY_WORLD_2         ::Aisite::Key::World2        /* non-US #2 */

/* Function keys */
#define AT_KEY_ESCAPE          ::Aisite::Key::Escape
#define AT_KEY_ENTER           ::Aisite::Key::Enter
#define AT_KEY_TAB             ::Aisite::Key::Tab
#define AT_KEY_BACKSPACE       ::Aisite::Key::Backspace
#define AT_KEY_INSERT          ::Aisite::Key::Insert
#define AT_KEY_DELETE          ::Aisite::Key::Delete
#define AT_KEY_RIGHT           ::Aisite::Key::Right
#define AT_KEY_LEFT            ::Aisite::Key::Left
#define AT_KEY_DOWN            ::Aisite::Key::Down
#define AT_KEY_UP              ::Aisite::Key::Up
#define AT_KEY_PAGE_UP         ::Aisite::Key::PageUp
#define AT_KEY_PAGE_DOWN       ::Aisite::Key::PageDown
#define AT_KEY_HOME            ::Aisite::Key::Home
#define AT_KEY_END             ::Aisite::Key::End
#define AT_KEY_CAPS_LOCK       ::Aisite::Key::CapsLock
#define AT_KEY_SCROLL_LOCK     ::Aisite::Key::ScrollLock
#define AT_KEY_NUM_LOCK        ::Aisite::Key::NumLock
#define AT_KEY_PRINT_SCREEN    ::Aisite::Key::PrintScreen
#define AT_KEY_PAUSE           ::Aisite::Key::Pause
#define AT_KEY_F1              ::Aisite::Key::F1
#define AT_KEY_F2              ::Aisite::Key::F2
#define AT_KEY_F3              ::Aisite::Key::F3
#define AT_KEY_F4              ::Aisite::Key::F4
#define AT_KEY_F5              ::Aisite::Key::F5
#define AT_KEY_F6              ::Aisite::Key::F6
#define AT_KEY_F7              ::Aisite::Key::F7
#define AT_KEY_F8              ::Aisite::Key::F8
#define AT_KEY_F9              ::Aisite::Key::F9
#define AT_KEY_F10             ::Aisite::Key::F10
#define AT_KEY_F11             ::Aisite::Key::F11
#define AT_KEY_F12             ::Aisite::Key::F12
#define AT_KEY_F13             ::Aisite::Key::F13
#define AT_KEY_F14             ::Aisite::Key::F14
#define AT_KEY_F15             ::Aisite::Key::F15
#define AT_KEY_F16             ::Aisite::Key::F16
#define AT_KEY_F17             ::Aisite::Key::F17
#define AT_KEY_F18             ::Aisite::Key::F18
#define AT_KEY_F19             ::Aisite::Key::F19
#define AT_KEY_F20             ::Aisite::Key::F20
#define AT_KEY_F21             ::Aisite::Key::F21
#define AT_KEY_F22             ::Aisite::Key::F22
#define AT_KEY_F23             ::Aisite::Key::F23
#define AT_KEY_F24             ::Aisite::Key::F24
#define AT_KEY_F25             ::Aisite::Key::F25

/* Keypad */
#define AT_KEY_KP_0            ::Aisite::Key::KP0
#define AT_KEY_KP_1            ::Aisite::Key::KP1
#define AT_KEY_KP_2            ::Aisite::Key::KP2
#define AT_KEY_KP_3            ::Aisite::Key::KP3
#define AT_KEY_KP_4            ::Aisite::Key::KP4
#define AT_KEY_KP_5            ::Aisite::Key::KP5
#define AT_KEY_KP_6            ::Aisite::Key::KP6
#define AT_KEY_KP_7            ::Aisite::Key::KP7
#define AT_KEY_KP_8            ::Aisite::Key::KP8
#define AT_KEY_KP_9            ::Aisite::Key::KP9
#define AT_KEY_KP_DECIMAL      ::Aisite::Key::KPDecimal
#define AT_KEY_KP_DIVIDE       ::Aisite::Key::KPDivide
#define AT_KEY_KP_MULTIPLY     ::Aisite::Key::KPMultiply
#define AT_KEY_KP_SUBTRACT     ::Aisite::Key::KPSubtract
#define AT_KEY_KP_ADD          ::Aisite::Key::KPAdd
#define AT_KEY_KP_ENTER        ::Aisite::Key::KPEnter
#define AT_KEY_KP_EQUAL        ::Aisite::Key::KPEqual

#define AT_KEY_LEFT_SHIFT      ::Aisite::Key::LeftShift
#define AT_KEY_LEFT_CONTROL    ::Aisite::Key::LeftControl
#define AT_KEY_LEFT_ALT        ::Aisite::Key::LeftAlt
#define AT_KEY_LEFT_SUPER      ::Aisite::Key::LeftSuper
#define AT_KEY_RIGHT_SHIFT     ::Aisite::Key::RightShift
#define AT_KEY_RIGHT_CONTROL   ::Aisite::Key::RightControl
#define AT_KEY_RIGHT_ALT       ::Aisite::Key::RightAlt
#define AT_KEY_RIGHT_SUPER     ::Aisite::Key::RightSuper
#define AT_KEY_MENU            ::Aisite::Key::Menu


#define AT_MOUSE_BUTTON_0      ::Aisite::Mouse::Button0
#define AT_MOUSE_BUTTON_1      ::Aisite::Mouse::Button1
#define AT_MOUSE_BUTTON_2      ::Aisite::Mouse::Button2
#define AT_MOUSE_BUTTON_3      ::Aisite::Mouse::Button3
#define AT_MOUSE_BUTTON_4      ::Aisite::Mouse::Button4
#define AT_MOUSE_BUTTON_5      ::Aisite::Mouse::Button5
#define AT_MOUSE_BUTTON_6      ::Aisite::Mouse::Button6
#define AT_MOUSE_BUTTON_7      ::Aisite::Mouse::Button7
#define AT_MOUSE_BUTTON_LAST   ::Aisite::Mouse::ButtonLast
#define AT_MOUSE_BUTTON_LEFT   ::Aisite::Mouse::ButtonLeft
#define AT_MOUSE_BUTTON_RIGHT  ::Aisite::Mouse::ButtonRight
#define AT_MOUSE_BUTTON_MIDDLE ::Aisite::Mouse::ButtonMiddle