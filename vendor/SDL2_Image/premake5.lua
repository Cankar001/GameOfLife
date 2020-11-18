project "SDL2_image"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
		{
		"src/**.h",
		"src/**.cpp"
		}
		
	includedirs
		{
		"include"
		}
		
	postbuildcommands
		{
		'{COPY} "lib/x64" "%{cfg.targetdir}/"'
		}
		
	filter "system:windows"
		systemversion "latest"

		filter "configurations:Debug"
			runtime "Debug"
			symbols "on"
			
			postbuildcommands
				{
				'{COPY} "lib/x64/SDL2_image.dll" "%{wks.location}/bin/Debug-x86_64-windows/GameOfLife/"',
				'{COPY} "lib/x64/libpng16-16.dll" "%{wks.location}/bin/Debug-x86_64-windows/GameOfLife/"'
				}

		filter "configurations:Release"
			runtime "Release"
			optimize "on"
			
			postbuildcommands
				{
				'{COPY} "lib/x64/SDL2_image.dll" "%{wks.location}/bin/Release-x86_64-windows/GameOfLife/"',
				'{COPY} "lib/x64/libpng16-16.dll" "%{wks.location}/bin/Release-x86_64-windows/GameOfLife/"'
				}

