#pragma once

#include "Layer.h"

namespace Aisite {

    class AISITE_API ImGuiLayer final : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnDebugUIRender() override;

        void Begin();
        void End();
        void OnUpdate(Timestep deltaTime) override
        {
            float currentFps = 1.0f / deltaTime;
            m_Fps = m_Fps * (1.0f - kAlpha) + currentFps * kAlpha;
        }

    private:
        float m_Time = 0.0f;
        float m_Fps = 60.0f; // 初始值可设为60
        const float kAlpha = 0.1f; // 平滑系数
    };

}

