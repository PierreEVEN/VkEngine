project "Engine"
	defineProject("ConsoleApp", true, "Engine")
	dependson("Reflection")
	dependson("Utils")
	includeModule("Tools/Reflection", "Reflection")
	includeModule("Core/Utils", "Utils")