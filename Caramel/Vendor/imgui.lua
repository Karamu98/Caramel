project "ImGui"
	location "imgui"
	kind "StaticLib"
	language "C++"
    cppdialect "C++latest"
    staticruntime "on"

	include "glfw"

	targetdir ("../../Binaries/" .. outputdir .. "/Vendor/%{prj.name}")
    objdir ("../../Intermediate/" .. outputdir .. "/Vendor/%{prj.name}")

	files
	{
		"imgui/imgui.cpp",
		"imgui/imgui_demo.cpp",
		"imgui/imgui_draw.cpp",
		"imgui/imgui_tables.cpp",
		"imgui/imgui_widgets.cpp",

		"imgui/imconfig.h",
		"imgui/imgui.h",
		"imgui/imgui_internal.h",
		"imgui/imstb_rectpack.h",
		"imgui/imstb_textedit.h",
		"imgui/imstb_truetype.h",

		"imgui/backends/imgui_impl_glfw.h",
		"imgui/backends/imgui_impl_glfw.cpp",
		"imgui/backends/imgui_impl_dx12.h",
		"imgui/backends/imgui_impl_dx12.cpp",
		"imgui/backends/imgui_impl_opengl3.h",
		"imgui/backends/imgui_impl_opengl3.cpp",
	}

	includedirs
	{
		"imgui/",
		"glfw/include"
	}

	links
	{
		"GLFW"
	}

