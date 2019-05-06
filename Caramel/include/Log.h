#ifndef __LOG_H_
#define __LOG_H_

#include "spdlog/spdlog.h"


class Log
{
public:
	Log();
	~Log();

	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	static void GetOpenGLErrors(const char* a_file, int a_line);

private:
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_ClientLogger;
};
#endif // !__LOG_H_

// Macros
#define CL_CORE_ERROR(...) ::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CL_CORE_WARN(...) ::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CL_CORE_INFO(...) ::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CL_CORE_TRACE(...) ::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CL_CORE_FATAL(...) ::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define CL_ERROR(...) ::Log::GetClientLogger()->error(__VA_ARGS__)
#define CL_WARN(...) ::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CL_INFO(...) ::Log::GetClientLogger()->info(__VA_ARGS__)
#define CL_TRACE(...) ::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CL_FATAL(...) ::Log::GetClientLogger()->critical(__VA_ARGS__)

#define CL_GETGL_ERRORS ::Log::GetOpenGLErrors(__FILE__, __LINE__)