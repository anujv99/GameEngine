workspace "GameEngine"
	startproject "Sandbox"
	architecture "x64"
	
	configurations {
        "Debug",
        "Release",
    }
	
	outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"
	
	IncludeDirs = {}
	
	project "Engine"
		location "engine"
		kind "StaticLib"
		language "C++"
        cppdialect "C++17"
		staticruntime "on"
		
		targetdir ("bin/" .. outputDir .. "%{prj.name}")
        objdir ("bin-int/" .. outputDir .. "%{prj.name}")
		
		files {
            "engine/src/**.*",
        }
		
		links {
			
		}
		
		includedirs {
			"engine/src",
		}
		
		defines {
			"PREV_GAME_ENGINE", --this is used to determine the engine. Don't remove this and/or define this in any other application which use this engine
			"ENGINE_LOGGING", --define this to enable logging
			"ENGINE_ASSERT", --define this to enable asserts
			--"ENGINE_UNICODE", --define this to enable unicode for fonts
		}
		
		undefines {
			"UNICODE",
			"_UNICODE",
		}
		
		pchsource "engine/src/enginepch.cpp"
        pchheader "enginepch.h"
		
		filter "system:windows"
			defines {"ENGINE_WINDOWS"}
		
		filter "configurations:Debug"
            defines {"ENGINE_DEBUG", "DEBUG", "_DEBUG"}
            runtime "Debug"
            symbols "On"

        filter "configurations:Release"
            defines {"ENGINE_RELEASE", "NDEBUG", "_NDEBUG"}
            runtime "Release"
	        optimize "On"
	
	project "Sandbox"
		location "sandbox"
		kind "ConsoleApp"
		language "C++"
        cppdialect "C++17"
		staticruntime "on"
		
		targetdir ("bin/" .. outputDir .. "%{prj.name}")
        objdir ("bin-int/" .. outputDir .. "%{prj.name}")
		
		files {
            "sandbox/src/**.*",
        }
		
		links {
			"Engine"
		}
		
		includedirs {
			"engine/includes",
			"sandbox/src",
		}
		
		pchsource "sandbox/src/sandboxpch.cpp"
        pchheader "sandboxpch.h"
		
		filter "configurations:Debug"
            runtime "Debug"
            symbols "On"

        filter "configurations:Release"
            runtime "Release"
	        optimize "On"