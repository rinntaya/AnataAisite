#pragma once

#include "AnataAisite/Input.h"

namespace Aisite {

    class WindowsInput : public Input
    {
    protected:
        virtual bool IsKeyPressedImpl(int keycode) override;

        virtual bool IsMouseButtonPressedImpl(int button) override;
        virtual std::pair<double, double> GetMousePositionImpl() override;
        virtual double GetMouseXImpl() override;
        virtual double GetMouseYImpl() override;
    };

}