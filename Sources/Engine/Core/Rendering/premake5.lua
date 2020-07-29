project "Rendering"
	defineProject("StaticLib", true, "Rendering")
	dependson("Reflection")
	dependson("Utils")
	includeModule("Tools/Reflection", "Reflection")
	includeModule("Core/Utils", "Utils")
	includeVulkan()
	includeGlfw()