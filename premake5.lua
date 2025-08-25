workspace "Aisite"
    architecture "x64"

    configurations
    { "Debug", "Release", "Dist"}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-${cfg.architecture}"

project "Engine"
    location "engine"
    kind "SharedLib"

    language "C++"

    targetdir ("lib/" .. outputdir .. "/%{prj.name}")
    objdir ("lib-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    include {
        "%{prj.name}/deps/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++23"
        staticruntime "On"
        systemversion "latest"

        defines { "_WIN32", "AISITE_EXPORTS" }
-- I use CMAKE , (*^▽^*)