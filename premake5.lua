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
--IncludeDir["GLFW"] = "Caramel/deps/GLFW/include"
--IncludeDir["Glad"] = "Caramel/deps/Glad/include"
--IncludeDir["ImGui"] = "Caramel/deps/imgui"
--IncludeDir["glm"] = "Caramel/deps/glm"
--IncludeDir["stb"] = "Caramel/deps/stb"
--IncludeDir["assimp"] = "Caramel/deps/assimp/include"

group "Dependencies"
	--include "Caramel/deps/GLFW"
	--include "Caramel/deps/Glad"
	--include "Caramel/deps/imgui"
	--include "Caramel/deps/assimp"

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
		--"%{prj.name}/deps/spdlog/include",
		--"%{IncludeDir.GLFW}",
		--"%{IncludeDir.Glad}",
		--"%{IncludeDir.ImGui}",
		--"%{IncludeDir.glm}",
		--"%{IncludeDir.stb}",
		--"%{IncludeDir.assimp}"
	}

	links 
	{ 
		--"GLFW",
		--"Glad",
		--"ImGui",
		--"assimp",
		--"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CL_PLATFORM_WINDOWS",
			--"GLFW_INCLUDE_NONE"
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
		--"Caramel/deps/spdlog/include",
		--"%{IncludeDir.GLFW}",
		--"%{IncludeDir.Glad}",
		--"%{IncludeDir.ImGui}",
		--"Caramel/deps",
		--"%{IncludeDir.glm}"
	}

	links
	{
		"Caramel"
	}

	filter "system:windows"
		systemversion "latest"

		--defines
		--{
		--	"CL_PLATFORM_WINDOWS",
		--	"_CRT_SECURE_NO_WARNINGS",
		--	"GLM_FORCE_SWIZZLE",
		--	"GLM_FORCE_RADIANS",
		--	"GLM_FORCE_PURE",
		--	"GLM_ENABLE_EXPERIMENTAL",
		--	"IMGUI_IMPL_OPENGL_LOADER_GLAD"
		--}

	filter "configurations:Debug"
		defines "CL_DEBUG"
		--prebuildcommands 
		--{
		--	"call %{wks.location}scripts/copyDepsToOutDebug.bat"
		--}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CL_RELEASE"
		--prebuildcommands 
		--{
		--	"call %{wks.location}scripts/copyDepsToOutRelease.bat"
		--}
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CL_DIST"
		runtime "Release"
		optimize "on"