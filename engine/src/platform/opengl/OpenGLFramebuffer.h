#pragma once
#include "AnataAisite/Renderer/Framebuffer.h"

namespace Aisite {

    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        virtual ~OpenGLFramebuffer();

        void Invalidate();

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void Resize(uint32_t width, uint32_t height) override;
        virtual size_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

        virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
    private:
        uint32_t m_RendererID;
        uint32_t m_ColorAttachment, m_DepthAttachment;
        FramebufferSpecification m_Specification;
    };

} // Aisite
