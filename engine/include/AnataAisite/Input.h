#pragma once

#include "KeyCodes.h"
#include "Core.h"

namespace Aisite {

    class AISITE_API Input
    {
    public:
        static bool IsKeyPressed(KeyCode key);

        static bool IsMouseButtonPressed(MouseCode button);
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };

}