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

	filter "configurations:Debug"
		defines {"CL_DEBUG", "CL_ENABLE_ASSERTS"}
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
		flags { "LinkTimeOptimization" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CL_PLATFORM_WINDOWS",
		}

	filter "system:linux"
		systemversion "latest"

		defines
		{
			"CL_PLATFORM_LINUX"
		}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Caramel/Vendor/glfw/include"
IncludeDir["glad"] = "Caramel/Vendor/glad/include"
IncludeDir["spdlog"] = "Caramel/Vendor/spdlog/include"
IncludeDir["imgui"] = "Caramel/Vendor/imgui"
IncludeDir["stb"] = "Caramel/Vendor/stb"

group "Dependencies"
include "Caramel/Vendor/glfw"
include "Caramel/Vendor/imgui"
include "Caramel/Vendor/glad"

group ""

project "Caramel"
	location "Caramel"
	kind "StaticLib"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"

	targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "clpch.h"
	pchsource "Caramel/Source/clpch.cpp"

	files
	{
		"%{prj.name}/Source/**.h", 
		"%{prj.name}/Source/**.cpp" 
	}

	defines
	{
		--"_CRT_SECURE_NO_WARNINGS",
		--"GLM_FORCE_SWIZZLE",
		--"GLM_FORCE_RADIANS",
		--"GLM_FORCE_PURE",
		--"GLM_ENABLE_EXPERIMENTAL",
		--"IMGUI_IMPL_OPENGL_LOADER_GLAD"
		"GLFW_INCLUDE_NONE",
		"SPDLOG_USE_STD_FORMAT"
	}

	includedirs
	{
		"%{prj.name}/Source",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.stb}",
	}

	links 
	{ 
		"GLFW",
		"ImGui",
		"glad",
		--"assimp",
		--"opengl32.lib"
	}

	filter "system:windows"
		links
		{
			"d3d12.lib",
			"dxgi.lib"
		}

project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"

	targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/Source",
		"Caramel/Source",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		--"Caramel/deps/spdlog/include",
		--"%{IncludeDir.ImGui}",
		--"Caramel/deps",
		--"%{IncludeDir.glm}"
		"%{IncludeDir.spdlog}"
	}

	links
	{
		"Caramel",
		"ImGui",
		"glad"
	}