#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Aisite
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type) {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
                return GL_FLOAT;
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
                return GL_INT;
            case ShaderDataType::Bool:
                return GL_BOOL;
            default: ;
        }

        AT_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        AT_PROFILE_FUNCTION();

        glCreateVertexArrays(1, &m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        AT_PROFILE_FUNCTION();

        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const
    {
        AT_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind() const
    {
        AT_PROFILE_FUNCTION();

        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        AT_PROFILE_FUNCTION();

        AT_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();

        uint32_t index = 0;
        const auto& layout = vertexBuffer->GetLayout();
        for (const auto& element: layout) {

            switch (element.Type)
            {
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4:
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                case ShaderDataType::Bool:
                {
                    glEnableVertexAttribArray(index);
                    glVertexAttribPointer(index,
                        element.GetComponentCount(),
                        ShaderDataTypeToOpenGLBaseType(element.Type),
                        element.Normalized ? GL_TRUE : GL_FALSE,
                        layout.GetStride(),
                        (const void*)element.Offset);
                    index++;
                    break;
                }
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4:
                {
                    uint8_t count = element.GetComponentCount();
                    for (uint8_t i = 0; i < count; i++)
                    {
                        glEnableVertexAttribArray(index);
                        glVertexAttribPointer(index,
                            count,
                            ShaderDataTypeToOpenGLBaseType(element.Type),
                            element.Normalized ? GL_TRUE : GL_FALSE,
                            layout.GetStride(),
                            (const void*)(element.Offset + sizeof(float) * count * i));
                        glVertexAttribDivisor(index, 1);
                        index++;
                    }
                    break;
                }
                default:
                    AT_CORE_ASSERT(false, "Unknown ShaderDataType!");
            }

        }
        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        AT_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }
}
