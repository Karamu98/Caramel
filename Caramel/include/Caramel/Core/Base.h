#pragma once

#include <memory>

namespace Caramel 
{
	void InitializeCore();
	void ShutdownCore();

}

#ifdef CL_DEBUG
	#define CL_ENABLE_ASSERTS
#endif

#ifdef CL_PLATFORM_WINDOWS

#else
	#error Caramel only supports Windows!
#endif

#ifdef CL_ENABLE_ASSERTS
	#define CL_ASSERT(x, ...) { if(!(x)) { CL_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define CL_CORE_ASSERT(x, ...) { if(!(x)) { CL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define CL_ASSERT(x, ...)
	#define CL_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define CL_BIND_EVENT_FN(fn) std::bind(&##fn, this, std::placeholders::_1)