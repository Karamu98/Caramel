workspace "Caramel"
	architecture "x64"
	targetdir "build"

	configurations
	{
		"Debug",
        "Release",
        "Dist"
    }

	startproject "Game"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Caramel/deps/GLFW/include"
IncludeDir["glad"] = "Caramel/deps/glad/include"
IncludeDir["ImGui"] = "Caramel/deps/ImGui"
IncludeDir["glm"] = "Caramel/deps/glm"
IncludeDir["spdlog"] = "Caramel/deps/spdlog/include"

include "Caramel/deps/GLFW"
include "Caramel/deps/glad"
include "Caramel/deps/ImGui"

project "Caramel"
    location "Caramel"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/" .. outputdir .. "/%{prj.name}/obj")

    

    pchheader "clpch.h"
    pchsource "Caramel/src/clpch.cpp"

	files
	{
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
    }

    includedirs
	{
		"%{prj.name}/include",
        "%{prj.name}/deps",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.spdlog}",
        "%{prj.name}/deps/assimp/include",
        "%{prj.name}/deps/stb/include"
    }

    links
	{
        "GLFW",
        "glad",
        "ImGui",
        "opengl32.lib"
    }

	filter "system:windows"
        systemversion "latest"

		defines
		{
            "CL_PLATFORM_WINDOWS"
		}

    filter "configurations:Debug"
        defines "CL_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "CL_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "CL_DIST"
        optimize "On"

project "Game"
    location "Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/" .. outputdir .. "/%{prj.name}/obj")

	links
	{
        "Caramel",
        "Caramel/deps/assimp/win64/assimp.lib"
    }

	files
	{
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
        "%{prj.name}/include",
        "Caramel/include",
        "%{IncludeDir.spdlog}",
        "Caramel/deps",
        "%{IncludeDir.glm}"
    }

	filter "system:windows"
        systemversion "latest"

		defines
		{
            "CL_PLATFORM_WINDOWS"
		}

    filter "configurations:Debug"
        defines "CL_DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "CL_RELEASE"
        optimize "on"

    filter "configurations:Dist"
        defines "CL_DIST"
        optimize "on"
