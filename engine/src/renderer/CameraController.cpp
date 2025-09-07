#include "AnataAisite/Renderer/CameraController.h"

#include "AnataAisite/Input.h"
#include "AnataAisite/KeyCodes.h"

namespace Aisite {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep deltaTime)
	{
		AT_PROFILE_FUNCTION();

		///// Input ///////////////////////////////////////////////////////
		///// Camera Control ////////////////////////////////////////
		if (Input::IsKeyPressed(KeyCode::LeftShift))
		    m_CameraTranslationSpeed = .001f * m_ZoomLevel,
		    m_CameraRotationSpeed = 100.f * deltaTime;
		else
		    m_CameraTranslationSpeed = .002f * m_ZoomLevel,
		    m_CameraRotationSpeed = 400.f * deltaTime;

		static double s_LastX, s_LastY;
		auto [ x, y ] = Input::GetMousePosition();

		if (Input::IsMouseButtonPressed(MouseCode::ButtonMiddle)) {
		    float dx = x - s_LastX, dy = y - s_LastY;
		    float rad = glm::radians(m_Camera.Rotation());
		    float cosR = cos(rad), sinR = sin(rad);

		    const glm::vec3 offset{
		        -m_CameraTranslationSpeed * (dx * cosR + dy * sinR),
		         m_CameraTranslationSpeed * (dy * cosR - dx * sinR),
		        0.0f
		    };
			m_CameraPosition += offset;
		}

		if      (Input::IsKeyPressed(KeyCode::Q)) m_CameraRotation += m_CameraRotationSpeed;
		else if (Input::IsKeyPressed(KeyCode::E)) m_CameraRotation -= m_CameraRotationSpeed;

		m_Camera.SetRotation(m_CameraRotation);
		m_Camera.SetPosition(m_CameraPosition);

		s_LastX = x, s_LastY = y;
		///////////////////////////////////////////////////////////////////
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		AT_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(AT_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(AT_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		AT_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		AT_PROFILE_FUNCTION();

		OnResize(e.GetWidth(), e.GetHeight());
		return false;
	}

}