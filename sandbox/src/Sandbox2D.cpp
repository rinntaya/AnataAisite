#include "Sandbox2D.h"

#include <imgui.h>

#include "AnataAisite/Renderer/RenderCommand.h"
#include "glm/ext/matrix_transform.hpp"
#include "platform/opengl/OpenGLShader.h"


#include <chrono>

#include "glm/gtc/type_ptr.hpp"

template<typename Fn>
class Timer
{
public:
    Timer(const char* name, Fn&& func)
        : m_Name(name), m_Func(func), m_Stopped(false)
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        if (!m_Stopped)
            Stop();
    }

    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().
                count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().
                count();

        m_Stopped = true;

        float duration = (end - start) * 0.001f;
        m_Func({m_Name, duration});
    }

private:
    const char* m_Name;
    Fn m_Func;
    std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
    bool m_Stopped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })


Sandbox2D::Sandbox2D()

    : Layer("Example"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
    m_CheckerboardTexture = Aisite::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
    Layer::OnDetach();
}

void Sandbox2D::OnUpdate(const Aisite::Timestep ts)
{
    PROFILE_SCOPE("Sandbox2D::OnUpdate"); {
        PROFILE_SCOPE("CameraController::OnUpdate");
        m_CameraController.OnUpdate(ts);
    }


    ///// Renderer Code ///////////////////////////////////////////////
    {
        PROFILE_SCOPE("Renderer Prep");
        Aisite::RenderCommand::SetClearColor({.1f, .1f, .1f, 0});
        Aisite::RenderCommand::Clear();
    } {
        PROFILE_SCOPE("Renderer Draw");
        Aisite::Renderer2D::BeginScene(m_CameraController.GetCamera());
        Aisite::Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
        Aisite::Renderer2D::DrawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, {0.2f, 0.3f, 0.8f, 1.0f});
        Aisite::Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {10.0f, 10.0f}, m_CheckerboardTexture);
        Aisite::Renderer2D::EndScene();
    }

    ///////////////////////////////////////////////////////////////////
}

void Sandbox2D::OnDebugUIRender()
{
    Aisite::OrthographicCamera m_Camera = m_CameraController.GetCamera();

    glm::vec3 position_display = m_Camera.Position();
    float rotation_display = m_Camera.Rotation();
    ImGui::Begin("Settings");
    ImGui::Text("Camera Position:");
    if (ImGui::SliderFloat3("P", &position_display.x, -100.f, 100.f))
        m_Camera.SetPosition(position_display);
    if (ImGui::SliderFloat("R", &rotation_display, -360.f, 360.f))
        m_Camera.SetRotation(rotation_display);

    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();


    static int location = 0;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                                    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                    ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (location >= 0) {
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoMove;
    }

    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("Debug Overlay", nullptr, window_flags)) {
        ImGui::Text("Debug Overlay");
        ImGui::Separator();
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        for (auto& result: m_ProfileResults) {
            ImGui::Text("%.3fms %s", result.Time, result.Name);
        }
        m_ProfileResults.clear();
        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::MenuItem("Custom", NULL, location == -1)) location = -1;
            if (ImGui::MenuItem("Top-left", NULL, location == 0)) location = 0;
            if (ImGui::MenuItem("Top-right", NULL, location == 1)) location = 1;
            if (ImGui::MenuItem("Bottom-left", NULL, location == 2)) location = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void Sandbox2D::OnEvent(Aisite::Event& e)
{
    m_CameraController.OnEvent(e);
}
