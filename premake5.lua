workspace "Caramel"
	architecture "x64"
	startproject "Game"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Caramel/deps/GLFW/include"
IncludeDir["Glad"] = "Caramel/deps/Glad/include"
IncludeDir["ImGui"] = "Caramel/deps/imgui"
IncludeDir["glm"] = "Caramel/deps/glm"
IncludeDir["stb"] = "Caramel/deps/stb"

group "Dependencies"
	include "Caramel/deps/GLFW"
	include "Caramel/deps/Glad"
	include "Caramel/deps/imgui"

group ""

project "Caramel"
	location "Caramel"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/" .. outputdir .. "/obj/%{prj.name}")

	pchheader "clpch.h"
	pchsource "Caramel/src/clpch.cpp"

	files
	{
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp" 
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLM_FORCE_SWIZZLE",
		"GLM_FORCE_RADIANS",
		"GLM_FORCE_PURE",
		"GLM_ENABLE_EXPERIMENTAL",
		"IMGUI_IMPL_OPENGL_LOADER_GLAD"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/deps/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CL_PLATFORM_WINDOWS",
			"CL_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "CL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CL_DIST"
		runtime "Release"
		optimize "on"

project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/" .. outputdir .. "/obj/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"Caramel/deps/spdlog/include",
		"Caramel/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"Caramel/deps",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Caramel"
	}

	prebuildcommands 
	{
		"call %{wks.location}scripts/copyDepsToOut.bat"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CL_PLATFORM_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS",
			"GLM_FORCE_SWIZZLE",
			"GLM_FORCE_RADIANS",
			"GLM_FORCE_PURE",
			"GLM_ENABLE_EXPERIMENTAL",
			"IMGUI_IMPL_OPENGL_LOADER_GLAD"
		}

	filter "configurations:Debug"
		defines "CL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CL_DIST"
		runtime "Release"
		optimize "on"