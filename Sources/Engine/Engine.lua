SrcDir = os.getcwd()

function defineProject(projectType)
	location (os.getcwd().."/Build")
	kind (projectType)
	language "C++"
	cppdialect "C++17"
	
	targetdir (os.getcwd().."/Binaries/")
	objdir (os.getcwd().."/Intermediates/".."%{cfg.longname}")  
	files { "**.cpp", "**.h" }
	includedirs("Sources")
	debugdir(SrcDir.."/../")
end

workspace "Engine"
	configurations { "Debug", "Release" }
	language "C++"
	startproject "Reflection"
	architecture "x86_64"
	
	location (SrcDir.."/../../")
		filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
	filter { }
		
	group("Core/Reflection/")
		include("Core/Reflection")
	
	