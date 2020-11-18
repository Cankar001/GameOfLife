outputdir = "%{cfg.buildcfg}-%{cfg.architecture}-%{cfg.system}"

includedir                = {}
includedir["SDL2"]		  = "vendor/SDL2/include"
includedir["SDL2Image"]   = "vendor/SDL2_Image/include"
includedir["SDL2TTF"]     = "vendor/SDL2_TTF/include"

workspace "GameOfLife"
	configurations
		{
		"Release",
		"Debug"
		}
	
	architecture "x86_64"
	startproject "GameOfLife"
		
	group "Dependencies"
	include "vendor/SDL2"
	include "vendor/SDL2_Image"
	include "vendor/SDL2_TTF"
	group ""
	
	project "GameOfLife"
		location "GameOfLife"
		kind "WindowedApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"
		entrypoint "mainCRTStartup"
		defines "_CRT_SECURE_NO_WARNINGS"
		
		targetdir ("bin/" .. outputdir)
		objdir ("bin-int/" .. outputdir)
		
		files
			{
			"GameOfLife/src/**.h",
			"GameOfLife/src/**.cpp"
			}
			
		includedirs
			{
			"src",
			"%{includedir.SDL2}",
			"%{includedir.SDL2Image}",
			"%{includedir.SDL2TTF}"
			}
			
		postbuildcommands
			{
			'{COPY} "assets" "%{cfg.targetdir}/assets"'
			}
			
		links
			{
			"SDL2",
			"SDL2_image",
			"SDL2_ttf"
			}
			
		filter "system:linux"
			systemversion "latest"
			defines "PLATFORM_LINUX"
				
			filter "configurations:Debug"
				defines "_DEBUG"
				runtime "Debug"
				symbols "on"
			
			filter "configurations:Release"
				defines "_RELEASE"
				runtime "Release"
				optimize "on"
				
		filter "system:windows"
			systemversion "latest"
			defines "PLATFORM_WINDOWS"
				
			filter "configurations:Debug"
				defines "_DEBUG"
				runtime "Debug"
				symbols "on"
			
			filter "configurations:Release"
				defines "_RELEASE"
				runtime "Release"
				optimize "on"