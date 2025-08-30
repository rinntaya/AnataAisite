#include "icon/engine_icon.h"
#include "stb/stb_image.h"

#include "WindowsWindow.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "AnataAisite/Log.h"
#include "AnataAisite/Events/ApplicationEvent.h"
#include "AnataAisite/Events/KeyEvent.h"
#include "AnataAisite/Events/MouseEvent.h"
#include "platform/opengl/OpenGLContext.h"

// TODO: 当前直接使用ImGUI的方法，以后替换为自己的虚拟按键码
static int ImGui_ImplGlfw_TranslateUntranslatedKey(int key, int scancode)
{
#if GLFW_HAS_GETKEYNAME && !defined(EMSCRIPTEN_USE_EMBEDDED_GLFW3)
    // GLFW 3.1+ attempts to "untranslate" keys, which goes the opposite of what every other framework does, making using lettered shortcuts difficult.
    // (It had reasons to do so: namely GLFW is/was more likely to be used for WASD-type game controls rather than lettered shortcuts, but IHMO the 3.1 change could have been done differently)
    // See https://github.com/glfw/glfw/issues/1502 for details.
    // Adding a workaround to undo this (so our keys are translated->untranslated->translated, likely a lossy process).
    // This won't cover edge cases but this is at least going to cover common cases.
    if (key >= GLFW_KEY_KP_0 && key <= GLFW_KEY_KP_EQUAL)
        return key;
    GLFWerrorfun prev_error_callback = glfwSetErrorCallback(nullptr);
    const char* key_name = glfwGetKeyName(key, scancode);
    glfwSetErrorCallback(prev_error_callback);
#if GLFW_HAS_GETERROR && !defined(EMSCRIPTEN_USE_EMBEDDED_GLFW3) // Eat errors (see #5908)
    (void)glfwGetError(nullptr);
#endif
    if (key_name && key_name[0] != 0 && key_name[1] == 0)
    {
        const char char_names[] = "`-=[]\\,;\'./";
        const int char_keys[] = { GLFW_KEY_GRAVE_ACCENT, GLFW_KEY_MINUS, GLFW_KEY_EQUAL, GLFW_KEY_LEFT_BRACKET, GLFW_KEY_RIGHT_BRACKET, GLFW_KEY_BACKSLASH, GLFW_KEY_COMMA, GLFW_KEY_SEMICOLON, GLFW_KEY_APOSTROPHE, GLFW_KEY_PERIOD, GLFW_KEY_SLASH, 0 };
        IM_ASSERT(IM_ARRAYSIZE(char_names) == IM_ARRAYSIZE(char_keys));
        if (key_name[0] >= '0' && key_name[0] <= '9')               { key = GLFW_KEY_0 + (key_name[0] - '0'); }
        else if (key_name[0] >= 'A' && key_name[0] <= 'Z')          { key = GLFW_KEY_A + (key_name[0] - 'A'); }
        else if (key_name[0] >= 'a' && key_name[0] <= 'z')          { key = GLFW_KEY_A + (key_name[0] - 'a'); }
        else if (const char* p = strchr(char_names, key_name[0]))   { key = char_keys[p - char_names]; }
    }
    // if (action == GLFW_PRESS) printf("key %d scancode %d name '%s'\n", key, scancode, key_name);
#else
    // IM_UNUSED(scancode);
#endif
    return key;
}


namespace Aisite
{
    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description)
    {
        AT_CORE_ERROR("GLFW Error({0}): {1}", error, description);
    }


    Window* Window::Create(const WindowProps& props)
    {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        WindowsWindow::Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        WindowsWindow::Shutdown();
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        AT_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        if (!s_GLFWInitialized) {
            // TODO: glfwTerminate on system shutdown
            const int success = glfwInit();
            AT_CORE_ASSERT(success, "Could not intialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        glfwGetMonitorContentScale(monitor, &m_DpiX, &m_DpiY);
        AT_CORE_DEBUG("first screen dpi X: {} Y: {}", m_DpiX, m_DpiY);

        m_Window = glfwCreateWindow((int) (props.Width * m_DpiX), (int) (props.Height * m_DpiX),
                                    m_Data.Title.c_str(), nullptr, nullptr);
        m_Context = new OpenGLContext(m_Window);

        int width, height, channels;
        if (unsigned char* pixels = stbi_load_from_memory(__engine_icon_png, (int) __engine_icon_png_len, &width,
                                                          &height,
                                                          &channels, 4)) {
            GLFWimage image;
            image.width = width;
            image.height = height;
            image.pixels = pixels;
            glfwSetWindowIcon(m_Window, 1, &image);
            stbi_image_free(pixels);
        }

        // HWND hwnd = glfwGetWin32Window(m_Window);
        // ImmAssociateContext(hwnd, NULL); // 禁用 IME

        m_Context->Init();
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);



        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, const int width, const int height) {
            WindowData* data = (WindowData *) glfwGetWindowUserPointer(window);

            data->Width = width;
            data->Height = height;

            WindowResizeEvent event(width, height);
            data->EventCallback(event);
        });
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            const WindowData* data = (WindowData *) glfwGetWindowUserPointer(window);

            WindowCloseEvent event;
            data->EventCallback(event);
        });
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            const WindowData* data = (WindowData *) glfwGetWindowUserPointer(window);
            const int keycode = ImGui_ImplGlfw_TranslateUntranslatedKey(key, scancode);

            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(keycode, 0);
                    data->EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(keycode);
                    data->EventCallback(event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent event(keycode, 1);
                    data->EventCallback(event);
                    break;
                }
                default: break;
            }
        });
        glfwSetCharCallback(m_Window, [](GLFWwindow* window, const unsigned int keycode) {
            const WindowData* data = (WindowData *) glfwGetWindowUserPointer(window);

            KeyTypedEvent event(keycode);
            data->EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
            const WindowData* data = (WindowData *) glfwGetWindowUserPointer(window);
            const int keycode = button;

            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(keycode);
                    data->EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(keycode);
                    data->EventCallback(event);
                    break;
                }

                default: break;
            }
        });
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
            const WindowData* data = (WindowData *) glfwGetWindowUserPointer(window);

            MouseScrolledEvent event(xoffset, yoffset);
            data->EventCallback(event);
        });
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
            const WindowData* data = (WindowData *) glfwGetWindowUserPointer(window);

            MouseMovedEvent event(xpos, ypos);
            data->EventCallback(event);
        });
    }
    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        m_Context->SwapBuffers();
        // TitleFps();
    }

    void WindowsWindow::SetVSync(const bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }


}
