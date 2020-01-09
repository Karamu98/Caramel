#pragma once

#include <spdlog/spdlog.h>

namespace Caramel
{
	class Log
	{
	public:
		Log();
		~Log();

		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}


// Macros
#define CL_CORE_ERROR(...) ::Caramel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CL_CORE_WARN(...) ::Caramel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CL_CORE_INFO(...) ::Caramel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CL_CORE_TRACE(...) ::Caramel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CL_CORE_FATAL(...) ::Caramel::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define CL_ERROR(...) ::Caramel::Log::GetClientLogger()->error(__VA_ARGS__)
#define CL_WARN(...) ::Caramel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CL_INFO(...) ::Caramel::Log::GetClientLogger()->info(__VA_ARGS__)
#define CL_TRACE(...) ::Caramel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CL_FATAL(...) ::Caramel::Log::GetClientLogger()->critical(__VA_ARGS__)

#ifdef CL_ENABLE_ASSERTS

#define CL_ASSERT_NO_MESSAGE(condition) { if(!(condition)) { CL_ERROR("Assertion failed."); __debugbreak(); } }
#define CL_ASSERT_MESSAGE(condition, ...) { if(!(condition)) { CL_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }

#define CL_ASSERT_RESOLVE(arg1, arg2, macro, ...) macro

#define CL_ASSERT(...) CL_ASSERT_RESOLVE(__VA_ARGS__, CL_ASSERT_MESSAGE, CL_ASSERT_NO_MESSAGE)(__VA_ARGS__)
#define CL_CORE_ASSERT(...) CL_ASSERT_RESOLVE(__VA_ARGS__, CL_ASSERT_MESSAGE, CL_ASSERT_NO_MESSAGE)(__VA_ARGS__)
#else
#define CL_ASSERT(...)
#define CL_CORE_ASSERT(...)
#endif