#include "clpch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Caramel::Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Caramel::Log::s_ClientLogger;

namespace Caramel
{
	Log::Log()
	{

	}

	Log::~Log()
	{
	}

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("Caramel");
		s_CoreLogger->set_level(spdlog::level::trace);
		s_ClientLogger = spdlog::stdout_color_mt("Game");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}