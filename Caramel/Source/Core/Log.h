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
		inline static std::string ConvertWChar(const wchar_t* target) { return s_Converter.to_bytes(target); }

	private:
		static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> s_Converter;
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

namespace std
{
	// Kinda gross but does the trick (Printing DXGI_ADAPTER_DESC::Description)
	template<>
	struct formatter<WCHAR[128]> : formatter<std::string>
	{
		auto format(const WCHAR(&wstr)[128], std::format_context& ctx) const
		{
			// Convert the wide string to a UTF-8 string
			std::string str = Caramel::Log::ConvertWChar(wstr);
			// Use the existing std::formatter<std::string> to format the string
			return formatter<std::string>::format(str, ctx);
		}
	};
}


// Macros
#define CL_CORE_ERROR(...) ::Caramel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CL_CORE_WARN(...) ::Caramel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CL_CORE_INFO(...) ::Caramel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CL_CORE_TRACE(...) ::Caramel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CL_CORE_FATAL(...) ::Caramel::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define CL_CORE_NOT_IMPLEMENTED ::Caramel::Log::GetCoreLogger()->critical("{} not implemented", __FUNCTION__)

#define CL_ERROR(...) ::Caramel::Log::GetClientLogger()->error(__VA_ARGS__)
#define CL_WARN(...) ::Caramel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CL_INFO(...) ::Caramel::Log::GetClientLogger()->info(__VA_ARGS__)
#define CL_TRACE(...) ::Caramel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CL_FATAL(...) ::Caramel::Log::GetClientLogger()->critical(__VA_ARGS__)
#define CL_NOT_IMPLEMENTED ::Caramel::Log::GetCoreLogger()->critical("{} not implemented", __FUNCTION__)

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
