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

#define PROFILE_SCOPE(name) Timer timerxxxx##__func__##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })


Sandbox2D::Sandbox2D()

    : Layer("Example"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
    AT_PROFILE_FUNCTION();

    m_CheckerboardTexture = Aisite::Texture2D::Create("assets/textures/Checkerboard.png");
	Aisite::FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = Aisite::Framebuffer::Create(fbSpec);
}

void Sandbox2D::OnDetach()
{
    AT_PROFILE_FUNCTION();

    Layer::OnDetach();
}

void Sandbox2D::OnUpdate(const Aisite::Timestep ts)
{
    AT_PROFILE_FUNCTION();
    m_CameraController.OnUpdate(ts);


    ///// Renderer Code ///////////////////////////////////////////////

	m_Framebuffer->Bind();
    Aisite::Renderer2D::ResetStats();
    {
        AT_PROFILE_SCOPE("Renderer Prep");
        Aisite::RenderCommand::SetClearColor({.1f, .1f, .1f, 0});
        Aisite::RenderCommand::Clear();
    } {
        AT_PROFILE_SCOPE("Renderer Draw");
        Aisite::Renderer2D::BeginScene(m_CameraController.GetCamera());

		{       // for (int y = 0; y < 50; y++)
        // {
        //     for (int x = 0; x < 50; x++)
        //     {
        //         glm::vec2 pos = { x * 1.1f, y * 1.1f };     // 稍微留点间隔，不重叠
        //         glm::vec2 size = { 1.0f, 1.0f };
        //
        //         // 颜色随便弄点变化，这里用x,y简单混合一下
        //         glm::vec4 color = {
        //             (x % 10) / 10.0f,
        //             (y % 10) / 10.0f,
        //             0.5f,
        //             1.0f
        //         };
        //
        //         Aisite::Renderer2D::DrawQuad(pos, size, color);
        //     }
        // }}
	}

        static float rotation = 0.0f;
        rotation += ts * 50.0f;

        Aisite::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
        Aisite::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        Aisite::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);

        Aisite::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
        Aisite::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckerboardTexture, 20.0f);
        Aisite::Renderer2D::EndScene();


        Aisite::Renderer2D::BeginScene(m_CameraController.GetCamera());
        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
                Aisite::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
            }
        }
        Aisite::Renderer2D::EndScene();
    }

	m_Framebuffer->Unbind();
    ///////////////////////////////////////////////////////////////////
}

void Sandbox2D::OnDebugUIRender()
{
    AT_PROFILE_FUNCTION();


// Note: Switch this to true to enable dockspace
	static bool dockingEnabled = true;
	if (dockingEnabled)
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) Aisite::Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");

		auto stats = Aisite::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		size_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(textureID, ImVec2{ 1280, 720 });
		ImGui::End();

		ImGui::End();
	}
	else
	{
		ImGui::Begin("Settings");

		auto stats = Aisite::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		uint32_t textureID = m_CheckerboardTexture->GetRendererID();
		ImGui::Image(textureID, ImVec2{ 1280, 720});
		ImGui::End();
	}




    auto stats = Aisite::Renderer2D::GetStats();
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
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

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
