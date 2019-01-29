workspace "Caramel"
    architecture "x64

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Caramel"
    location "GameEngine"
    kind "SharedLib"
    language "C++"

    targerdir ("bin/" .. outputDir .. "/%{prj.name"})
    objdir ("bin-int/" .. outputDir .. "/%{prj.name"})

    files
    {
        "%{prj.name}/source/**.cpp",
        "%{prj.name}/include/**.h"
    }

    include
    {
        "%{prj.name}/deps/gl_core_4_4",
        "%{prj.name}/deps/stb",
        "%{prj.name}/deps/glm",
        "%{prj.name}/deps/imgui/include",
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/../Framework/framework/include"
    }