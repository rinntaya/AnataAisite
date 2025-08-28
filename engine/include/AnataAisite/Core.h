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


#ifdef _AT_DEBUG
    #define _AT_ENABLE_ASSERTS
#endif

#ifdef _AT_ENABLE_ASSERTS
    #define AT_ASSERT(x, ...) { if(!(x)) { AT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define AT_CORE_ASSERT(x, ...) { if(!(x)) { AT_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define AT_ASSERT(x, ...)
    #define AT_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)

#define AT_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#include <memory>

namespace Aisite {

    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;

}
