#include <AnataAisite.h>
#include <imgui.h>

#include "glm/ext/matrix_transform.hpp"
#include "platform/opengl/OpenGLShader.h"


class ExampleLayer : public Aisite::Layer
{
    Aisite::Ref<Aisite::Shader> m_Shader;
    Aisite::Ref<Aisite::VertexArray> m_VertexArray;

    Aisite::Ref<Aisite::Shader> m_FlatColorShader, m_TextureShader;
    Aisite::Ref<Aisite::VertexArray> m_SquareVA;
	Aisite::Ref<Aisite::Texture2D> m_Texture;


    Aisite::OrthographicCamera m_Camera;
    float m_CameraMoveSpeed = .003f;
    float m_CameraRotationSpeed = 4.f;


	glm::vec3 m_SquareColor{ 1.f };

public:
    ExampleLayer()
        : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
    {
        m_VertexArray.reset(Aisite::VertexArray::Create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };
        const Aisite::BufferLayout layout = {
            {Aisite::ShaderDataType::Float3, "a_Position"},
            {Aisite::ShaderDataType::Float4, "a_Color"}
        };
        Aisite::Ref<Aisite::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Aisite::VertexBuffer::Create(vertices, sizeof(vertices)));
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t index[] = {1, 2, 3, 3, 2, 0,};
        Aisite::Ref<Aisite::IndexBuffer> indexBuffer;
        indexBuffer.reset(Aisite::IndexBuffer::Create(index, 6));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;


			out vec3 v_Position;
			out vec4 v_Color;


			void main()
			{
                v_Color = a_Color;
				v_Position = a_Position;

				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

        m_Shader.reset(Aisite::Shader::Create(vertexSrc, fragmentSrc));




        m_SquareVA.reset(Aisite::VertexArray::Create());

    	float squareVertices[5 * 4] = {
    		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

        Aisite::Ref<Aisite::VertexBuffer> squareVB;
        squareVB.reset(Aisite::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({
            { Aisite::ShaderDataType::Float3, "a_Position" },
            { Aisite::ShaderDataType::Float2, "a_TexCoord" }
        });
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        Aisite::Ref<Aisite::IndexBuffer> squareIB;
        squareIB.reset(Aisite::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);


    	std::string flatColorShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

    	std::string flatColorShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

    	m_FlatColorShader.reset(Aisite::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));


        std::string textureShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

        std::string textureShaderFragmentSrc  = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform sampler2D u_Texture;
			in vec2 v_TexCoord;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";


    	m_TextureShader.reset(Aisite::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

    	m_Texture = Aisite::Texture2D::Create("assets/textures/Checkerboard.png");

    	std::dynamic_pointer_cast<Aisite::OpenGLShader>(m_TextureShader)->Bind();
    	std::dynamic_pointer_cast<Aisite::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
    }

    void OnUpdate(Aisite::Timestep deltaTime) override
    {
        ///// Input Test //////////////////////////////////////////////////
              ///// Camera Control ////////////////////////////////////////
        if (Aisite::Input::IsKeyPressed(AT_KEY_LEFT_SHIFT))
            m_CameraMoveSpeed = .001f,
            m_CameraRotationSpeed = 100.f * deltaTime;
        else
            m_CameraMoveSpeed = .003f,
            m_CameraRotationSpeed = 400.f * deltaTime;

        static double s_LastX, s_LastY;
        auto [ x, y ] = Aisite::Input::GetMousePosition();

        if (Aisite::Input::IsMouseButtonPressed(AT_MOUSE_BUTTON_MIDDLE)) {
            float dx = x - s_LastX, dy = y - s_LastY;
            float rad = glm::radians(m_Camera.Rotation());
            float cosR = cos(rad), sinR = sin(rad);

            glm::vec3 offset{
                -m_CameraMoveSpeed * (dx * cosR + dy * sinR),
                 m_CameraMoveSpeed * (dy * cosR - dx * sinR),
                0.0f
            };
            m_Camera.AddPosition(offset);
        }
        if      (Aisite::Input::IsKeyPressed(AT_KEY_Q)) m_Camera.AddRotation(m_CameraRotationSpeed);
        else if (Aisite::Input::IsKeyPressed(AT_KEY_E)) m_Camera.AddRotation(-m_CameraRotationSpeed);

        s_LastX = x, s_LastY = y;
        ///////////////////////////////////////////////////////////////////



        ///// Renderer Code ///////////////////////////////////////////////
        Aisite::Renderer::BeginScene(m_Camera);



    	std::dynamic_pointer_cast<Aisite::OpenGLShader>(m_FlatColorShader)->Bind();
    	std::dynamic_pointer_cast<Aisite::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

    	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
    	for (int y = 0; y < 20; y++)
    	{
    		for (int x = 0; x < 20; x++)
    		{
    			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
    			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
    			Aisite::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
    		}
    	}

    	m_Texture->Bind();
    	Aisite::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    	// Aisite::Renderer::Submit(m_Shader, m_VertexArray);


        Aisite::Renderer::EndScene();
        ///////////////////////////////////////////////////////////////////

    }



    void OnDebugUIRender() override
    {

        glm::vec3 position_display = m_Camera.Position();
        float     rotation_display = m_Camera.Rotation();
        ImGui::Begin("Test");
        ImGui::Text("Camera Position:");
        if (ImGui::SliderFloat3("P", &position_display.x, -100.f , 100.f))
            m_Camera.SetPosition(position_display);
        if (ImGui::SliderFloat("R", &rotation_display, -360.f , 360.f))
            m_Camera.SetRotation(rotation_display);

    	ImGui::ColorEdit3("Square Color", &m_SquareColor[0]);
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
