project "Glad"
    kind "StaticLib"
    language "C"
    staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/" .. outputdir .. "/%{prj.name}/obj")

	files
	{
        "include/glad/glad.h",
        "src/glad.c"
    }

    includedirs
	{
        "include"
    }
    
	filter "system:windows"
        systemversion "latest"
