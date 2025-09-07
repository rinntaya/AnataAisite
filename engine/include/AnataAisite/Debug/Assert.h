#pragma once

#ifdef AT_ENABLE_ASSERTS

	namespace Aisite::Assert
	{
		// Returns the simple file name rather than full path as suggested by LovelySanta
		constexpr const AISITE_API char* CurrentFileName(const char* path) {
				const char* file = path;
				while (*path)
				{
					if (*path == '/' || *path == '\\')
						file = ++path;
					else
						path++;
				}
				return file;
			}
	}

	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define AT_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { AT##type##ERROR(msg, __VA_ARGS__); AT_DEBUGBREAK(); } }
	#define AT_INTERNAL_ASSERT_WITH_MSG(type, check, ...) AT_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define AT_INTERNAL_ASSERT_NO_MSG(type, check) AT_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", AT_STRINGIFY_MACRO(check), ::Aisite::Assert::CurrentFileName(__FILE__), __LINE__)

	#define AT_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define AT_INTERNAL_ASSERT_GET_MACRO(...) AT_EXPAND_MACRO( AT_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, AT_INTERNAL_ASSERT_WITH_MSG, AT_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define AT_ASSERT(...) AT_EXPAND_MACRO( AT_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define AT_CORE_ASSERT(...) AT_EXPAND_MACRO( AT_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define AT_ASSERT(...)
	#define AT_CORE_ASSERT(...)
#endif