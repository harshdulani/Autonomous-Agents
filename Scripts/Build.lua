workspace "Autonomous-Agents" --Solution name
	architecture "x64"

    configurations { 
		"Debug", 
		"Release" 
	}

	startproject "Autonomous-Agents"
	
	defines { 
		"SFML_STATIC" --builds the static version of SFML
	}

	filter "action:vs*"
        location "../VS2022" -- Location of the WORKSPACE solution file, but individual projects can have their own location
	filter "system:windows"
		systemversion "latest"
	
OutputDir = "%{cfg.buildcfg}-%{cfg.system}"

include "../Projects/SFML/src/SFML/Graphics/Build-SFML-Graphics.lua"
include "../Projects/SFML/src/SFML/System/Build-SFML-System.lua"
include "../Projects/SFML/src/SFML/Window/Build-SFML-Window.lua"
include "../Projects/SFML/src/SFML/Audio/Build-SFML-Audio.lua"
include "../Projects/Autonomous-Agents/Build-Autonomous-Agents.lua"