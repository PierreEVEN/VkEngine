#pragma once

#include "Constants.h"

namespace Rendering::ValidationLayers
{
	void LinkValidationLayers(VkInstanceCreateInfo& createInfos);

	VkDebugUtilsMessengerCreateInfoEXT PopulateDebugMessengerCreateInfo();
	void CreateValidationLayers();
	void DestroyValidationLayers();
}