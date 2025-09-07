#include "EditorLayer.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Aisite {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
	}

	void EditorLayer::OnAttach()
	{
		AT_PROFILE_FUNCTION();

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);


		m_ActiveScene = CreateRef<Scene>();
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		// Entity
		Entity square = m_ActiveScene->CreateEntity("Green Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		m_SquareEntity = square;


		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;




		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				AT_DEBUG("Native Script Create!");
				auto& transform = GetComponent<TransformComponent>().Transform;
				transform[3][0] = rand() % 10 - 5.0f;
			}

			void OnDestroy()
			{
			}

			void OnUpdate(Timestep ts)
			{
				auto& transform = GetComponent<TransformComponent>().Transform;
				float speed = 5.0f;

				if (Input::IsKeyPressed(KeyCode::A))
					transform[3][0] -= speed * ts;
				if (Input::IsKeyPressed(KeyCode::D))
					transform[3][0] += speed * ts;
				if (Input::IsKeyPressed(KeyCode::W))
					transform[3][1] += speed * ts;
				if (Input::IsKeyPressed(KeyCode::S))
					transform[3][1] -= speed * ts;
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
	}

	void EditorLayer::OnDetach()
	{
		AT_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		AT_PROFILE_FUNCTION();

		///////////////////////////////////////////////////////////////////
		if (m_ViewportHovered || m_ViewportFocused) m_CameraController.OnUpdate(ts); // Camera Controller Update
		///// Resize Code /////////////////////////////////////////////////
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
		}

		///// Renderer Code ///////////////////////////////////////////////
		Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();
		m_ActiveScene->OnUpdate(ts);
		m_Framebuffer->Unbind();

		{
			// AT_PROFILE_SCOPE("Renderer Draw");

			// static float rotation = 0.0f;
			// rotation += ts * 50.0f;
			// Renderer2D::BeginScene(m_CameraController.GetCamera());

			// Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
			// Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
			// Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
			// Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
			// Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckerboardTexture, 20.0f);
			// Renderer2D::EndScene();
			//
			// Renderer2D::BeginScene(m_CameraController.GetCamera());
			// for (float y = -5.0f; y < 5.0f; y += 0.5f)
			// {
			// 	for (float x = -5.0f; x < 5.0f; x += 0.5f)
			// 	{
			// 		glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
			// 		Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			// 	}
			// }
			// Renderer2D::EndScene();
		}
		///////////////////////////////////////////////////////////////////
	}


	void ImGuiOverlay()
	{
		auto stats = Renderer2D::GetStats();
		static int location = 3;
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
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

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
	void EditorLayer::OnDebugUIRender()
	{
		AT_PROFILE_FUNCTION();

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
			ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("EditorDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows,
					// which we can't undo at the moment without finer window depth/z control.
					//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

					if (ImGui::MenuItem("Exit")) Application::Get().Close();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			{
				m_SceneHierarchyPanel.OnImGuiRender();
				ImGuiOverlay();
				ImGui::Begin("Stats");

				auto stats = Renderer2D::GetStats();
				ImGui::Text("Renderer2D Stats:");
				ImGui::Text("Draw Calls: %d", stats.DrawCalls);
				ImGui::Text("Quads: %d", stats.QuadCount);
				ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
				ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

				ImGui::End();


				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
				ImGui::Begin("Viewport");
				m_ViewportFocused = ImGui::IsWindowFocused();
				m_ViewportHovered = ImGui::IsWindowHovered();
				Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

				ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

				size_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
				ImGui::Image(textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				ImGui::End();
				ImGui::PopStyleVar();

			}
			ImGui::End();
		}



	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}

}
