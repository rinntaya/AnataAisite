#pragma once


///// Define Export ///////////////////////////////////////////////
#ifdef _WIN32
#ifdef AISITE_EXPORTS
#define AISITE_API __declspec(dllexport)
#else
      #define AISITE_API __declspec(dllimport)
#endif
#else
    #define AISITE_API
#endif
#define IMGUI_API AISITE_API
// ----------------------------------------
///////////////////////////////////////////////////////////////////
#include "PlatformDetection.h"

#ifdef _AT_DEBUG
    #if defined(AT_PLATFORM_WINDOWS)
        #define AT_DEBUGBREAK() __debugbreak()
    #elif defined(AT_PLATFORM_LINUX)
        #include <signal.h>
        #define AT_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
    #define AT_ENABLE_ASSERTS
#else
    #define AT_DEBUGBREAK()
#endif


#define AT_EXPAND_MACRO(x) x
#define AT_STRINGIFY_MACRO(x) #x
#include "Debug/Assert.h"


#define BIT(x) (1 << x)

// #define AT_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define AT_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#include <memory>

namespace Aisite
{
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}
