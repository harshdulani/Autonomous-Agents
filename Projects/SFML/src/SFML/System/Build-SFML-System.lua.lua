project "SFML-System"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    
    files 
    {     
        "../../../include/SFML/System/**.h",
        "../../../include/SFML/System/**.hpp",
        "**.cpp",
        "**.hpp"
    }

    includedirs 
    { 
        "../../../include",
        "../.."
    }

    links
    {
    }

    targetdir ("../../../../../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../../../../../Binaries/Intermediate/" .. OutputDir .. "/%{prj.name}")

    filter "action:vs*"
		location "../../../" --setting location of project files to SFML folder

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "on"