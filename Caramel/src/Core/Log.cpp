#include "clpch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Caramel
{
	Log::~Log()
	{
		if (sm_instance == this)
		{
			sm_instance = nullptr;
		}
	}

	void Log::Init()
	{
		if (sm_instance != nullptr)
		{
			CL_CORE_ERROR("Logger was already initialised");
			return;
		}

		spdlog::set_pattern("%^[%T] %n: %v%$");
		m_coreLogger = spdlog::stdout_color_mt("Caramel");
		m_coreLogger->set_level(spdlog::level::trace);
		m_clientLogger = spdlog::stdout_color_mt("Game");
		m_clientLogger->set_level(spdlog::level::trace);

		sm_instance = this;
	}	

	Caramel::Log* Log::sm_instance;
}