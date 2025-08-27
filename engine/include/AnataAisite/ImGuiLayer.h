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
    private:
        float m_Time = 0.0f;
    };

}

