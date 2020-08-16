SrcDir = os.getcwd()
LibDir = os.getcwd().."/../ThirdParty/"

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

function includeVMA()
	includedirs(LibDir.."VulkanMemoryAllocator/src");
end

function includeObjLoad()
	includedirs(LibDir.."tinyobjloader");
end

function includeGlm()
	includedirs(LibDir.."glm")
end

function includeStb()
	includedirs(LibDir.."stb")
end

function includeRapidJson()
	includedirs(LibDir.."rapidjson/include/")
end

function includeVulkan()
	if type(os.getenv("vulkan_sdk")) ~= "" then
		includedirs(os.getenv("vulkan_sdk").."/Include")
		libdirs { os.getenv("vulkan_sdk").."/Lib/" }
		links { "vulkan-1.lib" }
	else
		print("failed to find vulkan sdk path 'vulkan_sdk'")
	end
end

function includeGaInput()
	includedirs(LibDir.."gainput/lib/include")
	links(LibDir.."/Libs/GaInput/Release/gainputstatic.lib")
end

function includeGlfw()
	includedirs(LibDir.."glfw/include")
	links(LibDir.."/Libs/glfw/Release/glfw3.lib")
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
		include("Core/Rendering")
	
	group("Engine/Tools")
		include("Tools/ReflectionTool")
		include("Tools/Reflection")
	