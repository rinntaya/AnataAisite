#include "AnataAisite/CameraController.h"

#include "AnataAisite/Input.h"
#include "AnataAisite/KeyCodes.h"

namespace Aisite {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep deltaTime)
	{
		///// Input ///////////////////////////////////////////////////////
		///// Camera Control ////////////////////////////////////////
		if (Aisite::Input::IsKeyPressed(AT_KEY_LEFT_SHIFT))
		    m_CameraTranslationSpeed = .001f * m_ZoomLevel,
		    m_CameraRotationSpeed = 100.f * deltaTime;
		else
		    m_CameraTranslationSpeed = .002f * m_ZoomLevel,
		    m_CameraRotationSpeed = 400.f * deltaTime;

		static double s_LastX, s_LastY;
		auto [ x, y ] = Aisite::Input::GetMousePosition();

		if (Aisite::Input::IsMouseButtonPressed(AT_MOUSE_BUTTON_MIDDLE)) {
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

		if      (Input::IsKeyPressed(AT_KEY_Q)) m_CameraRotation += m_CameraRotationSpeed;
		else if (Input::IsKeyPressed(AT_KEY_E)) m_CameraRotation -= m_CameraRotationSpeed;

		m_Camera.SetRotation(m_CameraRotation);
		m_Camera.SetPosition(m_CameraPosition);

		s_LastX = x, s_LastY = y;
		///////////////////////////////////////////////////////////////////

		// m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(AT_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(AT_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}