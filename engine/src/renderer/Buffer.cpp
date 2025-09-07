
#include "AnataAisite/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Aisite {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:    AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case Renderer::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(size);
		}

		AT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:    AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case Renderer::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		AT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:    AT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case Renderer::API::OpenGL:  return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		AT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}