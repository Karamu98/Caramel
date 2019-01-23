#pragma once

#ifdef CL_PLATFORM_WINDOWS
	#ifdef CL_BUILD_DLL
		#define CARAMEL_API __declspec(dllexport)
	#else
		#define CARAMEL_API __declspec(dllimport)

	#endif
#else
#error Caramel only supports Windows!
#endif