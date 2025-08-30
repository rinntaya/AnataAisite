#pragma once

#include <AnataAisite.h>
#include "AnataAisite/CameraController.h"

class Sandbox2D : public Aisite::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Aisite::Timestep ts) override;
    virtual void OnDebugUIRender() override;
    void OnEvent(Aisite::Event& e) override;
private:
    Aisite::OrthographicCameraController m_CameraController;


    Aisite::Ref<Aisite::Texture2D> m_CheckerboardTexture;
    struct ProfileResult
    {
        const char* Name;
        float Time;
    };

    std::vector<ProfileResult> m_ProfileResults;


    glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
