#include "clpch.h"
#include "Caramel/Core/Base.h"

#include "Caramel/Core/Log.h"

#define CARAMEL_BUILD_ID "v0.1a"

namespace Caramel 
{
	// This function is used to initialise all core things like the logging system or maybe an audio system
	void InitializeCore()
	{
		Caramel::Log::Init();

		CL_CORE_TRACE("Caramel Engine {}", CARAMEL_BUILD_ID);
		CL_CORE_TRACE("Initializing...");
	}

	// This function allows for any core initialised things to be saftly shutdown
	void ShutdownCore()
	{
		CL_CORE_TRACE("Shutting down...");
	}

}