#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Rendering::Vulkan::LogDevice
{
	void CreateLogicalDevice();
	void DestroyLogicalDevice();

	VkDevice& GetLogicalDevice();
	VkQueue& GetGraphicQueues();
	VkQueue& GetPresentQueue();
}