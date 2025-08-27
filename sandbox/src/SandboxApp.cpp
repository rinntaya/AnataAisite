#include <AnataAisite.h>
#include <imgui.h>

#include "AnataAisite/Input.h"
#include "AnataAisite/Events/KeyEvent.h"
#include "AnataAisite/Renderer/Renderer.h"

class ExampleLayer : public Aisite::Layer
{
    std::shared_ptr<Aisite::Shader> m_Shader;
    std::shared_ptr<Aisite::VertexArray> m_VertexArray;

    std::shared_ptr<Aisite::Shader> m_BlueShader;
    std::shared_ptr<Aisite::VertexArray> m_SquareVA;

public:
    ExampleLayer()
        : Layer("Example")
    {
        m_VertexArray.reset(Aisite::VertexArray::Create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };
        const Aisite::BufferLayout layout = {
            {Aisite::ShaderDataType::Vec3, "a_Position"},
            {Aisite::ShaderDataType::Vec4, "a_Color"}
        };
        std::shared_ptr<Aisite::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Aisite::VertexBuffer::Create(vertices, sizeof(vertices)));
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t index[] = {1, 2, 3, 3, 2, 0,};
        std::shared_ptr<Aisite::IndexBuffer> indexBuffer;
        indexBuffer.reset(Aisite::IndexBuffer::Create(index, 6));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;


			out vec3 v_Position;
			out vec4 v_Color;


			void main()
			{
                v_Color = a_Color;
				v_Position = a_Position;

				gl_Position = vec4(a_Position, 1.0);
			}
		)";

        std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;


			void main()
			{
                // vec3 temp = v_Position * 0.5 + 0.5;
				// color = vec4( temp.x, 0.0,temp.y, 1.0);
				color = v_Color;
			}
		)";

        m_Shader.reset(new Aisite::Shader(vertexSrc, fragmentSrc));
    }

    void OnUpdate() override
    {

        ///// Input Test //////////////////////////////////////////////////
        if (Aisite::Input::IsKeyPressed(AT_KEY_TAB))
            AT_TRACE("Tab key is pressed (poll)!");
        auto [x, y] = Aisite::Input::GetMousePosition();
        ///////////////////////////////////////////////////////////////////


        m_Shader->Bind();
        Aisite::Renderer::BeginScene();
        Aisite::Renderer::Submit(m_VertexArray);
        Aisite::Renderer::EndScene();

    }



    void OnEvent(Aisite::Event& event) override
    {
        if (event.GetEventType() == Aisite::EventType::KeyPressed) {
            Aisite::KeyPressedEvent& e = (Aisite::KeyPressedEvent &) event;
            if (e.GetKeyCode() == AT_KEY_TAB)
                AT_TRACE("Tab key is pressed (event)!");
            AT_TRACE("{0}: {1}", (char)e.GetKeyCode(), e.GetKeyCode());
        }
    }
    void OnDebugUIRender() override
    {
        ImGui::Begin("Test");
        ImGui::Text("Hello World");
        ImGui::End();
    };
};


class Sandbox : public Aisite::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    }
};

AISITE_APP(Sandbox);
