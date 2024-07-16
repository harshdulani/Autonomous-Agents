project "SFML-Audio"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    
    defines { "FLAC__NO_DLL"} -- This is needed to avoid linking to the FLAC DLL

    files 
    {     
        "../../../include/SFML/Audio/**.h",
        "../../../include/SFML/Audio/**.hpp",
        "**.cpp",
        "**.hpp"
    }

    includedirs 
    { 
        "../..",
        "../../../include",
        "../../../extlibs/headers",
        "../../../extlibs/headers/AL",
        "../../../extlibs/headers/minimp3",
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