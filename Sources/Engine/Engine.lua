SrcDir = os.getcwd()


function defineProject(projectType, useReflection)
	location (os.getcwd().."/Build")
	kind (projectType)
	language "C++"
	cppdialect "C++17"
	
	targetdir (os.getcwd().."/Binaries/")
	objdir (os.getcwd().."/Intermediates/".."%{cfg.longname}")  
	files { "**.cpp", "**.h" }
	includedirs("Sources/Private")
	includedirs("Sources/Public")
	includedirs("Sources")
	includedirs("\""..os.getcwd().."/Reflection/Public\"")
	includedirs("\""..os.getcwd().."/Reflection/Private\"")
	includedirs("\""..os.getcwd().."/Reflection\"")
	debugdir(SrcDir.."/../")
	
	if useReflection then
		prebuildcommands {"\""..SrcDir.."/Tools/ReflectionTool/Binaries/ReflectionTool.exe\" -m=\""..os.getcwd().."/Sources/\" -o=\""..os.getcwd().."/Reflection/\""}
	end
end

workspace "Engine"
	configurations { "Debug", "Release" }
	language "C++"
	startproject "ReflectionTool"
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
	
	group("Tools/ReflectionTool/")
		include("Tools/ReflectionTool")
	