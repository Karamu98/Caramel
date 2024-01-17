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
IncludeDir["spdlog"] = "Caramel/Vendor/spdlog/include"

group "Dependencies"
include "Caramel/Vendor/glfw"

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
	}

	includedirs
	{
		"%{prj.name}/Source",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.spdlog}"
	}

	links 
	{ 
		"GLFW",
		--"Glad",
		--"ImGui",
		--"assimp",
		--"opengl32.lib"
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
		--"Caramel/deps/spdlog/include",
		--"%{IncludeDir.Glad}",
		--"%{IncludeDir.ImGui}",
		--"Caramel/deps",
		--"%{IncludeDir.glm}"
		"%{IncludeDir.spdlog}"
	}

	links
	{
		"Caramel"
	}