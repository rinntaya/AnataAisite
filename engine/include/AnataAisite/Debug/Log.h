#pragma once

#include "../Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)


namespace Aisite {

    class AISITE_API Log
    {
    public:
        static void Init();

        static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    private:
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ClientLogger;
    };

}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
    return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
    return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
    return os << glm::to_string(quaternion);
}

// Core Log Macros
#define AT_CORE_TRACE(...)      ::Aisite::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define AT_CORE_DEBUG(...)      ::Aisite::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define AT_CORE_INFO(...)       ::Aisite::Log::GetCoreLogger()->info(__VA_ARGS__)
#define AT_CORE_WARN(...)       ::Aisite::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define AT_CORE_ERROR(...)      ::Aisite::Log::GetCoreLogger()->error(__VA_ARGS__)
#define AT_CORE_CRITICAL(...)        ::Aisite::Log::GetClientLogger()->critical(__VA_ARGS__)

// App Client Macros
#define AT_TRACE(...)           ::Aisite::Log::GetClientLogger()->trace(__VA_ARGS__)
#define AT_DEBUG(...)           ::Aisite::Log::GetClientLogger()->debug(__VA_ARGS__)
#define AT_INFO(...)            ::Aisite::Log::GetClientLogger()->info(__VA_ARGS__)
#define AT_WARN(...)            ::Aisite::Log::GetClientLogger()->warn(__VA_ARGS__)
#define AT_ERROR(...)           ::Aisite::Log::GetClientLogger()->error(__VA_ARGS__)
#define AT_CRITICAL(...)        ::Aisite::Log::GetClientLogger()->critical(__VA_ARGS__)