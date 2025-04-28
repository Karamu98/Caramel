workspace "Caramel"
    architecture "x64"
    startproject "Game"

    configurations
    {
        "Debug",
        "Release",
        "Dist",
    }

    platforms
    {
        "WindowsEditor64",
        "WindowsGame64"
    }
    
    flags
	{
		"MultiProcessorCompile"
	}

	filter "configurations:Debug"
		defines {"CL_DEBUG"}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines {"CL_RELEASE"}
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines {"CL_DIST"}
		runtime "Release"
		optimize "on"
		flags { "linktimeoptimization" }


	filter "system:WindowsEditor64"
		architecture "x64"
		systemversion "latest"

		defines
		{
			"CL_PLATFORM_WINDOWS",
            "CL_PLATFORM_EDITOR"
		}

    filter "system:WindowsGame64"
		architecture "x64"
		systemversion "latest"

		defines
		{
			"CL_PLATFORM_WINDOWS",
            "CL_PLATFORM_GAME"
		}

		
local Core = require("_Premake\\PremakeBuilder")


local discoverAllParams = Core.Project.DiscoveryParams()
discoverAllParams.RootDirectory = Core.GetWorkingDirectory() -- hope this works lol, can default if so
discoverAllParams.RootProjects = 
{
	"CaramelCore",
	"Caramel",
	"Game"
}


local C_OutputDirectoryCommon = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

local rootData = 
{
	-- All projects will have these settings
	ConstantPremakeExpression =
	{
		language=("C++"),
		cppdialect= ("C++latest"),
		staticruntime= ("on"),
	},
	
	-- Projects will have these as default, can override for upstream
	OptionalPremakeExpression = 
	{
		kind= "StaticLib",
		targetdir= ("%{prj.location}/Binaries/" .. C_OutputDirectoryCommon .. "/%{prj.name}"),
		objdir= ("%{prj.location}/Intermediate/" .. C_OutputDirectoryCommon .. "/%{prj.name}"),
		files = 
		{
			"%{prj.location}/Source/**.h", 
			"%{prj.location}/Source/**.cpp",
			Core.Project.GetProjectLuaPathWithExtension("%{prj.location}", "%{prj.name}")
		}
	},
}

local discovered = Core.Project.DiscoverAll(discoverAllParams, rootData)
Core.Project.Evaluate(discovered)