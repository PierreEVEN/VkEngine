SrcDir = os.getcwd()


function defineProject(projectType, useReflection, projectName)
	location (os.getcwd().."/Build")
	kind (projectType)
	language "C++"
	cppdialect "C++17"
	
	targetdir (os.getcwd().."/Binaries/")
	objdir (os.getcwd().."/Intermediates/".."%{cfg.longname}")  
	files { "**.cpp", "**.h" }
	includedirs("Sources/Public")
	includedirs("Intermediates/Reflection/Public")
	debugdir(SrcDir.."/../../Ressources")
	
	if useReflection then
		includeModule("Tools/Reflection", "Reflection")
		dependson("Reflection")
		dependson("ReflectionTool")
		prebuildcommands {"\""..SrcDir.."/Tools/ReflectionTool/Binaries/ReflectionTool.exe\" -m=\""..os.getcwd().."/Sources/\" -o=\""..os.getcwd().."/Intermediates/Reflection/\""}
	end
end

function includeModule(path, moduleName)
	includedirs(SrcDir.."/"..path.."/Sources/Public")
	includedirs(SrcDir.."/"..path.."/Intermediates/Reflection/Public")
	links(SrcDir.."/"..path.."/Binaries/"..moduleName..".lib")
end

workspace "Engine"
	configurations { "Debug", "Release" }
	language "C++"
	startproject "Engine"
	architecture("x64")
	
	
	location (SrcDir.."/../../")
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
	filter { }
		
	group("Engine/Core")
		include("Core/Engine")
		include("Core/Utils")
	
	group("Engine/Tools")
		include("Tools/ReflectionTool")
		include("Tools/Reflection")
	