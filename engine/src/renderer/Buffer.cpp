
#include "AnataAisite/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Aisite {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:    AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case Renderer::API::OpenGL:  return new OpenGLVertexBuffer(vertices, size);
		}

		AT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:    AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case Renderer::API::OpenGL:  return new OpenGLIndexBuffer(indices, size);
		}

		AT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}