#pragma once

#include "Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace Aisite
{
    class AISITE_API Log
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}


// Core Log Macros
#define AT_CORE_TRACE(...)      ::Aisite::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define AT_CORE_DEBUG(...)      ::Aisite::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define AT_CORE_INFO(...)       ::Aisite::Log::GetCoreLogger()->info(__VA_ARGS__)
#define AT_CORE_WARN(...)       ::Aisite::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define AT_CORE_ERROR(...)      ::Aisite::Log::GetCoreLogger()->error(__VA_ARGS__)
// App Client Macros
#define AT_TRACE(...)           ::Aisite::Log::GetClientLogger()->trace(__VA_ARGS__)
#define AT_DEBUG(...)           ::Aisite::Log::GetClientLogger()->debug(__VA_ARGS__)
#define AT_INFO(...)            ::Aisite::Log::GetClientLogger()->info(__VA_ARGS__)
#define AT_WARN(...)            ::Aisite::Log::GetClientLogger()->warn(__VA_ARGS__)
#define AT_ERROR(...)           ::Aisite::Log::GetClientLogger()->error(__VA_ARGS__)
