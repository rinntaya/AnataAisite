#include "EditorLayer.h"

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "AnataAisite/Math/Math.h"
#include "AnataAisite/Scene/SceneSerializer.h"
#include "AnataAisite/Utils/PlatformUtils.h"


namespace Aisite {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_ViewportBounds{}
	// , m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
	}

	void EditorLayer::OnAttach()
	{
		AT_PROFILE_FUNCTION();

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		fbSpec.Attachments = {
			FramebufferTextureFormat::RGBA8,
			FramebufferTextureFormat::RED_INTEGER,
			FramebufferTextureFormat::Depth };
		m_Framebuffer = Framebuffer::Create(fbSpec);
		m_RuntimeFrame = Framebuffer::Create(fbSpec);

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		m_ActiveScene = CreateRef<Scene>();
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		m_ActiveScene->OnViewportResize((uint32_t)m_Runtime_ViewportSize.x, (uint32_t)m_Runtime_ViewportSize.y);
		SceneSerializer serializer(m_ActiveScene);
		serializer.Deserialize("assets/scenes/Example.scene");


#if 0
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
				auto& translation = GetComponent<TransformComponent>().Translation;
				translation.x = rand() % 10 - 5.0f;
			}

			void OnDestroy()
			{
			}

			void OnUpdate(Timestep ts)
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;

				if (Input::IsKeyPressed(Key::A))
					translation.x -= speed * ts;
				if (Input::IsKeyPressed(Key::D))
					translation.x += speed * ts;
				if (Input::IsKeyPressed(Key::W))
					translation.y += speed * ts;
				if (Input::IsKeyPressed(Key::S))
					translation.y -= speed * ts;
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

	}

	void EditorLayer::OnDetach()
	{
		AT_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		AT_PROFILE_FUNCTION();

		///////////////////////////////////////////////////////////////////
		if (m_ViewportHovered || m_ViewportFocused) {
			m_CameraController.OnUpdate(ts);
			m_EditorCamera.OnUpdate(ts);
		}

		///// Resize Code /////////////////////////////////////////////////
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}


		if (FramebufferSpecification spec = m_RuntimeFrame->GetSpecification();
			m_Runtime_ViewportSize.x > 0.0f && m_Runtime_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_Runtime_ViewportSize.x || spec.Height != m_Runtime_ViewportSize.y))
		{
			m_RuntimeFrame->Resize((uint32_t)m_Runtime_ViewportSize.x, (uint32_t)m_Runtime_ViewportSize.y);
			m_ActiveScene->OnViewportResize(m_Runtime_ViewportSize.x, m_Runtime_ViewportSize.y);
		}

		///// Renderer Code ///////////////////////////////////////////////
		Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();
		m_Framebuffer->ClearAttachment(1, -1);
		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera.GetViewProjection());

		{
			auto[mx, my] = ImGui::GetMousePos();
			mx -= m_ViewportBounds[0].x;
			my -= m_ViewportBounds[0].y;
			my = m_ViewportSize.y - my;
			int mouseX = (int)mx;
			int mouseY = (int)my;

			if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)m_ViewportSize.x && mouseY < (int)m_ViewportSize.y)
			{
				int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
				if (pixelData != -1 && m_HoveredEntity != Entity((entt::entity)pixelData, m_ActiveScene.get()) )
					m_HoveredEntity = Entity((entt::entity)pixelData, m_ActiveScene.get());
				else if (pixelData == -1 && m_HoveredEntity != Entity() )
					m_HoveredEntity = Entity();
			}
		}

		m_Framebuffer->Unbind();


		m_RuntimeFrame->Bind();
		RenderCommand::SetClearColor({ 0.f, 0.f, 0.f, 0.f });
		RenderCommand::Clear();
		m_ActiveScene->OnUpdateRuntime(ts);
		m_RuntimeFrame->Unbind();

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


	void EditorLayer::OnDebugUIRender()
	{
		AT_PROFILE_FUNCTION();

		static bool DemoWindow = false;
		static bool StatsWindow = false;
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
			const ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();
			const float minWinSizeX = style.WindowMinSize.x;
			style.WindowMinSize.x = 370.0f;
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("DockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}
			style.WindowMinSize.x = minWinSizeX;

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows,
					// which we can't undo at the moment without finer window depth/z control.
					//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

					if (ImGui::MenuItem("New", "Ctrl+N"))
						NewScene();

					if (ImGui::MenuItem("Open...", "Ctrl+O"))
						OpenScene();

					if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
						SaveSceneAs();

					if (ImGui::MenuItem("Exit")) Application::Get().Close();
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Window"))
				{
					ImGui::MenuItem("ImGui Demo", nullptr, &DemoWindow);
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			{
				if(DemoWindow) ImGui::ShowDemoWindow(&DemoWindow);



				m_SceneHierarchyPanel.OnImGuiRender();
				// ImGuiOverlay();
				ImGui::Begin("Stats", &StatsWindow);

				std::string name = "None";
				if (m_HoveredEntity)
					name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
				ImGui::Text("Hovered Entity: %s", name.c_str());


				ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

				auto stats = Renderer2D::GetStats();
				ImGui::Text("Renderer2D Stats:");
				ImGui::Text("Draw Calls: %d", stats.DrawCalls);
				ImGui::Text("Quads: %d", stats.QuadCount);
				ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
				ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

				ImGui::End();


				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
				ImGui::Begin("Viewport");
					auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
					auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
					auto viewportOffset = ImGui::GetWindowPos();
					m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
					m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };


					m_ViewportFocused = ImGui::IsWindowFocused();
					m_ViewportHovered = ImGui::IsWindowHovered();
					Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

					ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
					m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

					size_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
					ImGui::Image(textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });


					{
						// Gizmos
						Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
						if (selectedEntity && m_GizmoType != -1)
						{
							ImGuizmo::SetOrthographic(false);
							ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());

							ImGuizmo::SetGizmoSizeClipSpace(.2f);
							ImGuizmo::SetRect(m_ViewportBounds[0].x,
								m_ViewportBounds[0].y,
								m_ViewportBounds[1].x - m_ViewportBounds[0].x,
								m_ViewportBounds[1].y - m_ViewportBounds[0].y);


							// Camera

							// // Runtime camera from entity
							// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
							// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
							// const glm::mat4& cameraProjection = camera.GetProjection();
							// const glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

							// Editor camera
							const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
							const glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

							// Entity transform
							auto& tc = selectedEntity.GetComponent<TransformComponent>();
							glm::mat4 transform = tc.GetTransform();

							// Snapping
							bool snap = Input::IsKeyPressed(Key::LeftControl);
							float snapValue = 0.5f; // Snap to 0.5m for translation/scale
							// Snap to 45 degrees for rotation
							if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
								snapValue = 45.0f;

							float snapValues[3] = { snapValue, snapValue, snapValue };

							ImGuizmo::Manipulate(
								glm::value_ptr(cameraView),
								glm::value_ptr(cameraProjection),
								(ImGuizmo::OPERATION)m_GizmoType,
								ImGuizmo::MODE::LOCAL,
								glm::value_ptr(transform),
								nullptr,
								snap ? snapValues : nullptr
							);

							if (ImGuizmo::IsUsing())
							{
								glm::vec3 translation, rotation, scale;
								Math::DecomposeTransform(transform, translation, rotation, scale);

								glm::vec3 deltaRotation = rotation - tc.Rotation;
								tc.Translation = translation;
								tc.Rotation += deltaRotation;
								tc.Scale = scale;
							}
						}
					}


				ImGui::End();


				ImGui::Begin("Runtime Viewprot");
					ImVec2 Runtime_viewportPanelSize = ImGui::GetContentRegionAvail();
					m_Runtime_ViewportSize = { Runtime_viewportPanelSize.x, Runtime_viewportPanelSize.y };

					size_t Runtime_textureID = m_RuntimeFrame->GetColorAttachmentRendererID();
					ImGui::Image(Runtime_textureID, ImVec2{ m_Runtime_ViewportSize.x, m_Runtime_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				ImGui::End();
				ImGui::PopStyleVar();

			}
			ImGui::End();
		}



	}



	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(AT_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(AT_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}
	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		const bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		const bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
			case Key::N:
			{
				if (control)
					NewScene();

				break;
			}
			case Key::O:
			{
				if (control)
					OpenScene();

				break;
			}
			case Key::S:
			{
				if (control && shift)
					SaveSceneAs();
				if (!ImGuizmo::IsUsing()) m_GizmoType = ImGuizmo::OPERATION::SCALE;

				break;
			}
			// Gizmos
			case Key::Minus:
				if (!ImGuizmo::IsUsing()) m_GizmoType = -1;
			break;
			case Key::T:
				if (!ImGuizmo::IsUsing()) m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
			case Key::R:
				if (!ImGuizmo::IsUsing()) m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
			case Key::G:
				if (!ImGuizmo::IsUsing()) m_GizmoType = ImGuizmo::OPERATION::UNIVERSAL;
			default: break;
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}
		return false;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_Runtime_ViewportSize.x, (uint32_t)m_Runtime_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}
	void EditorLayer::OpenScene()
	{
		std::optional<std::string> filepath = FileDialogs::OpenFile("AnataAiste Scene (*.scene)\0*.scene\0");
		if (filepath)
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_Runtime_ViewportSize.x, (uint32_t)m_Runtime_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(*filepath);
		}
	}
	void EditorLayer::SaveSceneAs()
	{
		std::optional<std::string> filepath = FileDialogs::SaveFile("AnataAiste Scene (*.scene)\0*.scene\0");
		if (filepath)
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(*filepath);
		}
	}
}
