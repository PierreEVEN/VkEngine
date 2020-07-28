#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Rendering::Vulkan::ValidationLayer
{
	void LinkValidationLayers(VkInstanceCreateInfo& createInfos);

	void CreateValidationLayers();
	void DestroyValidationLayers();

	VkDebugUtilsMessengerEXT& GetDebugMessenger();
}