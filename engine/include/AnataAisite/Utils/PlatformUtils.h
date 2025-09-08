
#pragma once

#include <optional>
#include <string>

namespace Aisite {

    class AISITE_API FileDialogs
    {
    public:
        // These return empty strings if cancelled
        static std::string OpenFile(const char* filter);
        static std::string SaveFile(const char* filter);
    };

}