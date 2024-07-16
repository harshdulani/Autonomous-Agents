project "Autonomous-Agents"
	kind "ConsoleApp" 
	language "C++"
    staticruntime "on" -- The staticruntime keyword is used to specify whether you want to link the C++ runtime library statically or dynamically. If you set staticruntime "on", it will use the /MT or /MTd flags for Visual Studio, which means that the runtime library will be embedded into your executable or library2. If you set staticruntime "off", it will use the /MD or /MDd flags, which means that the runtime library will be loaded from a DLL at runtime2.
	cppdialect "C++20"

	pchheader "pch.h"
	pchsource "src/pch.cpp"

	targetdir ("../../Binaries/" .. OutputDir .. "/%{prj.name}")
	objdir ("../../Binaries/Intermediate/" .. OutputDir .. "/%{prj.name}")

	files { 
		"src/**.h",
		"src/**.hpp", 
		"src/**.cpp" 
	}

	includedirs {
		"src",
		"../SFML/include",
	}
	libdirs {
		"../SFML/libsVS",
	}

    links {
		"SFML-Graphics",
		"SFML-Window",
		"SFML-System",
		"SFML-Audio",
		"opengl32.lib", --Graphics
		"freetype.lib", --Graphics
		"winmm.lib", --System and Window
		"gdi32.lib", --Window
		"openal32.lib", --Audio
		"ogg.lib", --Audio
		"flac.lib", --Audio
		"vorbisenc.lib", --Audio
		"vorbisfile.lib", --Audio
		"vorbis.lib", --Audio
    }

	filter "action:vs*"
		location "./" --setting location of project files to the same folder this is script is located

	filter "system:windows"
		systemversion "latest" --Windows SDK version

	filter "configurations:Debug"
		symbols "On"
		defines { "DEBUG" }

	filter "configurations:Release"
		optimize "On"
		defines { "NDEBUG" }
		defines { "RELEASE" }

