#pragma once

#include <memory>

namespace Caramel 
{
	void InitializeCore();
	void ShutdownCore();

}

// Enable all things to help debugging here, on release, these calls will be stripped out
#ifdef CL_DEBUG
	#define CL_ENABLE_ASSERTS
#endif

// Futerproofing!
#ifdef CL_PLATFORM_WINDOWS

#else
	#error Caramel only supports Windows!
#endif

// If asserts are enabled, these calls will break on failed tests
#ifdef CL_ENABLE_ASSERTS
	#define CL_ASSERT(x, ...) { if(!(x)) { CL_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define CL_CORE_ASSERT(x, ...) { if(!(x)) { CL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define CL_ASSERT(x, ...)
	#define CL_CORE_ASSERT(x, ...)
#endif

// An easy way to create a bit field BIT[1] == 1 << 1 (2)
#define BIT(x) (1 << x) 

// Macro to simplify binding a function
#define CL_BIND_EVENT_FN(fn) std::bind(&##fn, this, std::placeholders::_1)