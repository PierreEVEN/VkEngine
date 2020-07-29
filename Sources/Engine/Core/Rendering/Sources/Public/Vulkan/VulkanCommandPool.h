#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Rendering::Vulkan::CommandPool
{
	void CreateCommandPool();
	void DestroyCommandPool();

	VkCommandPool& GetCommandPool();
}