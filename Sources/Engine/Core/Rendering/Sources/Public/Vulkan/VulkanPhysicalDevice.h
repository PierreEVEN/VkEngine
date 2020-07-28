#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Rendering::Vulkan::PhysDevice
{
	void PickPhysicalDevice();

	VkPhysicalDevice& GetPhysicalDevice();
}