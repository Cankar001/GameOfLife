project "SDL2"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir)
	objdir ("bin-int/" .. outputdir)
	
	files
		{
		"src/**.h",
		"src/**.cpp"
		}
		
	includedirs
		{
		"include"
		}
		
	filter "system:windows"
		systemversion "latest"

		filter "configurations:Debug"
			runtime "Debug"
			symbols "on"
			
			postbuildcommands
				{
				'{COPY} "lib/x64" "%{cfg.targetdir}/"',
				'{COPY} "lib/x64/SDL2.dll" "%{wks.location}/bin/Debug-x86_64-windows/"'
				}
					
		filter "configurations:Release"
			runtime "Release"
			optimize "on"
			
			postbuildcommands
				{
				'{COPY} "lib/x64" "%{cfg.targetdir}/"',
				'{COPY} "lib/x64/SDL2.dll" "%{wks.location}/bin/Release-x86_64-windows/"'
				}
				
	filter "system:linux"
		systemversion "latest"
		
		