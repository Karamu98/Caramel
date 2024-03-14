project "glad"
	location "glad"
	kind "StaticLib"
	language "C++"
    cppdialect "C++latest"
    staticruntime "on"

	targetdir ("../../Binaries/" .. outputdir .. "/Vendor/%{prj.name}")
    objdir ("../../Intermediate/" .. outputdir .. "/Vendor/%{prj.name}")

	files
	{
		"glad/include/glad/glad.h",
		"glad/include/KHR/khrplatform.h",
		"glad/src/glad.c"
	}

	includedirs
	{
		"glad/include"
	}
		
	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter {"system:windows", "configurations:Release"}
		buildoptions "/MT"